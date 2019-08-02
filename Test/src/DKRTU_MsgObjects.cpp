/******************************************************************************/
/*  Source  File :                DKRTU_MsgObjects.c                          */
/******************************************************************************/
/*																			  */
/*																			  */
/*																			  */
/*	Message Objects Processes											  	  */
/*																			  */
/*							  						                          */
/*																			  */
/*  Author : Aykut                       									  */
/*	Date : 01-08-2019          												  */
/*	Copyright © 2019 Datakom Elektronik Muhendislik Ltd          			  */
/******************************************************************************/

//#ifndef __DKRTU_OBJECTS_C
//#define __DKRTU_OBJECTS_C

#if defined(_WIN32) || defined(EFIX64) || defined(EFI32)
#include "pch.h"
#undef UNICODE

#include <AY_Type.h>
#else
#include <DK_Types.h>
#endif
#include <messages/DKRTU_OD.h>
#include <DKRTU_MsgObjects.h>

#if defined(_WIN32) || defined(EFIX64) || defined(EFI32)
//#include <windows.h>
//#include <stdio.h>
//#define DK_Printf(n)			1//printf(n)
#else
//#define DK_Printf((n))			1
#endif

typedef		int(*PFO)(char *pBuf, int Len, int Obj);	// Function Pointer...
/* ------------------------------------------------------------------------- */
//=================================================================================================================================//
//=========================================================================================================//
//===================================== OBJECTS ===========================================================//
//=========================================================================================================//


#define DKRTU_PARSOBJ_DEMO		1
#if DKRTU_PARSOBJ_DEMO
const DKRTU_PARSOBJ DKRTU_MSTR_40101100_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_MSTR_40102000_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_MSTR_40102010_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_MSTR_40205100_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_MSTR_40205110_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_MSTR_40205200_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_MSTR_40205202_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_MSTR_402052AA_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_MSTR_40205300_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_MSTR_40205400_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_MSTR_40205700_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_MSTR_40303010_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_MSTR_40504303_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_MSTR_40504700_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_MSTR_40805600_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_MSTR_40D0300D_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_MSTR_40D0420D_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_MSTR_40D0430D_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_MSTR_40D0440D_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_MSTR_40D0560D_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_MSTR_40D0470D_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_MSTR_50101010_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_MSTR_50104110_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_MSTR_50205000_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_MSTR_50303000_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_MSTR_50404202_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_MSTR_50504201_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_MSTR_505042AA_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_MSTR_50504400_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_MSTR_50504510_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_MSTR_50804600_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_MSTR_55131000_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_MSTR_55121000_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_MSTR_55111000_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
//const DKRTU_PARSOBJ DKRTU_MSTR_55101000_OBJTABLE[] ={{0xAA,0x55,0xA55A}};
const DKRTU_PARSOBJ DKRTU_MSTR_606043A0_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_MSTR_60707000_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
//===================================================================================//
const DKRTU_PARSOBJ DKRTU_KLTE_60501000_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_KLTE_70D010D0_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_KLTE_60503000_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_KLTE_60503010_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_KLTE_6050300D_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_KLTE_70202001_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_KLTE_60503001_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_KLTE_60503003_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_KLTE_60503004_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_KLTE_60503007_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_KLTE_6050300E_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_KLTE_60503008_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_KLTE_70604001_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_KLTE_60503002_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_KLTE_605030A0_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_KLTE_70202002_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_KLTE_605030B0_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_KLTE_6050300F_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_KLTE_70202003_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_KLTE_60503100_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_KLTE_60503005_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_KLTE_70604002_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_KLTE_60503009_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
//===================================================================================//
const DKRTU_PARSOBJ DKRTU_SIEC_40300010_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_SIEC_40305010_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_SIEC_40300020_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_SIEC_40D000D0_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_SIEC_50300040_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_SIEC_50305000_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_SIEC_50300030_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_SIEC_60501030_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_SIEC_60501000_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_SIEC_60501100_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_SIEC_60501040_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_SIEC_60501020_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_SIEC_60501010_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_SIEC_70602000_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_SIEC_70602100_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
const DKRTU_PARSOBJ DKRTU_SIEC_80300050_OBJTABLE[] = { {0xAA,0x55,0xA55A} };
//===================================================================================//
#endif

const DKRTU_PARSOBJ DKRTU_HEADER_OBJTABLE[] = {
	{_OBJ_MAN,OL_0,0x5510},///< main
	//----------------------------------------------
		{_OBJ_MAN,OL_1,0x4FF0},///< header
		//----------------------------------------------
			{_OBJ_MAN,OL_2,0x106E},///< Komut Seti Kimlik Kodu
			{_OBJ_MAN,OL_2,0x1001},///< Komut Seti Versiyonu
			{_OBJ_MAN,OL_2,0x1002},///< Aksiyon Kodu
			{_OBJ_MAN,OL_2,0x1003},///< Mesaj Dizisi Kimlik Kodu
			{_OBJ_MAN,OL_2,0x1004},///< Mesaj Kimlik Kodu
			{_OBJ_MAN,OL_2,0x2002},///< Ýþletim Tipi
			{_OBJ_OPT,OL_2,0x2001},///< Ýþletim Önceliði
			{_OBJ_MAN,OL_2,0x1006},///< Üst Sistem Kodu
			{_OBJ_MAN,OL_2,0x2006},///< Kullanýcý Rolü
			{_OBJ_MAN,OL_2,0x1008},///< Mesaj Oluþturma Tarihi
			{_OBJ_MAN,OL_2,0x104B},///< Haberleþme Ünitesi Kimlik Kodu
		//----------------------------------------------
	//----------------------------------------------
		{_OBJ_MAN,OL_1,0x4FFF},///< content
		//----------------------------------------------

		//----------------------------------------------
	//----------------------------------------------
	{0xAA,0x55,0xA55A}///< end
};

