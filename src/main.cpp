#define ENABLE_USER_AUTH
#define ENABLE_DATABASE
#define ENABLE_FIRESTORE

#include <Arduino.h>
#include "services/sensorservice.hpp"
#include "services/wifiservice.hpp"
#include "services/timeservice.hpp"
#include "services/firebaseservice.hpp"

SensorService sensorService;
WifiService wifiService;
TimeService timeService;
FirebaseService firebaseService(wifiService, timeService);

void OnEnter()
{
  Serial.println("Person Enter");
  firebaseService.appendData(true);
}

void OnExit()
{
  Serial.println("Person Exit");
  firebaseService.appendData(false);
}


void setup()
{
  Serial.begin(115200);
  sensorService.startup();
  wifiService.startup();
  timeService.startup();
  firebaseService.startup();

  sensorService.onTrigger.on("enter", OnEnter);
  sensorService.onTrigger.on("exit", OnExit);
}

void loop()
{
  sensorService.loop();
  firebaseService.loop();
  timeService.loop();
}
