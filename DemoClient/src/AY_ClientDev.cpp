//#include "pch.h"
#undef UNICODE

#include <stdio.h>
#include <time.h>
//#include <windows.h>

#include <AY_Memory.h>
#include <AY_Client.h>
#include <AY_ClientDev.h>


static const Ui08 DefaultMac0[6] = { 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF, };

#define RMTDEV_MAX_CNT			4096
AY_DEVINFOLST	DevLevel1;
AY_DEVINFOLST	DevRemote;
Ui16			DevRemoteTOut[RMTDEV_MAX_CNT];
AY_DEVINFOLST	*pDevInfos[((MAX_DEVINFO_CNT/4096)-1)];///< total 4096 * (15+1) = 65536 devices supported

/*
*
*/
Ui08	RmtDev_TimeOut_Task = 0;
#define RMTDEV_MAX_TASK				((RMTDEV_MAX_CNT+31)/32)	
#define RMTDEV_TASK_CNT				32
int AYCLNT_RemoteDevTimeoutTest(void) {
	int i, j;
	j = ((int)RmtDev_TimeOut_Task* RMTDEV_TASK_CNT);
	for (i = j; i < (j + RMTDEV_TASK_CNT); i++) {
		if (DevRemoteTOut[i]) {
			DevRemoteTOut[i] --;
			if (DevRemoteTOut[i] == 0) {
				memset(&DevRemote.Info[i], 0, sizeof(AY_DEVINFO));
				DevRemote.Info[i].DevF.Full_ = 0;
			}
		}
	}
	RmtDev_TimeOut_Task++;
	if (RmtDev_TimeOut_Task >= RMTDEV_MAX_TASK) {
		RmtDev_TimeOut_Task = 0;
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
	case _DEV_UNIQUE_ALL:
		memset(pDeInf, 0, sizeof(AY_DEVINFO));
		memcpy(&pDeInf->DevRead._Unique ,pComp, 12);
		pDeInf->DevF.Full_ = 1;
		printf("AYDEV--> _DEV_UNIQUE_ALL \n");
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
AY_DEVINFO	*pAYCLNT_AddDevToList(Ui08 *pComp, Ui32 DevNo, Ui08 Comp) {
	Ui32 i;
	Ui16 j,k;
	AY_DEVINFO	*pDeInf;
	//=======================================================================================================//
	if ((DevNo & 0xFFFFFF) >= (MAX_DEVINFO_CNT)) {
		return nullptr;
	}
	else if ((DevNo & 0xFF000000)&& ((DevNo& 0xFFFFFF) < 0x001001)) {
		k = 0;
		DevNo &= 0xFFFFFF;
		if (DevNo == RMTDEV_MAX_CNT) {///< find empty slot
			pDeInf = &DevRemote.Info[0];
			for (i = 0; i < RMTDEV_MAX_CNT; i++) {
				if (!pDeInf->DevF.Full_) {
					k = i;
					break;
				}
			}
			if (i == RMTDEV_MAX_CNT) {///< all full, find oldest and use it
				j = 0xFFFF;
				for (i = 0; i < RMTDEV_MAX_CNT; i++) {
					if (DevRemoteTOut[i] < j) {
						j = DevRemoteTOut[i];
						k = i;
					}
				}
			}
			pDeInf = &DevRemote.Info[k];
			memset(pDeInf, 0, sizeof(AY_DEVINFO));
			pDeInf->DevF.Full_ = 1;
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
				return nullptr;
			}
		}
		pDeInf = &pDevInfos[(i >> 12)]->Info[i];
	}
	//=======================================================================================================//
	if (Comp < _DEV_LASTCOMP) {
		AYCLNT_UpdateDevInfo(pDeInf, pComp, Comp);
	}
	//=======================================================================================================//
	return pDeInf;
}
/*
*
*/
AY_DEVINFO *pAY_FindDevInfoByDevNo(Ui32 DevNo) {
	Ui32 i;
	AY_DEVINFO	*pDeInf=nullptr;
	if ((DevNo & 0xFFFFFF) >= (MAX_DEVINFO_CNT)) {
		return 0;
	}
	else if ((DevNo & 0xFF000000) && ((DevNo & 0xFFFFFF) < (RMTDEV_MAX_CNT))) {
		DevNo &= 0xFFFFFF;
		pDeInf = &DevRemote.Info[DevNo];
	}
	else if (DevNo < 4096 ) {
		pDeInf = &DevLevel1.Info[DevNo];
	}
	else {
		i = DevNo - 4096;
		if(pDevInfos[(i >> 12)] != nullptr){
			pDeInf = &pDevInfos[(i >> 12)]->Info[(i & 0xFFF)];
		}
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
					//if (pDeInf->DevRead._Type == _MIRROR_) {
						return pDeInf;
					//}
				}
			}
		}
	}
	return nullptr;///< not found
}