const DKRTU_PARSOBJ DKRTU_MSTR_55101000_OBJTABLE[] = {
	//----------------------------------------------
		{_OBJ_MAN,OL_2,0x1022},///< Cihaz Tarih-saat
		{_OBJ_MAN,OL_2,0x800D},///< Ýletiþim Kalite Bilgisi Listesi
		//----------------------------------------------
			{_OBJ_MAN,OL_3,0x4002},///< Ýletiþim Kalite Bilgisi Nesnesi
			//----------------------------------------------
				{_OBJ_MAN,OL_4,0x2016},///< Baðlantý Tipi
				{_OBJ_MAN,OL_4,0x1057},///< Kalite Deðeri
				{_OBJ_MAN,OL_4,0x1058},///< Kalite Baz Deðeri
				{_OBJ_OPT,OL_4,0x8015},///< Kalite Ek Deðerleri Listesi
				//----------------------------------------------
					{_OBJ_MAN,OL_5,0x4017},///< Kalite Ek Deðerleri Nesnesi
					//----------------------------------------------
						{_OBJ_MAN,OL_6,0x105F},///< Anahtar
						{_OBJ_MAN,OL_6,0x1060},///< Deðer
					//----------------------------------------------
				//----------------------------------------------
			//----------------------------------------------
		//----------------------------------------------
		{_OBJ_OPT,OL_2,0x4007},///< Lokasyon Bilgisi
		//----------------------------------------------
			{_OBJ_MAN,OL_3,0x200E},///< Koordinat Kaynaðý
			{_OBJ_MAN,OL_3,0x1033},///< Enlem
			{_OBJ_MAN,OL_3,0x1034},///< Boylam
			{_OBJ_OPT,OL_3,0x1035},///< Lokasyon Açýklamasý
			{_OBJ_OPT,OL_3,0x8012},///< Konuma Özel Alanlar Listesi
			//----------------------------------------------
				{_OBJ_MAN,OL_4,0x4017},///< Konuma Özel Alanlar Nesnesi
				//----------------------------------------------
					{_OBJ_MAN,OL_5,0x105F},///< Anahtar
					{_OBJ_MAN,OL_5,0x1060},///< Deðer
				//----------------------------------------------
			//----------------------------------------------
		//----------------------------------------------
		{_OBJ_OPT,OL_2,0x8002},///< Ekipman Kimlik Kodu Listesi
		//----------------------------------------------
			{_OBJ_MAN,OL_3,0x1017},///< Ekipman Kimlik Kodu
		//----------------------------------------------
		{_OBJ_MAN,OL_2,0x800E},///< Baðlantý Bilgileri Listesi
		//----------------------------------------------
			{_OBJ_MAN,OL_3,0x4013},///< Baðlantý Bilgileri Nesnesi
			//----------------------------------------------
				{_OBJ_MAN,OL_4,0x2016},///< Baðlantý Tipi
				{_OBJ_MAN,OL_4,0x1052},///< Cihaz IP
				{_OBJ_OPT,OL_4,0x1053},///< Baðlantý Numarasý
				{_OBJ_OPT,OL_4,0x1054},///< DNS Numarasý
				{_OBJ_OPT,OL_4,0x1055},///< Baðlantý Operatörü
				{_OBJ_OPT,OL_4,0x1056},///< Baðlantý Noktasý Adý
				{_OBJ_OPT,OL_4,0x8014},///< Baðlantýya Özel Deðerler Listesi
				//----------------------------------------------
					{_OBJ_MAN,OL_5,0x4017},///< Baðlantýya Özel Deðerler Nesnesi
					//----------------------------------------------
						{_OBJ_MAN,OL_6,0x105F},///< Anahtar
						{_OBJ_MAN,OL_6,0x1060},///< Deðer
					//----------------------------------------------
				//----------------------------------------------
			//----------------------------------------------
		//----------------------------------------------
		{_OBJ_MAN,OL_2,0x1023},///< Public Key Talebi
		//----------------------------------------------
	//----------------------------------------------
//----------------------------------------------
{0xAA,0x55,0xA55A}///< end
};

const Ui16 DKRTU_OBJ_LIST_MSTR[40][2] = {///<0x106E	Komut Seti Kimlik Kodu	0x4D535452 - (MSTR)
		{0x4010,	0x1100},
		{0x4010,	0x2000},
		{0x4010,	0x2010},
		{0x4020,	0x5100},
		{0x4020,	0x5110},
		{0x4020,	0x5200},
		{0x4020,	0x5202},
		{0x4020,	0x52AA},
		{0x4020,	0x5300},
		{0x4020,	0x5400},
		{0x4020,	0x5700},
		{0x4030,	0x3010},
		{0x4050,	0x4303},
		{0x4050,	0x4700},
		{0x4080,	0x5600},
		{0x40D0,	0x300D},
		{0x40D0,	0x420D},
		{0x40D0,	0x430D},
		{0x40D0,	0x440D},
		{0x40D0,	0x560D},
		{0x40D0,	0x470D},
		{0x5010,	0x1010},
		{0x5010,	0x4110},
		{0x5020,	0x5000},
		{0x5030,	0x3000},
		{0x5040,	0x4202},
		{0x5050,	0x4201},
		{0x5050,	0x42AA},
		{0x5050,	0x4400},
		{0x5050,	0x4510},
		{0x5080,	0x4600},
		{0x5513,	0x1000},
		{0x5512,	0x1000},
		{0x5511,	0x1000},
		{0x5510,	0x1000},
		{0x6060,	0x43A0},
		{0x6070,	0x7000},///< 36
		{0x0000,	0x0000},
		{0x0000,	0x0000},
		{0x0000,	0x0000}
};

const Ui16 DKRTU_OBJ_LIST_KLTE[26][2] = {///<0x106E	Komut Seti Kimlik Kodu	0x4B4C5445 - (KLTE)
		{0x6050,	0x1000},
		{0x70D0,	0x10D0},
		{0x6050,	0x3000},
		{0x6050,	0x3010},
		{0x6050,	0x300D},
		{0x7020,	0x2001},
		{0x6050,	0x3001},
		{0x6050,	0x3003},
		{0x6050,	0x3004},
		{0x6050,	0x3007},
		{0x6050,	0x300E},
		{0x6050,	0x3008},
		{0x7060,	0x4001},
		{0x6050,	0x3002},
		{0x6050,	0x30A0},
		{0x7020,	0x2002},
		{0x6050,	0x30B0},
		{0x6050,	0x300F},
		{0x7020,	0x2003},
		{0x6050,	0x3100},
		{0x6050,	0x3005},
		{0x7060,	0x4002},
		{0x6050,	0x3009},///< 22
		{0x0000,	0x0000},
		{0x0000,	0x0000},
		{0x0000,	0x0000}
};

const Ui16 DKRTU_OBJ_LIST_SIEC[19][2] = {///<0x106E	Komut Seti Kimlik Kodu	0x53494543 - (SIEC)
		{0x4030,	0x0010},
		{0x4030,	0x5010},
		{0x4030,	0x0020},
		{0x40D0,	0x00D0},
		{0x5030,	0x0040},
		{0x5030,	0x5000},
		{0x5030,	0x0030},
		{0x6050,	0x1030},
		{0x6050,	0x1000},
		{0x6050,	0x1100},
		{0x6050,	0x1040},
		{0x6050,	0x1020},
		{0x6050,	0x1010},
		{0x7060,	0x2000},
		{0x7060,	0x2100},
		{0x8030,	0x0050},///< 15
		{0x0000,	0x0000},
		{0x0000,	0x0000},
		{0x0000,	0x0000}
};

