//#include "pch.h"
#undef UNICODE

#include <stdio.h>
#include <time.h>
//#include <windows.h>

//#include <mysql.h>
#include <AY_MySql.h>

#include <AY_Printf.h>
#include <AY_Functions.h>
#include <AY_Socket.h>
#include <AY_Server.h>
#include <AY_ServerPrjtBased.h>
#include <AY_Crypt.h>
#include <AY_Memory.h>
#include <AY_Queue.h>
#include <AY_ClientConn.h>

////------------ TEST SIL !!! ___________________
//const Ui08 UniqId[12] = {0x74,0xd4,0x35 ,0x3c ,0x4a ,0xb2 ,0x74 ,0xd4 ,0x35 ,0x3c ,0x4a ,0xb2};
//const Ui08 Mac[8] = { 0x60 ,0xf6 ,0x77 ,0xbe ,0x5f ,0x31 ,0x00 ,0x00};
//const char Name[45]="";
//const char Psw[45]="";
//const Ui32 CertNo=1;
//const Ui32 ConnNo=0;
//const Ui08 Sssk[16] = { 0x6c ,0x3a ,0x1b ,0xcc ,0x2f ,0x52 ,0xf0 ,0x37 ,0x71 ,0x43 ,0x3c ,0x39 ,0xf7 ,0x4c ,0x90 ,0x00 };
//void AYSRV_TestMySql(void) {
//	MYSQL_AddNewGateway((char *)Name, (char *)Psw, (Ui32 *)&UniqId, *((Ui64 *)&Mac[0]), ConnNo, CertNo, (Ui08*)&Sssk[0], _GATEWAY_UPDATE_);
//	printf("AYDVSTRT--> SSK = "); AY_HexValPrint((Ui08 *)&Sssk[0], 16); printf("\r\n");
//	printf("AYDVSTRT--> SSK = "); AY_HexValPrint((Ui08 *)&MYSQL_Gateway._SessionKey[0], 16); printf("\r\n");
//}
////------------ TEST SIL !!! ___________________

