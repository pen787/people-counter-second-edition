#ifndef WIFI_HPP
#define WIFI_HPP
#include "EventEmitter.hpp"
#include "Arduino.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>


class WifiService {
public:
void startup();
void loop();
WiFiClientSecure* getSSL_Client();

private:
    WiFiClientSecure _ssl_client;

};

#endif