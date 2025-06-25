#include "services/dataservice.hpp"

DataService::DataService() {}

void DataService::startup() {}
void DataService::loop() {}

int DataService::getData(DATATYPE t) {
  switch (t)
  {
  case 1:
    return _currently;
    break;
  case 2:
    return _daliy;
    break;
  case 3:
    return _weekly;
    break;
  case 4:
    return _monthly;
    break;
  default:
    break;
  }

  return -1;
}

void DataService::setData(DATATYPE t, int a) {
  switch (t)
  {
  case 1:
    _currently = a;
    break;
  case 2:
    _daliy = a;
    break;
  case 3:
    _weekly = a;
    break;
  case 4:
    _monthly = a;
    break;
  default:
    break;
  }
}

void DataService::printAll() {
  Serial.printf("cur : %d, day : %d, week : %d, month : %d\n", _currently, _daliy, _weekly, _monthly);
}

void DataService::addData(DATATYPE t, int a) {
  int aAmount = getData(t);
  setData(t, aAmount + a);
}