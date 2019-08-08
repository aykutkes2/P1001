//#include "pch.h"
#undef UNICODE

#include <stdio.h>
#include <time.h>
//#include <windows.h>

#include <AY_Memory.h>
#include <AY_ClientDev.h>
			
AY_DEVINFOLST	DevLevel1;
AY_DEVINFOLST	DevRemote;
Ui16			DevRemoteTOut[4096];
AY_DEVINFOLST	*pDevInfos[((MAX_DEVINFO_CNT/4096)-1)];///< total 4096 * (15+1) = 65536 devices supported

/*
*
*/
int AYCLNT_RemoteDevTimeoutTest(void) {
	int i;
	for (i = 0; i < 4096; i++) {
		if (DevRemoteTOut[i]) {
			DevRemoteTOut[i] --;
			if (DevRemoteTOut[i] == 0) {
				memset(&DevRemote.Info[i], 0, sizeof(AY_DEVINFO));
				DevRemote.Info[i].DevF.Full_ = 0;
			}
		}
	}
	return 1;
}

/*
*
*/
int AYCLNT_UpdateDevInfo(AY_DEVINFO	*pDeInf, Ui08 *pComp, Ui08 Comp) {
	switch (Comp) {
	case _DEVINFOALL:
		*pDeInf = *((AY_DEVINFO	*)pComp);
		pDeInf->DevF.Full_ = 1;
		printf("AYDEV--> _DEVINFOALL \n");
		break;
	case _DEV_FLG:
		pDeInf->DevF = *((AY_DEVFLAGs	*)pComp);
		pDeInf->DevF.Full_ = 1;
		printf("AYDEV--> _DEV_FLG \n");
		break;
	case _DEV_UDPH:
		pDeInf->UDP_Hdr = *((udp_headerAll	*)pComp);
		pDeInf->DevF.Full_ = 1;
		printf("AYDEV--> _DEV_UDPH \n");
		break;
	case _DEV_SSK:
		memcpy(&pDeInf->Sessionkey[0], ((Ui08	*)pComp), 16);
		pDeInf->DevF.Full_ = 1;
		printf("AYDEV--> _DEV_SSK \n");
		break;
	case _DEV_READ:
		pDeInf->DevRead = *((AY_DeviceRead	*)pComp);
		pDeInf->DevF.Full_ = 1;
		printf("AYDEV--> _DEV_READ \n");
		break;
	case _DEV_READ_ALL:
		memset(pDeInf, 0, sizeof(AY_DEVINFO));
		pDeInf->DevRead = *((AY_DeviceRead	*)pComp);
		pDeInf->DevF.Full_ = 1;
		printf("AYDEV--> _DEV_READ_ALL \n");
		break;
	case _DEV_SNDCNT:
		pDeInf->SendCnt = *((Ui32	*)pComp);
		pDeInf->DevF.Full_ = 1;
		printf("AYDEV--> _DEV_SNDCNT \n");
		break;
	case _DEV_RDCNT:
		pDeInf->ReadCnt = *((Ui32	*)pComp);
		pDeInf->DevF.Full_ = 1;
		printf("AYDEV--> _DEV_RDCNT \n");
		break;
	case _DEV_ERRCNT:
		pDeInf->ErrCnt = *((Ui32	*)pComp);
		pDeInf->DevF.Full_ = 1;
		printf("AYDEV--> _DEV_ERRCNT \n");
		break;
	case _DEV_DELCNTS:
		pDeInf->SendCnt = 0;
		pDeInf->ReadCnt = 0;
		pDeInf->ErrCnt = 0;
		pDeInf->DevF.Full_ = 1;
		printf("AYDEV--> _DEV_DELCNTS \n");
		break;
	case _DEV_DELETE:
		memset(pDeInf, 0, sizeof(AY_DEVINFO));
		break;
	default:
		printf("AYDEV--> Undefined Object \n");
		break;
	};
	return 1;
}

