#include "pch.h"
#undef UNICODE
/*
=========== Demo Configurations ================
									|192.168.2.149|(SRV)
				|192.168.2.145|(GW1)						|192.168.2.146|(GW2)
				<|192.168.2.147|>(GHS1)							||192.168.2.148||(DEV1)
<<192.168.2.150>>

Server IP				192.168.2.149
Client 1				192.168.2.145	(1)		74-D4-35-3C-4A-B2		74-D4-35-3C-4A-B2-74-D4-35-3C-4A-B2
Client 2				192.168.2.146	(2)		74-D4-35-3C-4A-B3		74-D4-35-3C-4A-B3-74-D4-35-3C-4A-B3
Device 1 Ghost			192.168.2.147	()		(Client 2)
Device 1 				192.168.2.148	(4)		(Client 1)
Demo-1					192.168.2.150	(3)

PACKET (1)				DEMO1	--->	GHS1		(A request packet for Mirror Device)
PACKET (2)				GW1		--->	GW2			(Send request packet to GW includes real device)
PACKET (3)				GW2		--->	DEV1		(Send request packet to real device)
PACKET (4)				DEV1	--->	GW2			(A response packet from real Device)
PACKET (5)				GW2		--->	GW1			(Send response packet to GW includes mirror device)
PACKET (6)				GHS1	--->	DEMO1		(Send response packet to target device)

PACKET (A)				GW1		--->	SRV			(GW info request)
PACKET (B)				SRV		--->	GW2			(GW info rent)
PACKET (C)				SRV		--->	GW1			(GW info response)


*/
#include <stdio.h>
#include <time.h>
#include <windows.h>

#include <AY_Printf.h>
#include <AY_Functions.h>
#include <AY_Socket.h>
#include <AY_Demo.h>
#include <AY_DemoPrjtBased.h>
#include <AY_Crypt.h>
#include <AY_Memory.h>
//#include <AY_Command.h>
#include <AY_File.h>
#include <AY_DemoDev.h>
#define DK_DEMO							0//1
#if DK_DEMO
#include <DKRTU_MsgObjects.h>
#endif


AY_GlobalRAM			AY_Ram;
static void				*pMainSocket;
static void				*pMacSocket;
//static ip_address		MyIP_Address;
static ip_addressAll	MyIP_Addresses;
#define MyIP_Address	MyIP_Addresses._ip
static uip_eth_addr		MyEth_Address;
//static Ui08				MyUnique_ID[12];
static ip_address		SrvIP_Address;
static uip_eth_addr		SrvEth_Address;
static ip_address		GwIP_Address;
static Ui16				MyDemoInstPort;
static Ui08				MySocketBuff[1536];
static char				DNS_Searching[40];
AY_DeviceRead			*pAY_Demos;
static udp_headerAll	AY_UDPheader;
static Ui32				AY_DemoInitLoop;
static char				GetVal;
static char				DeviceType;
static udp_headerAll	UDPheader_Buff;

//struct timeval {
//	long    tv_sec;         /* seconds */
//	long    tv_usec;        /* and microseconds */
//};
struct pcap_pkthdr {
	struct timeval ts;	/* time stamp */
	Ui32 caplen;	/* length of portion present */
	Ui32 len;	/* length this packet (off wire) */
};


