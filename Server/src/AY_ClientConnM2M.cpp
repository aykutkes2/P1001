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
#include <AY_ClientConnM2M.h>

int AY_TestLoadDirectSendRqst(Ui08 *pPtr, Ui16 Len) {
	AY_GWDRCTHDR		*pGwDH;
	Ui32				j;
	Si32			i;

	pGwDH = (AY_GWDRCTHDR	*)(pPtr + sizeof(tcp_headerAll));
	if (pGwDH->_Test10 == PACKET_TEST_DATA10) {
#if STEP_TEST==1
		printf("********* STEP D2 *************\n********* STEP D2 *************\n********* STEP D2 *************\n");
		AYPRINT_TCP_Header((tcp_headerAll *)pPtr);
#endif
		printf("AYDVSTRT--> Packet type is Direct Send\n");
		AY_CONNTYPE		*pSrc;
		pSrc = pFindConnByTCPheader((tcp_headerAll *)pPtr);///< Find Request GW
		if (pSrc != nullptr) {
			AY_M2M_CONNTYPE			*pConnM2M;
			Ui08					*pPckt;

			// ack seq düþün !!!
			j = pSrc->_TCPh._tcpHeader.acknum;
			pSrc->_TCPh._tcpHeader.acknum = pSrc->_TCPh._tcpHeader.seqnum;
			pSrc->_TCPh._tcpHeader.seqnum = j;///< convert TCP counters
			pSrc->_TCPh._tcpHeader.acknum += _HTONSL( sizeof(AY_DeviceStart) );
			//----------------------------------------------------------------//
			pConnM2M = (AY_M2M_CONNTYPE	*)_AY_MallocMemory(sizeof(AY_M2M_CONNTYPE) + Len);
			pPckt = ((Ui08 *)pConnM2M) + sizeof(AY_M2M_CONNTYPE);
			memcpy(pPckt, pGwDH, Len);///< copy received packet
			//---------------------------//
			AY_Decrypt_AES128((Ui08 *)&pSrc->_SessionKey[0], ((Ui08 *)pPckt+sizeof(AY_GWDRCTHDR)), (Len- sizeof(AY_GWDRCTHDR)));
			//---------------------------//
			pConnM2M->_Src._pConn = pSrc;
			pConnM2M->_Src._DevNo = pGwDH->_DevNo;
			pConnM2M->M_LastUpdateMin = pGwDH->_ConM2M_Id;
			//---------------------------//
			//======= Load to Queue
			return(AYSRV_QueueLoad(AYSRV_QueueFindFirstFreeRow(), (Ui08 *)pConnM2M, (sizeof(AY_M2M_CONNTYPE) + Len), QTARGET_DIRECT_SEND, 0));
		}
	}
	else if (pGwDH->_Test10 == PACKET_TEST_DATA12) {
#if STEP_TEST==1
		printf("********* STEP D6 *************\n********* STEP D6 *************\n********* STEP D6 *************\n");
		AYPRINT_TCP_Header((tcp_headerAll *)pPtr);
#endif
		printf("AYDVSTRT--> Packet type is Direct Response\n");
		AY_CONNTYPE		*pSrc;
		pSrc = pFindConnByTCPheader((tcp_headerAll *)pPtr);///< Find Response GW
		if (pSrc != nullptr) {
			AY_M2M_CONNTYPE			*pConnM2M;
			Ui08					*pPckt;

			// ack seq düþün !!!
			j = pSrc->_TCPh._tcpHeader.acknum;
			pSrc->_TCPh._tcpHeader.acknum = pSrc->_TCPh._tcpHeader.seqnum;
			pSrc->_TCPh._tcpHeader.seqnum = j;///< convert TCP counters
			pSrc->_TCPh._tcpHeader.acknum += _HTONSL( sizeof(AY_DeviceStart) );
			//----------------------------------------------------------------//
			pConnM2M = (AY_M2M_CONNTYPE	*)_AY_MallocMemory(sizeof(AY_M2M_CONNTYPE) + Len);
			pPckt = ((Ui08 *)pConnM2M) + sizeof(AY_M2M_CONNTYPE);
			memcpy(pPckt, pGwDH, Len);///< copy received packet
			//---------------------------//
			AY_Decrypt_AES128((Ui08 *)&pSrc->_SessionKey[0], ((Ui08 *)pPckt + sizeof(AY_GWDRCTHDR)), (Len - sizeof(AY_GWDRCTHDR)));
			//---------------------------//			
			pConnM2M->_Src._pConn = pSrc;
			pConnM2M->_Src._DevNo = pGwDH->_DevNo;
			pConnM2M->M_LastUpdateMin = pGwDH->_ConM2M_Id;
			//---------------------------//
			//======= Load to Queue
			return(AYSRV_QueueLoad(AYSRV_QueueFindFirstFreeRow(), (Ui08 *)pConnM2M, (sizeof(AY_M2M_CONNTYPE) + Len), QTARGET_DIRECT_RESP, 0));
		}
	}
	return 0;///< not me
}

