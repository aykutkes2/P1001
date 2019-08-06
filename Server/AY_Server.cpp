//#include "pch.h"
#undef UNICODE

/*
=========== Demo Configurations ================
Server IP				192.168.2.149
Client 1				192.168.2.145	74-D4-35-3C-4A-B2		74-D4-35-3C-4A-B2-74-D4-35-3C-4A-B2
Client 2				192.168.2.146	74-D4-35-3C-4A-B3		74-D4-35-3C-4A-B3-74-D4-35-3C-4A-B3
Device 1				192.168.2.147
Device 1 Ghost			192.168.2.148
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
		//int len2 = header->len;
		//memcpy(&UDPh, pUDP, sizeof(udp_headerAll));
		//memcpy(&MySocketBuff2[0], (Ui08 *)pkt_data, header->len);
		if (UDP_packet_check((Ui08 *)pkt_data, &Len) == 1) {
			printf("New UDP packet arrived\n");
			if (AY_TestLoadDeviceStart((Ui08 *)pkt_data, Len) == 0) {
				printf("Not Device Start\n");
			}
		}

		




		//struct tm *ltime;
		//char timestr[16];
		//ip_header *ih;
		//udp_header *uh;
		//Ui32 ip_len;
		//Ui16 sport, dport;
		//time_t local_tv_sec;

		///*
		// * unused parameter
		// */
		//(void)(param);

		///* convert the timestamp to readable format */
		//local_tv_sec = header->ts.tv_sec;
		//ltime = localtime(&local_tv_sec);
		//strftime(timestr, sizeof timestr, "%H:%M:%S", ltime);

		///* print timestamp and length of the packet */
		//printf("%s.%.6d len:%d ", timestr, header->ts.tv_usec, header->len);

		///* retireve the position of the ip header */
		//ih = (ip_header *)(pkt_data + 14); //length of ethernet header

		///* retireve the position of the udp header */
		//ip_len = (pUDP->_ipHeader.ver_ihl & 0xf) * 4;
		//uh = (udp_header *)((ui08*)ih + ip_len);

		///* convert from network byte order to host byte order */
		//sport = _HTONS(uh->sport);
		//dport = _HTONS(uh->dport);

		///* print ip addresses and udp ports */
		//printf("%d.%d.%d.%d.%d -> %d.%d.%d.%d.%d\n",
		//	pUDP->_ipHeader.saddr.byte1,
		//	pUDP->_ipHeader.saddr.byte2,
		//	pUDP->_ipHeader.saddr.byte3,
		//	pUDP->_ipHeader.saddr.byte4,
		//	sport,
		//	pUDP->_ipHeader.daddr.byte1,
		//	pUDP->_ipHeader.daddr.byte2,
		//	pUDP->_ipHeader.daddr.byte3,
		//	pUDP->_ipHeader.daddr.byte4,
		//	dport);
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
	}

	return 0;
}






