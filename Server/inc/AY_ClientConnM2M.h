#pragma once
#include <AY_Type.h>
#include <AY_ClientConn.h>

#pragma pack(push, 1)

typedef struct _AY_M2M_TYPE {
	AY_CONNTYPE		*_pConn;
	Ui32			_DevNo;
	//Ui32			_SrcIP;
	Ui32			_DstIP;
	//Ui16			_SrcPort;
	//Ui16			_DstPort;
}AY_M2M_TYPE;

typedef struct  _AY_M2M_CONNTYPE {
	AY_M2M_TYPE		_Src;
	AY_M2M_TYPE		_Dst;
	//------------------------//
	Ui32			TempData;
	Ui32			M_LastUpdateMin;
}AY_M2M_CONNTYPE;


#pragma pack(pop)

extern void AYSRV_QueueDirectSend(AY_QUEUE *pQ);

extern void AYSRV_QueueDirectResponse(AY_QUEUE *pQ);

extern AY_M2M_CONNTYPE *pAYM2M_FindOrAddConn(AY_M2M_CONNTYPE *pTempM2M, Ui32 *pId, Ui08 Func);

extern Ui32 AYM2M_ReadConn(Ui32 ConnId, AY_M2M_CONNTYPE *pConnRd);

extern AY_M2M_CONNTYPE	*pAYM2M_ReadConn(Ui32 ConnId);

extern AY_M2M_CONNTYPE	*pAYM2M_FindConnByUniqueID(UNIQUE_ID *pUnique, Ui08 SrcDst);


extern void AYSRV_QueueSendRequestM2M(AY_QUEUE *pQ);

extern void AYSRV_QueueSendResponseM2M(AY_QUEUE *pQ);


