#pragma once
#include <ctime>
#include <AY_Type.h>
#include <mysql.h>
#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>
using namespace std;

#pragma pack(push, 8)

typedef struct _AYSQL_Gateway {
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
}AYSQL_Gateway;

typedef struct _AYSQL_Device {
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
}AYSQL_Device;

typedef struct _AYSQL_DeviceRead {
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
}AYSQL_DeviceRead;

typedef struct _AYSQL_User {
	Ui32		_id;
	Ui32		_Roles;
	char		_Name[45];
	char		_Password[45];
	Ui32		_RescueQuestionID;
	char		_RescueQuestionAnswer[45];
	Ui32		_AvatarID;
	Ui32		_PictureID;
}AYSQL_User;

typedef struct _AYSQL_Flags {
	Ui32		_ThisIsFirstF : 1;
}AYSQL_Flags;

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

class MySqlConnect {
	public:AYSQL_Flags		_Flgs;
	public:AYSQL_Gateway	Gateway;
	public:AYSQL_Device		Device;
	Ui32			GATEWAY_UserIDX, GATEWAY_GatewayIDX;
	Ui32			GATEWAY_DeviceList;
	Ui64			_SessionKeyTemp[3];
	char _Server[22];
	char _User[22];
	char _Password[22];
	char _Database[22];
	char _Temp[512];
	char _Temp0[45];
	int qstate;
	MYSQL_ROW row;
	MYSQL_RES* res;
	MYSQL *connect = mysql_init(NULL);
	//------------------------------//
	const char GW_L_Cs[19][20] = {"idGatewayList_001", "NAME", "UNIQUE_0", "UNIQUE_1", "UNIQUE_2", "MAC", "ADMIN", "CERTIFICATE_ID", "CONNECTION_ID", \
		"SESSION_KEY_0", "SESSION_KEY_1", "SESSION_KEY_2", "PACKET_CNT", "TOTAL_PACKETS", "SEND_BYTES", "RCV_BYTES", "FIRST_CONN", "LAST_CONN", "DEVICE_LIST" };	
	const char DV_L_Cs[13][20] = { "id", "NAME", "UNIQUE_0", "UNIQUE_1", "UNIQUE_2", "PARENT", "TYPE", "LocalIP", "Status", "PACKET_CNT", "TOTAL_PACKETS", "SEND_BYTES", "RCV_BYTES" };
	
