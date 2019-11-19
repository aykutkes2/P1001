//#include "pch.h"
#undef UNICODE

#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include <AY_Crypt.h>

#include "mbedtls/aes.h"
#include "mbedtls/rsa.h"
#include "mbedtls/pk.h"
#include "mbedtls/pk_internal.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/entropy.h"
#include "mbedtls/entropy_poll.h"
#include "mbedtls/sha512.h"




int AY_Crypt_AES128(Ui08 *pKey, Ui08 *pData, Ui16 Len) {
	mbedtls_aes_context ctx;
	unsigned char key[32];
	Ui16 j;
	Ui08 buff_orj[16];
	Ui08 buff_enc[16];

	//===== init
	memcpy(&key[0], pKey, 16);
	mbedtls_aes_init(&ctx);
	Len = ((Len + 15) >> 4);

	//==== encrpt
	mbedtls_aes_setkey_enc(&ctx, key, 128);
	for (j = 0; j < Len; j++) {
		memcpy(&buff_orj[0], pData, 16);
		memset(&buff_enc[0], 0, 16);
		mbedtls_aes_crypt_ecb(&ctx, MBEDTLS_AES_ENCRYPT, &buff_orj[0], &buff_enc[0]);
		memcpy(pData, &buff_enc[0], 16);
		pData += 16;
	}
	mbedtls_aes_free(&ctx);
	return (Len<<4);
}


int AY_Decrypt_AES128(Ui08 *pKey, Ui08 *pData, Ui16 Len) {
	mbedtls_aes_context ctx;
	unsigned char key[32];
	Ui16 j;
	Ui08 buff_enc[16];
	Ui08 buff_dec[16];

	//===== init
	memcpy(&key[0], pKey, 16);
	mbedtls_aes_init(&ctx);
	Len = ((Len + 15) >> 4);

	//==== decrpt
	mbedtls_aes_setkey_dec(&ctx, key, 128);
	for (j = 0; j < Len; j++) {
		memcpy(&buff_enc[0], pData, 16);
		memset(&buff_dec[0], 0, 16);
		mbedtls_aes_crypt_ecb(&ctx, MBEDTLS_AES_DECRYPT, &buff_enc[0], &buff_dec[0]);
		memcpy(pData, &buff_dec[0], 16);
		pData += 16;
	}
	mbedtls_aes_free(&ctx);
	return 1;
}

int AY_Generate_AES128(Ui08 *pKey) {
	int i;

	i = RAND_MAX;
	/* initialize random seed: */
	srand((unsigned int)time(NULL));

	for (i = 0; i < 16; i++) {
		*pKey = rand() % 256;
		pKey++;
	}
	return 1;	
}

int AY_Crypt_RSASign(Ui08 *pPrvK, Ui08 *Buffer, Ui16 Len, Ui08 *pSign) {
	Ui08 HashOutBuffer[64];
	Ui08 RSASignBuffer[256];
	Ui16 ret=0;
	Ui32 hash_len = 64;
	//Ui32 *sig_len;
	mbedtls_pk_context prktest;
	mbedtls_rsa_context *rsatest;

	mbedtls_pk_init(&prktest);

	ret = mbedtls_sha512_ret(Buffer, Len, HashOutBuffer, 0);
	if (ret != 0) { return -1; }
	ret = mbedtls_pk_parse_key(&prktest, pPrvK, strlen((const char*)pPrvK) + 1, NULL, 0);
	if (ret != 0) { return -1; }
	rsatest = (mbedtls_rsa_context *)(prktest).pk_ctx;

	mbedtls_rsa_pkcs1_sign(rsatest, NULL, NULL, MBEDTLS_RSA_PRIVATE, MBEDTLS_MD_SHA512,	hash_len, HashOutBuffer, RSASignBuffer);
	if (ret != 0) { return -1; }
	memcpy(pSign, RSASignBuffer, 256);
	return 1;
}

int AY_Crypt_RSAVerify(Ui08 *pPubK, Ui08 *Buffer, Ui16 Len, Ui08 *pSign) {
	mbedtls_pk_context PubKey_;
	Ui08 HashOutBuffer[64];
	Ui16 ret=0;
	size_t sig_len, hash_len;
	sig_len = 256;
	hash_len = 64;

	mbedtls_pk_init(&PubKey_);

	ret = mbedtls_sha512_ret(Buffer, Len, HashOutBuffer, 0);
	if (ret != 0) { return -1; }
	ret = mbedtls_pk_parse_public_key(&PubKey_, pPubK, strlen((const char*)pPubK) + 1);
	if (ret != 0) { return -1; }
	ret = mbedtls_pk_verify(&PubKey_, MBEDTLS_MD_SHA512, HashOutBuffer, hash_len, pSign, sig_len);
	if (ret != 0) { return -1; }
	return 1;
}

