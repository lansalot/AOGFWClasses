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



public:
	// Baudrates for detecting UBX receiver
	static uint32_t baudrates[9];

	static const uint32_t nrBaudrates;
	// Buffer to read chars from Serial, to check if "!AOG" is found
	static uint8_t aogSerialCmd[4];
	static uint8_t aogSerialCmdBuffer[6];
	static uint8_t aogSerialCmdCounter;

	static void init();
};


extern AOGSerialClass AOGSerial;
#endif

