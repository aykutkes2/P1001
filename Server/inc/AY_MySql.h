#pragma once
//#include <ctime>
#include <AY_Type.h>
//#include <mysql.h>
//#include <iostream>
//#include <sstream>
//#include <string>
//#include <stdlib.h>
//using namespace std;

#pragma pack(push, 1)

typedef struct _MYSQL_GATEWAY {
	Ui32		_id;
	char		_Name[45];
	Ui32		_Unique[3];
	Ui64		_MAC;
	Ui32		_Admin;
	Ui32		_CertificateId;
	Ui32		_ConnectionId;
	Ui64		_SessionKey[3];
	Ui32		_PacketCount;
	Ui32		_TotalPackets;
	Ui32		_SendBytes;
	Ui32		_ReceivedBytes;
	time_t		_FirstConnectTime;
	time_t		_LastConnectTime;
	Ui32		_DeviceListId;
}MYSQL_GATEWAY;

typedef struct _MYSQL_DEVICE {
	Ui32		_id;
	char		_Name[45];
	Ui32		_Unique[3];
	Ui64		_MAC;
	Ui32		_ParentId;
	Ui32		_Type;
	Ui32		_LocalIp;
	Ui32		_Status;
	Ui32		_PacketCount;
	Ui32		_TotalPackets;
	Ui32		_SendBytes;
	Ui32		_ReceivedBytes;
}MYSQL_DEVICE;

typedef struct _MYSQL_DeviceRead {
	Ui32		_id;	
	Ui32		_Unique[3];
	union {
		Ui32		_typ_prnt;
		struct {
			Ui32	_ParentId : 28;
			Ui32	_Type : 4;
		};
	};
	Ui32		_LocalIp;
}MYSQL_DeviceRead;

typedef struct _MYSQL_USER {
	Ui32		_id;
	Ui32		_Roles;
	char		_Name[45];
	char		_Password[45];
	Ui32		_RescueQuestionID;
	char		_RescueQuestionAnswer[45];
	Ui32		_AvatarID;
	Ui32		_PictureID;
}MYSQL_USER;

typedef struct _MYSQL_Flags {
	Ui32		_ThisIsFirstF : 1;
}MYSQL_Flags;

#pragma pack(pop)

enum {
	_LIST_KEEP,
	_LIST_DELETE,
};
enum {
	_GATEWAY_KEEP_,
	_GATEWAY_UPDATE_,
	_GATEWAY_REINIT_,
	_GATEWAY_RESET_,
};

enum {
	MYSQL_MANUEL_ID_,
	MYSQL_AUTO_ID_
};

enum {
	MYSQL_QUERY_BY_UNIQUE_,
	MYSQL_QUERY_BY_MAC_,
	MYSQL_QUERY_BY_CONN_,
	MYSQL_QUERY_BY_ID_,
};
enum _GATEWAYLIST_OBJECTS{
	GWL_Id,
	GWL_Nm,
	GWL_Uq0,
	GWL_Uq1,
	GWL_Uq2,
	GWL_Mac,
	GWL_Adm,
	GWL_cer,
	GWL_con,
	GWL_SK0,
	GWL_SK1,
	GWL_SK2,
	GWL_pkt,
	GWL_Tot,
	GWL_SdB,
	GWL_RvB,
	GWL_Fst,
	GWL_Lst,
	GWL_Div,
};

enum _DEVICELIST_OBJECTS {
	DVL_id,
	DVL_Nm,
	DVL_Uq0,
	DVL_Uq1,
	DVL_Uq2,
	DVL_pId,
	DVL_Typ,
	DVL_LIP,
	DVL_Stt,
	DVL_pkt,
	DVL_Tot,
	DVL_SdB,
	DVL_RvB,
};

enum _USERLIST_OBJECTS {
	USR_id,
	USR_Rls,
	USR_Nam,
	USR_Pwd,
	USR_RcQ,
	USR_Ans,
	USR_Atr,
	USR_Pic,
};

//class MySqlConnect {
//	public:AYSQL_Flags		_Flgs;
//	public:AYSQL_Gateway	Gateway;
//	public:AYSQL_Device		Device;
//	Ui32			GATEWAY_UserIDX, GATEWAY_GatewayIDX;
//	Ui32			GATEWAY_DeviceList;
//	Ui64			_SessionKeyTemp[3];
//	char _Server[22];
//	char _User[22];
//	char _Password[22];
//	char _Database[22];
//	char _Temp[512];
//	char _Temp0[45];
//	int qstate;
//	MYSQL_ROW row;
//	MYSQL_RES* res;
//	MYSQL *MYSQL_Conn = mysql_init(NULL);
//	//------------------------------//
//	const char GW_L_Cs[19][20] = {"idGatewayList_001", "NAME", "UNIQUE_0", "UNIQUE_1", "UNIQUE_2", "MAC", "ADMIN", "CERTIFICATE_ID", "CONNECTION_ID", \
//		"SESSION_KEY_0", "SESSION_KEY_1", "SESSION_KEY_2", "PACKET_CNT", "TOTAL_PACKETS", "SEND_BYTES", "RCV_BYTES", "FIRST_CONN", "LAST_CONN", "DEVICE_LIST" };	
//	const char DV_L_Cs[13][20] = { "id", "NAME", "UNIQUE_0", "UNIQUE_1", "UNIQUE_2", "PARENT", "TYPE", "LocalIP", "Status", "PACKET_CNT", "TOTAL_PACKETS", "SEND_BYTES", "RCV_BYTES" };
//	
//	//char Schema[45];// = "devicelist_001";
//	char Table[45];// = "gatewaylist_001";
//	char UserList[45];// = "user_list_001";
//	const char DevListStr[12] = "DeviceList_";
//	//------------------------//
//	
//};// MySqlConn;