//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
const DKRTU_PARSOBJ *pDKRTU_MSTR_OBJS[] = {
		&DKRTU_MSTR_40101100_OBJTABLE[0],
		&DKRTU_MSTR_40102000_OBJTABLE[0],
		&DKRTU_MSTR_40102010_OBJTABLE[0],
		&DKRTU_MSTR_40205100_OBJTABLE[0],
		&DKRTU_MSTR_40205110_OBJTABLE[0],
		&DKRTU_MSTR_40205200_OBJTABLE[0],
		&DKRTU_MSTR_40205202_OBJTABLE[0],
		&DKRTU_MSTR_402052AA_OBJTABLE[0],
		&DKRTU_MSTR_40205300_OBJTABLE[0],
		&DKRTU_MSTR_40205400_OBJTABLE[0],
		&DKRTU_MSTR_40205700_OBJTABLE[0],
		&DKRTU_MSTR_40303010_OBJTABLE[0],
		&DKRTU_MSTR_40504303_OBJTABLE[0],
		&DKRTU_MSTR_40504700_OBJTABLE[0],
		&DKRTU_MSTR_40805600_OBJTABLE[0],
		&DKRTU_MSTR_40D0300D_OBJTABLE[0],
		&DKRTU_MSTR_40D0420D_OBJTABLE[0],
		&DKRTU_MSTR_40D0430D_OBJTABLE[0],
		&DKRTU_MSTR_40D0440D_OBJTABLE[0],
		&DKRTU_MSTR_40D0560D_OBJTABLE[0],
		&DKRTU_MSTR_40D0470D_OBJTABLE[0],
		&DKRTU_MSTR_50101010_OBJTABLE[0],
		&DKRTU_MSTR_50104110_OBJTABLE[0],
		&DKRTU_MSTR_50205000_OBJTABLE[0],
		&DKRTU_MSTR_50303000_OBJTABLE[0],
		&DKRTU_MSTR_50404202_OBJTABLE[0],
		&DKRTU_MSTR_50504201_OBJTABLE[0],
		&DKRTU_MSTR_505042AA_OBJTABLE[0],
		&DKRTU_MSTR_50504400_OBJTABLE[0],
		&DKRTU_MSTR_50504510_OBJTABLE[0],
		&DKRTU_MSTR_50804600_OBJTABLE[0],
		&DKRTU_MSTR_55131000_OBJTABLE[0],
		&DKRTU_MSTR_55121000_OBJTABLE[0],
		&DKRTU_MSTR_55111000_OBJTABLE[0],
		&DKRTU_MSTR_55101000_OBJTABLE[0],
		&DKRTU_MSTR_606043A0_OBJTABLE[0],
		&DKRTU_MSTR_60707000_OBJTABLE[0]
};

const DKRTU_PARSOBJ *pDKRTU_KLTE_OBJS[] = {
		&DKRTU_KLTE_60501000_OBJTABLE[0],
		&DKRTU_KLTE_70D010D0_OBJTABLE[0],
		&DKRTU_KLTE_60503000_OBJTABLE[0],
		&DKRTU_KLTE_60503010_OBJTABLE[0],
		&DKRTU_KLTE_6050300D_OBJTABLE[0],
		&DKRTU_KLTE_70202001_OBJTABLE[0],
		&DKRTU_KLTE_60503001_OBJTABLE[0],
		&DKRTU_KLTE_60503003_OBJTABLE[0],
		&DKRTU_KLTE_60503004_OBJTABLE[0],
		&DKRTU_KLTE_60503007_OBJTABLE[0],
		&DKRTU_KLTE_6050300E_OBJTABLE[0],
		&DKRTU_KLTE_60503008_OBJTABLE[0],
		&DKRTU_KLTE_70604001_OBJTABLE[0],
		&DKRTU_KLTE_60503002_OBJTABLE[0],
		&DKRTU_KLTE_605030A0_OBJTABLE[0],
		&DKRTU_KLTE_70202002_OBJTABLE[0],
		&DKRTU_KLTE_605030B0_OBJTABLE[0],
		&DKRTU_KLTE_6050300F_OBJTABLE[0],
		&DKRTU_KLTE_70202003_OBJTABLE[0],
		&DKRTU_KLTE_60503100_OBJTABLE[0],
		&DKRTU_KLTE_60503005_OBJTABLE[0],
		&DKRTU_KLTE_70604002_OBJTABLE[0],
		&DKRTU_KLTE_60503009_OBJTABLE[0]
};

