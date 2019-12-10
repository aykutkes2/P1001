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
#include <AY_Main.h>
#include <stdio.h>
#include <time.h>
//#include <windows.h>

#include <AY_Printf.h>
#include <AY_Functions.h>
#include <AY_Socket.h>
#include <AY_Client.h>
#include <AY_ClientPrjtBased.h>
#include <AY_Crypt.h>
#include <AY_Memory.h>
#include <AY_Command.h>
#include <AY_File.h>


#if _LINUX_OS_
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <fstream>
#else
/* File Handling with C++ using ifstream & ofstream class object*/
/* To write the Content in File*/
/* Then to read the content of file*/
#include <iostream>
/* fstream header file for ifstream, ofstream,
  fstream classes */
#include <fstream> 

#include <string>
using namespace std;
#endif

#if _LINUX_OS_
static FILE * fio;
static FILE * fout;
static FILE * fin;
#else
static fstream fio;
static ofstream fout;
static ifstream fin;
#endif

const char AddIP_File0[] = "file/AddIP.bat";
const char AddIP_File1[] = "file/AddIP1.bat";
const char AddIP_File2[] = "file/AddIP2.bat";
const char *AddIP_File;

int AYFILE_OpenFile(char *pFile) {
	char line[256];
#if _LINUX_OS_
	ssize_t nrd;
	char c=1;
	Ui08 i;

	fin = fopen(pFile, "w+");
	fout = fopen(pFile, "w+");
	printf("FILE: %s opened\n", pFile);
	memset(line, 0, sizeof(line));
	while (c != EOF) {
		c = fgetc(fin);
		line[i] = c;
		i++;
		if (i > 254) {
			line[i] = 0;
			printf("FILE: %s LINE: %s\n", pFile, line);
			i = 0;
		}
	}
#else
	fin.open(pFile, ios::in | ios::binary);
	fout.open(pFile, ios::out | ios::binary);
	printf("FILE: %s opened\n", pFile);
	memset(line, 0, sizeof(line));
	while (fin.read(&line[0], 255)) {
		printf("FILE: %s LINE: %s\n", pFile, line);
	};
#endif
	return 1;
}
#if _LINUX_OS_
int AYFILE_IsOpenFile(FILE *fin) {
	return(fin->_fileno);
}

int AYFILE_ReadLine(unsigned int *fin2, char *pLine, Ui16 MaxLen) {
	Ui16 i=0;
	char c=1;
	FILE *fin = (FILE *)(*fin2);
	while ((c != EOF) && (c != '\r') && (c != '\r') && (i< MaxLen)) {
		c = fgetc(fin);
		*pLine = c;
		pLine++;
		i++;
	}
	return(i);
}
#else
int AYFILE_ReadLine(unsigned int *fin2, char *pLine, Ui16 MaxLen) {
	ifstream *fin = (ifstream *)fin2;
	if (fin->getline(pLine, MaxLen)) {
		return 1;
	}
	return 0;
}
#endif
int AYFILE_CloseFile(char *pFile) {
#if _LINUX_OS_
	if (AYFILE_IsOpenFile(fin)) {
		fclose(fin);
		printf("FILE: %s closed\n", pFile);
	}
	else {
		printf("FILE: %s error: open file for output failed!\n", pFile);
	}
	if (AYFILE_IsOpenFile(fout)) {
		fclose(fout);
		printf("SYSTEM: %s run\n", pFile);
		AYCMD_SystemWrite(pFile);
		printf("FILE: %s closed\n", pFile);
	}
	else {
		printf("FILE: %s error: open file for output failed!\n", pFile);
	}
#else
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
#endif
	return 1;
}

