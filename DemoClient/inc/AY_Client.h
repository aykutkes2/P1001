#pragma once
#include <AY_Type.h>
#include <AY_Memory.h>

#pragma pack(push, 1)

typedef union _AY_FLGS {
	Ui32	_Flgs;
	struct {
		Ui32	_IntroF : 1;
		Ui32	_ClientInited : 1;
		Ui32	_GetMACadr : 1;
		Ui32	_WaitMACadr : 1;
		Ui32	_GetSrvIPadr : 1;
		Ui32	_WaitSrvIPadr : 1;

		Ui32	_DeleteServerConn : 1;
		Ui32	_StartSyncToServer : 1;
		Ui32	_WaitSyncToServer : 1;

		Ui32	_SendServer : 1;
		Ui32	_RecvServer : 1;
		Ui32	_GenerateRemoteDevs : 1;
		Ui32	_ListenThreads : 1;
		Ui32	_RemoteDevListEnd : 1;
		Ui32	_MAC_NotFound : 1;
		Ui32	_SrvMAC_NotFound : 1;

		Ui32	_ConnectRemoteGws : 1;
		Ui32	_StartRemoteGws : 1;
		Ui32	_WaitRemoteGws : 1;

		Ui32	_ChngServerConn : 1;///< generate new connection with server
		Ui32	_DynamicIP : 1;
	};

}AY_FLGS;

typedef struct _AY_GlobalRAM {
	AY_FLGS			AY_Flgs;
	Ui32			AY_DeviceCnt;
	Ui16			AY_DevPcktNo;
	AY_DeviceRead	AY_DeviceList[256];
	AY_DeviceRead	AY_DeviceLast;
	Ui08			AY_Unique[12];
	Ui08			AY_Sessionkey[16];
	Ui32			AY_SendCnt;
	Ui32			AY_ReadCnt;
	Ui32			AY_ErrCnt;
}AY_GlobalRAM;

extern	AY_GlobalRAM	AY_Ram;

#define AY_Client_Flags					AY_Ram.AY_Flgs._Flgs
#define AY_Client_Intro					AY_Ram.AY_Flgs._IntroF
#define AY_Client_Initied				AY_Ram.AY_Flgs._ClientInited
#define AY_Client_GetMACadr				AY_Ram.AY_Flgs._GetMACadr
#define AY_Client_WaitMACadr			AY_Ram.AY_Flgs._WaitMACadr
#define AY_Client_GetSrvIPadr			AY_Ram.AY_Flgs._GetSrvIPadr
#define AY_Client_WaitSrvIPadr			AY_Ram.AY_Flgs._WaitSrvIPadr

#define AY_Client_StartSyncToServer		AY_Ram.AY_Flgs._StartSyncToServer
#define AY_Client_WaitSyncToServer		AY_Ram.AY_Flgs._WaitSyncToServer

#define AY_Client_SendServer			AY_Ram.AY_Flgs._SendServer
#define AY_Client_RecvServer			AY_Ram.AY_Flgs._RecvServer
#define AY_Client_GenerateRemoteDevs	AY_Ram.AY_Flgs._GenerateRemoteDevs
#define AY_Client_ListenThreads			AY_Ram.AY_Flgs._ListenThreads
#define AY_Client_RemoteDevListEnd		AY_Ram.AY_Flgs._RemoteDevListEnd
#define AY_Client_MAC_NotFound			AY_Ram.AY_Flgs._MAC_NotFound
#define AY_Client_SrvMAC_NotFound		AY_Ram.AY_Flgs._SrvMAC_NotFound

#define AY_Client_ConnectRemoteGws		AY_Ram.AY_Flgs._ConnectRemoteGws
#define AY_Client_StartRemoteGws		AY_Ram.AY_Flgs._StartRemoteGws
#define AY_Client_WaitRemoteGws			AY_Ram.AY_Flgs._WaitRemoteGws

#define AY_Client_ChngServerConn		AY_Ram.AY_Flgs._ChngServerConn
#define AY_Client_DynamicIP				AY_Ram.AY_Flgs._DynamicIP

#define AY_DynamicIP					AY_Client_DynamicIP
#define AY_SendCnt						AY_Ram.AY_SendCnt
#define AY_ReadCnt						AY_Ram.AY_ReadCnt
#define AY_ErrCnt						AY_Ram.AY_ErrCnt

#pragma pack(pop)

#define CLIENT_DEMO					0//1
#define CLIENT_DEMO2				1//1	
#if CLIENT_DEMO2
	#define REMOTE_TEST				0//1
#endif

enum _SRVSTRT_TYP {
	_USE_OLD,
	_GNRT_NEW
};

enum _DIRECT_CONNTYPE {
	_TOSRV_,
	_M2M_,
	_G2G_,
};
extern int AY_SendDeviceStartToServer(Ui08 Filter);

#define PCKT_CNT_LEN			4






