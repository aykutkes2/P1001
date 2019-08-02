#pragma once
#include <messages/DKRTU_OD.h>

////////////  ReadyState and AnswerReadyStateRequest  //////////////

#define DeviceDateTime_RS          0x1022
#define CQI_CommunicationType_RS   0x2016
#define QualityValue_RS            0x1057
#define QualityBaseValue_RS        0x1058
#define QAV_Key_RS                 0x105F
#define QAV_Value_RS               0x1060
#define CoordinateSource_RS        0x200E
#define Latitude_RS                0x1033
#define Longitude_RS               0x1034
#define LocationInformation_RS     0x1035
#define LSD_Key_RS                 0x105F
#define LSD_Value_RS               0x1060
#define EquipmentIdentityCode_RS   0x1017
#define CI_CommunicationType_RS    0x2016
#define DeviceIP_RS                0x1052
#define CommunicationNo_RS         0x1053
#define DNS_RS                     0x1054
#define CommunicationOperator_RS   0x1055
#define CommunicationName_RS       0x1056
#define CSD_Key_RS                 0x105F
#define CSD_Value_RS               0x1060

#define CommunicationQualityKnowledgeList_RS    0x800D
#define CommunicationQualityKnowledgeObject_RS  0x4002
#define QualityAddedValue_RS                    0x8015
#define KeyValueObject_RS                       0x4017
#define locationInformationObject_RS            0x4007
#define locationSpecificAreasList_RS            0x8012
#define EquipmentIdCodeList_RS                  0x8002
#define CommunicationInformaitonList_RS         0x800E
#define CommunicationInformaitonObject_RS       0x4013
#define CommunucationSpecificValueList_RS       0x8014

#pragma pack(1) // exact fit - no padding
typedef struct _DK_ReadyState {
	DK_RTU_SimpleType8Byte 	DevTime;// DeviceDateTime;
	DK_RTU_ListType  	CommunicationQualityInfo;
	struct _CommunicationQualityInfoList {
		DK_RTU_ObjectType CommunicationQualityInfoObject;
		DK_RTU_SimpleType1Byte CQI_CommunicationType;
		DK_RTU_SimpleType1Byte QualityValue;
		DK_RTU_SimpleType1Byte QualityBaseValue;
		DK_RTU_ListType   QualityAdditionalValues;
		struct _QualityAdditionalValuesList {
			DK_RTU_ObjectType QAV_KeyValueObject;
			DK_RTU_SimpleType1Byte QAV_Key;
			DK_RTU_SimpleType1Byte QAV_Value;
		} QualityAdditionalValuesList;
	}CommunicationQualityInfoList;
	DK_RTU_ObjectType LocationInformation;
	struct _LocationInformationObject {
		DK_RTU_SimpleType1Byte CoordinateSource;
		DK_RTU_SimpleType1Byte Latitude;
		DK_RTU_SimpleType1Byte Longitude;
		DK_RTU_SimpleType1Byte LocationInformation;
		DK_RTU_SimpleType1Byte LocationSpecialData;
		struct _LocationSpecialDataList {
			DK_RTU_ObjectType LSD_KeyValueObject;
			DK_RTU_SimpleType1Byte LSD_Key;
			DK_RTU_SimpleType1Byte LSD_Value;
		}LocationSpecialDataList;
	}LocationInformationObject;
	DK_RTU_ListType EquipmentIdentityCode;
	struct _EquipmentIdentityCodeList {
		DK_RTU_SimpleType1Byte EquipmentIdentityCode;
	}EquipmentIdentityCodeList;
	DK_RTU_ListType CommunicationInformation;
	struct _CommunicationInformationList {
		DK_RTU_ObjectType CommunicationInformationObject;
		DK_RTU_SimpleType1Byte CI_CommunicationType;
		DK_RTU_SimpleType1Byte DeviceIP;
		DK_RTU_SimpleType1Byte CommunicationNo;
		DK_RTU_SimpleType1Byte DNS;
		DK_RTU_SimpleType1Byte CommunicationOperator;
		DK_RTU_SimpleType1Byte CommunicationName;
		DK_RTU_ListType	  CommunicationSpecialData;
		struct _CommunicationSpecialDataList {
			DK_RTU_ObjectType CSD_KeyValueObject;
			DK_RTU_SimpleType1Byte CSD_Key;
			DK_RTU_SimpleType1Byte CSD_Value;
		}CommunicationSpecialDataList;
	}CommunicationInformationList;
	DK_RTU_SimpleType1Byte PublicKeyRequest;
}DK_ReadyState;

#pragma pack(pop)//back to whatever the previous packing mode was

extern Ui08 reqanswer_flag;
extern Si16 DKRTU_ReadyState(Ui16 SlotNo);
