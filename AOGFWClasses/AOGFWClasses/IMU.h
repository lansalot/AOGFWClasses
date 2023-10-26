
#ifndef _IMU_h
#define _IMU_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "Logger.h"

class IMU
{
protected:
	Logger::LogAreas moduleLogLevel = Logger::IMU;


public:
	struct IMUData {
		float roll = 0;
		float pitch = 0;
		float yaw = 0;
		double correctionHeading = 0;
	};
	IMUData imuData;
	enum rollState {
		noInvertRoll = 0,
		invertRoll = 1
	};
	enum imuAxisState {
		useYAxis = 0,
		useXAxis = 1
	};
	boolean devicePresent;
	virtual void initialize(Logger& logger) = 0;
	virtual IMUData getIMUData(rollState roll, imuAxisState imuAxis) = 0;
};


#endif