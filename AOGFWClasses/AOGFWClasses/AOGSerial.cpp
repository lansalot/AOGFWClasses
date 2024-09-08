// 
// 
// 

#include "AOGSerial.h"


uint8_t AOGSerialClass::aogSerialCmd[4] = { '!', 'A', 'O', 'G' };
uint8_t AOGSerialClass::aogSerialCmdBuffer[6];
uint8_t AOGSerialClass::aogSerialCmdCounter = 0;
uint32_t AOGSerialClass::baudrates[9]
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
uint32_t AOGSerialClass::nrBaudrates = sizeof(baudrates) / sizeof(baudrates[0]);

void AOGSerialClass::init()
{
	//SerialGPS->addMemoryForRead(GPSrxbuffer, serial_buffer_size);
	//SerialGPS->addMemoryForWrite(GPStxbuffer, serial_buffer_size);

	//delay(10);
	//SerialRTK.begin(baudRTK);
	//SerialRTK.addMemoryForRead(RTKrxbuffer, serial_buffer_size);

	//delay(10);
	//SerialGPS2->begin(baudGPS);
	//SerialGPS2->addMemoryForRead(GPS2rxbuffer, serial_buffer_size);
	//SerialGPS2->addMemoryForWrite(GPS2txbuffer, serial_buffer_size);

}


AOGSerialClass AOGSerial;
