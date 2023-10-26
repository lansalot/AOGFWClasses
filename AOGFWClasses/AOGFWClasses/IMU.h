
#ifndef _IMU_h
#define _IMU_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class IMU
{
protected:

public:
	struct IMUData {
		float roll = 0;
		float pitch = 0;
		float yaw = 0;
		double correctionHeading = 0;
	};
	IMUData imuData;


	virtual void initialize() = 0;
	boolean devicePresent;
	virtual IMUData getIMUData(bool invertRoll, bool useYAxis) = 0;
};


#endif