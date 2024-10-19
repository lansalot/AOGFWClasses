// SteerKeya.h

#ifndef _STEERKEYA_h
#define _STEERKEYA_h

#include <Arduino.h>

#include "Steer.h"
#include "..\Logger.h"


class SteerKeyaClass : public SteerClass
{
private:
	char beaconIdentifier[2] = { 0x80,0x99 };
	uint32_t KeyaBeacon = millis();

	float KeyaCurrentSensorReading = 0.0f;
	bool keyaDetected = false;

	void KeyaBus_Receive();
	void printIdAndReply(uint32_t id, uint8_t buf[8]);
	void keyaCommand(uint8_t command[]);
	bool isPatternMatch(const CAN_message_t& message, const uint8_t* pattern, size_t patternSize);

	uint8_t keyaDisableCommand[4] = { 0x23, 0x0C, 0x20, 0x01 };
	uint8_t keyaDisableResponse[4] = { 0x60, 0x0C, 0x20, 0x00 };

	uint8_t keyaEnableCommand[4] = { 0x23, 0x0D, 0x20, 0x01 };
	uint8_t keyaEnableResponse[4] = { 0x60, 0x0D, 0x20, 0x00 };

	uint8_t keyaSpeedCommand[4] = { 0x23, 0x00, 0x20, 0x01 };
	uint8_t keyaSpeedResponse[4] = { 0x60, 0x00, 0x20, 0x00 };

	uint8_t keyaCurrentQuery[4] = { 0x40, 0x00, 0x21, 0x01 };
	uint8_t keyaCurrentResponse[4] = { 0x60, 0x00, 0x21, 0x01 };

	uint8_t keyaFaultQuery[4] = { 0x40, 0x12, 0x21, 0x01 };
	uint8_t keyaFaultResponse[4] = { 0x60, 0x12, 0x21, 0x01 };

	uint8_t keyaVoltageQuery[4] = { 0x40, 0x0D, 0x21, 0x02 };
	uint8_t keyaVoltageResponse[4] = { 0x60, 0x0D, 0x21, 0x02 };

	uint8_t keyaTemperatureQuery[4] = { 0x40, 0x0F, 0x21, 0x01 };
	uint8_t keyaTemperatureResponse[4] = { 0x60, 0x0F, 0x21, 0x01 };

	uint8_t keyaVersionQuery[4] = { 0x40, 0x01, 0x11, 0x11 };
	uint8_t keyaVersionResponse[4] = { 0x60, 0x01, 0x11, 0x11 };

	uint64_t KeyaID = 0x06000001; // 0x01 is default ID

	String assembledMessage;
	uint32_t hbTime;
	uint32_t keyaTime;
	elapsedMillis keyaCurrentUpdateTimer = 0;
	bool keyaMotorStatus = false;

public:
	// need to pass this in to the init()  ?
	LoggerClass::LogCategories moduleLogLevel = LoggerClass::LogCategories::CANBUS;

	// properties
	float steerCurrent = 0.0f;

	//methods
	// take in a current log level parameter
	void init(LoggerClass::LogCategories moduleLogLevel) override;
	void SteerPWM(int steerSpeed) override;
	float getCurrent() override;

};

//extern SteerKeyaClass SteerKeya;

#endif

