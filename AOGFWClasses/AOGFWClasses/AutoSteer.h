// AutoSteerPID.h

#ifndef _AUTOSTEERPID_h
#define _AUTOSTEERPID_h

#include <Arduino.h>

//How many degrees before decreasing Max PWM
#define LOW_HIGH_DEGREES 3.0

struct Storage {
	uint8_t Kp = 40;              // proportional gain
	uint8_t lowPWM = 10;          // band of no action
	int16_t wasOffset = 0;
	uint8_t minPWM = 9;
	uint8_t highPWM = 60;         // max PWM value
	float steerSensorCounts = 30;
	float AckermanFix = 1;        // sent as percent
};

//Variables for settings - 0 is false
struct Setup {
	uint8_t InvertWAS = 0;
	uint8_t IsRelayActiveHigh = 0;    // if zero, active low (default)
	uint8_t MotorDriveDirection = 0;
	uint8_t SingleInputWAS = 1;
	uint8_t CytronDriver = 1;
	uint8_t SteerSwitch = 0;          // 1 if switch selected
	uint8_t SteerButton = 0;          // 1 if button selected
	uint8_t ShaftEncoder = 0;
	uint8_t PressureSensor = 0;
	uint8_t CurrentSensor = 0;
	uint8_t PulseCountMax = 5;
	uint8_t IsDanfoss = 0;
	uint8_t IsUseY_Axis = 0;     //Set to 0 to use X Axis, 1 to use Y avis
};

extern float steerAngleActual;
extern float steerAngleSetPoint;
extern int16_t steeringPosition;
extern float steerAngleError;
extern int16_t pwmDrive;
extern int16_t pwmDisplay;
extern float pValue;
extern float errorAbs;
extern float highLowPerDeg;

extern Setup steerConfig;
//extern Storage steerSettings;
#endif

