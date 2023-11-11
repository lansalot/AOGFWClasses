// AOGSerial.h

#ifndef _AOGSERIAL_h
#define _AOGSERIAL_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class AOGSerialClass
{
protected:
	// Baudrates for detecting UBX receiver
	uint32_t baudrates[9] =
	{
	  4800,
	  9600,
	  19200,
	  38400,
	  57600,
	  115200,
	  230400,
	  460800,
	  921600
	};

	const uint32_t nrBaudrates = sizeof(baudrates) / sizeof(baudrates[0]);


public:
#define SerialAOG Serial                //AgIO USB conection
#define SerialRTK Serial3               //RTK radio
	static HardwareSerial* SerialGPS; // =  &Serial7;   //Main postion receiver (GGA) (Serial2 must be used here with T4.0 / Basic Panda boards - Should auto swap)
	static HardwareSerial* SerialGPS2; // = &Serial2;  //Dual heading receiver 
	static HardwareSerial* SerialGPSTmp; // = NULL;
	
	static const int32_t baudAOG = 115200;
	static 	const int32_t baudGPS = 460800;
	static const int32_t baudRTK = 9600;
	void init();
};

extern AOGSerialClass AOGSerial;

#endif
