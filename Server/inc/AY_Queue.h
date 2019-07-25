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