void AY_MainSocket_CallBack(Ui08 *param, const struct pcap_pkthdr *header, const Ui08 *pkt_data) {
	uip_eth_hdr		*pHdr;
	ip_header		*ih;
	udp_headerAll	*pUDP;
	struct DNS_HEADER *dns = NULL;

	printf("Main Socket Callback\n");

	/* retireve the position of the ethernet header */
	pHdr = (uip_eth_hdr *)(pkt_data + 0); // ethernet header
	ih = (ip_header *)(pkt_data + 14); //length of ethernet header
	pUDP = (udp_headerAll *)(pkt_data + 0); // udp all header
	if (DeviceType == 4) {
		AY_Demo_DemoPacketReceived = 1;
#if STEP_TEST == 1
		printf("********* STEP 9 *************\n********* STEP 9 *************\n********* STEP 9 *************\n");
		AYPRINT_UDP_Header(pUDP);
#endif	
	}
	else {
#if STEP_TEST == 1
		printf("********* STEP 12 *************\n********* STEP 12 *************\n********* STEP 12 *************\n");
		AYPRINT_UDP_Header(pUDP);
#endif		
	}
}
#if 0
//
//void AY_SocketRead_CallBack(Ui08 *param, const struct pcap_pkthdr *header, const Ui08 *pkt_data) {
//	uip_eth_hdr		*pHdr;
//	ip_header		*ih;
//	udp_headerAll	*pUDP;
//	struct DNS_HEADER *dns = NULL;
//
//	printf("Read Socket Callback\n");
//
//	/* retireve the position of the ethernet header */
//	pHdr = (uip_eth_hdr *)(pkt_data + 0); // ethernet header
//	ih = (ip_header *)(pkt_data + 14); //length of ethernet header
//	pUDP = (udp_headerAll *)(pkt_data + 0); // udp all header
//
//	if (!AY_Demo_GetMACadr) {
//
//		printf("MAC Packet Received !!!\n\n\n===============================================\n\n\n===============================================\n\n\n");
//		/* print ip addresses and udp ports */
//		printf("%02x-%02x-%02x-%02x-%02x-%02x -> %02x-%02x-%02x-%02x-%02x-%02x\n\n\n\n",
//			pHdr->src.addr[0],
//			pHdr->src.addr[1],
//			pHdr->src.addr[2],
//			pHdr->src.addr[3],
//			pHdr->src.addr[4],
//			pHdr->src.addr[5],
//			pHdr->dest.addr[0],
//			pHdr->dest.addr[1],
//			pHdr->dest.addr[2],
//			pHdr->dest.addr[3],
//			pHdr->dest.addr[4],
//			pHdr->dest.addr[5]
//		);
//		if ((ih->saddr.byte1 == MyIP_Address.byte1) && (ih->saddr.byte2 == MyIP_Address.byte2) && (ih->saddr.byte3 == MyIP_Address.byte3) && (ih->saddr.byte4 == MyIP_Address.byte4)) {
//			MyEth_Address = pHdr->src;
//			SrvEth_Address = pHdr->dest;
//			//GwIP_Address = ih->daddr;
//		}
//		else {
//			MyEth_Address = pHdr->dest;
//			SrvEth_Address = pHdr->src;
//			//GwIP_Address = ih->saddr;
//		}
//		AY_Demo_GetMACadr = 1;
//	}
//	else if ((!AY_Demo_GetSrvIPadr)){
//		if ((pUDP->_udpHeader.sport == _HTONS(CngFile.DNSPort))) {
//			if ((pUDP->_udpHeader.dport == _HTONS((MyDemoInstPort + 1)))) {
//				unsigned char *qname, *reader;
//				struct RES_RECORD answer;
//				int i, stop;
//				Ui32 j;
//				char Temp[40];
//
//				printf("DNS Packet Received !!!\n\n\n===============================================\n\n\n===============================================\n\n\n");
//
//				dns = (struct DNS_HEADER*) (pkt_data + sizeof(udp_headerAll));
//				printf("\nThe response contains : ");
//				printf("\n %d Questions.", ntohs(dns->q_count));
//				printf("\n %d Answers.", ntohs(dns->ans_count));
//				printf("\n %d Authoritative Servers.", ntohs(dns->auth_count));
//				printf("\n %d Additional records.\n\n", ntohs(dns->add_count));
//
//				//point to the query portion
//				qname = (Ui08 *)(pkt_data + sizeof(udp_headerAll) + sizeof(struct DNS_HEADER));
//
//				//move ahead of the dns header and the query field
//				reader = (Ui08 *)(pkt_data + sizeof(udp_headerAll) + sizeof(struct DNS_HEADER) + (strlen((const char*)qname) + 1) + sizeof(struct QUESTION));
//				//Start reading answers
//				stop = 0;
//
//				for (i = 0; i < _HTONS(dns->ans_count); i++)
//				{
//					answer.name = ReadName(reader, (Ui08 *)(pkt_data + sizeof(udp_headerAll)), &stop);
//					reader = reader + stop;
//
//					answer.resource = (struct R_DATA*)(reader);
//					reader = reader + sizeof(struct R_DATA);
//
//					if (_HTONS(answer.resource->type) == 1) //if its an ipv4 address
//					{
//						answer.rdata = (unsigned char*)_AY_MallocMemory(_HTONS(answer.resource->data_len));
//
//						for (j = 0; j < _HTONS(answer.resource->data_len); j++)
//						{
//							answer.rdata[j] = reader[j];
//						}
//
//						answer.rdata[_HTONS(answer.resource->data_len)] = '\0';
//
//						reader = reader + _HTONS(answer.resource->data_len);
//
//						printf("Name : %s ", answer.name);
//
//						if (_HTONS(answer.resource->type) == T_A) //IPv4 address
//						{
//							printf("has IPv4 address : %s", AY_ConvertIPToStrRet(answer.rdata, Temp));
//							if ((strstr((char *)DNS_Searching, (char *)answer.name) != nullptr)) {
//#if Demo_DEMO
//									SrvIP_Address.byte1 = 192;
//									SrvIP_Address.byte2 = 168;
//									SrvIP_Address.byte3 = 2;
//									SrvIP_Address.byte4 = 149;
//#else
//								*((Ui32 *)&SrvIP_Address) = *((Ui32 *)answer.rdata);
//#endif
//									/* print ip addresses and udp ports */
//									printf("\n\nSrvIP_Address = %d.%d.%d.%d\n",
//										SrvIP_Address.byte1,
//										SrvIP_Address.byte2,
//										SrvIP_Address.byte3,
//										SrvIP_Address.byte4);
//									AY_Demo_GetSrvIPadr = 1;
//									return;
//							}
//						}
//
//						if (_HTONS(answer.resource->type) == 5)
//						{
//							//Canonical name for an alias
//							printf("has alias name : %s", answer.rdata);
//						}
//
//						printf("\n");
//					}
//					else
//					{
//						answer.rdata = ReadName(reader, (Ui08 *)(pkt_data + sizeof(udp_headerAll)), &stop);
//						reader = reader + stop;
//					}
//				}
//			}
//		}
//	}
//	else if ((!AY_Demo_RecvServer)) {
//		Ui32 i,j,k;
//		Ui32 m, n;
//		Ui08 Temp[45];
//		AY_DeviceStartResp	*pRsp = (AY_DeviceStartResp *)(pkt_data + sizeof(udp_headerAll)); // 
//		AY_DeviceRead		*pDev = (AY_DeviceRead *)(pkt_data + sizeof(udp_headerAll) + sizeof(AY_DeviceStartResp)); // 
//		if ((pUDP->_udpHeader.sport == _HTONS(CngFile.ServerPort))) {
//			if ((pRsp->_Test0 == PACKET_TEST_DATA0) && (pRsp->_Test1 == PACKET_TEST_DATA1)) {
//				if (AY_Ram.AY_DevPcktNo == pRsp->_DevPcktNo){
//					j = pRsp->_DevCnt;
//					k = AY_Ram.AY_DeviceCnt;
//					if (j) {
//						AY_Decrypt_AES128((Ui08 *)&AY_Ram.AY_Sessionkey[0], (Ui08 *)pDev/*&AY_Ram.AY_DeviceList[0]*/, header->len-(sizeof(udp_headerAll) + sizeof(AY_DeviceStartResp))/*(j * sizeof(AY_DeviceRead))*/);
//						memcpy(&AY_Ram.AY_DeviceList[0], pDev, (j * sizeof(AY_DeviceRead)));
//					}
//
//					//============== FILE========================//
//					printf("AYDVSTRT--> SSK = "); AY_HexValPrint((Ui08 *)&AY_Ram.AY_Sessionkey[0] , 16); printf("\r\n");
//					printf("AYDVSTRT--> %d ============ FILE START =========\n ",(header->len  - sizeof(udp_headerAll)));
//					m = sizeof(AY_DeviceStartResp) + (((Ui16)j) * sizeof(AY_DeviceRead));
//					n = 0;
//					while (m > 32) {
//						AY_HexValPrint((((Ui08 *)pRsp) + n), 32);
//						printf("\r\n ");
//						n += 32;
//						m -= 32;
//					}
//					if (m) {
//						AY_HexValPrint((((Ui08 *)pRsp) + n), m);
//						printf("\r\n ");
//					}
//					printf("AYDVSTRT--> ============ FILE END =========\n ");
//					//===========================================//
//					printf("\nDevice List Downloaded Packet Length = %d, DevCnt=%d\n", header->len, j);
//					for (i = 0; i < j; i++) {
//						printf("Device No:%d ID:%d Unq0:0x%08x Unq1:0x%08x  Unq2:0x%08x  Parent:%d Type:%d LocalIP:%s\n", i, AY_Ram.AY_DeviceList[i]._id, AY_Ram.AY_DeviceList[i]._Unique[0], AY_Ram.AY_DeviceList[i]._Unique[1], AY_Ram.AY_DeviceList[i]._Unique[2], AY_Ram.AY_DeviceList[i]._ParentId, AY_Ram.AY_DeviceList[i]._Type, AY_ConvertIPToStrRet((Ui08 *)&AY_Ram.AY_DeviceList[i]._LocalIp, (char*)&Temp[0]));
//						AY_Demo_AddDevToList((Ui08 *)&AY_Ram.AY_DeviceList[i], (k+i), _DEV_READ_ALL);
//						if (AY_Ram.AY_DeviceList[i]._Type== _MIRROR_) {
//							printf("Remote Device found Device No:%d ID:%d Type:%d LocalIP:%s\n", i, AY_Ram.AY_DeviceList[i]._id,  AY_Ram.AY_DeviceList[i]._Type, AY_ConvertIPToStrRet((Ui08 *)&AY_Ram.AY_DeviceList[i]._LocalIp, (char*)&Temp[0]));
//							AYFILE_AddIPsToFile((char*)&AddIP_File[0], CngFile.NetInterfaceName, &AY_Ram.AY_DeviceList[i]._LocalIp, 1, *((Ui32*)&CngFile.NetworkSubnetMask[0]), *((Ui32*)&CngFile.NetworkGatewayIp[0]), 1);
//						}
//					}
//					AY_Ram.AY_DeviceCnt += j;
//					AY_Ram.AY_DevPcktNo++;
//					if (j < 168) {
//						AY_Demo_RecvServer = 1;
//					}
//				}
//			}
//		}
//	}
//	else {
//		struct tm *ltime;
//		char timestr[16];
//		ip_header *ih;
//		udp_header *uh;
//		Ui32 ip_len;
//		Ui16 sport, dport;
//		time_t local_tv_sec;
//
//		/* convert the timestamp to readable format */
//		local_tv_sec = header->ts.tv_sec;
//		ltime = localtime(&local_tv_sec);
//		strftime(timestr, sizeof timestr, "%H:%M:%S", ltime);
//
//		/* print timestamp and length of the packet */
//		printf("%s.%.6d len:%d ", timestr, header->ts.tv_usec, header->len);
//
//		/* retireve the position of the ip header */
//		ih = (ip_header *)(pkt_data + 14); //length of ethernet header
//
//		/* retireve the position of the udp header */
//		ip_len = (ih->ver_ihl & 0xf) * 4;
//		uh = (udp_header *)((ui08*)ih + ip_len);
//
//		/* convert from network byte order to host byte order */
//		sport = _HTONS(uh->sport);
//		dport = _HTONS(uh->dport);
//
//		/* print ip addresses and udp ports */
//		printf("%d.%d.%d.%d.%d -> %d.%d.%d.%d.%d\n",
//			ih->saddr.byte1,
//			ih->saddr.byte2,
//			ih->saddr.byte3,
//			ih->saddr.byte4,
//			sport,
//			ih->daddr.byte1,
//			ih->daddr.byte2,
//			ih->daddr.byte3,
//			ih->daddr.byte4,
//			dport);
//	}
//}
#endif 

