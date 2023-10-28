// SteerCANBUS.h

#ifndef _STEERCANBUS_h
#define _STEERCANBUS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Steer.h"
#include "..\Logger.h"

class SteerCANBUS : public SteerClass
{
 protected:
	 LoggerClass::LogAreas moduleLogLevel = LoggerClass::LogAreas::CANBUS;


 public:
	void init();
};


#endif

