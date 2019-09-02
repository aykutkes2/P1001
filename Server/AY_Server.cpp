//#include "pch.h"
#undef UNICODE

/*
=========== Demo Configurations ================
									|192.168.2.149|(SRV)
				|192.168.2.145|(GW1)						|192.168.2.146|(GW2)
				<|192.168.2.147|>(GHS1)							||192.168.2.148||(DEV1)
<<192.168.2.150>>

Server IP				192.168.2.149
Client 1				192.168.2.145	(1)		74-D4-35-3C-4A-B2		74-D4-35-3C-4A-B2-74-D4-35-3C-4A-B2
Client 2				192.168.2.146	(2)		74-D4-35-3C-4A-B3		74-D4-35-3C-4A-B3-74-D4-35-3C-4A-B3
Device 1 Ghost			192.168.2.147	()		(Client 2)
Device 1 				192.168.2.148	(4)		(Client 1)
Demo-1					192.168.2.150	(3)

PACKET (1)				DEMO1	--->	GHS1		(A request packet for Mirror Device)
PACKET (2)				GW1		--->	GW2			(Send request packet to GW includes real device)
PACKET (3)				GW2		--->	DEV1		(Send request packet to real device)
PACKET (4)				DEV1	--->	GW2			(A response packet from real Device)
PACKET (5)				GW2		--->	GW1			(Send response packet to GW includes mirror device)
PACKET (6)				GHS1	--->	DEMO1		(Send response packet to target device)

PACKET (A)				GW1		--->	SRV			(GW info request)
PACKET (B)				SRV		--->	GW2			(GW info rent)
PACKET (C)				SRV		--->	GW1			(GW info response)


*/
#include <stdio.h>
#include <time.h>
#include <windows.h>

#include <AY_Printf.h>
#include <AY_Functions.h>
#include <AY_Socket.h>
#include <AY_Server.h>
#include <AY_ServerPrjtBased.h>
#include <AY_Crypt.h>
#include <AY_Memory.h>
#include <AY_Queue.h>
#include <AY_ClientConn.h>

//#include<mysql.h>
#include <AY_MySql.h>

CFG_FILES				CngFile;
AY_GlobalRAM			AY_Ram;
//MySqlConnect			MySqlConn;
ip_address				MyIP_Address;
uip_eth_addr			MyEth_Address;
static Ui08				MyUnique_ID[12];
static Ui08				MySocketBuff[1536];
static Ui32				AY_ServerInitLoop;

//static udp_headerAll	UDPh;
//static Ui08				MySocketBuff2[1536];

struct pcap_pkthdr {
	struct timeval ts;	/* time stamp */
	Ui32 caplen;	/* length of portion present */
	Ui32 len;	/* length this packet (off wire) */
};

void AY_MainSocket_CallBack(Ui08 *param, const struct pcap_pkthdr *header, const Ui08 *pkt_data) {
	udp_headerAll	*pUDP;
	struct DNS_HEADER *dns = NULL;

	/* retireve the position of the udp all header */
	pUDP = (udp_headerAll *)(pkt_data + 0); // udp all header

	if (!AY_Server_GetMACadr) {

		printf("MAC Packet Received !!!\n\n\n===============================================\n\n\n===============================================\n\n\n");
		/* print ip addresses and udp ports */
		printf("%02x-%02x-%02x-%02x-%02x-%02x -> %02x-%02x-%02x-%02x-%02x-%02x\n\n\n\n",
			pUDP->_ethHeader.src.addr[0],
			pUDP->_ethHeader.src.addr[1],
			pUDP->_ethHeader.src.addr[2],
			pUDP->_ethHeader.src.addr[3],
			pUDP->_ethHeader.src.addr[4],
			pUDP->_ethHeader.src.addr[5],
			pUDP->_ethHeader.dest.addr[0],
			pUDP->_ethHeader.dest.addr[1],
			pUDP->_ethHeader.dest.addr[2],
			pUDP->_ethHeader.dest.addr[3],
			pUDP->_ethHeader.dest.addr[4],
			pUDP->_ethHeader.dest.addr[5]
		);
		if ((pUDP->_ipHeader.saddr.byte1 == MyIP_Address.byte1) && (pUDP->_ipHeader.saddr.byte2 == MyIP_Address.byte2) && (pUDP->_ipHeader.saddr.byte3 == MyIP_Address.byte3) && (pUDP->_ipHeader.saddr.byte4 == MyIP_Address.byte4)) {
			MyEth_Address = pUDP->_ethHeader.src;
		}
		else {
			MyEth_Address = pUDP->_ethHeader.dest;
		}
		AY_Server_GetMACadr = 1;
	}
	else if (AY_Server_Initied && AY_Server_GetMACadr) {
		int Len;
		if (UDP_packet_check((Ui08 *)pkt_data, &Len) == 1) {
			printf("New UDP packet arrived\n");
			if (AY_TestLoadDeviceStart((Ui08 *)pkt_data, Len) == 0) {
				printf("Not Device Start\n");
				if (AY_TestLoadGwInfoRqst((Ui08 *)pkt_data, Len) == 0) {
					printf("Not GW info Request\n");
				}
			}
		}
	}
}

