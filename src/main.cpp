#include <Arduino.h>
#include "services/sensorservice.hpp"

SensorService sensorService;

void setup() {
  Serial.begin(115200);
  sensorService.startup();
}

void loop() {
  sensorService.loop();

  delay(100);
}