const DKRTU_PARSOBJ *pDKRTU_SIEC_OBJS[] = {
		&DKRTU_SIEC_40300010_OBJTABLE[0],
		&DKRTU_SIEC_40305010_OBJTABLE[0],
		&DKRTU_SIEC_40300020_OBJTABLE[0],
		&DKRTU_SIEC_40D000D0_OBJTABLE[0],
		&DKRTU_SIEC_50300040_OBJTABLE[0],
		&DKRTU_SIEC_50305000_OBJTABLE[0],
		&DKRTU_SIEC_50300030_OBJTABLE[0],
		&DKRTU_SIEC_60501030_OBJTABLE[0],
		&DKRTU_SIEC_60501000_OBJTABLE[0],
		&DKRTU_SIEC_60501100_OBJTABLE[0],
		&DKRTU_SIEC_60501040_OBJTABLE[0],
		&DKRTU_SIEC_60501020_OBJTABLE[0],
		&DKRTU_SIEC_60501010_OBJTABLE[0],
		&DKRTU_SIEC_70602000_OBJTABLE[0],
		&DKRTU_SIEC_70602100_OBJTABLE[0],
		&DKRTU_SIEC_80300050_OBJTABLE[0]
};
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
int DKRTU_MSTR_40101100_INITCALL(char *pBuf, int Len, int Obj) { return 1; }
int DKRTU_MSTR_55101000_INITCALL(char *pBuf, int Len, int Obj) { return 1; }
const PFO pDKRTU_MSTR_OBJ_INITCALL[] = {
		DKRTU_MSTR_40101100_INITCALL,
		//&DKRTU_MSTR_40102000_INITCALL,
		//&DKRTU_MSTR_40102010_INITCALL,
		//&DKRTU_MSTR_40205100_INITCALL,
		//&DKRTU_MSTR_40205110_INITCALL,
		//&DKRTU_MSTR_40205200_INITCALL,
		//&DKRTU_MSTR_40205202_INITCALL,
		//&DKRTU_MSTR_402052AA_INITCALL,
		//&DKRTU_MSTR_40205300_INITCALL,
		//&DKRTU_MSTR_40205400_INITCALL,
		//&DKRTU_MSTR_40205700_INITCALL,
		//&DKRTU_MSTR_40303010_INITCALL,
		//&DKRTU_MSTR_40504303_INITCALL,
		//&DKRTU_MSTR_40504700_INITCALL,
		//&DKRTU_MSTR_40805600_INITCALL,
		//&DKRTU_MSTR_40D0300D_INITCALL,
		//&DKRTU_MSTR_40D0420D_INITCALL,
		//&DKRTU_MSTR_40D0430D_INITCALL,
		//&DKRTU_MSTR_40D0440D_INITCALL,
		//&DKRTU_MSTR_40D0560D_INITCALL,
		//&DKRTU_MSTR_40D0470D_INITCALL,
		//&DKRTU_MSTR_50101010_INITCALL,
		//&DKRTU_MSTR_50104110_INITCALL,
		//&DKRTU_MSTR_50205000_INITCALL,
		//&DKRTU_MSTR_50303000_INITCALL,
		//&DKRTU_MSTR_50404202_INITCALL,
		//&DKRTU_MSTR_50504201_INITCALL,
		//&DKRTU_MSTR_505042AA_INITCALL,
		//&DKRTU_MSTR_50504400_INITCALL,
		//&DKRTU_MSTR_50504510_INITCALL,
		//&DKRTU_MSTR_50804600_INITCALL,
		//&DKRTU_MSTR_55131000_INITCALL,
		//&DKRTU_MSTR_55121000_INITCALL,
		//&DKRTU_MSTR_55111000_INITCALL,
		DKRTU_MSTR_55101000_INITCALL,
		//&DKRTU_MSTR_606043A0_INITCALL,
		//&DKRTU_MSTR_60707000_INITCALL
};

int DKRTU_KLTE_60501000_INITCALL(char *pBuf, int Len, int Obj) { return 1; }
const PFO pDKRTU_KLTE_INITCALL[] = {
		&DKRTU_KLTE_60501000_INITCALL,
		//		&DKRTU_KLTE_70D010D0_INITCALL,
		//		&DKRTU_KLTE_60503000_INITCALL,
		//		&DKRTU_KLTE_60503010_INITCALL,
		//		&DKRTU_KLTE_6050300D_INITCALL,
		//		&DKRTU_KLTE_70202001_INITCALL,
		//		&DKRTU_KLTE_60503001_INITCALL,
		//		&DKRTU_KLTE_60503003_INITCALL,
		//		&DKRTU_KLTE_60503004_INITCALL,
		//		&DKRTU_KLTE_60503007_INITCALL,
		//		&DKRTU_KLTE_6050300E_INITCALL,
		//		&DKRTU_KLTE_60503008_INITCALL,
		//		&DKRTU_KLTE_70604001_INITCALL,
		//		&DKRTU_KLTE_60503002_INITCALL,
		//		&DKRTU_KLTE_605030A0_INITCALL,
		//		&DKRTU_KLTE_70202002_INITCALL,
		//		&DKRTU_KLTE_605030B0_INITCALL,
		//		&DKRTU_KLTE_6050300F_INITCALL,
		//		&DKRTU_KLTE_70202003_INITCALL,
		//		&DKRTU_KLTE_60503100_INITCALL,
		//		&DKRTU_KLTE_60503005_INITCALL,
		//		&DKRTU_KLTE_70604002_INITCALL,
		//		&DKRTU_KLTE_60503009_INITCALL
};

int DKRTU_SIEC_40300010_INITCALL(char *pBuf, int Len, int Obj) { return 1; }
const PFO pDKRTU_SIEC_INITCALL[] = {
		&DKRTU_SIEC_40300010_INITCALL,
		//		&DKRTU_SIEC_40305010_INITCALL,
		//		&DKRTU_SIEC_40300020_INITCALL,
		//		&DKRTU_SIEC_40D000D0_INITCALL,
		//		&DKRTU_SIEC_50300040_INITCALL,
		//		&DKRTU_SIEC_50305000_INITCALL,
		//		&DKRTU_SIEC_50300030_INITCALL,
		//		&DKRTU_SIEC_60501030_INITCALL,
		//		&DKRTU_SIEC_60501000_INITCALL,
		//		&DKRTU_SIEC_60501100_INITCALL,
		//		&DKRTU_SIEC_60501040_INITCALL,
		//		&DKRTU_SIEC_60501020_INITCALL,
		//		&DKRTU_SIEC_60501010_INITCALL,
		//		&DKRTU_SIEC_70602000_INITCALL,
		//		&DKRTU_SIEC_70602100_INITCALL,
		//		&DKRTU_SIEC_80300050_INITCALL
};
////------------------------------------------------------------------//
////------------------------------------------------------------------//
////------------------------------------------------------------------//
int DKRTU_MSTR_40101100_CALLBACK(char *pBuf, int Len, int Obj) { return 1; }
int DKRTU_MSTR_55101000_CALLBACK(char *pBuf, int Len, int Obj) { return 1; }
const PFO pDKRTU_MSTR_OBJ_CALLBACK[] = {
		&DKRTU_MSTR_40101100_CALLBACK,
		//		&DKRTU_MSTR_40102000_CALLBACK,
		//		&DKRTU_MSTR_40102010_CALLBACK,
		//		&DKRTU_MSTR_40205100_CALLBACK,
		//		&DKRTU_MSTR_40205110_CALLBACK,
		//		&DKRTU_MSTR_40205200_CALLBACK,
		//		&DKRTU_MSTR_40205202_CALLBACK,
		//		&DKRTU_MSTR_402052AA_CALLBACK,
		//		&DKRTU_MSTR_40205300_CALLBACK,
		//		&DKRTU_MSTR_40205400_CALLBACK,
		//		&DKRTU_MSTR_40205700_CALLBACK,
		//		&DKRTU_MSTR_40303010_CALLBACK,
		//		&DKRTU_MSTR_40504303_CALLBACK,
		//		&DKRTU_MSTR_40504700_CALLBACK,
		//		&DKRTU_MSTR_40805600_CALLBACK,
		//		&DKRTU_MSTR_40D0300D_CALLBACK,
		//		&DKRTU_MSTR_40D0420D_CALLBACK,
		//		&DKRTU_MSTR_40D0430D_CALLBACK,
		//		&DKRTU_MSTR_40D0440D_CALLBACK,
		//		&DKRTU_MSTR_40D0560D_CALLBACK,
		//		&DKRTU_MSTR_40D0470D_CALLBACK,
		//		&DKRTU_MSTR_50101010_CALLBACK,
		//		&DKRTU_MSTR_50104110_CALLBACK,
		//		&DKRTU_MSTR_50205000_CALLBACK,
		//		&DKRTU_MSTR_50303000_CALLBACK,
		//		&DKRTU_MSTR_50404202_CALLBACK,
		//		&DKRTU_MSTR_50504201_CALLBACK,
		//		&DKRTU_MSTR_505042AA_CALLBACK,
		//		&DKRTU_MSTR_50504400_CALLBACK,
		//		&DKRTU_MSTR_50504510_CALLBACK,
		//		&DKRTU_MSTR_50804600_CALLBACK,
		//		&DKRTU_MSTR_55131000_CALLBACK,
		//		&DKRTU_MSTR_55121000_CALLBACK,
		//		&DKRTU_MSTR_55111000_CALLBACK,
				&DKRTU_MSTR_55101000_CALLBACK,
				//		&DKRTU_MSTR_606043A0_CALLBACK,
				//		&DKRTU_MSTR_60707000_CALLBACK
};

