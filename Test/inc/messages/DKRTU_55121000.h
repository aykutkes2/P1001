#pragma once

#include <messages/DKRTU_OD.h>

#pragma pack(1) // exact fit - no padding
typedef struct _DK_AnswerLogin {


	Ui08 AES_EncodedKey[16];


}DK_AnswerLogin;
#pragma pack(pop)//back to whatever the previous packing mode was
