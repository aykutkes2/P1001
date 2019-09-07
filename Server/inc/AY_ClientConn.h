#pragma once
#include <AY_Type.h>

#pragma pack(push, 8)

typedef struct _AY_DEVSTRTIN {
	udp_headerAll	_UDPh;
	Ui32			_LocalCertNo;
	Ui32			_ServerCertNo;
	Ui08			_MAC[6];
	Ui08			_Unique[12];
	Ui08			_SessionKey[16];
	char			_Name[45];
	char			_Pswd[45];
}AY_DEVSTRTIN;

typedef struct _AY_CONNADR  {
	void *pConnAdr[256];
}AY_CONNADR;
typedef struct  _AY_CONNTYPE {
	Ui32			_UnqiueId[3];
	Ui32			_LastUpdateMin;
	Ui08			_SessionKey[16];
	udp_headerAll	_UDPh;
}AY_CONNTYPE;

enum AY_CONNFUNC {
	AY_CONN_UPDATE = 0,
	AY_CONN_DELETE = 1,
	AY_CONN_FIND = 2,
	AY_CONN_READ = 3
};



#pragma pack(pop)


extern void AYSRV_QueueClientConn(AY_QUEUE *pQ);

extern int AY_TestLoadDeviceStart(Ui08 *pPtr, Ui16 Len);
extern int AY_TestLoadGwInfoRqst(Ui08 *pPtr, Ui16 Len);

//============================================================================================================
//====================== CONNECTIONS =========================================================================
#define AYCONN_TIMEOUT_SLEEP_mSEC		(30*60*1000)///< 30 minutes
extern Ui32	ConnectionCount;
extern Ui32 AYCONN_ThisMinute(void);
extern int AYCONN_TestMinute(Ui32 RecMin, Ui32 TimeOut);
extern Ui32 AYCONN_FindOrAddConn(Ui32 Unique0, Ui32 Unique1, Ui32 Unique2, udp_headerAll *pUDP, Ui08 *pSSK, Ui08 Func);
extern Ui32 AYCONN_ReadConn(Ui32 ConnId, AY_CONNTYPE *pConnRd);
extern AY_CONNTYPE	*pAYCONN_ReadConn(Ui32 ConnId);
extern AY_CONNTYPE	*pFindConnByUDPheader(udp_headerAll *pUDP);
extern AY_CONNTYPE	*pFindConnByUniqueID(UNIQUE_ID *pUnique);
extern int AYCONN_UpdateTime(Ui32 ConnId);
extern int AYCONN_ClientConnInit(void);
extern void TimeoutDoTask(int ql);

//----------test sil !!!!
extern void AYSRV_TestMySql(void);


