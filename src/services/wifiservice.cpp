#include "services/wifiservice.hpp"
#include "secrets.h"

const char* ssid     = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

const char* ntpServer = "pool.ntp.org";
// Thailand gmt +7
const long  gmtOffset_sec = 25200;
const int   daylightOffset_sec = 0;

void WifiService::startup() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.print("RRSI: ");
  Serial.println(WiFi.RSSI());

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void WifiService::loop() {

}