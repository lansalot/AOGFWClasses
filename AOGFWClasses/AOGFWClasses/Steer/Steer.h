// Steer.h

#ifndef _STEER_h
#define _STEER_h

#include <arduino.h>
#include "../Logger.h"

class SteerClass
{
 protected:

 public:
	 static float steerCurrent;
	 virtual void init(LoggerClass::LogCategories moduleLogLevel) = 0;
	 virtual void SteerPWM(int steerSpeed) = 0;
	 virtual float getCurrent() = 0;
};

#endif