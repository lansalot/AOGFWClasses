
// the setup function runs once when you press reset or power the board

#include "AOGCANBUS.h"
#include "AOGSerial.h"
#include "AOGStatus.h"
#include "AOGEthernet.h"
#include "WAS\\WASCANBUS.h"
#include "WAS\\WASAnalog.h"
#include "WAS\\WAS.h"
#include "Steer\\SteerCANBUS.h"
#include "Steer\\SteerCytron.h"
#include "Steer\\SteerKeya.h"
#include "Steer\\Steer.h"
#include "AOGEEPROM.h"
#include "LED.h"
#include "Logger.h"
#include "IMU\\IMU.h"
#include "IMU\\BNO08x.h"
#include "zNMEAParser.h"


extern "C" uint32_t set_arm_clock(uint32_t frequency); // required prototype


IMU* imu;
//GPS* gps;

LEDClass led;

void setup() {

	led.init();

	delay(10);
	Serial.begin(AOGSerialClass::baudAOG);
	delay(10);
	Serial.println("Start setup");

	Logger.LoggingDestination = LoggerClass::LogDestination::USB;
	Logger.LoggingAreaOfInterest = LoggerClass::LogAreas::GPS + LoggerClass::LogAreas::IMU + LoggerClass::LogAreas::General;

	Logger.LogMessage("Starting Ethernet...",LoggerClass::LogAreas::General);
	AOGStatus.Autosteer_running = true;
	AOGEthernet.EthernetStart();

	//gps = new UbloxF9P;

	Serial.begin(115200);
	Logger.LogMessage("Starting",LoggerClass::LogAreas::General);
	
	delay(1000);
	// normally, we'd check for CMPS14 first but let's look for BNO first instead as it's more popular
	imu = new BNO080;
	imu->initialize();
	if (imu->devicePresent) {
		Logger.LogMessage("Found BNO!",LoggerClass::LogAreas::General);
	}
	else {
		Logger.LogMessage("No BNO found - will search for CMPS14 instead",LoggerClass::LogAreas::General);
		//imu = new CMPS14;
	}
}

// the loop function runs over and over again until power down or reset
void loop() {
	IMU::IMUData imuData = imu->getIMUData(imu->noInvertRoll, imu->useXAxis);
	// just here for testing, not of interest really
	Logger.LogMessage("Pitch: " + String(imuData.pitch),LoggerClass::LogAreas::IMU);
	led.ledOn(led.GGAReceivedLED);
	delay(250);
	led.ledOff(led.GGAReceivedLED);
	delay(250);

	SteerKeya.init(); // so that works OK...
	// To work AOG-style, loop should
		// check if GGA available
		// check if ntrip available
		// check if NMEA from GPS
		// udpNtrip?
		// check for RTK radio
		// if both dual messages are ready, send to AOG
		// handle relposdata on serialgps2
		// check for GGA timeout and turn off GPS LEDs
		// read BNO/IMU
		// enter autosteer loop or receive UDP
		// update LEDs
}