int DKRTU_KLTE_60501000_CALLBACK(char *pBuf, int Len, int Obj) { return 1; }
const PFO pDKRTU_KLTE_CALLBACK[] = {
		&DKRTU_KLTE_60501000_CALLBACK,
		//		&DKRTU_KLTE_70D010D0_CALLBACK,
		//		&DKRTU_KLTE_60503000_CALLBACK,
		//		&DKRTU_KLTE_60503010_CALLBACK,
		//		&DKRTU_KLTE_6050300D_CALLBACK,
		//		&DKRTU_KLTE_70202001_CALLBACK,
		//		&DKRTU_KLTE_60503001_CALLBACK,
		//		&DKRTU_KLTE_60503003_CALLBACK,
		//		&DKRTU_KLTE_60503004_CALLBACK,
		//		&DKRTU_KLTE_60503007_CALLBACK,
		//		&DKRTU_KLTE_6050300E_CALLBACK,
		//		&DKRTU_KLTE_60503008_CALLBACK,
		//		&DKRTU_KLTE_70604001_CALLBACK,
		//		&DKRTU_KLTE_60503002_CALLBACK,
		//		&DKRTU_KLTE_605030A0_CALLBACK,
		//		&DKRTU_KLTE_70202002_CALLBACK,
		//		&DKRTU_KLTE_605030B0_CALLBACK,
		//		&DKRTU_KLTE_6050300F_CALLBACK,
		//		&DKRTU_KLTE_70202003_CALLBACK,
		//		&DKRTU_KLTE_60503100_CALLBACK,
		//		&DKRTU_KLTE_60503005_CALLBACK,
		//		&DKRTU_KLTE_70604002_CALLBACK,
		//		&DKRTU_KLTE_60503009_CALLBACK
};

int DKRTU_SIEC_40300010_CALLBACK(char *pBuf, int Len, int Obj) { return 1; }
const PFO pDKRTU_SIEC_CALLBACK[] = {
		&DKRTU_SIEC_40300010_CALLBACK,
		//		&DKRTU_SIEC_40305010_CALLBACK,
		//		&DKRTU_SIEC_40300020_CALLBACK,
		//		&DKRTU_SIEC_40D000D0_CALLBACK,
		//		&DKRTU_SIEC_50300040_CALLBACK,
		//		&DKRTU_SIEC_50305000_CALLBACK,
		//		&DKRTU_SIEC_50300030_CALLBACK,
		//		&DKRTU_SIEC_60501030_CALLBACK,
		//		&DKRTU_SIEC_60501000_CALLBACK,
		//		&DKRTU_SIEC_60501100_CALLBACK,
		//		&DKRTU_SIEC_60501040_CALLBACK,
		//		&DKRTU_SIEC_60501020_CALLBACK,
		//		&DKRTU_SIEC_60501010_CALLBACK,
		//		&DKRTU_SIEC_70602000_CALLBACK,
		//		&DKRTU_SIEC_70602100_CALLBACK,
		//		&DKRTU_SIEC_80300050_CALLBACK
};
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//



/****************************************************************************/
/*! \fn DKRTU_OBJ_NEXT DKRTU_Obj_FindNextSameLevel(Ui08 *p, DKRTU_PARSOBJ *pObjA)
**
** \brief		        Find and test next object that has same level
**
** \param    			p -> message buffer instant position and it points address of instant message id
** 						pObjA -> object folder instant position and it points address of instant object
**
** \return				Next object informations and return value
**
*****************************************************************************/
#if DKRTU_PARSOBJ_DEMO
Si08 ErrCode;
Ui16 ErrObjNo;
void *p55101000;
void *pMSTR_OBJS;
void *p55101000_orj;
void *pMSTR_OBJS_orj;
Ui08 ObjCnt;
Si08 ErrCode2;
Ui16 ErrObjNo2;
DKRTU_PARSOBJ	InstObj;
Ui16 InstObj_Len;
Ui16	ErrObjNo3;
Ui16 	RmnData;
Ui16	testMsj, testEvt;
Ui08	testObjCnt;
DKRTU_PARSOBJ	testObj;