////#include "pch.h"
//#undef UNICODE
//
//#define WIN32_LEAN_AND_MEAN
//#include<iostream>
//#include<cstdio>
//#include<fstream>
//#include<sstream>
//#include<string>
//#include<cstdlib>
//#include<conio.h>
//#include<windows.h>
////#include<mysql.h>
//
////#include <windows.h>
////#include <winsock2.h>
////#include <ws2tcpip.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include <time.h>
//#include <AY_Printf.h>
//#include <AY_MySql.h>
//#include <AY_Socket.h>
//#include <process.h>
////using namespace std;
////__________________________ MYSQL _____________________________________________________________//
////#include "mysql.h" 
////#define SERVER "localhost"
////#define USER "root"
////#define PASSWORD "4154521"
////#define DATABASE "devicelist_001" 
////__________________________ MYSQL _____________________________________________________________//
//
//// Need to link with Ws2_32.lib
//#pragma comment (lib, "Ws2_32.lib")
//// #pragma comment (lib, "Mswsock.lib")
//
//#define DEFAULT_BUFLEN	512
//#define DEFAULT_PORT	1982
//
////using namespace std;
//
////void ListenSocket_Thread(void *param){
////	printf( "In thread function \n" );
////	AY_Socket_ListenPortStart();
////	printf( "Thread function ends \n" );
////	_endthread();
////}
//
//const Ui08 Macs[2][6] = { {0x60,0xF6,0x77,0xBE,0x5F,0x30},{0x60,0xF6,0x77,0xBE,0x5F,0x2E} };
//const Ui08 Uniques[2][12] = { {0x60,0xF6,0x77,0xBE,0x5F,0x30,0x60,0xF6,0x77,0xBE,0x5F,0x30},{0x60,0xF6,0x77,0xBE,0x5F,0x2E,0x60,0xF6,0x77,0xBE,0x5F,0x2E} };
//
//int main(void)//int __cdecl main(void)
//{
//	HANDLE hThread;
//	//__________________________ MYSQL _____________________________________________________________//
//	MySqlConnect NewConn;
//	clock_t begin, end;
//	Ui32 UniqueIDs[3] = { 74,45,43 };
//	Ui32 UniqueIDX[3] = { 0xFFFFFFF4,0xFFFFFEAB,0xFFFFFABC };
//	Ui32 UniqueIDX2[3] = { 0xFFFFFFF5,0xFFFFFEAB,0xFFFFFABC };
//	Ui08 SSK[24] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
//
//	AYSQL_DeviceRead ReadFile[10];
//	int idx = 0;
//
//	// Initial Load
//	system("cls");
//	system("Make It Local Server Program");
//	system("color 0f");
//	// Initial Load End
/////* MYSQL  kapalý !!!
//	NewConn._Connect((char *)"localhost", (char *)"root", (char *)"Asel2010", (char *)"devicelist_001");
//	begin = clock();
//	
//	//(char Admin[], char Pass[], Ui32 Unique[], Ui64 Mac, Ui32 ConnId, Ui32 CertificateID, Ui08 *pSSK, Ui08 WhatToDo)
//		
//	NewConn._AddNewGateway((char *)"admin3", (char *)"1234", (Ui32 *)&Uniques[0][0], *((Ui64 *)&Macs[0][0]), 1, 1, &SSK[0], _GATEWAY_UPDATE_);
//	NewConn._AddNewGateway((char *)"admin3", (char *)"1234", (Ui32 *)&Uniques[1][0], *((Ui64 *)&Macs[1][0]), 2, 1, &SSK[0], _GATEWAY_UPDATE_);
//
//
//
//
//
//	/*idx = NewConn._ReadDeviceList(0, &ReadFile[idx], 3, idx);
//	idx = NewConn._ReadDeviceList(0, &ReadFile[idx], 3, idx);	
//	
//	UniqueIDs[0] = 75;
//	NewConn._AddNewGateway((char *)"admin2", (char *)"1234", &UniqueIDs[0], 4154521, 2, 1, &SSK[0], _GATEWAY_UPDATE_);	UniqueIDs[0] = 76;
//	NewConn._AddNewGateway((char *)"user1", (char *)"1234", &UniqueIDs[0], 67685, 3, 1, &SSK[0], _GATEWAY_KEEP_);		UniqueIDs[0] = 77;
//	NewConn._AddNewGateway((char *)"user2", (char *)"1234", &UniqueIDs[0], 349087, 4, 1, &SSK[0], _GATEWAY_UPDATE_);	UniqueIDs[0] = 79;
//	NewConn._AddNewGateway((char *)"user3", (char *)"1234", &UniqueIDs[0], 439870, 5, 1, &SSK[0], _GATEWAY_UPDATE_);	UniqueIDs[0] = 80;
//	NewConn._AddNewGateway((char *)"admin1", (char *)"1234", &UniqueIDs[0], 123456, 5, 1, &SSK[0], _GATEWAY_UPDATE_);	
//	
//	NewConn.Gateway._TotalPackets = 100000;
//	NewConn._ReloadGatewayListRow(1 << GWL_Tot);*/
//	
//	
//	end = clock();
//	//cout << ("Execution Time: %f seconds\n", (double)(end - begin) / CLOCKS_PER_SEC) << endl;
//
//	//cout << "Execution Time:" << (double)(end - begin) / CLOCKS_PER_SEC) << "seconds\n"   << endl;
//	//printf("Execution Time: %f seconds\n", (double)(end - begin) / CLOCKS_PER_SEC);
//	NewConn._CloseConnect();
//	//*/
//	//AY_Socket_ListenPort(DEFAULT_PORT);
//	//hThread = (HANDLE)_beginthread(ListenSocket_Thread, 16*1024/*0*/, NULL);
//
//	printf("Wait New Connection\n-------------- ------- ----------------\n");
//
//	getch();
//	system("cls");
//	   
//	return 0;
//}
