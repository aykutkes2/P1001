#pragma once
#include <AY_Type.h>
#include <AY_Memory.h>

#pragma pack(push, 1)

#pragma pack(pop)


extern int AYFILE_OpenFile(char *pFile);

extern int AYFILE_CloseFile(char *pFile);

extern int AYFILE_ClearIpList(char *pFile);
/*
* netsh interface ip add address "Local Area Connection" 192.168.2.213 255.255.255.0 192.168.2.1
*/
extern int AYFILE_AddIPsToFile(char *pFile, Ui32 *pIP, Ui16 cnt, Ui32 GwMask, Ui32 GwIP);
#define AYFILE_AddIPsFileStart(pFile)		AYFILE_AddIPsToFile(pFile, 0, 0, 0, 0)
#define AYFILE_AddIPsFileStop(pFile)		AYFILE_AddIPsToFile(pFile, 0, 0xFFFF, 0, 0)

extern const char AddIP_File[];
