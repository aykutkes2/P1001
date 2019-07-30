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
		printf("FILE: %s LINE: %s\n", pFile, line);
	};
	return 1;
}

int AYFILE_CloseFile(char *pFile) {
	if (fin.is_open()) {
		fin.close();
		printf("FILE: %s closed\n", pFile);
	}
	else {
		printf("FILE: %s error: open file for output failed!\n", pFile);
	}
	if (fout.is_open()) {
		fout.close();
		printf("SYSTEM: %s run\n", pFile);
		AYCMD_SystemWrite(pFile);
		printf("FILE: %s closed\n", pFile);
	}
	else {
		printf("FILE: %s error: open file for output failed!\n", pFile);
	}
	return 1;
}

int AYFILE_ClearFile(char *pFile) {
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
int AYFILE_AddIPsToFile(char *pFile, char *pInterface, Ui32 *pIP, Ui16 cnt, Ui32 GwMask, Ui32 GwIP, Ui08 System) {
	int i = 0;
	char line[256];
	const char netsh[] = "netsh interface ip add address \"";// Local Area Connection\" ";
	const char cd[] = "cd..";
	const char exit[] = "exit";

	//==============================================================================
	if (cnt == 0) {
		for (i = 0; i < 6; i++) {
			if (System) {	AYCMD_SystemWrite((char*)&cd[0]);		}
			strcpy(line, cd);		strcat(line, "\r\n");
			fout.write((char *)&line, strlen(line));
			printf("FILE: %s NEW LINE: %s", pFile, line);
		}
	}
	else if (cnt > 0xFFFE) {
		if (System) { AYCMD_SystemWrite((char*)&exit[0]); }
		strcpy(line, exit);		strcat(line, "\r\n");
		fout.write((char *)&line, strlen(line));
		printf("FILE: %s NEW LINE: %s", pFile, line);
	}
	else {
		while (cnt) {
			strcpy(line, netsh);
			strcat(line, pInterface);
			strcat(line, "\" ");
			AY_ConvertIPAddToStrRet((Ui08 *)pIP, line); strcat(line, " ");
			AY_ConvertIPAddToStrRet((Ui08 *)&GwMask, line); strcat(line, " ");
			AY_ConvertIPAddToStrRet((Ui08 *)&GwIP, line); //strcat(line, " ");

			if (System) { AYCMD_SystemWrite((char*)&line[0]); }
			strcat(line, "\r\n");
			fout.write((char *)&line, strlen(line));
			printf("FILE: %s NEW LINE: %s", pFile, line);

			cnt--;
		}
	}
	//==============================================================================
	return 1;
}
/*
* netsh interface ip delete address "Wi-Fi" addr=192.168.2.148 gateway=all
*/
int AYFILE_DeleteIPsFromFile(char *pFile, char *pInterface, Ui32 *pIP, Ui16 cnt, Ui08 System) {
	int i = 0;
	char line[256];
	const char netsh[] = "netsh interface ip delete address \"";// Local Area Connection\" ";
	const char netsh2[] = "\" addr=";// Local Area Connection\" ";
	const char netsh3[] = " gateway=all";// Local Area Connection\" ";
	const char cd[] = "cd..";
	const char exit[] = "exit";

	//==============================================================================
	if (cnt == 0) {
		for (i = 0; i < 6; i++) {
			if (System) { AYCMD_SystemWrite((char*)&cd[0]); }
			strcpy(line, cd);		strcat(line, "\r\n");
			fout.write((char *)&line, strlen(line));
			printf("FILE: %s NEW LINE: %s", pFile, line);
		}
	}
	else if (cnt > 0xFFFE) {
		if (System) { AYCMD_SystemWrite((char*)&exit[0]); }
		strcpy(line, exit);		strcat(line, "\r\n");
		fout.write((char *)&line, strlen(line));
		printf("FILE: %s NEW LINE: %s", pFile, line);
	}
	else {
		while (cnt) {
			strcpy(line, netsh);
			strcat(line, pInterface);
			strcat(line, netsh2);
			AY_ConvertIPAddToStrRet((Ui08 *)pIP, line);
			strcat(line, netsh3);			
			if (System) { AYCMD_SystemWrite((char*)&line[0]); }
			strcat(line, "\r\n");
			fout.write((char *)&line, strlen(line));
			printf("FILE: %s NEW LINE: %s", pFile, line);

			cnt--;
		}
	}
	//==============================================================================
	return 1;
}

//=================== CONFIG FILE LOAD / UPLOAD =====================================//
/*
* netsh interface ip delete address "Wi-Fi" addr=192.168.2.148 gateway=all
*/
const char cfg_file[] = "file/cfg.xml";
int AYFILE_TestConfigFile(Ui08 Create) {
	int i = 0;
	int j = 0;
	char line[1024];

	fin.open(cfg_file, ios::in | ios::binary);
	if (!fin) {
		if (Create) {
			printf("FILE: %s not found\n", (char *)&cfg_file[0]);
			fout.open(cfg_file, ios::trunc);
			fout.close();
			fout.open(cfg_file, ios::out | ios::app | ios::binary);
			printf("FILE: %s opened as app\n", (char *)&cfg_file[0]);
			while (Default_Config_File[i] != 0) {
				memset(line, 0, sizeof(line));
				while (Default_Config_File[i] != '\n') {
					line[j] = Default_Config_File[i];
					i++;
					j++;
				}
				strcat(line, "\r\n");
				j = 0;
				i++;
				fout.write((char *)&line, strlen(line));
				printf("FILE: %s NEW LINE: %s\n", cfg_file, line);
			}
			fout.close();
			printf("FILE: %s closed\n", cfg_file);
		}
		else {
			printf("FILE: %s not found\n", cfg_file);
			return 0;

		}
	}
	else {
		printf("FILE: %s opened\n", cfg_file);
		fin.close();
		printf("FILE: %s closed\n", cfg_file);
		return 1;
	}
	return 1;
}

int AYFILE_ConfigFileReadComp(char *pVal, int comp) {
	int i = 0;
	int j = 0;
	char line[1024];
	const char item[] = "<Item id = \"";
	const char value[] = "Value = \"";
	char *p,*q;

	fin.open(cfg_file, ios::in | ios::binary);
	if (!fin) {
		printf("FILE: %s not found\n", cfg_file);
		return -1;///< file no
	}
	else {
		printf("FILE: %s opened\n", cfg_file);
		memset(line, 0, sizeof(line));	
		while (fin.getline(&line[0], 1023)) {
			printf("FILE: %s LINE: %s\n", cfg_file, line);
			p = strstr(line, item);
			if (p != 0) {
				p += strlen(item);
				i = AY_ConvertStringToUi32(&p);
				if (i == comp) {
					p = strstr(p, value);
					p += strlen(value);
					q = pVal;///< start
					while (*p != '"') {
						*pVal = *p;
						pVal++;
						p++;
					}
					*pVal = 0;
					printf("FILE: %s COMPONENT: %s\n", cfg_file, q);
					printf("\n");
					fin.close();
					printf("FILE: %s comp. found, file closed\n", cfg_file);
					return 1;
				}
			}
		};
	}
	fin.close();
	printf("FILE: %s comp. not found, file closed\n", cfg_file);
	return -1;
}

int AYFILE_ConfigFileWriteComp(char *pVal, int comp) {
	int i = 0;
	int j = 0;
	char file[32768];///< file temp
	char line[1024];
	char line2[1024];
	const char item[] = "<Item id = \"";
	const char value[] = "Value = \"";
	char *p,*q;


	fin.open(cfg_file, ios::in | ios::binary);
	if (!fin) {
		printf("FILE: %s not found\n", cfg_file);
		return -1;///< file no
	}
	else {
		memset(file, 0, sizeof(file));

		printf("FILE: %s opened to read \n", cfg_file);
		memset(line, 0, sizeof(line));
		while (fin.getline(&line[0], 1023)) {
			printf("FILE: %s LINE: %s\n", cfg_file, line);
			p = strstr(line, item);
			if(p != 0){
				p += strlen(item);
				i = AY_ConvertStringToUi32(&p);
				if (i == comp) {
					strcpy(line2, line);
					p = strstr(p, value);
					p += strlen(value);
					*p = 0;
					//-------------
					q = &line2[strlen(line)];
					while (*q != '"') {
						q++;
					}
					//-----------------
					strcat(p, pVal);
					//-----------------
					strcat(p, q);
					//-----------------
					printf("FILE: %s COMPONENT: %s\n", cfg_file, pVal);
				}
			}
			strcat(line, "\n");
			strcat(file, line);
		};
		fout.open(cfg_file, ios::out | ios::binary);
		printf("FILE: %s opened to write \n", cfg_file);
		fout.write((char *)"", 0);
		printf("FILE: %s clear\n", cfg_file);
		fout.close();
		printf("FILE: %s closed\n", cfg_file);
		fout.open(cfg_file, ios::out | ios::app | ios::binary);
		printf("FILE: %s opened as app\n", cfg_file);
		fout.write((char *)&file, strlen(file));
		fout.close();
		printf("FILE: %s closed\n", cfg_file);
	}
	fin.close();
	printf("FILE: %s reader closed\n", cfg_file);
	return -1;
}
//=================== CERTIFICATE FILE LOAD / UPLOAD =====================================//
/*
* netsh interface ip delete address "Wi-Fi" addr=192.168.2.148 gateway=all
*/
const char cert_file[] = "file/cert.ayk";
const char cert_aes[16] = { 0x62,0xa3,0x3b,0xe9,0x1f,0xff,0xc2,0x0e,0xc9,0xfd,0xfc,0xda,0x6a,0x71,0x25,0x29 };
int AYFILE_TestCertFile(Ui08 Create) {
	int i = 0;
	int j = 0;
	char file[32768];

	fin.open(cert_file, ios::in | ios::binary);
	if (!fin) {
		if (Create) {
			printf("FILE: %s not found\n", (char *)&cert_file[0]);
			fout.open(cert_file, ios::trunc);
			fout.close();
			fout.open(cert_file, ios::out | ios::app | ios::binary);
			printf("FILE: %s opened as app\n", (char *)&cert_file[0]);
			
			i = sizeof((char *)&cert_file[0]);
			memcpy(&file[0], (char *)&cert_file[0], i);
			j = i % 16;
			while (j) {
				file[i] = 0x0;
				i++;
				j--;
			}
			AY_Crypt_AES128((Ui08 *)&cert_aes[0], (Ui08 *)&file[0], (Ui16)i);
			fout.write((char *)&file, i);
			printf("FILE: %s encprted write \n", cert_file);
			fout.close();
			printf("FILE: %s closed\n", cert_file);
		}
		else {
			printf("FILE: %s not found\n", cert_file);
			return 0;

		}
	}
	else {
		printf("FILE: %s opened\n", cert_file);
		fin.close();
		printf("FILE: %s closed\n", cert_file);
		return 1;
	}
	return 1;
}

int AYFILE_ReadCertFile(void) {
	int i = 0;
	int j = 0;
	char file[32768];

	fin.open(cert_file, ios::in | ios::binary);
	if (!fin) {
		printf("FILE: %s not found\n", (char *)&cert_file[0]);
		fout.open(cert_file, ios::trunc);
		fout.close();
		fout.open(cert_file, ios::out | ios::app | ios::binary);
		printf("FILE: %s opened as app\n", (char *)&cert_file[0]);

		i = sizeof((char *)&cert_file[0]);
		memcpy(&file[0], (char *)&cert_file[0], i);
		j = i % 16;
		while (j) {
			file[i] = 0x0;
			i++;
			j--;
		}
		AY_Crypt_AES128((Ui08 *)&cert_aes[0], (Ui08 *)&file[0], (Ui16)i);
		fout.write((char *)&file, i);
		printf("FILE: %s encprted write \n", cert_file);
		fout.close();
		printf("FILE: %s closed\n", cert_file);
	}
	else {
		printf("FILE: %s opened\n", cert_file);
		fin.close();
		printf("FILE: %s closed\n", cert_file);
		return 1;
	}
	return 1;
}
