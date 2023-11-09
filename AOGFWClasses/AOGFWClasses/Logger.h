#ifndef _LOGGER_h
#define _LOGGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class LoggerClass
{
 protected:


 public:
	 enum LogCategories
	 {
		 Basic = 1,
		 GPS = 2,
		 IMU = 4,
		 PGN = 8,
		 CANBUS = 16,
		 PWM = 32,
		 WAS = 64,
		 General = 128
	 };
	 enum LogDestination
	 {
		 USB = 1,
		 UDP = 2
	 };
	 int8_t LoggingInterest = 0;
	 int8_t LoggingDestination = 0;

	 void LogMessage(String message, LogCategories logCategories);
	 
};

extern LoggerClass Logger;


#endif

