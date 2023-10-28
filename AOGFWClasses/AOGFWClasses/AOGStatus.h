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
	void init();
};

extern AOGStatusClass AOGStatus;

#endif