void AYSRV_QueueDirectSend(AY_QUEUE *pQ) {
	AY_M2M_CONNTYPE			*pConnM2M;
	tcp_headerAll			*pTCP;
	AY_CONNTYPE				*pDst;
	Ui08					*pData;
	tcp_headerAll			TCPheader;
	Ui16					oLen;
	Ui32					ConM2M_Id;

	pConnM2M = (AY_M2M_CONNTYPE	*)pQ->pIn;
	pConnM2M = pAYM2M_FindOrAddConn(pConnM2M, &ConM2M_Id, AY_CONN_UPDATE);

	pDst = pConnM2M->_Dst._pConn;
	pData = ((Ui08 *)pQ->pIn) + sizeof(AY_M2M_CONNTYPE);// +sizeof(AY_GWDRCTHDR);
	((AY_GWDRCTHDR *)pData)->_Test10 = PACKET_TEST_DATA11;
	((AY_GWDRCTHDR *)pData)->_ConM2M_Id = ConM2M_Id;
	((AY_GWDRCTHDR *)pData)->_DevNo = pConnM2M->_Dst._DevNo;
	oLen = ((pQ->InLen - (sizeof(AY_M2M_CONNTYPE) + sizeof(AY_GWDRCTHDR)) + 15) & 0xFFF0);
	//---------------------------//
	AY_Crypt_AES128((Ui08 *)&pDst->_SessionKey[0], (pData + sizeof(AY_GWDRCTHDR)), oLen);
	//------- SEND
	memcpy(&TCPheader, &pDst->_TCPh, sizeof(tcp_headerAll));
	AY_ChngPacketDest_TCP(&TCPheader, &MyEth_Address, _ETH_DST_);
#if STEP_TEST==1
	printf("********* STEP D3 *************\n********* STEP D3 *************\n********* STEP D3 *************\n");
	AYPRINT_TCP_Header(&TCPheader);
#endif
	oLen += sizeof(AY_GWDRCTHDR);
	TCP_packet_send(_MAIN_SCKT, &TCPheader, (Ui08 *)pData, oLen);
	pDst->_TCPh._tcpHeader.acknum = TCPheader._tcpHeader.acknum;
	pDst->_TCPh._tcpHeader.seqnum = TCPheader._tcpHeader.seqnum;
	//--------------
	///< delete from queue
	pQ->QFlg._QFinishedF = 1;
	pQ->QFlg._QKeepF = 0;
	pQ->QFlg._QBusyF = 1;
	return;
}

void AYSRV_QueueDirectResponse(AY_QUEUE *pQ) {
	AY_M2M_CONNTYPE			*pConnM2M;
	tcp_headerAll			*pTCP;
	AY_CONNTYPE				*pDst;
	Ui08					*pData;
	tcp_headerAll			TCPheader;
	Ui16					oLen;
	Ui32					ConM2M_Id;

	pConnM2M = (AY_M2M_CONNTYPE	*)pQ->pIn;
	ConM2M_Id = pConnM2M->M_LastUpdateMin;
	pConnM2M = pAYM2M_ReadConn(ConM2M_Id);// pAYM2M_FindOrAddConn(pConnM2M, &ConM2M_Id, AY_CONN_FIND);

	pDst = pConnM2M->_Src._pConn;
	pData = ((Ui08 *)pQ->pIn) + sizeof(AY_M2M_CONNTYPE);// +sizeof(AY_GWDRCTHDR);
	((AY_GWDRCTHDR *)pData)->_Test10 = PACKET_TEST_DATA13;
	((AY_GWDRCTHDR *)pData)->_ConM2M_Id = ConM2M_Id;
	((AY_GWDRCTHDR *)pData)->_DevNo = pConnM2M->_Src._DevNo;
	oLen = ((pQ->InLen - (sizeof(AY_M2M_CONNTYPE) + sizeof(AY_GWDRCTHDR)) + 15) & 0xFFF0);
	//---------------------------//
	AY_Crypt_AES128((Ui08 *)&pDst->_SessionKey[0], (pData + sizeof(AY_GWDRCTHDR)), oLen);
	//------- SEND
	memcpy(&TCPheader, &pDst->_TCPh, sizeof(tcp_headerAll));
	AY_ChngPacketDest_TCP(&TCPheader, &MyEth_Address, _ETH_DST_);
#if STEP_TEST==1
	printf("********* STEP D7 *************\n********* STEP D7 *************\n********* STEP D7 *************\n");
	AYPRINT_TCP_Header(&TCPheader);
#endif
	oLen += sizeof(AY_GWDRCTHDR);
	TCP_packet_send(_MAIN_SCKT, &TCPheader, (Ui08 *)pData, oLen);
	pDst->_TCPh._tcpHeader.acknum = TCPheader._tcpHeader.acknum;
	pDst->_TCPh._tcpHeader.seqnum = TCPheader._tcpHeader.seqnum;
	//--------------
	///< delete from queue
	pQ->QFlg._QFinishedF = 1;
	pQ->QFlg._QKeepF = 0;
	pQ->QFlg._QBusyF = 1;
	return;
}



