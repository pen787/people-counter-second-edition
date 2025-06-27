#include "services/firebaseservice.hpp"

GUID guid;

FirebaseService::FirebaseService(WifiService &wifis, TimeService &tser)
    : _wifiservice(wifis),
      _timeservice(tser),
      _ssl_client(wifis.getSSL_Client()),
      _aClient(*_ssl_client),
      _auth{WEBAPI_KEY, USER_EMAIL, USER_PASS, 3000}
{
}

void processData(AsyncResult &aResult)
{
  // Exits when no result available when calling from the loop.
  if (!aResult.isResult())
    return;

  if (aResult.isEvent())
  {
    Firebase.printf("Event task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.eventLog().message().c_str(), aResult.eventLog().code());
  }

  if (aResult.isDebug())
  {
    Firebase.printf("Debug task: %s, msg: %s\n", aResult.uid().c_str(), aResult.debug().c_str());
  }

  if (aResult.isError())
  {
    Firebase.printf("Error task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.error().message().c_str(), aResult.error().code());
  }

  if (aResult.available())
  {
    Firebase.printf("task: %s, payload: %s\n", aResult.uid().c_str(), aResult.c_str());
  }
}

void FirebaseService::create_document_async(Document<Values::Value> &doc, const String &documentPath)
{
  Serial.println("Creating a document... ");

  // Async call with callback function.
  _Docs.createDocument(_aClient, Firestore::Parent(PROJECT_ID), documentPath, DocumentMask(), doc, processData, "createDocumentTask");
  // _Docs.createDocument(_aClient, Firestore::Parent(PROJECT_ID), documentPath, DocumentMask(), doc);
}

void FirebaseService::appendData(bool enter)
{
  String documentPath = "entries/";
  guid.generate();
  char *guidStr = guid.toCharArray();
  if (guidStr != nullptr)
  {
    documentPath += guidStr;
  }
  else
  {
    Serial.println("Failed to get UUID");
  }

  Values::TimestampValue createAtValue(_timeservice.getCurrentISO());

  // boolean
  Values::BooleanValue bolV(enter);

  Document<Values::Value> doc("createAt", Values::Value(createAtValue));
  doc.add("enter", Values::Value(bolV));

  create_document_async(doc, documentPath);
  Serial.println("Done create document.");
}

void FirebaseService::setRealtimeData(int c, int d, int w, int m)
{
  object_t json, obj1, obj2, obj3, obj4, obj5;
  JsonWriter writer;

  writer.create(obj1, "current", c);
  writer.create(obj2, "day", d);
  writer.create(obj3, "week", w);
  writer.create(obj4, "month", m);

  writer.join(obj5, 4 /* no. of object_t (s) to join */, obj1, obj2, obj3, obj4);
  writer.create(json, "data", obj5);

  // To print object_t
  // Serial.println(json);
  Serial.println("Setting the JSON value... ");
  _RTDatabase.set<object_t>(_aClient, "/entries/", json, processData, "setJsonTask");
  Serial.println("Done setting data!");
}

FirebaseApp& FirebaseService::getApp() {
  return _app;
}

void FirebaseService::updateRealtimeData(String d, int a)
{
  object_t json;
  JsonWriter writer;
  writer.create(json, d, a);

  // String atLoc = "/entries/data";
  // atLoc += d;

  _RTDatabase.update(_aClient, "/entries/data", json, processData, "updateTask");
  Serial.println("Done update data!");
}

void FirebaseService::startup()
{
  Firebase.printf("Firebase Client v%s\n", FIREBASE_CLIENT_VERSION);

  Serial.println("Initializing app...");

  initializeApp(_aClient, _app, getAuth(_auth), 120 * 1000, processData);

  _app.getApp<RealtimeDatabase>(_RTDatabase);
  _app.getApp<Firestore::Documents>(_Docs);
  _RTDatabase.url(DATABASE_URL);
}

void FirebaseService::loop()
{
  _app.loop();
}