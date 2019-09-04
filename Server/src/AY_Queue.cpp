//#include "pch.h"
#undef UNICODE


#include<iostream>
#include<cstdio>
#include<fstream>
#include<sstream>
#include<string>
#include<cstdlib>
#include<conio.h>
#include<windows.h>

#include <stdio.h>
#include <time.h>
#include <AY_Printf.h>
#include <AY_Memory.h>
#include <AY_Socket.h>
#include <AY_Queue.h>
#include <AY_ClientConn.h>


#include <process.h>

AY_QUEUE				AY_Queue[QUEUE_DEPTH];
Ui16					AY_QueueIdx = 0;

void QUEUE_Process(int ql) {
	AY_QUEUE *pQ;
	if (ql >= QUEUE_DEPTH) { ql = 0; }
	pQ = &AY_Queue[ql];
	while (1) {
		switch (pQ->Target) {
		case QTARGET_CLIENT_CONN:
			AYSRV_QueueClientConn(pQ);
			break;
		}
		if ((pQ->QFlg._QFinishedF == 1) && (pQ->QFlg._QKeepF == 0) && (pQ->QFlg._QBusyF == 1)) {
			if (pQ->InLen) {
				_AY_FreeMemory(pQ->pIn);///< release input buffer
			}
			if (pQ->OutLen) {
				_AY_FreeMemory(pQ->pOut);///< release output buffer
			}
			memset((Ui08 *)pQ, 0, sizeof(AY_QUEUE));///< clear queue row
		}
		if (ql >= QUEUE_THREAD_ALLWAYS) {
			break;///*goto _EndThread_;//*/ _endthread();///< end thread
		}
		Sleep(1);
	}
//_EndThread_:

}

void TIMEOUT_Process(int ql) {
	while (1) {
		TimeoutDoTask(ql);
		Sleep(1);
	}
}

