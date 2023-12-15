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

	// IMU
	static char imuHeading[6];
	static char imuRoll[6];
	static char imuPitch[6];
	static char imuYawRate[6];



public:
#define SerialAOG Serial                //AgIO USB conection - I say we don't implement this?
#define SerialRTK Serial3               //RTK radio

	struct ubxPacket
	{
		uint8_t cls;
		uint8_t id;
		uint16_t len; //Length of the payload. Does not include cls, id, or checksum bytes
		uint16_t counter; //Keeps track of number of overall bytes received. Some responses are larger than 255 bytes.
		uint16_t startingSpot; //The counter value needed to go past before we begin recording into payload array
		uint8_t* payload; // We will allocate RAM for the payload if/when needed.
		uint8_t checksumA; //Given to us from module. Checked against the rolling calculated A/B checksums.
		uint8_t checksumB;

		////sfe_ublox_packet_validity_e valid;			 //Goes from NOT_DEFINED to VALID or NOT_VALID when checksum is checked
		////sfe_ublox_packet_validity_e classAndIDmatch; // Goes from NOT_DEFINED to VALID or NOT_VALID when the Class and ID match the requestedClass and requestedID
	};

	static bool blink;
	static const int32_t baudAOG = 115200;
	static const int32_t baudGPS = 460800;
	static const int32_t baudRTK = 9600;

	static uint32_t PortSwapTime;
	static HardwareSerial* SerialGPS; // =  &Serial7;   //Main postion receiver
	static HardwareSerial* SerialGPS2; // = &Serial2;  //Dual heading receiver 
	static HardwareSerial* SerialGPSTmp; // = NULL;

	enum { serial_buffer_size = 512 };
	static uint8_t GPSrxbuffer[serial_buffer_size];    //Extra serial rx buffer
	static uint8_t GPStxbuffer[serial_buffer_size];    //Extra serial tx buffer
	static uint8_t GPS2rxbuffer[serial_buffer_size];   //Extra serial rx buffer
	static uint8_t GPS2txbuffer[serial_buffer_size];   //Extra serial tx buffer
	static uint8_t RTKrxbuffer[serial_buffer_size];


	static bool GGA_Available;
	static bool useDual;
	static bool dualReadyGGA;
	static bool dualReadyRelPos;

	static bool passThroughGPS;
	static bool passThroughGPS2;
	static uint32_t gpsReadyTime;        //Used for GGA timeout

	// should probably have a constructor that initilisess stuff instead of relying on this

	void initialize();
	static void swapSerial();
	static void calcChecksum(ubxPacket* msg);
	static void parseRTK() ;


};


#endif