	//char Schema[45];// = "devicelist_001";
	char Table[45];// = "gatewaylist_001";
	char UserList[45];// = "user_list_001";
	const char DevListStr[12] = "DeviceList_";
	//------------------------//
	Si64 _ConvertStringToSi64(char *ptr) {
		Si64 Rslt=0;
		Ui08 Sign=0;
		while (*ptr == ' ') { ptr++; }
		if (*ptr == '-') {
			Sign = 1;
			ptr++;
		}
		while ((*ptr >= '0') && (*ptr <= '9')) {
			Rslt = (Rslt * 10) + (*ptr - '0');
			ptr++;
		}
		if (Sign) { Rslt = 0 - Rslt;  }
		return (Rslt);
	}
	Ui64 _ConvertStringToUi64(char *ptr) {
		Ui64 Rslt = 0;
		while (*ptr == ' ') { ptr++; }
		while ((*ptr >= '0') && (*ptr <= '9')) {
			Rslt = (Rslt * 10) + (*ptr - '0');
			ptr++;
		}
		return (Rslt);
	}
	Ui32 _ConvertStringToUi32(char **ptr) {
		Ui32 Rslt = 0;
		Ui08 i = 0;
		char *p;
		p = *ptr;
		while ((++i<200)&&(*p != 0x00)&&(!((*p >= '0') && (*p <= '9')))) { p++; }
		while ((*p >= '0') && (*p <= '9')) {
			Rslt = (Rslt * 10) + (*p - '0');
			p++;
		}
		*ptr = p;
		return (Rslt);
	}
	char *_ConvertUi32AddToStr0(Ui32 Val, char *ptr, char Add, char RetStrt) {
		char i = 0;
		char b[12];
		char *p = ptr;
		if (Add) {
			while (*ptr != 0) { ptr++; }
		}
		do {
			b[i] = (Val % 10) + '0';
			i++;
			Val /= 10;
		} while (Val);
		while (i) {
			i--;
			*ptr = b[i];
			ptr++;
		}
		*ptr = 0;
		if (RetStrt) { ptr = p; }
		return (ptr);
	}
	#define _ConvertUi32AddToStr(Val, ptr)		_ConvertUi32AddToStr0(Val, ptr, 1, 0)
	#define _ConvertUi32ToStr(Val, ptr)			_ConvertUi32AddToStr0(Val, ptr, 0, 0)
	#define _ConvertUi32ToStrRet(Val, ptr)		_ConvertUi32AddToStr0(Val, ptr, 0, 1)
	char *_ConvertUi64AddToStr0(Ui64 Val, char *ptr, char Add, char RetStrt) {
		char i = 0;
		char b[20];
		char *p = ptr;
		if (Add) {
			while (*ptr != 0) { ptr++; }
		}
		do {
			b[i] = (Val % 10) + '0';
			i++;
			Val /= 10;
		} while (Val);
		while (i) {
			i--;
			*ptr = b[i];
			ptr++;
		}
		*ptr = 0;
		if (RetStrt) { ptr = p; }
		return (ptr);
	}
	#define _ConvertUi64AddToStr(Val, ptr)		_ConvertUi64AddToStr0(Val, ptr, 1,0)
	#define _ConvertUi64ToStr(Val, ptr)			_ConvertUi64AddToStr0(Val, ptr, 0,0)
	#define _ConvertUi64ToStrRet(Val, ptr)		_ConvertUi64AddToStr0(Val, ptr, 0,1)
	void _Sprintf(char *pDst, char *pSrc) {
		int i=0;
		if (pSrc != nullptr) {
			while ((*pSrc != 0) && (i < 1024)) {
				*pDst = *pSrc;
				pDst++;
				pSrc++;
				i++;
			};
			*pDst = 0;
		}
		return;
	}
	void _Sprintf_Add(char *pDst, char *pSrc) {
		int i = 0;
		if (pSrc != nullptr) {
			while (*pDst != 0) { pDst++; }
			while ((*pSrc != 0) && (i < 1024)) {
				*pDst = *pSrc;
				pDst++;
				pSrc++;
				i++;
			};
			*pDst = 0;
		}
		return;
	}
	void _Start_AddRow(char *pTable, char _nll) {
		_Sprintf(&_Temp[0], (char *)"INSERT INTO ");
		_Sprintf_Add(&_Temp[0], pTable);
		if (_nll == MYSQL_AUTO_ID_) {
			_Sprintf_Add(&_Temp[0], (char *)" VALUES ( NULL ,'");
		}
		else {
			_Sprintf_Add(&_Temp[0], (char *)" VALUES ( '");
		}
		return;
	}
	void _UpdateCellValue(char *pCellName, char *pNewValue) {
		//UPDATE `devicelist_001`.`gatewaylist_001` SET `CERTIFICATE ID` = '2' WHERE(`idGatewayList_001` = '6');
		if (_Flgs._ThisIsFirstF == 0)	{ _Sprintf_Add(&_Temp[0], (char *)","); }
		else							{ _Flgs._ThisIsFirstF = 0; }
		_Sprintf_Add(&_Temp[0], (char *)" ");
		_Sprintf_Add(&_Temp[0], pCellName);
		_Sprintf_Add(&_Temp[0], (char *)" = '");
		_Sprintf_Add(&_Temp[0], pNewValue);
		_Sprintf_Add(&_Temp[0], (char *)"' ");
	}
	void _UpdateCellStart(char *pTable) {
		//UPDATE `devicelist_001`.`gatewaylist_001` SET `CERTIFICATE ID` = '2' WHERE(`idGatewayList_001` = '6');
		_Sprintf(&_Temp[0], (char *)"UPDATE ");
		_Sprintf_Add(&_Temp[0], pTable);
		_Sprintf_Add(&_Temp[0], (char *)" SET");
		_Flgs._ThisIsFirstF = 1;

	}
	void _UpdateCellEnd(char *pIdName, Ui64 _RowId) {
		//UPDATE `devicelist_001`.`gatewaylist_001` SET `CERTIFICATE ID` = '2' WHERE(`idGatewayList_001` = '6');
		_Sprintf_Add(&_Temp[0], (char *)"WHERE ");
		_Sprintf_Add(&_Temp[0], pIdName);
		_Sprintf_Add(&_Temp[0], (char *)" = '");
		_Ui64_Add(_RowId);
		_Sprintf_Add(&_Temp[0], (char *)"'");
	}
	void _Ui32_Add(Ui32 Val) {
		_ConvertUi32AddToStr(Val, &_Temp[0]);
		return;
	}
	void _Ui64_Add(Ui64 Val) {
		_ConvertUi64AddToStr(Val, &_Temp[0]);
		return;
	}
	void _Stop_Add(void) {
		_Sprintf_Add(&_Temp[0], (char *)"')");
		return;
	}
	void _Break_Add(void) {
		_Sprintf_Add(&_Temp[0], (char *)"' , '");
		return;
	}
	void _String_Add(char *Str) {
		_Sprintf_Add(&_Temp[0], Str);
		return;
	}
	void _Ui32_Array_Add(Ui32 Val[], Ui08 Cnt){
		Ui08 i;
		for (i = 0; i < Cnt; i++) {
			_ConvertUi32AddToStr(Val[i], &_Temp[0]);
			if (i < (Cnt - 1)){
				_Sprintf_Add(&_Temp[0], (char *)"','");
			}
		}
		return;
	}
	void _Ui64_Array_Add(Ui64 Val[], Ui08 Cnt) {
		Ui08 i;
		for (i = 0; i < Cnt; i++) {
			_ConvertUi64AddToStr(Val[i], &_Temp[0]);
			if (i < (Cnt - 1)) {
				_Sprintf_Add(&_Temp[0], (char *)"','");
			}
		}
		return;
	}
	int _String_Compare(char *Str0, char *Str1) {
		int i = 0;
		while ((*Str0 != 0) && (*Str1 != 0) && (i<255)) {
			if (*Str0 != *Str1) {
				return APPX_E_FILE_COMPRESSION_MISMATCH;
			}
			Str0 ++;
			Str1 ++;
			i++;
		}
		return TRUE;
	}
	int MYSQL_Search(string pListx, string pColumn, string pObject) {

		string findbyname_query = "select * from " + pListx + " where " + pColumn + " = '" + pObject + "'";
		const char* qn = findbyname_query.c_str();
		return( mysql_query(connect, qn) );
	}
	int MYSQL_ReadAllRows(string pListx) {

		string findbyname_query = "select * from " + pListx ;
		const char* qn = findbyname_query.c_str();
		return(mysql_query(connect, qn));
	}
	MYSQL_TIME _TimeToMysqTime(struct tm * timeinfo) {
		MYSQL_TIME time ={ (unsigned int)(timeinfo->tm_year + 1900), (unsigned int)timeinfo->tm_mon, (unsigned int)timeinfo->tm_mday, (unsigned int)timeinfo->tm_hour, (unsigned int)timeinfo->tm_min, (unsigned int)timeinfo->tm_sec, 0, 0, MYSQL_TIMESTAMP_NONE };
		return(time);
	}
	char *_Time_Add0(MYSQL_TIME time, char *pStr, char Add) {
		char *p = pStr;
		_ConvertUi32AddToStr0(time.year, pStr, Add, 1);
		_Sprintf_Add(pStr, (char *)"/");
		_ConvertUi32AddToStr((time.month+1), pStr);
		_Sprintf_Add(pStr, (char *)"/");
		_ConvertUi32AddToStr(time.day, pStr);
		_Sprintf_Add(pStr, (char *)" ");
		_ConvertUi32AddToStr(time.hour, pStr);
		_Sprintf_Add(pStr, (char *)":");
		_ConvertUi32AddToStr(time.minute, pStr);
		_Sprintf_Add(pStr, (char *)":");
		_ConvertUi32AddToStr(time.second, pStr);
		return p;
	}
	#define _Time_Add(time)		_Time_Add0(time, &_Temp[0], 1)