#if (QUEUE_DEPTH>0)
void QUEUE_Thread_0(void *pParams) { printf("THREAD=0 In thread function \n"); QUEUE_Process(0);	printf("THREAD=0 Thread function ends \n");	_endthread();}
void QUEUE_Thread_1(void *pParams) { printf("THREAD=1 In thread function \n"); QUEUE_Process(1);	printf("THREAD=1 Thread function ends \n");	_endthread(); }
void QUEUE_Thread_2(void *pParams) { printf("THREAD=2 In thread function \n"); QUEUE_Process(2);	printf("THREAD=2 Thread function ends \n");	_endthread(); }
void QUEUE_Thread_3(void *pParams) { printf("THREAD=3 In thread function \n"); QUEUE_Process(3);	printf("THREAD=3 Thread function ends \n");	_endthread(); }
void QUEUE_Thread_4(void *pParams) { printf("THREAD=4 In thread function \n"); QUEUE_Process(4);	printf("THREAD=4 Thread function ends \n");	_endthread(); }
void QUEUE_Thread_5(void *pParams) { printf("THREAD=5 In thread function \n"); QUEUE_Process(5);	printf("THREAD=5 Thread function ends \n");	_endthread(); }
void QUEUE_Thread_6(void *pParams) { printf("THREAD=6 In thread function \n"); QUEUE_Process(6);	printf("THREAD=6 Thread function ends \n");	_endthread(); }
void QUEUE_Thread_7(void *pParams) { printf("THREAD=7 In thread function \n"); QUEUE_Process(7);	printf("THREAD=7 Thread function ends \n");	_endthread(); }
void QUEUE_Thread_8(void *pParams) { printf("THREAD=8 In thread function \n"); QUEUE_Process(8);	printf("THREAD=8 Thread function ends \n");	_endthread(); }
void QUEUE_Thread_9(void *pParams) { printf("THREAD=9 In thread function \n"); QUEUE_Process(9);	printf("THREAD=9 Thread function ends \n");	_endthread(); }
#endif
#if (QUEUE_DEPTH>10)
void QUEUE_Thread_10(void *pParams) { printf("THREAD=10 In thread function \n"); QUEUE_Process(10);	printf("THREAD=10 Thread function ends \n");	_endthread(); }
void QUEUE_Thread_11(void *pParams) { printf("THREAD=11 In thread function \n"); QUEUE_Process(11);	printf("THREAD=11 Thread function ends \n");	_endthread(); }
void QUEUE_Thread_12(void *pParams) { printf("THREAD=12 In thread function \n"); QUEUE_Process(12);	printf("THREAD=12 Thread function ends \n");	_endthread(); }
void QUEUE_Thread_13(void *pParams) { printf("THREAD=13 In thread function \n"); QUEUE_Process(13);	printf("THREAD=13 Thread function ends \n");	_endthread(); }
void QUEUE_Thread_14(void *pParams) { printf("THREAD=14 In thread function \n"); QUEUE_Process(14);	printf("THREAD=14 Thread function ends \n");	_endthread(); }
void QUEUE_Thread_15(void *pParams) { printf("THREAD=15 In thread function \n"); QUEUE_Process(15);	printf("THREAD=15 Thread function ends \n");	_endthread(); }
void QUEUE_Thread_16(void *pParams) { printf("THREAD=16 In thread function \n"); QUEUE_Process(16);	printf("THREAD=16 Thread function ends \n");	_endthread(); }
void QUEUE_Thread_17(void *pParams) { printf("THREAD=17 In thread function \n"); QUEUE_Process(17);	printf("THREAD=17 Thread function ends \n");	_endthread(); }
void QUEUE_Thread_18(void *pParams) { printf("THREAD=18 In thread function \n"); QUEUE_Process(18);	printf("THREAD=18 Thread function ends \n");	_endthread(); }
void QUEUE_Thread_19(void *pParams) { printf("THREAD=19 In thread function \n"); QUEUE_Process(19);	printf("THREAD=19 Thread function ends \n");	_endthread(); }
#endif
#if (QUEUE_DEPTH>20)
void QUEUE_Thread_20(void *pParams) { printf("THREAD=20 In thread function \n"); QUEUE_Process(20);	printf("THREAD=20 Thread function ends \n");	_endthread(); }
void QUEUE_Thread_21(void *pParams) { printf("THREAD=21 In thread function \n"); QUEUE_Process(21);	printf("THREAD=21 Thread function ends \n");	_endthread(); }
void QUEUE_Thread_22(void *pParams) { printf("THREAD=22 In thread function \n"); QUEUE_Process(22);	printf("THREAD=22 Thread function ends \n");	_endthread(); }
void QUEUE_Thread_23(void *pParams) { printf("THREAD=23 In thread function \n"); QUEUE_Process(23);	printf("THREAD=23 Thread function ends \n");	_endthread(); }
void QUEUE_Thread_24(void *pParams) { printf("THREAD=24 In thread function \n"); QUEUE_Process(24);	printf("THREAD=24 Thread function ends \n");	_endthread(); }
void QUEUE_Thread_25(void *pParams) { printf("THREAD=25 In thread function \n"); QUEUE_Process(25);	printf("THREAD=25 Thread function ends \n");	_endthread(); }
void QUEUE_Thread_26(void *pParams) { printf("THREAD=26 In thread function \n"); QUEUE_Process(26);	printf("THREAD=26 Thread function ends \n");	_endthread(); }
void QUEUE_Thread_27(void *pParams) { printf("THREAD=27 In thread function \n"); QUEUE_Process(27);	printf("THREAD=27 Thread function ends \n");	_endthread(); }
void QUEUE_Thread_28(void *pParams) { printf("THREAD=28 In thread function \n"); QUEUE_Process(28);	printf("THREAD=28 Thread function ends \n");	_endthread(); }
void QUEUE_Thread_29(void *pParams) { printf("THREAD=29 In thread function \n"); QUEUE_Process(29);	printf("THREAD=29 Thread function ends \n");	_endthread(); }
#endif

const void *pQUEUE_ThreadAddress[QUEUE_DEPTH] = {
#if (QUEUE_DEPTH>0)
	QUEUE_Thread_0,QUEUE_Thread_1,QUEUE_Thread_2,QUEUE_Thread_3,QUEUE_Thread_4,QUEUE_Thread_5,QUEUE_Thread_6,QUEUE_Thread_7,QUEUE_Thread_8,QUEUE_Thread_9
#endif
#if (QUEUE_DEPTH>10)
	,QUEUE_Thread_10,QUEUE_Thread_11,QUEUE_Thread_12,QUEUE_Thread_13,QUEUE_Thread_14,QUEUE_Thread_15,QUEUE_Thread_16,QUEUE_Thread_17,QUEUE_Thread_18,QUEUE_Thread_19
#endif
#if (QUEUE_DEPTH>20)
	,QUEUE_Thread_20,QUEUE_Thread_21,QUEUE_Thread_22,QUEUE_Thread_23,QUEUE_Thread_24,QUEUE_Thread_25,QUEUE_Thread_26,QUEUE_Thread_27,QUEUE_Thread_28,QUEUE_Thread_29
#endif
};

int AYSRV_QueueFindFirstFreeRow(void) {
	int i;
	for (i = 0; i < QUEUE_DEPTH; i++) {
		if (AY_Queue[i].QFlg._QBusyF == 0) {
			return i;
		}
	}
	return -1;
}

