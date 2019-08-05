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
		Ui32	_SendServer : 1;
		Ui32	_RecvServer : 1;
		Ui32	_GenerateRemoteDevs : 1;
		Ui32	_ListenThreads : 1;

		Ui32	_DynamicIP : 1;
	};

}AY_FLGS;

typedef struct _AY_GlobalRAM {
	AY_FLGS			AY_Flgs;
	Ui32			AY_DeviceCnt;
	Ui16			AY_DevPcktNo;
	AY_DeviceRead	AY_DeviceList[256];
	Ui08			AY_Unique[12];
	Ui08			AY_Sessionkey[16];
}AY_GlobalRAM;

extern	AY_GlobalRAM	AY_Ram;

#define AY_Client_Flags					AY_Ram.AY_Flgs._Flgs
#define AY_Client_Intro					AY_Ram.AY_Flgs._IntroF
#define AY_Client_Initied				AY_Ram.AY_Flgs._ClientInited
#define AY_Client_GetMACadr				AY_Ram.AY_Flgs._GetMACadr
#define AY_Client_WaitMACadr			AY_Ram.AY_Flgs._WaitMACadr
#define AY_Client_GetSrvIPadr			AY_Ram.AY_Flgs._GetSrvIPadr
#define AY_Client_WaitSrvIPadr			AY_Ram.AY_Flgs._WaitSrvIPadr
#define AY_Client_SendServer			AY_Ram.AY_Flgs._SendServer
#define AY_Client_RecvServer			AY_Ram.AY_Flgs._RecvServer
#define AY_Client_GenerateRemoteDevs	AY_Ram.AY_Flgs._GenerateRemoteDevs
#define AY_Client_ListenThreads			AY_Ram.AY_Flgs._ListenThreads


#define AY_Client_DynamicIP				AY_Ram.AY_Flgs._DynamicIP


#pragma pack(pop)

#define CLIENT_DEMO					0//1
#define CLIENT_DEMO2				1				


