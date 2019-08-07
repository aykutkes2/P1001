#pragma once
#include <AY_Type.h>

#pragma pack(push, 1)
typedef struct _MYSQL_STRS {
	char	_ServerAdr[45];
	char	_ServerUser[45];
	char	_ServerPass[45];
	char	_ServerSchema[45];
	char	_UserList[45];
	char	_GatewayList[45];
}MYSQL_STRS;

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
#pragma pack(pop)

//--------------------------------------------------------------------------------------//
#define AY_NetworkSubnetMask		CngFile.NetworkSubnetMask
#define AY_NetworkGatewayIp			CngFile.NetworkGatewayIp
#define AY_NetSubnetIp				CngFile.NetSubnetIp


enum _MYSOCKETS {
	_MAIN_SCKT,
	_SLVS_SCKT
};

#define PACKET_TEST_DATA0			0x1EB99AB7
#define PACKET_TEST_DATA1			0x92D1D121

extern const MYSQL_STRS MySQLParams;

extern const Ui16 MyUDP_StartPort;
extern const Ui16 SERVER_Port;
extern const Ui08 SERVER_Dns[];
extern const Ui08 MyMac[6];
extern const Ui08 MyUnique[12];

/*
=========== Demo Configurations ================
Server IP				192.168.2.149	74-D4-35-3C-4A-B4		74-D4-35-3C-4A-B4-74-D4-35-3C-4A-B4
Client 1				192.168.2.145	74-D4-35-3C-4A-B2		74-D4-35-3C-4A-B2-74-D4-35-3C-4A-B2
Client 2				192.168.2.146	74-D4-35-3C-4A-B3		74-D4-35-3C-4A-B3-74-D4-35-3C-4A-B3
Device 1				192.168.2.147
Device 1 Ghost			192.168.2.148
WireShark filter : ip.addr==192.168.2.145 || ip.addr==192.168.2.146 || ip.addr==192.168.2.147 || ip.addr==192.168.2.148 || ip.addr==192.168.2.149
*/
extern const Ui16 DEMO_SRV_Port;
extern const Ui08 DEMO_SRV_IP[4];
extern const Ui08 DEMO_SRV_MAC[6];
extern const Ui08 DEMO_SRV_UNIQUE[12];
extern const Ui08 DEMO_Mac[6];
extern const Ui08 DNS_SRV_IP[4];
extern const Ui16 DNS_Port;

extern const Ui32 LOCAL_CertNo;
extern const Ui32 SERVER_CertNo;

extern const Ui08 * const CLIENT_PUB_KEY;
extern const Ui08 * const CLIENT_PR_KEY;
extern const Ui08 * const SERVER_PUB_KEY;
extern const Ui08 * const SERVER_PR_KEY;

extern const Ui08			DNS_SRV_IP[4];
extern const Ui16			DNS_Port;


extern const char TestData[65];
extern const Ui08 DemoAESkey[16];




