#include "AOGEEPROM.h"
#include <EEPROM.h>


void AOGEEPROMClass::readEEPROM()
{
	EEPROM.get(0, EEread);              // read identifier

	//if (EEread != EEP_Ident)            // check on first start and write EEPROM
	//{
	//	EEPROM.put(0, EEP_Ident);
	//	EEPROM.put(10, steerSettings);
	//	EEPROM.put(40, steerConfig);
	//	EEPROM.put(60, networkAddress);
	//}
	//else
	//{
	//	EEPROM.get(10, steerSettings);     // read the Settings
	//	EEPROM.get(40, steerConfig);
	//	EEPROM.get(60, networkAddress);
	//}
}




