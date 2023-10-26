#include "Logger.h"


void Logger::LogMessage(String message, LogAreas logArea)
{
	//Serial.println("LoggingAreaOfInterest: " + String(LoggingAreaOfInterest) + " logArea: " + String(logArea));
	if (LoggingAreaOfInterest & logArea || logArea == LogAreas::General) {
		if (LoggingDestination & LogDestination::USB) {
			Serial.println(message);
		}
		if (LoggingDestination & LogDestination::UDP) {
			// fire this out over UDP
		}
	}
}
