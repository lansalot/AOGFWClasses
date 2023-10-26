// GPS.h

#ifndef _GPS_h
#define _GPS_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "Logger.h"
class GPS
{
protected:
	Logger::LogAreas moduleLogLevel = Logger::GPS;

public:
	virtual void initialize() = 0;
	virtual float getLatitude() = 0;
	virtual float getLongitude() = 0;
	void init();
};



#endif

