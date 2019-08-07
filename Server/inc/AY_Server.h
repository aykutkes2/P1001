#pragma once
#include <AY_Type.h>

#pragma pack(push, 1)

typedef union _AY_FLGS {
	Ui32	_Flgs;
	struct {
		Ui32	_IntroF : 1;
		Ui32	_ClientInited : 1;
		Ui32	_GetMACadr : 1;
		Ui32	_WaitMACadr : 1;
		Ui32	_SetFilter : 1;
		Ui32	_StartSqlConn: 1;

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
	AY_FLGS		AY_Flgs;
	Ui08			AY_Unique[12];
}AY_GlobalRAM;

extern	AY_GlobalRAM			AY_Ram;
//extern	MySqlConnect			MySqlConn;
extern	ip_address				MyIP_Address;
extern	uip_eth_addr			MyEth_Address;

#define AY_Server_Flags					AY_Ram.AY_Flgs._Flgs
#define AY_Server_Intro					AY_Ram.AY_Flgs._IntroF
#define AY_Server_Initied				AY_Ram.AY_Flgs._ClientInited
#define AY_Server_GetMACadr				AY_Ram.AY_Flgs._GetMACadr
#define AY_Server_WaitMACadr			AY_Ram.AY_Flgs._WaitMACadr
#define AY_Server_SetFilter				AY_Ram.AY_Flgs._SetFilter
#define AY_Server_StartSqlConn			AY_Ram.AY_Flgs._StartSqlConn


#define AY_Server_GetSrvIPadr			AY_Ram.AY_Flgs._GetSrvIPadr
#define AY_Server_WaitSrvIPadr			AY_Ram.AY_Flgs._WaitSrvIPadr
#define AY_Server_SendServer			AY_Ram.AY_Flgs._SendServer
#define AY_Server_RecvServer			AY_Ram.AY_Flgs._RecvServer
#define AY_Server_GenerateRemoteDevs	AY_Ram.AY_Flgs._GenerateRemoteDevs
#define AY_Server_ListenThreads			AY_Ram.AY_Flgs._ListenThreads

#define AY_Server_DynamicIP				AY_Ram.AY_Flgs._DynamicIP
#define AY_DynamicIP					AY_Server_DynamicIP

#pragma pack(pop)

#define CLIENT_DEMO					1


