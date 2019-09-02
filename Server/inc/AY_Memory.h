#pragma once
#include <AY_Type.h>
#include <AY_Socket.h>

#pragma pack(push, 1)

typedef union _UNIQUE_ID {
	Ui32		_UniqueL[3];
	Ui08		_Unique[12];
}UNIQUE_ID;

typedef struct _AY_DeviceStart {
	Ui32		_Test0;
	Ui32		_Test1;
	Ui32		_LocalCertNo;
	Ui32		_ServerCertNo;
	union {
		Ui08	_Input[256];
		struct {
			Ui08		_MAC[6];
			Ui08		_Unique[12];
			Ui08		_SessionKey[16];
			char		_Name[45];
			char		_Pswd[45];
			char		_Rsrvd[108];
			char		_RsaUsed[16];
		};
	};
	Ui08		_Sign[256];
}AY_DeviceStart;

typedef struct _AY_DeviceRead {
	union {
		Ui32		_id;					///< row number on this gateway ( bu gatewaydeki sira numrasi)
		struct {
			Ui16	_sport;					///< source port no for guest connection
			Ui16	_dport;					///< dest port no for guest connection
		};
	};
	Ui32		_Unique[3];				///< Gateway's Unique ID ( This gateway for local and remote gateway ID for remote type device)
	union {
		Ui32		_typ_prnt;
		struct {
			Ui32	_ParentId : 28;		///< row number on remote gateway for only remote type device
			Ui32	_Type : 4;			///< Real:0 Mirror:1
		};
	};
	Ui32		_LocalIp;				///< local IP address on this gateway ( allways IP on this gateway, not remote IP for remote type device)
}AY_DeviceRead;


typedef struct _AY_DevStrtRspHdr {
	Ui32		_Test0;
	Ui32		_Test1;
	Ui32		_DevPcktNo;
	Ui32		_DevCnt;
}AY_DeviceStartResp;
//-----------------------------------------------------//
typedef struct _AY_GWINFORQST {
	Ui32		_Test2;
	Ui32		_Test3;
	Ui32		_QueRowNo;
	union {
		Ui08	_InfoCont[32];
		struct {
			Ui08		_Unique[12];
			Ui32		SendCnt;
			Ui32		ReadCnt;
			Ui32		ErrCnt;
			//Ui32		reserved0;
			//Ui32		reserved1;
		};
	};
}AY_GWINFORQST;

typedef struct _AY_GWINFORESP {
	Ui32		_Test2;
	Ui32		_Test3;
	Ui32		_QueRowNo;
	union {
		Ui08	_InfoCont[64];
		struct {
			Ui32			_LastUpdateMin;
			Ui08			_SessionKey[16];
			udp_headerAll	_UDPh;//42 bytes
			//Ui16			reserved0;
		};
	};
}AY_GWINFORESP;

typedef struct _AY_GWRENTRQST {
	Ui32		_Test4;
	Ui32		_Test5;
	union {
		Ui08	_InfoCont[80];
		struct {
			Ui32			_LastUpdateMin;
			Ui08			_Unique[12];
			Ui08			_SessionKey[16];
			udp_headerAll	_UDPh;//42 bytes
			//Ui16			reserved0;
		};
	};
}AY_GWRENTRQST;

typedef struct _AY_GWDATAHDR {
	Ui32		_Test6;///< _Test8 for guest connecetion
	Ui32		_Test7;///< _Test9 for guest connecetion
	Ui32		_DevNoOnTrgt;///< DeviceNo for Mirror or Locap IP no for Guest Connection	
}AY_GWDATAHDR;

#pragma pack(pop)

enum _DEVTYPES {
	_REAL_,
	_MIRROR_,
	_GUEST_,
};

extern unsigned char	*_AY_MallocMemory(Ui32 Cnt);

extern void _AY_FreeMemory(unsigned char	*pBuff);

extern AY_DeviceRead	*_AY_MallocClientsMemory(Ui32 ClntCnt);

extern void _AY_FreeClientsMemory(AY_DeviceRead	*ptr);

/*
 * This will convert www.google.com to 3www6google3com
 * got it :)
 * */
extern void ChangetoDnsNameFormat(unsigned char* dns, unsigned char* host);

/*
 *
 * */
extern unsigned char* ReadName(unsigned char* reader, unsigned char* buffer, int* count);

extern int AY_Delay(Ui64 mSec);

