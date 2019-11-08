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
	AY_CONNTYPE			*pConnTyp;
	AY_DEVSTRTIN		*pDevStrtIn;
	AY_DeviceStartResp	*pDevStrtPspHdr;
	tcp_headerAll		*pTCP,TCP_Send;
	Ui32				i,j,k;
	Ui32				m, n;//test sil !!!

	pDevStrtIn = (AY_DEVSTRTIN	*)pQ->pIn;
	printf("\n\t\tBEFORE-> ConnectionCount = %d\n Uniq0: 0x%08x , Uniq1: 0x%08x , Uniq2: 0x%08x \n", ConnectionCount, *((Ui32 *)&pDevStrtIn->_Unique[0]),*((Ui32 *)&pDevStrtIn->_Unique[4]), *((Ui32 *)&pDevStrtIn->_Unique[8]));
	i = AYCONN_FindOrAddConn(*((Ui32 *)&pDevStrtIn->_Unique[0]), *((Ui32 *)&pDevStrtIn->_Unique[4]), *((Ui32 *)&pDevStrtIn->_Unique[8]), &pDevStrtIn->_TCPh, &pDevStrtIn->_SessionKey[0], &pConnTyp, AY_CONN_UPDATE);
	if ((i != 0xFFFFFFFF) && (pConnTyp != nullptr)) {
		printf("\n\t\tAFTER-> ConnectionCount = %d\n", ConnectionCount);
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
		j = pConnTyp->_TCPh._tcpHeader.acknum;
		pConnTyp->_TCPh._tcpHeader.acknum = pConnTyp->_TCPh._tcpHeader.seqnum;
		pConnTyp->_TCPh._tcpHeader.seqnum = j;///< convert TCP counters
		pConnTyp->_TCPh._tcpHeader.acknum += sizeof(AY_DeviceStart);

		pTCP = (tcp_headerAll *)_AY_MallocMemory(4096);///< max packet size
		memcpy(pTCP, &pDevStrtIn->_TCPh, sizeof(tcp_headerAll));
		pTCP->_ethHeader.dest = pDevStrtIn->_TCPh._ethHeader.src;
		pTCP->_ethHeader.src = MyEth_Address;//pDevStrtIn->_TCPh._ethHeader.dest;
		pTCP->_ipHeader.daddr = pDevStrtIn->_TCPh._ipHeader.saddr;
		pTCP->_ipHeader.saddr = pDevStrtIn->_TCPh._ipHeader.daddr;
		pTCP->_tcpHeader.dport = pDevStrtIn->_TCPh._tcpHeader.sport;
		pTCP->_tcpHeader.sport = pDevStrtIn->_TCPh._tcpHeader.dport;
		pTCP->_tcpHeader.seqnum = pConnTyp->_TCPh._tcpHeader.seqnum;
		pTCP->_tcpHeader.acknum = pConnTyp->_TCPh._tcpHeader.acknum;

		pDevStrtPspHdr = (AY_DeviceStartResp	*)(((Ui08 *)pTCP)+sizeof(tcp_headerAll));///< max packet size
		j = 0; k = 0;
		printf("AYDVSTRT--> SSK = "); AY_HexValPrint((Ui08 *)&pDevStrtIn->_SessionKey[0], 16); printf("\r\n");
		printf("AYDVSTRT--> SSK = "); AY_HexValPrint((Ui08 *)&MYSQL_Gateway._SessionKey[0], 16); printf("\r\n");
		do {
			i = MYSQL_ReadDeviceList(0, (MYSQL_DeviceRead *)(((Ui08 *)pDevStrtPspHdr) + sizeof(AY_DeviceStartResp)), 168, j);
			pDevStrtPspHdr->_Test0 = PACKET_TEST_DATA0;
			pDevStrtPspHdr->_Test1 = PACKET_TEST_DATA1;
			pDevStrtPspHdr->_DevCnt = i;
			pDevStrtPspHdr->_DevPcktNo = k;
			//============== FILE========================//
			m = sizeof(AY_DeviceStartResp) + (((Ui16)i) * sizeof(AY_DeviceRead));
			printf("AYDVSTRT--> %d ============ FILE START =========\n ", m);
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
			TCP_packet_send(_MAIN_SCKT, pTCP, (Ui08 *)pDevStrtPspHdr, (sizeof(AY_DeviceStartResp) + n/*(((Ui16)i) * sizeof(AY_DeviceRead))*/));
			pConnTyp->_TCPh._tcpHeader.seqnum = pTCP->_tcpHeader.seqnum;
#if STEP_TEST==1
			printf("********* STEP 0 *************\n********* STEP 5 *************\n********* STEP 7 *************\n");
			AYPRINT_TCP_Header(pTCP);
#endif
		} while (i >= 168);

		MYSQL_FindLoadDeviceInfoID(0, (Ui32 *)&pDevStrtIn->_Unique);
		printf("AYDVSTRT--> SSK = "); AY_HexValPrint((Ui08 *)/*&pDevStrtIn->_SessionKey[0]*/ &MYSQL_Gateway._SessionKey[0], 16); printf("\n\n\n\t\t************** CONN END ************\n\t\t************** CONN END ************\n\t\t************** CONN END ************\n\n\r\n");

		_AY_FreeMemory((unsigned char*)pTCP);
	}
	else {
		printf("AYDVSTRT--> ERROR !!! Connection ID ConnId=%d \n", i);
	}
	///< delete from queue
	pQ->QFlg._QFinishedF = 1;
	pQ->QFlg._QKeepF = 0;
	pQ->QFlg._QBusyF = 1;
}