	time_t _MYSQL_TimeToTime(char *pMysqlTime) {
		struct tm timeinfo;
		char *ptr = &_Temp0[0];
		Ui32 val;

		_Sprintf(&_Temp0[0], pMysqlTime);

		val = _ConvertStringToUi32(&ptr);			if ((val > 1900) && (val < 5000))	{ timeinfo.tm_year = val - 1900;	} else { timeinfo.tm_year = 1900; }
		val = _ConvertStringToUi32(&ptr);			if ((val > 0) && (val < 13))		{ timeinfo.tm_mon = val - 1;		} else { timeinfo.tm_mon = 0; }
		val = _ConvertStringToUi32(&ptr);			if ((val > 0) && (val < 32))		{ timeinfo.tm_mday = val;			} else { timeinfo.tm_mday = 1; }
		val = _ConvertStringToUi32(&ptr);			if ((val >= 0) && (val < 24))		{ timeinfo.tm_hour = val;			} else { timeinfo.tm_hour = 0; }
		val = _ConvertStringToUi32(&ptr);			if ((val >= 0) && (val < 60))		{ timeinfo.tm_min = val;			} else { timeinfo.tm_min = 0; }
		val = _ConvertStringToUi32(&ptr);			if ((val >= 0) && (val < 60))		{ timeinfo.tm_sec = val;			} else { timeinfo.tm_sec = 0; }
		return(mktime(&timeinfo));
	}


