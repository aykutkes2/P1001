#pragma once
#include <AY_Type.h>

#pragma pack(push, 1)

enum _MYSOCKETS {
	_MAIN_SCKT,
	_SLVS_SCKT
};

#define PACKET_TEST_DATA0			0x1EB99AB7
#define PACKET_TEST_DATA1			0x92D1D121

typedef struct _CFG_FILES {
	char NetInterfaceName[45];	// 0	<!--exp: \"Local Area Network\", \"Wi-Fi\", \"Local Area Network 2\", \"\"(means Local Area Connection)-->
	Ui08 NetIpAddress[4];		// 1	<!--exp: \"192.168.2.101\", \"\"(means automatic)-->
	Ui08 NetworkSubnetMask[4];	// 2	<!--exp: \"255.255.255.0\", \"\"(means automatic)-->
	Ui08 NetworkGatewayIp[4];	// 3	<!--exp: \"192.168.2.1\", \"\"(means automatic)-->
	Ui08 NetSubnetIp[4];		// 4	<!--exp: \"192.168.2.0\", \"\"(means automatic)-->
	char GatewayName[45];		// 5	<!--exp: \"Gateway-1\", \"Office\", \"Home\", \"\"(means empty)-->
	char GatewayPass[45];		// 6	<!--exp: \"1234\", \"A1b4c32\", \"\"(means empty)-->
	Ui08 AllowAddRemove;		// 7	<!--exp: \"YES\", \"NO\", \"\"(means YES)-->
	Ui08 UniqueID[12];			// 8	<!--exp: \"112233445566778899AABBCC\", \"\"(means generate new)-->
	char ServerDns[45];			// 9	<!--exp: \"LocalAndSafe.com\" -->
	Ui16 ServerPort;			// 10	<!--exp: \"1982\" -->
	Ui08 DNSIp[4];				// 11	<!--exp: \"8.8.4.4\" -->
	Ui16 DNSPort;				// 12	<!--exp: \"53\" -->
	Ui32 ServerID;				// 13	<!--exp: \"1\", \"121\", \"\"(means 1 LocalAndSafe.com default server)-->
	char ServerPublicKey[2048]; // 14	< !--exp: -->
}CFG_FILES;

extern CFG_FILES	CngFile;
extern const Ui08 DefaultInterface[45];
extern const Ui08 DefaultServerDns[];
extern const Ui16 DefaultServerPort;
extern const Ui08 DefaultDNS_Ip[4];
extern const Ui16 DefaultDNS_Port;
extern const Ui16 DefaultServerID;

extern const Ui08 * const Default_Config_File;
extern const Ui08 * const Default_Cert_File;

extern Ui08 SIGNING_PUB_KEY[2048];
extern Ui08 SIGNING_PR_KEY[2048];
extern Ui32 Demo_CERT_NO;


extern const Ui16			MyUDP_StartPort;
extern const Ui08			MyMac[6];
extern const Ui08			DefaultMac[6];

//--------------------------------------------------------------------------------------//
extern const Ui08 * const SERVER_PUB_KEY;
extern const Ui08 * const SERVER_PR_KEY;
extern const Ui08 DEMO_CLNT_MAC[5][6];
extern const char TestData[65];
extern const Ui08 DemoAESkey[16];

extern const Ui08 * const DEMO_STR_1;
extern const Ui08 * const DEMO_STR_2;
//--------------------------------------------------------------------------------------//
#define AY_NetworkSubnetMask		CngFile.NetworkSubnetMask
#define AY_NetworkGatewayIp			CngFile.NetworkGatewayIp
#define AY_NetSubnetIp				CngFile.NetSubnetIp
#pragma pack(pop)