int AY_DNS_Query(char *pDNS, ip_address *pDNS_SRV) {
	udp_headerAll		UDPheader;
	unsigned char		*qname;
	int					i;

	struct DNS_HEADER *dns = NULL;
	struct QUESTION *qinfo = NULL;

	//============= SET FILTER ==========================//
	AYSCKT_FilterFreeA(_SLVS_SCKT);
	strcpy((char *)&MySocketBuff[0], "udp src port ");
	AY_ConvertUi32AddToStrRet(CngFile.DNSPort, (char *)&MySocketBuff[0] );
	strcat((char *)&MySocketBuff[0], " and udp dst port ");
	AY_ConvertUi32AddToStrRet((MyDemoInstPort + 1), (char *)&MySocketBuff[0] );
	AYSCKT_FilterSetA(_SLVS_SCKT, (char *)&MySocketBuff[0]);
	
	//============= SEND PACKET ==========================//
	UDP_header_init(&UDPheader);
	UDP_header_load(&UDPheader, SrvEth_Address/**((uip_eth_addr *)&DEMO_Mac[0])*/, *((ip_address *)&CngFile.DNSIp[0]), CngFile.DNSPort, MyEth_Address, MyIP_Address, (MyDemoInstPort+1));

	//Set the DNS structure to standard queries
	dns = (struct DNS_HEADER *)&MySocketBuff[0];
	dns->id = (unsigned short)_HTONS(GetCurrentProcessId ());
	dns->qr = 0; //This is a query
	dns->opcode = 0; //This is a standard query
	dns->aa = 0; //Not Authoritative
	dns->tc = 0; //This message is not truncated
	dns->rd = 1; //Recursion Desired
	dns->ra = 0; //Recursion not available! hey we dont have it (lol)
	dns->z = 0;
	dns->ad = 0;
	dns->cd = 0;
	dns->rcode = 0;
	dns->q_count = _HTONS(1); //we have only 1 question
	dns->ans_count = 0;
	dns->auth_count = 0;
	dns->add_count = 0;
	//point to the query portion
	qname = (unsigned char*)&MySocketBuff[sizeof(struct DNS_HEADER)];
	i = strlen(pDNS);
	//memcpy(qname, pDNS, i);
	strcpy(DNS_Searching,pDNS);
	ChangetoDnsNameFormat(qname, (unsigned char*)DNS_Searching);

	qinfo = (struct QUESTION*)&MySocketBuff[sizeof(struct DNS_HEADER) + (strlen((const char*)qname) + 1)]; //fill it
	qinfo->qtype = _HTONS(T_A); //type of the query , A , MX , CNAME , NS etc
	qinfo->qclass = _HTONS(1); //its internet (lol)

	return (UDP_packet_send(_SLVS_SCKT, &UDPheader, (Ui08 *)&MySocketBuff, sizeof(struct DNS_HEADER) + (strlen((const char*)qname) + 1) + sizeof(struct QUESTION)));
}

