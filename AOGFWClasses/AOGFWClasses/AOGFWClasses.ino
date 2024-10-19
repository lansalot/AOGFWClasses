
// Fhe setup function runs once when you press reset or power the board

#include "Timer.h"
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
#include "IMU\\BNO_rvc.h"
#include "GPS\\GPS.h"
#include "GPS\\UbloxF9P.h"
#include "Timer.h"

extern "C" uint32_t set_arm_clock(uint32_t frequency); // required prototype


IMUClass* imuInstance; // doesn't declare an instance of the class, just a pointer to it
//BNO_rvc* imuInstance;
GPSClass* gps;
LEDClass led;
uint32_t lastmillis = millis();
ElapsedTimer timer;

void setup() {
	led.init();
	Serial.println("Initialising!");
	delay(10);
	Serial.begin(GPSClass::baudAOG); // talk to the world !
	delay(10);
	Logger.LogMessage("Initialising logging...", LoggerClass::LogCategories::General);

	Logger.LoggingDestination = LoggerClass::LogDestination::USB;
	Logger.LoggingInterest = LoggerClass::LogCategories::GPS + LoggerClass::LogCategories::IMU + LoggerClass::LogCategories::General;

	Logger.LogMessage("Starting Ethernet...", LoggerClass::LogCategories::General);
	AOGStatus.Autosteer_running = true;
	AOGEthernet.EthernetStart();

	AOGSerialClass::init();

	Logger.LogMessage("SerialAOG, SerialRTK, SerialGPS and SerialGPS2 initialized", LoggerClass::LogCategories::General);
	Logger.LogMessage("Looking for IMU", LoggerClass::LogCategories::General);

	delay(1000);

	imuInstance = new BNO_rvc();
	imuInstance->initialize(IMUClass::rollState::Normal, IMUClass::imuAxisState::XOrientation);

	// normally, we'd check for CMPS14 first but let's look for BNO first instead as it's more popular
	// instantiate an IMU object of subclass bno_rvc


	//BNO_rvcData bnoData;
	//BNO_rvc imu = new BNO_rvc();
	//if (imu.read(&bnoData)) useBNO08xRVC = true;
	//imu->initialize();
	//if (imu->devicePresent) {
	//	Logger.LogMessage("Found BNO!",LoggerClass::LogCategories::General);
	//}
	//else {
	//	Logger.LogMessage("No BNO found - will search for CMPS14 instead",LoggerClass::LogCategories::General);
	//	//imu = new CMPS14;
	//}

	//gps  = new UbloxF9P;
	//gps->initialize();

	//SteerKeya.init(); // so that works OK...
	// here, we would be initialising CANBUS

}

// the loop function runs over and over again until power down or reset
void loop() {
	//Logger.LogMessage("Looping...", LoggerClass::LogCategories::IMU);
	// a CANBUS receive here perhaps to kick the loop off?

	//IMUClass::IMUData imuData = 

	imuInstance->read();
	if (timer.getCheckpoint() > 250) {
		led.ledOn(led.GGAReceivedLED);
		Logger.LogMessage("yaw: " + String(imuInstance->imuData.yaw) + "  pitch : " + String(imuInstance->imuData.pitch) + 
		"  roll: " + String(imuInstance->imuData.roll) + "  xaccel: " + String(imuInstance->imuData.x_accel) + 
		"  yaccel: " + String(imuInstance->imuData.y_accel) + "  zaccel: " + String(imuInstance->imuData.z_accel),LoggerClass::IMU);
		timer.checkpoint();
	}

	// just here for testing, not of interest really
	//delay(500);
	//led.ledOff(led.GGAReceivedLED);
	//delay(500);
	//Serial.print("Roll: "); Serial.println(imuInstance->imuData.roll);
	// incoming RTK
	//gps->parseRTK();

	//unsigned int packetLength = AOGEthernet.parsePacket();

	//if (packetLength > 0)
	//{
	//	if (packetLength > serial_buffer_size) packetLength = serial_buffer_size;
	//	Eth_udpNtrip.read(Eth_NTRIP_packetBuffer, packetLength);
	//	SerialGPS->write(Eth_NTRIP_packetBuffer, packetLength);
	//}
	// 
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