/*
*
*/
int AYCLNT_AddDevToList(Ui08 *pComp, Ui32 DevNo, Ui08 Comp) {
	Ui32 i;
	Ui16 j,k;
	AY_DEVINFO	*pDeInf;
	//=======================================================================================================//
	if ((DevNo & 0xFFFFFF) >= (MAX_DEVINFO_CNT)) {
		return -1;
	}
	else if ((DevNo & 0xFF000000)&& ((DevNo& 0xFFFFFF) < 0x001001)) {
		k = 0;
		DevNo &= 0xFFFFFF;
		if (DevNo == 4096) {///< find empty slot
			pDeInf = &DevRemote.Info[0];
			for (i = 0; i < 4096; i++) {
				if (!pDeInf->DevF.Full_) {
					k = i;
					break;
				}
			}
			if (i == 4096) {///< all full, find oldest and use it
				j = 0xFFFF;
				for (i = 0; i < 4096; i++) {
					if (DevRemoteTOut[i] < j) {
						j = DevRemoteTOut[i];
						k = i;
					}
				}
			}
			pDeInf = &DevRemote.Info[k];
			pDeInf->DevF.Full_ = 1;
			memset(pDeInf, 0, sizeof(AY_DEVINFO));
			DevRemoteTOut[k] = DEV_REMOTE_TIMEOUT;
			printf("AYDEV--> New Device Remote Added DevNo=%d \n", (k+1));
		}
		else {
			k = DevNo;
			pDeInf = &DevRemote.Info[k];
			DevRemoteTOut[k] = DEV_REMOTE_TIMEOUT;
			printf("AYDEV--> Device Remote Found DevNo=%d \n", DevNo);
		}
		//i = k;
	}
	else if (DevNo < 4096) {
		pDeInf = &DevLevel1.Info[DevNo];
		//i = DevNo;
		printf("AYDEV--> New Device Level 1 created DevNo=%d \n", DevNo);
	}
	else {
		i = DevNo - 4096;
		pDeInf = &pDevInfos[(i >> 12)]->Info[0];
		i = (i & 0xFFF);
		if (pDeInf == nullptr) {
			pDeInf = (AY_DEVINFO	*)_AY_MallocMemory(sizeof(DevLevel1));
			printf("AYDEV--> New Device List %d created DevNo=%d \n", ((DevNo >> 12) + 2), DevNo);
			if (pDeInf != nullptr) {
				memset(pDeInf, 0, sizeof(DevLevel1));
			}
			else {
				printf("AYDEV--> RAM NOT ENOUGH FREE SPACE !!!! \n");
				printf("AYDEV--> RAM NOT ENOUGH FREE SPACE !!!! \n");
				printf("AYDEV--> RAM NOT ENOUGH FREE SPACE !!!! \n");
				return -1;
			}
		}
		pDeInf = &pDevInfos[(i >> 12)]->Info[i];
	}
	//=======================================================================================================//
	if (Comp < _DEV_LASTCOMP) {
		AYCLNT_UpdateDevInfo(pDeInf, pComp, Comp);
	}
	//=======================================================================================================//
	return 1;
}
/*
*
*/
AY_DEVINFO *pAY_FindDevInfoByDevNo(Ui32 DevNo) {
	Ui32 i;
	AY_DEVINFO	*pDeInf;
	if ((DevNo & 0xFFFFFF) >= (MAX_DEVINFO_CNT)) {
		return 0;
	}
	else if ((DevNo & 0xFF000000) && ((DevNo & 0xFFFFFF) < 0x001001)) {
		DevNo &= 0xFFFFFF;
		pDeInf = &DevRemote.Info[DevNo];
	}
	else if (DevNo < 4096 ) {
		pDeInf = &DevLevel1.Info[DevNo];
	}
	else {
		i = DevNo - 4096;
		pDeInf = &pDevInfos[(i >> 12)]->Info[(i & 0xFFF)];
	}
	return pDeInf;
}

/*
*
*/
AY_DEVINFO *pAY_FindLocDevInfoByIP(Ui32 LocIP) {
	Ui32 i;
	AY_DEVINFO	*pDeInf;
	for (i = 0; i < (MAX_DEVINFO_CNT); i++) {
		pDeInf = pAY_FindDevInfoByDevNo(i);
		if (pDeInf) {
			if (pDeInf->DevF.Full_) {
				if (pDeInf->DevRead._LocalIp == LocIP) {
					return pDeInf;
				}
			}
		}
	}
	return 0;///< not found
}

/*
*
*/
AY_DEVINFO *pAY_FindRmtDevInfoByMAC(Ui08 *pMac, Ui08 SrcDst) {
	Ui32 i;
	AY_DEVINFO	*pDeInf;
	Ui08 *pM;
	for (i = 0; i < 256; i++) {
		pDeInf = pAY_FindDevInfoByDevNo(i+0xFF000000);
		if (pDeInf) {
			if (pDeInf->DevF.Full_) {
				if (SrcDst == 0) {///< SRC
					pM = (Ui08 *)&pDeInf->UDP_Hdr._ethHeader.src;
				}
				else {///< DST
					pM = (Ui08 *)&pDeInf->UDP_Hdr._ethHeader.src;
				}
				if( memcmp(pM, pMac,sizeof(uip_eth_addr)) == 0){
					return pDeInf;
				}
			}
		}
	}
	return 0;///< not found
}





