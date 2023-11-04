// 
// 
// 

#include "AOGSerial.h"

void AOGSerialClass::init()
{
	SerialGPS = &Serial7; //Main postion receiver (GGA) (Serial2 must be used here with T4.0 / Basic Panda boards - Should auto swap)
	SerialGPS = &Serial2; //Dual heading receiver 
}


AOGSerialClass AOGSerial;

