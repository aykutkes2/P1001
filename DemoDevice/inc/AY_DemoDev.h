#pragma once
#include <AY_Type.h>
#include <AY_Memory.h>
#include <AY_Socket.h>

#define DEV_REMOTE_TIMEOUT			50000

#pragma pack(push, 1)

typedef struct _AY_ADRLST {
	void *pAdr[256];
}AY_ADRLST;
typedef union _AY_DEVFLAGs {
	Ui16 _DevFlgs;
	struct {
		Ui16	Full_ : 1;
		Ui16	InfoLoaded_ : 1;
	};
}AY_DEVFLAGs;
typedef struct  _AY_DEVINFO{///< 2 + 42 + 16 + 16 + 4+4+4 = 88 Bytes
	AY_DEVFLAGs		DevF;
	udp_headerAll	UDP_Hdr;
	Ui08			Sessionkey[16];
	AY_DeviceRead	DevRead;
	Ui32			SendCnt;
	Ui32			ReadCnt;
	Ui32			ErrCnt;
}AY_DEVINFO;

#pragma pack(pop)

enum _DEVINFO_COMPs{
	_DEVINFOALL,
	_DEV_FLG,
	_DEV_UDPH,
	_DEV_SSK,
	_DEV_READ,
	_DEV_READ_ALL,
	_DEV_SNDCNT,
	_DEV_RDCNT,
	_DEV_ERRCNT,
	_DEV_DELCNTS,
	_DEV_DELETE,
	_DEV_LASTCOMP
};

extern int AY_Demo_RemoteDevTimeoutTest(void);
extern int AY_Demo_UpdateDevInfo(AY_DEVINFO	*pDeInf, Ui08 *pComp, Ui08 Comp);
extern int AY_Demo_AddDevToList(Ui08 *pComp, Ui32 DevNo, Ui08 Comp);
extern AY_DEVINFO *pAY_FindDevInfoByDevNo(Ui32 DevNo);
extern AY_DEVINFO *pAY_FindLocDevInfoByIP(Ui32 LocIP);
extern AY_DEVINFO *pAY_FindRmtDevInfoByMAC(Ui08 *pMac, Ui08 SrcDst);
#define pAY_FindRmtDevInfoBySrcMAC(pMac)			pAY_FindRmtDevInfoByMAC(pMac, 0)
#define pAY_FindRmtDevInfoByDstMAC(pMac)			pAY_FindRmtDevInfoByMAC(pMac, 1)


