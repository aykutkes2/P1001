#pragma once
#include <AY_Type.h>

#pragma pack(push, 8)

enum _MYSOCKETS {
	_MAIN_SCKT,
	_SLVS_SCKT
};

#define PACKET_TEST_DATA0			0x1EB99AB7
#define PACKET_TEST_DATA1			0x92D1D121

extern const Ui08 DEMO_SRV_IP[4];
extern const Ui08 DEMO_CLNT_IP[2][4];
extern const Ui08 DEMO_CLNT_MAC[2][6];
extern const Ui08 DEMO_CLNT_UNIQUE[2][12];
extern const Ui08 DEMO_Mac[6];
extern const Ui16 DEMO_SRV_Port;

extern const Ui16			SERVER_Port;
extern const Ui08			SERVER_Dns[40];
extern const Ui16			MyUDP_StartPort;
extern const Ui08			MyMac[6];
extern const Ui08			MyUnique[12];
extern const Ui08			MyInterface[45];

extern const Ui32			LOCAL_CertNo;
extern const Ui32			SERVER_CertNo;

extern const Ui08 MY_NAME[45];
extern const Ui08 MY_PASSWORD[45];

extern const Ui08 * const Default_Config_File;

extern const Ui08 * const SIGNING_PUB_KEY;
extern const Ui08 * const SIGNING_PR_KEY;
extern const Ui08 * const SERVER_PUB_KEY;
extern const Ui08 * const SERVER_PR_KEY;

//extern const Ui08			MyPublicKey[256];
//extern const Ui08			MyPrivateKey[256];
//extern const Ui08			MyRSA_CAR[8];
//extern const Ui08			MyRSA_Exp[8];
//extern const RSA2048		MyRSA2048_Cert;

extern const Ui08			DNS_SRV_IP[4];
extern const Ui16			DNS_Port;


extern const char TestData[65];
extern const Ui08 DemoAESkey[16];

#pragma pack(pop)



