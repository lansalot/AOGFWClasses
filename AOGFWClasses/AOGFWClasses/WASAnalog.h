// WASAnalog.h

#ifndef _WASANALOG_h
#define _WASANALOG_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class WASAnalogClass : public WAS
{
 protected:


 public:
	void init() override;
};

extern WASAnalogClass WASAnalog;

#endif

