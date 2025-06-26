#ifndef FIREBASE_HPP
#define FIREBASE_HPP

#define ENABLE_USER_AUTH
#define ENABLE_DATABASE
#define ENABLE_FIRESTORE

#include "EventEmitter.hpp"
#include "Arduino.h"
#include "FirebaseClient.h"
#include "secrets.h"
#include "wifiservice.hpp"
#include "timeservice.hpp"
#include "UUID.h"

using AsyncClient = AsyncClientClass;

class FirebaseService {
public:
    FirebaseService(WifiService &wifis, TimeService &tser);

    void startup();
    void loop();
    
    void appendData(bool enter);
    void setRealtimeData(int c, int d, int w, int m);
    void updateRealtimeData(String d, int a);
private:
    WifiService &_wifiservice;
    TimeService &_timeservice;
    WiFiClientSecure *_ssl_client;
    FirebaseApp _app;
    AsyncClient _aClient;
    UserAuth _auth;
    RealtimeDatabase _RTDatabase;
    Firestore::Documents _Docs;

    void create_document_async(Document<Values::Value> &doc, const String &documentPath);
};

#endif