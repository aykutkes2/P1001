#pragma once
#include <AY_Type.h>

#pragma pack(push, 8)

enum _MYSOCKETS {
	_MAIN_SCKT,
	_SLVS_SCKT
};

#define PACKET_TEST_DATA0			0x1EB99AB7
#define PACKET_TEST_DATA1			0x92D1D121

typedef struct _CFG_FILES {
	char NetInterfaceName[45];	// <!--exp: \"Local Area Network\", \"Wi-Fi\", \"Local Area Network 2\", \"\"(means Local Area Connection)-->
	Ui08 NetIpAddress[4];		// <!--exp: \"192.168.2.101\", \"\"(means automatic)-->
	Ui08 NetworkSubnetMask[4];	// <!--exp: \"255.255.255.0\", \"\"(means automatic)-->
	Ui08 NetworkGatewayIp[4];	// <!--exp: \"192.168.2.1\", \"\"(means automatic)-->
	Ui08 NetSubnetIp[4];		// <!--exp: \"192.168.2.0\", \"\"(means automatic)-->
	char GatewayName[45];		// <!--exp: \"Gateway-1\", \"Office\", \"Home\", \"\"(means empty)-->
	char GatewayPass[45];		// <!--exp: \"1234\", \"A1b4c32\", \"\"(means empty)-->
	Ui08 AllowAddRemove;		// <!--exp: \"YES\", \"NO\", \"\"(means YES)-->
	Ui08 UniqueID[12];			// <!--exp: \"112233445566778899AABBCC\", \"\"(means generate new)-->
	char ServerDns[45];			// <!--exp: \"LocalAndSafe.com\" -->
	Ui16 ServerPort;			// <!--exp: \"1982\" -->
	Ui08 DNSIp[4];				// <!--exp: \"8.8.4.4\" -->
	Ui16 DNSPort;				// <!--exp: \"53\" -->
	Ui32 ServerID;				// <!--exp: \"1\", \"121\", \"\"(means 1 LocalAndSafe.com default server)-->
	char ServerPublicKey[2048]; // < !--exp: -->
}CFG_FILES;

extern CFG_FILES	CngFile;
extern const Ui08 DefaultInterface[45];
extern const Ui08 DefaultDns[];
extern const Ui16 DefaultDNSPort;

extern const Ui08 * const Default_Config_File;
extern const Ui08 * const Default_Cert_File;

extern Ui08 SIGNING_PUB_KEY[2048];
extern Ui08 SIGNING_PR_KEY[2048];
extern Ui08 SERVER_PUB_KEY[2048];
extern Ui32 SERVER_NO;


















extern const Ui08 DEMO_SRV_IP[4];
extern const Ui08 DEMO_CLNT_IP[2][4];
extern const Ui08 DEMO_CLNT_MAC[2][6];
extern const Ui08 DEMO_CLNT_UNIQUE[2][12];
extern const Ui08 DEMO_Mac[6];
extern const Ui16 DEMO_SRV_Port;

extern const Ui16			SERVER_Port;
extern const Ui08			SERVER_Dns[40];
extern const Ui16			MyUDP_StartPort;
extern const Ui08			MyMac[6];
extern const Ui08			MyUnique[12];
extern const Ui08			MyInterface[45];

extern const Ui32			LOCAL_CertNo;
extern const Ui32			SERVER_CertNo;

extern const Ui08 MY_NAME[45];
extern const Ui08 MY_PASSWORD[45];


//extern const Ui08 * const SIGNING_PUB_KEY;
//extern const Ui08 * const SIGNING_PR_KEY;
//extern const Ui08 * const SERVER_PUB_KEY;
//extern const Ui08 * const SERVER_PR_KEY;

//extern const Ui08			MyPublicKey[256];
//extern const Ui08			MyPrivateKey[256];
//extern const Ui08			MyRSA_CAR[8];
//extern const Ui08			MyRSA_Exp[8];
//extern const RSA2048		MyRSA2048_Cert;

extern const Ui08			DNS_SRV_IP[4];
extern const Ui16			DNS_Port;


extern const char TestData[65];
extern const Ui08 DemoAESkey[16];

#pragma pack(pop)



