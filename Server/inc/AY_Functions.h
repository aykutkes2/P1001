#pragma once
#include <AY_Type.h>

#pragma pack(push, 8)



#pragma pack(pop)


extern Si64 AY_ConvertStringToSi64(char *ptr);
extern Ui64 AY_ConvertStringToUi64(char *ptr);
extern Ui32 AY_ConvertStringToUi32(char **ptr);
extern char *AY_ConvertUi32AddToStr0(Ui32 Val, char *ptr, char Add, char RetStrt);
#define AY_ConvertUi32AddToStr(Val, ptr)		AY_ConvertUi32AddToStr0(Val, ptr, 1, 0)
#define AY_ConvertUi32ToStr(Val, ptr)			AY_ConvertUi32AddToStr0(Val, ptr, 0, 0)
#define AY_ConvertUi32ToStrRet(Val, ptr)		AY_ConvertUi32AddToStr0(Val, ptr, 0, 1)
#define AY_ConvertUi32AddToStrRet(Val, ptr)		AY_ConvertUi32AddToStr0(Val, ptr, 1, 1)
extern char *AY_ConvertUi64AddToStr0(Ui64 Val, char *ptr, char Add, char RetStrt);
#define AY_ConvertUi64AddToStr(Val, ptr)		AY_ConvertUi64AddToStr0(Val, ptr, 1,0)
#define AY_ConvertUi64ToStr(Val, ptr)			AY_ConvertUi64AddToStr0(Val, ptr, 0,0)
#define AY_ConvertUi64ToStrRet(Val, ptr)		AY_ConvertUi64AddToStr0(Val, ptr, 0,1)
extern void AY_Sprintf(char *pDst, char *pSrc);
extern void AY_Sprintf_Add(char *pDst, char *pSrc);

extern char *AY_ConvertIPAddToStr0(Ui08 *pVal, char *ptr, char Add, char RetStrt);
#define AY_ConvertIPAddToStr(Val, ptr)			AY_ConvertIPAddToStr0(Val, ptr, 1, 0)
#define AY_ConvertIPToStr(Val, ptr)				AY_ConvertIPAddToStr0(Val, ptr, 0, 0)
#define AY_ConvertIPToStrRet(Val, ptr)			AY_ConvertIPAddToStr0(Val, ptr, 0, 1)
#define AY_ConvertIPAddToStrRet(Val, ptr)		AY_ConvertIPAddToStr0(Val, ptr, 1, 1)
extern Ui32 AY_ConvertStringToIP(char **ptr);
extern int AY_IsStringToIP(char *ptr);

//---------------------------------------------------------------------------------------------//
#define HexToChar(a)	((a>9)?((a+'A'-0xA)):((a+'0')))
#define _CharToHex(a)	((a>='A')?((a+0xA-'A')):((a-'0')))
#define HexUpper(a)		((a>='a')?((a+'A'-'a')):((a)))
#define CharToHex(a)	_CharToHex(HexUpper(a))
#define IsHex(a)		((( ((a >= 'A') && (a <= 'F')) || ((a >= '0') && (a <= '9')) || ((a >= 'a') && (a <= 'f')) ))?(1):(0))
extern void AY_HexToStr(char *pDst, Ui08 *pSrc, Ui08 ByteCnt, Ui08 add);
extern void AY_StrToHex(Ui08 *pDst, char *pSrc, Ui08 ByteCnt);

