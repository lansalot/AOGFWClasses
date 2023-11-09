// SteerCytron.h

#ifndef _STEERCYTRON_h
#define _STEERCYTRON_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Steer.h"
#include "..\Logger.h"

class SteerCytronClass : public SteerClass
{
 protected:
	 LoggerClass::LogCategories moduleLogLevel = LoggerClass::LogCategories::PWM;


 public:
	void init();
};

extern SteerCytronClass SteerCytron;


#endif

