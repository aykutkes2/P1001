/******************************************************************************/
/*  Header  File :               DKRTU_Parser.h                               */
/******************************************************************************/
/*																			  */
/*																			  */
/*																			  */
/*	Parser Processes											  			  */
/*																			  */
/*							  						                          */
/*																			  */
/*  Author : Sinan                       									  */
/*	Date : 27-07-2019          												  */
/*	Copyright © 2019 Datakom Elektronik Muhendislik Ltd          			  */
/******************************************************************************/

#include <DK_Types.h>
#include "messages/DKRTU_MessageList.h"

#ifndef	_DKRTU_PARSER_H_
#define	_DKRTU_PARSER_H_


#ifndef MAX_MESSAGEHEADERSIZE
#define MAX_MESSAGEHEADERSIZE						100
#endif

#ifndef MAX_PARSERQUEUE_LENGTH
#define MAX_PARSERQUEUE_LENGTH						8
#endif

#ifndef MAX_MSGTRMNUMBER
#define MAX_MSGTRMNUMBER							5
#endif

#ifndef DKPARSERQUEUE_SLOT_TIMEOUT_VAL
#define DKPARSERQUEUE_SLOT_TIMEOUT_VAL				10000
#define DKPARSERQUEUE_SLOT_TIMEOUT_ERR				1000
#define DKPARSERQUEUE_SLOT_TIMEOUT_END				500
#endif

#define _DKPARSER_NULLPTR			0		///< null pointer

#ifdef __GNUC__
#pragma pack(4)
#else
#pragma pack(push, 4)
#endif

typedef union _PARSER_QUEUE_FLGS{
	Ui16	_OFlags;
	struct{
		Ui16	IsThisFull:1;
		Ui16	ModemNo:3;
		Ui16	OptionalTitle:1;
		//Ui16    CoreUse:1; //0:Parser, 1:Core
		//Ui16	WaitACK:1;
		Ui16	reserved:11;
	};
}PARSERQUEUE_FLGS;

typedef union _PARSERQUEUE_OPTCONTENTFLGS{
	Ui32	_QFlags;
	struct{
		Ui32	reserve:32;
	};
}PARSERQUEUE_OPTCONTENTFLGS;

typedef struct {
	Ui32								MsgHeader;
	Ui32								MsgTransID;
	Ui08								*pTrmData;
	Ui08								*pRcvData;
	Ui16								TrmDataLen;
	Ui16								RcvDataLen;
	Si16								TimeOutCntDown;		///< set to 5000 at the beginning
	PARSERQUEUE_OPTCONTENTFLGS			QOptContFlg;
	PARSERQUEUE_FLGS					QFlg;
	Ui08								State;
	Ui08								TrmCounter;
	Si08								Status;				///< core process status
	void								*pFinishedCallBack;
} PARSERQUEUE_TYPE;

typedef union _PARSERLOGIN_FLGS{
	Ui08	_PFlags;
	struct{
		Ui08	LoginComplete:1;
		Ui08	RSAKeyExist:1;
		Ui08	reserved:6;
	};
}PARSERLOGIN_FLGS;

typedef struct{
	const 	Ui08		ConfirmBuffer[16];
			Ui08		AESKey[16];
	const 	Ui08		AESIV[16];
	PARSERLOGIN_FLGS 	LoginFlgs;
} PARSER_LOGIN;

extern PARSER_LOGIN	DKRTU_Parser_LoginConfig;

#define DKRTU_LOGINCOMPLETEFLG		DKRTU_Parser_LoginConfig.LoginFlgs.LoginComplete
#define DKRTU_RSAKEYFLG				DKRTU_Parser_LoginConfig.LoginFlgs.RSAKeyExist
#define DKRTU_CONFIRMBUF			DKRTU_Parser_LoginConfig.ConfirmBuffer
#define DKRTU_AESKEY				DKRTU_Parser_LoginConfig.AESKey
#define DKRTU_AESIV					DKRTU_Parser_LoginConfig.AESIV