//============================================================================================================
//====================== M2M CONNECTIONS =========================================================================
Ui32		M2M_ConnectionCount = 0;
AY_CONNADR	M2M_ConnLevel1;
Ui32 AYM2M_ThisMinute(void) {
	return ((Ui32)(((Ui64)time(0)) / 60));
}
int AYM2M_TestMinute(Ui32 RecMin, Ui32 TimeOut) {
	Ui32 Val = AYM2M_ThisMinute() - RecMin;
	if (Val & 0x80000000) { Val = 0 - Val; }
	if (Val > TimeOut) { return 1; }
	else { return 0; }
}
AY_M2M_CONNTYPE *pAYM2M_FindOrAddConn(AY_M2M_CONNTYPE *pTempM2M, Ui32 *pId, Ui08 Func) {
	void					*ptr;
	AY_CONNADR				*pConnList;
	AY_M2M_CONNTYPE			*pConnTypM2M, *pConnTst;
	AY_CONNTYPE				*pConnTyp = nullptr;
	Ui32					FirstFree = M2M_ConnectionCount;
	Ui08					FirstFound = 0;
	Ui32					i;

#define i0		((i>>24)&0xFF)
#define i1		((i>>16)&0xFF)
#define i2		((i>>8)&0xFF)
#define i3		((i>>0)&0xFF)
	ptr = M2M_ConnLevel1.pConnAdr[0];
	i = 0;
	pConnTypM2M = 0;
	if (pId != nullptr) {*pId = 0;}
	do {
		//pConnTypM2M = (AY_M2M_CONNTYPE *)(((AY_CONNADR	*)((AY_CONNADR	*)(((AY_CONNADR	*)M2M_ConnLevel1.pConnAdr[i0])->pConnAdr[i1]))->pConnAdr[i2])->pConnAdr[i3]);//test!!!!
		if ((i & 0x00FFFFFF) == 0) {
			printf("AYM2M--> Conn Test Level 1 i0=%d \n", i0);
			ptr = M2M_ConnLevel1.pConnAdr[i0];
			if (ptr == 0) {///< create new level1 list
				if ((Func == AY_CONN_FIND) || (Func == AY_CONN_DELETE)) {
					printf("AYM2M--> Conn Level 1 not found i=%d \n", i);
					M2M_ConnectionCount = i;
					return nullptr;///< fail !!!
				}
				else {
					pConnList = (AY_CONNADR	*)_AY_MallocMemory(sizeof(AY_CONNADR));
					memset(pConnList, 0, sizeof(AY_CONNADR));
					M2M_ConnLevel1.pConnAdr[i0] = (void *)pConnList;
					printf("AYM2M--> New Conn Level 1 created i0=%d \n", i0);
				}
			}
		}
		if ((i & 0x0000FFFF) == 0) {
			printf("AYM2M--> Conn Test Level 2 i1=%d \n", i1);
			ptr = ((AY_CONNADR	*)M2M_ConnLevel1.pConnAdr[i0])->pConnAdr[i1];
			if (ptr == 0) {///< create new level2 list
				if ((Func == AY_CONN_FIND) || (Func == AY_CONN_DELETE)) {
					printf("AYM2M--> Conn Level 2 not found i=%d \n", i);
					M2M_ConnectionCount = i;
					return nullptr;///< fail !!!
				}
				else {
					pConnList = (AY_CONNADR	*)_AY_MallocMemory(sizeof(AY_CONNADR));
					memset(pConnList, 0, sizeof(AY_CONNADR));
					((AY_CONNADR	*)M2M_ConnLevel1.pConnAdr[i0])->pConnAdr[i1] = (void *)pConnList;
					printf("AYM2M--> New Conn Level 2 created i1=%d \n", i1);
				}
			}
		}
		if ((i & 0x000000FF) == 0) {
			printf("AYM2M--> Conn Test Level 3 i2=%d \n", i2);
			ptr = ((AY_CONNADR	*)(((AY_CONNADR	*)M2M_ConnLevel1.pConnAdr[i0])->pConnAdr[i1]))->pConnAdr[i2];
			if (ptr == 0) {///< create new level3 list
				if ((Func == AY_CONN_FIND) || (Func == AY_CONN_DELETE)) {
					printf("AYM2M--> Conn Level 3 not found i=%d \n", i);
					M2M_ConnectionCount = i;
					return nullptr;///< fail !!!
				}
				else {
					pConnList = (AY_CONNADR	*)_AY_MallocMemory(sizeof(AY_CONNADR));
					memset(pConnList, 0, sizeof(AY_CONNADR));
					((AY_CONNADR	*)(((AY_CONNADR	*)M2M_ConnLevel1.pConnAdr[i0])->pConnAdr[i1]))->pConnAdr[i2] = (void *)pConnList;
					ptr = ((AY_CONNADR	*)(((AY_CONNADR	*)M2M_ConnLevel1.pConnAdr[i0])->pConnAdr[i1]))->pConnAdr[i2];
					printf("AYM2M--> New Conn Level 3 created i2=%d \n", i2);
				}
			}
			pConnTypM2M = (AY_M2M_CONNTYPE *)(((AY_CONNADR	*)ptr)->pConnAdr[0]);///< level4 start index
		}
		if (pConnTypM2M == 0) {///< free socket found
			if (FirstFound != 0xAA) {
				FirstFound = 0xAA;
				FirstFree = i;
				printf("AYM2M--> First free Conn found FirstFree=%d \n", FirstFree);
			}
		}
		else {
			if ((pTempM2M->_Src._pConn == pConnTypM2M->_Src._pConn) && (pConnTypM2M->_Src._DevNo == pTempM2M->_Src._DevNo)){
				pConnTyp = pTempM2M->_Src._pConn;// pAYCONN_ReadConn(pConnTypM2M->_Src._ConnNo);
				if ((pConnTyp->_UnqiueId[0] == pConnTypM2M->_Src._pConn->_UnqiueId[0]) && (pConnTyp->_UnqiueId[1] == pConnTypM2M->_Src._pConn->_UnqiueId[1]) && (pConnTyp->_UnqiueId[2] == pConnTypM2M->_Src._pConn->_UnqiueId[2])) {///< Connection found
					printf("AYM2M--> Conn found FirstFree=%d \n", i);
					if (Func == AY_CONN_UPDATE) {
						if ((pConnTypM2M->_Dst._pConn != nullptr)/*&&(pConnTypM2M->_Dst._pConn->)*/) {
							pConnTypM2M->M_LastUpdateMin = AYM2M_ThisMinute();
							if (pId != nullptr) { *pId = i; }
							return pConnTypM2M;
						}
					}
					else if (Func == AY_CONN_DELETE) {
						_AY_FreeMemory((unsigned char *)pConnTypM2M);
						((AY_CONNADR	*)((AY_CONNADR	*)(((AY_CONNADR	*)M2M_ConnLevel1.pConnAdr[i0])->pConnAdr[i1]))->pConnAdr[i2])->pConnAdr[i3] = 0;///< nullptr
					}
					return nullptr;
				}
			}
		}
		if (i == M2M_ConnectionCount) {
			printf("AYM2M--> Conn not found use FirstFree=%d \n", FirstFree);
			i = FirstFree;
			ptr = ((AY_CONNADR	*)(((AY_CONNADR	*)M2M_ConnLevel1.pConnAdr[i0])->pConnAdr[i1]))->pConnAdr[i2];
			if (Func == AY_CONN_UPDATE) {///< upload
				((AY_CONNADR	*)ptr)->pConnAdr[i3] = _AY_MallocMemory(sizeof(AY_M2M_CONNTYPE));
				pConnTypM2M = (AY_M2M_CONNTYPE *)(((AY_CONNADR	*)ptr)->pConnAdr[i3]);///< level4 start index
				memset(pConnTypM2M, 0, sizeof(AY_M2M_CONNTYPE));
				memcpy(pConnTypM2M, pTempM2M, sizeof(AY_M2M_CONNTYPE));
				pConnTypM2M->M_LastUpdateMin = AYM2M_ThisMinute();

				if (MYSQL_FindLoadDeviceInfoID(pConnTypM2M->_Src._DevNo, (Ui32 *)&pConnTypM2M->_Src._pConn->_UnqiueId[0]) >= 0) {///< SRC device is active
					if (MYSQL_FindLoadDeviceInfoID(MYSQL_Device._ParentId, (Ui32 *)&MYSQL_Device._Unique[0]) >= 0) {///< DST device is active
						pConnTypM2M->_Dst._pConn = pFindConnByUniqueID((UNIQUE_ID *)&MYSQL_Device._Unique[0]);
						pConnTypM2M->_Dst._DevNo = MYSQL_Device._ParentId;
						pConnTypM2M->_Dst._DstIP = MYSQL_Device._LocalIp;
						//...
						if (pId != nullptr) { *pId = i; }
						return pConnTypM2M;
					}
				}
				//============ TEST =========================//
				printf("AYM2M--> Input Conn i0=0x%08x, i1=0x%08x, i2=0x%08x, i3=0x%08x \n", (Ui32)M2M_ConnLevel1.pConnAdr[i0], (Ui32)(((AY_CONNADR	*)M2M_ConnLevel1.pConnAdr[i0])->pConnAdr[i1]), (Ui32)(((AY_CONNADR	*)(((AY_CONNADR	*)M2M_ConnLevel1.pConnAdr[i0])->pConnAdr[i1]))->pConnAdr[i2]), (Ui32)pConnTypM2M);
				pConnTst = pAYM2M_ReadConn(i);
				//============ TEST =========================//
				M2M_ConnectionCount++;
				printf("AYM2M--> Conn new added i3=%d M2M_ConnectionCount=%d Adr=0x%08x \n", i3, M2M_ConnectionCount, (Ui32)pConnTypM2M);
			}
			return pConnTypM2M;
		}
		pConnTypM2M = (AY_M2M_CONNTYPE *)(((AY_CONNADR	*)ptr)->pConnAdr[i3]);///< level4 start index//pConnTypM2M++;
	} while ((++i) <= M2M_ConnectionCount);
	printf("AYM2M--> Conn There is something wrong ! \n");
	return nullptr;///< fail !!!
}