	public:int _Connect(char Srv[], char Usr[], char Pass[], char DB[]) {
		_Sprintf(_Server, Srv);
		_Sprintf(_User, Usr);
		_Sprintf(_Password, Pass);
		_Sprintf(_Database, DB);

		connect = mysql_init(0);
		if (connect)
		{
#ifdef WIN32
			printf( "Database Connected\n" );
			printf( "Press any key to continue...\n" );
			//getch();
			system("cls");
#endif
		}
		else {
#ifdef WIN32
			printf("Failed To Connect! %d \n", mysql_errno(connect) );
#endif
		}
		connect = mysql_real_connect(connect, _Server, _User, _Password, _Database, 0, NULL, 0);
		if (connect)
		{
#ifdef WIN32
			printf( "Database Connected To MySql %d \n" , connect );
			printf( "Press any key to continue... \n" );
			//getch();
			system("cls");
#endif
		}
		else {
#ifdef WIN32
			printf( "Failed To Connect! %d \n" , mysql_errno(connect) );
#endif
		}
		return 1;
	}
	public:int _AddNewGateway(char Admin[], char Pass[], Ui32 Unique[], Ui64 Mac, Ui32 ConnId, Ui32 CertificateID, Ui08 *pSSK, Ui08 WhatToDo) {		
		time_t	Time = time(0);
		struct tm * timeinfo = localtime(&Time);
		string list,object,column;
		int result;
		char AdminBuff[45];
		char PassBuff[45];

		if (strlen(Admin) > 1) {
			strcpy(AdminBuff, Admin);
			strcpy(PassBuff, Pass);
		}
		else {
			strcpy(AdminBuff, "unknown");
			strcpy(PassBuff, "1234");
		}
		/*
		Find Admin and test password from UserList
		*/
		list = UserList;
		object = AdminBuff;
		column = "UserName";
		qstate = MYSQL_Search(list, column, object);

		if (!qstate)
		{
			res = mysql_store_result(connect);
			if ((row = mysql_fetch_row(res)))///< existing user
			{
#ifdef WIN32
				cout << "ID: " << row[USR_id] << "\nRoles: " << row[USR_Rls] << "\nName: " << row[USR_Nam] << "\nPassword: " << row[USR_Pwd] << "\nQuestion: " << row[USR_RcQ] << "\nAnswer: " << row[USR_Ans] << "\nAvatar: " << row[USR_Atr] << "\nPicture: " << row[USR_Pic] << endl << endl;
#endif				
				mysql_free_result(res);
				
				if (_String_Compare(row[USR_Pwd], PassBuff) == TRUE) {///< PASSWORD CORRECT
					/*
						Search UniqueID from GatewayList if any result test Credits and process request; else add as new gateway						
					*/
					GATEWAY_UserIDX = (Ui32)_ConvertStringToUi64(row[USR_id]);
					result = 0;
					_Temp[0] = 0;	_ConvertUi32AddToStr(Unique[0], &_Temp[0]);
					list = Table;
					object = _Temp;
					column = GW_L_Cs[GWL_Uq0];// "UNIQUE_0";
					qstate = MYSQL_Search(list, column, object);
					if (!qstate)
					{
						res = mysql_store_result(connect);
						while ((row = mysql_fetch_row(res)))///< UNIQUE_0 Match
						{
							_Temp[0] = 0;	_ConvertUi32AddToStr(Unique[1], &_Temp[0]);
							if (_String_Compare(row[GWL_Uq1], _Temp) == TRUE)///< UNIQUE_1 Match
							{
								_Temp[0] = 0;	_ConvertUi32AddToStr(Unique[2], &_Temp[0]);
								if (_String_Compare(row[GWL_Uq2], _Temp) == TRUE)///< UNIQUE_2 Match
								{
									GATEWAY_GatewayIDX = (Ui32) _ConvertStringToUi64(row[GWL_Id]);
									GATEWAY_DeviceList = (Ui32) _ConvertStringToUi64(row[GWL_Div]);
#ifdef WIN32
									cout << "MySQL Gateway = " << row[GWL_Id]  << " DeviceList = " << row[GWL_Div] << mysql_errno(connect) << endl;
#endif
									result = 1;
									break;
								}
							}
						}
						if (result == 0) {	
							memcpy(&_SessionKeyTemp[0], pSSK, 24);
							_Start_AddRow((char *)Table,MYSQL_AUTO_ID_);
							_String_Add((char *)"Undefined Gateway");		_Break_Add();	///< name
							_Ui32_Array_Add(&Unique[0], 3);					_Break_Add();	///< unique id
							_Ui64_Add(Mac);									_Break_Add();	///< mac
							_Ui32_Add(GATEWAY_UserIDX);						_Break_Add();	///<user id
							_Ui32_Add(CertificateID);						_Break_Add();	///<certificate id
							_Ui32_Add(ConnId);								_Break_Add();	///< conneciton id
							_Ui64_Array_Add(&_SessionKeyTemp[0], 3);		_Break_Add();	///< session key
							_Ui32_Add(0);									_Break_Add();	///< packet
							_Ui32_Add(0);									_Break_Add();	///< total
							_Ui32_Add(0);									_Break_Add();	///< send 
							_Ui32_Add(0);									_Break_Add();	///< received
							_Time_Add(_TimeToMysqTime(localtime(&Time)));	_Break_Add();	///< first conn
							_Time_Add(_TimeToMysqTime(localtime(&Time)));	_Break_Add();	///< last conn
							_Ui32_Add(0);													///< Undefined DEvice List
							_Stop_Add();

							mysql_query(connect, (const char *)&_Temp[0]);
#ifdef WIN32
							cout << "MySQL Initialization or Connection new " << mysql_errno(connect) << endl;
#endif
							mysql_free_result(res);
						}
						else if (WhatToDo == _GATEWAY_KEEP_) {
#ifdef WIN32
							cout << "Gateway Found - KEEP IT  " << endl;
#endif
							mysql_free_result(res);
						}
						else {
							_UpdateCellStart((char *)Table);
							_UpdateCellValue((char *)GW_L_Cs[GWL_Mac], _ConvertUi64ToStrRet(Mac, &_Temp0[0]));
							_UpdateCellValue((char *)GW_L_Cs[GWL_Adm], _ConvertUi32ToStrRet(GATEWAY_UserIDX, &_Temp0[0]));
							_UpdateCellValue((char *)GW_L_Cs[GWL_cer], _ConvertUi32ToStrRet(CertificateID, &_Temp0[0]));
							_UpdateCellValue((char *)GW_L_Cs[GWL_con], _ConvertUi32ToStrRet(ConnId, &_Temp0[0]));
							memcpy(&_SessionKeyTemp[0], pSSK, 24);
							_UpdateCellValue((char *)GW_L_Cs[GWL_SK0], _ConvertUi64ToStrRet(_SessionKeyTemp[0], &_Temp0[0]));
							_UpdateCellValue((char *)GW_L_Cs[GWL_SK1], _ConvertUi64ToStrRet(_SessionKeyTemp[1], &_Temp0[0]));
							_UpdateCellValue((char *)GW_L_Cs[GWL_SK2], _ConvertUi64ToStrRet(_SessionKeyTemp[2], &_Temp0[0]));
							if ((WhatToDo == _GATEWAY_RESET_)|| (WhatToDo == _GATEWAY_REINIT_)) {
								_UpdateCellValue((char *)GW_L_Cs[GWL_pkt], _ConvertUi32ToStrRet(0, &_Temp0[0]));
								_UpdateCellValue((char *)GW_L_Cs[GWL_Tot], _ConvertUi32ToStrRet(0, &_Temp0[0]));
								_UpdateCellValue((char *)GW_L_Cs[GWL_SdB], _ConvertUi32ToStrRet(0, &_Temp0[0]));
								_UpdateCellValue((char *)GW_L_Cs[GWL_RvB], _ConvertUi32ToStrRet(0, &_Temp0[0]));
								_UpdateCellValue((char *)GW_L_Cs[GWL_Fst], _Time_Add0(_TimeToMysqTime(localtime(&Time)), &_Temp0[0], 0));
								_UpdateCellValue((char *)GW_L_Cs[GWL_Lst], _Time_Add0(_TimeToMysqTime(localtime(&Time)), &_Temp0[0], 0));
								_UpdateCellValue((char *)GW_L_Cs[GWL_Div], _ConvertUi32ToStrRet(0, &_Temp0[0]));
							}
							else {
								_UpdateCellValue((char *)GW_L_Cs[GWL_Lst], _Time_Add0(_TimeToMysqTime(localtime(&Time)), &_Temp0[0], 0));
							}
							_UpdateCellEnd((char *)GW_L_Cs[GWL_Id], GATEWAY_GatewayIDX);
							mysql_query(connect, (const char *)&_Temp[0]);
#ifdef WIN32
							cout << "Gateway Found - CHANGE == " << _Temp << mysql_errno(connect) << endl;
#endif
							mysql_free_result(res);
						}
						_QueryAndLoadGateway(NULL, Unique, NULL, NULL, MYSQL_QUERY_BY_UNIQUE_);///< LOAD TO Gateway Instant Structure
						_TestCreateNewDeviceList(Gateway._id);
						return TRUE;
					}
					else
					{
#ifdef WIN32
						cout << column << "Query Execution Problem!" << mysql_errno(connect) << endl;
#endif
						return ERROR_APPEXEC_HOST_ID_MISMATCH;
					}
				}
				else {///< WRONG PASSWORD !!!
#ifdef WIN32
					cout << "WRONG PASSWORD !!!  " << row[3] << " -- " << PassBuff << endl;
#endif
					return ERROR_WRONG_PASSWORD;
				}			
			}
			else {///< UNKNOWN USER !!!
				return ERROR_APPEXEC_UNKNOWN_USER;
			}
		}
		else
		{
#ifdef WIN32
			cout << "Query Execution Problem!" << mysql_errno(connect) << endl;
#endif
			return ERROR_APPEXEC_HOST_ID_MISMATCH;
		}
		return TRUE;
	}
	public:int _CloseConnect(void) {
		mysql_close(connect);
		return 1;
	}
	public:int _QueryAndLoadGateway(Ui32 id, Ui32 Unique[], Ui64 Mac, Ui32 ConnId, char QueryType) {
		string list, object, column;
		int result;

		result = 0;
		switch (QueryType) {
		case MYSQL_QUERY_BY_ID_:
			_ConvertUi32ToStr(id, &_Temp[0]);
			list = Table;
			object = _Temp;
			column = (char *)GW_L_Cs[GWL_Id];
			qstate = MYSQL_Search(list, column, object);
			if (!qstate)
			{
				res = mysql_store_result(connect);
				if ((row = mysql_fetch_row(res)))///< ID Match
				{					
					result = 1;
				}
				mysql_free_result(res);
			}
			else
			{
#ifdef WIN32
				cout << column << "Query Execution Problem!" << mysql_errno(connect) << endl;
#endif
				return ERROR_APPEXEC_HOST_ID_MISMATCH;
			}
		break;
		case MYSQL_QUERY_BY_MAC_:
			_ConvertUi64ToStr(Mac, &_Temp[0]);
			list = Table;
			object = _Temp;
			column = (char *)GW_L_Cs[GWL_Mac];
			qstate = MYSQL_Search(list, column, object);
			if (!qstate)
			{
				res = mysql_store_result(connect);
				if ((row = mysql_fetch_row(res)))///< ID Match
				{
					result = 1;
				}
				mysql_free_result(res);
			}
			else
			{
#ifdef WIN32
				cout << column << "Query Execution Problem!" << mysql_errno(connect) << endl;
#endif
				return ERROR_APPEXEC_HOST_ID_MISMATCH;
			}
		break;
		case MYSQL_QUERY_BY_UNIQUE_:
			_ConvertUi32ToStr(Unique[0], &_Temp[0]);
			list = Table;
			object = _Temp;
			column = (char *)GW_L_Cs[GWL_Uq0];
			qstate = MYSQL_Search(list, column, object);
			if (!qstate)
			{
				res = mysql_store_result(connect);
				while ((row = mysql_fetch_row(res)))///< UNIQUE_0 Match
				{
					_ConvertUi32ToStr(Unique[1], &_Temp[0]);
					if (_String_Compare(row[GWL_Uq1], _Temp) == TRUE)///< UNIQUE_1 Match
					{
						_ConvertUi32ToStr(Unique[2], &_Temp[0]);
						if (_String_Compare(row[GWL_Uq2], _Temp) == TRUE)///< UNIQUE_2 Match
						{
							result = 1;
							break;
						}
					}
				}
				mysql_free_result(res);
			}
			else
			{
#ifdef WIN32
				cout << column << "Query Execution Problem!" << mysql_errno(connect) << endl;
#endif
				return ERROR_APPEXEC_HOST_ID_MISMATCH;
			}
		break;
		case MYSQL_QUERY_BY_CONN_:
			_ConvertUi32ToStr(ConnId, &_Temp[0]);
			list = Table;
			object = _Temp;
			column = (char *)GW_L_Cs[GWL_con];
			qstate = MYSQL_Search(list, column, object);
			if (!qstate)
			{
				res = mysql_store_result(connect);
				if ((row = mysql_fetch_row(res)))///< ID Match
				{
					result = 1;
				}
				mysql_free_result(res);
			}
			else
			{
#ifdef WIN32
				cout << column << "Query Execution Problem!" << mysql_errno(connect) << endl;
#endif
				return ERROR_APPEXEC_HOST_ID_MISMATCH;
			}
		break;
		};
		if (result == 1) {
			Gateway._id					= (Ui32) _ConvertStringToUi64(row[GWL_Id]);
			_Sprintf(&Gateway._Name[0], row[GWL_Nm]);
			Gateway._Unique[0]			= (Ui32)_ConvertStringToUi64(row[GWL_Uq0]);
			Gateway._Unique[1]			= (Ui32)_ConvertStringToUi64(row[GWL_Uq1]);
			Gateway._Unique[2]			= (Ui32)_ConvertStringToUi64(row[GWL_Uq2]);
			Gateway._MAC				= (Ui64)_ConvertStringToUi64(row[GWL_Mac]);
			Gateway._Admin				= (Ui32)_ConvertStringToUi64(row[GWL_Adm]);
			Gateway._CertificateId		= (Ui32)_ConvertStringToUi64(row[GWL_cer]);
			Gateway._ConnectionId		= (Ui32)_ConvertStringToUi64(row[GWL_con]);
			Gateway._SessionKey[0]		= (Ui64)_ConvertStringToUi64(row[GWL_SK0]);
			Gateway._SessionKey[0]		= (Ui64)_ConvertStringToUi64(row[GWL_SK1]);
			Gateway._SessionKey[0]		= (Ui64)_ConvertStringToUi64(row[GWL_SK2]);
			Gateway._PacketCount		= (Ui32)_ConvertStringToUi64(row[GWL_pkt]);
			Gateway._TotalPackets		= (Ui32)_ConvertStringToUi64(row[GWL_Tot]);
			Gateway._SendBytes			= (Ui32)_ConvertStringToUi64(row[GWL_SdB]);
			Gateway._ReceivedBytes		= (Ui32)_ConvertStringToUi64(row[GWL_RvB]);
			Gateway._DeviceListId		= (Ui32)_ConvertStringToUi64(row[GWL_Div]);
			Gateway._FirstConnectTime	= (time_t)_MYSQL_TimeToTime(row[GWL_Fst]);
			Gateway._LastConnectTime	= (time_t)_MYSQL_TimeToTime(row[GWL_Lst]);
#ifdef WIN32
			cout << "MySQL Gateway = " << row[GWL_Id] << " DeviceList = " << row[GWL_Div] << mysql_errno(connect) << endl;
#endif
		}
		else {
#ifdef WIN32
			cout << "There is no match found !!! "  << endl;
#endif
			return ERROR_NO_MATCH;
		}
		return TRUE;
	}
	public:int _ReloadGatewayListRow(Ui32 Flgs) {
		_UpdateCellStart((char *)Table);
		//_UpdateCellValue((char *)GW_L_Cs[GWL_Id], _ConvertUi32ToStrRet(Gateway._id, &_Temp0[0]), 1);
		if (Flgs&(1<< GWL_Nm))	{	_UpdateCellValue((char *)GW_L_Cs[GWL_Nm], &Gateway._Name[0]);																	}
		if (Flgs&(1<< GWL_Uq0)) {	_UpdateCellValue((char *)GW_L_Cs[GWL_Uq0], _ConvertUi32ToStrRet(Gateway._Unique[0], &_Temp0[0]));								}
		if (Flgs&(1<< GWL_Uq1)) {	_UpdateCellValue((char *)GW_L_Cs[GWL_Uq1], _ConvertUi32ToStrRet(Gateway._Unique[1], &_Temp0[0]));								}
		if (Flgs&(1<< GWL_Uq2)) {	_UpdateCellValue((char *)GW_L_Cs[GWL_Uq2], _ConvertUi32ToStrRet(Gateway._Unique[2], &_Temp0[0]));								}
		if (Flgs&(1<< GWL_Mac)) {	_UpdateCellValue((char *)GW_L_Cs[GWL_Mac], _ConvertUi64ToStrRet(Gateway._MAC, &_Temp0[0]));										}
		if (Flgs&(1<< GWL_Adm)) {	_UpdateCellValue((char *)GW_L_Cs[GWL_Adm], _ConvertUi32ToStrRet(Gateway._Admin, &_Temp0[0]));									}
		if (Flgs&(1<< GWL_cer)) {	_UpdateCellValue((char *)GW_L_Cs[GWL_cer], _ConvertUi32ToStrRet(Gateway._CertificateId, &_Temp0[0]));							}
		if (Flgs&(1<< GWL_con)) {	_UpdateCellValue((char *)GW_L_Cs[GWL_con], _ConvertUi32ToStrRet(Gateway._ConnectionId, &_Temp0[0]));							}
		if (Flgs&(1<< GWL_SK0)) {	_UpdateCellValue((char *)GW_L_Cs[GWL_SK0], _ConvertUi64ToStrRet(Gateway._SessionKey[0], &_Temp0[0]));							}
		if (Flgs&(1<< GWL_SK1)) {	_UpdateCellValue((char *)GW_L_Cs[GWL_SK1], _ConvertUi64ToStrRet(Gateway._SessionKey[1], &_Temp0[0]));							}
		if (Flgs&(1<< GWL_SK2)) {	_UpdateCellValue((char *)GW_L_Cs[GWL_SK2], _ConvertUi64ToStrRet(Gateway._SessionKey[2], &_Temp0[0]));							}
		if (Flgs&(1<< GWL_pkt)) {	_UpdateCellValue((char *)GW_L_Cs[GWL_pkt], _ConvertUi32ToStrRet(Gateway._PacketCount, &_Temp0[0]));								}
		if (Flgs&(1<< GWL_Tot)) {	_UpdateCellValue((char *)GW_L_Cs[GWL_Tot], _ConvertUi32ToStrRet(Gateway._TotalPackets, &_Temp0[0]));							}
		if (Flgs&(1<< GWL_SdB)) {	_UpdateCellValue((char *)GW_L_Cs[GWL_SdB], _ConvertUi32ToStrRet(Gateway._SendBytes, &_Temp0[0]));								}
		if (Flgs&(1<< GWL_RvB)) {	_UpdateCellValue((char *)GW_L_Cs[GWL_RvB], _ConvertUi32ToStrRet(Gateway._ReceivedBytes, &_Temp0[0]));							}
		if (Flgs&(1<< GWL_Fst)) {	_UpdateCellValue((char *)GW_L_Cs[GWL_Fst], _Time_Add0(_TimeToMysqTime(localtime(&Gateway._FirstConnectTime)), &_Temp0[0], 0));	}
		if (Flgs&(1<< GWL_Lst)) {	_UpdateCellValue((char *)GW_L_Cs[GWL_Lst], _Time_Add0(_TimeToMysqTime(localtime(&Gateway._LastConnectTime)), &_Temp0[0], 0));	}
		if (Flgs&(1<< GWL_Div)) {	_UpdateCellValue((char *)GW_L_Cs[GWL_Div], _ConvertUi32ToStrRet(Gateway._DeviceListId, &_Temp0[0]));							}

		_UpdateCellEnd((char *)GW_L_Cs[GWL_Id], Gateway._id);
		mysql_query(connect, (const char *)&_Temp[0]);
		return TRUE;
	}