int main(void) {
	int i, j = 0;
	char GetVal;

	AY_ServerInitLoop = 0;
	while (1) {
#if CLIENT_DEMO
		if (!AY_Server_Intro) {
			printf("============ CLIENT DEMO PROJECT =================\n\n");
			printf("My IP Address = %s \n\n", AY_ConvertIPAddToStrRet((Ui08 *)&DEMO_SRV_IP[0], (char *)&MySocketBuff[0]));
			AY_Server_Intro = 1;
			AY_Server_DynamicIP = 1;//sil !!!
		}
		else if (!AY_Server_Initied) {
			memcpy(&MyIP_Address.byte1, &DEMO_SRV_IP[0], 4);
			memcpy(&MyEth_Address.addr[0], &DEMO_SRV_MAC[0], 6);
			memcpy(&MyUnique_ID[0], &DEMO_SRV_UNIQUE[0], 12);

			strcpy((char *)&MySocketBuff[0], "udp dst port ");
			AY_ConvertUi32AddToStrRet(SERVER_Port, (char *)&MySocketBuff[0]);
			if (AYSCKT_Socket_Init(_MAIN_SCKT, (Ui08 *)&MyMac[0], &MyIP_Address.byte1, SERVER_Port, 0, AY_MainSocket_CallBack, AY_ServerInitLoop) == 1) {
				AY_Server_Initied = 1;
				AY_Server_GetMACadr = 1;
				AY_Server_WaitMACadr = 1;
			}
		}
#else
		if (!AY_Server_Intro) {
			printf("============ CLIENT PROJECT =================\n\n");
			printf("================ START =================\n\n");
			AY_Server_Intro = 1;
			AY_Server_DynamicIP = 1;//sil !!!
		}
		else if (!AY_Server_Initied) {
			MyIP_Address.byte1 = 0;	MyIP_Address.byte2 = 0;	MyIP_Address.byte3 = 0;	MyIP_Address.byte4 = 0;
			if (AYSCKT_Socket_Init(_MAIN_SCKT, (Ui08 *)&MyMac[0], &MyIP_Address.byte1, SERVER_Port, 0, AY_MainSocket_CallBack, AY_ServerInitLoop) == 1) {
				AY_Server_Initied = 1;
			}
		}
		else if (!AY_Server_GetMACadr) {
			if (!AY_Server_WaitMACadr) {
				//============= SET FILTER ==========================//
				AYSCKT_FilterFreeA(_MAIN_SCKT);
				strcpy((char *)&MySocketBuff[0], "ip src host ");
				AY_ConvertIPAddToStrRet((Ui08 *)&MyIP_Address.byte1, (char *)&MySocketBuff[0]);
				AYSCKT_FilterSetA(_MAIN_SCKT, (char *)&MySocketBuff[0]); 
				AY_Server_WaitMACadr = 1;
				j = 0;
			}
			else {
				if (++j < 10) {
					AY_Delay(1000);
				}
				else {
					AY_ServerInitLoop++;
					AYSCKT_FilterFreeB(_MAIN_SCKT);
					AY_Server_WaitMACadr = 0;
					AY_Server_Initied = 0;
				}
			}
		}
#endif
		else if (!AY_Server_SetFilter) {
			//============= SET FILTER ==========================//
			AYSCKT_FilterFreeA(_MAIN_SCKT);
			strcpy((char *)&MySocketBuff[0], "udp dst port ");
			AY_ConvertUi32AddToStrRet(SERVER_Port, (char *)&MySocketBuff[0]);
			strcat((char *)&MySocketBuff[0], " and ip dst host ");
			AY_ConvertIPAddToStrRet((Ui08 *)&MyIP_Address.byte1, (char *)&MySocketBuff[0]);
			AYSCKT_FilterSetA(_MAIN_SCKT, (char *)&MySocketBuff[0]);
			AY_Server_SetFilter = 1;
		}
		else if (!AY_Server_StartSqlConn) {
			MYSQL_Init();
			strcpy(MYSQL_Table, MySQLParams._GatewayList);
			strcpy(MYSQL_UserList, MySQLParams._UserList);
			MYSQL_Connect((char *)MySQLParams._ServerAdr, (char *)MySQLParams._ServerUser, (char *)MySQLParams._ServerPass, (char *)MySQLParams._ServerSchema);
			AYCONN_ClientConnInit();
			AYSRV_QueueInit();
			AY_Server_StartSqlConn = 1;
		}
		else {
			if (++j < 2) {
				AY_Delay(1000);
			}
			else {
				//----------test sil !!!!
				//AYSRV_TestMySql();
				//...
			}
		}
		Sleep(1);
	}

	return 0;
}
