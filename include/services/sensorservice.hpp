#ifndef SENSOR_HPP
#define SENSOR_HPP
#include "EventEmitter.hpp"

class SensorService {
public:
    SensorService();

    void startup();
    void loop();

    EventEmitter<> onTrigger;

private:
    unsigned long _lastReadingTime = 0;
};

#endif