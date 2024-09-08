// 
// 
// 

#include "GPS.h"
#include "..\\AOGSerial.h"

HardwareSerial* GPSClass::SerialGPS = &Serial7; //Main postion receiver (GGA) (Serial2 must be used here with T4.0 / Basic Panda boards - Should auto swap)
HardwareSerial* GPSClass::SerialGPS2 = &Serial2; //Dual heading receiver 

bool blink = false;
bool GPSClass::passThroughGPS = false;
bool GPSClass::passThroughGPS2 = false;
bool GGA_Available = false;
bool useDual = false;
bool dualReadyGGA = false;
bool dualReadyRelPos = false;

uint8_t GPSrxbuffer[512];
uint8_t GPStxbuffer[512];
uint8_t GPS2rxbuffer[512];
uint8_t GPS2txbuffer[512];
uint8_t RTKrxbuffer[512];
uint32_t gpsReadyTime = 0;
uint32_t PortSwapTime = 0;

void GPSClass::initialize()
{

	SerialGPS->addMemoryForRead(GPSrxbuffer, serial_buffer_size);
	SerialGPS->addMemoryForWrite(GPStxbuffer, serial_buffer_size);

	delay(10);
	SerialRTK.begin(baudRTK);
	SerialRTK.addMemoryForRead(RTKrxbuffer, serial_buffer_size);

	delay(10);
	SerialGPS2->begin(baudGPS);
	SerialGPS2->addMemoryForRead(GPS2rxbuffer, serial_buffer_size);
	SerialGPS2->addMemoryForWrite(GPS2txbuffer, serial_buffer_size);

	Logger.LogMessage("Looking for GPS...", LoggerClass::LogCategories::GPS);

	// stall until GPS is receiving - NOTE, this is in LOOP in original, but surely no need to swap past init?
	while (GGA_Available == false && !GPSClass::passThroughGPS && !GPSClass::passThroughGPS2)
	{
		if (systick_millis_count - PortSwapTime >= 10000)
		{
			Logger.LogMessage("Swapping GPS ports...", LoggerClass::LogCategories::GPS);
			swapSerial();
			PortSwapTime = systick_millis_count;
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



void GPSClass::parseRTK() {
	if (SerialAOG.available())
	{
		uint8_t incoming_char = SerialAOG.read();

		// Check incoming char against the aogSerialCmd array
		// The configuration utility will send !AOGR1, !AOGR2 or !AOGED (close/end)
		if (AOGSerialClass::aogSerialCmdCounter < 4 && AOGSerialClass::aogSerialCmd[AOGSerialClass::aogSerialCmdCounter] == incoming_char)
		{
			AOGSerialClass::aogSerialCmdBuffer[AOGSerialClass::aogSerialCmdCounter] = incoming_char;
			AOGSerialClass::aogSerialCmdCounter++;
		}
		// Whole command prefix is in, handle it
		else if (AOGSerialClass::aogSerialCmdCounter == 4)
		{
			AOGSerialClass::aogSerialCmdBuffer[AOGSerialClass::aogSerialCmdCounter] = incoming_char;
			AOGSerialClass::aogSerialCmdBuffer[AOGSerialClass::aogSerialCmdCounter + 1] = SerialAOG.read();

			if (AOGSerialClass::aogSerialCmdBuffer[AOGSerialClass::aogSerialCmdCounter] == 'R')
			{
				HardwareSerial* autoBaudSerial = NULL;

				// Reset SerialGPS and SerialGPS2
				SerialGPS = &Serial7;
				SerialGPS2 = &Serial2;

				if (AOGSerialClass::aogSerialCmdBuffer[AOGSerialClass::aogSerialCmdCounter + 1] == '1')
				{
					GPSClass::passThroughGPS = true;
					GPSClass::passThroughGPS2 = false;
					autoBaudSerial = SerialGPS;
				}
				else if (AOGSerialClass::aogSerialCmdBuffer[AOGSerialClass::aogSerialCmdCounter + 1] == '2')
				{
					GPSClass::passThroughGPS = false;
					GPSClass::passThroughGPS2 = true;
					autoBaudSerial = SerialGPS2;
				}

				const uint8_t UBX_SYNCH_1 = 0xB5;
				const uint8_t UBX_SYNCH_2 = 0x62;
				const uint8_t UBX_CLASS_ACK = 0x05;
				const uint8_t UBX_CLASS_CFG = 0x06;
				const uint8_t UBX_CFG_RATE = 0x08;

				ubxPacket packetCfg{};

				packetCfg.cls = UBX_CLASS_CFG;
				packetCfg.id = UBX_CFG_RATE;
				packetCfg.len = 0;
				packetCfg.startingSpot = 0;

				calcChecksum(&packetCfg);

				byte mon_rate[] = { 0xB5, 0x62, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
				mon_rate[2] = packetCfg.cls;
				mon_rate[3] = packetCfg.id;
				mon_rate[4] = packetCfg.len & 0xFF;
				mon_rate[5] = packetCfg.len >> 8;
				mon_rate[6] = packetCfg.checksumA;
				mon_rate[7] = packetCfg.checksumB;

				// Check baudrate
				bool communicationSuccessfull = false;
				uint32_t baudrate = 0;

				for (uint32_t i = 0; i < AOGSerialClass::nrBaudrates; i++)
				{
					baudrate = AOGSerialClass::baudrates[i];

					Serial.print(F("Checking baudrate: "));
					Serial.println(baudrate);

					autoBaudSerial->begin(baudrate);
					delay(100);

					// first send dumb data to make sure its on
					autoBaudSerial->write(0xFF);

					// Clear
					while (autoBaudSerial->available() > 0)
					{
						autoBaudSerial->read();
					}

					// Send request
					autoBaudSerial->write(mon_rate, 8);

					uint32_t millis_read = systick_millis_count;
					constexpr uint32_t UART_TIMEOUT = 1000;
					int ubxFrameCounter = 0;
					bool isUbx = false;
					uint8_t incoming = 0;

					uint8_t requestedClass = packetCfg.cls;
					uint8_t requestedID = packetCfg.id;

					uint8_t packetBufCls = 0;
					uint8_t packetBufId = 0;

					do
					{
						while (autoBaudSerial->available() > 0)
						{
							incoming = autoBaudSerial->read();

							if (!isUbx && incoming == UBX_SYNCH_1) // UBX binary frames start with 0xB5, aka
							{
								ubxFrameCounter = 0;
								isUbx = true;
							}

							if (isUbx)
							{
								// Decide what type of response this is
								if ((ubxFrameCounter == 0) && (incoming != UBX_SYNCH_1))      // ISO
								{
									isUbx = false;                                            // Something went wrong. Reset.
								}
								else if ((ubxFrameCounter == 1) && (incoming != UBX_SYNCH_2)) // ASCII 'b'
								{
									isUbx = false;                                            // Something went wrong. Reset.
								}
								else if (ubxFrameCounter == 1 && incoming == UBX_SYNCH_2)
								{
									// Serial.println("UBX_SYNCH_2");
									// isUbx should be still true
								}
								else if (ubxFrameCounter == 2) // Class
								{
									// Record the class in packetBuf until we know what to do with it
									packetBufCls = incoming; // (Duplication)
								}
								else if (ubxFrameCounter == 3) // ID
								{
									// Record the ID in packetBuf until we know what to do with it
									packetBufId = incoming; // (Duplication)

									// We can now identify the type of response
									// If the packet we are receiving is not an ACK then check for a class and ID match
									if (packetBufCls != UBX_CLASS_ACK)
									{
										// This is not an ACK so check for a class and ID match
										if ((packetBufCls == requestedClass) && (packetBufId == requestedID))
										{
											// This is not an ACK and we have a class and ID match
											communicationSuccessfull = true;
										}
										else
										{
											// This is not an ACK and we do not have a class and ID match
											// so we should keep diverting data into packetBuf and ignore the payload
											isUbx = false;
										}
									}
								}
							}

							// Finally, increment the frame counter
							ubxFrameCounter++;
						}
					} while (systick_millis_count - millis_read < UART_TIMEOUT);

					if (communicationSuccessfull)
					{
						break;
					}
				}

				if (communicationSuccessfull)
				{
					SerialAOG.write(AOGSerialClass::aogSerialCmdBuffer, 6);
					SerialAOG.print(F("Found reciever at baudrate: "));
					SerialAOG.println(baudrate);

					// Let the configuring program know it can proceed
					SerialAOG.println("!AOGOK");
				}
				else
				{
					SerialAOG.println(F("u-blox GNSS not detected. Please check wiring."));
				}

				AOGSerialClass::aogSerialCmdCounter = 0;
			}
			// END command. maybe think of a different abbreviation
			else if (AOGSerialClass::aogSerialCmdBuffer[AOGSerialClass::aogSerialCmdCounter] == 'E' && AOGSerialClass::AOGSerialClass::aogSerialCmdBuffer[AOGSerialClass::aogSerialCmdCounter + 1] == 'D')
			{
				GPSClass::passThroughGPS = false;
				GPSClass::passThroughGPS2 = false;
				AOGSerialClass::aogSerialCmdCounter = 0;
			}
		}
		else
		{
			AOGSerialClass::aogSerialCmdCounter = 0;
		}

		if (GPSClass::passThroughGPS)
		{
			SerialGPS->write(incoming_char);
		}
		else if (GPSClass::passThroughGPS2)
		{
			SerialGPS2->write(incoming_char);
		}
		else
		{
			SerialGPS->write(incoming_char);
		}
	}
}

void GPSClass::calcChecksum(ubxPacket* msg)
{
	msg->checksumA = 0;
	msg->checksumB = 0;

	msg->checksumA += msg->cls;
	msg->checksumB += msg->checksumA;

	msg->checksumA += msg->id;
	msg->checksumB += msg->checksumA;

	msg->checksumA += (msg->len & 0xFF);
	msg->checksumB += msg->checksumA;

	msg->checksumA += (msg->len >> 8);
	msg->checksumB += msg->checksumA;

	for (uint16_t i = 0; i < msg->len; i++)
	{
		msg->checksumA += msg->payload[i];
		msg->checksumB += msg->checksumA;
	}
}
