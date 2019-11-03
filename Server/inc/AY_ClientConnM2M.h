#pragma once
#include <AY_Type.h>

#pragma pack(push, 1)

typedef struct _AY_M2M_TYPE {
	AY_CONNTYPE		*_pConn;
	Ui32			_RowNo;
	Ui32			_SrcIP;
	Ui32			_DstIP;
	Ui16			_SrcPort;
	Ui16			_DstPort;
}AY_M2M_TYPE;

typedef struct  _AY_M2M_CONNTYPE {
	AY_M2M_TYPE		_Src;
	AY_M2M_TYPE		_Dst;
	//------------------------//
	Ui32			M_LastUpdateMin;
}AY_M2M_CONNTYPE;


#pragma pack(pop)

extern void AYSRV_QueueDirectSend(AY_QUEUE *pQ);

extern AY_M2M_CONNTYPE *pAYM2M_FindOrAddConn(AY_M2M_CONNTYPE *pTempM2M, Ui08 Func);

