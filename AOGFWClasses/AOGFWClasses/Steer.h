// Steer.h

#ifndef _STEER_h
#define _STEER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class SteerClass
{
 protected:


 public:
	virtual void init() = 0;
};


#endif

