// SteerKeya.h

#ifndef _STEERKEYA_h
#define _STEERKEYA_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Steer.h"
#include "..\Logger.h"


class SteerKeyaClass : public SteerClass
{
 protected:
	 LoggerClass::LogAreas moduleLogLevel = LoggerClass::LogAreas::CANBUS;


 public:
	void init() override;
};

extern SteerKeyaClass SteerKeya;

#endif

