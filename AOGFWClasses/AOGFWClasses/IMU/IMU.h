
#ifndef _IMU_h
#define _IMU_h

#include "..\\Logger.h"


class IMUClass
{
protected:
	LoggerClass::LogCategories moduleLogLevel = LoggerClass::LogCategories::IMU;

public:
	struct IMUData {
		float yaw = 0;
		float pitch = 0;
		float roll = 0;
		float x_accel = 0;
		float y_accel = 0;
		float z_accel = 0;
		double correctionHeading = 0;
	};
	// add an enum for rollState and imuAxisState
	enum rollState
	{
		Inverted = 0,
		Normal = 1
	};
	enum imuAxisState
	{
		XOrientation = 0,
		YOrientation = 1
	};


	IMUData imuData;
	boolean devicePresent;
	virtual bool initialize(rollState rollState, imuAxisState imuAxis) = 0;
	virtual bool read() = 0;


	// why should this class perform the initialisation?  who cares?
	//virtual void initialize() = 0;
	//virtual IMUData getIMUData(rollState roll, imuAxisState imuAxis) = 0;
};


#endif