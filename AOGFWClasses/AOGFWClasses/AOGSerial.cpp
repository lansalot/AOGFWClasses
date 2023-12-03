// 
// 
// 

#include "AOGSerial.h"


HardwareSerial* AOGSerialClass::SerialGPS = &Serial7; //Main postion receiver (GGA) (Serial2 must be used here with T4.0 / Basic Panda boards - Should auto swap)
HardwareSerial* AOGSerialClass::SerialGPS2 = &Serial2; //Dual heading receiver 


void AOGSerialClass::init()
{

	SerialGPS->addMemoryForRead(GPSrxbuffer, serial_buffer_size);
	SerialGPS->addMemoryForWrite(GPStxbuffer, serial_buffer_size);

	delay(10);
	SerialRTK.begin(baudRTK);
	SerialRTK.addMemoryForRead(RTKrxbuffer, serial_buffer_size);

	delay(10);
	SerialGPS2->begin(baudGPS);
	SerialGPS2->addMemoryForRead(GPS2rxbuffer, serial_buffer_size);
	SerialGPS2->addMemoryForWrite(GPS2txbuffer, serial_buffer_size);

}

void AOGSerialClass::swapSerial() {
	HardwareSerial* temp = SerialGPS;
	SerialGPS = SerialGPS2;
	SerialGPS2 = temp;
}

AOGSerialClass AOGSerial;