/*
*
*/
AY_DEVINFO *pAY_FindLocDevInfoByDevRowNo (Ui32 RowNo) {
	Ui32 i;
	AY_DEVINFO	*pDeInf;
	for (i = 0; i < (MAX_DEVINFO_CNT); i++) {
		pDeInf = pAY_FindDevInfoByDevNo(i);
		if (pDeInf) {
			if (pDeInf->DevF.Full_) {
				if (pDeInf->DevRead._id == RowNo) {
					return pDeInf;
				}
			}
		}
	}
	return nullptr;///< not found
}

/*
*
*/
AY_DEVINFO *pAY_FindRmtDevInfoByAll(AY_DEVINFO *pInfo) {
	Ui32 i;
	AY_DEVINFO	*pDeInf;
	for (i = 0; i < RMTDEV_MAX_CNT; i++) {
		pDeInf = pAY_FindDevInfoByDevNo(i + 0xFF000000);
		if (pDeInf) {
			if (pDeInf->DevF.Full_) {
				if (memcmp(pDeInf, pInfo, sizeof(AY_DEVINFO)) == 0) {
					return pDeInf;
				}
			}
		}
	}
	return nullptr;///< not found
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
			pGw->TimeOut = AY_CLNTGW_TIMEOUT_VAL;
			if (pId != nullptr) { *pId = i; }
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

	if (pId != nullptr) { *pId = -1; }
	AYCLNT_GW_Cnt = AYCLNT_CalcGwCnt(0);
	for (i = 0; i < AYCLNT_GW_Cnt; i++) {
		pGw = pAYCLNT_FindGwById(i);
		if (pGw != nullptr) {
			if (pGw->GwF.Full_) {
				if ((pGw->_Unique[0] == pUnique[0]) && (pGw->_Unique[1] == pUnique[1]) && (pGw->_Unique[2] == pUnique[2])) {
					if (pId != nullptr) { *pId = i; }
					return pGw;
				}
			}
		}
	}
	return pGw;
}

