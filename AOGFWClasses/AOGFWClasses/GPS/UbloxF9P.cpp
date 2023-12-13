// 
// 
// 

#include "UbloxF9P.h"
#include "GPS.h"
#include "..\\LED.h"


// parser << SerialGPS->read();  << is overloaded, it feeds the parser

static NMEAParser<2> parser;
static char vtgHeading[12];
static char speedKnots[10];


void UbloxF9P::initialize() {
	GPSClass::initialize();
	// merge zhandlers and znmeaParser
	// no, better to build a generic handler for znmeaparser
	// and just stuff it with data from $GPS?

	parser.setErrorHandler(errorHandler);
	//parser.addHandler("G-GGA", GGA_Handler);
	parser.addHandler("G-VTG", VTG_Handler);
	Logger.LogMessage("F9P initialized", moduleLogLevel);
}

void UbloxF9P::VTG_Handler()
{

	// vtg heading
	parser.getArg(0, vtgHeading);

	// vtg Speed knots
	parser.getArg(4, speedKnots);

}

void UbloxF9P::errorHandler()
{
	//nothing at the moment
}

void UbloxF9P::ReadSerialPosition()
{
	if (SerialGPS->available())
	{
		if (GPSClass::passThroughGPS)
		{
			SerialAOG.write(SerialGPS->read());
		}
		else
		{
			parser << SerialGPS->read();
		}
	}
}

void UbloxF9P::GGA_Handler()
 //Rec'd GGA
{

	// do you need to move the vtg_handler into the znmeaparser perhaps??

	// fix time
	parser.getArg(0, GPSClass::fixTime);

	// latitude
	parser.getArg(1, GPSClass::latitude);
	parser.getArg(2, GPSClass::latNS);

	// longitude
	parser.getArg(3, GPSClass::longitude);
	parser.getArg(4, GPSClass::lonEW);

	// fix quality
	parser.getArg(5, GPSClass::fixQuality);

	// satellite #
	parser.getArg(6, GPSClass::numSats);

	// HDOP
	parser.getArg(7, GPSClass::HDOP);

	// altitude
	parser.getArg(8, GPSClass::altitude);

	// time of last DGPS update
	parser.getArg(12, GPSClass::ageDGPS);
#ifndef CANCOMPILE
	if (blink)
	{
		digitalWrite(LEDClass::GGAReceivedLED, HIGH);
	}
	else
	{
		digitalWrite(LEDClass::GGAReceivedLED, LOW);
	}
	blink = !blink;
	GPSClass::GGA_Available = true;

	if (useDual)
	{
		dualReadyGGA = true;
	}

	//if (useBNO08x || useCMPS)
	{
		//imuHandler();          //Get IMU data ready
		//BuildNmea();           //Build & send data GPS data to AgIO (Both Dual & Single)
		dualReadyGGA = false;  //Force dual GGA ready false because we just sent it to AgIO based off the IMU data
		if (!useDual)
		{
			led.ledOn(LEDClass::GPSRED_LED);
			led.ledOff(LEDClass::GPSGREEN_LED);
		}
	}
	//else if (!useBNO08x && !useCMPS && !useDual)
	{
		led.ledBlink(LEDClass::GPSRED_LED, blink);   //Flash red GPS LED, we have GGA but no IMU or dual
		led.ledOff(LEDClass::GPSGREEN_LED);   //Make sure the Green LED is OFF
		itoa(65535, imuHeading, 10);       //65535 is max value to stop AgOpen using IMU in Panda
		//BuildNmea();
	}

	GPSClass::gpsReadyTime = systick_millis_count;    //Used for GGA timeout (LED's ETC) 
#endif
}