Ui32 AYM2M_ReadConn(Ui32 ConnId, AY_M2M_CONNTYPE *pConnRd) {
	void *ptr;
	AY_M2M_CONNTYPE	*pConnTypM2M;
	Ui32 i;

#define i0		((i>>24)&0xFF)
#define i1		((i>>16)&0xFF)
#define i2		((i>>8)&0xFF)
#define i3		((i>>0)&0xFF)
	i = ConnId;
	ptr = M2M_ConnLevel1.pConnAdr[i0];
	if (ptr != 0) {
		printf("AYM2M--> Conn Test Level 1 i0=%d Adr=0x%08x \n", i0, (Ui32)ptr);
		ptr = ((AY_CONNADR	*)ptr)->pConnAdr[i1];
		if (ptr != 0) {
			printf("AYM2M--> Conn Test Level 2 i1=%d Adr=0x%08x \n", i1, (Ui32)ptr);
			ptr = ((AY_CONNADR	*)ptr)->pConnAdr[i2];
			if (ptr != 0) {///< valid address
				printf("AYM2M--> Conn Test Level 3 i2=%d Adr=0x%08x \n", i2, (Ui32)ptr);
				pConnTypM2M = (AY_M2M_CONNTYPE *)((AY_CONNADR	*)ptr)->pConnAdr[i3];
				if (pConnTypM2M != 0) {///< valid connecton
					printf("AYM2M--> Conn Test Level 4 i3=%d Adr=0x%08x \n", i3, (Ui32)pConnTypM2M);
					memcpy(pConnRd, pConnTypM2M, sizeof(AY_M2M_CONNTYPE));
					return i;
				}
			}
		}
	}
	printf("AYM2M--> Conn There is something wrong ! \n");
	return 0xFFFFFFFF;
}