int AY_TestLoadDeviceStart(Ui08 *pPtr,Ui16 Len) {
	tcp_headerAll		*pTCP;
	AY_DeviceStart	*pDevStrt;
	AY_DEVSTRTIN	*pDevStrtIn;
	Ui08			*pOut;
	Ui16			j;
	Si32			i;

	pDevStrt = (AY_DeviceStart	*)(pPtr + sizeof(tcp_headerAll));
	if ((pDevStrt->_Test0 == PACKET_TEST_DATA0) && (pDevStrt->_Test1 == PACKET_TEST_DATA1)) {
		printf("AYDVSTRT--> Packet type is DeviceStart\n");
		if (Len == sizeof(AY_DeviceStart)) {
			pOut = (Ui08	*)_AY_MallocMemory(256);
			pDevStrt = (AY_DeviceStart	*)_AY_MallocMemory(sizeof(AY_DeviceStart));
			memcpy(pDevStrt, (AY_DeviceStart	*)(pPtr + sizeof(tcp_headerAll)), sizeof(AY_DeviceStart));
			///<===== Check Client's Sign ===================
			AY_Crypt_RSAVerify((Ui08 *)&CLIENT_PUB_KEY[0], (Ui08 *)&pDevStrt->_Input[0], 256, (Ui08 *)&pDevStrt->_Sign[0]);
			///<===== Decrpt Data ===================
			AY_Crypt_RSADecrpt((Ui08 *)&SERVER_PR_KEY[0], (Ui08 *)&pDevStrt->_Input[0], 256, (Ui08 *)pOut, &j);
			memcpy(&pDevStrt->_Input[0], pOut, 256);
			_AY_FreeMemory((unsigned char*)pOut);
			//======= Generate AY_DEVSTRTIN
			pTCP = (tcp_headerAll *)(pPtr + 0); // udp all header
#if STEP_TEST==1
			printf("********* STEP 0 *************\n********* STEP 5 *************\n********* STEP 7 *************\n");
			AYPRINT_TCP_Header(pTCP);
#endif
			pDevStrtIn = (AY_DEVSTRTIN	*)_AY_MallocMemory(sizeof(AY_DEVSTRTIN));
			memcpy(&pDevStrtIn->_TCPh, pTCP, sizeof(tcp_headerAll));
			pDevStrtIn->_LocalCertNo = pDevStrt->_LocalCertNo;
			pDevStrtIn->_ServerCertNo = pDevStrt->_ServerCertNo;
			memcpy(&pDevStrtIn->_MAC, &pDevStrt->_MAC, 6);
			memcpy(&pDevStrtIn->_Name, &pDevStrt->_Name, 45);
			memcpy(&pDevStrtIn->_Pswd, &pDevStrt->_Pswd, 45);
			memcpy(&pDevStrtIn->_SessionKey, &pDevStrt->_SessionKey, sizeof(SSK_));
			printf("AYDVSTRT--> SSK = "); AY_HexValPrint(&pDevStrtIn->_SessionKey[0], 16); printf("\r\n");
			memcpy(&pDevStrtIn->_Unique, &pDevStrt->_Unique, 12);
			//======= Release AY_DeviceStart
			_AY_FreeMemory((unsigned char*)pDevStrt);
			//==================== Test & Response if There is a waiting GWInfo Request
			AY_CONNTYPE	*pSrc, *pDst;
			pDst = pFindConnByUniqueID((UNIQUE_ID *)&pDevStrtIn->_Unique[0]);
			if (pDst != nullptr) {//ieriki adým sonra aç !!!
				//pDst->_TCPh._tcpHeader.acknum += Len;
				i = AYSRV_FindUniqQ(*((UNIQUE_ID *)&pDevStrtIn->_Unique[0]/*not used*/), *((UNIQUE_ID *)&pDevStrtIn->_Unique[0]), _UNIQ_NOT_SRC);
				if (i >= 0) {
					if (UniqQ_Lst.UniqQ[i].UniqFnc == _UNIQUE_Q_RENT) {
						pSrc = pFindConnByUniqueID((UNIQUE_ID *)&UniqQ_Lst.UniqQ[i].SrcUniq);
						if (pSrc != nullptr) {
							AY_GWINFORESP		GwRsp;
							tcp_headerAll		TCPheader;
							Ui16				oLen;

							GwRsp._Test2 = PACKET_TEST_DATA2;
							GwRsp._Test3 = PACKET_TEST_DATA3;
							GwRsp._LastUpdateMin =  AYCONN_ThisMinute();
							GwRsp._QueRowNo = UniqQ_Lst.UniqQ[i].PrcsNo;
							memcpy(&GwRsp._SessionKey, &pDevStrtIn->_SessionKey, sizeof(SSK_));
							memcpy(&GwRsp._TCPh, &pDst->_TCPh/*pDevStrtIn->_TCPh*/, sizeof(tcp_headerAll));
							AY_Crypt_AES128((Ui08 *)&pSrc->_SessionKey[0], (Ui08 *)&GwRsp._InfoCont[0], sizeof(GwRsp._InfoCont));
							//--------------
							memcpy(&TCPheader, &pSrc->_TCPh, sizeof(tcp_headerAll));
							AY_ChngPacketDest_TCP(&TCPheader, &MyEth_Address, _ETH_DST_);
							oLen = sizeof(AY_GWINFORESP);
#if STEP_TEST==1
							printf("********* STEP 6 *************\n********* STEP 6 *************\n********* STEP 6 *************\n");
							AYPRINT_TCP_Header(&TCPheader);
#endif
							TCP_packet_send(_MAIN_SCKT, &TCPheader, (Ui08 *)&GwRsp, oLen);
							pSrc->_TCPh._tcpHeader.seqnum += oLen;
							//--------------
							AYSRV_UniqQ_Init(i);
						}
					}
				}
			}
			//======= Load to Queue
			return( AYSRV_QueueLoad(AYSRV_QueueFindFirstFreeRow(), (Ui08 *)pDevStrtIn, sizeof(AY_DEVSTRTIN), QTARGET_CLIENT_CONN, 0));
		}
		printf("AYDVSTRT--> Packet fail\n");
	}
	return 0;///< not me
}

