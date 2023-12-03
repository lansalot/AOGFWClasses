
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
//#include "zNMEAParser.h"
#include "GPS\\GPS.h"
#include "GPS\\UbloxF9P.h"



extern "C" uint32_t set_arm_clock(uint32_t frequency); // required prototype


IMUClass* imu;
GPSClass* gps;

LEDClass led;

void setup() {

	led.init();

	delay(10);
	Serial.begin(AOGSerialClass::baudAOG); // talk to the world !
	delay(10);
	Logger.LogMessage("Initialising logging...", LoggerClass::LogCategories::General);

	Logger.LoggingDestination = LoggerClass::LogDestination::USB;
	Logger.LoggingInterest = LoggerClass::LogCategories::GPS + LoggerClass::LogCategories::IMU + LoggerClass::LogCategories::General;

	Logger.LogMessage("Starting Ethernet...",LoggerClass::LogCategories::General);
	AOGStatus.Autosteer_running = true;
	AOGEthernet.EthernetStart();

	AOGSerial.init();

	Logger.LogMessage("SerialAOG, SerialRTK, SerialGPS and SerialGPS2 initialized",LoggerClass::LogCategories::General);
	Logger.LogMessage("Looking for IMU",LoggerClass::LogCategories::General);
	
	delay(1000);
	// normally, we'd check for CMPS14 first but let's look for BNO first instead as it's more popular
	imu = new BNO080;
	imu->initialize();
	if (imu->devicePresent) {
		Logger.LogMessage("Found BNO!",LoggerClass::LogCategories::General);
	}
	else {
		Logger.LogMessage("No BNO found - will search for CMPS14 instead",LoggerClass::LogCategories::General);
		//imu = new CMPS14;
	}

	Logger.LogMessage("Looking for GPS", LoggerClass::LogCategories::General);
	gps  = new UbloxF9P;
	gps->initialize();
	SteerKeya.init(); // so that works OK...
	// here, we would be initialising CANBUS


}

// the loop function runs over and over again until power down or reset
void loop() {

	// a CANBUS receive here perhaps to kick the loop off?

	if (gps->GGA_Available == false && !gps->passThroughGPS && !gps->passThroughGPS2)
	{
		if (systick_millis_count - AOGSerial.PortSwapTime >= 10000)
		{
			Serial.println("Swapping GPS ports...\r\n");
			AOGSerial.swapSerial();
			AOGSerial.PortSwapTime = systick_millis_count;
		}
	}

	IMUClass::IMUData imuData = imu->getIMUData(imu->noInvertRoll, imu->useXAxis);
	// just here for testing, not of interest really
	Logger.LogMessage("Pitch: " + String(imuData.pitch),LoggerClass::LogCategories::IMU);
	led.ledOn(led.GGAReceivedLED);
	delay(250);
	led.ledOff(led.GGAReceivedLED);
	delay(250);

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
		// consider abstract CRC class, for AOG, $manufacturers etc
}
