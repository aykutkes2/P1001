
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

#define LOGINCONFIRMATIONLABEL       	 	0x1103
#define LOGINCONFIRMATIONSIZE             	0x0010

#pragma pack(1) // exact fit - no padding

typedef struct{
	Ui16 Label;
	Ui16 Size;
	char ConfirmationBuffer[16];
}Dk_LoginConfirmationContent_Request_Type;

typedef struct{
	Dk_LoginConfirmationContent_Request_Type 	LoginConfirmation;
}Dk_LoginConfirmationContent_Request;

#pragma pack(pop)//back to whatever the previous packing mode was