int AYFILE_ClearFile(char *pFile) {
#if _LINUX_OS_
	if (AYFILE_IsOpenFile(fout)) {
		printf("FILE: %s error: file already opened!\n", pFile);
		fclose(fout);
	}
	fout = fopen(pFile, "w+");///< opens a text file for both reading and writing. (it first cuts the length of the file to zero if it exists, or create a file if it does not exist)
	printf("FILE: %s opened\n", pFile);
	//fputc(0x00, fout);
	printf("FILE: %s clear\n", pFile);
	fclose(fout);
	printf("FILE: %s closed\n", pFile);
	fout = fopen(pFile, "a+");//< opens a text file in both reading and writing mode. (It creates a file if it does not exist. Reading will start from the beginning but writing can only be done at the end)
	printf("FILE: %s opened as app\n", pFile);
#else
	if (fout.is_open()) {
		printf("FILE: %s error: file already opened!\n", pFile); 
		fout.close();
	}
	fout.open(pFile, ios::out | ios::binary);
	printf("FILE: %s opened\n", pFile);
	fout.write((char *)"", 0);
	printf("FILE: %s clear\n", pFile);
	fout.close();
	printf("FILE: %s closed\n", pFile);
	fout.open(pFile, ios::out | ios::app | ios::binary);
	printf("FILE: %s opened as app\n", pFile);
#endif
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
#if _LINUX_OS_
			fputs((char *)&line, fout);
#else
			fout.write((char *)&line, strlen(line));
#endif
			printf("FILE: %s NEW LINE: %s", pFile, line);
		}
	}
	else if (cnt > 0xFFFE) {
		if (System) { AYCMD_SystemWrite((char*)&exit[0]); }
		strcpy(line, exit);		strcat(line, "\r\n");
#if _LINUX_OS_
		fputs((char *)&line, fout);
#else
		fout.write((char *)&line, strlen(line));
#endif
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
#if _LINUX_OS_
			fputs((char *)&line, fout);
#else
			fout.write((char *)&line, strlen(line));
#endif
			printf("FILE: %s NEW LINE: %s", pFile, line);
			pIP++;
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
#if _LINUX_OS_
			fputs((char *)&line, fout);
#else
			fout.write((char *)&line, strlen(line));
#endif
			printf("FILE: %s NEW LINE: %s", pFile, line);
		}
	}
	else if (cnt > 0xFFFE) {
		if (System) { AYCMD_SystemWrite((char*)&exit[0]); }
		strcpy(line, exit);		strcat(line, "\r\n");
#if _LINUX_OS_
		fputs((char *)&line, fout);
#else
		fout.write((char *)&line, strlen(line));
#endif
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
#if _LINUX_OS_
			fputs((char *)&line, fout);
#else
			fout.write((char *)&line, strlen(line));
#endif
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
const char cfg_file0[] = "file/cfg.xml";
const char cfg_file1[] = "file/cfg1.xml";
const char cfg_file2[] = "file/cfg2.xml";
const char *cfg_file;
int AYFILE_SelectConfigFile(Ui08 FileNo) {
	switch (FileNo) {
	case 1:		cfg_file = &cfg_file1[0];	AddIP_File = &AddIP_File1[0];  break;
	case 2:		cfg_file = &cfg_file2[0];	AddIP_File = &AddIP_File2[0];  break;
	default:	cfg_file = &cfg_file0[0];	AddIP_File = &AddIP_File0[0];  break;
	}
	return 1;
}

int AYFILE_TestConfigFile(Ui08 Create) {
	int i = 0;
	int j = 0;
	char line[1024];

#if _LINUX_OS_
	fin = fopen(cfg_file, "w+");
#else
	fin.open(cfg_file, ios::in | ios::binary);
#endif
	if ( (!fin)|| (Create==0xAA) ) {
		if (Create) {
			printf("FILE: %s not found\n", (char *)&cfg_file[0]);
#if _LINUX_OS_
			fout = fopen(cfg_file, "w+");///< opens a text file for both reading and writing. (it first cuts the length of the file to zero if it exists, or create a file if it does not exist)
			//fputc(0x00, fout);
			fclose(fout);
			fout = fopen(cfg_file, "a+");//< opens a text file in both reading and writing mode. (It creates a file if it does not exist. Reading will start from the beginning but writing can only be done at the end)

#else
			fout.open(cfg_file, ios::trunc);
			fout.close();
			fout.open(cfg_file, ios::out | ios::app | ios::binary);
#endif
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
#if _LINUX_OS_
				fputs((char *)&line, fout);
#else
				fout.write((char *)&line, strlen(line));
#endif
				printf("FILE: %s NEW LINE: %s\n", cfg_file, line);
			}
#if _LINUX_OS_
			fclose(fout);
#else
			fout.close();
#endif
			printf("FILE: %s closed\n", cfg_file);
		}
		else {
			printf("FILE: %s not found\n", cfg_file);
			return 0;

		}
	}
	else {
		printf("FILE: %s opened\n", cfg_file);
#if _LINUX_OS_
		fclose(fin);
#else
		fin.close();
#endif
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

#if _LINUX_OS_
	fin = fopen(cfg_file, "w+");
#else
	fin.open(cfg_file, ios::in | ios::binary);
#endif
	if (!fin) {
		printf("FILE: %s not found\n", cfg_file);
		return -1;///< file no
	}
	else {
		printf("FILE: %s opened\n", cfg_file);
		memset(line, 0, sizeof(line));	
		while (AYFILE_ReadLine((unsigned int *)&fin, &line[0], 1023)) {
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
						if (*p == 0) {
							AYFILE_ReadLine((unsigned int *)&fin, &line[0], 1023);
							printf("FILE: %s LINE: %s\n", cfg_file, line);
							p = &line[0];
						}
						if (*p != '"') {
							if ((comp == _ServerPublicKey)&&(*p=='\r')) {
								*p = '\n';
							}
							*pVal = *p;
							pVal++;
							p++;
						}
					}
					*pVal = 0;
					printf("FILE: %s COMPONENT: %s\n", cfg_file, q);
					printf("\n");
#if _LINUX_OS_
					fclose(fin);
#else
					fin.close();
#endif
					printf("FILE: %s comp. found, file closed\n", cfg_file);
					return 1;
				}
			}
		};
	}
