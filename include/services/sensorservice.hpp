#ifndef SENSOR_HPP
#define SENSOR_HPP
#include "EventEmitter.hpp"
#include "Arduino.h"

class SensorService {
public:
    void startup();
    void loop();

    EventEmitter<> onTrigger;

private:
    unsigned long _lastReadingTime = 0;
    bool _LastAStage = LOW;
    bool _LastBStage = LOW;

    unsigned long _timeA = 0;
    unsigned long _timeB = 0;
};

#endif