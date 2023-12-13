
#ifndef _UBLOXF9P_h
#define _UBLOXF9P_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "GPS.h"
#include "..\\zNMEAParser.h"


class UbloxF9P : public GPSClass
{
 protected:
	 LoggerClass::LogCategories moduleLogLevel = LoggerClass::LogCategories::GPS;


 public:



	 static void GGA_Handler();
	 static void VTG_Handler();
	 static void errorHandler();
	 static void ReadSerialPosition();
	 static int PortSwapTime;
	 void initialize();

};

#endif