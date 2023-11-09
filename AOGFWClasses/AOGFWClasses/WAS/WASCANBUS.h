// WASCANBUS.h

#ifndef _WASCANBUS_h
#define _WASCANBUS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "WAS.h"
#include "..\Logger.h"

class WASCANBUSClass : public WASClass
{
 protected:
	 LoggerClass::LogCategories moduleLogLevel = LoggerClass::LogCategories::CANBUS;


	 // there's something to watch here - FIFOfilters will need to be set on the same bus for WAS and steer

 public:
	void init() override;
};

extern WASCANBUSClass WASCANBUS;

#endif

