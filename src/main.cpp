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

enum class FirebaseCommandType
{
  Update,
  Append,
  Set
};

struct FirebaseCommand
{
  FirebaseCommandType type;
  String path;  // e.g. "current", "day", etc
  int value;    // for update
  bool isEnter; // for appendData
};

QueueHandle_t firebaseQueue;

void firebaseTask(void *pvParameters)
{
  FirebaseApp app = firebaseService.getApp();
  FirebaseCommand cmd;
  while (true)
  {
    // app.loop(); // Keep auth alive
    if (xQueueReceive(firebaseQueue, &cmd, portMAX_DELAY) == pdPASS)
    {
      if (app.isInitialized())
      {
        if (cmd.type == FirebaseCommandType::Update)
        {
          firebaseService.updateRealtimeData(cmd.path, cmd.value);
        }
        else if (cmd.type == FirebaseCommandType::Append)
        {
          firebaseService.appendData(cmd.isEnter);
        }
        else if (cmd.type == FirebaseCommandType::Set)
        {
          firebaseService.setRealtimeData(cmd.value, cmd.value, cmd.value, cmd.value);
        }
      } else {
        Serial.println("App not ready yet!");
      }
    }
    // vTaskDelay(10 / portTICK_PERIOD_MS); // Avoid tight loop
  }
}

void OnEnter()
{
  Serial.println("Person Enter");
  dataService.addData(DATATYPE::current, 1);
  dataService.addData(DATATYPE::day, 1);
  dataService.addData(DATATYPE::week, 1);
  dataService.addData(DATATYPE::month, 1);

  dataService.printAll();

  FirebaseCommand cmd;

  cmd = {FirebaseCommandType::Update, "current", dataService.getData(DATATYPE::current), false};
  xQueueSend(firebaseQueue, &cmd, 0);

  cmd = {FirebaseCommandType::Update, "day", dataService.getData(DATATYPE::day), false};
  xQueueSend(firebaseQueue, &cmd, 0);

  cmd = {FirebaseCommandType::Update, "week", dataService.getData(DATATYPE::week), false};
  xQueueSend(firebaseQueue, &cmd, 0);

  cmd = {FirebaseCommandType::Update, "month", dataService.getData(DATATYPE::month), false};
  xQueueSend(firebaseQueue, &cmd, 0);

  cmd = {FirebaseCommandType::Append, "", 0, true};
  xQueueSend(firebaseQueue, &cmd, 0);
}

void OnExit()
{
  Serial.println("Person Exit");
  dataService.addData(DATATYPE::current, -1);
  firebaseService.updateRealtimeData("current", dataService.getData(DATATYPE::current));

  dataService.printAll();

  FirebaseCommand cmd;

  cmd = {FirebaseCommandType::Update, "current", dataService.getData(DATATYPE::current), false};
  xQueueSend(firebaseQueue, &cmd, 0);

  cmd = {FirebaseCommandType::Append, "", 0, false};
  xQueueSend(firebaseQueue, &cmd, 0);
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

  firebaseQueue = xQueueCreate(60, sizeof(FirebaseCommand));
  xTaskCreatePinnedToCore(firebaseTask, "FirebaseTask", 8192, NULL, 1, NULL, 1);

  FirebaseCommand cmd;
  cmd = {FirebaseCommandType::Set, "", 0, false};
  xQueueSend(firebaseQueue, &cmd, 0);
}

void loop()
{
  sensorService.loop();
  firebaseService.loop();
  timeService.loop();
}
