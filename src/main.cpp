#define ENABLE_USER_AUTH
#define ENABLE_DATABASE
#define ENABLE_FIRESTORE

#include <Arduino.h>
#include "services/sensorservice.hpp"
#include "services/wifiservice.hpp"
#include "services/timeservice.hpp"
#include "services/firebaseservice.hpp"
#include "services/dataservice.hpp"

SensorService sensorService;
DataService dataService;
WifiService wifiService;
TimeService timeService;
FirebaseService firebaseService(wifiService, timeService);

void OnEnter()
{
  Serial.println("Person Enter");
  dataService.addData(DATATYPE::current, 1);
  dataService.addData(DATATYPE::day, 1);
  dataService.addData(DATATYPE::week, 1);
  dataService.addData(DATATYPE::month, 1);

  dataService.printAll();

  firebaseService.updateRealtimeData("current", dataService.getData(DATATYPE::current));
  // firebaseService.updateRealtimeData("day", dataService.getData(DATATYPE::day));
  // firebaseService.updateRealtimeData("week", dataService.getData(DATATYPE::week));
  // firebaseService.updateRealtimeData("month", dataService.getData(DATATYPE::month));

  firebaseService.appendData(true);
}

void OnExit()
{
  Serial.println("Person Exit");
  dataService.addData(DATATYPE::current, -1);
  firebaseService.updateRealtimeData("current", dataService.getData(DATATYPE::current));

  dataService.printAll();

  firebaseService.appendData(false);
}

void resetDay()
{
  enum DATATYPE t = day;
  dataService.setData(t, 0);
}

void resetWeek()
{
  enum DATATYPE t = week;
  dataService.setData(t, 0);
}

void resetMonth()
{
  enum DATATYPE t = month;
  dataService.setData(t, 0);
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

  timeService.onTime.on("day", resetDay);
  timeService.onTime.on("week", resetWeek);
  timeService.onTime.on("month", resetMonth);

  firebaseService.setRealtimeData(0, 0, 0, 0);
}

void loop()
{
  sensorService.loop();
  firebaseService.loop();
  timeService.loop();
}
