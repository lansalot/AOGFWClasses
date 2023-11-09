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
	 char beaconIdentifier[2] = { 0x80,0x99 };
	 uint32_t KeyaBeacon = millis();
	 
	 int8_t KeyaCurrentSensorReading = 0;
	 bool keyaDetected = false;

	 LoggerClass::LogCategories moduleLogLevel = LoggerClass::LogCategories::CANBUS;


 public:

	void init() override;

};

extern SteerKeyaClass SteerKeya;

#endif