#if _LINUX_OS_
	fclose(fin);
#else
	fin.close();
#endif
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
	
#if _LINUX_OS_
	fin = fopen(cfg_file, "w+");
#else
	fin.open(cfg_file, ios::in | ios::binary);
#endif
	if (!fin) {
		printf("FILE: %s not found\n", cfg_file);
		return -1;///< file no
	}
	else {
		memset(file, 0, sizeof(file));

		printf("FILE: %s opened to read \n", cfg_file);
		memset(line, 0, sizeof(line));
		while (AYFILE_ReadLine((unsigned int *)&fin, &line[0], 1023)) {
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
#if _LINUX_OS_
		fout = fopen(cfg_file, "w+");///< opens a text file for both reading and writing. (it first cuts the length of the file to zero if it exists, or create a file if it does not exist)
		printf("FILE: %s opened\n", cfg_file);
		//fputc(0x00, fout);
		printf("FILE: %s clear\n", cfg_file);
		fclose(fout);
		printf("FILE: %s closed\n", cfg_file);
		fout = fopen(cfg_file, "a+");//< opens a text file in both reading and writing mode. (It creates a file if it does not exist. Reading will start from the beginning but writing can only be done at the end)
		printf("FILE: %s opened as app\n", cfg_file);
		fputs((char *)&file, fout);
		fclose(fout);
		printf("FILE: %s closed\n", cfg_file);
#else
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
#endif
	}
#if _LINUX_OS_
	fclose(fin);
#else
	fin.close();
#endif
	printf("FILE: %s reader closed\n", cfg_file);
	return -1;
}

