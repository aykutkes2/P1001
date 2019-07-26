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
#include <AY_ClientSocket.h>
#include <AY_Client.h>
#include <AY_ClientPrjtBased.h>
#include <AY_Crypt.h>
#include <AY_Memory.h>
#include <AY_Command.h>
#include <AY_File.h>

/* File Handling with C++ using ifstream & ofstream class object*/
/* To write the Content in File*/
/* Then to read the content of file*/
#include <iostream>
/* fstream header file for ifstream, ofstream,
  fstream classes */
#include <fstream> 

#include <string>
using namespace std;

static fstream fio;
static ofstream fout;
static ifstream fin;

const char AddIP_File[] = "file/AddIP.bat";

int AYFILE_OpenFile(char *pFile) {
	char line[256];
	fin.open(pFile, ios::in | ios::binary);
	fout.open(pFile, ios::out | ios::binary);
	printf("FILE: %s opened\n", pFile);
	memset(line, 0, sizeof(line));
	while (fin.read(&line[0], 255)) {
		//line[16] = 0;
		printf("FILE: %s LINE: %s\n", pFile, line);
	};
	return 1;
}

int AYFILE_CloseFile(char *pFile) {
	if (fin.is_open()) {
		fin.close();
		printf("FILE: %s closed\n", pFile);
		//return 1;
	}
	else {
		printf("FILE: %s error: open file for output failed!\n", pFile);
		//return -1;
	}
	if (fout.is_open()) {
		fout.close();
		printf("FILE: %s closed\n", pFile);
	}
	else {
		printf("FILE: %s error: open file for output failed!\n", pFile);
	}
	return 1;
}

int AYFILE_ClearIpList(char *pFile) {
	if (!fout.is_open()) {
		printf("FILE: %s error: open file for clear failed!\n", pFile);
	}
	else {
		fout.open(pFile, ios::out | ios::binary);
		printf("FILE: %s opened\n", pFile);
	}
	fout.write((char *)"", 0);
	printf("FILE: %s clear\n", pFile);
	fout.close();
	printf("FILE: %s closed\n", pFile);
	fout.open(pFile, ios::out | ios::app | ios::binary);
	printf("FILE: %s opened as app\n", pFile);

	return 1;
}
/*
* netsh interface ip add address "Local Area Connection" 192.168.2.213 255.255.255.0 192.168.2.1
*/
int AYFILE_AddIPsToFile(char *pFile, Ui32 *pIP, Ui16 cnt, Ui32 GwMask, Ui32 GwIP) {
	int i = 0;
	char line[256];
	const char netsh[] = "netsh interface ip add address \"Local Area Connection\" ";
	const char cd[] = "cd..\r\n\0";
	const char exit[] = "exit\r\n\0";

	//==============================================================================
	if (cnt == 0) {
		fout.write((char *)&cd, strlen(cd));	printf("FILE: %s NEW LINE: %s", pFile, cd);
		fout.write((char *)&cd, strlen(cd));	printf("FILE: %s NEW LINE: %s", pFile, cd);
		fout.write((char *)&cd, strlen(cd));	printf("FILE: %s NEW LINE: %s", pFile, cd);
		fout.write((char *)&cd, strlen(cd));	printf("FILE: %s NEW LINE: %s", pFile, cd);
		fout.write((char *)&cd, strlen(cd));	printf("FILE: %s NEW LINE: %s", pFile, cd);
	}
	else if (cnt > 0xFFFE) {
		fout.write((char *)&exit, strlen(cd));	printf("FILE: %s NEW LINE: %s", pFile, exit);
	}
	else {
		while (cnt) {
			strcpy(line, netsh);
			AY_ConvertIPAddToStrRet((Ui08 *)pIP, line); strcat(line, " ");
			AY_ConvertIPAddToStrRet((Ui08 *)&GwMask, line); strcat(line, " ");
			AY_ConvertIPAddToStrRet((Ui08 *)&GwIP, line); //strcat(line, " ");
			strcat(line, "\r\n");
			fout.write((char *)&line, strlen(line)); printf("FILE: %s NEW LINE: %s", pFile, line);
			cnt--;
		}
	}
	//==============================================================================
	return 1;
}
