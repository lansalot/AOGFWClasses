// SteerKeya.h

#ifndef _STEERKEYA_h
#define _STEERKEYA_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class SteerKeyaClass : public SteerClass
{
 protected:


 public:
	void init() override;
};

extern SteerKeyaClass SteerKeya;

#endif