int AYSRV_QueueLoad(int ql,Ui08 *pIn,Ui16 InLen,Ui08 Target,Ui08 Type) {
	HANDLE hThread;
	//int i;

	if ((ql >= QUEUE_DEPTH)||(ql < 0)) { 
		printf("AYQUEUE--> Depth Fail ql = %d \n", ql);
		return -1; 
	}
	if (AY_Queue[ql].QFlg._QBusyF != 0) {
		printf("AYQUEUE--> Fail it is full ql = %d \n", ql);
		return -2; 
	}
	memset((Ui08 *)&AY_Queue[ql], 0, sizeof(AY_QUEUE));

	AY_Queue[ql].QFlg._QBusyF	= 1;
	AY_Queue[ql].InLen			= InLen;
	AY_Queue[ql].pIn			= pIn;
	AY_Queue[ql].Target			= Target;
	AY_Queue[ql].Type			= Type;

	printf("AYQUEUE--> ql = %d\n InLen = %d\n pIn = %d\n Target = %d\n Type = %d\n", ql, InLen, (unsigned int)pIn, Target, Type);

	if (ql >= QUEUE_THREAD_ALLWAYS) {
		hThread = (HANDLE)_beginthread((_beginthread_proc_type)pQUEUE_ThreadAddress[ql], QUEUE_STACK_SIZE, 0);
	}	
	return ql;
}
void AYSRV_QueueInit(void) {
	HANDLE hThread;
	int i;
	memset((Ui08 *)&AY_Queue[0], 0, sizeof(AY_Queue));
	for (i = 0; i < QUEUE_THREAD_ALLWAYS; i++) {
		hThread = (HANDLE)_beginthread((_beginthread_proc_type)pQUEUE_ThreadAddress[i], QUEUE_STACK_SIZE, 0);
	}	
}

//========================================================//
//========== UNIQUE QUEUE =================================//
AY_UNIQ_QUEUELST	UniqQ_Lst;

/****************************************************************************/
/*! \fn int AYSRV_UniqQ_FindFirstFreeRow(void)
**
** \brief		        find first free row 
**
** \param    			-
**
** \return				0-UNIQUE_QUEUE_LEN	: Row
** 						-					: there is something wrong
**
*****************************************************************************/
int AYSRV_UniqQ_FindFirstFreeRow(void) {
	int i;
	for (i = 0; i < UNIQUE_QUEUE_LEN; i++) {
		if (UniqQ_Lst.UniqQ[i].UniqQFlg.Full_ == 0) {
			return i;
		}
	}
	return -1;
}

/****************************************************************************/
/*! \fn int AYSRV_UniqQ_Load( int ql, UNIQUE_ID Src, UNIQUE_ID Dst, Ui32 PrcsNo, Ui08 Func, Ui16 Len, Ui08 *pData )
**
** \brief		        load unique queue
**
** \param    			ql			:	row no
**						Src			:	source GW unique id
**						Dst			:	destination GW unique id
**						PrcsNo		:	process no ( RowNo)
**						Func		:	function
**						Len			:	data length
**						pData		:	data start address
**
** \return				0-UNIQUE_QUEUE_LEN	: Row
** 						-					: there is something wrong
**
*****************************************************************************/
int AYSRV_UniqQ_Load( int ql, UNIQUE_ID Src, UNIQUE_ID Dst, Ui32 PrcsNo, Ui08 Func, Ui16 Len, Ui08 *pData ) {
	
	if ((ql >= UNIQUE_QUEUE_LEN) || (ql < 0)) {
		printf("AYQUEUE--> UNIQUE_QUEUE Depth Fail ql = %d \n", ql);
		return -1;
	}
	if (UniqQ_Lst.UniqQ[ql].UniqQFlg.Full_ != 0) {
		printf("AYQUEUE--> UNIQUE_QUEUE Fail it is full ql = %d \n", ql);
		return -2;
	}
	memset((Ui08 *)&UniqQ_Lst.UniqQ[ql], 0, sizeof(AY_UNIQUE_QUEUE));

	UniqQ_Lst.UniqQ[ql].UniqQFlg.Full_ = 1;
	UniqQ_Lst.UniqQ[ql].SrcUniq = Src;
	UniqQ_Lst.UniqQ[ql].DstUniq = Dst;
	UniqQ_Lst.UniqQ[ql].PrcsNo = PrcsNo;
	UniqQ_Lst.UniqQ[ql].UniqFnc = Func;
	UniqQ_Lst.UniqQ[ql].DataLen = Len;
	UniqQ_Lst.UniqQ[ql].pData = pData;
	UniqQ_Lst.UniqQ[ql].TimeOut = UNIQUE_QUEUE_TIMEOUT;

	printf("AYQUEUE--> UNIQUE_QUEUE ql =%d\n Func = %d\n Src =  0x%08x , 0x%08x, 0x%08x\n Dst =  0x%08x , 0x%08x, 0x%08x\n", ql, Func, Src._UniqueL[0], Src._UniqueL[1], Src._UniqueL[2], Dst._UniqueL[0], Dst._UniqueL[1], Dst._UniqueL[2]);
		
	return ql;
}