/*
	Generate random 48 bytes of data
*/
int mbedtls_hardware_poll(void *data, Ui08 *output, size_t len, size_t *olen) {
	int i;

	i = RAND_MAX;
	/* initialize random seed: */
	srand((unsigned int)time(NULL));

	for (i = 0; i < 48; i++) {
		*output = 0;
		while ((*output == 0) || (*output == 0xFF)) {
			*output = rand() % 256;
		}
		output++;
	}
	return 0;///< success for tls functions
	
}
int mbedtls_hardware_poll2(void *data, Ui08 *output, size_t len) {
	int i;
	return mbedtls_hardware_poll(data, output, len, (ui32 *)&i);
}

int AY_Crypt_RSAEncrpt(Ui08 *pPubK, Ui08 *Buffer, Ui16 Len, Ui08 *BufferO, Ui16 *oLen) {
	mbedtls_pk_context PubKey_;
	mbedtls_ctr_drbg_context ctr_drbg;
	Ui16 ret;
	size_t olen = 0;

	*oLen = 0;
	mbedtls_pk_init(&PubKey_);
	mbedtls_ctr_drbg_init(&ctr_drbg);
	
	ret = mbedtls_pk_parse_public_key(&PubKey_, pPubK, strlen((const char*)pPubK) + 1);
	if (ret == 0) {
		ret = mbedtls_pk_encrypt(&PubKey_, Buffer, Len, BufferO, &olen, Len + 1024, mbedtls_hardware_poll2/*mbedtls_ctr_drbg_random*/, &ctr_drbg);
		if (ret == 0) {
			*oLen = (Ui16)olen;
		}
	}

	mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_pk_free(&PubKey_);
	if (ret == 0) {
		return 1;
	}
	else {
		return -1;
	}
}

int AY_Crypt_RSADecrpt(Ui08 *pPrvK, Ui08 *Buffer, Ui16 Len, Ui08 *BufferO, Ui16 *oLen) {
	mbedtls_pk_context PrvKey_;
	mbedtls_ctr_drbg_context ctr_drbg;
	Ui16 ret;
	size_t olen = 0;

	*oLen = 0;
	mbedtls_pk_init(&PrvKey_);
	mbedtls_ctr_drbg_init(&ctr_drbg);

	ret = mbedtls_pk_parse_key(&PrvKey_, pPrvK, strlen((const char*)pPrvK) + 1, NULL, 0);
	if (ret == 0) {
		ret = mbedtls_pk_decrypt(&PrvKey_, Buffer, Len, BufferO, &olen, Len + 1024, mbedtls_hardware_poll2/*mbedtls_ctr_drbg_random*/, &ctr_drbg);
		if (ret == 0) {
			*oLen = (Ui16)olen;
		}
	}

	mbedtls_pk_free(&PrvKey_);
	mbedtls_ctr_drbg_free(&ctr_drbg);

	if (ret == 0) {
		return 1;
	}
	else {
		return -1;
	}
}

/*
	Generate random 4 bytes of data
*/
Ui32 AY_Crypt_GenerateUi32Random(void) {
	/* initialize random seed: */
	srand((unsigned int)time(NULL));
	return rand();///< success for tls functions
}

#if 0//RSA_DEMO_ACT
#include <AY_ClientPrjtBased.h>

int AY_Crypt_RSA_test(void) {
	const char TestData[] = "Merhaba Dunya. Aykut Yakisir test data for RSA 2048.17-07-2019 13:39. There is no greater threat to the critics and cynics and fearmongers than those of us who are willing to fall because we have learned how to rise. It’s the possibility of having a dream come true that makes life interesting.People think a soul mate is your perfect fit, and that’s what everyone wants. But a true soul mate is a mirror, the person who shows you everything that is holding you back, the person who brings you to your own attention so you can change your life.";
	char OrjData[1025];
	char EncData[2049];
	char DecData[2049];
	int o0, o1, o2, o3;
	Ui16 oLen0, oLen1;

	Ui08 Sign[256];

	memcpy(&OrjData[0], &TestData[0], sizeof(OrjData));
	memset(&EncData[0], 0, sizeof(EncData));
	memset(&DecData[0], 0, sizeof(DecData));

	o0 = AY_Crypt_RSASign((Ui08 *)&SIGNING_PR_KEY[0], (Ui08 *)&OrjData[0], 1024, &Sign[0]);

	o1 = AY_Crypt_RSAVerify((Ui08 *)&SIGNING_PUB_KEY[0], (Ui08 *)&OrjData[0], 1024, &Sign[0]);

	o2 = AY_Crypt_RSAEncrpt((Ui08 *)&SERVER_PUB_KEY[0], (Ui08 *)&OrjData[0], 240, (Ui08 *)&EncData[0], &oLen0);

	o3 = AY_Crypt_RSADecrpt((Ui08 *)&SERVER_PR_KEY[0], (Ui08 *)&EncData[0], oLen0, (Ui08 *)&DecData[0], &oLen1);

	while (1) {

	}
	return 1;

}
#endif




