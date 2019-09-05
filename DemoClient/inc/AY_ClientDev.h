#pragma once
#include <AY_Type.h>
#include <AY_Memory.h>
#include <AY_Socket.h>

#define MAX_DEVINFO_CNT					65536
#define MAX_CLNTGWLIST_CNT				65536
#define MAX_CLNTQUEUE_CNT				65536
#define MAX_LOCCONNINFO_CNT				65536

#define DEV_REMOTE_TIMEOUT				50000
#define AY_CLNTQUEUE_TIMEOUT_VAL		50000
#define AY_CLNTLOCCONN_TIMEOUT_VAL		50000
#define AY_CLNTGW_TIMEOUT_VAL			50000


#define AY_CLNTCORE_DO_EACH				4096

#pragma pack(push, 1)

typedef union _AY_DEVFLAGs {
	Ui16 _DevFlgs;
	struct {
		Ui16	Full_ : 1;
		Ui16	InfoLoaded_ : 1;
	};
}AY_DEVFLAGs;
typedef struct  _AY_DEVINFO{///< 2 + 24 = 26 Bytes 
	AY_DEVFLAGs		DevF;
	AY_DeviceRead	DevRead;
}AY_DEVINFO;
typedef struct  _AY_DEVINFOLST {///< 4096*26 = 98304 Bytes
	AY_DEVINFO		Info[4096];
}AY_DEVINFOLST;
//=========================================================//
typedef union _AY_LOCCONNFLAGs {
	Ui16 _DevFlgs;
	struct {
		Ui16	Full_ : 1;
		Ui16	InfoLoaded_ : 1;
	};
}AY_LOCCONNFLAGs;
typedef struct  _AY_LOCCONNINFO {///< 2 + 4 + 12 + 4 = 22 Bytes 
	AY_LOCCONNFLAGs		LocConnF;//2 
	AY_DEVINFO			*pDevInfo;//4
	ip_headerAll		IPA_Hdr;//12
	Si32				TimeOut;
}AY_LOCCONNINFO;
typedef struct  _AY_LOCCONNINFOLST {///< 4096*26 = 98304 Bytes
	AY_LOCCONNINFO		Info[4096];
}AY_LOCCONNINFOLST;
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
	Ui16			MyPortNo;
	Si32			TimeOut;
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
typedef struct _AY_CLNTQUEUE {///< 4 + 4 + 4+ 2 + 1 + 1+ 4+ 4 = 24 Bytes
	AY_DEVINFO		*pInfo;
	AY_GWINFO		*pGw;
	AY_LOCCONNINFO	*pLocConn;
	Ui08			*pDataIO;
	Ui16			DataIOLen;
	Ui08			Status;
	CLNTQ_FLG		QueF;
	Si32			TimeOut;
}AY_CLNTQUEUE;
typedef struct  _AY_GWQUEUELST {///< 4096*24 = 98304 Bytes
	AY_CLNTQUEUE	Info[4096];
}AY_GWQUEUELST;

#pragma pack(pop)

enum _DEVINFO_COMPs{
	_DEVINFOALL,
	_DEV_FLG,
	_DEV_READ,
	_DEV_READ_ALL,
	_DEV_UNIQUE_ALL,
	_DEV_DELETE,
	_DEV_LASTCOMP
};