int AY_SendDeviceStartToServer(void) {
	udp_headerAll		UDPheader;
	AY_DeviceStart		DevStrt;
	Ui08				OutData[256];
	Ui16				oLen;

	//============= SET FILTER ==========================//
	AYSCKT_FilterFreeA(_SLVS_SCKT);
	strcpy((char *)&MySocketBuff[0], "udp src port ");
	AY_ConvertUi32AddToStrRet(CngFile.ServerPort, (char *)&MySocketBuff[0]);
	AYSCKT_FilterSetA(_SLVS_SCKT, (char *)&MySocketBuff[0]);

	//------- LOAD
	memset(&DevStrt,0,sizeof(DevStrt));
	DevStrt._Test0 = PACKET_TEST_DATA0;
	DevStrt._Test1 = PACKET_TEST_DATA1;
	DevStrt._LocalCertNo = Demo_CERT_NO;
	DevStrt._ServerCertNo = CngFile.ServerID;
	memcpy(&DevStrt._Name[0], &CngFile.GatewayName, sizeof(CngFile.GatewayName));
	memcpy(&DevStrt._Pswd[0], &CngFile.GatewayPass, sizeof(CngFile.GatewayPass));
	memcpy(&DevStrt._MAC, &MyEth_Address, sizeof(MyEth_Address));
	memcpy(&DevStrt._Unique, &CngFile.UniqueID, sizeof(CngFile.UniqueID));
	//------ Generate AES Key
	AY_Generate_AES128(&DevStrt._SessionKey[0]);
	memcpy(&AY_Ram.AY_Sessionkey[0], &DevStrt._SessionKey[0],16);
	//------ ENCRPT
	AY_Crypt_RSAEncrpt((Ui08 *)&/*SERVER_PUB_KEY*/CngFile.ServerPublicKey[0], (Ui08 *)&DevStrt._Input[0], 240, (Ui08 *)&OutData[0], &oLen);
	memcpy(&DevStrt._Input[0], &OutData[0], oLen);
	//------- SIGN
	AY_Crypt_RSASign((Ui08 *)&SIGNING_PR_KEY[0], (Ui08 *)&DevStrt._Input[0], 256, (Ui08 *)&DevStrt._Sign[0]);
	//------- SEND
	UDP_header_init(&UDPheader);
	UDP_header_load(&UDPheader, SrvEth_Address, SrvIP_Address, CngFile.ServerPort, MyEth_Address, MyIP_Address, MyDemoInstPort);
	oLen = sizeof(AY_DeviceStart);
	return ( UDP_packet_send(_MAIN_SCKT, &UDPheader, (Ui08 *)&DevStrt, oLen/*sizeof(AY_DeviceStart)*/) );

}

