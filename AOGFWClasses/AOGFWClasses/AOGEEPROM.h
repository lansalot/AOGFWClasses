#ifndef _EEPROM_h
#define _EEPROM_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define EEP_Ident 2400

class AOGEEPROMClass
{
 protected:

	 static const int16_t EEread = 0;

 public:
	 static void readEEPROM();
};



#endif