void AYSRV_QueueClientConn(AY_QUEUE *pQ) {
	AY_DEVSTRTIN		*pDevStrtIn;
	AY_DeviceStartResp	*pDevStrtPspHdr;
	udp_headerAll		*pUDP;
	Ui32				i,j,k;
	Ui32				m, n;//test sil !!!

	pDevStrtIn = (AY_DEVSTRTIN	*)pQ->pIn;
	i = AYCONN_FindOrAddConn(*((Ui32 *)&pDevStrtIn->_Unique[0]), *((Ui32 *)&pDevStrtIn->_Unique[4]), *((Ui32 *)&pDevStrtIn->_Unique[8]), &pDevStrtIn->_UDPh, AY_CONN_UPDATE);
	printf("AYDVSTRT--> Connection ID ConnId=%d \n", i);
	MYSQL_AddNewGateway((char *)pDevStrtIn->_Name, (char *)pDevStrtIn->_Pswd, (Ui32 *)&pDevStrtIn->_Unique, *((Ui64 *)&pDevStrtIn->_MAC), i, pDevStrtIn->_ServerCertNo, &pDevStrtIn->_SessionKey[0], _GATEWAY_UPDATE_);
	//----------------------------//
	/*memcpy(&UniqId[0], &pDevStrtIn->_Unique[0], 12);
	memcpy(&Mac[0], &pDevStrtIn->_MAC[0], 6);
	strcpy(Name, pDevStrtIn->_Name);
	strcpy(Psw, pDevStrtIn->_Pswd);
	CertNo = pDevStrtIn->_ServerCertNo;
	ConnNo = i;
	memcpy(&Sssk[0], &pDevStrtIn->_SessionKey[0], 16);*/
	//--------------------------------//
	pUDP = (udp_headerAll *)_AY_MallocMemory(4096);///< max packet size
	memcpy(pUDP, &pDevStrtIn->_UDPh, sizeof(udp_headerAll));
	pUDP->_ethHeader.dest = pDevStrtIn->_UDPh._ethHeader.src;
	pUDP->_ethHeader.src = MyEth_Address;//pDevStrtIn->_UDPh._ethHeader.dest;
	pUDP->_ipHeader.daddr = pDevStrtIn->_UDPh._ipHeader.saddr;
	pUDP->_ipHeader.saddr = pDevStrtIn->_UDPh._ipHeader.daddr;
	pUDP->_udpHeader.dport = pDevStrtIn->_UDPh._udpHeader.sport;
	pUDP->_udpHeader.sport = pDevStrtIn->_UDPh._udpHeader.dport;

	pDevStrtPspHdr = (AY_DeviceStartResp	*)(((Ui08 *)pUDP)+sizeof(udp_headerAll));///< max packet size
	j = 0; k = 0;
	printf("AYDVSTRT--> SSK = "); AY_HexValPrint((Ui08 *)&pDevStrtIn->_SessionKey[0], 16); printf("\r\n");
	printf("AYDVSTRT--> SSK = "); AY_HexValPrint((Ui08 *)&MYSQL_Gateway._SessionKey[0], 16); printf("\r\n");
	do {
		i = MYSQL_ReadDeviceList(0, (MYSQL_DeviceRead *)(((Ui08 *)pDevStrtPspHdr)+sizeof(AY_DeviceStartResp)), 168, j);
		pDevStrtPspHdr->_Test0 = PACKET_TEST_DATA0;
		pDevStrtPspHdr->_Test1 = PACKET_TEST_DATA1;
		pDevStrtPspHdr->_DevCnt = i;
		pDevStrtPspHdr->_DevPcktNo= k;
		//============== FILE========================//
		m = sizeof(AY_DeviceStartResp) + (((Ui16)i) * sizeof(AY_DeviceRead));
		printf("AYDVSTRT--> %d ============ FILE START =========\n ",m);
		n = 0;
		while (m > 32) {
			AY_HexValPrint((((Ui08 *)pDevStrtPspHdr) + n), 32);
			printf("\r\n ");
			n += 32;
			m -= 32;
		}
		if (m) {
			AY_HexValPrint((((Ui08 *)pDevStrtPspHdr) + n), m);
			printf("\r\n ");
		}
		printf("AYDVSTRT--> ============ FILE END =========\n ");
		//===========================================//
		if (i) {
			n = AY_Crypt_AES128((Ui08 *)&pDevStrtIn->_SessionKey[0] /*&MYSQL_Gateway._SessionKey[0]*/, (((Ui08 *)pDevStrtPspHdr) + sizeof(AY_DeviceStartResp)), (((Ui16)i) * sizeof(AY_DeviceRead)));
		}
		k++;
		j += i;
		UDP_packet_send(_MAIN_SCKT, pUDP, (Ui08 *)pDevStrtPspHdr, (sizeof(AY_DeviceStartResp) + n/*(((Ui16)i) * sizeof(AY_DeviceRead))*/));

	} while (i >= 168);
	_AY_FreeMemory((unsigned char*)pUDP);	
	///< delete from queue
	pQ->QFlg._QFinishedF = 1; 
	pQ->QFlg._QKeepF = 0; 
	pQ->QFlg._QBusyF = 1;
	MYSQL_LoadDeviceInfo(MYSQL_Gateway._id, (Ui32 *)&pDevStrtIn->_Unique);
	printf("AYDVSTRT--> SSK = "); AY_HexValPrint((Ui08 *)/*&pDevStrtIn->_SessionKey[0]*/ &MYSQL_Gateway._SessionKey[0], 16); printf("\r\n");
}