int AY_StartSlaveListenA(void) {
	//============= SET FILTER ==========================//
	// //ip.src != 192.168.2.144 && ip.dst != 192.168.2.144
	strcpy((char *)&MySocketBuff[0], "ip src host not ");
	AY_ConvertIPAddToStrRet(&MyIP_Address.byte1, (char*)&MySocketBuff[0]);
	strcat((char *)&MySocketBuff[0], " and ip dst host not ");
	AY_ConvertIPAddToStrRet(&MyIP_Address.byte1, (char*)&MySocketBuff[0]);
	return 1;
}
int AY_StartSlaveListen(void) {
	//============= SET FILTER ==========================//
#if 0//1
	Ui32 j = 1;
	// //ip.src != 192.168.2.144 && ip.dst != 192.168.2.144
	AYSCKT_FilterFreeA(_SLVS_SCKT);
	strcpy((char *)&MySocketBuff[0], "ip src host not ");
	AY_ConvertIPAddToStrRet(&MyIP_Address.byte1, (char*)&MySocketBuff[0]);
	strcat((char *)&MySocketBuff[0], " and ip dst host not ");
	AY_ConvertIPAddToStrRet(&MyIP_Address.byte1, (char*)&MySocketBuff[0]);
#else
	Ui32 i,j=0;
	AY_DEVINFO		*pDevIfo;
	// //ip.src == 192.168.2.147 || ip.src == 192.168.2.148
	AYSCKT_FilterFreeA(_SLVS_SCKT);

	for (i = 0; i < AY_Ram.AY_DeviceCnt; i++) {
		pDevIfo = pAY_FindDevInfoByDevNo(i);
		if (pDevIfo) {
			if (pDevIfo->DevRead._Type == _MIRROR_) {///< remote
				if (j) {
					strcat((char *)&MySocketBuff[0], " or ip dst host ");
				}
				else {
					strcpy((char *)&MySocketBuff[0], "ip dst host ");
				}
				AY_ConvertIPAddToStrRet((Ui08*)&pDevIfo->DevRead._LocalIp, (char*)&MySocketBuff[0]);
				j++;
			}
		}
	}	
	if (j > 49) {
		AY_StartSlaveListenA();
	}
#endif
	if (j) {
		AYSCKT_FilterSetA(_MAIN_SCKT, (char *)&MySocketBuff[0]);
	}
	else {
		MySocketBuff[0] = 0;
	}
	printf("_SLVS_SCKT filter: %s \r\n", (char *)&MySocketBuff[0]);
	return 1;
}


