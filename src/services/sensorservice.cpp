#include "services/sensorservice.hpp"

#define FIRSTSENSOR 2
#define SECONDSENSOR 4

const ulong MAXTIME = 500;

void SensorService::startup()
{
	pinMode(FIRSTSENSOR, INPUT);
	pinMode(SECONDSENSOR, INPUT);
}

void SensorService::loop()
{
	bool first = digitalRead(FIRSTSENSOR);
	bool second = digitalRead(SECONDSENSOR);

	if (_LastAStage == LOW && first == HIGH)
	{
		_timeA = millis();
	}
	if (_LastBStage == LOW && second == HIGH)
	{
		_timeB = millis();
	}

	if ((_timeA > 0 && _timeB > 0) && abs((long)(_timeB - _timeA)) <= MAXTIME)
	{
		if (_timeA < _timeB)
		{
			onTrigger.emit("enter");
		}
		else
		{
			onTrigger.emit("exit");
		}
		_timeA = 0;
		_timeB = 0;
	}

	if (_timeA > 0 && millis() - _timeA > MAXTIME)
	{
		_timeA = 0;
	}
	if (_timeB > 0 && millis() - _timeB > MAXTIME)
	{
		_timeB = 0;
	}

	// Save current state for next loop
	_LastAStage = first;
	_LastBStage = second;
}