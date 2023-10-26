
#ifndef _UBLOXF9P_h
#define _UBLOXF9P_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "GPS.h"
class UbloxF9P : public GPS
{
 protected:

 public:
	 void initialize() override {
		 return;
		 // Initialization code for Model A GPS
	 }

	 float getLatitude() override  {
		 return 0.0f;
		 // Implementation for Model A GPS
	 }

	 float getLongitude() override {
		 return 0.0f;
		 // Implementation for Model A GPS
	 }
};


#endif