int AY_TestLoadDeviceStart(Ui08 *pPtr,Ui16 Len) {
	udp_headerAll	*pUDP; 
	AY_DeviceStart	*pDevStrt;
	AY_DEVSTRTIN	*pDevStrtIn;
	Ui08			*pOut;
	Ui16			i;

	pDevStrt = (AY_DeviceStart	*)(pPtr + sizeof(udp_headerAll));
	if ((pDevStrt->_Test0 == PACKET_TEST_DATA0) && (pDevStrt->_Test1 == PACKET_TEST_DATA1)) {
		printf("AYDVSTRT--> Packet type is DeviceStart\n");
		if (Len == sizeof(AY_DeviceStart)) {
			pOut = (Ui08	*)_AY_MallocMemory(256);
			pDevStrt = (AY_DeviceStart	*)_AY_MallocMemory(sizeof(AY_DeviceStart));
			memcpy(pDevStrt, (AY_DeviceStart	*)(pPtr + sizeof(udp_headerAll)), sizeof(AY_DeviceStart));
			///<===== Check Client's Sign ===================
			AY_Crypt_RSAVerify((Ui08 *)&CLIENT_PUB_KEY[0], (Ui08 *)&pDevStrt->_Input[0], 256, (Ui08 *)&pDevStrt->_Sign[0]);
			///<===== Decrpt Data ===================
			AY_Crypt_RSADecrpt((Ui08 *)&SERVER_PR_KEY[0], (Ui08 *)&pDevStrt->_Input[0], 256, (Ui08 *)pOut, &i);
			memcpy(&pDevStrt->_Input[0], pOut, 256);
			_AY_FreeMemory((unsigned char*)pOut);
			//======= Generate AY_DEVSTRTIN
			pUDP = (udp_headerAll *)(pPtr + 0); // udp all header
			pDevStrtIn = (AY_DEVSTRTIN	*)_AY_MallocMemory(sizeof(AY_DEVSTRTIN));
			memcpy(&pDevStrtIn->_UDPh, pUDP, sizeof(udp_headerAll));
			pDevStrtIn->_LocalCertNo = pDevStrt->_LocalCertNo;
			pDevStrtIn->_ServerCertNo = pDevStrt->_ServerCertNo;
			memcpy(&pDevStrtIn->_MAC, &pDevStrt->_MAC, 6);
			memcpy(&pDevStrtIn->_Name, &pDevStrt->_Name, 45);
			memcpy(&pDevStrtIn->_Pswd, &pDevStrt->_Pswd, 45);
			memcpy(&pDevStrtIn->_SessionKey, &pDevStrt->_SessionKey, 16);
			printf("AYDVSTRT--> SSK = "); AY_HexValPrint(&pDevStrtIn->_SessionKey[0], 16); printf("\r\n");
			memcpy(&pDevStrtIn->_Unique, &pDevStrt->_Unique, 12);
			//======= Release AY_DeviceStart
			_AY_FreeMemory((unsigned char*)pDevStrt);
			//======= Load to Queue
			return( AYSRV_QueueLoad(AYSRV_QueueFindFirstFreeRow(), (Ui08 *)pDevStrtIn, sizeof(AY_DEVSTRTIN), QTARGET_CLIENT_CONN, 0));
		}
		printf("AYDVSTRT--> Packet fail\n");
	}
	return 0;///< not me
}

int AY_TestLoadGwInfoRqst(Ui08 *pPtr, Ui16 Len) {
	udp_headerAll	*pUDP;
	AY_GWINFORQST	*pInfoRqst, *pInfoRqst2;
	AY_DEVSTRTIN	*pDevStrtIn;
	Ui08			*pOut;
	Ui16			i;

	pInfoRqst = (AY_GWINFORQST	*)(pPtr + sizeof(udp_headerAll));
	if ((pInfoRqst->_Test2 == PACKET_TEST_DATA2) && (pInfoRqst->_Test3 == PACKET_TEST_DATA3)) {
		printf("AYDVSTRT--> Packet type is Info Request\n");
		if (Len == sizeof(AY_GWINFORQST)) {
			pOut = (Ui08	*)_AY_MallocMemory(256);
			pInfoRqst2 = (AY_GWINFORQST	*)_AY_MallocMemory(sizeof(AY_GWINFORQST));
			memcpy(pInfoRqst2, (AY_GWINFORQST	*)(pPtr + sizeof(udp_headerAll)), sizeof(AY_GWINFORQST));
			//---------------------------//
			AY_Decrypt_AES128((Ui08 *)&AY_Ram.AY_Sessionkey[0], (Ui08 *)GwRqst._InfoCont[0], sizeof(GwRqst._InfoCont));
			
			
			
			///<===== Check Client's Sign ===================
			AY_Crypt_RSAVerify((Ui08 *)&CLIENT_PUB_KEY[0], (Ui08 *)&pDevStrt->_Input[0], 256, (Ui08 *)&pDevStrt->_Sign[0]);
			///<===== Decrpt Data ===================
			AY_Crypt_RSADecrpt((Ui08 *)&SERVER_PR_KEY[0], (Ui08 *)&pDevStrt->_Input[0], 256, (Ui08 *)pOut, &i);
			memcpy(&pDevStrt->_Input[0], pOut, 256);
			_AY_FreeMemory((unsigned char*)pOut);
			//======= Generate AY_DEVSTRTIN
			pUDP = (udp_headerAll *)(pPtr + 0); // udp all header
			pDevStrtIn = (AY_DEVSTRTIN	*)_AY_MallocMemory(sizeof(AY_DEVSTRTIN));
			memcpy(&pDevStrtIn->_UDPh, pUDP, sizeof(udp_headerAll));
			pDevStrtIn->_LocalCertNo = pDevStrt->_LocalCertNo;
			pDevStrtIn->_ServerCertNo = pDevStrt->_ServerCertNo;
			memcpy(&pDevStrtIn->_MAC, &pDevStrt->_MAC, 6);
			memcpy(&pDevStrtIn->_Name, &pDevStrt->_Name, 45);
			memcpy(&pDevStrtIn->_Pswd, &pDevStrt->_Pswd, 45);
			memcpy(&pDevStrtIn->_SessionKey, &pDevStrt->_SessionKey, 16);
			printf("AYDVSTRT--> SSK = "); AY_HexValPrint(&pDevStrtIn->_SessionKey[0], 16); printf("\r\n");
			memcpy(&pDevStrtIn->_Unique, &pDevStrt->_Unique, 12);
			//======= Release AY_DeviceStart
			_AY_FreeMemory((unsigned char*)pDevStrt);
			//======= Load to Queue
			return(AYSRV_QueueLoad(AYSRV_QueueFindFirstFreeRow(), (Ui08 *)pDevStrtIn, sizeof(AY_DEVSTRTIN), QTARGET_CLIENT_CONN, 0));
		}
		printf("AYDVSTRT--> Packet fail\n");
	}
	return 0;///< not me
}


