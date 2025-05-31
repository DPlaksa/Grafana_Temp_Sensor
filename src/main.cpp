#include <WiFiMulti.h>
WiFiMulti wifiMulti;

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

#define TZ_INFO "UTC2"

#define LIGHT_SENSOR_PIN 34
#define DEVICE "ESP32"

// === Включить ЭМУЛЯЦИЮ данных ===
// закомментируй следующую строку, чтобы использовать реальный датчик
#define EMULATE_SENSOR_DATA

InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);
Point sensor("light");

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);

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
  int lightValue;

#ifdef EMULATE_SENSOR_DATA
  // === Эмуляция значения ===
  static int emulatedLightValue = 200;           // начальное значение
  static int increment = 100;                     // шаг нарастания
  static unsigned long lastUpdate = 0;
  unsigned long currentMillis = millis();

  if (currentMillis - lastUpdate >= 500) {       // обновляем каждые 100 мс
    emulatedLightValue += increment;
    if (emulatedLightValue >= 3500) {
      emulatedLightValue = 500;                  // перезапуск цикла
    }
    lastUpdate = currentMillis;
  }

  lightValue = emulatedLightValue;
  Serial.print("[EMULATION MODE] Light value: ");
  Serial.println(lightValue);
#else
  // === Реальное чтение ===
  lightValue = analogRead(LIGHT_SENSOR_PIN);
#endif

  float lightPercent = (lightValue / 4095.0) * 100.0;

  sensor.clearFields();
  sensor.addField("light_raw", lightValue);
  sensor.addField("light_percent", lightPercent);

  if (!client.writePoint(sensor)) {
    Serial.print("Write failed: ");
    Serial.println(client.getLastErrorMessage());
  } else {
#ifdef EMULATE_SENSOR_DATA
    Serial.print("Emulated Light: ");
#else
    Serial.print("Light: ");
#endif
    Serial.print(lightPercent);
    Serial.println(" %");
  }

  delay(1000);
}
