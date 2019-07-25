#include "pch.h"
#include <iostream>
#include <AY_Printf.h>

#if 1
void AY_Printf(const char str[]) {
	std::cout << str;
}
#else
typedef		const char			CnstCh;

void AY_Printf(CnstCh str0[], CnstCh str1[], CnstCh str2[], CnstCh str3[], CnstCh str4[], CnstCh str5[], CnstCh str6[], CnstCh str7[], CnstCh str8[], CnstCh str9[], CnstCh str10[], CnstCh str11[], CnstCh str12[], CnstCh str13[], CnstCh str14[], CnstCh str15[], CnstCh str16[]) {
	if (std::cout << str;
}
#endif