//============================================================================================================
//====================== CONNECTIONS =========================================================================
Ui32		ConnectionCount = 0;
AY_CONNADR	ConnLevel1;
Ui32 AYCONN_ThisMinute(void) {
	return ((Ui32)(((Ui64)time(0)) / 60));
}
int AYCONN_TestMinute(Ui32 RecMin, Ui32 TimeOut) {
	Ui32 Val = AYCONN_ThisMinute() - RecMin;
	if (Val & 0x80000000) { Val = 0 - Val; }
	if (Val > TimeOut) { return 1; }
	else { return 0; }
}
Ui32 AYCONN_FindOrAddConn(Ui32 Unique0, Ui32 Unique1, Ui32 Unique2, udp_headerAll *pUDP, Ui08 Func) {
	void *ptr;
	AY_CONNADR	*pConnList;
	AY_CONNTYPE	*pConnTyp;
	Ui32 FirstFree;
	Ui08 FirstFound = 0;
	Ui32 i;

#define i0		((i>>24)&0xFF)
#define i1		((i>>16)&0xFF)
#define i2		((i>>8)&0xFF)
#define i3		((i>>0)&0xFF)
	ptr = ConnLevel1.pConnAdr[0];
	i = 0; 
	pConnTyp = 0;
	do{
		if ((i & 0x00FFFFFF) == 0) {
			printf("AYCONN--> Conn Test Level 1 i0=%d \n", i0);
			ptr = ConnLevel1.pConnAdr[i0];
			if (ptr == 0) {///< create new level1 list
				if ((Func == AY_CONN_FIND) || (Func == AY_CONN_DELETE)) {
					printf("AYCONN--> Conn Level 1 not found i=%d \n", i);
					ConnectionCount = i;
					return 0xFFFFFFFF;///< fail !!!
				}
				else {
					pConnList = (AY_CONNADR	*)_AY_MallocMemory(sizeof(AY_CONNADR));
					memset(pConnList, 0, sizeof(AY_CONNADR));
					ConnLevel1.pConnAdr[i0] = (void *)pConnList;
					printf("AYCONN--> New Conn Level 1 created i0=%d \n", i0);
				}
			}
		}
		if ((i & 0x0000FFFF) == 0) {
			printf("AYCONN--> Conn Test Level 2 i1=%d \n", i1);
			ptr = ((AY_CONNADR	*)ConnLevel1.pConnAdr[i0])->pConnAdr[i1];
			if (ptr == 0) {///< create new level2 list
				if ((Func == AY_CONN_FIND) || (Func == AY_CONN_DELETE)) {
					printf("AYCONN--> Conn Level 2 not found i=%d \n", i);
					ConnectionCount = i;
					return 0xFFFFFFFF;///< fail !!!
				}
				else {
					pConnList = (AY_CONNADR	*)_AY_MallocMemory(sizeof(AY_CONNADR));
					memset(pConnList, 0, sizeof(AY_CONNADR));
					((AY_CONNADR	*)ConnLevel1.pConnAdr[i0])->pConnAdr[i1] = (void *)pConnList;
					printf("AYCONN--> New Conn Level 2 created i1=%d \n", i1);
				}
			}
		}
		if ((i & 0x000000FF) == 0) {
			printf("AYCONN--> Conn Test Level 3 i2=%d \n", i2);
			ptr = ((AY_CONNADR	*)(((AY_CONNADR	*)ConnLevel1.pConnAdr[i0])->pConnAdr[i1]))->pConnAdr[i2];
			if (ptr == 0) {///< create new level3 list
				if ((Func == AY_CONN_FIND) || (Func == AY_CONN_DELETE)) {
					printf("AYCONN--> Conn Level 3 not found i=%d \n", i);
					ConnectionCount = i;
					return 0xFFFFFFFF;///< fail !!!
				}
				else {
					pConnList = (AY_CONNADR	*)_AY_MallocMemory(sizeof(AY_CONNADR));
					memset(pConnList, 0, sizeof(AY_CONNADR));
					((AY_CONNADR	*)(((AY_CONNADR	*)ConnLevel1.pConnAdr[i0])->pConnAdr[i1]))->pConnAdr[i2] = (void *)pConnList;
					ptr = ((AY_CONNADR	*)(((AY_CONNADR	*)ConnLevel1.pConnAdr[i0])->pConnAdr[i1]))->pConnAdr[i2];
					printf("AYCONN--> New Conn Level 3 created i2=%d \n", i2);
				}
			}
			pConnTyp = (AY_CONNTYPE *)(((AY_CONNADR	*)ptr)->pConnAdr[0]);///< level4 start index
		}
		if (pConnTyp == 0) {///< free socket found
			if (FirstFound != 0xAA) {
				FirstFound = 0xAA;
				FirstFree = i;
				printf("AYCONN--> First free Conn found FirstFree=%d \n", FirstFree);
			}
		}
		else if ((pConnTyp->_UnqiueId[0] == Unique0) && (pConnTyp->_UnqiueId[1] == Unique1) && (pConnTyp->_UnqiueId[2] == Unique2)) {///< device found
			printf("AYCONN--> Conn found FirstFree=%d \n", i);
			if (Func == AY_CONN_UPDATE) {
				memcpy(&pConnTyp->_UDPh, pUDP, sizeof(udp_headerAll));
			}
			else if (Func == AY_CONN_DELETE) {
				_AY_FreeMemory((unsigned char *)pConnTyp);
				((AY_CONNADR	*)((AY_CONNADR	*)(((AY_CONNADR	*)ConnLevel1.pConnAdr[i0])->pConnAdr[i1]))->pConnAdr[i2])->pConnAdr[i3] = 0;///< nullptr
			}
			return i;
		}
		if (i == ConnectionCount) {
			printf("AYCONN--> Conn not found use FirstFree=%d \n", FirstFree);
			i = FirstFree;
			ptr = ((AY_CONNADR	*)(((AY_CONNADR	*)ConnLevel1.pConnAdr[i0])->pConnAdr[i1]))->pConnAdr[i2];			
			if (Func == AY_CONN_UPDATE) {///< upload
				((AY_CONNADR	*)ptr)->pConnAdr[i3] = _AY_MallocMemory(sizeof(AY_CONNTYPE));
				pConnTyp = (AY_CONNTYPE *)(((AY_CONNADR	*)ptr)->pConnAdr[i3]);///< level4 start index
				memcpy(&pConnTyp->_UDPh, pUDP, sizeof(udp_headerAll));
				pConnTyp->_LastUpdateMin = AYCONN_ThisMinute();
				ConnectionCount++;
				printf("AYCONN--> Conn new added ConnectionCount=%d \n", ConnectionCount);
			}
			/*else if (Func == AY_CONN_READ) {///< download
				memcpy(pUDP, &pConnTyp->_UDPh, sizeof(udp_headerAll));
			}*/
			return i;
		}
		pConnTyp++;
	} while ((++i) < ConnectionCount);
	printf("AYCONN--> Conn There is something wrong ! \n");
	return 0xFFFFFFFF;///< fail !!!
}