Ui32 DemoInt0;
Ui32 DemoInt1;
Ui16 DemoUi16[4];
#endif
DKRTU_OBJ_NEXT DKRTU_Obj_FindNextSameLevel(Ui08 *p, DKRTU_PARSOBJ *pObjA) {
	DKRTU_OBJ_NEXT			Nxt;
#define pObjN 			Nxt.pNo
#define pNxt 			Nxt.pNa
#define Ret 			Nxt.RetVal
	pObjN = pObjA;

#if DKRTU_PARSOBJ_DEMO
	ErrObjNo = pObjA->_MsgId;
	testObjCnt = 0;
#endif
	do {
		pObjN++;
#if DKRTU_PARSOBJ_DEMO
		ErrObjNo3 = pObjN->_MsgId;
		testObjCnt++;
		if (pObjN->_ObjLvl == 0x55) {
			testObj = *pObjN;
		}
#endif
		if (pObjN->_ObjLvl == pObjA->_ObjLvl) {
#if DKRTU_PARSOBJ_DEMO
			DemoInt0 = *(Ui32*)p;
			DemoInt1 = *(Ui32*)(p + 4 + *((Ui16 *)(p + 2)));
			DemoUi16[0] = *((Ui16 *)(p + 0));
			DemoUi16[1] = *((Ui16 *)(p + 2));
			DemoUi16[2] = *((Ui16 *)(p + 4 + *((Ui16 *)(p + 2))));
			DemoUi16[3] = *((Ui16 *)(p + 4 + *((Ui16 *)(p + 4))));
#endif
			pNxt = p + 4 + *((Ui16 *)(p + 2));
			if (*((Ui16 *)(pNxt)) == pObjN->_MsgId) {///< pHdr+2 = pLen --> pHdr+ *plen = pHdrNext
				Ret = _DKRTU_OBJ_OK;
#if DKRTU_PARSOBJ_DEMO
				ErrCode = Ret;
#endif
				return Nxt;
			}
			else if (pObjN->_ObjOpt == _OBJ_OPT) {///< this object is optional and not including
				do {///< skip all sub-objects of this optional object
					pObjN++;
				} while ((pObjN->_ObjLvl > pObjA->_ObjLvl) && (pObjN->_ObjLvl != 0x55));///< lower level object so continue
				pObjN--;
			}
			else {
				pObjN = 0;
				Ret = _DKRTU_OBJ_ERR_NOTFOUND;
#if DKRTU_PARSOBJ_DEMO
				ErrCode = Ret;
#endif
				return Nxt;
			}
		}
		else if (pObjN->_ObjLvl < pObjA->_ObjLvl) {///< upper level object
			pNxt = p + 4 + *((Ui16 *)(p + 2));
			if (*((Ui16 *)(pNxt)) == pObjN->_MsgId) {///< pHdr+2 = pLen --> pHdr+ *plen = pHdrNext
				Ret = _DKRTU_OBJ_LVLEND;
#if DKRTU_PARSOBJ_DEMO
				ErrCode = Ret;
#endif
				return Nxt;
			}
			else if (pObjN->_ObjOpt == _OBJ_OPT) {///< this object is optional and not including
				do {///< skip all sub-objects of this optional object
					pObjN++;
				} while ((pObjN->_ObjLvl > pObjA->_ObjLvl) && (pObjN->_ObjLvl != 0xAA));///< lower level object so continue
				pObjN--;
			}
			else {
				pObjN = 0;
				Ret = _DKRTU_OBJ_ERR_NOTFOUND;
#if DKRTU_PARSOBJ_DEMO
				ErrCode = Ret;
#endif
				return Nxt;
			}
		}
		else {///< lower level object so continue, not normal !!!
			//...
		}
	} while (!((pObjN->_ObjOpt == 0xAA) && (pObjN->_ObjLvl == 0x55) && (pObjN->_MsgId == 0xA55A)));
	pObjN = 0;
	Ret = _DKRTU_OBJ_LAST;
#if DKRTU_PARSOBJ_DEMO
	ErrCode = Ret;
#endif
	return Nxt;
}

/****************************************************************************/
/*! \fn int DKRTU_ObjCnt(DKRTU_PARSOBJ	*pObjA)
**
** \brief		        cntof(Obj)
**
** \param    			pObjA -> object list pointer
**
** \return				Number of objects
**
*****************************************************************************/
int DKRTU_ObjCnt(DKRTU_PARSOBJ	*pObjA) {
	int i = 0;

	while (!((pObjA->_ObjOpt == 0xAA) && (pObjA->_ObjLvl == 0x55) && (pObjA->_MsgId == 0xA55A))) {
		i++;
		pObjA++;
	}
	return i;
}

