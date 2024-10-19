// 
// 
// 

#include <FlexCAN_T4.h>
#include "SteerKeya.h"
#include "../AOGCANBUS.h"
#include "../AutoSteer.h"

FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_256> Keya_Bus;
float SteerClass::steerCurrent = 0;


void SteerKeyaClass::init(LoggerClass::LogCategories moduleLogLevel)
{
	Logger.LogMessage("SteerKeyaClass::init()", moduleLogLevel);
	Keya_Bus.begin();
	Keya_Bus.setBaudRate(250000);
	Logger.LogMessage("Initialised Keya CANBUS @ 250k", moduleLogLevel);
}

void SteerKeyaClass::SteerPWM(int steerSpeed)
{
	if (keyaCurrentUpdateTimer > 99)
		keyaCommand(keyaCurrentQuery); // for motors with slow HB

	if (steerSpeed == 0)
	{
		keyaCommand(keyaDisableCommand);
		return; // don't need to go any further, if we're disabling, we're disabling
	}

	if (keyaDetected)
	{
		int actualSpeed = map(steerSpeed, -255, 255, -995, 995);
		Logger.LogMessage("I was told to steer, with " + String(steerSpeed) + " so I converted that to speed " + String(actualSpeed), moduleLogLevel);

		CAN_message_t KeyaBusSendData;
		KeyaBusSendData.id = KeyaID;
		KeyaBusSendData.flags.extended = true;
		KeyaBusSendData.len = 8;
		memcpy(KeyaBusSendData.buf, keyaSpeedCommand, 4);
		if (steerSpeed < 0)
		{
			KeyaBusSendData.buf[4] = highByte(actualSpeed);
			KeyaBusSendData.buf[5] = lowByte(actualSpeed);
			KeyaBusSendData.buf[6] = 0xff;
			KeyaBusSendData.buf[7] = 0xff;
			Logger.LogMessage("pwmDrive < zero - clockwise - steerSpeed " + String(steerSpeed), moduleLogLevel);
		}
		else
		{
			KeyaBusSendData.buf[4] = highByte(actualSpeed);
			KeyaBusSendData.buf[5] = lowByte(actualSpeed);
			KeyaBusSendData.buf[6] = 0x00;
			KeyaBusSendData.buf[7] = 0x00;
			Logger.LogMessage("pwmDrive > zero - anticlock-clockwise - steerSpeed " + String(steerSpeed), moduleLogLevel);
		}
		Keya_Bus.write(KeyaBusSendData);
		keyaCommand(keyaEnableCommand);
	}
}

float SteerKeyaClass::getCurrent()
{
	steerCurrent = KeyaCurrentSensorReading;
	return KeyaCurrentSensorReading;
}

void SteerKeyaClass::keyaCommand(uint8_t command[])
{
	if (keyaDetected)
	{
		CAN_message_t KeyaBusSendData;
		KeyaBusSendData.id = KeyaID;
		KeyaBusSendData.flags.extended = true;
		KeyaBusSendData.len = 8;
		memcpy(KeyaBusSendData.buf, command, 4);
		Keya_Bus.write(KeyaBusSendData);
	}
}

bool SteerKeyaClass::isPatternMatch(const CAN_message_t& message, const uint8_t* pattern, size_t patternSize)
{
	return false;
}

