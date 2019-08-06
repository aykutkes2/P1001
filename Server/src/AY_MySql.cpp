//#include "pch.h"
#include <ctime>
#include <iostream>
#include <AY_Type.h>
#include <AY_Printf.h>
#include <AY_MySql.h>

#include "mysql.h" 

//#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>
using namespace std;

MYSQL_CONNECT	MySqlConn;
Ui32			GATEWAY_UserIDX, GATEWAY_GatewayIDX;
Ui32			GATEWAY_DeviceList;
Ui64			MYSQL_SessionKeyTemp[3];
char			MYSQL_Server[22];
char			MYSQL_User[22];
char			MYSQL_Password[22];
char			MYSQL_Database[22];
char			MYSQL_Temp[512];
char			MYSQL_Temp0[45];
int				qstate;
MYSQL_ROW		row;
MYSQL_RES		*res;
MYSQL			*MYSQL_Conn;// = mysql_init(NULL);
//------------------------------//
const char GW_L_Cs[19][20] = { "idGatewayList_001", "NAME", "UNIQUE_0", "UNIQUE_1", "UNIQUE_2", "MAC", "ADMIN", "CERTIFICATE_ID", "CONNECTION_ID", \
	"SESSION_KEY_0", "SESSION_KEY_1", "SESSION_KEY_2", "PACKET_CNT", "TOTAL_PACKETS", "SEND_BYTES", "RCV_BYTES", "FIRST_CONN", "LAST_CONN", "DEVICE_LIST" };
const char DV_L_Cs[13][20] = { "id", "NAME", "UNIQUE_0", "UNIQUE_1", "UNIQUE_2", "PARENT", "TYPE", "LocalIP", "Status", "PACKET_CNT", "TOTAL_PACKETS", "SEND_BYTES", "RCV_BYTES" };

//char Schema[45];// = "devicelist_001";
char MYSQL_Table[45];// = "gatewaylist_001";
char MYSQL_UserList[45];// = "user_list_001";
const char DevListStr[12] = "DeviceList_";