int AYFILE_ConfigFileUpdate(void) {
	char *p,rd[45] = { 0 };
	Ui32 i;
	if (AYFILE_ConfigFileReadComp(rd, _NetInterfaceName)) {///< read NetInterfaceName
		if (strlen(rd) == 0) {	strcpy(rd, (char*)&DefaultInterface[0]);	}///< use default
		strcpy(CngFile.NetInterfaceName, rd);		rd[0] = 0;
		//-------------------------//
		if (AYFILE_ConfigFileReadComp(rd, _NetIpAddress)) {///< read NetIpAddress
			if (strlen(rd) == 0) {///< use default
				AY_Client_DynamicIP = 1;
				memset(CngFile.NetIpAddress, 0, sizeof(CngFile.NetIpAddress));
				memset(CngFile.NetworkGatewayIp, 0, sizeof(CngFile.NetworkGatewayIp));
				memset(CngFile.NetSubnetIp, 0, sizeof(CngFile.NetSubnetIp));
				memset(CngFile.NetworkSubnetMask, 0, sizeof(CngFile.NetworkSubnetMask));
			}
			else {
				AY_Client_DynamicIP = 0;
				p = &rd[0];
				*((Ui32 *)&CngFile.NetIpAddress) = AY_ConvertStringToIP(&p);
			}
			//-------------------------//
			if (AYFILE_ConfigFileReadComp(rd, _NetworkSubnetMask)) {///< read NetworkSubnetMask
				if (strlen(rd) == 0) {///< use default
					if(!AY_Client_DynamicIP) { return -28; }					
				}
				else {
					if (!AY_Client_DynamicIP) { 
						p = &rd[0];
						*((Ui32 *)&CngFile.NetworkSubnetMask) = AY_ConvertStringToIP(&p);
					}
				}
				//-------------------------//
				if (AYFILE_ConfigFileReadComp(rd, _NetworkGatewayIp)) {///< read NetworkGatewayIp
					if (strlen(rd) == 0) {///< use default
						if (!AY_Client_DynamicIP) { return -27; }
					}
					else {
						if (!AY_Client_DynamicIP) {
							p = &rd[0];
							*((Ui32 *)&CngFile.NetworkGatewayIp) = AY_ConvertStringToIP(&p);
						}
					}
					//-------------------------//
					if (AYFILE_ConfigFileReadComp(rd, _NetSubnetIp)) {///< read NetSubnetIp
						if (strlen(rd) == 0) {///< use default
							if (!AY_Client_DynamicIP) { return -26; }
						}
						else {
							if (!AY_Client_DynamicIP) {
								p = &rd[0];
								*((Ui32 *)&CngFile.NetSubnetIp) = AY_ConvertStringToIP(&p);
							}
						}
						//-------------------------//
						if (AYFILE_ConfigFileReadComp(rd, _GatewayName)) {///< read GatewayName
							strcpy(CngFile.GatewayName, rd);		rd[0] = 0;
							//-------------------------//
							if (AYFILE_ConfigFileReadComp(rd, _GatewayPass)) {///< read GatewayPass
								strcpy(CngFile.GatewayPass, rd);		rd[0] = 0;
								//-------------------------//
								if (AYFILE_ConfigFileReadComp(rd, _AllowAddRemove)) {///< read AllowAddRemove
									p = &rd[0];
									if ((strstr(p, "YES")) || (strstr(p, "yes")) || (strstr(p, "Yes"))) { CngFile.AllowAddRemove = 1; } 
									else { CngFile.AllowAddRemove = 0; }
									//-------------------------//
									if (AYFILE_ConfigFileReadComp(rd, _UniqueID)) {///< read UniqueID
										if (strlen(rd) == 0) {///< generate new
											AY_Generate_AES128((Ui08 *)&rd[26]);
											AY_HexToStr(&rd[0], (Ui08 *)&rd[26], 12, 0);
											AYFILE_ConfigFileWriteComp(&rd[0], _UniqueID);
										}
										AY_StrToHex(CngFile.UniqueID, &rd[0], 12);
										//-------------------------//
										if (AYFILE_ConfigFileReadComp(rd, _ServerDns)) {///< read ServerDns
											if (strlen(rd) == 0) { strcpy(rd, (char*)&DefaultServerDns[0]); }///< use default
											strcpy(CngFile.ServerDns, rd);		rd[0] = 0;
											//-------------------------//
											if (AYFILE_ConfigFileReadComp(rd, _ServerPort)) {///< read ServerPort
												if (strlen(rd) == 0) { CngFile.ServerPort = DefaultServerPort; }///< use default
												else {	CngFile.ServerPort = AY_ConvertStringToUi64(rd); }
												//-------------------------//
												if (AYFILE_ConfigFileReadComp(rd, _DNSIp)) {///< read DNSIp
													if (strlen(rd) == 0) {///< use default
														*((Ui32 *)&CngFile.DNSIp) = *((Ui32 *)&DefaultDNS_Ip);
													}
													else {
														p = &rd[0];
														*((Ui32 *)&CngFile.DNSIp[0]) = AY_ConvertStringToIP(&p);
													}
													//-------------------------//
													if (AYFILE_ConfigFileReadComp(rd, _DNSPort)) {///< read DNSPort
														if (strlen(rd) == 0) { CngFile.DNSPort = DefaultDNS_Port; }///< use default
														else { CngFile.DNSPort = AY_ConvertStringToUi64(rd); }
														//-------------------------//
														if (AYFILE_ConfigFileReadComp(rd, _ServerID)) {///< read ServerID
															if (strlen(rd) == 0) { CngFile.ServerID = DefaultServerID; }///< use default
															else { CngFile.ServerID = AY_ConvertStringToUi64(rd); }
															//-------------------------//
															if (AYFILE_ConfigFileReadComp(&CngFile.ServerPublicKey[0], _ServerPublicKey)) {///< read ServerPublicKey
																if (strlen(&CngFile.ServerPublicKey[0]) == 0) {///< use default 
																	AYFILE_TestConfigFile(0xAA);
																	return -16; 
																}
																else { 
																	//strcpy(&CngFile.ServerPublicKey[0],rd); 
																}
																return 1;
															}
															else { return -15; }
														}
														else { return -14; }
													}
													else { return -13; }
												}
												else { return -12; }
											}
											else { return -11; }
										}
										else { return -10; }
									}
									else { return -9; }
								}
								else { return -8; }
							}
							else { return -7; }
						}
						else { return -6; }
					}
					else { return -5; }
				}
				else { return -4; }
			}
			else { return -3; }
		}
		else { return -2; }
	}
	else { return -1; }
}



