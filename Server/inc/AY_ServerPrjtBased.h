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
/*
4c1593a321e88eec4c66a143
26b6f3236c6de567b68e3d46c7a88452
87361ce2f003b86ea88c6ae49690a319
b13d96b5c9b9e958882d8501ebb4aae5
1cc7fbbeef5debc47a5d720aada25fdf
*/
#define PACKET_TEST_DATA0			0x1EB99AB7		///< GW Device List 
#define PACKET_TEST_DATA1			0x92D1D121
#define PACKET_TEST_DATA2			0x27c0362f		///< GW Info Request 
#define PACKET_TEST_DATA3			0x64879a38
#define PACKET_TEST_DATA4			0xb64dcdb4		///< GW Port Rent
#define PACKET_TEST_DATA5			0x77f7bd3c
#define PACKET_TEST_DATA6			0xb8afb28d		///< New Remote Packet
#define PACKET_TEST_DATA7			0x3ab96c05
#define PACKET_TEST_DATA8			0x54675c92		///< New Remote Response
#define PACKET_TEST_DATA9			0x873ed73d
#define PACKET_TEST_DATA10			0x5c7980f1		///< New Direct Send Data		GW1 - SRV
#define PACKET_TEST_DATA11			0xd822673e		///< New Direct Send Data		SRV - GW2
#define PACKET_TEST_DATA12			0xd49c9668		///< New Direct Response Data	GW2 - SRV		
#define PACKET_TEST_DATA13			0x9aca50a9		///< New Direct Response Data	SRV - GW1
#define PACKET_TEST_DATA14			0x7243bed4		///< M2M New Direct Send Data		GW1 - SRV
#define PACKET_TEST_DATA15			0x92000000		///< M2M New Direct Send Data		SRV - GW2
#define PACKET_TEST_DATA16			0xda9a4971		///< M2M New Direct Response Data	GW2 - SRV
#define PACKET_TEST_DATA17			0x8f000000		///< M2M New Direct Send Data		GW1 - SRV
#define PACKET_TEST_DATA18			0x9296b90b		///< G2G New Direct Send Data		GW1 - SRV
#define PACKET_TEST_DATA19			0xfb8199e1		///< G2G New Direct Send Data		SRV - GW2
#define PACKET_TEST_DATA20			0xa7e6f030		///< G2G New Direct Response Data	GW2 - SRV
#define PACKET_TEST_DATA21			0x6edd1823		///< G2G New Direct Send Data		GW1 - SRV


extern const MYSQL_STRS MySQLParams;

extern const Ui16 MyUDP_StartPort;
extern const Ui16 SERVER_Port;
extern const Ui08 SERVER_Dns[];
extern const Ui08 MyMac[6];
extern const Ui08 DefaultMac[6];
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




