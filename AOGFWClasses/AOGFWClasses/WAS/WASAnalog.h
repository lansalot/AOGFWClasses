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
	 LoggerClass::LogCategories moduleLogLevel = LoggerClass::LogCategories::WAS;


 public:
	void init() override;
};

extern WASAnalogClass WASAnalog;

#endif

