#pragma once

#if defined(_WIN32) || defined(EFIX64) || defined(EFI32)
#include <AY_Type.h>
#else
#include <DK_Type.h>
#endif
#define TOTALMESSAGENUMBER				5

#define SIZE_INSTRUCTIONSETIDENTITYCODE	4
#define SIZE_INSTRUCTIONSETVERSION 		3
#define SIZE_EVENTCODE             	    2
#define SIZE_MESSAGEARRAYIDENTITYCODE   16
#define SIZE_MESSAGEIDENTITYCODE        2
#define SIZE_OPERATIONTYPE              1
#define SIZE_OPERATIONPRIORITY          1
#define SIZE_SUPERIORSYSTEMCODE        	2
#define SIZE_USERROLE                   1
#define SIZE_DATEOFMESSAGE             	8
#define SIZE_RTUIDENTITYCODE			16

#define TITLESIZE	 100


#define RXDATABUFFERSIZE	1024
#define TXDATABUFFERSIZE	1024
#define RTU_UART_TIMEOUTVALUE 100 //ms

#ifdef __GNUC__
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

typedef struct _DK_RTU_RXTX{
    Ui08	TxBuffer[TXDATABUFFERSIZE];
    Ui08	RxBuffer[TXDATABUFFERSIZE];
}DK_RTU_RXTX;

typedef struct _DK_RTU_SimpleTypeValue {
	Ui08 Value;
}DK_RTU_SimpleTypeValue;

typedef struct _DK_RTU_SimpleType {
	Ui16 Label;
	Ui16 Size;
	DK_RTU_SimpleTypeValue* pSTValue;
}DK_RTU_SimpleType;

typedef struct _DK_RTU_ObjectType {
	Ui16 Label;
	Ui16 Size;
}DK_RTU_ObjectType;

typedef struct _DK_RTU_ListType {
	Ui16 Label;
	Ui16 Size;
}DK_RTU_ListType;
//#pragma pack(pop)//back to whatever the previous packing mode was


//#pragma pack(1) // exact fit - no padding
typedef struct _DK_RTU_SimpleType1Byte {
		Ui16 Label;
		Ui16 Size;
		Ui08 Value;
}DK_RTU_SimpleType1Byte;
//#pragma pack(pop)//back to whatever the previous packing mode was


//#pragma pack(1) // exact fit - no padding
typedef struct _DK_RTU_SimpleType2Byte {
		Ui16 Label;
		Ui16 Size;
		Ui08 Value[2];
}DK_RTU_SimpleType2Byte;
//#pragma pack(pop)//back to whatever the previous packing mode was


//#pragma pack(1) // exact fit - no padding
typedef struct _DK_RTU_SimpleType3Byte {
		Ui16 Label;
		Ui16 Size;
		Ui08 Value[3];
}DK_RTU_SimpleType3Byte;
//#pragma pack(pop)//back to whatever the previous packing mode was


//#pragma pack(1) // exact fit - no padding
typedef struct _DK_RTU_SimpleType4Byte {
		Ui16 Label;
		Ui16 Size;
		Ui08 Value[4];
}DK_RTU_SimpleType4Byte;
//#pragma pack(pop)//back to whatever the previous packing mode was


//#pragma pack(1) // exact fit - no padding
typedef struct _DK_RTU_SimpleType8Byte {
		Ui16 Label;
		Ui16 Size;
		Ui08 Value[8];
}DK_RTU_SimpleType8Byte;
//#pragma pack(pop)//back to whatever the previous packing mode was


//#pragma pack(1) // exact fit - no padding
typedef struct _DK_RTU_SimpleType12Byte {
		Ui16 Label;
		Ui16 Size;
		Ui08 Value[12];
}DK_RTU_SimpleType12Byte;
//#pragma pack(pop)//back to whatever the previous packing mode was


//#pragma pack(1) // exact fit - no padding
typedef struct _DK_RTU_SimpleType16Byte {
		Ui16 Label;
		Ui16 Size;
		Ui08 Value[16];
}DK_RTU_SimpleType16Byte;
//#pragma pack(pop)//back to whatever the previous packing mode was

//#pragma pack(1) // exact fit - no padding
typedef struct _DK_RTU_MsgTitle_rec {
		DK_RTU_SimpleType4Byte    InstructionSetIdentityCode;
		DK_RTU_SimpleType3Byte    InstructionSetVersion;
		DK_RTU_SimpleType2Byte    EventCode;
		DK_RTU_SimpleType16Byte   MessageArrayIdentityCode;
		DK_RTU_SimpleType2Byte    MessageIdentityCode;
		DK_RTU_SimpleType1Byte    OperationType;
		DK_RTU_SimpleType1Byte    OperationPriority;
		DK_RTU_SimpleType2Byte    SuperiorSystemCode;
		DK_RTU_SimpleType1Byte    UserRole;
		DK_RTU_SimpleType8Byte    DateofMessage;
		DK_RTU_SimpleType12Byte   RTUIdentityCode;
}DK_RTU_MsgTitle_rec;
//#pragma pack(pop)//back to whatever the previous packing mode was


//#pragma pack(1) // exact fit - no padding

typedef struct _S_RTU_bV0{
		Ui16                 MainMessageCode;    // Label
		Ui16                 FrameLength;        // Length

		Ui16                 MessageHeaderCode;  // Label
		Ui16                 HeaderLength;       // Only Header Length

		DK_RTU_MsgTitle_rec  MessageTitle;       // Content of header
}S_RTU_bV0;

typedef struct _S_RTU_bV1{
		Ui16                 MainMessageCode;    // Label
		Ui16                 FrameLength;        // Length

		Ui16                 MessageHeaderCode;  // Label
		Ui16                 HeaderLength;       // Only Header Length

		DK_RTU_MsgTitle_rec  MessageTitle;       // Content of header

		Ui16                 ContentMessageCode; // Label
		Ui16                 ContentLength;      // Only Header Length   

		Ui08				*pMessageContent;	 // Content Pointer
}S_RTU_bV1;

#ifdef __GNUC__
#pragma pack()
#else
#pragma pack(pop)
#endif