	public:int _DeleteDeviceList(Ui32 Id) {
		const char DeviceListString2[] = "DROP TABLE DeviceList_";	
		_Sprintf(&_Temp[0], (char *)&DeviceListString2[0]);
		_ConvertUi32AddToStr(Id, &_Temp[0]);
		mysql_query(connect, (const char *)&_Temp[0]);
#ifdef WIN32
		cout << "MySQL Delete Device List Error No : " << mysql_errno(connect) << endl;
#endif
		return TRUE;
	}

	public:int _CreateNewDeviceList(Ui32 Id, char Delete) {
		const char DeviceListString0[] = "CREATE TABLE DeviceList_";
		const char DeviceListString1[] = " (\
			id INT(10) UNSIGNED AUTO_INCREMENT PRIMARY KEY,\
			NAME VARCHAR(45) NOT NULL,\
			UNIQUE_0 INT(10) UNSIGNED NOT NULL,\
			UNIQUE_1 INT(10) UNSIGNED NOT NULL,\
			UNIQUE_2 INT(10) UNSIGNED NOT NULL,\
			PARENT INT(10) UNSIGNED NOT NULL,\
			TYPE INT(10) UNSIGNED NOT NULL,\
			LocalIP INT(10) UNSIGNED NOT NULL,\
			Status INT(10) UNSIGNED NOT NULL,\
			PACKET_CNT INT(10) UNSIGNED NOT NULL,\
			TOTAL_PACKETS INT(10) UNSIGNED NOT NULL,\
			SEND_BYTES INT(10) UNSIGNED NOT NULL,\
			RCV_BYTES INT(10) UNSIGNED NOT NULL\
			)";
		if (Delete == _LIST_DELETE) {
			_DeleteDeviceList(Id);
			//Sleep(100);
		}
		_Sprintf(&_Temp[0], (char *)&DeviceListString0[0]);
		_ConvertUi32AddToStr(Id, &_Temp[0]);
		_Sprintf_Add(&_Temp[0], (char *)&DeviceListString1[0]);
		mysql_query(connect, (const char *)&_Temp[0]);
#ifdef WIN32
		cout << "MySQL Create New Device List Error No : " << mysql_errno(connect) << endl;
#endif
		//Sleep(100);
		return TRUE;
	}

