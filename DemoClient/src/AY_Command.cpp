#include "pch.h"
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
#include <AY_Client.h>
#include <AY_ClientPrjtBased.h>
#include <AY_Crypt.h>
#include <AY_Memory.h>
#include <AY_Command.h>

#include <iostream>

int AYCMD_SystemWrite(char *pSys) {
	return system(pSys);
}

int AYCMD_TakeThisIP(Ui08 *pIP) {
	printf("Trying to run cmd using runas cmd");
	//system("cmd;");
	//system("runas /user:administrator regedit");
	//system("runas /user:administrator C:\\netshTst.bat");
	system("cd..");
	system("cd..");
	system("cd..");
	system("cd..");
	system("cd..");
	system("netsh interface ip add address \"Local Area Connection\" 192.168.2.213 255.255.255.0 192.168.2.1");
	system("netsh interface ip add address \"Local Area Connection\" 192.168.2.214 255.255.255.0 192.168.2.1");
	system("netsh interface ip add address \"Local Area Connection\" 192.168.2.215 255.255.255.0 192.168.2.1");
	system("netsh interface ip add address \"Local Area Connection\" 192.168.2.216 255.255.255.0 192.168.2.1");
	//system("runas /user:administrator \"netsh interface ip add address \"Local Area Connection\" 192.168.2.213 255.255.255.0 192.168.2.1\"");
	//system("runas /user:administrator netsh interface ip add address \"Local Area Connection\" 192.168.2.214 255.255.255.0 192.168.2.1");
	//system("runas /user:administrator netsh interface ip add address \"Local Area Connection\" 192.168.2.215 255.255.255.0 192.168.2.1");
	//system("runas /user:administrator netsh interface ip add address \"Local Area Connection\" 192.168.2.216 255.255.255.0 192.168.2.1");
	system("pause");
	return 1;
}


int AYCMD_LeaveThisIP(Ui08 *pIP) {
	printf("Trying to run cmd using runas cmd");
	system("cmd;");

	return 1;
}