Si64 MYSQL_ConvertStringToSi64(char *ptr) {
	Si64 Rslt = 0;
	Ui08 Sign = 0;
	while (*ptr == ' ') { ptr++; }
	if (*ptr == '-') {
		Sign = 1;
		ptr++;
	}
	while ((*ptr >= '0') && (*ptr <= '9')) {
		Rslt = (Rslt * 10) + (*ptr - '0');
		ptr++;
	}
	if (Sign) { Rslt = 0 - Rslt; }
	return (Rslt);
}
Ui64 MYSQL_ConvertStringToUi64(char *ptr) {
	Ui64 Rslt = 0;
	while (*ptr == ' ') { ptr++; }
	while ((*ptr >= '0') && (*ptr <= '9')) {
		Rslt = (Rslt * 10) + (*ptr - '0');
		ptr++;
	}
	return (Rslt);
}
Ui32 MYSQL_ConvertStringToUi32(char **ptr) {
	Ui32 Rslt = 0;
	Ui08 i = 0;
	char *p;
	p = *ptr;
	while ((++i < 200) && (*p != 0x00) && (!((*p >= '0') && (*p <= '9')))) { p++; }
	while ((*p >= '0') && (*p <= '9')) {
		Rslt = (Rslt * 10) + (*p - '0');
		p++;
	}
	*ptr = p;
	return (Rslt);
}
char *MYSQL_ConvertUi32AddToStr0(Ui32 Val, char *ptr, char Add, char RetStrt) {
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
//#define MYSQL_ConvertUi32AddToStr(Val, ptr)		MYSQL_ConvertUi32AddToStr0(Val, ptr, 1, 0)
//#define MYSQL_ConvertUi32ToStr(Val, ptr)			MYSQL_ConvertUi32AddToStr0(Val, ptr, 0, 0)
//#define MYSQL_ConvertUi32ToStrRet(Val, ptr)		MYSQL_ConvertUi32AddToStr0(Val, ptr, 0, 1)
char *MYSQL_ConvertUi64AddToStr0(Ui64 Val, char *ptr, char Add, char RetStrt) {
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
//#define MYSQL_ConvertUi64AddToStr(Val, ptr)		MYSQL_ConvertUi64AddToStr0(Val, ptr, 1,0)
//#define MYSQL_ConvertUi64ToStr(Val, ptr)			MYSQL_ConvertUi64AddToStr0(Val, ptr, 0,0)
//#define MYSQL_ConvertUi64ToStrRet(Val, ptr)		MYSQL_ConvertUi64AddToStr0(Val, ptr, 0,1)
void MYSQL_Sprintf(char *pDst, char *pSrc) {
	int i = 0;
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
void MYSQL_Sprintf_Add(char *pDst, char *pSrc) {
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
void MYSQL_Start_AddRow(char *pTable, char _nll) {
	MYSQL_Sprintf(&MYSQL_Temp[0], (char *)"INSERT INTO ");
	MYSQL_Sprintf_Add(&MYSQL_Temp[0], pTable);
	if (_nll == MYSQL_AUTO_ID_) {
		MYSQL_Sprintf_Add(&MYSQL_Temp[0], (char *)" VALUES ( NULL ,'");
	}
	else {
		MYSQL_Sprintf_Add(&MYSQL_Temp[0], (char *)" VALUES ( '");
	}
	return;
}
void MYSQL_UpdateCellValue(char *pCellName, char *pNewValue) {
	//UPDATE `devicelist_001`.`gatewaylist_001` SET `CERTIFICATE ID` = '2' WHERE(`idGatewayList_001` = '6');
	if (MySqlConn._Flgs._ThisIsFirstF == 0) { MYSQL_Sprintf_Add(&MYSQL_Temp[0], (char *)","); }
	else { MySqlConn._Flgs._ThisIsFirstF = 0; }
	MYSQL_Sprintf_Add(&MYSQL_Temp[0], (char *)" ");
	MYSQL_Sprintf_Add(&MYSQL_Temp[0], pCellName);
	MYSQL_Sprintf_Add(&MYSQL_Temp[0], (char *)" = '");
	MYSQL_Sprintf_Add(&MYSQL_Temp[0], pNewValue);
	MYSQL_Sprintf_Add(&MYSQL_Temp[0], (char *)"' ");
}
void MYSQL_UpdateCellStart(char *pTable) {
	//UPDATE `devicelist_001`.`gatewaylist_001` SET `CERTIFICATE ID` = '2' WHERE(`idGatewayList_001` = '6');
	MYSQL_Sprintf(&MYSQL_Temp[0], (char *)"UPDATE ");
	MYSQL_Sprintf_Add(&MYSQL_Temp[0], pTable);
	MYSQL_Sprintf_Add(&MYSQL_Temp[0], (char *)" SET");
	MySqlConn._Flgs._ThisIsFirstF = 1;

}
void MYSQL_UpdateCellEnd(char *pIdName, Ui64 _RowId) {
	//UPDATE `devicelist_001`.`gatewaylist_001` SET `CERTIFICATE ID` = '2' WHERE(`idGatewayList_001` = '6');
	MYSQL_Sprintf_Add(&MYSQL_Temp[0], (char *)"WHERE ");
	MYSQL_Sprintf_Add(&MYSQL_Temp[0], pIdName);
	MYSQL_Sprintf_Add(&MYSQL_Temp[0], (char *)" = '");
	MYSQL_Ui64_Add(_RowId);
	MYSQL_Sprintf_Add(&MYSQL_Temp[0], (char *)"'");
}
void MYSQL_Ui32_Add(Ui32 Val) {
	MYSQL_ConvertUi32AddToStr(Val, &MYSQL_Temp[0]);
	return;
}
void MYSQL_Ui64_Add(Ui64 Val) {
	MYSQL_ConvertUi64AddToStr(Val, &MYSQL_Temp[0]);
	return;
}
void MYSQL_Stop_Add(void) {
	MYSQL_Sprintf_Add(&MYSQL_Temp[0], (char *)"')");
	return;
}
void MYSQL_Break_Add(void) {
	MYSQL_Sprintf_Add(&MYSQL_Temp[0], (char *)"' , '");
	return;
}
void MYSQL_String_Add(char *Str) {
	MYSQL_Sprintf_Add(&MYSQL_Temp[0], Str);
	return;
}
void MYSQL_Ui32_Array_Add(Ui32 Val[], Ui08 Cnt) {
	Ui08 i;
	for (i = 0; i < Cnt; i++) {
		MYSQL_ConvertUi32AddToStr(Val[i], &MYSQL_Temp[0]);
		if (i < (Cnt - 1)) {
			MYSQL_Sprintf_Add(&MYSQL_Temp[0], (char *)"','");
		}
	}
	return;
}
void MYSQL_Ui64_Array_Add(Ui64 Val[], Ui08 Cnt) {
	Ui08 i;
	for (i = 0; i < Cnt; i++) {
		MYSQL_ConvertUi64AddToStr(Val[i], &MYSQL_Temp[0]);
		if (i < (Cnt - 1)) {
			MYSQL_Sprintf_Add(&MYSQL_Temp[0], (char *)"','");
		}
	}
	return;
}
int MYSQL_String_Compare(char *Str0, char *Str1) {
	int i = 0;
	while ((*Str0 != 0) && (*Str1 != 0) && (i < 255)) {
		if (*Str0 != *Str1) {
			return APPX_E_FILE_COMPRESSION_MISMATCH;
		}
		Str0++;
		Str1++;
		i++;
	}
	return TRUE;
}
int MYSQL_Search(string pListx, string pColumn, string pObject) {

	string findbyname_query = "select * from " + pListx + " where " + pColumn + " = '" + pObject + "'";
	const char* qn = findbyname_query.c_str();
	return(mysql_query(MYSQL_Conn, qn));
}
int MYSQL_ReadAllRows(string pListx) {

	string findbyname_query = "select * from " + pListx;
	const char* qn = findbyname_query.c_str();
	return(mysql_query(MYSQL_Conn, qn));
}
MYSQL_TIME MYSQL_TimeToMysqTime(struct tm * timeinfo) {
	MYSQL_TIME time = { (unsigned int)(timeinfo->tm_year + 1900), (unsigned int)timeinfo->tm_mon, (unsigned int)timeinfo->tm_mday, (unsigned int)timeinfo->tm_hour, (unsigned int)timeinfo->tm_min, (unsigned int)timeinfo->tm_sec, 0, 0, MYSQL_TIMESTAMP_NONE };
	return(time);
}
char *MYSQL_Time_Add0(MYSQL_TIME time, char *pStr, char Add) {
	char *p = pStr;
	MYSQL_ConvertUi32AddToStr0(time.year, pStr, Add, 1);
	MYSQL_Sprintf_Add(pStr, (char *)"/");
	MYSQL_ConvertUi32AddToStr((time.month + 1), pStr);
	MYSQL_Sprintf_Add(pStr, (char *)"/");
	MYSQL_ConvertUi32AddToStr(time.day, pStr);
	MYSQL_Sprintf_Add(pStr, (char *)" ");
	MYSQL_ConvertUi32AddToStr(time.hour, pStr);
	MYSQL_Sprintf_Add(pStr, (char *)":");
	MYSQL_ConvertUi32AddToStr(time.minute, pStr);
	MYSQL_Sprintf_Add(pStr, (char *)":");
	MYSQL_ConvertUi32AddToStr(time.second, pStr);
	return p;
}
//#define MYSQL_Time_Add(time)		MYSQL_Time_Add0(time, &MYSQL_Temp[0], 1)

time_t MYSQL_TimeToTime(char *pMysqlTime) {
	struct tm timeinfo;
	char *ptr = &MYSQL_Temp0[0];
	Ui32 val;

	MYSQL_Sprintf(&MYSQL_Temp0[0], pMysqlTime);

	val = MYSQL_ConvertStringToUi32(&ptr);			if ((val > 1900) && (val < 5000)) { timeinfo.tm_year = val - 1900; }
	else { timeinfo.tm_year = 1900; }
	val = MYSQL_ConvertStringToUi32(&ptr);			if ((val > 0) && (val < 13)) { timeinfo.tm_mon = val - 1; }
	else { timeinfo.tm_mon = 0; }
	val = MYSQL_ConvertStringToUi32(&ptr);			if ((val > 0) && (val < 32)) { timeinfo.tm_mday = val; }
	else { timeinfo.tm_mday = 1; }
	val = MYSQL_ConvertStringToUi32(&ptr);			if ((val >= 0) && (val < 24)) { timeinfo.tm_hour = val; }
	else { timeinfo.tm_hour = 0; }
	val = MYSQL_ConvertStringToUi32(&ptr);			if ((val >= 0) && (val < 60)) { timeinfo.tm_min = val; }
	else { timeinfo.tm_min = 0; }
	val = MYSQL_ConvertStringToUi32(&ptr);			if ((val >= 0) && (val < 60)) { timeinfo.tm_sec = val; }
	else { timeinfo.tm_sec = 0; }
	return(mktime(&timeinfo));
}

int MYSQL_Init(void) {
	MYSQL_Conn = mysql_init(NULL);
	return 1;
}

	int MYSQL_Connect(char Srv[], char Usr[], char Pass[], char DB[]) {
		MYSQL_Sprintf(MYSQL_Server, Srv);
		MYSQL_Sprintf(MYSQL_User, Usr);
		MYSQL_Sprintf(MYSQL_Password, Pass);
		MYSQL_Sprintf(MYSQL_Database, DB);

		MYSQL_Conn = mysql_init(0);
		if (MYSQL_Conn)
		{
#ifdef WIN32
			printf("Database Connected\n");
			printf("Press any key to continue...\n");
			//getch();
			//system("cls");
#endif
		}
		else {
#ifdef WIN32
			printf("Failed To Connect! %d \n", mysql_errno(MYSQL_Conn));
#endif
		}
		MYSQL_Conn = mysql_real_connect(MYSQL_Conn, MYSQL_Server, MYSQL_User, MYSQL_Password, MYSQL_Database, 0, NULL, 0);
		if (MYSQL_Conn)
		{
#ifdef WIN32
			printf("Database Connected To MySql %d \n", MYSQL_Conn);
			printf("Press any key to continue... \n");
			//getch();
			//system("cls");
#endif
		}
		else {
#ifdef WIN32
			printf("Failed To Connect! %d \n", mysql_errno(MYSQL_Conn));
#endif
		}
		return 1;
	}
	int MYSQL_AddNewGateway(char Admin[], char Pass[], Ui32 Unique[], Ui64 Mac, Ui32 ConnId, Ui32 CertificateID, Ui08 *pSSK, Ui08 WhatToDo) {
		time_t	Time = time(0);
		struct tm * timeinfo = localtime(&Time);
		string list, object, column;
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
		list = MYSQL_UserList;
		object = AdminBuff;
		column = "UserName";
		qstate = MYSQL_Search(list, column, object);

		if (!qstate)
		{
			res = mysql_store_result(MYSQL_Conn);
			if ((row = mysql_fetch_row(res)))///< existing user
			{
#ifdef WIN32
				printf("ID: %s \nRoles: %s \nName: %s \nPassword: %s \nQuestion: %s \nAnswer: %s \nAvatar: %s \nPicture: %s\n", row[USR_id], row[USR_Rls], row[USR_Nam], row[USR_Pwd], row[USR_RcQ], row[USR_Ans], row[USR_Atr], row[USR_Pic]);
				//cout << "ID: " << row[USR_id] <<"\nRoles: " << row[USR_Rls] << "\nName: " << row[USR_Nam] << "\nPassword: " << row[USR_Pwd] <<"\nQuestion: " << row[USR_RcQ] << "\nAnswer: " << row[USR_Ans] << "\nAvatar: " << row[USR_Atr] << "\nPicture: " << row[USR_Pic] << endl << endl;
#endif				
				mysql_free_result(res);

				if (MYSQL_String_Compare(row[USR_Pwd], PassBuff) == TRUE) {///< PASSWORD CORRECT
					/*
						Search UniqueID from GatewayList if any result test Credits and process request; else add as new gateway
					*/
					GATEWAY_UserIDX = (Ui32)MYSQL_ConvertStringToUi64(row[USR_id]);
					result = 0;
					MYSQL_Temp[0] = 0;	MYSQL_ConvertUi32AddToStr(Unique[0], &MYSQL_Temp[0]);
					list = MYSQL_Table;
					object = MYSQL_Temp;
					column = GW_L_Cs[GWL_Uq0];// "UNIQUE_0";
					qstate = MYSQL_Search(list, column, object);
					if (!qstate)
					{
						res = mysql_store_result(MYSQL_Conn);
						while ((row = mysql_fetch_row(res)))///< UNIQUE_0 Match
						{
							MYSQL_Temp[0] = 0;	MYSQL_ConvertUi32AddToStr(Unique[1], &MYSQL_Temp[0]);
							if (MYSQL_String_Compare(row[GWL_Uq1], MYSQL_Temp) == TRUE)///< UNIQUE_1 Match
							{
								MYSQL_Temp[0] = 0;	MYSQL_ConvertUi32AddToStr(Unique[2], &MYSQL_Temp[0]);
								if (MYSQL_String_Compare(row[GWL_Uq2], MYSQL_Temp) == TRUE)///< UNIQUE_2 Match
								{
									GATEWAY_GatewayIDX = (Ui32)MYSQL_ConvertStringToUi64(row[GWL_Id]);
									GATEWAY_DeviceList = (Ui32)MYSQL_ConvertStringToUi64(row[GWL_Div]);
#ifdef WIN32
									printf("B MySQL Gateway = %s\n  DeviceList = %s\n errno=%d\n", row[GWL_Id], row[GWL_Div], mysql_errno(MYSQL_Conn));
									//cout << "MySQL Gateway = " << row[GWL_Id] << " DeviceList = " << row[GWL_Div] << mysql_errno(MYSQL_Conn) << endl;
#endif
									result = 1;
									break;
								}
							}
						}
						if (result == 0) {
							memcpy(&MYSQL_SessionKeyTemp[0], pSSK, 24);
							MYSQL_Start_AddRow((char *)MYSQL_Table, MYSQL_AUTO_ID_);
							MYSQL_String_Add((char *)"Undefined Gateway");		MYSQL_Break_Add();	///< name
							MYSQL_Ui32_Array_Add(&Unique[0], 3);					MYSQL_Break_Add();	///< unique id
							MYSQL_Ui64_Add(Mac);									MYSQL_Break_Add();	///< mac
							MYSQL_Ui32_Add(GATEWAY_UserIDX);						MYSQL_Break_Add();	///<user id
							MYSQL_Ui32_Add(CertificateID);						MYSQL_Break_Add();	///<certificate id
							MYSQL_Ui32_Add(ConnId);								MYSQL_Break_Add();	///< conneciton id
							MYSQL_Ui64_Array_Add(&MYSQL_SessionKeyTemp[0], 3);		MYSQL_Break_Add();	///< session key
							MYSQL_Ui32_Add(0);									MYSQL_Break_Add();	///< packet
							MYSQL_Ui32_Add(0);									MYSQL_Break_Add();	///< total
							MYSQL_Ui32_Add(0);									MYSQL_Break_Add();	///< send 
							MYSQL_Ui32_Add(0);									MYSQL_Break_Add();	///< received
							MYSQL_Time_Add(MYSQL_TimeToMysqTime(localtime(&Time)));	MYSQL_Break_Add();	///< first conn
							MYSQL_Time_Add(MYSQL_TimeToMysqTime(localtime(&Time)));	MYSQL_Break_Add();	///< last conn
							MYSQL_Ui32_Add(0);													///< Undefined DEvice List
							MYSQL_Stop_Add();

							mysql_query(MYSQL_Conn, (const char *)&MYSQL_Temp[0]);
#ifdef WIN32
							printf("MySQL Initialization or Connection new  %d\n", mysql_errno(MYSQL_Conn));
							//cout << "MySQL Initialization or Connection new " << mysql_errno(MYSQL_Conn) << endl;
#endif
							mysql_free_result(res);
						}
						else if (WhatToDo == _GATEWAY_KEEP_) {
#ifdef WIN32
							printf("Gateway Found - KEEP IT  ");
							//cout << "Gateway Found - KEEP IT  " << endl;
#endif
							mysql_free_result(res);
						}
						else {
							MYSQL_UpdateCellStart((char *)MYSQL_Table);
							MYSQL_UpdateCellValue((char *)GW_L_Cs[GWL_Mac], MYSQL_ConvertUi64ToStrRet(Mac, &MYSQL_Temp0[0]));
							MYSQL_UpdateCellValue((char *)GW_L_Cs[GWL_Adm], MYSQL_ConvertUi32ToStrRet(GATEWAY_UserIDX, &MYSQL_Temp0[0]));
							MYSQL_UpdateCellValue((char *)GW_L_Cs[GWL_cer], MYSQL_ConvertUi32ToStrRet(CertificateID, &MYSQL_Temp0[0]));
							MYSQL_UpdateCellValue((char *)GW_L_Cs[GWL_con], MYSQL_ConvertUi32ToStrRet(ConnId, &MYSQL_Temp0[0]));
							memcpy(&MYSQL_SessionKeyTemp[0], pSSK, 24);
							MYSQL_UpdateCellValue((char *)GW_L_Cs[GWL_SK0], MYSQL_ConvertUi64ToStrRet(MYSQL_SessionKeyTemp[0], &MYSQL_Temp0[0]));
							MYSQL_UpdateCellValue((char *)GW_L_Cs[GWL_SK1], MYSQL_ConvertUi64ToStrRet(MYSQL_SessionKeyTemp[1], &MYSQL_Temp0[0]));
							MYSQL_UpdateCellValue((char *)GW_L_Cs[GWL_SK2], MYSQL_ConvertUi64ToStrRet(MYSQL_SessionKeyTemp[2], &MYSQL_Temp0[0]));
							if ((WhatToDo == _GATEWAY_RESET_) || (WhatToDo == _GATEWAY_REINIT_)) {
								MYSQL_UpdateCellValue((char *)GW_L_Cs[GWL_pkt], MYSQL_ConvertUi32ToStrRet(0, &MYSQL_Temp0[0]));
								MYSQL_UpdateCellValue((char *)GW_L_Cs[GWL_Tot], MYSQL_ConvertUi32ToStrRet(0, &MYSQL_Temp0[0]));
								MYSQL_UpdateCellValue((char *)GW_L_Cs[GWL_SdB], MYSQL_ConvertUi32ToStrRet(0, &MYSQL_Temp0[0]));
								MYSQL_UpdateCellValue((char *)GW_L_Cs[GWL_RvB], MYSQL_ConvertUi32ToStrRet(0, &MYSQL_Temp0[0]));
								MYSQL_UpdateCellValue((char *)GW_L_Cs[GWL_Fst], MYSQL_Time_Add0(MYSQL_TimeToMysqTime(localtime(&Time)), &MYSQL_Temp0[0], 0));
								MYSQL_UpdateCellValue((char *)GW_L_Cs[GWL_Lst], MYSQL_Time_Add0(MYSQL_TimeToMysqTime(localtime(&Time)), &MYSQL_Temp0[0], 0));
								MYSQL_UpdateCellValue((char *)GW_L_Cs[GWL_Div], MYSQL_ConvertUi32ToStrRet(0, &MYSQL_Temp0[0]));
							}
							else {
								MYSQL_UpdateCellValue((char *)GW_L_Cs[GWL_Lst], MYSQL_Time_Add0(MYSQL_TimeToMysqTime(localtime(&Time)), &MYSQL_Temp0[0], 0));
							}
							MYSQL_UpdateCellEnd((char *)GW_L_Cs[GWL_Id], GATEWAY_GatewayIDX);
							mysql_query(MYSQL_Conn, (const char *)&MYSQL_Temp[0]);
#ifdef WIN32
							printf("Gateway Found - CHANGE == %s %d", MYSQL_Temp, mysql_errno(MYSQL_Conn));
							//cout << "Gateway Found - CHANGE == " << MYSQL_Temp << mysql_errno(MYSQL_Conn) << endl;
#endif
							mysql_free_result(res);
						}
						MYSQL_QueryAndLoadGateway(NULL, Unique, NULL, NULL, MYSQL_QUERY_BY_UNIQUE_);///< LOAD TO Gateway Instant Structure
						MYSQL_TestCreateNewDeviceList(MYSQL_Gateway._id);
						return TRUE;
					}
					else
					{
#ifdef WIN32
						printf("\n%s Query Execution Problem A! %d\n", column, mysql_errno(MYSQL_Conn));
						//cout << column << "Query Execution Problem!" << mysql_errno(MYSQL_Conn) << endl;
#endif
						return ERROR_APPEXEC_HOST_ID_MISMATCH;
					}
				}
				else {///< WRONG PASSWORD !!!
#ifdef WIN32
					printf("WRONG PASSWORD !!!  %s  --  %s", row[3], PassBuff);
					//cout << "WRONG PASSWORD !!!  " << row[3] << " -- " << PassBuff << endl;
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
		printf("Query Execution Problem B! %d\n", mysql_errno(MYSQL_Conn));
			//cout << "Query Execution Problem!" << mysql_errno(MYSQL_Conn) << endl;
#endif
			return ERROR_APPEXEC_HOST_ID_MISMATCH;
		}
		return TRUE;
	}
	int MYSQL_CloseConnect(void) {
		mysql_close(MYSQL_Conn);
		return 1;
	}
	int MYSQL_QueryAndLoadGateway(Ui32 id, Ui32 Unique[], Ui64 Mac, Ui32 ConnId, char QueryType) {
		string list, object, column;
		int result;

		result = 0;
		switch (QueryType) {
		case MYSQL_QUERY_BY_ID_:
			MYSQL_ConvertUi32ToStr(id, &MYSQL_Temp[0]);
			list = MYSQL_Table;
			object = MYSQL_Temp;
			column = (char *)GW_L_Cs[GWL_Id];
			qstate = MYSQL_Search(list, column, object);
			if (!qstate)
			{
				res = mysql_store_result(MYSQL_Conn);
				if ((row = mysql_fetch_row(res)))///< ID Match
				{
					result = 1;
				}
				mysql_free_result(res);
			}
			else
			{
#ifdef WIN32
				printf("%s Query Execution Problem C! %d\n", column, mysql_errno(MYSQL_Conn));
				//cout << column << "Query Execution Problem!" << mysql_errno(MYSQL_Conn) << endl;
#endif
				return ERROR_APPEXEC_HOST_ID_MISMATCH;
			}
			break;
		case MYSQL_QUERY_BY_MAC_:
			MYSQL_ConvertUi64ToStr(Mac, &MYSQL_Temp[0]);
			list = MYSQL_Table;
			object = MYSQL_Temp;
			column = (char *)GW_L_Cs[GWL_Mac];
			qstate = MYSQL_Search(list, column, object);
			if (!qstate)
			{
				res = mysql_store_result(MYSQL_Conn);
				if ((row = mysql_fetch_row(res)))///< ID Match
				{
					result = 1;
				}
				mysql_free_result(res);
			}
			else
			{
#ifdef WIN32
				printf("%s Query Execution Problem D! %d\n", column, mysql_errno(MYSQL_Conn));
				//cout << column << "Query Execution Problem!" << mysql_errno(MYSQL_Conn) << endl;
#endif
				return ERROR_APPEXEC_HOST_ID_MISMATCH;
			}
			break;
		case MYSQL_QUERY_BY_UNIQUE_:
			MYSQL_ConvertUi32ToStr(Unique[0], &MYSQL_Temp[0]);
			list = MYSQL_Table;
			object = MYSQL_Temp;
			column = (char *)GW_L_Cs[GWL_Uq0];
			qstate = MYSQL_Search(list, column, object);
			if (!qstate)
			{
				res = mysql_store_result(MYSQL_Conn);
				while ((row = mysql_fetch_row(res)))///< UNIQUE_0 Match
				{
					MYSQL_ConvertUi32ToStr(Unique[1], &MYSQL_Temp[0]);
					if (MYSQL_String_Compare(row[GWL_Uq1], MYSQL_Temp) == TRUE)///< UNIQUE_1 Match
					{
						MYSQL_ConvertUi32ToStr(Unique[2], &MYSQL_Temp[0]);
						if (MYSQL_String_Compare(row[GWL_Uq2], MYSQL_Temp) == TRUE)///< UNIQUE_2 Match
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
				printf("%s Query Execution Problem E! %d\n", column, mysql_errno(MYSQL_Conn));
				//cout << column << "Query Execution Problem!" << mysql_errno(MYSQL_Conn) << endl;
#endif
				return ERROR_APPEXEC_HOST_ID_MISMATCH;
			}
			break;
		case MYSQL_QUERY_BY_CONN_:
			MYSQL_ConvertUi32ToStr(ConnId, &MYSQL_Temp[0]);
			list = MYSQL_Table;
			object = MYSQL_Temp;
			column = (char *)GW_L_Cs[GWL_con];
			qstate = MYSQL_Search(list, column, object);
			if (!qstate)
			{
				res = mysql_store_result(MYSQL_Conn);
				if ((row = mysql_fetch_row(res)))///< ID Match
				{
					result = 1;
				}
				mysql_free_result(res);
			}
			else
			{
#ifdef WIN32
				printf("%s Query Execution Problem F! %d\n", column, mysql_errno(MYSQL_Conn));
				//cout << column << "Query Execution Problem!" << mysql_errno(MYSQL_Conn) << endl;
#endif
				return ERROR_APPEXEC_HOST_ID_MISMATCH;
			}
			break;
		};
		if (result == 1) {
			MYSQL_Gateway._id = (Ui32)MYSQL_ConvertStringToUi64(row[GWL_Id]);
			MYSQL_Sprintf(&MYSQL_Gateway._Name[0], row[GWL_Nm]);
			MYSQL_Gateway._Unique[0] = (Ui32)MYSQL_ConvertStringToUi64(row[GWL_Uq0]);
			MYSQL_Gateway._Unique[1] = (Ui32)MYSQL_ConvertStringToUi64(row[GWL_Uq1]);
			MYSQL_Gateway._Unique[2] = (Ui32)MYSQL_ConvertStringToUi64(row[GWL_Uq2]);
			MYSQL_Gateway._MAC = (Ui64)MYSQL_ConvertStringToUi64(row[GWL_Mac]);
			MYSQL_Gateway._Admin = (Ui32)MYSQL_ConvertStringToUi64(row[GWL_Adm]);
			MYSQL_Gateway._CertificateId = (Ui32)MYSQL_ConvertStringToUi64(row[GWL_cer]);
			MYSQL_Gateway._ConnectionId = (Ui32)MYSQL_ConvertStringToUi64(row[GWL_con]);
			MYSQL_Gateway._SessionKey[0] = (Ui64)MYSQL_ConvertStringToUi64(row[GWL_SK0]);
			MYSQL_Gateway._SessionKey[1] = (Ui64)MYSQL_ConvertStringToUi64(row[GWL_SK1]);
			MYSQL_Gateway._SessionKey[2] = (Ui64)MYSQL_ConvertStringToUi64(row[GWL_SK2]);
			MYSQL_Gateway._PacketCount = (Ui32)MYSQL_ConvertStringToUi64(row[GWL_pkt]);
			MYSQL_Gateway._TotalPackets = (Ui32)MYSQL_ConvertStringToUi64(row[GWL_Tot]);
			MYSQL_Gateway._SendBytes = (Ui32)MYSQL_ConvertStringToUi64(row[GWL_SdB]);
			MYSQL_Gateway._ReceivedBytes = (Ui32)MYSQL_ConvertStringToUi64(row[GWL_RvB]);
			MYSQL_Gateway._DeviceListId = (Ui32)MYSQL_ConvertStringToUi64(row[GWL_Div]);
			MYSQL_Gateway._FirstConnectTime = (time_t)MYSQL_TimeToTime(row[GWL_Fst]);
			MYSQL_Gateway._LastConnectTime = (time_t)MYSQL_TimeToTime(row[GWL_Lst]);
#ifdef WIN32
			printf("A MySQL Gateway =  %s\n  DeviceList = %s\n errno= %d\n", row[GWL_Id], row[GWL_Div], mysql_errno(MYSQL_Conn));
			//cout << "MySQL Gateway = " << row[GWL_Id] << " DeviceList = " << row[GWL_Div] << mysql_errno(MYSQL_Conn) << endl;
#endif
		}
		else {
#ifdef WIN32
			printf("There is no match found !!! ");
			//cout << "There is no match found !!! " << endl;
#endif
			return ERROR_NO_MATCH;
		}
		return TRUE;
	}
	int MYSQL_ReloadGatewayListRow(Ui32 Flgs) {
		MYSQL_UpdateCellStart((char *)MYSQL_Table);
		//MYSQL_UpdateCellValue((char *)GW_L_Cs[GWL_Id], MYSQL_ConvertUi32ToStrRet(MYSQL_Gateway._id, &MYSQL_Temp0[0]), 1);
		if (Flgs&(1 << GWL_Nm)) { MYSQL_UpdateCellValue((char *)GW_L_Cs[GWL_Nm], &MYSQL_Gateway._Name[0]); }
		if (Flgs&(1 << GWL_Uq0)) { MYSQL_UpdateCellValue((char *)GW_L_Cs[GWL_Uq0], MYSQL_ConvertUi32ToStrRet(MYSQL_Gateway._Unique[0], &MYSQL_Temp0[0])); }
		if (Flgs&(1 << GWL_Uq1)) { MYSQL_UpdateCellValue((char *)GW_L_Cs[GWL_Uq1], MYSQL_ConvertUi32ToStrRet(MYSQL_Gateway._Unique[1], &MYSQL_Temp0[0])); }
		if (Flgs&(1 << GWL_Uq2)) { MYSQL_UpdateCellValue((char *)GW_L_Cs[GWL_Uq2], MYSQL_ConvertUi32ToStrRet(MYSQL_Gateway._Unique[2], &MYSQL_Temp0[0])); }
		if (Flgs&(1 << GWL_Mac)) { MYSQL_UpdateCellValue((char *)GW_L_Cs[GWL_Mac], MYSQL_ConvertUi64ToStrRet(MYSQL_Gateway._MAC, &MYSQL_Temp0[0])); }
		if (Flgs&(1 << GWL_Adm)) { MYSQL_UpdateCellValue((char *)GW_L_Cs[GWL_Adm], MYSQL_ConvertUi32ToStrRet(MYSQL_Gateway._Admin, &MYSQL_Temp0[0])); }
		if (Flgs&(1 << GWL_cer)) { MYSQL_UpdateCellValue((char *)GW_L_Cs[GWL_cer], MYSQL_ConvertUi32ToStrRet(MYSQL_Gateway._CertificateId, &MYSQL_Temp0[0])); }
		if (Flgs&(1 << GWL_con)) { MYSQL_UpdateCellValue((char *)GW_L_Cs[GWL_con], MYSQL_ConvertUi32ToStrRet(MYSQL_Gateway._ConnectionId, &MYSQL_Temp0[0])); }
		if (Flgs&(1 << GWL_SK0)) { MYSQL_UpdateCellValue((char *)GW_L_Cs[GWL_SK0], MYSQL_ConvertUi64ToStrRet(MYSQL_Gateway._SessionKey[0], &MYSQL_Temp0[0])); }
		if (Flgs&(1 << GWL_SK1)) { MYSQL_UpdateCellValue((char *)GW_L_Cs[GWL_SK1], MYSQL_ConvertUi64ToStrRet(MYSQL_Gateway._SessionKey[1], &MYSQL_Temp0[0])); }
		if (Flgs&(1 << GWL_SK2)) { MYSQL_UpdateCellValue((char *)GW_L_Cs[GWL_SK2], MYSQL_ConvertUi64ToStrRet(MYSQL_Gateway._SessionKey[2], &MYSQL_Temp0[0])); }
		if (Flgs&(1 << GWL_pkt)) { MYSQL_UpdateCellValue((char *)GW_L_Cs[GWL_pkt], MYSQL_ConvertUi32ToStrRet(MYSQL_Gateway._PacketCount, &MYSQL_Temp0[0])); }
		if (Flgs&(1 << GWL_Tot)) { MYSQL_UpdateCellValue((char *)GW_L_Cs[GWL_Tot], MYSQL_ConvertUi32ToStrRet(MYSQL_Gateway._TotalPackets, &MYSQL_Temp0[0])); }
		if (Flgs&(1 << GWL_SdB)) { MYSQL_UpdateCellValue((char *)GW_L_Cs[GWL_SdB], MYSQL_ConvertUi32ToStrRet(MYSQL_Gateway._SendBytes, &MYSQL_Temp0[0])); }
		if (Flgs&(1 << GWL_RvB)) { MYSQL_UpdateCellValue((char *)GW_L_Cs[GWL_RvB], MYSQL_ConvertUi32ToStrRet(MYSQL_Gateway._ReceivedBytes, &MYSQL_Temp0[0])); }
		if (Flgs&(1 << GWL_Fst)) { MYSQL_UpdateCellValue((char *)GW_L_Cs[GWL_Fst], MYSQL_Time_Add0(MYSQL_TimeToMysqTime(localtime(&MYSQL_Gateway._FirstConnectTime)), &MYSQL_Temp0[0], 0)); }
		if (Flgs&(1 << GWL_Lst)) { MYSQL_UpdateCellValue((char *)GW_L_Cs[GWL_Lst], MYSQL_Time_Add0(MYSQL_TimeToMysqTime(localtime(&MYSQL_Gateway._LastConnectTime)), &MYSQL_Temp0[0], 0)); }
		if (Flgs&(1 << GWL_Div)) { MYSQL_UpdateCellValue((char *)GW_L_Cs[GWL_Div], MYSQL_ConvertUi32ToStrRet(MYSQL_Gateway._DeviceListId, &MYSQL_Temp0[0])); }

		MYSQL_UpdateCellEnd((char *)GW_L_Cs[GWL_Id], MYSQL_Gateway._id);
		mysql_query(MYSQL_Conn, (const char *)&MYSQL_Temp[0]);
		return TRUE;
	}

	int MYSQL_DeleteDeviceList(Ui32 Id) {
		const char DeviceListString2[] = "DROP TABLE DeviceList_";
		MYSQL_Sprintf(&MYSQL_Temp[0], (char *)&DeviceListString2[0]);
		MYSQL_ConvertUi32AddToStr(Id, &MYSQL_Temp[0]);
		mysql_query(MYSQL_Conn, (const char *)&MYSQL_Temp[0]);
#ifdef WIN32
		printf("MySQL Delete Device List Error No : %d", mysql_errno(MYSQL_Conn));
		//cout << "MySQL Delete Device List Error No : " << mysql_errno(MYSQL_Conn) << endl;
#endif
		return TRUE;
	}

	int MYSQL_CreateNewDeviceList(Ui32 Id, char Delete) {
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
			MYSQL_DeleteDeviceList(Id);
			//Sleep(100);
		}
		MYSQL_Sprintf(&MYSQL_Temp[0], (char *)&DeviceListString0[0]);
		MYSQL_ConvertUi32AddToStr(Id, &MYSQL_Temp[0]);
		MYSQL_Sprintf_Add(&MYSQL_Temp[0], (char *)&DeviceListString1[0]);
		mysql_query(MYSQL_Conn, (const char *)&MYSQL_Temp[0]);
#ifdef WIN32
		printf("MySQL Create New Device List Error No : %d\n", mysql_errno(MYSQL_Conn));
		//cout << "MySQL Create New Device List Error No : " << mysql_errno(MYSQL_Conn) << endl;
#endif
		//Sleep(100);
		return TRUE;
	}

	int MYSQL_TestCreateNewDeviceList(Ui32 Id) {
		const char DeviceListString5[] = "SELECT 1 FROM DeviceList_";
		const char DeviceListString6[] = " LIMIT 1";
		int result;
		MYSQL_Sprintf(&MYSQL_Temp[0], (char *)&DeviceListString5[0]);
		MYSQL_ConvertUi32AddToStr(Id, &MYSQL_Temp[0]);
		MYSQL_Sprintf_Add(&MYSQL_Temp[0], (char *)&DeviceListString6[0]);
		mysql_query(MYSQL_Conn, (const char *)&MYSQL_Temp[0]);
		result = (Ui32)mysql_errno(MYSQL_Conn);// MYSQL_ConvertStringToUi64(mysql_errno(MYSQL_Conn));
		if (result != 0) {
			MYSQL_CreateNewDeviceList(Id, _LIST_KEEP);
		}
		else {
			res = mysql_store_result(MYSQL_Conn);
			mysql_free_result(res);
		}
		return result;
	}

	int MYSQL_LoadDeviceInfo(Ui32 gId, Ui32 Id) {
		int result = 0;
		string list, object, column;

		MYSQL_Sprintf(&MYSQL_Temp[0], (char *)&DevListStr[0]);
		if (gId == 0) { gId = MYSQL_Gateway._id; }///< loaded before
		MYSQL_ConvertUi32AddToStr(gId, &MYSQL_Temp[0]);
		list = MYSQL_Temp;
		MYSQL_ConvertUi32ToStr(Id, &MYSQL_Temp0[0]);
		object = MYSQL_Temp0;
		column = (char *)DV_L_Cs[DVL_id];
		qstate = MYSQL_Search(list, column, object);
		if (!qstate)
		{
			res = mysql_store_result(MYSQL_Conn);
			if ((row = mysql_fetch_row(res)))///< ID Match
			{
				MYSQL_Device._id = (Ui32)MYSQL_ConvertStringToUi64(row[DVL_id]);
				MYSQL_Sprintf(&MYSQL_Device._Name[0], row[DVL_Nm]);
				MYSQL_Device._Unique[0] = (Ui32)MYSQL_ConvertStringToUi64(row[DVL_Uq0]);
				MYSQL_Device._Unique[1] = (Ui32)MYSQL_ConvertStringToUi64(row[DVL_Uq1]);
				MYSQL_Device._Unique[2] = (Ui32)MYSQL_ConvertStringToUi64(row[DVL_Uq2]);
				MYSQL_Device._ParentId = (Ui32)MYSQL_ConvertStringToUi64(row[DVL_pId]);
				MYSQL_Device._Type = (Ui32)MYSQL_ConvertStringToUi64(row[DVL_Typ]);
				MYSQL_Device._LocalIp = (Ui32)MYSQL_ConvertStringToUi64(row[DVL_LIP]);
				MYSQL_Device._Status = (Ui32)MYSQL_ConvertStringToUi64(row[DVL_Stt]);
				MYSQL_Device._PacketCount = (Ui32)MYSQL_ConvertStringToUi64(row[DVL_pkt]);
				MYSQL_Device._TotalPackets = (Ui32)MYSQL_ConvertStringToUi64(row[DVL_Tot]);
				MYSQL_Device._SendBytes = (Ui32)MYSQL_ConvertStringToUi64(row[DVL_SdB]);
				MYSQL_Device._ReceivedBytes = (Ui32)MYSQL_ConvertStringToUi64(row[DVL_RvB]);
				result = 1;
			}
			else {
#ifdef WIN32
				printf("MySQL Device Not Found !!! Error No : %d", mysql_errno(MYSQL_Conn));
				//cout << list << "MySQL Device Not Found !!! Error No : " << mysql_errno(MYSQL_Conn) << endl;
#endif

			}
			mysql_free_result(res);
			return result;
		}
		else {
#ifdef WIN32
			printf("MySQL Device List Not Found !!! Error No : %d", mysql_errno(MYSQL_Conn));
			//cout << list << "MySQL Device List Not Found !!! Error No : " << mysql_errno(MYSQL_Conn) << endl;
#endif
			return 0;
		}

	}

	int MYSQL_ReadDeviceList(Ui32 Id, MYSQL_DeviceRead *pReadFile, Ui32 MaxCnt, Ui32 StrtIdx) {
		Ui32 i = 0;
		string list;

		MYSQL_Sprintf(&MYSQL_Temp[0], (char *)&DevListStr[0]);
		if (Id == 0) { Id = MYSQL_Gateway._id; }///< loaded before
		if (MaxCnt == 0) { MaxCnt = 0xFFFFFFFF; }///< read all
		MYSQL_ConvertUi32AddToStr(Id, &MYSQL_Temp[0]);

		list = MYSQL_Temp;
		qstate = MYSQL_ReadAllRows(list);
		if (!qstate)
		{
			res = mysql_store_result(MYSQL_Conn);
			while ((row = mysql_fetch_row(res)))///< ID Match
			{
				if (i >= StrtIdx) {
					pReadFile->_id = (Ui32)MYSQL_ConvertStringToUi64(row[DVL_id]);
					pReadFile->_Unique[0] = (Ui32)MYSQL_ConvertStringToUi64(row[DVL_Uq0]);
					pReadFile->_Unique[1] = (Ui32)MYSQL_ConvertStringToUi64(row[DVL_Uq1]);
					pReadFile->_Unique[2] = (Ui32)MYSQL_ConvertStringToUi64(row[DVL_Uq2]);
					pReadFile->_LocalIp = (Ui32)MYSQL_ConvertStringToUi64(row[DVL_LIP]);
					pReadFile->_ParentId = (Ui32)MYSQL_ConvertStringToUi64(row[DVL_pId]);
					pReadFile->_Type = (Ui32)MYSQL_ConvertStringToUi64(row[DVL_Typ]);
					pReadFile++;
#ifdef WIN32
					printf("MySQL Found Device id = %s _Unique0 = %s _Unique1 = %s _Unique2 = %s NAME = %s", row[DVL_id], row[DVL_Uq0], row[DVL_Uq1], row[DVL_Uq2], row[DVL_Nm]);
					//cout << "MySQL Found Device id = " << row[DVL_id] <<" _Unique0 = " << row[DVL_Uq0] <<	" _Unique1 = " << row[DVL_Uq1] << " _Unique2 = " << row[DVL_Uq2] << " NAME = " << row[DVL_Nm] << endl;
#endif
				}
				i++;
				if (i >= (MaxCnt + StrtIdx)) {
					break;
				}
			}
			mysql_free_result(res);
#ifdef WIN32
			printf("MySQL Total Found Device i = %s\n", MYSQL_ConvertUi32ToStrRet((i - StrtIdx), &MYSQL_Temp0[0]));
			//cout << "MySQL Total Found Device i = " << MYSQL_ConvertUi32ToStrRet((i - StrtIdx), &MYSQL_Temp0[0]) << endl;
#endif
			return (i - StrtIdx);
		}
		else
		{
#ifdef WIN32
			printf("MySQL Device List Not Found !!! Error No : %d", mysql_errno(MYSQL_Conn));
			//cout << list << "MySQL Device List Not Found !!! Error No : " << mysql_errno(MYSQL_Conn) << endl;
#endif
			return 0;
		}
	}

	int MYSQL_LoadDeviceInfo(Ui32 id, Ui32 Unique[]) {
		if (MYSQL_QueryAndLoadGateway(NULL, Unique, NULL, NULL, MYSQL_QUERY_BY_UNIQUE_) == TRUE) {
			if (id != 0) {
				MYSQL_LoadDeviceInfo(MYSQL_Gateway._id, id);
			}
			return MYSQL_Gateway._ConnectionId;
		}
		else {
			return 0;
		}

	}
//#define FindConnectionIdOfGateway(Unique)		_LoadDeviceInfo(0, Unique)