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


	void init();
};

extern AOGSerialClass AOGSerial;

#endif

