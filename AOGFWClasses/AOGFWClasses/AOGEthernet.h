// Ethernet.h

#ifndef _AOGETHERNET_h
#define _AOGETHERNET_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <NativeEthernet.h>
#include <NativeEthernetUdp.h>
#include "Logger.h"


class AOGEthernetClass
{
protected:
	LoggerClass::LogCategories moduleLogLevel = LoggerClass::LogCategories::General;
	byte Eth_myip[4] = { 0, 0, 0, 0 }; //This is now set via AgIO
	byte mac[6] = { 0x00, 0x00, 0x56, 0x00, 0x00, 0x78 };

	struct ConfigIP {
		uint8_t ipOne = 192;
		uint8_t ipTwo = 168;
		uint8_t ipThree = 5;
	};  ConfigIP networkAddress;   //3 bytes

	unsigned int portMy = 5120;             // port of this module
	unsigned int AOGNtripPort = 2233;       // port NTRIP data from AOG comes in
	unsigned int AOGAutoSteerPort = 8888;   // port Autosteer data from AOG comes in
	unsigned int portDestination = 9999;    // Port of AOG that listens
	char Eth_NTRIP_packetBuffer[512];       // buffer for receiving ntrip data

	// An EthernetUDP instance to let us send and receive packets over UDP
	EthernetUDP Eth_udpPAOGI;     //Out port 5544
	EthernetUDP Eth_udpNtrip;     //In port 2233
	EthernetUDP Eth_udpAutoSteer; //In & Out Port 8888

	IPAddress Eth_ipDestination;


public:
	bool Ethernet_running = false; //Auto set on in ethernet setup
	void EthernetStart();
};

extern AOGEthernetClass AOGEthernet;

#endif

