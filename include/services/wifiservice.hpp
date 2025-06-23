#ifndef WIFI_HPP
#define WIFI_HPP
#include "EventEmitter.hpp"
#include "Arduino.h"
#include <WiFi.h>

class WifiService {
public:
    void startup();
    void loop();

private:

};

#endif