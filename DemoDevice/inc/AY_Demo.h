#pragma once
#include <AY_Type.h>
#include <AY_Memory.h>

#pragma pack(push, 1)

typedef union _AY_FLGS {
	Ui32	_Flgs;
	struct {
		Ui32	_IntroF : 1;
		Ui32	_DemoInited : 1;
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

#define AY_Demo_Flags					AY_Ram.AY_Flgs._Flgs
#define AY_Demo_Intro					AY_Ram.AY_Flgs._IntroF
#define AY_Demo_Initied				AY_Ram.AY_Flgs._DemoInited
#define AY_Demo_GetMACadr				AY_Ram.AY_Flgs._GetMACadr
#define AY_Demo_WaitMACadr			AY_Ram.AY_Flgs._WaitMACadr
#define AY_Demo_GetSrvIPadr			AY_Ram.AY_Flgs._GetSrvIPadr
#define AY_Demo_WaitSrvIPadr			AY_Ram.AY_Flgs._WaitSrvIPadr
#define AY_Demo_SendServer			AY_Ram.AY_Flgs._SendServer
#define AY_Demo_RecvServer			AY_Ram.AY_Flgs._RecvServer
#define AY_Demo_GenerateRemoteDevs	AY_Ram.AY_Flgs._GenerateRemoteDevs
#define AY_Demo_ListenThreads			AY_Ram.AY_Flgs._ListenThreads


#define AY_Demo_DynamicIP				AY_Ram.AY_Flgs._DynamicIP
#define AY_DynamicIP					AY_Demo_DynamicIP

#pragma pack(pop)

#define Demo_DEMO					0//1
#define Demo_DEMO2				1				

#define AYCMD_SystemWrite(pSys)				(1)//system(pSys)

