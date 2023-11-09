// GPS.h

#ifndef _GPS_h
#define _GPS_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "..\\Logger.h"


class GPSClass
{
protected:
	LoggerClass::LogCategories moduleLogLevel = LoggerClass::LogCategories::GPS;

	// Conversion to Hexadecimal
	const char* asciiHex = "0123456789ABCDEF";

	// the new PANDA sentence buffer
	char nmea[100];

	// GGA
	static char fixTime[12];
	static char latitude[15];
	static char latNS[3];
	static char longitude[15];
	static char lonEW[3];
	static char fixQuality[2];
	static char numSats[4];
	static char HDOP[5];
	static char altitude[12];
	static char ageDGPS[10];

	// VTG
	static constexpr char vtgHeading[12] = { };
	static constexpr char speedKnots[10] = { };

	// IMU
	static char imuHeading[6];
	static char imuRoll[6];
	static char imuPitch[6];
	static char imuYawRate[6];

	boolean blink = false;

	boolean GGA_Available = false;
	bool useDual = false;
	bool dualReadyGGA = false;
	bool dualReadyRelPos = false;

public:
	virtual void initialize() = 0;


	uint32_t gpsReadyTime = 0;        //Used for GGA timeout

};


#endif