#include <WiFiMulti.h>
WiFiMulti wifiMulti;

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

#define TZ_INFO "UTC2"
#define DEVICE "ESP32"

// Настройки для измерения температуры
#define THERMISTOR_PIN 39      // VN = GPIO39
#define T0 28.2               // Температура при калибровке
#define VF0 373.60           // Напряжение на диоде при T0 в милливольтах
#define K 0.4545454            // 1 / 2.2mV/°C

InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);
Point sensor("temperature");

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);  // 12-битный АЦП

  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to WiFi");
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }
  Serial.println(" connected.");

  sensor.addTag("device", DEVICE);
  timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");

  if (client.validateConnection()) {
    Serial.println("Connected to InfluxDB");
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
  }
}

void loop() {
  float dtemp_avg = 0;
  float vf_raw_avg = 0;

  // Измерим 1024 раза и усредним
  for (int i = 0; i < 1024; i++) {
    float raw = analogRead(THERMISTOR_PIN);
    float vf = raw * (3300.0 / 4095.0);  // перевод в милливольты
    float dtemp = (vf - VF0) * K;
    dtemp_avg += dtemp;
    vf_raw_avg += vf;
  }

  float temperatureC = T0 - (dtemp_avg / 1024.0);
  float vf_avg = vf_raw_avg / 1024.0;

  // Отладка
  Serial.print("Temperature (°C): ");
  Serial.println(temperatureC);
  Serial.print("Vf avg (mV): ");
  Serial.println(vf_avg);

  // Отправка в InfluxDB
  sensor.clearFields();
  sensor.addField("temperature_c", temperatureC);
  sensor.addField("temperature_raw_mv", vf_avg);

  if (!client.writePoint(sensor)) {
    Serial.print("Write failed: ");
    Serial.println(client.getLastErrorMessage());
  }

  delay(1000);
}