AY_M2M_CONNTYPE	*pAYM2M_ReadConn(Ui32 ConnId) {
	void *ptr;
	AY_M2M_CONNTYPE	*pConnTypM2M;
	Ui32 i;

#define i0		((i>>24)&0xFF)
#define i1		((i>>16)&0xFF)
#define i2		((i>>8)&0xFF)
#define i3		((i>>0)&0xFF)
	i = ConnId;
	ptr = M2M_ConnLevel1.pConnAdr[i0];
	if ((ptr != 0) || (ptr != nullptr)) {
		printf("AYM2M--> Conn Test Level 1 i0=%d Adr=0x%08x \n", i0, (Ui32)ptr);
		ptr = ((AY_CONNADR	*)ptr)->pConnAdr[i1];
		if ((ptr != 0) || (ptr != nullptr)) {
			printf("AYM2M--> Conn Test Level 2 i1=%d Adr=0x%08x \n", i1, (Ui32)ptr);
			ptr = ((AY_CONNADR	*)ptr)->pConnAdr[i2];
			if ((ptr != 0) || (ptr != nullptr)) {///< valid address
				printf("AYM2M--> Conn Test Level 3 i2=%d Adr=0x%08x \n", i2, (Ui32)ptr);
				pConnTypM2M = (AY_M2M_CONNTYPE *)((AY_CONNADR	*)ptr)->pConnAdr[i3];
				if ((pConnTypM2M != 0) || (pConnTypM2M != nullptr)) {///< valid connecton
					printf("AYM2M--> Conn found - Conn Test Level 4 i=%d Adr=0x%08x \n", i, (Ui32)pConnTypM2M);
					//memcpy(pConnRd, pConnTypM2M, sizeof(AY_CONNTYPE));
					return pConnTypM2M;
				}
			}
		}
	}
	printf("AYM2M--> Conn Not found - Conn There is something wrong ! i=%d \n", i);
	return nullptr;
}