/****************************************************************************/
/*! \fn int DKRTU_TestMessageStruct(Ui08 *pMsg, Ui16 len)
**
** \brief		        Test all message with object list
**
** \param    			pMsg -> message buffer pointer
** 						len -> length of incoming data
**
** \return				OK or Error Code
**
*****************************************************************************/
int DKRTU_TestMessageStruct(Ui08 *pMsg, Ui16 len, Ui08 hdr) {
	DKRTU_OBJ_NEXT			Nxt;
#define pObjN 			Nxt.pNo
#define pNxt 			Nxt.pNa
#define Ret 			Nxt.RetVal
	Ui32 			i, j, k;
	Ui16			m, n;
	S_RTU_bV1		*pHdr = (S_RTU_bV1	*)pMsg;
	DKRTU_PARSOBJ	*pObjA = 0;
	Ui08 			*p;
	DKRTU_PARSOBJ	**pL;
	int jj;

	if (len >= (sizeof(S_RTU_bV1) - 1)) {
		if (hdr) {
			pObjA = (DKRTU_PARSOBJ	*)&DKRTU_HEADER_OBJTABLE[0];
			p = pMsg;
			j = len;///< remaining data
		}
		else {
			//---------- Test Header ------------------//
			jj = DKRTU_TestMessageStruct(pMsg, len, 1);///< test for header
			if (jj < 0) {
				return jj;///< return error code
			}
			//---------- Call Init ------------------//
			//------------- Load IDs ----------------//
			i = pHdr->MainMessageCode;
			j = *((Ui32 *)&pHdr->MessageTitle.InstructionSetIdentityCode.Value[0]);
			k = *((Ui16 *)&pHdr->MessageTitle.EventCode.Value[0]);
			n = 0;
			//------------- Find Object ----------------//
			if (j == _MSTR) {
				pL = (DKRTU_PARSOBJ	**)&pDKRTU_MSTR_OBJS[0];
				n = cntof(pDKRTU_MSTR_OBJS);
				p = (Ui08 *)&DKRTU_OBJ_LIST_MSTR[0][0];
			}
			else if (j == _KLTE) {
				pL = (DKRTU_PARSOBJ	**)&pDKRTU_KLTE_OBJS[0];
				n = cntof(pDKRTU_KLTE_OBJS);
				p = (Ui08 *)&DKRTU_OBJ_LIST_KLTE[0][0];
			}
			else if (j == _SIEC) {
				pL = (DKRTU_PARSOBJ	**)&pDKRTU_SIEC_OBJS[0];
				n = cntof(pDKRTU_SIEC_OBJS);
				p = (Ui08 *)&DKRTU_OBJ_LIST_SIEC[0][0];
			}
			else {
				return _DKRTU_OBJ_ERR_FAMILY;
			}
			for (m = 0; m < n; m++) {
#if DKRTU_PARSOBJ_DEMO
				testMsj = (((const Ui16 *)p)[(m * 2)]);
				testEvt = (((const Ui16 *)p)[(m * 2 + 1)]);
#endif
				if (((((const Ui16 *)p)[(m * 2)]) == i) && ((((const Ui16 *)p)[(m * 2 + 1)]) == k)) {
					pObjA = pL[m];
#if DKRTU_PARSOBJ_DEMO
					pMSTR_OBJS = *pL;
					p55101000 = (void *)pObjA;
					pMSTR_OBJS_orj = (void *)&pDKRTU_MSTR_OBJS;
					p55101000_orj = (void *)&DKRTU_MSTR_55101000_OBJTABLE;
#endif
					break;
				}
			}
			if (m >= n) {
				return _DKRTU_OBJ_ERR_UNDEFOBJ;
			}
			p = pMsg + (len - jj);
			j = jj;///< remaining data
		}
		//------------- Test Object ----------------//
		k = DKRTU_ObjCnt(pObjA);
#if DKRTU_PARSOBJ_DEMO
		ObjCnt = k;
#endif
		for (n = 0; n < k; n++) {///< objects in the file
#if DKRTU_PARSOBJ_DEMO
			InstObj = *pObjA;
			InstObj_Len = *(Ui16 *)(p + 2);///< length;
			ErrObjNo2 = pObjA->_MsgId;
			RmnData = j;
			if (ErrObjNo2 == 0x1022) {
				DemoInt0 = *(Ui32*)p;
				DemoUi16[0] = *((Ui16 *)(p + 0));
				DemoUi16[1] = *((Ui16 *)(p + 2));
			}
			else if (ErrObjNo2 == 0x104B) {
				DemoInt0 = *(Ui32*)p;
				DemoUi16[0] = *((Ui16 *)(p + 0));
				DemoUi16[1] = *((Ui16 *)(p + 2));
			}
#endif
			if (*(Ui16 *)p == pObjA->_MsgId) {///< object including
				i = *(Ui16 *)(p + 2);///< length;
				Nxt = DKRTU_Obj_FindNextSameLevel(p, pObjA);
				if (Ret < _DKRTU_OBJ_NULL) {///< fail
#if DKRTU_PARSOBJ_DEMO
					ErrCode2 = _DKRTU_OBJ_ERR_NOTFOUND;
#endif
					return _DKRTU_OBJ_ERR_NOTFOUND;
				}
				else if (Ret == _DKRTU_OBJ_OK) {///< do not need to test length because it is Ok
					//...
				}
				else if (Ret == _DKRTU_OBJ_LVLEND) {///< do not need to test length because it is Ok
					//...
				}
				else if (Ret == _DKRTU_OBJ_LAST) {///< this is the last object of this level so check length
					if (j != (i + 4)) {
#if DKRTU_PARSOBJ_DEMO
						ErrCode2 = _DKRTU_OBJ_ERR_LENGTH;
#endif
						return _DKRTU_OBJ_ERR_LENGTH;
					}
				}
				else {///< unknown return value !!!
#if DKRTU_PARSOBJ_DEMO
					ErrCode2 = _DKRTU_OBJ_ERR_NOTFOUND;
#endif
					return _DKRTU_OBJ_ERR_NOTFOUND;
				}
				pObjN = pObjA;
				pObjN++;
				p += 4;
				j -= 4;
				if ((pObjN->_ObjOpt == 0xAA) && (pObjN->_ObjLvl == 0x55) && (pObjN->_MsgId == 0xA55A)) {
					if (i == j) {
#if DKRTU_PARSOBJ_DEMO
						ErrCode2 = _DKRTU_OBJ_OK;
#endif
						if (hdr) { return j;/*remaining data*/ }
						else { return _DKRTU_OBJ_OK; }
					}
					else {
#if DKRTU_PARSOBJ_DEMO
						ErrCode2 = _DKRTU_OBJ_ERR_LENGTH;
#endif
						return _DKRTU_OBJ_ERR_LENGTH;
					}
				}
				else if (pObjN->_ObjLvl > pObjA->_ObjLvl) {///< low level object
					//..
				}
				else {///< same or upper level object
					p += i;
					j -= i;
				}
				pObjA++;
			}
			else if (pObjA->_ObjOpt == _OBJ_OPT) {///< optional object not including
				///< continue...
			}
			else {///< mandatory object but not including
#if DKRTU_PARSOBJ_DEMO
				ErrCode2 = _DKRTU_OBJ_ERR_NOTFOUND;
#endif
				return _DKRTU_OBJ_ERR_NOTFOUND;
			}
		}
		//-----------------------------//
#if DKRTU_PARSOBJ_DEMO
		ErrCode2 = -50;///< undef
#endif
		if (hdr) { return j;/*remaining data*/ }
		else { return _DKRTU_OBJ_OK; }
	}
	else {
#if DKRTU_PARSOBJ_DEMO
		ErrCode2 = _DKRTU_OBJ_ERR_LENGTH;
#endif
		return _DKRTU_OBJ_ERR_LENGTH;
	}
}


#if DKRTU_PARSOBJ_TEST
Ui08 msgtestarray[273] = {
0x10, 0x55, 0x0d, 0x01,
		0xf0, 0x4f, 0x60, 0x00,
					0x6e, 0x10, 0x04, 0x00, 0x52, 0x54, 0x53, 0x4d,
					0x01, 0x10, 0x03, 0x00, 0x00, 0x00, 0x00,
					0x02, 0x10, 0x02, 0x00, 0x00, 0x10,
					0x03, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					0x04, 0x10, 0x02, 0x00, 0x10, 0x55,
					0x02, 0x20, 0x01, 0x00, 0xbb,
					0x01, 0x20, 0x01, 0x00, 0x00,
					0x06, 0x10, 0x02, 0x00, 0x00, 0x00,
					0x06, 0x20, 0x01, 0x00, 0x00,
					0x08, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					0x4b, 0x10, 0x0C, 0x00, 0xbb, 0xaa, 0x00, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11,
		0xff, 0x4f, 0xa5, 0x00,
					0x22, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //zorunlu
					0x0d, 0x80, 0x25, 0x00,
						0x02, 0x40, 0x21, 0x00,
							0x16, 0x20, 0x01, 0x00, 0x00, //zorunlu
							0x57, 0x10, 0x01, 0x00, 0x00, //zorunlu
							0x58, 0x10, 0x01, 0x00, 0x00, //zorunlu
							0x15, 0x80, 0x0e, 0x00,
								0x17, 0x40, 0x0a, 0x00,
									0x5f, 0x10, 0x01, 0x00, 0x00,
									0x60, 0x10, 0x01, 0x00, 0x00,
					0x07, 0x40, 0x26, 0x00,
						0x0e, 0x20, 0x01, 0x00, 0x00, //zorunlu
						0x33, 0x10, 0x01, 0x00, 0x00,
						0x34, 0x10, 0x06, 0x00, 0x00,0x00,0x00,0x00,0x00,0x00,//0x34, 0x10, 0x01, 0x00, 0x00,
						//0x35, 0x10, 0x01, 0x00, 0x00,
						0x12, 0x80, 0x0e, 0x00,
							0x17, 0x40, 0x0a, 0x00,
								0x5f, 0x10, 0x01, 0x00, 0x00,
								0x60, 0x10, 0x01, 0x00, 0x00,
					0x02, 0x80, 0x05, 0x00,
						0x17, 0x10, 0x01, 0x00, 0x00,
					0x0e, 0x80, 0x34, 0x00,
						0x13, 0x40, 0x30, 0x00,
							0x16, 0x20, 0x01, 0x00, 0x00, //zorunlu
							0x52, 0x10, 0x01, 0x00, 0x00,
							0x53, 0x10, 0x01, 0x00, 0x00,
							0x54, 0x10, 0x01, 0x00, 0x00,
							0x55, 0x10, 0x01, 0x00, 0x00,
							0x56, 0x10, 0x01, 0x00, 0x00,
							0x14, 0x80, 0x0e, 0x00,
								0x17, 0x40, 0x0a, 0x00,
									0x5f, 0x10, 0x01, 0x00, 0x00,
									0x60, 0x10, 0x01, 0x00, 0x00,
					0x23, 0x10, 0x01, 0x00, 0x01
};