typedef struct _MYSQL_CONNECT {
	MYSQL_Flags		_Flgs;
	MYSQL_GATEWAY	_Gateway;
	MYSQL_DEVICE	_Device;
}MYSQL_CONNECT;

extern MYSQL_CONNECT	MySqlConn;
extern char MYSQL_Table[45];
extern char MYSQL_UserList[45];

#define MYSQL_Flgs		MySqlConn._Flgs
#define MYSQL_Gateway	MySqlConn._Gateway
#define MYSQL_Device	MySqlConn._Device

extern Si64 MYSQL_ConvertStringToSi64(char *ptr);
extern Ui64 MYSQL_ConvertStringToUi64(char *ptr);
extern Ui32 MYSQL_ConvertStringToUi32(char **ptr);
extern char *MYSQL_ConvertUi32AddToStr0(Ui32 Val, char *ptr, char Add, char RetStrt);
#define MYSQL_ConvertUi32AddToStr(Val, ptr)		MYSQL_ConvertUi32AddToStr0(Val, ptr, 1, 0)
#define MYSQL_ConvertUi32ToStr(Val, ptr)			MYSQL_ConvertUi32AddToStr0(Val, ptr, 0, 0)
#define MYSQL_ConvertUi32ToStrRet(Val, ptr)		MYSQL_ConvertUi32AddToStr0(Val, ptr, 0, 1)
extern char *MYSQL_ConvertUi64AddToStr0(Ui64 Val, char *ptr, char Add, char RetStrt);
#define MYSQL_ConvertUi64AddToStr(Val, ptr)		MYSQL_ConvertUi64AddToStr0(Val, ptr, 1,0)
#define MYSQL_ConvertUi64ToStr(Val, ptr)			MYSQL_ConvertUi64AddToStr0(Val, ptr, 0,0)
#define MYSQL_ConvertUi64ToStrRet(Val, ptr)		MYSQL_ConvertUi64AddToStr0(Val, ptr, 0,1)
extern void MYSQL_Sprintf(char *pDst, char *pSrc);
extern void MYSQL_Sprintf_Add(char *pDst, char *pSrc);
extern void MYSQL_Start_AddRow(char *pTable, char _nll);
extern void MYSQL_UpdateCellValue(char *pCellName, char *pNewValue);
extern void MYSQL_UpdateCellStart(char *pTable);
extern void MYSQL_UpdateCellEnd(char *pIdName, Ui64 _RowId);
extern void MYSQL_Ui32_Add(Ui32 Val);
extern void MYSQL_Ui64_Add(Ui64 Val);
extern void MYSQL_Stop_Add(void);
extern void MYSQL_Break_Add(void);
extern void MYSQL_String_Add(char *Str);
extern void MYSQL_Ui32_Array_Add(Ui32 Val[], Ui08 Cnt);
extern void MYSQL_Ui64_Array_Add(Ui64 Val[], Ui08 Cnt);
extern int MYSQL_String_Compare(char *Str0, char *Str1);
//extern int MYSQL_Search(string pListx, string pColumn, string pObject);
//extern int MYSQL_ReadAllRows(string pListx);
//extern MYSQL_TIME MYSQL_TimeToMysqTime(struct tm * timeinfo);
//extern char *MYSQL_Time_Add0(MYSQL_TIME time, char *pStr, char Add);
#define MYSQL_Time_Add(time)		MYSQL_Time_Add0(time, &MYSQL_Temp[0], 1)

extern time_t MYSQL_TimeToTime(char *pMysqlTime);

extern int MYSQL_Init(void);
extern int MYSQL_Connect(char Srv[], char Usr[], char Pass[], char DB[]);
extern int MYSQL_AddNewGateway(char Admin[], char Pass[], Ui32 Unique[], Ui64 Mac, Ui32 ConnId, Ui32 CertificateID, Ui08 *pSSK, Ui08 WhatToDo);
extern int MYSQL_CloseConnect(void);
extern int MYSQL_QueryAndLoadGateway(Ui32 id, Ui32 Unique[], Ui64 Mac, Ui32 ConnId, char QueryType);
extern int MYSQL_ReloadGatewayListRow(Ui32 Flgs);

extern int MYSQL_DeleteDeviceList(Ui32 Id);
extern int MYSQL_CreateNewDeviceList(Ui32 Id, char Delete);

extern int MYSQL_TestCreateNewDeviceList(Ui32 Id);

extern int MYSQL_LoadDeviceInfoL(Ui32 gId, Ui32 Id, Ui32 IP);
#define MYSQL_LoadDeviceInfoID(gId, Id)		MYSQL_LoadDeviceInfoL(gId, Id, 0)
#define MYSQL_LoadDeviceInfoIP(gId, IP)		MYSQL_LoadDeviceInfoL(gId, 0, IP)

extern int MYSQL_ReadDeviceList(Ui32 Id, MYSQL_DeviceRead *pReadFile, Ui32 MaxCnt, Ui32 StrtIdx);

extern int MYSQL_FindLoadDeviceInfoL(Ui32 id, Ui32 Unique[], Ui32 IP);
#define MYSQL_FindLoadDeviceInfoIP(Unique, IP)		MYSQL_FindLoadDeviceInfoL(0, Unique, IP)
#define MYSQL_FindLoadDeviceInfoID(id, Unique)		MYSQL_FindLoadDeviceInfoL(id, Unique, 0)
#define FindConnectionIdOfGateway(Unique)		_LoadDeviceInfo(0, Unique)



