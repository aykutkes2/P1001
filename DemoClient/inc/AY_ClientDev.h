#pragma once
#include <AY_Type.h>
#include <AY_Memory.h>
#include <AY_Socket.h>

#define DEV_REMOTE_TIMEOUT			50000
#define MAX_DEVINFO_CNT				65536
#define MAX_CLNTGWLIST_CNT			65536
#define MAX_CLNTQUEUE_CNT			65536

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
typedef struct  _AY_DEVINFO{///< 2 + 24 = 26 Bytes ----------- 2 + 42 + 16 + 16 + 4+4+4 = 88 Bytes
	AY_DEVFLAGs		DevF;
	AY_DeviceRead	DevRead;
	//---------- Bunlar Gidecek ---------//
	udp_headerAll	UDP_Hdr;
	Ui08			Sessionkey[16];
	Ui32			SendCnt;
	Ui32			ReadCnt;
	Ui32			ErrCnt;
}AY_DEVINFO;
typedef struct  _AY_DEVINFOLST {///< 4096*26 = 98304 Bytes
	AY_DEVINFO		Info[4096];
}AY_DEVINFOLST;
//=========================================================//
typedef union _AY_GWFLAGs {
	Ui16 _GwFlgs;
	struct {
		Ui16	Full_ : 1;
		Ui16	InfoLoaded_ : 1;
	};
}AY_GWFLAGs;
typedef struct  _AY_GWINFO {///< 2 + 12 + 42 + 16 +  4+4+4+4 = 88 Bytes
	AY_GWFLAGs		GwF;
	Ui32			_Unique[3];				///< Gateway's Unique ID ( This gateway for local and remote gateway ID for remote type device)
	udp_headerAll	UDP_Hdr;
	Ui08			Sessionkey[16];
	Ui32			TimeOut;
	Ui32			SendCnt;
	Ui32			ReadCnt;
	Ui32			ErrCnt;
}AY_GWINFO;
typedef struct  _AY_GWINFOLST {///< 4096*88 = 360448 Bytes
	AY_GWINFO	Info[4096];
}AY_GWINFOLST;
//==============================================================//
typedef union _CLNTQ_FLG {
	Ui08	AllFlgs;
	struct {
		Ui08		Full_:1;
		Ui08		Finished : 1;
	};
}CLNTQ_FLG;
typedef struct _AY_CLNTQUEUE {///< 4 + 4+ 2 + 1 + 1+ 4+ 4 = 20 Bytes
	AY_DEVINFO		*pInfo;
	Ui08			*pDataIO;
	Ui16			DataIOLen;
	Ui08			Status;
	CLNTQ_FLG		QueF;
	Ui32			TimeOut;
	Ui32			DevListNo;
}AY_CLNTQUEUE;
typedef struct  _AY_GWQUEUELST {///< 4096*20 = 81920 Bytes
	AY_CLNTQUEUE	Info[4096];
}AY_GWQUEUELST;

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

enum _GWINFO_COMPs {
	_GWINFOALL,
	_GW_FLG,
	_GW_UNQUE,
	_GW_UDPH,
	_GW_SSK,
	_GW_UNQUE_ALL,
	_GW_SNDCNT,
	_GW_RDCNT,
	_GW_ERRCNT,
	_GW_DELCNTS,
	_GW_DELETE,
	_GW_LASTCOMP
};
extern int AYCLNT_RemoteDevTimeoutTest(void);
extern int AYCLNT_UpdateDevInfo(AY_DEVINFO	*pDeInf, Ui08 *pComp, Ui08 Comp);
extern int AYCLNT_AddDevToList(Ui08 *pComp, Ui32 DevNo, Ui08 Comp);
extern AY_DEVINFO *pAY_FindDevInfoByDevNo(Ui32 DevNo);
extern AY_DEVINFO *pAY_FindLocDevInfoByIP(Ui32 LocIP);
extern AY_DEVINFO *pAY_FindRmtDevInfoByMAC(Ui08 *pMac, Ui08 SrcDst);
#define pAY_FindRmtDevInfoBySrcMAC(pMac)			pAY_FindRmtDevInfoByMAC(pMac, 0)
#define pAY_FindRmtDevInfoByDstMAC(pMac)			pAY_FindRmtDevInfoByMAC(pMac, 1)



//============================ GW LISTS ================================================================//
//============================ GW LISTS ================================================================//
//============================ GW LISTS ================================================================//
//============================ GW LISTS ================================================================//;
extern Ui32			AYCLNT_GW_Cnt;

extern AY_GWINFO	*pAYCLNT_FindGwById(int Id);
extern AY_GWINFO	*pAYCLNT_FindFirstFreeGwId(int *pId);
extern int			AYCLNT_CalcGwCnt(int *pCnt);
extern int			AYCLNT_FindGwId(AY_GWINFO	*pGw);
extern AY_GWINFO	*pAYCLNT_FindGwByUnique(Ui32 *pUnique, int *pId);
extern int			AYCLNT_TestAddOrUpdateGw(AY_GWINFO	*pGw, int *pId);
extern int			AYCLNT_UpdateGwInfo(AY_GWINFO	*pGw, Ui08 *pComp, Ui08 Comp);
extern AY_GWINFO	*pAYCLNT_AddGwToList(Ui08 *pComp, Ui32 *pUnique, Ui08 Comp);
extern int			AYCLNT_GwReleaseSlot(AY_GWINFO	*pGw);
extern int			AYCLNT_GwTimeoutTest(void);