Ui32 AYCONN_ReadConn(Ui32 ConnId, AY_CONNTYPE *pConnRd) {
	void *ptr;
	AY_CONNTYPE	*pConnTyp;
	Ui32 i;

#define i0		((i>>24)&0xFF)
#define i1		((i>>16)&0xFF)
#define i2		((i>>8)&0xFF)
#define i3		((i>>0)&0xFF)
	i = ConnId;
	ptr = ConnLevel1.pConnAdr[i0];
	if (ptr != 0) {
		printf("AYCONN--> Conn Test Level 1 i0=%d \n", i0);
		ptr = ((AY_CONNADR	*)ptr)->pConnAdr[i1];
		if (ptr != 0) {
			printf("AYCONN--> Conn Test Level 2 i1=%d \n", i1);
			ptr = ((AY_CONNADR	*)ptr)->pConnAdr[i2];
			if (ptr != 0) {///< valid address
				printf("AYCONN--> Conn Test Level 3 i2=%d \n", i2);
				pConnTyp = (AY_CONNTYPE *)((AY_CONNADR	*)ptr)->pConnAdr[i3];
				if (pConnTyp != 0) {///< valid connecton
					printf("AYCONN--> Conn Test Level 4 i3=%d \n", i3);
					memcpy(pConnRd, pConnTyp, sizeof(AY_CONNTYPE));
					return i;
				}
			}
		}
	}
	printf("AYCONN--> Conn There is something wrong ! \n");
	return 0xFFFFFFFF;
}

