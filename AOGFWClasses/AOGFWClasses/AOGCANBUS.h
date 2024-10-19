// AOGCANBUS.h

#ifndef _AOGCANBUS_h
#define _AOGCANBUS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <FlexCAN_T4.h>
class AOGCANBUSClass
{
 protected:

 public:
	 // CRX1/CTX1 on Teensy are CAN3 on Tony's board
	 // CRX2/CTX2 on Teensy are CAN2 on AIO board, CAN2 on Tony's board
	 // CRX3/CTX3 on Teensy are CAN1 on AIO board, CAN3 on Tony's board
	 FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_256> CANBUS1;
	 FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_256> CANBUS2;
	 FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_256> CANBUS3;
	void init();
};

extern AOGCANBUSClass AOGCANBUS;

#endif

