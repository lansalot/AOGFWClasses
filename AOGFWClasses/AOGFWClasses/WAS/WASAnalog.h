// WASAnalog.h

#ifndef _WASANALOG_h
#define _WASANALOG_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "WAS.h"
#include "..\Logger.h"


class WASAnalogClass : public WASClass
{
 protected:
	 LoggerClass::LogAreas moduleLogLevel = LoggerClass::LogAreas::WAS;


 public:
	void init() override;
};

extern WASAnalogClass WASAnalog;

#endif