int AY_TestLoadGwInfoRqst(Ui08 *pPtr, Ui16 Len) {
	AY_GWINFORQST	*pInfoRqst;
	Si32			i;

	pInfoRqst = (AY_GWINFORQST	*)(pPtr + sizeof(tcp_headerAll));
	if ((pInfoRqst->_Test2 == PACKET_TEST_DATA2) && (pInfoRqst->_Test3 == PACKET_TEST_DATA3)) {
#if STEP_TEST==1
		printf("********* STEP 3 *************\n********* STEP 3 *************\n********* STEP 3 *************\n");
		AYPRINT_TCP_Header((tcp_headerAll *)pPtr);
#endif
		printf("AYDVSTRT--> Packet type is Info Request\n");
		if (Len == sizeof(AY_GWINFORQST)) {
			AY_CONNTYPE	*pSrc, *pDst;
			AY_GWRENTRQST	GwRent;
			pSrc = pFindConnByTCPheader((tcp_headerAll *)pPtr);
			if (pSrc != nullptr) {
				i = AYSRV_UniqQ_FindFirstFreeRow();
				if (i != -1) {
					pInfoRqst = (AY_GWINFORQST	*)_AY_MallocMemory(sizeof(AY_GWINFORQST) + 16);
					memcpy(pInfoRqst, (AY_GWINFORQST	*)(pPtr + sizeof(tcp_headerAll)), sizeof(AY_GWINFORQST));
					//---------------------------//
					AY_Decrypt_AES128((Ui08 *)&pSrc->_SessionKey[0], (Ui08 *)&pInfoRqst->_InfoCont[0], AY_GWINFORQST_SIZE_OF_INFO_CONT);
					pDst = pFindConnByUniqueID((UNIQUE_ID *)&pInfoRqst->_Unique[0]);
					if (pDst != nullptr) {
						tcp_headerAll		TCPheader;
						Ui16 oLen;

						pDst->_TCPh._tcpHeader.acknum += Len;
						//---------------------------//
						AYSRV_UniqQ_Load(i, *((UNIQUE_ID *)&pSrc->_UnqiueId[0]), *((UNIQUE_ID *)&pInfoRqst->_Unique[0]), pInfoRqst->_QueRowNo, _UNIQUE_Q_RENT, 0, 0);
						//---------------------------//
						GwRent._Test4 = PACKET_TEST_DATA4;
						GwRent._Test5 = PACKET_TEST_DATA5;
						GwRent._LastUpdateMin = AYCONN_ThisMinute();
						memcpy(&GwRent._TCPh, (tcp_headerAll *)pPtr, sizeof(tcp_headerAll));
						memcpy(&GwRent._SessionKey, &pSrc->_SessionKey, sizeof(SSK_));
						memcpy(&GwRent._Unique, &pSrc->_UnqiueId, sizeof(UNIQUE_ID));
						AY_Crypt_AES128((Ui08 *)&pDst->_SessionKey[0], (Ui08 *)&GwRent._InfoCont[0], AY_GWRENTRQSTT_SIZE_OF_INFO_CONT);
						//---------------------------//
						memcpy(&TCPheader, &pDst->_TCPh, sizeof(tcp_headerAll));
						AY_ChngPacketDest_TCP(&TCPheader, &MyEth_Address, _ETH_DST_);
						oLen = sizeof(AY_GWRENTRQST);
#if STEP_TEST==1
						printf("********* STEP 4 *************\n********* STEP 4 *************\n********* STEP 4 *************\n");
						AYPRINT_TCP_Header(&TCPheader);
#endif
						i =  TCP_packet_send(_MAIN_SCKT, &TCPheader, (Ui08 *)&GwRent, oLen);
						pDst->_TCPh._tcpHeader.seqnum += oLen;
						_AY_FreeMemory((unsigned char *)pInfoRqst);
						return i;
					}
					_AY_FreeMemory((unsigned char *)pInfoRqst);
				}				
			}
		}
		printf("AYGWINFORQST--> Packet fail\n");
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
Ui32 AYCONN_FindOrAddConn(Ui32 Unique0, Ui32 Unique1, Ui32 Unique2, tcp_headerAll *pTCP, Ui08 *pSSK, AY_CONNTYPE **pConn, Ui08 Func) {
	void *ptr;
	AY_CONNADR	*pConnList;
	AY_CONNTYPE	*pConnTyp, *pConnTst;
	Ui32 FirstFree= ConnectionCount;
	Ui08 FirstFound = 0;
	Ui32 i;

#define i0		((i>>24)&0xFF)
#define i1		((i>>16)&0xFF)
#define i2		((i>>8)&0xFF)
#define i3		((i>>0)&0xFF)
	ptr = ConnLevel1.pConnAdr[0];
	i = 0; 
	pConnTyp = 0;
	*pConn = nullptr;
	do{
		//pConnTyp = (AY_CONNTYPE *)(((AY_CONNADR	*)((AY_CONNADR	*)(((AY_CONNADR	*)ConnLevel1.pConnAdr[i0])->pConnAdr[i1]))->pConnAdr[i2])->pConnAdr[i3]);//test!!!!
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
				memcpy(&pConnTyp->_TCPh, pTCP, sizeof(tcp_headerAll));
				memcpy(&pConnTyp->_SessionKey[0], pSSK, 16);
			}
			else if (Func == AY_CONN_DELETE) {
				_AY_FreeMemory((unsigned char *)pConnTyp);
				((AY_CONNADR	*)((AY_CONNADR	*)(((AY_CONNADR	*)ConnLevel1.pConnAdr[i0])->pConnAdr[i1]))->pConnAdr[i2])->pConnAdr[i3] = 0;///< nullptr
			}
			*pConn = pConnTyp;///< point conn values
			return i;
		}
		if (i == ConnectionCount) {
			printf("AYCONN--> Conn not found use FirstFree=%d \n", FirstFree);
			i = FirstFree;
			ptr = ((AY_CONNADR	*)(((AY_CONNADR	*)ConnLevel1.pConnAdr[i0])->pConnAdr[i1]))->pConnAdr[i2];			
			if (Func == AY_CONN_UPDATE) {///< upload
				((AY_CONNADR	*)ptr)->pConnAdr[i3] = _AY_MallocMemory(sizeof(AY_CONNTYPE));
				pConnTyp = (AY_CONNTYPE *)(((AY_CONNADR	*)ptr)->pConnAdr[i3]);///< level4 start index
				pConnTyp->_UnqiueId[0] = Unique0;
				pConnTyp->_UnqiueId[1] = Unique1;
				pConnTyp->_UnqiueId[2] = Unique2;
				memcpy(&pConnTyp->_TCPh, pTCP, sizeof(tcp_headerAll));
				memcpy(&pConnTyp->_SessionKey[0], pSSK, 16);
				pConnTyp->_LastUpdateMin = AYCONN_ThisMinute();
				//============ TEST =========================//
				printf("AYCONN--> Input Conn i0=0x%08x, i1=0x%08x, i2=0x%08x, i3=0x%08x \n", (Ui32)ConnLevel1.pConnAdr[i0], (Ui32)(((AY_CONNADR	*)ConnLevel1.pConnAdr[i0])->pConnAdr[i1]), (Ui32)(((AY_CONNADR	*)(((AY_CONNADR	*)ConnLevel1.pConnAdr[i0])->pConnAdr[i1]))->pConnAdr[i2]), (Ui32)pConnTyp);
				pConnTst = pAYCONN_ReadConn(i);				
				//============ TEST =========================//
				ConnectionCount++;
				printf("AYCONN--> Conn new added i3=%d ConnectionCount=%d Adr=0x%08x \n", i3, ConnectionCount, (Ui32)pConnTyp);
			}
			/*else if (Func == AY_CONN_READ) {///< download
				memcpy(pUDP, &pConnTyp->_UDPh, sizeof(udp_headerAll));
			}*/
			*pConn = pConnTyp;///< point conn values
			return i;
		}
		pConnTyp = (AY_CONNTYPE *)(((AY_CONNADR	*)ptr)->pConnAdr[i3]);///< level4 start index//pConnTyp++;
	} while ((++i) <= ConnectionCount);
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
		printf("AYCONN--> Conn Test Level 1 i0=%d Adr=0x%08x \n", i0, (Ui32)ptr);
		ptr = ((AY_CONNADR	*)ptr)->pConnAdr[i1];
		if (ptr != 0) {
			printf("AYCONN--> Conn Test Level 2 i1=%d Adr=0x%08x \n", i1, (Ui32)ptr);
			ptr = ((AY_CONNADR	*)ptr)->pConnAdr[i2];
			if (ptr != 0) {///< valid address
				printf("AYCONN--> Conn Test Level 3 i2=%d Adr=0x%08x \n", i2, (Ui32)ptr);
				pConnTyp = (AY_CONNTYPE *)((AY_CONNADR	*)ptr)->pConnAdr[i3];
				if (pConnTyp != 0) {///< valid connecton
					printf("AYCONN--> Conn Test Level 4 i3=%d Adr=0x%08x \n", i3, (Ui32)pConnTyp);
					memcpy(pConnRd, pConnTyp, sizeof(AY_CONNTYPE));
					return i;
				}
			}
		}
	}
	printf("AYCONN--> Conn There is something wrong ! \n");
	return 0xFFFFFFFF;
}

