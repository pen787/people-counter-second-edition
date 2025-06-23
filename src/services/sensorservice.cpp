#include "services/sensorservice.hpp"
#include "Arduino.h"

#define FIRSTSENSOR 2
#define SECONDSENSOR 4

SensorService::SensorService() {}

void SensorService::startup() {
  pinMode(FIRSTSENSOR, INPUT);
  pinMode(SECONDSENSOR, INPUT);
}

void SensorService::loop() {
  bool first = digitalRead(FIRSTSENSOR);
  bool second = digitalRead(SECONDSENSOR);

  Serial.printf("%d, %d\n", first, second);
}