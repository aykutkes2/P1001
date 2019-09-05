//#include "pch.h"
#undef UNICODE

#include <AY_Functions.h>

Si64 AY_ConvertStringToSi64(char *ptr) {
	Si64 Rslt = 0;
	Ui08 Sign = 0;
	while (*ptr == ' ') { ptr++; }
	if (*ptr == '-') {
		Sign = 1;
		ptr++;
	}
	while ((*ptr >= '0') && (*ptr <= '9')) {
		Rslt = (Rslt * 10) + (*ptr - '0');
		ptr++;
	}
	if (Sign) { Rslt = 0 - Rslt; }
	return (Rslt);
}
Ui64 AY_ConvertStringToUi64(char *ptr) {
	Ui64 Rslt = 0;
	while (*ptr == ' ') { ptr++; }
	while ((*ptr >= '0') && (*ptr <= '9')) {
		Rslt = (Rslt * 10) + (*ptr - '0');
		ptr++;
	}
	return (Rslt);
}
Ui32 AY_ConvertStringToUi32(char **ptr) {
	Ui32 Rslt = 0;
	Ui08 i = 0;
	char *p;
	p = *ptr;
	while ((++i < 200) && (*p != 0x00) && (!((*p >= '0') && (*p <= '9')))) { p++; }
	while ((*p >= '0') && (*p <= '9')) {
		Rslt = (Rslt * 10) + (*p - '0');
		p++;
	}
	*ptr = p;
	return (Rslt);
}
char *AY_ConvertUi32AddToStr0(Ui32 Val, char *ptr, char Add, char RetStrt) {
	char i = 0;
	char b[12];
	char *p = ptr;
	if (Add) {
		while (*ptr != 0) { ptr++; }
	}
	do {
		b[i] = (Val % 10) + '0';
		i++;
		Val /= 10;
	} while (Val);
	while (i) {
		i--;
		*ptr = b[i];
		ptr++;
	}
	*ptr = 0;
	if (RetStrt) { ptr = p; }
	return (ptr);
}
char *AY_ConvertUi64AddToStr0(Ui64 Val, char *ptr, char Add, char RetStrt) {
	char i = 0;
	char b[20];
	char *p = ptr;
	if (Add) {
		while (*ptr != 0) { ptr++; }
	}
	do {
		b[i] = (Val % 10) + '0';
		i++;
		Val /= 10;
	} while (Val);
	while (i) {
		i--;
		*ptr = b[i];
		ptr++;
	}
	*ptr = 0;
	if (RetStrt) { ptr = p; }
	return (ptr);
}
void AY_Sprintf(char *pDst, char *pSrc) {
	int i = 0;
	if (pSrc != nullptr) {
		while ((*pSrc != 0) && (i < 1024)) {
			*pDst = *pSrc;
			pDst++;
			pSrc++;
			i++;
		};
		*pDst = 0;
	}
	return;
}
void AY_Sprintf_Add(char *pDst, char *pSrc) {
	int i = 0;
	if (pSrc != nullptr) {
		while (*pDst != 0) { pDst++; }
		while ((*pSrc != 0) && (i < 1024)) {
			*pDst = *pSrc;
			pDst++;
			pSrc++;
			i++;
		};
		*pDst = 0;
	}
	return;
}

char *AY_ConvertIPAddToStr0(Ui08 *pVal, char *ptr, char Add, char RetStrt) {
	char j,i;
	char b[12];
	char *p = ptr;
	Ui08 Val;
	if (Add) {
		while (*ptr != 0) { ptr++; }
	}
	for (j = 0; j < 4; j++) {
		Val = pVal[j];
		i = 0;
		do {
			b[i] = (Val % 10) + '0';
			i++;
			Val /= 10;
		} while (Val);
		while (i) {
			i--;
			*ptr = b[i];
			ptr++;
		}
		if (j < 3) {
			*ptr = '.';
			ptr++;
		}
	}
	*ptr = 0;
	if (RetStrt) { ptr = p; }
	return (ptr);
}
Ui32 AY_ConvertStringToIP(char **ptr) {
	Ui32 Rslt = 0;
	Ui08 i = 0;
	Ui08 j = 0;
	Ui08 Vals[4] = { 0 };
	char *p;
	p = *ptr;
	while ((++i < 200) && (*p != 0x00) && (!((*p >= '0') && (*p <= '9')))) { p++; }
	for (j = 0; j < 4; j++) {
		while ((*p >= '0') && (*p <= '9')) {
			Vals[j] = (Vals[j] * 10) + (*p - '0');
			p++;
		}
		if (*p == '.') {
			p++;
		}
	}
	Rslt = ((((Ui32)Vals[3]) << 24) | (((Ui32)Vals[2]) << 16) | (((Ui32)Vals[1]) << 8) | (((Ui32)Vals[0]) << 0));
	*ptr = p;
	return (Rslt);
}
int AY_IsStringToIP(char *ptr) {
	Ui08 j,i = 0;
	char *p;
	p = ptr;
	j = 0;
	while((*p)&&(j<100)){
		if (!(((*p >= '0') && (*p <= '9')) || (*p == '.'))) {
			return -1;
		}
		else if (*p == '.') {
			i++;
		}
		else if (i >= 3) {
			break;
		}
		p++;
		j++;
	}
	return 1;
}

void AY_HexToStr(char *pDst, Ui08 *pSrc,Ui08 ByteCnt, Ui08 add) {
	int i = 0;
	char x, y;
	if (add) {
		while (*pDst != 0) { pDst++; }
	}
	if (ByteCnt) {
		while (ByteCnt) {
			x = (((*pSrc) & 0xF0) >> 4);
			y = (((*pSrc) & 0x0F) >> 0);
			*pDst = HexToChar(x);	pDst++;
			*pDst = HexToChar(y);	pDst++;
			pSrc++;
			ByteCnt--;
			if(add==2)			{ *pDst = '.';	pDst++; }///< IP Address
			else if (add == 3)	{ *pDst = ':';	pDst++; }///< MAC Address
		}
		*pDst = 0;
	}	
	return;
}
void AY_StrToHex(Ui08 *pDst, char *pSrc, Ui08 ByteCnt) {
	int i = 0;
	char x, y;
	
	x = *pSrc;
	while (IsHex(x)) {
		x = CharToHex(x);
		pSrc++; y = *pSrc;
		if (IsHex(y)) {
			y = CharToHex(y);
			x = (((x << 4) & 0xF0) | y);
			*pDst = x;
			pDst++;
			pSrc++; x = *pSrc;
		}
		else {
			return;
		}
	}
}


