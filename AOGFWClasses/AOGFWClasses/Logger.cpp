#include "Logger.h"



void LoggerClass::LogMessage(String message, LogCategories logCategories)
{
	//Serial.println("LoggingAreaOfInterest: " + String(LoggingAreaOfInterest) + " logCategories: " + String(logArea));
	if (LoggingInterest & logCategories || logCategories == LogCategories::General) {
		if (LoggingDestination & LogDestination::USB) {
			Serial.println(message);
		}
		if (LoggingDestination & LogDestination::UDP) {
			// fire this out over UDP
		}
	}
}



LoggerClass Logger;