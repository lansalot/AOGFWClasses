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
	 static bool Autosteer_running; //Auto set off in autosteer setup
	 static double rollDelta;
	 static double rollDeltaSmooth;
	 static double correctionHeading;
	 static double gyroDelta;
	 static double imuGPS_Offset;
	 static double gpsHeading;
	 static double imuCorrected;
	void init();
};

extern AOGStatusClass AOGStatus;

#endif