int AY_StartDemoListen(void) {
	//============= SET FILTER ==========================//
	// //ip.src != 192.168.2.144 && ip.dst != 192.168.2.144
	AYSCKT_FilterFreeA(_MAIN_SCKT);
	//AYSCKT_FilterFreeA(_SLVS_SCKT);
	strcpy((char *)&MySocketBuff[0], "ip dst host ");
	AY_ConvertIPAddToStrRet((Ui08*)&MyIP_Address.byte1, (char*)&MySocketBuff[0]);
	AYSCKT_FilterSetA(_MAIN_SCKT, (char *)&MySocketBuff[0]);
	return 1;
}

int AYDEMO_SendDemoPacket(void) {
	udp_headerAll		UDPheader;
	Ui16				oLen;
	//------- SEND
	UDP_header_init(&UDPheader);
	UDP_header_load(&UDPheader, SrvEth_Address, SrvIP_Address, CngFile.ServerPort, MyEth_Address, MyIP_Address, MyDemoInstPort);
	oLen = sizeof(DEMO_STR_1);
#if STEP_TEST==1
	printf("********* STEP 1 *************\n********* STEP 1 *************\n********* STEP 1 *************\n");
	AYPRINT_UDP_Header(&UDPheader);
#endif
	return (UDP_packet_send(_MAIN_SCKT, &UDPheader, (Ui08 *)&DEMO_STR_1, oLen/*sizeof(AY_DeviceStart)*/));
}

int AYDEMO_SendDemoPacket2(void) {
	udp_headerAll		UDPheader;
	Ui16				oLen;

	UDPheader = UDPheader_Buff;
	UDPheader._ethHeader.dest = UDPheader_Buff._ethHeader.src;
	UDPheader._ethHeader.src = MyEth_Address;// UDPheader_Buff._ethHeader.dest;
	UDPheader._ipHeader.daddr = UDPheader_Buff._ipHeader.saddr;
	UDPheader._ipHeader.saddr = UDPheader_Buff._ipHeader.daddr;
	UDPheader._udpHeader.dport = UDPheader_Buff._udpHeader.sport;
	UDPheader._udpHeader.sport = UDPheader_Buff._udpHeader.dport;

	//------- SEND
	//UDP_header_init(&UDPheader);
	//UDP_header_load(&UDPheader, SrvEth_Address, SrvIP_Address, CngFile.ServerPort, MyEth_Address, MyIP_Address, MyDemoInstPort);
	oLen = sizeof(DEMO_STR_2); 

#if STEP_TEST == 1
		printf("********* STEP 10 *************\n********* STEP 10 *************\n********* STEP 10 *************\n");
	AYPRINT_UDP_Header(&UDPheader);
#endif	
	return (UDP_packet_send(_MAIN_SCKT, &UDPheader, (Ui08 *)&DEMO_STR_2, oLen/*sizeof(AY_DeviceStart)*/));
}

