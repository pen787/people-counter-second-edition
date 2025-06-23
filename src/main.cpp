#include <Arduino.h>
#include "services/sensorservice.hpp"
#include "services/wifiservice.hpp"
#include "services/timeservice.hpp"

SensorService sensorService;
WifiService wifiService;
TimeService timeService;

void setup() {
  Serial.begin(115200);
  sensorService.startup();
  wifiService.startup();
  timeService.startup();
}

void loop() {
  timeService.loop();
  sensorService.loop();
  // delay(100);
}
