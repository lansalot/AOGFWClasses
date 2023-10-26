// WASCANBUS.h

#ifndef _WASCANBUS_h
#define _WASCANBUS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "WAS.h"


class WASCANBUSClass : public WAS
{
 protected:


 public:
	void init() override;
};

extern WASCANBUSClass WASCANBUS;

#endif