//AY_M2M_CONNTYPE	*pFindConnByTCPheader(tcp_headerAll *pTCP) {
//	AY_M2M_CONNTYPE	*pConnTypM2M = nullptr;
//	Ui32 i;
//
//	for (i = 0; i < M2M_ConnectionCount; i++) {
//		pConnTypM2M = pAYM2M_ReadConn(i);
//		if (pConnTypM2M != nullptr) {
//			if ((pConnTypM2M->_TCPh._ipHeader.saddr.longip == pTCP->_ipHeader.saddr.longip) && (pConnTypM2M->_TCPh._tcpHeader.sport == pTCP->_tcpHeader.sport)) {
//				printf("AYM2M--> Conn found - UDP header matched i=%d \n", i);
//				return pConnTypM2M;
//			}
//		}
//	}
//	printf("AYM2M--> Conn not found - UDP header not matched i=%d \n", i);
//	return nullptr;
//}

//AY_M2M_CONNTYPE	*pAYM2M_FindConnByUniqueID(UNIQUE_ID *pUnique) {
//	AY_M2M_CONNTYPE	*pConnTypM2M = nullptr;
//	Ui32 i;
//
//	for (i = 0; i < M2M_ConnectionCount; i++) {
//		pConnTypM2M = pAYM2M_ReadConn(i);
//		if (pConnTypM2M != nullptr) {
//			printf("\n\t\tLook = %d\n Uniq0: 0x%08x , Uniq1: 0x%08x , Uniq2: 0x%08x \n", i, pUnique->_UniqueL[0], pUnique->_UniqueL[1], pUnique->_UniqueL[2]);
//			printf("\n\t\tConnectionNo = %d\n Uniq0: 0x%08x , Uniq1: 0x%08x , Uniq2: 0x%08x \n", i, pConnTypM2M->_UnqiueId[0], pConnTypM2M->_UnqiueId[1], pConnTypM2M->_UnqiueId[2]);
//			if ((pConnTypM2M->_UnqiueId[0] == pUnique->_UniqueL[0]) && (pConnTypM2M->_UnqiueId[1] == pUnique->_UniqueL[1]) && (pConnTypM2M->_UnqiueId[2] == pUnique->_UniqueL[2])) {///< device found 
//				printf("AYM2M--> Conn found - Unique ID matched i=%d \n", i);
//				return pConnTypM2M;
//			}
//		}
//	}
//	printf("AYM2M--> Conn not found - Unique ID not matched i=%d \n", i);
//	return nullptr;
//}










