
// the setup function runs once when you press reset or power the board
#include "Logger.h"
#include "UbloxF9P.h"
#include "IMU.h"
#include "BNO08x.h"
#include "GPS.h"
#include "Logger.h"

double rollDelta;
double rollDeltaSmooth;
double correctionHeading;
double gyroDelta;
double imuGPS_Offset;
double gpsHeading;
double imuCorrected;
IMU* imu;

Logger logger;

void setup() {
	Serial.begin(115200);
	Serial.println("Starting");
	
	delay(1000);
	// normally, we'd check for CMPS14 first
	imu = new BNO080;
	imu->initialize();
	if (imu->devicePresent) {
		Serial.println("Found IMU!");
	}
	else {
		Serial.println("No IMU Found");
	}
	logger.LoggingDestination = Logger::LogDestination::USB;
	logger.LoggingAreaOfInterest = Logger::LogAreas::GPS; // +Logger::LogAreas::IMU;
}

// the loop function runs over and over again until power down or reset
void loop() {
	IMU::IMUData imuData = imu->getIMUData(false, false); // careful, protected in BNO08x class
	logger.LogMessage("Pitch: " + String(imuData.pitch),Logger::IMU);
	delay(500);
}