void SteerKeyaClass::KeyaBus_Receive()
{
	CAN_message_t KeyaBusReceiveData;
	if (Keya_Bus.read(KeyaBusReceiveData))
	{
		// parse the different message types

		// heartbeat 00:07:00:00:00:00:00:[ID]
		if (KeyaBusReceiveData.id == 0x07000001)
		{
			if (!keyaDetected)
			{
				assembledMessage = "Keya heartbeat detected! Enabling Keya canbus & using reported motor current for disengage";
				keyaDetected = true;
				keyaCommand(keyaVersionQuery);
				if (Keya_Bus.read(KeyaBusReceiveData)) {
					assembledMessage += "Keya Version: ";
					for (byte b = 0; b < KeyaBusReceiveData.len; b++) {
						assembledMessage += String(KeyaBusReceiveData.buf[b], HEX);
					}
				}
			}
			// 0-1 - Cumulative value of angle (360 def / circle)
			// 2-3 - Motor speed, signed int eg -500 or 500
			// 4-5 - Motor current, with "symbol" ? Signed I think that means, but it does appear to be a crap int. 1, 2 for 1, 2 amps etc
			//		is that accurate enough for us?
			// 6-7 - Control_Close (error code)
			// TODO Yeah, if we ever see something here, fire off a disable, refuse to engage autosteer or..?
			if (moduleLogLevel == LoggerClass::LogCategories::CANBUS) {
				uint32_t time = millis();
				assembledMessage = time;
				assembledMessage += " ";
				assembledMessage += time - hbTime;
				assembledMessage += " ";
				hbTime = time;
				printIdAndReply(KeyaBusReceiveData.id, KeyaBusReceiveData.buf);
				assembledMessage += " HB ";

				// gonna need a CR in here somewhere !
				// calc speed
				assembledMessage += KeyaBusReceiveData.buf[2];
				assembledMessage += ":";
				assembledMessage += KeyaBusReceiveData.buf[3];
				assembledMessage += "=";
				if (KeyaBusReceiveData.buf[2] == 0xFF)
				{
					assembledMessage += "-";
					assembledMessage += 255 - KeyaBusReceiveData.buf[3];
				}
				else
				{
					assembledMessage += KeyaBusReceiveData.buf[3];
				}
				assembledMessage += " ";

				// calc current
				assembledMessage += KeyaBusReceiveData.buf[4];
				assembledMessage += ":";
				assembledMessage += KeyaBusReceiveData.buf[5];
				assembledMessage += "=";
				if (KeyaBusReceiveData.buf[4] == 0xFF)
				{
					assembledMessage += "-";
					assembledMessage += 255 - KeyaBusReceiveData.buf[5];
					// KeyaCurrentSensorReading = (255 - KeyaBusReceiveData.buf[5]) * 20;  // use other motor current query data
				}
				else
				{
					assembledMessage += KeyaBusReceiveData.buf[5];
					// KeyaCurrentSensorReading = KeyaBusReceiveData.buf[5] * 20;
				}
				// keyaCurrentUpdateTimer = 0;
				assembledMessage += " ";

				// print error status
				assembledMessage += KeyaBusReceiveData.buf[6];
				assembledMessage += ":";
				assembledMessage += KeyaBusReceiveData.buf[7];
				assembledMessage += " "; // assembledMessage += KeyaCurrentSensorReading);
				keyaMotorStatus = !bitRead(KeyaBusReceiveData.buf[7], 0);
				assembledMessage += "\r\nmotor status ";
				assembledMessage += keyaMotorStatus;
			}
			// check if there's any motor diag/error data and parse it
			if (KeyaBusReceiveData.buf[7] != 0)
			{

				// motor disabled bit
				if (bitRead(KeyaBusReceiveData.buf[7], 0))
				{
					if (steerConfig.SteerSwitch == 0 && keyaMotorStatus == 1)
					{
						assembledMessage += "\r\nMotor disabled";
						assembledMessage += " - set AS off";
						steerConfig.SteerSwitch = 1; // turn off AS if motor's internal shutdown triggers
						// currentState = 1;
						// prevSteerReading = 0;
					}
				}
				if (!(moduleLogLevel & LoggerClass::LogCategories::CANBUS)) {
					return;
				}
				if (bitRead(KeyaBusReceiveData.buf[7], 1))
				{
					Logger.LogMessage("Over voltage", moduleLogLevel);
				}
				else if (bitRead(KeyaBusReceiveData.buf[7], 2))
				{
					Logger.LogMessage("Hardware protection", moduleLogLevel);
				}
				else if (bitRead(KeyaBusReceiveData.buf[7], 3))
				{
					Logger.LogMessage("E2PROM", moduleLogLevel);
				}
				else if (bitRead(KeyaBusReceiveData.buf[7], 4))
				{
					Logger.LogMessage("Under voltage", moduleLogLevel);
				}
				else if (bitRead(KeyaBusReceiveData.buf[7], 5))
				{
					Logger.LogMessage("N/A", moduleLogLevel);
				}
				else if (bitRead(KeyaBusReceiveData.buf[7], 6))
				{
					Logger.LogMessage("Over current", moduleLogLevel);
				}
				else if (bitRead(KeyaBusReceiveData.buf[7], 7))
				{
					Logger.LogMessage("Mode failure", moduleLogLevel);
				}
			}
		}

		if (KeyaBusReceiveData.buf[6] != 0)
		{
			if (!(moduleLogLevel & LoggerClass::LogCategories::CANBUS)) {
				return;
			}
			if (bitRead(KeyaBusReceiveData.buf[6], 0))
			{
				Logger.LogMessage("Less phase", moduleLogLevel);
			}
			else if (bitRead(KeyaBusReceiveData.buf[6], 1))
			{
				Logger.LogMessage("Motor stall", moduleLogLevel);
			}
			else if (bitRead(KeyaBusReceiveData.buf[6], 2))
			{
				Logger.LogMessage("Reserved", moduleLogLevel);
			}
			else if (bitRead(KeyaBusReceiveData.buf[6], 3))
			{
				Logger.LogMessage("Hall failure", moduleLogLevel);
			}
			else if (bitRead(KeyaBusReceiveData.buf[6], 4))
			{
				Logger.LogMessage("Current sensing", moduleLogLevel);
			}
			else if (bitRead(KeyaBusReceiveData.buf[6], 5))
			{
				Logger.LogMessage("232 disconnected", moduleLogLevel);
			}
			else if (bitRead(KeyaBusReceiveData.buf[6], 6))
			{
				Logger.LogMessage("CAN disconnected", moduleLogLevel);
			}
			else if (bitRead(KeyaBusReceiveData.buf[6], 7))
			{
				Logger.LogMessage("Motor stalled", moduleLogLevel);
			}
		}
	}

	// parse query/command 00:05:08:00:00:00:00:[ID] responses
	if (KeyaBusReceiveData.id == 0x05800001)
	{
		if (isPatternMatch(KeyaBusReceiveData, keyaCurrentResponse, sizeof(keyaCurrentResponse)))
		{
			uint32_t time = millis();
			assembledMessage = time;
			assembledMessage += " ";
			assembledMessage += time - keyaTime;
			assembledMessage += " ";
			printIdAndReply(KeyaBusReceiveData.id, KeyaBusReceiveData.buf);
			assembledMessage += " current reply ";
			assembledMessage += KeyaBusReceiveData.buf[4];
			assembledMessage += " ave ";
			Logger.LogMessage(assembledMessage, moduleLogLevel);
			//assembledMessage += sensorReading / 2.5; // to print ave in "amps"
			keyaTime = time;
			//KeyaCurrentSensorReading = KeyaBusReceiveData.buf[4] * 2.5; // so that AoG's display shows "amps"
			//mtz8302 
			KeyaCurrentSensorReading = float(KeyaBusReceiveData.buf[4]) * 25; // so that AoG's display shows "amps"

			keyaCurrentUpdateTimer -= 100;
		}

		// just logging after this, so let's bail and save some time
		if (!(moduleLogLevel & LoggerClass::LogCategories::CANBUS)) {
			return;
		}
		// Disable command response
		else if (isPatternMatch(KeyaBusReceiveData, keyaDisableResponse, sizeof(keyaDisableResponse)))
		{
			// printIdAndReply(KeyaBusReceiveData.id, KeyaBusReceiveData.buf);
			// assembledMessage += " disable reply ");
		}

		// Enable command response
		else if (isPatternMatch(KeyaBusReceiveData, keyaEnableResponse, sizeof(keyaEnableResponse)))
		{
			// printIdAndReply(KeyaBusReceiveData.id, KeyaBusReceiveData.buf);
			// assembledMessage += " enable reply ");
		}

		// Speed command response
		else if (isPatternMatch(KeyaBusReceiveData, keyaSpeedResponse, sizeof(keyaSpeedResponse)))
		{
			// printIdAndReply(KeyaBusReceiveData.id, KeyaBusReceiveData.buf);
			// assembledMessage += " speed reply ");
		}

		// Current query response (this is also in heartbeat)

	}

	// just logging after this, so let's bail and save some time
	if (!(moduleLogLevel & LoggerClass::LogCategories::CANBUS)) {
		return;
	}
	// Fault query response
	else if (isPatternMatch(KeyaBusReceiveData, keyaFaultResponse, sizeof(keyaFaultResponse)))
	{
		printIdAndReply(KeyaBusReceiveData.id, KeyaBusReceiveData.buf);
		assembledMessage = " fault reply ";
		assembledMessage += KeyaBusReceiveData.buf[4];
		assembledMessage += ":";
		assembledMessage += KeyaBusReceiveData.buf[5];
	}

	// Voltage query response
	else if (isPatternMatch(KeyaBusReceiveData, keyaVoltageResponse, sizeof(keyaVoltageResponse)))
	{
		printIdAndReply(KeyaBusReceiveData.id, KeyaBusReceiveData.buf);
		assembledMessage = " voltage reply ";
		assembledMessage += KeyaBusReceiveData.buf[4];
	}

	// Temperature query response
	else if (isPatternMatch(KeyaBusReceiveData, keyaTemperatureResponse, sizeof(keyaTemperatureResponse)))
	{
		printIdAndReply(KeyaBusReceiveData.id, KeyaBusReceiveData.buf);
		assembledMessage = " temperature reply ";
		assembledMessage += KeyaBusReceiveData.buf[4];
	}

	// Version query response
	else if (isPatternMatch(KeyaBusReceiveData, keyaVersionResponse, sizeof(keyaVersionResponse)))
	{
		printIdAndReply(KeyaBusReceiveData.id, KeyaBusReceiveData.buf);
		assembledMessage = " version reply ";
		assembledMessage += KeyaBusReceiveData.buf[4];
		assembledMessage += ":";
		assembledMessage += KeyaBusReceiveData.buf[5];
		assembledMessage += ":";
		assembledMessage += KeyaBusReceiveData.buf[6];
		assembledMessage += ":";
		assembledMessage += KeyaBusReceiveData.buf[7];
	}
	else
	{
		printIdAndReply(KeyaBusReceiveData.id, KeyaBusReceiveData.buf);
		assembledMessage = " unknown reply ";
	}
	Logger.LogMessage(assembledMessage, moduleLogLevel);
}

void SteerKeyaClass::printIdAndReply(uint32_t id, uint8_t buf[8])
{
	assembledMessage = "";
	assembledMessage += String(id, HEX);
	assembledMessage += (" <> ");
	for (byte i = 0; i < 8; i++)
	{
		if (buf[i] < 16)
			assembledMessage += ("0");
		assembledMessage += (buf[i], HEX);
		if (i < 7)
			assembledMessage += (":");
	}
	Logger.LogMessage(assembledMessage, moduleLogLevel);
}



