#ifndef _BNO_rvc_h
#define _BNO_rvc_h



#include "IMU.h"
#include "..\\Logger.h"

constexpr auto MILLI_G_TO_MS2 = 0.0098067; ///< Scalar to convert milli-gs to m/s^2;
constexpr auto DEGREE_SCALE = 0.01;        ///< To convert the degree values

//typedef struct BNO_rvcData {
//	int16_t yawX10,     ///< Yaw in Degrees x 10
//		pitchX10,     ///< Pitch in Degrees x 10
//		rollX10,     ///< Roll in Degrees x 10
//		yawX100,      // yaw in original x100
//		angVel;         //running total of angular velocity
//} BNO_rvcData;

class BNO_rvc : public IMUClass
{

protected:

public:

		IMUClass::IMUData imuData;
		//int16_t yawX10,     ///< Yaw in Degrees x 10
		//	pitchX10,     ///< Pitch in Degrees x 10
		//	rollX10,     ///< Roll in Degrees x 10
		//	yawX100,      // yaw in original x100
		//	angVel;         //running total of angular velocity

	BNO_rvc() {
		devicePresent = false;
		angCounter = 0;
		imuData.correctionHeading = 0;
		imuData.yaw = -1;
		imuData.pitch = 0;
		imuData.roll = 0;
		imuData.x_accel = 0;
		imuData.y_accel = 0;
		imuData.z_accel = 0;
	};
	
	~BNO_rvc() {};

	bool initialize(rollState rollState, imuAxisState imuAxis) override;
	
	bool read() override;

	bool read(IMUClass::IMUData *imudata) override; // this is a generic, that triggers the device-specific read
	bool rvcRead(IMUClass::IMUData *bnoData);


	//using IMUClass::imuData;
	//using IMUClass::devicePresent;
	//using IMUClass::rollState;
	//using IMUClass::imuAxisState;

	//bool begin();

private:
	uint32_t angCounter;
	HardwareSerial *bnoSerial;
	IMUData bnoData;
	rollState _rollState;
	imuAxisState _imuAxisState;
	int16_t prevYaw;


};

#endif

