// SteerCANBUS.h

#ifndef _STEERCANBUS_h
#define _STEERCANBUS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class SteerCANBUS : public SteerClass
{
 protected:


 public:
	void init();
};


#endif