//============================ GW LISTS ================================================================//
//============================ GW LISTS ================================================================//
//============================ GW LISTS ================================================================//
//============================ GW LISTS ================================================================//
AY_GWINFOLST			AYCLNT_GWLISTE_L1;
AY_GWINFOLST			*pGwInfos[((MAX_CLNTGWLIST_CNT / 4096) - 1)];///< total 4096 * (15+1) = 65536 devices supported
Ui32					AYCLNT_GW_Cnt = 0;

AY_GWINFO	*pAYCLNT_FindGwById(int Id) {
	Ui32 i,j;
	if (Id >= 0) {
		if (Id < 4096) {
			return (&AYCLNT_GWLISTE_L1.Info[Id]);
		}
		else {
			Id -= 4096;
			j = Id >> 12;
			i = Id & 0xFFF;
			if (pGwInfos[j] != nullptr) {
				return (&pGwInfos[j]->Info[i]);
			}
			else {
				return nullptr;
			}
		}
	}
	return nullptr;
}

AY_GWINFO	*pAYCLNT_FindFirstFreeGwId(int *pId) {
	AY_GWINFO	*pGw = nullptr;
	Ui32 i, j=0;

	for (i = 0; i < MAX_CLNTGWLIST_CNT; i++) {
		if ((i & 0xFFF) == 0) {
			if (i == 0) {
				pGw = &AYCLNT_GWLISTE_L1.Info[0];
			}
			else {
				j = (i-4096) >> 12;
				pGw = (AY_GWINFO	*)pGwInfos[j];
				if (pGw == nullptr) {
					pGwInfos[j] = (AY_GWINFOLST	*)_AY_MallocMemory(sizeof(AY_GWINFOLST));
					memset(pGwInfos[j], 0, sizeof(AY_GWINFOLST));
				}
				pGw = &pGwInfos[j]->Info[0];
			}
		}
		if (pGw->GwF.Full_ == 0) {
			memset(pGw, 0, sizeof(AY_GWINFO));
			*pId = i;
			return pGw;
		}
		pGw++;
	}
	return nullptr;
}

int AYCLNT_CalcGwCnt(int *pCnt) {
	AY_GWINFO	*pGw = nullptr;
	Ui32 i, j=0;
	int tot = 0;
	int lst = -1;

	tot = 0;
	for (i = 0; i < MAX_CLNTGWLIST_CNT; i++) {
		if ((i & 0xFFF) == 0) {
			if (i == 0) {
				pGw = &AYCLNT_GWLISTE_L1.Info[0];
			}
			else {
				j = (i-4096) >> 12;
				pGw = (AY_GWINFO	*)pGwInfos[j];
				if (pGw == nullptr) {
					if (pCnt != 0) { *pCnt = tot; }
					return(lst+1);
				}
				pGw = &pGwInfos[j]->Info[0];
			}
		}
		if (pGw->GwF.Full_ == 1) {
			lst = i;
		}
		tot++;
		pGw++;
	}
	if (pCnt != 0) { *pCnt = tot; }
	return(lst+1);
}

int AYCLNT_FindGwId(AY_GWINFO	*pGw) {
	Ui32 i, j;
	Ui08 *p;
	if ( (pGw >= &AYCLNT_GWLISTE_L1.Info[0]) && (pGw <= &AYCLNT_GWLISTE_L1.Info[4095]) ){
		p = (Ui08 *)pGw;
		i = ( (p - ((Ui08 *)&AYCLNT_GWLISTE_L1.Info[0])) / sizeof(AY_GWINFO) );
		return (i);
	}
	else {
		for (j = 0; j < cntof(pGwInfos); j++) {
			if (pGwInfos[j] != nullptr) {
				if ((pGw >= &pGwInfos[j]->Info[0]) && (pGw <= &pGwInfos[j]->Info[4095])) {
					p = (Ui08 *)pGw;
					i = ((p - ((Ui08 *)&pGwInfos[j]->Info[0])) / sizeof(AY_GWINFO));
					return (i+((j+1)*4096));
				}
			}
		}
	}
	return -1;
}

