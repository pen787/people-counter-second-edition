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
  // _Docs.createDocument(_aClient, Firestore::Parent(PROJECT_ID), documentPath, DocumentMask(), doc, processData, "createDocumentTask");
  _Docs.createDocument(_aClient, Firestore::Parent(PROJECT_ID), documentPath, DocumentMask(), doc);
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