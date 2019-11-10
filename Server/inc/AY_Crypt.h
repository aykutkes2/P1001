#pragma once
#include <AY_Type.h>

#define RSA_DEMO_ACT			1

#pragma pack(push, 1)


#pragma pack(pop)


extern int AY_Crypt_AES128(Ui08 *pKey, Ui08 *pData, Ui16 Len);

extern int AY_Decrypt_AES128(Ui08 *pKey, Ui08 *pData, Ui16 Len);

extern int AY_Generate_AES128(Ui08 *pKey);

extern int AY_Crypt_RSASign(Ui08 *pPrvK, Ui08 *Buffer, Ui16 Len, Ui08 *pSign);

extern int AY_Crypt_RSAVerify(Ui08 *pPubK, Ui08 *Buffer, Ui16 Len, Ui08 *pSign);

extern int AY_Crypt_RSAEncrpt(Ui08 *pPubK, Ui08 *Buffer, Ui16 Len, Ui08 *BufferO, Ui16 *oLen);

extern int AY_Crypt_RSADecrpt(Ui08 *pPrvK, Ui08 *Buffer, Ui16 Len, Ui08 *BufferO, Ui16 *oLen);

extern Ui32 AY_Crypt_GenerateUi32Random(void);

#if 0//RSA_DEMO_ACT
extern int AY_Crypt_RSA_test(void);
#endif