	public:int _TestCreateNewDeviceList(Ui32 Id) {
		const char DeviceListString5[] = "SELECT 1 FROM DeviceList_";
		const char DeviceListString6[] = " LIMIT 1";
		int result;
		_Sprintf(&_Temp[0], (char *)&DeviceListString5[0]);
		_ConvertUi32AddToStr(Id, &_Temp[0]);
		_Sprintf_Add(&_Temp[0], (char *)&DeviceListString6[0]);
		mysql_query(connect, (const char *)&_Temp[0]);
		result = (Ui32)mysql_errno(connect);// _ConvertStringToUi64(mysql_errno(connect));
		if (result != 0) {
			_CreateNewDeviceList(Id, _LIST_KEEP);
		}
		else {
			res = mysql_store_result(connect);
			mysql_free_result(res);
		}
		return result;
	}

	public:int _LoadDeviceInfo(Ui32 gId, Ui32 Id) {
		int result = 0;
		string list, object, column;

		_Sprintf(&_Temp[0], (char *)&DevListStr[0]);
		if (gId == 0) { gId = Gateway._id; }///< loaded before
		_ConvertUi32AddToStr(gId, &_Temp[0]);
		list = _Temp;
		_ConvertUi32ToStr(Id, &_Temp0[0]);
		object = _Temp0;
		column = (char *)DV_L_Cs[DVL_id];
		qstate = MYSQL_Search(list, column, object);
		if (!qstate)
		{
			res = mysql_store_result(connect);
			if ((row = mysql_fetch_row(res)))///< ID Match
			{
				Device._id				= (Ui32)_ConvertStringToUi64(row[DVL_id]);
				_Sprintf(&Device._Name[0], row[DVL_Nm]);
				Device._Unique[0]		= (Ui32)_ConvertStringToUi64(row[DVL_Uq0]);
				Device._Unique[1]		= (Ui32)_ConvertStringToUi64(row[DVL_Uq1]);
				Device._Unique[2]		= (Ui32)_ConvertStringToUi64(row[DVL_Uq2]);
				Device._ParentId		= (Ui32)_ConvertStringToUi64(row[DVL_pId]);
				Device._Type			= (Ui32)_ConvertStringToUi64(row[DVL_Typ]);
				Device._LocalIp			= (Ui32)_ConvertStringToUi64(row[DVL_LIP]);
				Device._Status			= (Ui32)_ConvertStringToUi64(row[DVL_Stt]);
				Device._PacketCount		= (Ui32)_ConvertStringToUi64(row[DVL_pkt]);
				Device._TotalPackets	= (Ui32)_ConvertStringToUi64(row[DVL_Tot]);
				Device._SendBytes		= (Ui32)_ConvertStringToUi64(row[DVL_SdB]);
				Device._ReceivedBytes	= (Ui32)_ConvertStringToUi64(row[DVL_RvB]);
				result = 1;
			}
			else {
#ifdef WIN32
				cout << list << "MySQL Device Not Found !!! Error No : " << mysql_errno(connect) << endl;
#endif

			}
			mysql_free_result(res);
			return result;
		}
		else {
#ifdef WIN32
			cout << list << "MySQL Device List Not Found !!! Error No : " << mysql_errno(connect) << endl;
#endif
			return 0;
		}

	}