int RtrnVal = 0;
int ErrCnt = 0;
int OkCnt = 0;
void DKRTU_ParsObj_Test(void) {
	//S_RTU_bV1 H1;

	RtrnVal = DKRTU_TestMessageStruct(&msgtestarray[0], 273,0);

	if (RtrnVal < 0) {
		ErrCnt++;
	}
	else {
		OkCnt++;
	}
}
#endif
//#endif





/****************************************************************************/
/*! \fn int DKRTU_TestHeaderStruct(Ui08 *pMsg, Ui16 len)
**
** \brief		        Test message header with object list
**
** \param    			pMsg -> message buffer pointer
** 						len -> length of incoming data
**
** \return				OK or Error Code
**
*****************************************************************************/
int DKRTU_TestHeaderStruct(Ui08 *pMsg, Ui16 len) {
	DKRTU_OBJ_NEXT			Nxt;
#define pObjN 			Nxt.pNo
#define pNxt 			Nxt.pNa
#define Ret 			Nxt.RetVal
	Ui32 			i, j, k;
	Ui16			n;
	DKRTU_PARSOBJ	*pObjA = 0;
	Ui08 			*p;

	if (len >= (sizeof(S_RTU_bV1) - 1)) {
		pObjA = (DKRTU_PARSOBJ	*)&DKRTU_HEADER_OBJTABLE[0];
		//------------- Test Object ----------------//
		p = pMsg;
		j = len;///< remaining data
		k = DKRTU_ObjCnt(pObjA);
#if DKRTU_PARSOBJ_DEMO
		ObjCnt = k;
#endif
		for (n = 0; n < k; n++) {///< objects in the file
#if DKRTU_PARSOBJ_DEMO
			InstObj = *pObjA;
			InstObj_Len = *(Ui16 *)(p + 2);///< length;
			ErrObjNo2 = pObjA->_MsgId;
			RmnData = j;
			if (ErrObjNo2 == 0x4FFF) {
				DemoInt0 = *(Ui32*)p;
				DemoUi16[0] = *((Ui16 *)(p + 0));
				DemoUi16[1] = *((Ui16 *)(p + 2));
			}
			else if (ErrObjNo2 == 0x104B) {
				DemoInt0 = *(Ui32*)p;
				DemoUi16[0] = *((Ui16 *)(p + 0));
				DemoUi16[1] = *((Ui16 *)(p + 2));
			}
#endif
			if (*(Ui16 *)p == pObjA->_MsgId) {///< object including
				i = *(Ui16 *)(p + 2);///< length;
				Nxt = DKRTU_Obj_FindNextSameLevel(p, pObjA);
				if (Ret < _DKRTU_OBJ_NULL) {///< fail
#if DKRTU_PARSOBJ_DEMO
					ErrCode2 = _DKRTU_OBJ_ERR_NOTFOUND;
#endif
					return _DKRTU_OBJ_ERR_NOTFOUND;
				}
				else if (Ret == _DKRTU_OBJ_OK) {///< do not need to test length because it is Ok
					//...
				}
				else if (Ret == _DKRTU_OBJ_LVLEND) {///< do not need to test length because it is Ok
					//...
				}
				else if (Ret == _DKRTU_OBJ_LAST) {///< this is the last object of this level so check length
					if (j != (i + 4)) {
#if DKRTU_PARSOBJ_DEMO
						ErrCode2 = _DKRTU_OBJ_ERR_LENGTH;
#endif
						return _DKRTU_OBJ_ERR_LENGTH;
					}
				}
				else {///< unknown return value !!!
#if DKRTU_PARSOBJ_DEMO
					ErrCode2 = _DKRTU_OBJ_ERR_NOTFOUND;
#endif
					return _DKRTU_OBJ_ERR_NOTFOUND;
				}
				pObjN = pObjA;
				pObjN++;
				p += 4;
				j -= 4;
				if ((pObjN->_ObjOpt == 0xAA) && (pObjN->_ObjLvl == 0x55) && (pObjN->_MsgId == 0xA55A)) {
#if DKRTU_PARSOBJ_DEMO
					ErrCode2 = _DKRTU_OBJ_OK;
#endif
					return j;//remaining data
				}
				else if (pObjN->_ObjLvl > pObjA->_ObjLvl) {///< low level object
					//..
				}
				else {///< same or upper level object
					p += i;
					j -= i;
				}
				pObjA++;
			}
			else if (pObjA->_ObjOpt == _OBJ_OPT) {///< optional object not including
				///< continue...
			}
			else {///< mandatory object but not including
#if DKRTU_PARSOBJ_DEMO
				ErrCode2 = _DKRTU_OBJ_ERR_NOTFOUND;
#endif
				return _DKRTU_OBJ_ERR_NOTFOUND;
			}
		}
		//-----------------------------//
#if DKRTU_PARSOBJ_DEMO
		ErrCode2 = -50;///< undef
#endif
		return j;//remaining data  _DKRTU_OBJ_OK;
	}
	else {
#if DKRTU_PARSOBJ_DEMO
		ErrCode2 = _DKRTU_OBJ_ERR_LENGTH;
#endif
		return _DKRTU_OBJ_ERR_LENGTH;
	}
}

