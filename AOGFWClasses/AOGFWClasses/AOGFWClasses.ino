
// the setup function runs once when you press reset or power the board
#include "LED.h"
#include "Logger.h"
#include "UbloxF9P.h"
#include "IMU.h"
#include "BNO08x.h"
#include "GPS.h"
#include "Logger.h"
#include "LED.h"

double rollDelta;
double rollDeltaSmooth;
double correctionHeading;
double gyroDelta;
double imuGPS_Offset;
double gpsHeading;
double imuCorrected;
IMU* imu;

Logger logger;
LEDClass led;

void setup() {

	led.init();
	logger.LoggingDestination = Logger::LogDestination::USB;
	logger.LoggingAreaOfInterest = Logger::LogAreas::GPS + Logger::LogAreas::IMU;


	Serial.begin(115200);
	logger.LogMessage("Starting",Logger::LogAreas::General);
	
	delay(1000);
	// normally, we'd check for CMPS14 first but let's look for BNO first instead as it's more popular
	imu = new BNO080;
	imu->initialize(logger);
	if (imu->devicePresent) {
		logger.LogMessage("Found BNO!",Logger::LogAreas::General);
	}
	else {
		logger.LogMessage("No BNO found - will search for CMPS14 instead",Logger::LogAreas::General);
		//imu = new CMPS14;
	}
}

// the loop function runs over and over again until power down or reset
void loop() {
	IMU::IMUData imuData = imu->getIMUData(false, false); // careful, protected in BNO08x class
	// just here for testing, not of interest really
	logger.LogMessage("Pitch: " + String(imuData.pitch),Logger::IMU);
	led.ledOn(led.GGAReceivedLED);
	delay(250);
	led.ledOff(led.GGAReceivedLED);
	delay(250);

}