AY_GWINFO	*pAYCLNT_FindGwByPortNo(Ui16 PortNo, int *pId) {
	AY_GWINFO	*pGw = nullptr;
	Ui32 i, j;
	Ui08 *p;

	if (pId != nullptr) { *pId = -1; }
	AYCLNT_GW_Cnt = AYCLNT_CalcGwCnt(0);
	for (i = 0; i < AYCLNT_GW_Cnt; i++) {
		pGw = pAYCLNT_FindGwById(i);
		if (pGw != nullptr) {
			if (pGw->GwF.Full_) {
				if (pGw->MyPortNo == PortNo) {
					if (pId != nullptr) { *pId = i; }
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
	pGw0->TimeOut = AY_CLNTGW_TIMEOUT_VAL;

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
	case _GW_TCPH:
		pGw->TCP_Hdr = *((tcp_headerAll	*)pComp);
		pGw->GwF.Full_ = 1;
		printf("AYGW--> _GW_TCPH \n");
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
	case _GW_PORTNO:
		memcpy(&pGw->MyPortNo, pComp, 2);
		pGw->GwF.Full_ = 1;
		printf("AYGW--> _GW_PORTNO \n");
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


//============================ LOCAL CONNECTIONs ================================================================//
//============================ LOCAL CONNECTIONs ================================================================//
//============================ LOCAL CONNECTIONs ================================================================//
//============================ LOCAL CONNECTIONs ================================================================//
AY_LOCCONNINFOLST		AYCLNT_LOCCONNLIST_L1;
AY_LOCCONNINFOLST		*pLocConnInfos[((MAX_LOCCONNINFO_CNT / 4096) - 1)];///< total 4096 * (15+1) = 65536 devices supported
Si32					AYCLNT_LocConn_Cnt = 0;
Ui16					LocConnTestTask = 0;


/****************************************************************************/
/*! \fn AY_LOCCONNINFO	*pAYCLNT_LocConnById(int Id)
**
** \brief		      Find Local Connection
**
** \param    			Id -> local connection no
**
** \return				*pLocConn	: local connection address if valid or nullptr
**
*****************************************************************************/
AY_LOCCONNINFO	*pAYCLNT_LocConnById(int Id) {
	Ui32 i, j;
	if (Id >= 0) {
		if (Id < 4096) {
			return (&AYCLNT_LOCCONNLIST_L1.Info[Id]);
		}
		else {
			Id -= 4096;
			j = Id >> 12;
			i = Id & 0xFFF;
			if (pLocConnInfos[j] != nullptr) {
				return (&pLocConnInfos[j]->Info[i]);
			}
			else {
				return nullptr;
			}
		}
	}
	return nullptr;
}

/****************************************************************************/
/*! \fn AY_LOCCONNINFO	*pAYCLNT_FindFirstFreeLocConnId(int *pId)
**
** \brief		      Find First Free Local Connection
**
** \param    			&pId -> address of free Local Connection no register
**
** \return				*pQue	: free Local Connection's address
**
*****************************************************************************/
AY_LOCCONNINFO	*pAYCLNT_FindFirstFreeLocConnId(int *pId) {
	AY_LOCCONNINFO	*pLocConn = nullptr;
	Ui32 i, j = 0;

	for (i = 0; i < MAX_LOCCONNINFO_CNT; i++) {
		if ((i & 0xFFF) == 0) {
			if (i == 0) {
				pLocConn = &AYCLNT_LOCCONNLIST_L1.Info[0];
			}
			else {
				j = (i - 4096) >> 12;
				pLocConn = (AY_LOCCONNINFO	*)pLocConnInfos[j];
				if (pLocConn == nullptr) {
					pLocConnInfos[j] = (AY_LOCCONNINFOLST	*)_AY_MallocMemory(sizeof(AY_LOCCONNINFOLST));
					memset(pLocConnInfos[j], 0, sizeof(AY_LOCCONNINFOLST));
				}
				pLocConn = &pLocConnInfos[j]->Info[0];
			}
		}
		if (pLocConn->LocConnF.Full_ == 0) {
			memset(pLocConn, 0, sizeof(AY_LOCCONNINFO));
			pLocConn->TimeOut = AY_CLNTLOCCONN_TIMEOUT_VAL;
			if (pId != 0) { *pId = i; }
			return pLocConn;
		}
		pLocConn++;
	}
	return nullptr;
}


/****************************************************************************/
/*! \fn Si32 AYCLNT_CalcLocConnCnt(int *pCnt)
**
** \brief		       Calculate Local Connection Length
**
** \param    			&pCnt -> address of total Local Connections register
**
** \return				*pCnt	: total Local Connections
** 						lst		: last index of full Local Connection + 1
**
*****************************************************************************/
Si32 AYCLNT_CalcLocConnCnt(int *pCnt) {
	AY_LOCCONNINFO	*pLocConn = nullptr;
	Ui32 i, j = 0;
	int tot = 0;
	int lst = -1;

	tot = 0;
	for (i = 0; i < MAX_LOCCONNINFO_CNT; i++) {
		if ((i & 0xFFF) == 0) {
			if (i == 0) {
				pLocConn = &AYCLNT_LOCCONNLIST_L1.Info[0];
			}
			else {
				j = (i - 4096) >> 12;
				pLocConn = (AY_LOCCONNINFO	*)pLocConnInfos[j];
				if (pLocConn == nullptr) {
					if (pCnt != 0) { *pCnt = tot; }
					return(lst + 1);
				}
				pLocConn = &pLocConnInfos[j]->Info[0];
			}
		}
		if (pLocConn->LocConnF.Full_ == 1) {
			lst = i;
		}
		tot++;
		pLocConn++;
	}
	if (pCnt != 0) { *pCnt = tot; }
	return(lst + 1);
}


/****************************************************************************/
/*! \fn int AYCLNT_FindLocConnId(AY_LOCCONNINFO	*pLocConn)
**
** \brief		        convert Local Connection address to Local Connection no
**
** \param    			pQ
**
** \return				0-MAX_LOCCONNINFO_CNT	: Local Connection No
** 						-					: there is something wrong
**
*****************************************************************************/
int AYCLNT_FindLocConnId(AY_LOCCONNINFO	*pLocConn) {
	Ui32 i, j;
	Ui08 *p;
	if ((pLocConn >= &AYCLNT_LOCCONNLIST_L1.Info[0]) && (pLocConn <= &AYCLNT_LOCCONNLIST_L1.Info[4095])) {
		p = (Ui08 *)pLocConn;
		i = ((p - ((Ui08 *)&AYCLNT_LOCCONNLIST_L1.Info[0])) / sizeof(AY_LOCCONNINFO));
		return (i);
	}
	else {
		for (j = 0; j < cntof(pLocConnInfos); j++) {
			if (pLocConnInfos[j] != nullptr) {
				if ((pLocConn >= &pLocConnInfos[j]->Info[0]) && (pLocConn <= &pLocConnInfos[j]->Info[4095])) {
					p = (Ui08 *)pLocConn;
					i = ((p - ((Ui08 *)&pLocConnInfos[j]->Info[0])) / sizeof(AY_LOCCONNINFO));
					return (i + ((j + 1) * 4096));
				}
			}
		}
	}
	return -1;
}


/****************************************************************************/
/*! \fn AY_LOCCONNINFO	*pAYCLNT_FindLocConnByIPA(ip_headerAll *pIPA, int *pId, uip_eth_addr *pMAC, Ui08 SrcDst)
**
** \brief		        find Local Connection address for determined PI Header packet
**
** \param    			pIPA -> PI Header packet
**
** \return				pLocConn	: Local Connection Address
** 						pId			: Local Connection no
** 						pMAC		: MAC match required ( OPTIONAL)
** 						SrcDst		: MAC type source or destination ( OPTIONAL)
**
*****************************************************************************/
AY_LOCCONNINFO	*pAYCLNT_FindLocConnByIPA(ip_headerAll *pIPA, int *pId, uip_eth_addr *pMAC, Ui08 SrcDst) {
	AY_LOCCONNINFO	*pLocConn = nullptr;
	Ui32 i, j;
	Ui08 *p;

	if (pId != 0) { *pId = -1; }
	AYCLNT_LocConn_Cnt = AYCLNT_CalcLocConnCnt(0);
	if (AYCLNT_LocConn_Cnt >= 0) {
		for (i = 0; i <= AYCLNT_LocConn_Cnt; i++) {
			pLocConn = pAYCLNT_LocConnById(i);
			if (pLocConn != nullptr) {
				if (pLocConn->LocConnF.Full_) {
					if (memcmp(&pLocConn->IPA_Hdr, pIPA, sizeof(ip_headerAll)) == 0) {
						if ((SrcDst == _ETH_NULL_) || (memcmp(&DefaultMac0[0], pMAC, sizeof(uip_eth_addr)) == 0) || ((SrcDst== _ETH_SRC_)&&(memcmp(&pLocConn->src, pMAC, sizeof(uip_eth_addr)) == 0)) || ((SrcDst == _ETH_DST_) && (memcmp(&pLocConn->dest, pMAC, sizeof(uip_eth_addr)) == 0))) {
							if (pId != 0) { *pId = i; }
							return pLocConn;
						}
					}
				}
			}
		}
	}
	return pLocConn;
}


/****************************************************************************/
/*! \fn AY_LOCCONNINFO	*pAYCLNT_FindLocConnByIPA_Rvs(ip_headerAll *pIPA, int *pId, uip_eth_addr *pMAC, Ui08 SrcDst)
**
** \brief		        find Local Connection address for determined PI Header packet
**
** \param    			pIPA -> PI Header packet
**
** \return				pLocConn	: Local Connection Address
** 						pId			: Local Connection no
** 						pMAC		: MAC match required ( OPTIONAL)
** 						SrcDst		: MAC type source or destination ( OPTIONAL)
**
*****************************************************************************/
AY_LOCCONNINFO	*pAYCLNT_FindLocConnByIPA_Rvs(ip_headerAll *pIPA, int *pId, uip_eth_addr *pMAC, Ui08 SrcDst) {
	AY_LOCCONNINFO	*pLocConn = nullptr;
	ip_headerAll IPA;
	Ui32 i, j;
	Ui08 *p;

	if (pId != 0) { *pId = -1; }
	IPA.saddr = pIPA->daddr;
	IPA.daddr = pIPA->saddr;
	IPA.dport = pIPA->dport;
	IPA.sport = pIPA->sport;
	AYCLNT_LocConn_Cnt = AYCLNT_CalcLocConnCnt(0);
	if (AYCLNT_LocConn_Cnt >= 0) {
		for (i = 0; i <= AYCLNT_LocConn_Cnt; i++) {
			pLocConn = pAYCLNT_LocConnById(i);
			if (pLocConn != nullptr) {
				if (pLocConn->LocConnF.Full_) {
					if ((SrcDst == _ETH_NULL_) || (memcmp(&DefaultMac0[0], pMAC, sizeof(uip_eth_addr)) == 0) || ((SrcDst == _ETH_SRC_) && (memcmp(&pLocConn->src, pMAC, sizeof(uip_eth_addr)) == 0)) || ((SrcDst == _ETH_DST_) && (memcmp(&pLocConn->dest, pMAC, sizeof(uip_eth_addr)) == 0))) {
						if (pId != 0) { *pId = i; }
						return pLocConn;
					}
				}
			}
		}
	}
	return pLocConn;
}

/****************************************************************************/
/*! \fn AY_LOCCONNINFO	*pAYCLNT_TestAddOrUpdateLocConn(AY_LOCCONNINFO	*pLocConn, int *pId, uip_eth_addr *pMAC, Ui08 SrcDst)
**
** \brief		        if valid update else generate new Local Connection
**
** \param    			pLocConn	: Local Connection Address
**
** \return				pLocConn0	: New local connection
** 						pId			: Local Connection no
** 						pMAC		: MAC match required ( OPTIONAL)
** 						SrcDst		: MAC type source or destination ( OPTIONAL)
**
*****************************************************************************/
AY_LOCCONNINFO	*pAYCLNT_TestAddOrUpdateLocConn(AY_LOCCONNINFO	*pLocConn, int *pId, uip_eth_addr *pMAC, Ui08 SrcDst) {
	AY_LOCCONNINFO	*pLocConn0 = nullptr;

	pLocConn0 = pAYCLNT_FindLocConnByIPA(&pLocConn->IPA_Hdr, pId, pMAC, SrcDst);
	if (pLocConn0 != nullptr) {///< update
		*pLocConn0 = *pLocConn;
	}
	else {
		pLocConn0 = pAYCLNT_FindFirstFreeLocConnId(pId);
		*pLocConn0 = *pLocConn;
	}
	pLocConn0->TimeOut = AY_CLNTLOCCONN_TIMEOUT_VAL;
	pLocConn0->LocConnF.Full_ = 1;
	return pLocConn0;
}


/****************************************************************************/
/*! \fn int AYCLNT_LocConnTimeoutTest(void)
**
** \brief		        Test timeout for Local Connection list
**
** \param    			-
**
** \return				1
**
*****************************************************************************/
int AYCLNT_LocConnTimeoutTest(void) {
	AY_LOCCONNINFO	*pLocConn;
	int i;
	AYCLNT_LocConn_Cnt = AYCLNT_CalcLocConnCnt(0);
	if (AYCLNT_LocConn_Cnt >= 0) {
		for (i = 0; i <= AYCLNT_LocConn_Cnt; i++) {
			pLocConn = pAYCLNT_LocConnById(i);
			if (pLocConn != nullptr) {
				if (pLocConn->LocConnF.Full_) {
					if (pLocConn->TimeOut) {
						pLocConn->TimeOut--;
						if (pLocConn->TimeOut == 0) {
							memset(pLocConn, 0, sizeof(AY_LOCCONNINFO));
							pLocConn->LocConnF.Full_ = 0;///< release slot
						}
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
 


/****************************************************************************/
/*! \fn AY_CLNTQUEUE	*pAYCLNT_FindFirstFreeQueueId(int *pId)
**
** \brief		      Find First Free Slot
**
** \param    			&pId -> address of free slot no register
**
** \return				*pQue	: free slot's address
**
*****************************************************************************/
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
			pQue->TimeOut = AY_CLNTQUEUE_TIMEOUT_VAL;
			*pId = i;
			return pQue;
		}
		pQue++;
	}
	return nullptr;
}

/****************************************************************************/
/*! \fn int AYCLNT_CalcQueueCnt(int *pCnt)
**
** \brief		       Calculate Queue Length
**
** \param    			&pCnt -> address of total slots register
**
** \return				*pCnt	: total slots
** 						lst		: last index of full slot + 1
**
*****************************************************************************/
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

/****************************************************************************/
/*! \fn int AYCLNT_QueueReleaseSlot(AY_CLNTQUEUE	*pQue)
**
** \brief		        Release slot
**
** \param    			SlotNo
**
** \return				1	: OK
** 						-	: there is something wrong
**
*****************************************************************************/
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




/****************************************************************************/
/*! \fn AY_CLNTQUEUE *AYCLNT_QueueReadSlot(int SlotNo)
**
** \brief		        Read slot information
**
** \param    			SlotNo
**
** \return				slot pointer
** 						0	: there is something wrong
**
*****************************************************************************/
AY_CLNTQUEUE *AYCLNT_QueueReadSlot(int SlotNo) {
	AY_CLNTQUEUE *pQue;
	int j;

	if (SlotNo < 4096) {
		pQue = &AYCLNT_QUEUELIST_L1.Info[SlotNo];
	}
	else {
		j = (SlotNo - 4096) >> 12;
		pQue = (AY_CLNTQUEUE	*)pQueueInfos[j];
		if (pQue != nullptr){
			pQue = &pQueueInfos[j]->Info[(SlotNo & 0xFFF)];
		}
	}
	if (pQue != nullptr){
		if (pQue->QueF.Full_) {
			return pQue;

		}
	}
	return nullptr;
}


/****************************************************************************/
/*! \fn int AYCLNT_QueueReadStatus(int SlotNo)
**
** \brief		        Read slot status
**
** \param    			SlotNo
**
** \return				-1	: there is something wrong
** 						0-255	: status
**
*****************************************************************************/
int AYCLNT_QueueReadStatus(int SlotNo) {
	AY_CLNTQUEUE *pQue;
	int j;

	pQue = AYCLNT_QueueReadSlot(SlotNo);
	if (pQue != nullptr) {
		return ((int)pQue->Status);
	}
	else {
		return -1;
	}
}

/****************************************************************************/
/*! \fn int AYCLNT_QueueFindSlotNo(AY_CLNTQUEUE *pQue)
**
** \brief		        convert slot address to slot no
**
** \param    			pQ
**
** \return				0-MAX_QUEUE_LENGTH	: SlotNo
** 						-					: there is something wrong
**
*****************************************************************************/
int AYCLNT_QueueFindSlotNo(AY_CLNTQUEUE *pQue) {
	Ui32 i, j;
	Ui08 *p;
	if ((pQue >= &AYCLNT_QUEUELIST_L1.Info[0]) && (pQue <= &AYCLNT_QUEUELIST_L1.Info[4095])) {
		p = (Ui08 *)pQue;
		i = ((p - ((Ui08 *)&AYCLNT_QUEUELIST_L1.Info[0])) / sizeof(AY_CLNTQUEUE));
		return (i);
	}
	else {
		for (j = 0; j < cntof(pQueueInfos); j++) {
			if (pQueueInfos[j] != nullptr) {
				if ((pQue >= &pQueueInfos[j]->Info[0]) && (pQue <= &pQueueInfos[j]->Info[4095])) {
					p = (Ui08 *)pQue;
					i = ((p - ((Ui08 *)&pQueueInfos[j]->Info[0])) / sizeof(AY_CLNTQUEUE));
					return (i + ((j + 1) * 4096));
				}
			}
		}
	}
	return -1;
}




//============================  CORE  ================================================================//
//============================  CORE  ================================================================//
//============================  CORE  ================================================================//
//============================  CORE  ================================================================//

/****************************************************************************/
/*! \fn void AYCLNT_CoreInit(void)
**
** \brief		       init process
**
** \param    			-
**
** \return				-
**
*****************************************************************************/
void AYCLNT_CoreInit(void) {
	memset(	&DevLevel1				, 0, sizeof(AY_DEVINFOLST));
	memset( &DevRemote				, 0, sizeof(AY_DEVINFOLST));
	memset( &DevRemoteTOut			, 0, sizeof(DevRemoteTOut));
	memset(	&pDevInfos				, 0, sizeof(pDevInfos));
	memset(	&AYCLNT_GWLISTE_L1		, 0, sizeof(AY_GWINFOLST));

	memset(	&pGwInfos				, 0, sizeof(pGwInfos));
	memset(	&AYCLNT_LOCCONNLIST_L1	, 0, sizeof(AY_LOCCONNINFOLST));
	memset(	&pLocConnInfos			, 0, sizeof(pLocConnInfos));
	memset(	&AYCLNT_GWLISTE_L1		, 0, sizeof(AY_GWINFOLST));

	memset(	&AYCLNT_QUEUELIST_L1	, 0, sizeof(AY_GWQUEUELST));
	memset(	&pQueueInfos			, 0, sizeof(pQueueInfos));

	/*
	AY_DEVINFOLST	DevLevel1;
	AY_DEVINFOLST	DevRemote;
	Ui16			DevRemoteTOut[RMTDEV_MAX_CNT];
	AY_DEVINFOLST	*pDevInfos[((MAX_DEVINFO_CNT / 4096) - 1)];
	AY_GWINFOLST			AYCLNT_GWLISTE_L1;
	AY_GWINFOLST			*pGwInfos[((MAX_CLNTGWLIST_CNT / 4096) - 1)];
	AY_LOCCONNINFOLST		AYCLNT_LOCCONNLIST_L1;
	AY_LOCCONNINFOLST		*pLocConnInfos[((MAX_LOCCONNINFO_CNT / 4096) - 1)]; /
		AY_GWQUEUELST			AYCLNT_QUEUELIST_L1;
	AY_GWQUEUELST			*pQueueInfos[((MAX_CLNTQUEUE_CNT / 4096) - 1)];
	*/
}

/****************************************************************************/
/*! \fn void AYCLNT_CoreDoTask(void)
**
** \brief		       main process
**
** \param    			-
**
** \return				-
**
*****************************************************************************/
int AY_CLNTCORE_InstRow=0;
void AYCLNT_CoreDoTask(void) {
	AY_CLNTQUEUE *pQue;
	AY_GWINFO	*pGw0;
	int tmp,i,j,RowCnt;

	RowCnt = AYCLNT_CalcQueueCnt(&tmp);
	for (i = 0; i < RowCnt; i++) {
		pQue = AYCLNT_QueueReadSlot(i);
		if (pQue != nullptr) {///< it is a valid slot
			if (pQue->QueF.Full_) {///< not empty slot. there works to do
				switch (pQue->Status) {
				case _SEND_TO_SRV:

				break;
				case _FIND_GW:///< first step for side packet
					pGw0 = pAYCLNT_FindGwByUnique((Ui32 *)&pQue->pInfo->DevRead._Unique[0], &tmp);
					if (pGw0 != nullptr) {
						pQue->pGw = pGw0;
						//AY_Client_ChngServerConn = 0;
						pQue->Status = _CHNG_SERVER_CONN;
					}
					else if(pQue->pInfo->DevRead._Type == _MIRROR_){
						AY_SendGwInfoRequest(pQue,i);
						pQue->Status = _WAIT_SERVER_FOR_GW;
					}
					else {///< SIDE connection and unknown destination so discard packet
						AYCLNT_QueueReleaseSlot(pQue);
					}
				break;
				case _FIND_GW2:///< first step for quest packet
					pGw0 = pAYCLNT_FindGwByUnique( (Ui32 *)&pQue->pInfo->DevRead._Unique[0], &tmp);
					if (pGw0 != nullptr) {
						pQue->pGw = pGw0;
						pQue->Status = _PRE_SEND_PCKT;
						AY_SendGwInfoSend2(pQue, i);
						//AY_Client_ChngServerConn = 0;
					}
					else {///< SIDE connection and unknown destination so discard packet
						AYCLNT_QueueReleaseSlot(pQue);
					}
				break;
				case _ASK_FOR_GW:
				case _WAIT_SERVER_FOR_GW:
					///... wait untill packet arrive or timeout
				break;
				case _CHNG_SERVER_CONN:
					if (!AY_Client_ChngServerConn) {
						AY_SendGwInfoSend(pQue, i);///< Ready To Send Packet To Destination
						pQue->Status = _PRE_SEND_PCKT;
					}
				break;
				case _PRE_SEND_PCKT:
					if (pQue->pInfo->DevRead._Type == _REAL_) {
					}
				default:
					AYCLNT_QueueReleaseSlot(pQue);
				break;
				//===========================================================//
				//============== 
				//case _FIND_GW2:
				//	break;
				};
				//---------- Test Timeouts -------------//
				pQue->TimeOut--;
				if (pQue->TimeOut <= 0) {///< Timeout release this slot.
					AYCLNT_QueueReleaseSlot(pQue);
				}
			}
		}
	}
}