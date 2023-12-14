// 
// 
// 

#include "GPS.h"

HardwareSerial* SerialGPS = &Serial7; //Main postion receiver (GGA) (Serial2 must be used here with T4.0 / Basic Panda boards - Should auto swap)
HardwareSerial* SerialGPS2 = &Serial2; //Dual heading receiver 

bool blink = false;
bool passThroughGPS = false;
bool passThroughGPS2 = false;
bool GGA_Available = false;
bool useDual = false;
bool dualReadyGGA = false;
bool dualReadyRelPos = false;

uint8_t GPSrxbuffer[512];
uint8_t GPStxbuffer[512];
uint8_t GPS2rxbuffer[512];
uint8_t GPS2txbuffer[512];
uint8_t RTKrxbuffer[512];
uint32_t gpsReadyTime = 0;
uint32_t PortSwapTime = 0;


void GPSClass::initialize()
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

	Logger.LogMessage("Looking for GPS...", LoggerClass::LogCategories::GPS);

	// stall until GPS is receiving
	while (GGA_Available == false && !passThroughGPS && !passThroughGPS2)
	{
		if (systick_millis_count - PortSwapTime >= 10000)
		{
			Logger.LogMessage("Swapping GPS ports...", LoggerClass::LogCategories::GPS);
			swapSerial();
			PortSwapTime = systick_millis_count;
		}
		delay(500);
	}
	Logger.LogMessage("Found GPS - we ride!", LoggerClass::LogCategories::GPS);

}

void GPSClass::swapSerial() {
	HardwareSerial* temp = SerialGPS;
	SerialGPS = SerialGPS2;
	SerialGPS2 = temp;
}