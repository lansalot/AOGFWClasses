// 
// 
// 

#include "LED.h"

void LEDClass::init()
{
	pinMode(GGAReceivedLED, OUTPUT);
	pinMode(Power_on_LED, OUTPUT);
	pinMode(Ethernet_Active_LED, OUTPUT);
	pinMode(GPSRED_LED, OUTPUT);
	pinMode(GPSGREEN_LED, OUTPUT);
	pinMode(AUTOSTEER_STANDBY_LED, OUTPUT);
	pinMode(AUTOSTEER_ACTIVE_LED, OUTPUT);

}

void LEDClass::ledOn(LEDs led)
{
	digitalWrite(led, HIGH);

}

void LEDClass::ledOff(LEDs led)
{
	digitalWrite(led, LOW);

}

void LEDClass::ledBlink(LEDs led, boolean State)
{
	digitalWrite(led, State);
}
