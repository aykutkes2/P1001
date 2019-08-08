//#include "pch.h"
#undef UNICODE

#include <stdio.h>
#include <time.h>
//#include <windows.h>

#include <AY_Memory.h>
#include <AY_DemoDev.h>
			
AY_DEVINFO	DevLevel1[4096];
AY_DEVINFO	DevRemote[256];
Ui16		DevRemoteTOut[256];
AY_DEVINFO	**pDevInfos[15];///< total 4096 * (15+1) = 65536 devices supported

/*
*
*/
int AY_Demo_RemoteDevTimeoutTest(void) {
	int i;
	for (i = 0; i < 256; i++) {
		if (DevRemoteTOut[i]) {
			DevRemoteTOut[i] --;
			if (DevRemoteTOut[i] == 0) {
				DevRemote[i].DevF.Full_ = 0;
			}
		}
	}
	return 1;
}

/*
*
*/
int AY_Demo_UpdateDevInfo(AY_DEVINFO	*pDeInf, Ui08 *pComp, Ui08 Comp) {
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
int AY_Demo_AddDevToList(Ui08 *pComp, Ui32 DevNo, Ui08 Comp) {
	Ui32 i;
	Ui16 j,k;
	AY_DEVINFO	*pDeInf;
	//=======================================================================================================//
	if ((DevNo & 0xFFFFFF) > 65535) {
		return -1;
	}
	else if ((DevNo & 0xFF000000)&& ((DevNo& 0xFFFFFF) < 0x000101)) {
		k = 0;
		DevNo &= 0xFFFFFF;
		if (DevNo == 256) {///< find empty slot
			pDeInf = &DevRemote[0];
			for (i = 0; i < 256; i++) {
				if (!pDeInf->DevF.Full_) {
					k = i;
					break;
				}
			}
			if (i == 256) {///< all full, find oldest and use it
				j = 0xFFFF;
				for (i = 0; i < 256; i++) {
					if (DevRemoteTOut[i] < j) {
						j = DevRemoteTOut[i];
						k = i;
					}
				}
			}
			pDeInf = &DevRemote[k];
			pDeInf->DevF.Full_ = 1;
			memset(pDeInf, 0, sizeof(AY_DEVINFO));
			DevRemoteTOut[k] = DEV_REMOTE_TIMEOUT;
			printf("AYDEV--> New Device Remote Added DevNo=%d \n", (k+1));
		}
		else {
			k = DevNo;
			pDeInf = &DevRemote[k];
			DevRemoteTOut[k] = DEV_REMOTE_TIMEOUT;
			printf("AYDEV--> Device Remote Found DevNo=%d \n", DevNo);
		}
		i = k;
	}
	else if (DevNo < 4096) {
		pDeInf = &DevLevel1[0];
		i = DevNo;
		printf("AYDEV--> New Device Level 1 created DevNo=%d \n", DevNo);
	}
	else {
		i = DevNo - 4096;
		pDeInf = *pDevInfos[(i >> 12)];
		i = (i & 0xFFF);
		if (pDeInf == 0) {
			pDeInf = (AY_DEVINFO	*)_AY_MallocMemory(sizeof(DevLevel1));
			memset(pDeInf, 0, sizeof(DevLevel1));
			printf("AYDEV--> New Device List %d created DevNo=%d \n", ((DevNo >> 12)+2), DevNo);
		}
	}
	//=======================================================================================================//
	if (Comp < _DEV_LASTCOMP) {
		AY_Demo_UpdateDevInfo(&pDeInf[i], pComp, Comp);
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
	if ((DevNo & 0xFFFFFF) > 65535) {
		return 0;
	}
	else if ((DevNo & 0xFF000000) && ((DevNo & 0xFFFFFF) < 0x000101)) {
		DevNo &= 0xFFFFFF;
		pDeInf = &DevRemote[DevNo];
	}
	else if (DevNo < 4096 ) {
		pDeInf = &DevLevel1[DevNo];
	}
	else {
		i = DevNo - 4096;
		pDeInf = *pDevInfos[(i >> 12)];
		i = (i & 0xFFF);
		pDeInf += i;
	}
	return pDeInf;
}

/*
*
*/
AY_DEVINFO *pAY_FindLocDevInfoByIP(Ui32 LocIP) {
	Ui32 i;
	AY_DEVINFO	*pDeInf;
	for (i = 0; i < 65536; i++) {
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