enum _GWINFO_COMPs {
	_GWINFOALL,
	_GW_FLG,
	_GW_UNQUE,
	_GW_PORTNO,
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
extern AY_DEVINFO *pAYCLNT_AddDevToList(Ui08 *pComp, Ui32 DevNo, Ui08 Comp);
extern AY_DEVINFO *pAY_FindDevInfoByDevNo(Ui32 DevNo);
extern AY_DEVINFO *pAY_FindLocDevInfoByIP(Ui32 LocIP);
extern AY_DEVINFO *pAY_FindRmtDevInfoByAll(AY_DEVINFO *pInfo);


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
extern AY_GWINFO	*pAYCLNT_FindGwByPortNo(Ui16 PortNo, int *pId);
extern int			AYCLNT_TestAddOrUpdateGw(AY_GWINFO	*pGw, int *pId);
extern int			AYCLNT_UpdateGwInfo(AY_GWINFO	*pGw, Ui08 *pComp, Ui08 Comp);
extern AY_GWINFO	*pAYCLNT_AddGwToList(Ui08 *pComp, Ui32 *pUnique, Ui08 Comp);
extern int			AYCLNT_GwReleaseSlot(AY_GWINFO	*pGw);
extern int			AYCLNT_GwTimeoutTest(void);



//============================ LOCAL CONNECTIONs ================================================================//
//============================ LOCAL CONNECTIONs ================================================================//
//============================ LOCAL CONNECTIONs ================================================================//
//============================ LOCAL CONNECTIONs ================================================================//
extern AY_LOCCONNINFOLST		AYCLNT_LOCCONNLIST_L1;
extern AY_LOCCONNINFOLST		*pLocConnInfos[((MAX_LOCCONNINFO_CNT / 4096) - 1)];///< total 4096 * (15+1) = 65536 devices supported
extern Ui32						AYCLNT_LocConn_Cnt;

/****************************************************************************/
/*! \fn AY_LOCCONNINFO	*pAYCLNT_LocConnById(int Id)
** \brief		      Find Local Connection
*****************************************************************************/
extern AY_LOCCONNINFO	*pAYCLNT_LocConnById(int Id);

/****************************************************************************/
/*! \fn AY_LOCCONNINFO	*pAYCLNT_FindFirstFreeLocConnId(int *pId)
** \brief		      Find First Free Local Connection
*****************************************************************************/
extern AY_LOCCONNINFO	*pAYCLNT_FindFirstFreeLocConnId(int *pId);

/****************************************************************************/
/*! \fn int AYCLNT_CalcLocConnCnt(int *pCnt)
** \brief		       Calculate Local Connection Length
*****************************************************************************/
extern int AYCLNT_CalcLocConnCnt(int *pCnt);

/****************************************************************************/
/*! \fn int AYCLNT_FindLocConnId(AY_LOCCONNINFO	*pLocConn)
** \brief		        convert Local Connection address to Local Connection no
*****************************************************************************/
extern int AYCLNT_FindLocConnId(AY_LOCCONNINFO	*pLocConn);

/****************************************************************************/
/*! \fn AY_LOCCONNINFO	*pAYCLNT_FindLocConnByIPA(ip_headerAll *pIPA, int *pId)
** \brief		        find Local Connection address for determined PI Header packet
*****************************************************************************/
extern AY_LOCCONNINFO	*pAYCLNT_FindLocConnByIPA(ip_headerAll *pIPA, int *pId);


/****************************************************************************/
/*! \fn AY_LOCCONNINFO	*pAYCLNT_FindLocConnByIPA_Rvs(ip_headerAll *pIPA, int *pId)
** \brief		        find Local Connection address for determined PI Header packet
*****************************************************************************/
extern AY_LOCCONNINFO	*pAYCLNT_FindLocConnByIPA_Rvs(ip_headerAll *pIPA, int *pId);

/****************************************************************************/
/*! \fn AY_LOCCONNINFO	*pAYCLNT_TestAddOrUpdateLocConn(AY_LOCCONNINFO	*pLocConn, int *pId)
** \brief		        if valid update else generate new Local Connection
*****************************************************************************/
extern AY_LOCCONNINFO	*pAYCLNT_TestAddOrUpdateLocConn(AY_LOCCONNINFO	*pLocConn, int *pId);

/****************************************************************************/
/*! \fn int AYCLNT_LocConnTimeoutTest(void)
** \brief		        Test timeout for Local Connection list
*****************************************************************************/
extern int AYCLNT_LocConnTimeoutTest(void);



//============================ QUEUE ================================================================//
//============================ QUEUE ================================================================//
//============================ QUEUE ================================================================//
//============================ QUEUE ================================================================//
extern Ui32					AYCLNT_QUEUE_Cnt;
/****************************************************************************/
/*! \fn AY_CLNTQUEUE	*pAYCLNT_FindFirstFreeQueueId(int *pId)
** \brief		      Find First Free Slot
*****************************************************************************/
extern AY_CLNTQUEUE	*pAYCLNT_FindFirstFreeQueueId(int *pId);

/****************************************************************************/
/*! \fn int AYCLNT_CalcQueueCnt(int *pCnt)
** \brief		       Calculate Queue Length
*****************************************************************************/
extern int AYCLNT_CalcQueueCnt(int *pCnt);

/****************************************************************************/
/*! \fn int AYCLNT_QueueReleaseSlot(AY_CLNTQUEUE	*pQue)
** \brief		        Release slot
*****************************************************************************/
extern int AYCLNT_QueueReleaseSlot(AY_CLNTQUEUE	*pQue);

/****************************************************************************/
/*! \fn AY_CLNTQUEUE *AYCLNT_QueueReadSlot(int SlotNo)
** \brief		        Read slot information
*****************************************************************************/
extern AY_CLNTQUEUE *AYCLNT_QueueReadSlot(int SlotNo);

/****************************************************************************/
/*! \fn int AYCLNT_QueueReadStatus(int SlotNo)
** \brief		        Read slot status
*****************************************************************************/
extern int AYCLNT_QueueReadStatus(int SlotNo);

/****************************************************************************/
/*! \fn int AYCLNT_QueueFindSlotNo(AY_CLNTQUEUE *pQue)
** \brief		        convert slot address to slot no
*****************************************************************************/
extern int AYCLNT_QueueFindSlotNo(AY_CLNTQUEUE *pQue);


enum _AYCLNT_STATUS {
	_FIND_GW,
	_ASK_FOR_GW,
	_WAIT_SERVER_FOR_GW,
	_CHNG_SERVER_CONN,
	_PRE_SEND_PCKT,

	_FIND_GW2=16,
};

//================== Extrenals ===========================//
extern int AY_SendGwInfoRequest(AY_CLNTQUEUE *pQue, Si32 row);
extern int AY_ChngPacketDest(udp_headerAll *pUDP, uip_eth_addr eth, Ui08 SrcDst);
extern int AY_SendGwInfoSend(AY_CLNTQUEUE *pQue, Si32 row);
extern int AY_SendGwInfoSend2(AY_CLNTQUEUE *pQue, Si32 row);