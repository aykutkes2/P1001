/******************************************************************************/
/*  Header  File :               DKRTU_MsgObjects.h                                */
/******************************************************************************/
/*																			  */
/*																			  */
/*																			  */
/*	Message Objects Processes								  				  */
/*																			  */
/*							  						                          */
/*																			  */
/*  Author : Aykut                       									  */
/*	Date : 01-08-2019          												  */
/*	Copyright © 2019 Datakom Elektronik Muhendislik Ltd          			  */
/******************************************************************************/

#ifndef __DKRTU_OBJECTS_H
#define __DKRTU_OBJECTS_H

#if defined(_WIN32) || defined(EFIX64) || defined(EFI32)
#include <AY_Type.h>
#else
#include <DK_Type.h>
#endif

#define DKRTU_OBJ_NULLPTR		0

#define DKRTU_PARSOBJ_TEST		1

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

typedef struct _DKRTU_OBJ_NEXT{
	DKRTU_PARSOBJ 	*pNo;
	Ui08 			*pNa;
	Si08			RetVal;
}DKRTU_OBJ_NEXT;

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

#if DKRTU_PARSOBJ_TEST
extern void DKRTU_ParsObj_Test(void);
#endif

#endif