AY_GWINFO	*pAYCLNT_FindGwByUnique(Ui32 *pUnique, int *pId) {
	AY_GWINFO	*pGw = nullptr;
	Ui32 i, j;
	Ui08 *p;

	*pId = -1;
	AYCLNT_GW_Cnt = AYCLNT_CalcGwCnt(0);
	for (i = 0; i < AYCLNT_GW_Cnt; i++) {
		pGw = pAYCLNT_FindGwById(i);
		if (pGw != nullptr) {
			if (pGw->GwF.Full_) {
				if ((pGw->_Unique[0] == pUnique[0]) && (pGw->_Unique[1] == pUnique[1]) && (pGw->_Unique[2] == pUnique[2])) {
					*pId = i;
					return pGw;
				}
			}
		}
	}
	return pGw;
}

int AYCLNT_TestAddOrUpdateGw(AY_GWINFO	*pGw, int *pId) {
	AY_GWINFO	*pGw0 = nullptr;
	int i=0;

	pGw0 = pAYCLNT_FindGwByUnique(&pGw->_Unique[0], pId);
	if (pGw0 != nullptr) {///< update
		i = 1;
	}
	else {
		pGw0 = pAYCLNT_FindFirstFreeGwId(pId);
		i = 2;
	}
	*pGw0 = *pGw;

	return i;
}

/*
*
*/
int AYCLNT_UpdateGwInfo(AY_GWINFO	*pGw, Ui08 *pComp, Ui08 Comp) {
	switch (Comp) {
	case _GWINFOALL:
		*pGw = *((AY_GWINFO	*)pComp);
		pGw->GwF.Full_ = 1;
		printf("AYGW--> _GWINFOALL \n");
		break;
	case _GW_FLG:
		pGw->GwF = *((AY_GWFLAGs	*)pComp);
		pGw->GwF.Full_ = 1;
		printf("AYGW--> _GW_FLG \n");
		break;
	case _GW_UDPH:
		pGw->UDP_Hdr = *((udp_headerAll	*)pComp);
		pGw->GwF.Full_ = 1;
		printf("AYGW--> _GW_UDPH \n");
		break;
	case _GW_SSK:
		memcpy(&pGw->Sessionkey[0], ((Ui08	*)pComp), 16);
		pGw->GwF.Full_ = 1;
		printf("AYGW--> _GW_SSK \n");
		break;
	case _GW_UNQUE_ALL:
		memset(pGw, 0, sizeof(AY_GWINFO));
	case _GW_UNQUE:
		memcpy(&pGw->_Unique[0], pComp, 12);
		pGw->GwF.Full_ = 1;
		printf("AYGW--> _GW_UNQUE \n");
		break;
	case _GW_SNDCNT:
		pGw->SendCnt = *((Ui32	*)pComp);
		pGw->GwF.Full_ = 1;
		printf("AYGW--> _GW_SNDCNT \n");
		break;
	case _GW_RDCNT:
		pGw->ReadCnt = *((Ui32	*)pComp);
		pGw->GwF.Full_ = 1;
		printf("AYGW--> _GW_RDCNT \n");
		break;
	case _GW_ERRCNT:
		pGw->ErrCnt = *((Ui32	*)pComp);
		pGw->GwF.Full_ = 1;
		printf("AYGW--> _GW_ERRCNT \n");
		break;
	case _GW_DELCNTS:
		pGw->SendCnt = 0;
		pGw->ReadCnt = 0;
		pGw->ErrCnt = 0;
		pGw->GwF.Full_ = 1;
		printf("AYGW--> _GW_DELCNTS \n");
		break;
	case _GW_DELETE:
		memset(pGw, 0, sizeof(AY_GWINFO));
		break;
	default:
		printf("AYGW--> Undefined Object \n");
		break;
	};
	return 1;
}


/*
*
*/
AY_GWINFO	*pAYCLNT_AddGwToList(Ui08 *pComp, Ui32 *pUnique, Ui08 Comp) {
	AY_GWINFO	*pGw0 = nullptr;
	int Id = 0;
	int i = 0;

	pGw0 = pAYCLNT_FindGwByUnique(&pUnique[0], &Id);
	if (pGw0 != nullptr) {///< update
		i = 1;
	}
	else {
		pGw0 = pAYCLNT_FindFirstFreeGwId(&Id);
		i = 2;
	}
	AYCLNT_UpdateGwInfo(pGw0, pComp, Comp);

	return pGw0;
}


/*
*
*/
int AYCLNT_GwReleaseSlot(AY_GWINFO	*pGw) {
	if (pGw != nullptr) {
		if (pGw->GwF.Full_) {
			memset(pGw, 0, sizeof(AY_GWINFO));
			pGw->GwF.Full_ = 0;///< release slot
		}
	}
	return 1;
}

