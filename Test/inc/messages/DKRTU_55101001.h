
/*----------------------------------------------------------------------------*/
/*                                  DATAKOM                                   */
/*----------------------------------------------------------------------------*/

/******************************************************************************/
/*! \file DK_RTU_55101000.c
*
*
*
*   \Answer Ready State Request 55101001
*
*
*
*   \author
*   \version 1.01
*   \date    29/07/2019
*   History  \n
*		08.07.2019  Ver 1.00    Preliminary version, first Release 		 \n
*******************************************************************************/
#pragma once
#include <messages/DKRTU_OD.h>

/*Request*/
#define PUBLICKEYSTATELABEL       	 	0x1100
#define RSAPUBLICKEYLABEL             	0x1101
#define PUBLICKEYSTATESIZE   			0x0001
#define RSAPUBLICKEYSIZE		        0x0126

typedef struct{
	Ui16 Label;
	Ui16 Size;
	Ui08 PublicKeyBuffer[RSAPUBLICKEYSIZE];
}DKRTU_LoginContent_Request_Type;

typedef struct{
	DK_RTU_SimpleType1Byte 				PublicKeyStatus;
	DKRTU_LoginContent_Request_Type 	PublicKey;
}DKRTU_LoginContent_Request;

/*Response*/
#define ENCRYPTEDAESKEYLABEL			0x1102
#define ENCRYPTEDAESKEYSIZE				0x0100

typedef struct{
	Ui16 Label;
	Ui16 Size;
	Ui08 EncryptedAESkeyBuffer[ENCRYPTEDAESKEYSIZE];
}DKRTU_LoginContent_Response_Type;

typedef struct{
	DKRTU_LoginContent_Response_Type 	Encrypted_AesKey;
}DKRTU_LoginContent_Response;
