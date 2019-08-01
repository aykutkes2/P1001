#pragma once
#include <AY_Type.h>
#include <AY_Memory.h>

#pragma pack(push, 1)

#pragma pack(pop)


extern int AYFILE_OpenFile(char *pFile);

extern int AYFILE_CloseFile(char *pFile);

extern int AYFILE_ClearFile(char *pFile);
/*
* netsh interface ip add address "Local Area Connection" 192.168.2.213 255.255.255.0 192.168.2.1
*/
extern int AYFILE_AddIPsToFile(char *pFile, char *pInterface, Ui32 *pIP, Ui16 cnt, Ui32 GwMask, Ui32 GwIP, Ui08 System);
#define AYFILE_AddIPsFileStart(pFile, pInterface,  System)		AYFILE_AddIPsToFile(pFile, pInterface,  0, 0, 0, 0, System)
#define AYFILE_AddIPsFileStop(pFile, pInterface,  System)		AYFILE_AddIPsToFile(pFile, pInterface,  0, 0xFFFF, 0, 0, System)

/*
* netsh interface ip delete address "Wi-Fi" addr=192.168.2.148 gateway=all
*/
extern int AYFILE_DeleteIPsFromFile(char *pFile, char *pInterface, Ui32 *pIP, Ui16 cnt, Ui08 System);
 
extern const char AddIP_File[];

enum _CFG_FILE {
	_UNDEF,					///< 0
	_NetInterfaceName,		///< 1
	_NetIpAddress,			///< 2
	_NetworkSubnetMask,		///< 3
	_NetworkGatewayIp,		///< 4
	_NetSubnetIp,			///< 5
	_GatewayName,			///< 6
	_GatewayPass,			///< 7
	_AllowAddRemove,		///< 8
	_UniqueID,				///< 9
	_ServerDns,				///< 10
	_ServerPort,			///< 11
	_DNSIp,					///< 12
	_DNSPort,				///< 13
	_ServerID,				///< 14
	_ServerPublicKey,		///< 15
};

extern int AYFILE_TestConfigFile(Ui08 Create);
extern int AYFILE_ConfigFileReadComp(char *pVal, int comp);
extern int AYFILE_ConfigFileWriteComp(char *pVal, int comp);
extern int AYFILE_ConfigFileUpdate(void);

extern int AYFILE_TestCertFile(Ui08 Create);
extern int AYFILE_ReadCertFile(void);