int main(void)//(int argc, char **argv)
{
#if DK_DEMO
	while (1) {
		DKRTU_ParsObj_Test();
	}
#else	
	char *p;
	int i,j=0;
	Ui08 packet[114];

	MyDemoInstPort = MyUDP_StartPort;
	AY_Demo_Flags = 0;///< clear all flags
	AY_DemoInitLoop = 0;
	while (1) {
#if Demo_DEMO
		if (!AY_Demo_Intro) {
			printf("============ Demo DEMO PROJECT =================\n\n");
			printf("Enter Demo No (1-2)\n\n"); 
			GetVal = getchar();
			//------------------------------//
			AYFILE_TestCertFile(1);
			AYFILE_ReadCertFile();
			AYFILE_TestConfigFile(1);
			AYFILE_ConfigFileUpdate();
			//------------------------------//
			AY_Demo_Intro = 1;
		}
		else if (!AY_Demo_Initied) {
			if ((GetVal != '1') && (GetVal != '2')) {
				printf("ERROR !!!!\n\n\n\n");
				printf("Try Again\n\n\n\n");
				AY_Demo_Intro = 0;
			}
			else {
				if (GetVal == '1') {
					memcpy(&SrvIP_Address.byte1, &DEMO_SRV_IP[0], 4);
					memcpy(&MyIP_Address.byte1, &DEMO_CLNT_IP[0][0], 4);
					memcpy(&MyEth_Address.addr[0], &DEMO_CLNT_MAC[0][0], 6);
					memcpy(&CngFile.UniqueID[0], &DEMO_CLNT_UNIQUE[0][0], 12);
					memcpy(&SrvEth_Address.addr[0], &DEMO_Mac[0], 6);
				} 
				else{
					memcpy(&SrvIP_Address.byte1, &DEMO_SRV_IP[1], 4);
					memcpy(&MyIP_Address.byte1, &DEMO_CLNT_IP[1][0], 4);
					memcpy(&MyEth_Address.addr[0], &DEMO_CLNT_MAC[1][0], 6);
					memcpy(&CngFile.UniqueID[0], &DEMO_CLNT_UNIQUE[1][0], 12);
					memcpy(&SrvEth_Address.addr[0], &DEMO_Mac[0], 6);
				}
				strcpy((char *)&MySocketBuff[0], "udp src port ");
				AY_ConvertUi32AddToStrRet(CngFile.ServerPort, (char *)&MySocketBuff[0] );
				if (AYSCKT_Socket_Init(_MAIN_SCKT, (Ui08 *)&MyMac[0], &MyIP_Address.byte1, CngFile.ServerPort, 0, AY_MainSocket_CallBack, AY_DemoInitLoop) == 1) {
					AY_Demo_Initied = 1;
					AY_Demo_GetMACadr = 1;
					AY_Demo_WaitMACadr = 1;
				}

			}
		}
		else if (!AY_Demo_GetSrvIPadr) {
			strcpy((char *)&MySocketBuff[0], "udp src port ");
			AY_ConvertUi32AddToStrRet(CngFile.DNSPort, (char *)&MySocketBuff[0] );
			strcat((char *)&MySocketBuff[0], " and udp dst port ");
			AY_ConvertUi32AddToStrRet((MyDemoInstPort + 1), (char *)&MySocketBuff[0] );
			if (AYSCKT_Socket_Init(_SLVS_SCKT, (Ui08 *)&MyMac[0], &MyIP_Address.byte1, CngFile.ServerPort, 0, AY_SocketRead_CallBack, AY_DemoInitLoop) == 1) {
				AY_Demo_GetSrvIPadr = 1;
				AY_Demo_WaitSrvIPadr = 1;
			}
		}
#else
		if (!AY_Demo_Intro) {
			GetVal = 0;
#if Demo_DEMO2
			printf("============ Demo DEMO PROJECT =================\n\n");
			printf("Enter Demo No (3-4)\n\n");
			GetVal = getchar();
			if ((GetVal != '3') && (GetVal != '4')) {
				GetVal = 0;
			}
			else {
				GetVal -= '0';
			}

#endif
			printf("============ Demo PROJECT =================\n\n");
			printf("================ START =================\n\n");
			DeviceType = GetVal;
			AYFILE_SelectConfigFile(GetVal);
			AYFILE_TestCertFile(1);
			AYFILE_ReadCertFile();
			AYFILE_TestConfigFile(1);
			AYFILE_ConfigFileUpdate();
			AY_Demo_Intro = 1;
		}
		else if (!AY_Demo_Initied) {
			Ui08 Temp[45];
			if (AY_Demo_DynamicIP) {
				*((Ui32 *)&MyIP_Address) = 0;
			}
			else {
				*((Ui32 *)&MyIP_Address) = *((Ui32 *)&CngFile.NetIpAddress);
			}
			if (AYSCKT_Socket_Init(_MAIN_SCKT, (Ui08 *)&MyMac[0], &MyIP_Address.byte1, CngFile.ServerPort, 0, AY_MainSocket_CallBack, AY_DemoInitLoop) == 1) {

				printf("IP address: %s\n", AY_ConvertIPToStrRet((Ui08 *)(((Ui32 *)&MyIP_Address.byte1) + _IP_), (char*)&Temp[0]));
				printf("Subnet address: %s\n", AY_ConvertIPToStrRet((Ui08 *)(((Ui32 *)&MyIP_Address.byte1) + _SUBNET_), (char*)&Temp[0]));
				printf("Subnet mask: %s\n", AY_ConvertIPToStrRet((Ui08 *)(((Ui32 *)&MyIP_Address.byte1) + _MASK_), (char*)&Temp[0]));
				printf("Gateway address: %s\n", AY_ConvertIPToStrRet((Ui08 *)(((Ui32 *)&MyIP_Address.byte1) + _GW_), (char*)&Temp[0]));
				if (AY_IsStringToIP((char *)CngFile.ServerDns)) {
					p = (char *)CngFile.ServerDns;
					*((Ui32 *)&SrvIP_Address) = AY_ConvertStringToIP(&p);
				}
				AY_Demo_Initied = 1;
			}
		}
		else if (!AY_Demo_GetMACadr) {
			if (!AY_Demo_WaitMACadr) {
				if (AYSCKT_Socket_Init(_MAIN_SCKT, (Ui08 *)&MyMac[0], &MyIP_Address.byte1, 0, 0, AY_MainSocket_CallBack, AY_DemoInitLoop) == 1) {
					AY_Demo_WaitMACadr = 1; 
					j = 0;
				}
			}
			else {		
#if Demo_DEMO2
				if (GetVal > 0) {
					memcpy(&MyEth_Address.addr[0], &DEMO_CLNT_MAC[GetVal][0], 6);
					memcpy(&SrvEth_Address.addr[0], &DefaultMac/*MyMac*/[0], 6);
					AY_Demo_GetMACadr = 1;
				}

#else
				if (++j < 10) {
					AY_Delay(1000);
				}
				else {
					AY_DemoInitLoop ++;
					AYSCKT_FilterFreeB(_MAIN_SCKT);
					AYSCKT_FilterFreeB(_SLVS_SCKT);
					AY_Demo_WaitMACadr = 0;
					AY_Demo_Initied = 0;
				}
#endif
			}
		}
		else if (!AY_Demo_SendPacket) {
			if (!AY_Demo_DemoListen) {
				AY_StartDemoListen();
				AY_Demo_DemoListen = 1;
			}
			
			if (DeviceType == 3) {
				if (++j < 3) {
					AY_Delay(1000);
				}
				else {
					j = 0;
					AYDEMO_SendDemoPacket();
				}
			}
			else if(AY_Demo_DemoPacketReceived){
				AYDEMO_SendDemoPacket2();
				AY_Demo_DemoPacketReceived = 0;
			}
		}




		/*else if (!AY_Demo_GetSrvIPadr) {			
			if (AY_IsStringToIP((char *)CngFile.ServerDns)) {
				p = (char *)CngFile.ServerDns;
				*((Ui32 *)&SrvIP_Address) = AY_ConvertStringToIP(&p);
				AY_Demo_GetSrvIPadr = 1;
			}
			else {
				if (!AY_Demo_WaitSrvIPadr) {
					if (AY_DNS_Query((char *)CngFile.ServerDns, (ip_address *)&CngFile.DNSIp) == 1) {
						AY_Demo_WaitSrvIPadr = 1;
						j = 0;
					}
				}
				else {
					if (++j < 5) {
						AY_Delay(1000);
					}
					else {
						AY_Demo_WaitSrvIPadr = 0;
					}
				}
			}
		}*/
#endif
		//else if (!AY_Demo_SendServer) {
		//	AY_Ram.AY_DeviceCnt = 0;
		//	AY_Ram.AY_DevPcktNo = 0;
		//	AYFILE_ClearFile((char*)&AddIP_File[0]);
		//	AYFILE_AddIPsToFile((char*)&AddIP_File[0], CngFile.NetInterfaceName, 0, 0, 0, 0, 1);
		//	AY_SendDeviceStartToServer();			
		//	
		//	AY_Demo_SendServer = 1;
		//}
		//else if (!AY_Demo_RecvServer) {
		//	//.. wait
		//}
		//else if (!AY_Demo_GenerateRemoteDevs) {
		//	AYFILE_AddIPsToFile((char*)&AddIP_File[0], CngFile.NetInterfaceName, 0, 0xFFFF, 0, 0, 1);
		//	AYFILE_CloseFile((char*)&AddIP_File[0]);
		//	if (AY_Ram.AY_DevPcktNo >0) {
		//		AY_Demo_GenerateRemoteDevs = 1;
		//	}
		//	else {
		//		AY_Demo_SendServer = 0;
		//	}			
		//}
		//else if (!AY_Demo_ListenThreads) {
		//	AY_StartSlaveListen();
		//	AY_Demo_ListenThreads = 1;
		//}
		//else {
		//		///< check processes
		//}

	}

	//===================================================//

#endif
	return 0;
}


