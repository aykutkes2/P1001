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

//static tcp_headerAll	TCPh;
//static Ui08				MySocketBuff2[1536];

int AY_ServerSendStatus(tcp_headerAll *pTCP, Ui08 flgs);

struct pcap_pkthdr {
	struct timeval ts;	/* time stamp */
	Ui32 caplen;	/* length of portion present */
	Ui32 len;	/* length this packet (off wire) */
};

void AY_MainSocket_CallBack(Ui08 *param, const struct pcap_pkthdr *header, const Ui08 *pkt_data) {
	tcp_headerAll	*pTCP;
	struct DNS_HEADER *dns = NULL;

	/* retireve the position of the tcp all header */
	pTCP = (tcp_headerAll *)(pkt_data + 0); // tcp all header

	if (!AY_Server_GetMACadr) {

		printf("MAC Packet Received !!!\n\n\n===============================================\n\n\n===============================================\n\n\n");
		/* print ip addresses and tcp ports */
		printf("%02x-%02x-%02x-%02x-%02x-%02x -> %02x-%02x-%02x-%02x-%02x-%02x\n\n\n\n",
			pTCP->_ethHeader.src.addr[0],
			pTCP->_ethHeader.src.addr[1],
			pTCP->_ethHeader.src.addr[2],
			pTCP->_ethHeader.src.addr[3],
			pTCP->_ethHeader.src.addr[4],
			pTCP->_ethHeader.src.addr[5],
			pTCP->_ethHeader.dest.addr[0],
			pTCP->_ethHeader.dest.addr[1],
			pTCP->_ethHeader.dest.addr[2],
			pTCP->_ethHeader.dest.addr[3],
			pTCP->_ethHeader.dest.addr[4],
			pTCP->_ethHeader.dest.addr[5]
		);
		if ((pTCP->_ipHeader.saddr.byte1 == MyIP_Address.byte1) && (pTCP->_ipHeader.saddr.byte2 == MyIP_Address.byte2) && (pTCP->_ipHeader.saddr.byte3 == MyIP_Address.byte3) && (pTCP->_ipHeader.saddr.byte4 == MyIP_Address.byte4)) {
			MyEth_Address = pTCP->_ethHeader.src;
		}
		else {
			MyEth_Address = pTCP->_ethHeader.dest;
		}
		AY_Server_GetMACadr = 1;
	}
	else if (AY_Server_Initied && AY_Server_GetMACadr) {
		Ui32 j;
		int Len;
		AY_CONNTYPE	*pConnTyp;
		pConnTyp = pFindConnByTCPheader((tcp_headerAll *)pkt_data);
		if (TCP_packet_check((Ui08 *)pkt_data, &Len) == 1) {
			if (pConnTyp != nullptr) {
				tcp_headerAll	TCP;

				j = pConnTyp->_TCPh._tcpHeader.acknum;
				pConnTyp->_TCPh._tcpHeader.acknum = pConnTyp->_TCPh._tcpHeader.seqnum;
				pConnTyp->_TCPh._tcpHeader.seqnum = j;///< convert TCP counters
				pConnTyp->_TCPh._tcpHeader.acknum += _HTONSL(Len);

				memcpy(&TCP, (tcp_headerAll *)&pConnTyp->_TCPh, sizeof(tcp_headerAll));

				AY_ChngPacketDest_TCP(&TCP, &MyEth_Address, _ETH_DST_);

				if ((pTCP->_tcpHeader.flags & (_SYN | _ACK)) == (_SYN | _ACK)) {
					AY_ServerSendStatus(&TCP, _ACK);
					//AY_Client_WaitSyncToServer = 1;
				}
				else if (pTCP->_tcpHeader.flags & _SYN) {
					AY_ServerSendStatus(&TCP, (_SYN | _ACK));
				}
				else {
					if (pTCP->_tcpHeader.flags & _FIN) {
						AYCONN_FindOrAddConn(*((Ui32 *)&pConnTyp->_UnqiueId[0]), *((Ui32 *)&pConnTyp->_UnqiueId[1]), *((Ui32 *)&pConnTyp->_UnqiueId[2]), &pConnTyp->_TCPh, &pConnTyp->_SessionKey[0], &pConnTyp, AY_CONN_DELETE);
						//AY_Client_StartSyncToServer = 0;///< re-start connection
					}
					AY_ServerSendStatus(&TCP, _ACK);
				}
			}
			else {
				tcp_headerAll	TCP;
				memcpy(&TCP, (tcp_headerAll *)pkt_data, sizeof(tcp_headerAll));
				TCP._tcpHeader.acknum = ((tcp_headerAll *)pkt_data)->_tcpHeader.seqnum;
				TCP._tcpHeader.acknum += _HTONSL(1);
				TCP._tcpHeader.seqnum = _HTONSL(0xA0B1C2D3);///< convert TCP counters

				AY_ChngPacketDest_TCP(&TCP, &MyEth_Address, _ETH_DST_);

				if ((pTCP->_tcpHeader.flags & (_SYN | _ACK))== (_SYN | _ACK)) {
					AY_ServerSendStatus(&TCP, _ACK);
					//AY_Client_WaitSyncToServer = 1;
				}
				else if (pTCP->_tcpHeader.flags & _SYN) {
					AY_ServerSendStatus(&TCP, (_SYN | _ACK));
				}
				else {
					if (pTCP->_tcpHeader.flags & _FIN) {
						//AY_Client_StartSyncToServer = 0;///< re-start connection
					}
					AY_ServerSendStatus(&TCP, _ACK);
				}
			}
			if (Len == 0) {
			}
			else {
				printf("New TCP packet arrived\n");
				if (AY_TestLoadDeviceStart((Ui08 *)pkt_data, Len) == 0) {
					printf("Not Device Start\n");
					if (AY_TestLoadDirectSendRqst((Ui08 *)pkt_data, Len) == 0) {
						printf("Not Direct Send Request\n");
						if (AY_TestLoadGwInfoRqst((Ui08 *)pkt_data, Len) == 0) {
							printf("Not GW info Request\n");
						}
					}
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

			strcpy((char *)&MySocketBuff[0], "tcp dst port ");
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
			strcpy((char *)&MySocketBuff[0], "tcp dst port ");
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
			MYSQL_Connect((char *)&MySQLParams._ServerAdr[0], (char *)&MySQLParams._ServerUser[0], (char *)&MySQLParams._ServerPass[0], (char *)&MySQLParams._ServerSchema[0]);
			AYCONN_ClientConnInit();
			AYSRV_QueueInit();
			AY_Server_StartSqlConn = 1;
		}
		else {
			AYSRV_TimeoutTestUniqQ();
			//if (++j < 2) {
			//	AY_Delay(1000);
			//}
			//else {
			//	//----------test sil !!!!
			//	//AYSRV_TestMySql();
			//	//...
			//}
		}
		Sleep(1);
	}

	return 0;
}


int AY_ServerSendStatus(tcp_headerAll *pTCP, Ui08 flgs) {
	Ui32 Val;
	/*if ((flgs & _SYN) && ((flgs & _ACK) == 0)) {
		Val = AY_Crypt_GenerateUi32Random();
		MyClientInstPort = 48615 + (Val & 0x3FFF);
		TCP_header_init(pTCP);
		pTCP->_tcpHeader.acknum = 0;
		pTCP->_tcpHeader.seqnum = Val;
	}
	TCP_header_load(pTCP, SrvEth_Address, SrvIP_Address, CngFile.ServerPort, MyEth_Address, MyIP_Address, MyClientInstPort, pTCP->_tcpHeader.acknum, pTCP->_tcpHeader.seqnum, flgs);
*/
	pTCP->_tcpHeader.flags = flgs;
#if STEP_TEST==1
	char Buff[64];
	Buff[0] = 0; AY_Sprintf_Add(&Buff[0], (char *)"\tAYCLNT--> FLAGS -- ");
	if (pTCP->_tcpHeader.flags & _FIN) { AY_Sprintf_Add(&Buff[0], (char *)"[FIN] "); }
	if (pTCP->_tcpHeader.flags & _SYN) { AY_Sprintf_Add(&Buff[0], (char *)"[SYN] "); }
	if (pTCP->_tcpHeader.flags & _RST) { AY_Sprintf_Add(&Buff[0], (char *)"[RST] "); }
	if (pTCP->_tcpHeader.flags & _PSH) { AY_Sprintf_Add(&Buff[0], (char *)"[PSH] "); }
	if (pTCP->_tcpHeader.flags & _ACK) { AY_Sprintf_Add(&Buff[0], (char *)"[ACK] "); }
	if (pTCP->_tcpHeader.flags & _URG) { AY_Sprintf_Add(&Buff[0], (char *)"[URG] "); }
	if (pTCP->_tcpHeader.flags & _ECN) { AY_Sprintf_Add(&Buff[0], (char *)"[ECN] "); }
	if (pTCP->_tcpHeader.flags & _CWR) { AY_Sprintf_Add(&Buff[0], (char *)"[CWR] "); }
	printf("\t\t %s \n", &Buff[0]);
#endif
	return (TCP_packet_send(_MAIN_SCKT, pTCP, 0, 0));
}
