
#include "../Logger.h"
#include "BNO_rvc.h"

//set the object variables
HardwareSerial* bnoSerial = &Serial5; // AIO 4.5


bool BNO_rvc::initialize(IMUClass::rollState rollState, IMUClass::imuAxisState imuAxisState) {
	bnoSerial = &Serial5;  // extra_p26 sometimes on the schematic
	bnoSerial->begin(115200);
	_rollState = rollState;
	_imuAxisState = imuAxisState;
	return true;
}



bool BNO_rvc::read() {
	//if (!imuData) {
	//	//Serial.println("no data");
	//	return false;
	//}

	if (!bnoSerial->available()) return false;
	//Serial.print(" 0x");
	//// print byte in hex
	//Serial.print(bnoSerial->read(),HEX);

	//Serial.println("Available: " + String(bnoSerial->available()));
	if (bnoSerial->peek() != 0xAA)
	{
		bnoSerial->read();
		return false;
	}

	// Now read all 19 bytes
	if (bnoSerial->available() < 19) return false;

	// at this point we know there's at least 19 bytes available and the first is AA
	if (bnoSerial->read() != 0xAA) return false;

	// make sure the next byte is the second 0xAA
	if (bnoSerial->read() != 0xAA) return false;

	uint8_t buffer[19];
	if (!bnoSerial->readBytes(buffer, 17)) return false;


	// get checksum ready
	uint8_t sum = 0;
	for (uint8_t i = 0; i < 16; i++) sum += buffer[i];

	if (sum != buffer[16]) {
		Serial.println("bad crc");
		return false;
	}

	//clean out any remaining bytes in case teensy was busy
	//while (bnoSerial->available() > 0) bnoSerial->read();

	int16_t temp[6];
	//read the 16 byte sentence AA AA Index Yaw Pitch Roll LSB MSB

	for (uint8_t i = 0; i < 6; i++) {

		temp[i] = (buffer[1 + (i * 2)]);
		temp[i] += (buffer[1 + (i * 2) + 1] << 8);
	}

	// The data comes in endian'd, this solves it so it works on all platforms
	int16_t buffer_16[6];

	for (uint8_t i = 0; i < 6; i++) {

		buffer_16[i] = (buffer[1 + (i * 2)]);
		buffer_16[i] += (buffer[1 + (i * 2) + 1] << 8);
	}
	imuData.yaw = (float)buffer_16[0] * DEGREE_SCALE;
	imuData.pitch = (float)buffer_16[1] * DEGREE_SCALE;
	imuData.roll = (float)buffer_16[2] * DEGREE_SCALE;
	imuData.x_accel = (float)buffer_16[3] * MILLI_G_TO_MS2;
	imuData.y_accel = (float)buffer_16[4] * MILLI_G_TO_MS2;
	imuData.z_accel = (float)buffer_16[5] * MILLI_G_TO_MS2;

	//Serial.print("updated!");
	//if (angCounter < 20)
	//{
	//	imuData->yawX100 = temp; //For angular velocity calc
	//	imuData->angVel += (temp - prevYaw);
	//	angCounter++;
	//	prevYaw = temp;
	//}
	//else
	//{
	//	angCounter = 0;
	//	prevYaw = imuData->angVel = 0;
	//}

	//imuData->yawX10 = (int16_t)((float)temp * DEGREE_SCALE);
	//if (imuData->yawX10 < 0) imuData->yawX10 += 3600;
	//temp = buffer[3] + (buffer[4] << 8);
	//imuData->pitchX10 = (int16_t)((float)temp * DEGREE_SCALE);

	//temp = buffer[5] + (buffer[6] << 8);
	//imuData->rollX10 = (int16_t)((float)temp * DEGREE_SCALE);

	//imuData.yaw = (float)imuData->yawX10 / 10.0;
	//imuData.pitch = (float)imuData->pitchX10 / 10.0;
	//imuData.roll = (float)imuData->rollX10 / 10.0;

	return true;
}