AY_CONNTYPE	*pAYCONN_ReadConn(Ui32 ConnId) {
	void *ptr;
	AY_CONNTYPE	*pConnTyp;
	Ui32 i;

#define i0		((i>>24)&0xFF)
#define i1		((i>>16)&0xFF)
#define i2		((i>>8)&0xFF)
#define i3		((i>>0)&0xFF)
	i = ConnId;
	ptr = ConnLevel1.pConnAdr[i0];
	if ((ptr != 0)|| (ptr != nullptr)) {
		printf("AYCONN--> Conn Test Level 1 i0=%d Adr=0x%08x \n", i0, (Ui32)ptr);
		ptr = ((AY_CONNADR	*)ptr)->pConnAdr[i1];
		if ((ptr != 0) || (ptr != nullptr)) {
			printf("AYCONN--> Conn Test Level 2 i1=%d Adr=0x%08x \n", i1, (Ui32)ptr);
			ptr = ((AY_CONNADR	*)ptr)->pConnAdr[i2];
			if ((ptr != 0) || (ptr != nullptr)) {///< valid address
				printf("AYCONN--> Conn Test Level 3 i2=%d Adr=0x%08x \n", i2, (Ui32)ptr);
				pConnTyp = (AY_CONNTYPE *)((AY_CONNADR	*)ptr)->pConnAdr[i3];
				if ((pConnTyp != 0) || (pConnTyp != nullptr)) {///< valid connecton
					printf("AYCONN--> Conn found - Conn Test Level 4 i=%d Adr=0x%08x \n", i, (Ui32)pConnTyp);
					//memcpy(pConnRd, pConnTyp, sizeof(AY_CONNTYPE));
					return pConnTyp;
				}
			}
		}
	}
	printf("AYCONN--> Conn Not found - Conn There is something wrong ! i=%d \n", i);
	return nullptr;
}

