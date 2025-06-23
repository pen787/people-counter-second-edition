#ifndef TIME_HPP
#define TIME_HPP
#include "EventEmitter.hpp"
#include "Arduino.h"
#include "time.h"

class TimeService {
public:

  void startup();
  void loop();

  EventEmitter<> onTime;

private:
  tm *_lasttime;
  int _lastDay=-1;
  int _lastMonth=-1;
  int _lastDayOfWeek=-1;
  ulong _lastupdate;
  ulong _updateTime=60*1000;
};

#endif