/****************************************************************************/
/*! \fn void AYSRV_UniqQ_InitAll(void)
**
** \brief		        initialize all
**
** \param    			-
**
** \return				-
**
*****************************************************************************/
void AYSRV_UniqQ_InitAll(void) {
	int i;
	for (i = 0; i < UNIQUE_QUEUE_LEN; i++) {
		memset((Ui08 *)&UniqQ_Lst.UniqQ[i], 0, sizeof(AY_UNIQUE_QUEUE));
		UniqQ_Lst.UniqQ[i].TimeOut = UNIQUE_QUEUE_TIMEOUT;
	}	
}

/****************************************************************************/
/*! \fn void AYSRV_UniqQ_Init(int ql)
**
** \brief		        initialize row
**
** \param    			ql			:	row no
**
** \return				-
**
*****************************************************************************/
void AYSRV_UniqQ_Init(int ql) {
	memset((Ui08 *)&UniqQ_Lst.UniqQ[ql], 0, sizeof(AY_UNIQUE_QUEUE));
	UniqQ_Lst.UniqQ[ql].TimeOut = UNIQUE_QUEUE_TIMEOUT;
}

/****************************************************************************/
/*! \fn int AYSRV_FindUniqQ(UNIQUE_ID Src, UNIQUE_ID Dst, Ui08 Func)
**
** \brief		        find row no for input parameters
**
** \param    			Src			:	source GW unique id
**						Dst			:	destination GW unique id
**						Func		:	function 0x40 --> don't check dest.  0x80 --> don't check src.
**
** \return				0-UNIQUE_QUEUE_LEN	: Row
** 						-					: there is something wrong
**
*****************************************************************************/
int AYSRV_FindUniqQ(UNIQUE_ID Src, UNIQUE_ID Dst, Ui08 Func) {
	int i;
	for (i = 0; i < UNIQUE_QUEUE_LEN; i++) {
		if (UniqQ_Lst.UniqQ[i].UniqQFlg.Full_) {
			if ( (memcmp(&UniqQ_Lst.UniqQ[i].DstUniq, &Dst, sizeof(AY_UNIQUE_QUEUE)) == 0) || (Func & _UNIQ_NOT_DST) ){
				if ( (memcmp(&UniqQ_Lst.UniqQ[i].SrcUniq, &Src, sizeof(AY_UNIQUE_QUEUE)) == 0) || (Func & _UNIQ_NOT_SRC) ) {
					return i;
				}
			}
		}
	}
	return -1;
}

/****************************************************************************/
/*! \fn UNIQUE_ID AYSRV_ReadUniqQ(int _Id, int SrcOrDst)
**
** \brief		        read uniqQ row
**
** \param    			_Id			:	id
**						SrcOrDst	:	0:Source 1: Destination
**
** \return				UNIQUE_ID	: Unique ID
**
*****************************************************************************/
UNIQUE_ID AYSRV_ReadUniqQ(int _Id, int SrcOrDst) {	
	if (SrcOrDst == 0) {
		return UniqQ_Lst.UniqQ[_Id].SrcUniq;
	} 
	else {
		return UniqQ_Lst.UniqQ[_Id].DstUniq;
	}
}

/****************************************************************************/
/*! \fn void AYSRV_TimeoutTestUniqQ(void)
**
** \brief		        timeout function for uniqQ queue members
**
** \param    			-
**
** \return				-
**
*****************************************************************************/
Ui08	UniqQ_TimeOut_Task = 0;
#define UNIQQ_MAX_TASK				((UNIQUE_QUEUE_LEN+31)/32)	
#define UNIQQ_TASK_CNT				32
void AYSRV_TimeoutTestUniqQ(void) {
	int i,j;
	j = (UniqQ_TimeOut_Task* UNIQQ_TASK_CNT);
	for (i = j; i < (j+ UNIQQ_TASK_CNT); i++) {
		if (UniqQ_Lst.UniqQ[i].UniqQFlg.Full_) {
			if (UniqQ_Lst.UniqQ[i].TimeOut) {
				UniqQ_Lst.UniqQ[i].TimeOut--;
			}
			else {
				AYSRV_UniqQ_Init(i);
			}
		}
	}
	UniqQ_TimeOut_Task++;
	if (UniqQ_TimeOut_Task >= UNIQQ_MAX_TASK) {
		UniqQ_TimeOut_Task = 0;
	}
}