int AYCONN_UpdateTime(Ui32 ConnId) {
	void *ptr;
	AY_CONNTYPE	*pConnTyp;
	Ui32 i;

#define i0		((i>>24)&0xFF)
#define i1		((i>>16)&0xFF)
#define i2		((i>>8)&0xFF)
#define i3		((i>>0)&0xFF)
	i = ConnId;
	ptr = ConnLevel1.pConnAdr[i0];
	if (ptr != 0) {
		printf("AYCONN--> Time Test Level 1 i0=%d \n", i0);
		ptr = ((AY_CONNADR	*)ptr)->pConnAdr[i1];
		if (ptr != 0) {
			printf("AYCONN--> Time Test Level 2 i1=%d \n", i1);
			ptr = ((AY_CONNADR	*)ptr)->pConnAdr[i2];
			if (ptr != 0) {///< valid address
				printf("AYCONN--> Time Test Level 3 i2=%d \n", i2);
				pConnTyp = (AY_CONNTYPE *)((AY_CONNADR	*)ptr)->pConnAdr[i3];
				if (pConnTyp != 0) {///< valid connecton
					printf("AYCONN--> Time Test Level 4 i3=%d \n", i3);
					pConnTyp->_LastUpdateMin = AYCONN_ThisMinute();
					return i;
				}
			}
		}
	}
	printf("AYCONN--> Time There is something wrong ! \n");
	return 0xFFFFFFFF;
}