/*
*
*/
int AYCLNT_GwTimeoutTest(void) {
	AY_GWINFO	*pGw;
	int i;
	AYCLNT_GW_Cnt = AYCLNT_CalcGwCnt(0);
	for (i = 0; i < AYCLNT_GW_Cnt; i++) {
		pGw = pAYCLNT_FindGwById(i);
		if (pGw != nullptr) {
			if (pGw->GwF.Full_) {
				if (pGw->TimeOut) {
					pGw->TimeOut--;
					if (pGw->TimeOut == 0) {
						memset(pGw, 0, sizeof(AY_GWINFO));
						pGw->GwF.Full_ = 0;///< release slot
					}
				}

			}
		}
	}
	return 1;
}




//============================ QUEUE ================================================================//
//============================ QUEUE ================================================================//
//============================ QUEUE ================================================================//
//============================ QUEUE ================================================================//
AY_GWQUEUELST			AYCLNT_QUEUELIST_L1;
AY_GWQUEUELST			*pQueueInfos[((MAX_CLNTQUEUE_CNT / 4096) - 1)];///< total 4096 * (15+1) = 65536 devices supported
Ui32					AYCLNT_QUEUE_Cnt = 0;
 


AY_CLNTQUEUE	*pAYCLNT_FindFirstFreeQueueId(int *pId) {
	AY_CLNTQUEUE	*pQue = nullptr;
	Ui32 i, j = 0;

	for (i = 0; i < MAX_CLNTQUEUE_CNT; i++) {
		if ((i & 0xFFF) == 0) {
			if (i == 0) {
				pQue = &AYCLNT_QUEUELIST_L1.Info[0];
			}
			else {
				j = (i - 4096) >> 12;
				pQue = (AY_CLNTQUEUE	*)pQueueInfos[j];
				if (pQue == nullptr) {
					pQueueInfos[j] = (AY_GWQUEUELST	*)_AY_MallocMemory(sizeof(AY_GWQUEUELST));
					memset(pQueueInfos[j], 0, sizeof(AY_GWQUEUELST));
				}
				pQue = &pQueueInfos[j]->Info[0];
			}
		}
		if (pQue->QueF.Full_ == 0) {
			memset(pQue, 0, sizeof(AY_CLNTQUEUE));
			*pId = i;
			return pQue;
		}
		pQue++;
	}
	return nullptr;
}

int AYCLNT_CalcQueueCnt(int *pCnt) {
	AY_CLNTQUEUE	*pQue = nullptr;
	Ui32 i, j = 0;
	int tot = 0;
	int lst = -1;

	tot = 0;
	for (i = 0; i < MAX_CLNTQUEUE_CNT; i++) {
		if ((i & 0xFFF) == 0) {
			if (i == 0) {
				pQue = &AYCLNT_QUEUELIST_L1.Info[0];
			}
			else {
				j = (i - 4096) >> 12;
				pQue = (AY_CLNTQUEUE	*)pQueueInfos[j];
				if (pQue == nullptr) {
					if (pCnt != 0) { *pCnt = tot; }
					return(lst + 1);
				}
				pQue = &pQueueInfos[j]->Info[0];
			}
		}
		if (pQue->QueF.Full_ == 1) {
			lst = i;
		}
		tot++;
		pQue++;
	}
	if (pCnt != 0) { *pCnt = tot; }
	return(lst + 1);
}




/*
*
*/
int AYCLNT_QueueReleaseSlot(AY_CLNTQUEUE	*pQue) {
	if (pQue != nullptr) {
		if (pQue->QueF.Full_) {
			if ((pQue->pDataIO != nullptr) && (pQue->DataIOLen)) {
				free(pQue->pDataIO);
			}
			memset(pQue, 0, sizeof(AY_CLNTQUEUE));
			pQue->QueF.Full_ = 0;///< release slot
		}
	}
	return 1;
}

/*
*
*/
//int AYCLNT_QueueTimeoutTest(void) {
//	AY_CLNTQUEUE	*pQue;
//	int i;
//	AYCLNT_QUEUE_Cnt = AYCLNT_CalcQueueCnt(0);
//	for (i = 0; i < AYCLNT_QUEUE_Cnt; i++) {
//		pQue = pAYCLNT_FindGwById(i);
//		if (pQue != nullptr) {
//			if (pQue->QueF.Full_) {
//				if (pQue->TimeOut) {
//					pQue->TimeOut--;
//					if (pQue->TimeOut == 0) {
//						memset(pQue, 0, sizeof(AY_GWINFO));
//						pQue->QueF.Full_ = 0;///< release slot
//					}
//				}
//
//			}
//		}
//	}
//	return 1;
//}
