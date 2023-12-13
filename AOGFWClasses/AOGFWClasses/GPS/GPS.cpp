// 
// 
// 

#include "GPS.h"

HardwareSerial* GPSClass::SerialGPS = &Serial7; //Main postion receiver (GGA) (Serial2 must be used here with T4.0 / Basic Panda boards - Should auto swap)
HardwareSerial* GPSClass::SerialGPS2 = &Serial2; //Dual heading receiver 

bool GPSClass::blink = false;
bool GPSClass::passThroughGPS = false;
bool GPSClass::passThroughGPS2 = false;
bool GPSClass::GGA_Available = false;
bool GPSClass::useDual = false;
bool GPSClass::dualReadyGGA = false;
bool GPSClass::dualReadyRelPos = false;

uint8_t GPSClass::GPSrxbuffer[512];
uint8_t GPSClass::GPStxbuffer[512];
uint8_t GPSClass::GPS2rxbuffer[512];
uint8_t GPSClass::GPS2txbuffer[512];
uint8_t GPSClass::RTKrxbuffer[512];
uint32_t GPSClass::gpsReadyTime = 0;
uint32_t GPSClass::PortSwapTime = 0;


void GPSClass::initialize()
{

	GPSClass::SerialGPS->addMemoryForRead(GPSClass::GPSrxbuffer, GPSClass::serial_buffer_size);
	GPSClass::SerialGPS->addMemoryForWrite(GPSClass::GPStxbuffer, GPSClass::serial_buffer_size);

	delay(10);
	SerialRTK.begin(GPSClass::baudRTK);
	SerialRTK.addMemoryForRead(GPSClass::RTKrxbuffer, GPSClass::serial_buffer_size);

	delay(10);
	GPSClass::SerialGPS2->begin(GPSClass::baudGPS);
	GPSClass::SerialGPS2->addMemoryForRead(GPSClass::GPS2rxbuffer, GPSClass::serial_buffer_size);
	GPSClass::SerialGPS2->addMemoryForWrite(GPSClass::GPS2txbuffer, GPSClass::serial_buffer_size);

	Logger.LogMessage("Looking for GPS...", LoggerClass::LogCategories::GPS);

	// stall until GPS is receiving
	while (GPSClass::GGA_Available == false && !GPSClass::passThroughGPS && !GPSClass::passThroughGPS2)
	{
		if (systick_millis_count - GPSClass::PortSwapTime >= 10000)
		{
			Logger.LogMessage("Swapping GPS ports...", LoggerClass::LogCategories::GPS);
			GPSClass::swapSerial();
			GPSClass::PortSwapTime = systick_millis_count;
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