void AYCONN_ClientConnTimeoutTest(void) {
//	void *ptr;
//	AY_CONNADR	*pConnList;
//	AY_CONNTYPE	*pConnTyp;
//	Ui32 FirstFree;
//	Ui08 FirstFound = 0;
//	Ui32 i;
//
//#define i0		((i>>24)&0xFF)
//#define i1		((i>>16)&0xFF)
//#define i2		((i>>8)&0xFF)
//#define i3		((i>>0)&0xFF)
//	ptr = ConnLevel1.pConnAdr[0];
//	for (i = 0; i < ConnectionCount; i++) {
//		if ((i & 0x00FFFFFF) == 0) {
//			printf("AYCONN--> Tout Test Level 1 i0=%d \n", i0);
//			ptr = ConnLevel1.pConnAdr[i0];
//			if (ptr == 0) {///< create new level1 list
//				printf("AYCONN--> Tout this is last i=%d \n", i);
//				//ConnectionCount = i;
//			}
//		}
//		if ((i & 0x0000FFFF) == 0) {
//			printf("AYCONN--> Tout Test Level 2 i1=%d \n", i1);
//			ptr = ((AY_CONNADR	*)ConnLevel1.pConnAdr[i0])->pConnAdr[i1];
//			if (ptr == 0) {///< create new level2 list
//				printf("AYCONN--> Tout this is last i=%d \n", i);
//			}
//		}
//		if ((i & 0x000000FF) == 0) {
//			printf("AYCONN--> Tout Test Level 3 i2=%d \n", i2);
//			ptr = ((AY_CONNADR	*)(((AY_CONNADR	*)ConnLevel1.pConnAdr[i0])->pConnAdr[i1]))->pConnAdr[i2];
//			if (ptr == 0) {///< create new level3 list
//				if ((Func == AY_CONN_FIND) || (Func == AY_CONN_DELETE)) {
//					printf("AYCONN--> Tout Level 3 not found i=%d \n", i);
//					ConnectionCount = i;
//					return 0xFFFFFFFF;///< fail !!!
//				}
//				else {
//					pConnList = (AY_CONNADR	*)_AY_MallocMemory(sizeof(AY_CONNADR));
//					memset(pConnList, 0, sizeof(AY_CONNADR));
//					((AY_CONNADR	*)(((AY_CONNADR	*)ConnLevel1.pConnAdr[i0])->pConnAdr[i1]))->pConnAdr[i2] = (void *)pConnList;
//					ptr = ((AY_CONNADR	*)(((AY_CONNADR	*)ConnLevel1.pConnAdr[i0])->pConnAdr[i1]))->pConnAdr[i2];
//					printf("AYCONN--> New Conn Level 3 created i2=%d \n", i2);
//				}
//			}
//			pConnTyp = (AY_CONNTYPE *)(((AY_CONNADR	*)ptr)->pConnAdr[0]);///< level4 start index
//		}
//		if (pConnTyp == 0) {///< free socket found
//			if (FirstFound != 0xAA) {
//				FirstFound = 0xAA;
//				FirstFree = i;
//				printf("AYCONN--> First free Conn found FirstFree=%d \n", FirstFree);
//			}
//		}
//		else if ((pConnTyp->_UnqiueId[0] == Unique0) && (pConnTyp->_UnqiueId[1] == Unique1) && (pConnTyp->_UnqiueId[2] == Unique2)) {///< device found
//			printf("AYCONN--> Conn found FirstFree=%d \n", i);
//			if (Func == AY_CONN_UPDATE) {
//				memcpy(&pConnTyp->_UDPh, pUDP, sizeof(udp_headerAll));
//			}
//			else if (Func == AY_CONN_DELETE) {
//				_AY_FreeMemory((unsigned char *)pConnTyp);
//				((AY_CONNADR	*)((AY_CONNADR	*)(((AY_CONNADR	*)ConnLevel1.pConnAdr[i0])->pConnAdr[i1]))->pConnAdr[i2])->pConnAdr[i3] = 0;///< nullptr
//			}
//			return i;
//		}
//		if (i == ConnectionCount) {
//			printf("AYCONN--> Conn not found use FirstFree=%d \n", FirstFree);
//			i = FirstFree;
//			ptr = ((AY_CONNADR	*)(((AY_CONNADR	*)ConnLevel1.pConnAdr[i0])->pConnAdr[i1]))->pConnAdr[i2];
//			pConnTyp = (AY_CONNTYPE *)(((AY_CONNADR	*)ptr)->pConnAdr[i3]);///< level4 start index
//			if (Func == AY_CONN_UPDATE) {///< upload
//				memcpy(&pConnTyp->_UDPh, pUDP, sizeof(udp_headerAll));
//				pConnTyp->_LastUpdateMin = AYCONN_ThisMinute();
//			}
//			else if (Func == AY_CONN_READ) {///< download
//				memcpy(pUDP, &pConnTyp->_UDPh, sizeof(udp_headerAll));
//			}
//			return i;
//		}
//		pConnTyp++;
//	};
	printf("AYCONN--> Tout There is something wrong ! \n");
//	Sleep(AYCONN_TIMEOUT_SLEEP_mSEC);///< thread sleep
}
//extern int AYSCKT_StartThread(void *pCallBack);
int AYCONN_ClientConnInit(void) {
	memset(&ConnLevel1, 0, sizeof(AY_CONNADR));
	AYSCKT_StartThread(AYCONN_ClientConnTimeoutTest);
	return 1;
}

