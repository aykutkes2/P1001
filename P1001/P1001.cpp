
#include "pch.h"
#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#include<iostream>
#include<cstdio>
#include<fstream>
#include<sstream>
#include<string>
#include<cstdlib>
#include<conio.h>
#include<windows.h>
//#include<mysql.h>

//#include <windows.h>
//#include <winsock2.h>
//#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <AY_Printf.h>
#include <AY_MySql.h>
#include <AY_Socket.h>
#include <process.h>
//using namespace std;
//__________________________ MYSQL _____________________________________________________________//
//#include "mysql.h" 
//#define SERVER "localhost"
//#define USER "root"
//#define PASSWORD "4154521"
//#define DATABASE "devicelist_001" 
//__________________________ MYSQL _____________________________________________________________//

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN	512
#define DEFAULT_PORT	1982

//using namespace std;

//void ListenSocket_Thread(void *param){
//	printf( "In thread function \n" );
//	AY_Socket_ListenPortStart();
//	printf( "Thread function ends \n" );
//	_endthread();
//}

const Ui08 Macs[2][6] = { {0x60,0xF6,0x77,0xBE,0x5F,0x30},{0x60,0xF6,0x77,0xBE,0x5F,0x2E} };
const Ui08 Uniques[2][12] = { {0x60,0xF6,0x77,0xBE,0x5F,0x30,0x60,0xF6,0x77,0xBE,0x5F,0x30},{0x60,0xF6,0x77,0xBE,0x5F,0x2E,0x60,0xF6,0x77,0xBE,0x5F,0x2E} };

int main(void)//int __cdecl main(void)
{
	HANDLE hThread;
	//__________________________ MYSQL _____________________________________________________________//
	MySqlConnect NewConn;
	clock_t begin, end;
	Ui32 UniqueIDs[3] = { 74,45,43 };
	Ui32 UniqueIDX[3] = { 0xFFFFFFF4,0xFFFFFEAB,0xFFFFFABC };
	Ui32 UniqueIDX2[3] = { 0xFFFFFFF5,0xFFFFFEAB,0xFFFFFABC };
	Ui08 SSK[24] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

	AYSQL_DeviceRead ReadFile[10];
	int idx = 0;

	// Initial Load
	system("cls");
	system("Make It Local Server Program");
	system("color 0f");
	// Initial Load End
///* MYSQL  kapalý !!!
	NewConn._Connect((char *)"localhost", (char *)"root", (char *)"Asel2010", (char *)"devicelist_001");
	begin = clock();
	
	//(char Admin[], char Pass[], Ui32 Unique[], Ui64 Mac, Ui32 ConnId, Ui32 CertificateID, Ui08 *pSSK, Ui08 WhatToDo)
		
	NewConn._AddNewGateway((char *)"admin3", (char *)"1234", (Ui32 *)&Uniques[0][0], *((Ui64 *)&Macs[0][0]), 1, 1, &SSK[0], _GATEWAY_UPDATE_);
	NewConn._AddNewGateway((char *)"admin3", (char *)"1234", (Ui32 *)&Uniques[1][0], *((Ui64 *)&Macs[1][0]), 2, 1, &SSK[0], _GATEWAY_UPDATE_);





	/*idx = NewConn._ReadDeviceList(0, &ReadFile[idx], 3, idx);
	idx = NewConn._ReadDeviceList(0, &ReadFile[idx], 3, idx);	
	
	UniqueIDs[0] = 75;
	NewConn._AddNewGateway((char *)"admin2", (char *)"1234", &UniqueIDs[0], 4154521, 2, 1, &SSK[0], _GATEWAY_UPDATE_);	UniqueIDs[0] = 76;
	NewConn._AddNewGateway((char *)"user1", (char *)"1234", &UniqueIDs[0], 67685, 3, 1, &SSK[0], _GATEWAY_KEEP_);		UniqueIDs[0] = 77;
	NewConn._AddNewGateway((char *)"user2", (char *)"1234", &UniqueIDs[0], 349087, 4, 1, &SSK[0], _GATEWAY_UPDATE_);	UniqueIDs[0] = 79;
	NewConn._AddNewGateway((char *)"user3", (char *)"1234", &UniqueIDs[0], 439870, 5, 1, &SSK[0], _GATEWAY_UPDATE_);	UniqueIDs[0] = 80;
	NewConn._AddNewGateway((char *)"admin1", (char *)"1234", &UniqueIDs[0], 123456, 5, 1, &SSK[0], _GATEWAY_UPDATE_);	
	
	NewConn.Gateway._TotalPackets = 100000;
	NewConn._ReloadGatewayListRow(1 << GWL_Tot);*/
	
	
	end = clock();
	//cout << ("Execution Time: %f seconds\n", (double)(end - begin) / CLOCKS_PER_SEC) << endl;

	//cout << "Execution Time:" << (double)(end - begin) / CLOCKS_PER_SEC) << "seconds\n"   << endl;
	//printf("Execution Time: %f seconds\n", (double)(end - begin) / CLOCKS_PER_SEC);
	NewConn._CloseConnect();
	//*/
	//AY_Socket_ListenPort(DEFAULT_PORT);
	//hThread = (HANDLE)_beginthread(ListenSocket_Thread, 16*1024/*0*/, NULL);

	printf("Wait New Connection\n-------------- ------- ----------------\n");

	getch();
	system("cls");
	   
	return 0;
}