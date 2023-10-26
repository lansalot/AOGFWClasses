// Logger.h

#ifndef _LOGGER_h
#define _LOGGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Logger
{
 protected:


 public:
	 enum LogAreas
	 {
		 Basic = 1,
		 GPS = 2,
		 IMU = 4,
		 PGN = 8,
		 CANBUS = 16,
		 PWM = 32,
		 Unspecified = 128
	 };
	 enum LogDestination
	 {
		 USB = 1,
		 UDP = 2
	 };
	 int8_t LoggingAreaOfInterest = 0;
	 int8_t LoggingDestination = 0;
	 void LogMessage(String message, LogAreas logAreas);
};


#endif