//=================== CERTIFICATE FILE LOAD / UPLOAD =====================================//
/*
* netsh interface ip delete address "Wi-Fi" addr=192.168.2.148 gateway=all
*/
const char cert_file[] = "file/cert.ayk";
const unsigned char cert_aes[16] = { 0x62,0xa3,0x3b,0xe9,0x1f,0xff,0xc2,0x0e,0xc9,0xfd,0xfc,0xda,0x6a,0x71,0x25,0x29 };
int AYFILE_TestCertFile(Ui08 Create) {
	int i = 0;
	int j = 0;
	char file[32768];

#if _LINUX_OS_
	fin = fopen(cert_file, "w+");
#else
	fin.open(cert_file, ios::in | ios::binary);
#endif
	if (!fin){
		if (Create) {
			printf("FILE: %s not found\n", (char *)&cert_file[0]);
#if _LINUX_OS_
			fout = fopen(cert_file, "w+");///< opens a text file for both reading and writing. (it first cuts the length of the file to zero if it exists, or create a file if it does not exist)
			//fputc(0x00, fout);
			fclose(fout);
			fout = fopen(cert_file, "a+");//< opens a text file in both reading and writing mode. (It creates a file if it does not exist. Reading will start from the beginning but writing can only be done at the end)

#else
			fout.open(cert_file, ios::trunc);
			fout.close();
			fout.open(cert_file, ios::out | ios::app | ios::binary);
#endif
			printf("FILE: %s opened as app\n", (char *)&cert_file[0]);
			
			i = strlen((char *)Default_Cert_File);
			memcpy(&file[0], Default_Cert_File, i);
			j = (16 - (i & 0xF))& 0xF;
			while (j) {
				file[i] = 0x0;
				i++;
				j--;
			}
			AY_Crypt_AES128((Ui08 *)&cert_aes[0], (Ui08 *)&file[0], (Ui16)i);
#if _LINUX_OS_
			fwrite((char *)&file, sizeof(char), sizeof(file), fout);
#else
			fout.write((char *)&file, i);
#endif
			printf("FILE: %s encprted write \n", cert_file);
#if _LINUX_OS_
			fclose(fout);
#else
			fout.close();
#endif
			printf("FILE: %s closed\n", cert_file);
		}
		else {
			printf("FILE: %s not found\n", cert_file);
			return 0;
		}
	}
	else {
		printf("FILE: %s opened\n", cert_file);
#if _LINUX_OS_
		fclose(fin);
#else
		fin.close();
#endif
		printf("FILE: %s closed\n", cert_file);
		return 1;
	}
	return 1;
}

int AYFILE_ReadCertFile(void) {
	int i = 0;
	int j = 0;
	char file[32768];
	char *p,*q;

	if (AYFILE_TestCertFile(1)) {
#if _LINUX_OS_
		fin = fopen(cert_file, "w+");
#else
		fin.open(cert_file, ios::in | ios::binary);
#endif
		if (!fin) {
			printf("FILE: %s There is something wrong!!\n", (char *)&cert_file[0]);
#if _LINUX_OS_
			fclose(fin);
#else
			fin.close();
#endif
			printf("FILE: %s closed\n", cert_file);
			return -1;
		}
		else {
			i = 0;			
			p = &file[0];
#if _LINUX_OS_
			char c = 1;
			while (c != EOF)
			{
				c = fgetc(fin); /* read from file*/
				*p = c;
				i++;
				p++;
			}
#else
			while (fin.read(p, 1)) {
				i++;
				p++;
			}
#endif
			AY_Decrypt_AES128((Ui08 *)&cert_aes[0], (Ui08 *)&file[0], (Ui16)i);
			p = (char *)&file[0];
			CLIENT_CERT_NO = (Ui32)AY_ConvertStringToUi64(p);
			p = strstr(p, "-----BEGIN PUBLIC KEY-----\n");
			q = strstr(p, "-----END PUBLIC KEY-----\n");
			if ((p) && (q)) {
				memcpy(&SIGNING_PUB_KEY[0], p, (q - p + sizeof("-----END PUBLIC KEY-----\n")));				
			}
			p = strstr(p, "-----BEGIN RSA PRIVATE KEY-----\n");
			q = strstr(p, "-----END RSA PRIVATE KEY-----\n");
			if ((p) && (q)) {
				memcpy(&SIGNING_PR_KEY[0], p, (q - p + sizeof("-----END RSA PRIVATE KEY-----\n")));
			}
#if _LINUX_OS_
			fclose(fin);
#else
			fin.close();
#endif
			printf("FILE: %s closed\n", cert_file);
			return 1;
		}
	}
	return -1;	
}
