#pragma once

#include <messages/DKRTU_OD.h>
/* REQUEST CONTENT  */
#define IRSC_REQUESTCONTENTSIZE 				20

#define IRSC_SIZE_REQ_RELAYIDENTITYCODE			1
#define IRSC_SIZE_REQ_RELAYOPERATION           	1
#define IRSC_SIZE_REQ_RELAYADDRESS				1


/*
typedef struct _DK_RTU_Req_Content_IntegratedRelayStateChange {
	DK_RTU_ListType  	Req_RelayStateChange;
	struct _Req_RelayStateChangeList {
		DK_RTU_SimpleType Req_RelayIdentityCode;
		DK_RTU_SimpleType Req_RelayOperation;
		DK_RTU_SimpleType Req_RelayAddress;
	}Req_RelayStateChangeList;
}DK_RTU_Req_Content_IntegratedRelayStateChange;



 extern DK_RTU_MsgFrame RTUMsg_IntegratedRelayStateChange_Req;


*/

/* RESPONSE CONTENT */
#define IRSC_RESPONSECONTENTSIZE 				20

#define IRSC_SIZE_OPERATIONRESULTINFORMATION	1
#define IRSC_SIZE_LOGCODE                   	1
#define IRSC_SIZE_RELAYIDENTITYCODE          	1
#define IRSC_SIZE_RELAYOPERATION             	4
#define IRSC_SIZE_INFORMATION                	2
#define IRSC_SIZE_KEY                      		2
#define IRSC_SIZE_VALUE                    		2

 /*

typedef struct _DK_RTU_Content_IntegratedRelayStateChange {
	DK_RTU_SimpleType 	OperationResultInformation;
	DK_RTU_ListType  	OperationLogs;
	struct _OperationLogsList {
		DK_RTU_ObjectType LogDataObject;
		DK_RTU_SimpleType LogCode;
		DK_RTU_SimpleType RelayIdentityCode;
		DK_RTU_SimpleType RelayOperation;
		DK_RTU_SimpleType Information;
		DK_RTU_ListType   AdditionalData;
		struct _AdditionalDataList {
			DK_RTU_ObjectType KeyValueObject;
			DK_RTU_SimpleType Key;
			DK_RTU_SimpleType Value;
		} AdditionalDataList;
	}OperationLogsList;
}DK_RTU_Content_IntegratedRelayStateChange;

extern DK_RTU_MsgFrame RTUMsg_IntegratedRelayStateChange;
*/

