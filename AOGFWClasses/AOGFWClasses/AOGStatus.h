// AOGStatus.h

#ifndef _AOGSTATUS_h
#define _AOGSTATUS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class AOGStatusClass
{
 protected:


 public:
	 bool Autosteer_running; //Auto set off in autosteer setup
	 double rollDelta;
	 double rollDeltaSmooth;
	 double correctionHeading;
	 double gyroDelta;
	 double imuGPS_Offset;
	 double gpsHeading;
	 double imuCorrected;

	 //Steer switch button  ***********************************************************************************************************
	 uint8_t currentState = 1, reading, previous = 0;
	 uint8_t pulseCount = 0; // Steering Wheel Encoder
	 bool encEnable = false; //debounce flag
	 uint8_t thisEnc = 0, lastEnc = 0;


	void init();
};

extern AOGStatusClass AOGStatus;

#endif

