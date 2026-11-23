# ESP32 Temperature Monitor with Grafana Dashboard

[![IoT](https://img.shields.io/badge/Tag-IoT-blue)](https://github.com/your-repo)
[![Language: C++](https://img.shields.io/badge/Language-C++-green.svg)](https://github.com/your-repo)
[![Python](https://img.shields.io/badge/Backend-Python3-yellow.svg)](https://www.python.org/)
[![Grafana](https://img.shields.io/badge/Visualization-Grafana-orange)](https://grafana.com/)
[![Driver: CP210x](https://img.shields.io/badge/Driver-CP210x-lightgrey)](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)






A IoT solution that reads temperature from an **ESP32** using 1N4148 as sensor and pushes data to a **Grafana dashboard** for real-time monitoring. Built with **C++** on the hardware side and a **Python** with `.env` configuration support.

![Alt Text](embedded_schematics.jpg)
![Alt Text](sensor_real_look_2.jpeg)
![Alt Text](sensor_real_look.jpeg)


**Indication:**
- **Green LED blinking** – Wi-Fi connected successfully  
- **Red LED blinking** – Sensor threshold reached (hardcoded in C++)

---

## Features

- Real-time temperature monitoring via ESP32-WROOM
- WIFI
- Environment-based configuration using `python-dotenv`
- Grafana dashboard for data visualization

![Alt Text](dashboard.jpeg)


---

### Software Requirements


- [Platform IO](https://platformio.org/) - needed for flashing the ESP32 and auto-installing required C++ libraries(list of libs described in platformio.ini file)

- [Python 3.7+] 
+ Install dependencies:
```bash
pip install -r requirements.txt
```

- [CP210x driver](https://www.silabs.com/developer-tools/usb-to-uart-bridge-vcp-drivers) for USB communication with ESP32

- [Grafana](https://grafana.com) check related repo folder: "import_to_your grafana"
- [Influx DB](https://www.influxdata.com/) - provide your fluxDB pool creditionals in grafana datasources, your pool name must be "esp" or just replace "esp" with yours in all dashboard queries and in fluxDB datasource in Grafana. You can check queries and FluxDB connection setting in containerized example:

## Example of Grafana Configuration

### Docker Image

**Image:**  
[dplaksa/grafana_custom](https://hub.docker.com/r/dplaksa/grafana_custom)

### Run Command

```bash
docker run -d -p 8080:3000 dplaksa/grafana_custom:latest

```

### Environment variables:
```bash
Before flashing your ESP32, create a .env file in the repo root folder, add your secrets using the following format:

WIFI_SSID=YOUR_WIFI_SSID (Isolate spaces and special symbols with / or "" )
WIFI_PASSWORD=YOUR_PASS(Isolate spaces and special symbols with / or "" )
INFLUXDB_TOKEN="YOUR TOKEN"
INFLUXDB_URL="YOUR_URL"
INFLUXDB_ORG="YOUR_ORG"
INFLUXDB_BUCKET="YOUR_BUCKET"