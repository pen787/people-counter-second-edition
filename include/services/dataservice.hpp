#ifndef DATA_HPP
#define DATA_HPP

#include "EventEmitter.hpp"
#include "Arduino.h"

enum DATATYPE {
  current=1,
  day,
  week,
  month
};

class DataService {
public:
    DataService();

    void startup();
    void loop();
    
    void setData(DATATYPE t, int a);
    void addData(DATATYPE t, int a);
    int getData(DATATYPE t);

    void printAll();
private:
  int _currently = 0;
  int _daliy = 0;
  int _weekly = 0;
  int _monthly = 0;
};

#endif