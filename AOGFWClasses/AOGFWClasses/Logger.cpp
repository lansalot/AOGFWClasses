// 
// 
// 

#include "Logger.h"


void Logger::LogMessage(String message, LogAreas logArea)
{
	if (LoggingAreaOfInterest & logArea) {
		if (LoggingDestination & LogDestination::USB) {
			Serial.println(message);
		}
		if (LoggingDestination & LogDestination::UDP) {
			// fire this out over UDP
		}
	}
}
