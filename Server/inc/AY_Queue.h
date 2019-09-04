#pragma once
#include <AY_Type.h>

#pragma pack(push, 1)

#define QUEUE_DEPTH				30
#define QUEUE_THREAD_ALLWAYS	5
#define QUEUE_STACK_SIZE		(16*1024)

typedef union _AY_QUEUE_FLGS {
	Ui32	_QFlgs;
	struct {
		Ui32	_QBusyF : 1;
		Ui32	_QFinishedF : 1;
		Ui32	_QKeepF : 1;
	};
}_AY_QUEUE_FLGS;

typedef struct _AY_QUEUE {
	_AY_QUEUE_FLGS		QFlg;
	Ui08				*pIn;
	Ui08				*pOut;
	Ui16				InLen;
	Ui16				OutLen;
	Ui08				Target;
	Ui08				Type;
}AY_QUEUE;


#pragma pack(pop)

enum MY_QTARGET_ {
	QTARGET_CLIENT_CONN = 0xA0,

};

enum MY_QTYPES_ {
	NOT_USED = 0,

};

extern	AY_QUEUE				AY_Queue[QUEUE_DEPTH];
extern	Ui16					AY_QueueIdx;

extern int AYSRV_QueueFindFirstFreeRow(void);
extern int AYSRV_QueueLoad(int ql, Ui08 *pIn, Ui16 InLen, Ui08 Target, Ui08 Type);
extern void AYSRV_QueueInit(void);

//============================================================//
//==============================================================//
#pragma pack(push, 1)
typedef union _UNIQ_Q_FLG {
	Ui08	AllFlgs;
	struct {
		Ui08		Full_ : 1;
		Ui08		Finished : 1;
	};
}UNIQ_Q_FLG;
typedef struct _AY_UNIQUE_QUEUE {///< 12 + 12 + 4  + 4 + 4 + 4- = 40 Bytes
	UNIQUE_ID		SrcUniq;
	UNIQUE_ID		DstUniq;
	Ui32			PrcsNo;
	Si32			TimeOut;
	UNIQ_Q_FLG		UniqQFlg;
	Ui08			UniqFnc;
	Ui16			DataLen;
	Ui08			*pData;
}AY_UNIQUE_QUEUE;
#define UNIQUE_QUEUE_LEN			4096
typedef struct  _AY_UNIQ_QUEUELST {///< 4096*36 = 144 KBytes
	AY_UNIQUE_QUEUE	UniqQ[UNIQUE_QUEUE_LEN];
}AY_UNIQ_QUEUELST;

#define UNIQUE_QUEUE_TIMEOUT		5000;

enum _UNIQUE_Q_FUNC {
	_UNIQUE_Q_UNDEF,
	_UNIQUE_Q_RENT,
};

#pragma pack(pop)

extern AY_UNIQ_QUEUELST	UniqQ_Lst;

/****************************************************************************/
/*! \fn int AYSRV_UniqQ_FindFirstFreeRow(void)
** \brief		        find first free row
*****************************************************************************/
extern int AYSRV_UniqQ_FindFirstFreeRow(void);

/****************************************************************************/
/*! \fn int AYSRV_UniqQ_Load( int ql, UNIQUE_ID Src, UNIQUE_ID Dst, Ui32 PrcsNo, Ui08 Func, Ui16 Len, Ui08 *pData )
** \brief		        load unique queue
*****************************************************************************/
extern int AYSRV_UniqQ_Load(int ql, UNIQUE_ID Src, UNIQUE_ID Dst, Ui32 PrcsNo, Ui08 Func, Ui16 Len, Ui08 *pData);

/****************************************************************************/
/*! \fn void AYSRV_UniqQ_InitAll(void)
** \brief		        initialize all
*****************************************************************************/
extern void AYSRV_UniqQ_InitAll(void);

/****************************************************************************/
/*! \fn void AYSRV_UniqQ_Init(int ql)
** \brief		        initialize row
*****************************************************************************/
extern void AYSRV_UniqQ_Init(int ql);

/****************************************************************************/
/*! \fn int AYSRV_FindUniqQ(UNIQUE_ID Src, UNIQUE_ID Dst, Ui08 Func)
** \brief		        find row no for input parameters
*****************************************************************************/
enum _UNIQ_TEST {
	_UNIQ_ALL = 0x00,
	_UNIQ_NOT_SRC = 0x40,
	_UNIQ_NOT_DST = 0x80,
};
extern int AYSRV_FindUniqQ(UNIQUE_ID Src, UNIQUE_ID Dst, Ui08 Func);


/****************************************************************************/
/*! \fn UNIQUE_ID AYSRV_ReadUniqQ(int _Id, int SrcOrDst)
** \brief		        read uniqQ row  - SrcOrDst	:	0:Source 1: Destination
*****************************************************************************/
extern UNIQUE_ID AYSRV_ReadUniqQ(int _Id, int SrcOrDst);


/****************************************************************************/
/*! \fn void AYSRV_TimeoutTestUniqQ(void)
** \brief		        timeout function for uniqQ queue members
*****************************************************************************/
extern void AYSRV_TimeoutTestUniqQ(void);

extern void TIMEOUT_Process(int ql);