AY_CONNTYPE	*pFindConnByTCPheader(tcp_headerAll *pTCP) {
	AY_CONNTYPE	*pConnTyp = nullptr;
	Ui32 i;

	for (i = 0; i < ConnectionCount; i++) {
		pConnTyp = pAYCONN_ReadConn(i);
		if (pConnTyp != nullptr) {
			if ((pConnTyp->_TCPh._ipHeader.saddr.longip == pTCP->_ipHeader.saddr.longip) && (pConnTyp->_TCPh._tcpHeader.sport == pTCP->_tcpHeader.sport)) {
				printf("AYCONN--> Conn found - UDP header matched i=%d \n", i);
				return pConnTyp;
			}
		}
	}
	printf("AYCONN--> Conn not found - UDP header not matched i=%d \n", i);
	return nullptr;
}

AY_CONNTYPE	*pFindConnByUniqueID(UNIQUE_ID *pUnique) {
	AY_CONNTYPE	*pConnTyp = nullptr;
	Ui32 i;

	for (i = 0; i < ConnectionCount; i++) {
		pConnTyp = pAYCONN_ReadConn(i);
		if (pConnTyp != nullptr) {
			printf("\n\t\tLook = %d\n Uniq0: 0x%08x , Uniq1: 0x%08x , Uniq2: 0x%08x \n", i, pUnique->_UniqueL[0], pUnique->_UniqueL[1], pUnique->_UniqueL[2]);
			printf("\n\t\tConnectionNo = %d\n Uniq0: 0x%08x , Uniq1: 0x%08x , Uniq2: 0x%08x \n", i, pConnTyp->_UnqiueId[0], pConnTyp->_UnqiueId[1], pConnTyp->_UnqiueId[2]);
			if ((pConnTyp->_UnqiueId[0] == pUnique->_UniqueL[0]) && (pConnTyp->_UnqiueId[1] == pUnique->_UniqueL[1]) && (pConnTyp->_UnqiueId[2] == pUnique->_UniqueL[2])) {///< device found 
				printf("AYCONN--> Conn found - Unique ID matched i=%d \n", i);
				return pConnTyp;
			}
		}
	}
	printf("AYCONN--> Conn not found - Unique ID not matched i=%d \n", i);
	return nullptr;
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
	if ((ptr != 0) || (ptr != nullptr)) {
		printf("AYCONN--> Time Test Level 1 i0=%d \n", i0);
		ptr = ((AY_CONNADR	*)ptr)->pConnAdr[i1];
		if ((ptr != 0) || (ptr != nullptr)) {
			printf("AYCONN--> Time Test Level 2 i1=%d \n", i1);
			ptr = ((AY_CONNADR	*)ptr)->pConnAdr[i2];
			if ((ptr != 0) || (ptr != nullptr)) {///< valid address
				printf("AYCONN--> Time Test Level 3 i2=%d \n", i2);
				pConnTyp = (AY_CONNTYPE *)((AY_CONNADR	*)ptr)->pConnAdr[i3];
				if ((pConnTyp != 0) || (pConnTyp != nullptr)) {///< valid connecton
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
	/*while (1) {		
		Sleep(1);
	}*/


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
//	printf("AYCONN--> Tout There is something wrong ! \n");
//	Sleep(AYCONN_TIMEOUT_SLEEP_mSEC);///< thread sleep
}
//extern int AYSCKT_StartThread(void *pCallBack);
int AYCONN_ClientConnInit(void) {
	memset(&ConnLevel1, 0, sizeof(AY_CONNADR));
	AYSCKT_StartThread(TIMEOUT_Process);
	return 1;
}

void TimeoutDoTask(int ql) {
	AYCONN_ClientConnTimeoutTest();
}



