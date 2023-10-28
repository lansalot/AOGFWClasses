// WAS.h

#ifndef _WAS_h
#define _WAS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class WASClass
{
 protected:


 public:
	virtual void init() = 0;
};


#endif