	public:int _ReadDeviceList(Ui32 Id, AYSQL_DeviceRead *pReadFile, Ui32 MaxCnt, Ui32 StrtIdx) {
		Ui32 i=0;
		string list;

		_Sprintf(&_Temp[0], (char *)&DevListStr[0]);
		if (Id == 0) {	Id = Gateway._id;	}///< loaded before
		if (MaxCnt == 0) { MaxCnt = 0xFFFFFFFF; }///< read all
		_ConvertUi32AddToStr(Id, &_Temp[0]);

		list = _Temp;
		qstate = MYSQL_ReadAllRows(list);
		if (!qstate)
		{
			res = mysql_store_result(connect);
			while ((row = mysql_fetch_row(res)))///< ID Match
			{
				if (i >= StrtIdx) {
					pReadFile->_id = (Ui32)_ConvertStringToUi64(row[DVL_id]);
					pReadFile->_Unique[0] = (Ui32)_ConvertStringToUi64(row[DVL_Uq0]);
					pReadFile->_Unique[1] = (Ui32)_ConvertStringToUi64(row[DVL_Uq1]);
					pReadFile->_Unique[2] = (Ui32)_ConvertStringToUi64(row[DVL_Uq2]);
					pReadFile->_LocalIp = (Ui32)_ConvertStringToUi64(row[DVL_LIP]);
					pReadFile->_ParentId = (Ui32)_ConvertStringToUi64(row[DVL_pId]);
					pReadFile->_Type = (Ui32)_ConvertStringToUi64(row[DVL_Typ]);
					pReadFile ++;
#ifdef WIN32
					cout << "MySQL Found Device id = " << row[DVL_id] << " _Unique0 = " << row[DVL_Uq0] << " _Unique1 = " << row[DVL_Uq1] << " _Unique2 = " << row[DVL_Uq2] << " NAME = " << row[DVL_Nm] << endl;
#endif
				}
				i++;
				if (i >= (MaxCnt+ StrtIdx)) {
					break;
				}
			}
			mysql_free_result(res);
#ifdef WIN32
			cout << "MySQL Total Found Device i = " << _ConvertUi32ToStrRet((i- StrtIdx), &_Temp0[0]) << endl;
#endif
			return (i- StrtIdx);
		}
		else
		{
#ifdef WIN32
			cout << list << "MySQL Device List Not Found !!! Error No : " << mysql_errno(connect) << endl;
#endif
			return 0;
		}
	}

	public:int _LoadDeviceInfo(Ui32 id, Ui32 Unique[]) {
		if (_QueryAndLoadGateway(NULL, Unique, NULL, NULL, MYSQL_QUERY_BY_UNIQUE_) == TRUE) {
			if(id != 0){ 
				_LoadDeviceInfo(Gateway._id, id);
			}
			return Gateway._ConnectionId;
		}
		else {
			return 0;
		}

	}
	#define FindConnectionIdOfGateway(Unique)		_LoadDeviceInfo(0, Unique)
};// MySqlConn;





