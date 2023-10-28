#include "AOGEthernet.h"
#include "AOGStatus.h"

void AOGEthernetClass::EthernetStart()
{
	// start the Ethernet connection:
	Logger.LogMessage("Initializing ethernet with static IP address", moduleLogLevel);

	// try to congifure using IP:
	Ethernet.begin(mac, 0);          // Start Ethernet with IP 0.0.0.0

	// Check for Ethernet hardware present
	if (Ethernet.hardwareStatus() == EthernetNoHardware)
	{
		Logger.LogMessage("Ethernet shield was not found. GPS via USB only.",moduleLogLevel);

		return;
	}

	if (Ethernet.linkStatus() == LinkOFF)
	{
		Logger.LogMessage("Ethernet cable is not connected - Who cares we will start ethernet anyway.",moduleLogLevel);
	}

	//grab the ip from EEPROM
	Eth_myip[0] = networkAddress.ipOne;
	Eth_myip[1] = networkAddress.ipTwo;
	Eth_myip[2] = networkAddress.ipThree;
	if (AOGStatus.Autosteer_running)
	{
		Eth_myip[3] = 126;  //126 is steer module, with or without GPS
	}
	else
	{
		Eth_myip[3] = 120;  //120 is GPS only module
	}

	Ethernet.setLocalIP(Eth_myip);  // Change IP address to IP set by user
	Logger.LogMessage("Ethernet status OK - IP set Manually: " + String(Ethernet.localIP()),moduleLogLevel);

	Ethernet_running = true;

	Eth_ipDestination[0] = Eth_myip[0];
	Eth_ipDestination[1] = Eth_myip[1];
	Eth_ipDestination[2] = Eth_myip[2];
	Eth_ipDestination[3] = 255;

	// Sort this out... byte array to string
	Logger.LogMessage("Ethernet IP of module: " + String(Ethernet.localIP()), moduleLogLevel);
	Logger.LogMessage("Ethernet sending to IP: " + String(Eth_ipDestination),moduleLogLevel);
	Logger.LogMessage("All data sending to port: " + String(portDestination),moduleLogLevel);

	// init UPD Port sending to AOG
	if (Eth_udpPAOGI.begin(portMy))
	{
		Logger.LogMessage("Ethernet GPS UDP sending from port: " + String(portMy),moduleLogLevel);
	}

	// init UPD Port getting NTRIP from AOG
	if (Eth_udpNtrip.begin(AOGNtripPort)) // AOGNtripPort
	{
		Logger.LogMessage("Ethernet NTRIP UDP listening to port: " + String(AOGNtripPort),moduleLogLevel);
	}

	// init UPD Port getting AutoSteer data from AOG
	if (Eth_udpAutoSteer.begin(AOGAutoSteerPort)) // AOGAutoSteerPortipPort
	{
		Logger.LogMessage("Ethernet AutoSteer UDP listening to & send from port: " + String(AOGAutoSteerPort),moduleLogLevel);
	}
}



AOGEthernetClass AOGEthernet;

