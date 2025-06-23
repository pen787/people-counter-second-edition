#ifndef DATA_HPP
#define DATA_HPP
#include "EventEmitter.hpp"
#include "Arduino.h"
#include <map>

class DataService {
public:
void startup();
void loop();
void setValue(String type,int amount);
void getValue(String type);

private:
std::map<String, int> data;
};

#endif