#ifdef __GNUC__
#pragma pack()
#else
#pragma pack(pop)
#endif


enum DKPARSERQUEUE_FUNC_STAT{
	_DKPARSER_FIRSTPROCESSOK				=	2,
	_DKPARSER_OK							=	1,
	_DKPARSERQUEUE_UNUSED					=	0,
	_DKPARSERQUEUE_ERR_OVERFLOW				=	-1,
	_DKPARSER_ERR_SENDNACK					=	-2,
	_DKPARSER_ERR_MSGNOTFOUND				=	-3,
	_DKPARSER_ERR_MISSINGMSG				=	-4,
};

enum DKPARSERQUEUE_STATUS{
	_DKPARSERQUEUE_ERROR					=	-3,
	_DKPARSERQUEUE_EMPTYSLOT				=	-2,
	_DKPARSERQUEUE_MSGNOTSEND				=	-1,
	_DKPARSERQUEUE_IDLE						=	0,
	_DKPARSERQUEUE_READY					=	1,
	_DKPARSERQUEUE_PROCESSING				=	2,
	_DKPARSERQUEUE_FINISHED					=	3,
};

extern PARSERQUEUE_TYPE DKPARSER_Queue[MAX_PARSERQUEUE_LENGTH];

/* Functions ------------------------------------------------------------------*/
extern Si16				 DKRTU_Parser_SearchQueue(Ui32 MsgHeader, Ui32 MsgTransID, Ui16 DataLen, Ui08 *pData);
extern PARSERQUEUE_TYPE *DKRTU_Parser_FindFirstFreeSlot(void);
extern void 			 DKRTU_Parser_SendNACK(S_RTU_bV0* pRecvMsg_source);
extern Si16 			 DKRTU_Parser_MsgCheck(S_RTU_bV1* pRecvMsg);
extern void 			 DKRTU_Parser_PushQueue(S_RTU_bV1* pRecvMsg, PARSERQUEUE_TYPE *pQ, Ui08 ModemType);


//=========================================================================================================//
//===================================== OBJECTS ===========================================================//
//=========================================================================================================//
enum _PARSER_OBJ_RET{
	_DKRTU_OBJ_ERR_NOTFOUND=-4,
	_DKRTU_OBJ_ERR_UNDEFOBJ=-3,
	_DKRTU_OBJ_ERR_FAMILY=-2,
	_DKRTU_OBJ_ERR_LENGTH=-1,
	_DKRTU_OBJ_NULL=0,
	_DKRTU_OBJ_OK=1,
	_DKRTU_OBJ_LVLEND=2,
	_DKRTU_OBJ_LAST=3
};
enum _PARSER_OBJ_FMLY{
	_MSTR = 0x4D535452,
	_KLTE = 0x4B4C5445,
	_SIEC = 0x53494543
};

typedef union _DKRTU_PARSOBJF{
	Ui08	_ParsObjF;
	struct {
		Ui08	_ParsObjOpt:1;///< 0: Mandatory 1: Optional
		Ui08	_ParsObjLvl:4;///< 0-15
	};
}DKRTU_PARSOBJF;
typedef struct _DKRTU_PARSOBJ{
	Ui08			_ObjOpt;///< 0: Mandatory 1: Optional
	Ui08			_ObjLvl;///< 0-15
	Ui16			_MsgId;
	//Ui16			_MsgLen;
}DKRTU_PARSOBJ;
enum _PARSER_OBJ_OPT{
	_OBJ_MAN,
	_OBJ_OPT
};
enum _PARSER_OBJ_LVL{
	OL_0,
	OL_1,
	OL_2,
	OL_3,
	OL_4,
	OL_5,
	OL_6,
	OL_7,
	OL_8,
	OL_9,
	OL_10,
	OL_11,
	OL_12,
	OL_13,
	OL_14,
	OL_15
};

#endif /* _DKRTU_PARSER_H_ */
