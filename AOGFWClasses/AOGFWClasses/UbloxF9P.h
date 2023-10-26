
#ifndef _UBLOXF9P_h
#define _UBLOXF9P_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "GPS.h"
#include "zNMEAParser.h"
class UbloxF9P : public GPS
{
 protected:
	 static NMEAParser<2> parser;

 public:
	 static void GGA_Handler();
	 static void VTG_Handler();
	 static void errorHandler();
	 void initialize() override {};
};


#endif

