// LED.h

#ifndef _LED_h
#define _LED_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class LEDClass
{
protected:


public:

#define	AIOBoard
#ifdef AIOBoard
	enum LEDs
	{
		GGAReceivedLED = 13,         //Teensy onboard LED
		Power_on_LED = 5,            //Red
		Ethernet_Active_LED = 6,     //Green
		GPSRED_LED = 9,              //Red (Flashing = NO IMU or Dual, ON = GPS fix with IMU)
		GPSGREEN_LED = 10,           //Green (Flashing = Dual bad, ON = Dual good)
		AUTOSTEER_STANDBY_LED = 11,  //Red
		AUTOSTEER_ACTIVE_LED = 12,   //Green
	};
#endif

	void init();
	void ledOn(LEDs led);
	void ledOff(LEDs led);

};


#endif

