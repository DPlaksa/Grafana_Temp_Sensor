from dotenv import load_dotenv
import os

load_dotenv()

ssid = os.getenv("WIFI_SSID")
password = os.getenv("WIFI_PASSWORD")
influxdbtoken = os.getenv("INFLUXDB_TOKEN")
influxdburl = os.getenv("INFLUXDB_URL")
influxdborg = os.getenv("INFLUXDB_ORG")
influxdbbucket = os.getenv("INFLUXDB_BUCKET")



Import("env")

if ssid is None or password is None or influxdbtoken is None:
    print("Error: WIFI_SSID, WIFI_PASSWORD or INFLUXDB_TOKEN not set in .env")
else:
    env.Append(
        BUILD_FLAGS=[
            f'-DWIFI_SSID=\\"{ssid}\\"',
            f'-DWIFI_PASSWORD=\\"{password}\\"',
            f'-DINFLUXDB_TOKEN=\\"{influxdbtoken}\\"',
            f'-DINFLUXDB_URL=\\"{influxdburl}\\"',
            f'-DINFLUXDB_ORG=\\"{influxdborg}\\"',
            f'-DINFLUXDB_BUCKET=\\"{influxdbbucket}\\"',
        ]
    )
