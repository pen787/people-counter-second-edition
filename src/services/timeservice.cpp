#include "services/timeservice.hpp"

tm getTime()
{
  struct tm timeinfo = {};
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time 1");
    return timeinfo;
  }
  // Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S zone %Z %z ");
  return timeinfo;
}

void TimeService::startup()
{
  struct tm timeinfo = getTime();

  _lastDay = timeinfo.tm_mday;
  _lastMonth = timeinfo.tm_mon;
  _lastDayOfWeek = timeinfo.tm_wday;
  _lastupdate = millis();
}

void TimeService::loop()
{
  if (millis() - _lastupdate >= _updateTime)
  {
    struct tm timeinfo = getTime();

    // Check for new day (detects day change)
    if (timeinfo.tm_mday != _lastDay)
    {
      onTime.emit("day");
      _lastDay = timeinfo.tm_mday;
    }

    // Check for new week (Monday detection)
    if (timeinfo.tm_wday == 1)
    { // Monday = 1
      if (_lastDayOfWeek != 1)
      {
        onTime.emit("week");
      }
    }
    _lastDayOfWeek = timeinfo.tm_wday; // Always update weekday

    // Check for new month
    if (timeinfo.tm_mon != _lastMonth)
    {
      onTime.emit("month");
      _lastMonth = timeinfo.tm_mon;
    }
    _lastupdate = millis();

    Serial.println("Checking Time");
  }
}