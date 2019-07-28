#include "pch.h"
#undef UNICODE
/*
=========== Demo Configurations ================
Server IP				192.168.2.149	
Client 1				192.168.2.145	74-D4-35-3C-4A-B2		74-D4-35-3C-4A-B2-74-D4-35-3C-4A-B2		
Client 2				192.168.2.146	74-D4-35-3C-4A-B3		74-D4-35-3C-4A-B3-74-D4-35-3C-4A-B3
Device 1				192.168.2.147
Device 1 Ghost			192.168.2.148
*/
#include <stdio.h>
#include <time.h>
#include <windows.h>

#include <AY_Printf.h>
#include <AY_Functions.h>
#include <AY_ClientSocket.h>
#include <AY_Client.h>
#include <AY_ClientPrjtBased.h>
#include <AY_Crypt.h>
#include <AY_Memory.h>
#include <AY_Command.h>
#include <AY_File.h>


AY_GlobalRAM			AY_Ram;
static void				*pMainSocket;
static void				*pMacSocket;
//static ip_address		MyIP_Address;
static ip_addressAll	MyIP_Addresses;
#define MyIP_Address	MyIP_Addresses._ip
static uip_eth_addr		MyEth_Address;
static Ui08				MyUnique_ID[12];
static ip_address		SrvIP_Address;
static uip_eth_addr		SrvEth_Address;
static ip_address		GwIP_Address;
static Ui16				MyClientInstPort;
static Ui08				MySocketBuff[1536];
static char				DNS_Searching[40];
AY_DeviceRead			*pAY_Clients;
static udp_headerAll	AY_UDPheader;
static Ui32				AY_ClientInitLoop;

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
	struct tm *ltime;
	char timestr[16];
	ip_header *ih;
	udp_header *uh;
	Ui32 ip_len;
	Ui16 sport, dport;
	time_t local_tv_sec;

	printf("Main Socket Callback\n");
	/*
	 * unused parameter
	 */
	(void)(param);

	/* convert the timestamp to readable format */
	local_tv_sec = header->ts.tv_sec;
	ltime = localtime(&local_tv_sec);
	strftime(timestr, sizeof timestr, "%H:%M:%S", ltime);

	/* print timestamp and length of the packet */
	printf("%s.%.6d len:%d ", timestr, header->ts.tv_usec, header->len);

	/* retireve the position of the ip header */
	ih = (ip_header *)(pkt_data + 14); //length of ethernet header

	/* retireve the position of the udp header */
	ip_len = (ih->ver_ihl & 0xf) * 4;
	uh = (udp_header *)((ui08*)ih + ip_len);

	/* convert from network byte order to host byte order */
	sport = _HTONS(uh->sport);
	dport = _HTONS(uh->dport);

	/* print ip addresses and udp ports */
	printf("%d.%d.%d.%d.%d -> %d.%d.%d.%d.%d\n",
		ih->saddr.byte1,
		ih->saddr.byte2,
		ih->saddr.byte3,
		ih->saddr.byte4,
		sport,
		ih->daddr.byte1,
		ih->daddr.byte2,
		ih->daddr.byte3,
		ih->daddr.byte4,
		dport);
}

void AY_SocketRead_CallBack(Ui08 *param, const struct pcap_pkthdr *header, const Ui08 *pkt_data) {
	uip_eth_hdr		*pHdr;
	ip_header		*ih;
	udp_headerAll	*pUDP;
	struct DNS_HEADER *dns = NULL;

	printf("Read Socket Callback\n");

	/* retireve the position of the ethernet header */
	pHdr = (uip_eth_hdr *)(pkt_data + 0); // ethernet header
	ih = (ip_header *)(pkt_data + 14); //length of ethernet header
	pUDP = (udp_headerAll *)(pkt_data + 0); // udp all header

	if (!AY_Client_GetMACadr) {

		printf("MAC Packet Received !!!\n\n\n===============================================\n\n\n===============================================\n\n\n");
		/* print ip addresses and udp ports */
		printf("%02x-%02x-%02x-%02x-%02x-%02x -> %02x-%02x-%02x-%02x-%02x-%02x\n\n\n\n",
			pHdr->src.addr[0],
			pHdr->src.addr[1],
			pHdr->src.addr[2],
			pHdr->src.addr[3],
			pHdr->src.addr[4],
			pHdr->src.addr[5],
			pHdr->dest.addr[0],
			pHdr->dest.addr[1],
			pHdr->dest.addr[2],
			pHdr->dest.addr[3],
			pHdr->dest.addr[4],
			pHdr->dest.addr[5]
		);
		if ((ih->saddr.byte1 == MyIP_Address.byte1) && (ih->saddr.byte2 == MyIP_Address.byte2) && (ih->saddr.byte3 == MyIP_Address.byte3) && (ih->saddr.byte4 == MyIP_Address.byte4)) {
			MyEth_Address = pHdr->src;
			SrvEth_Address = pHdr->dest;
			//GwIP_Address = ih->daddr;
		}
		else {
			MyEth_Address = pHdr->dest;
			SrvEth_Address = pHdr->src;
			//GwIP_Address = ih->saddr;
		}
		AY_Client_GetMACadr = 1;
	}
	else if ((!AY_Client_GetSrvIPadr)){
		if ((pUDP->_udpHeader.sport == _HTONS(DNS_Port))) {
			if ((pUDP->_udpHeader.dport == _HTONS((MyClientInstPort + 1)))) {
				unsigned char *qname, *reader;
				struct RES_RECORD answer;
				int i, stop;
				Ui32 j;
				char Temp[40];

				printf("DNS Packet Received !!!\n\n\n===============================================\n\n\n===============================================\n\n\n");

				dns = (struct DNS_HEADER*) (pkt_data + sizeof(udp_headerAll));
				printf("\nThe response contains : ");
				printf("\n %d Questions.", ntohs(dns->q_count));
				printf("\n %d Answers.", ntohs(dns->ans_count));
				printf("\n %d Authoritative Servers.", ntohs(dns->auth_count));
				printf("\n %d Additional records.\n\n", ntohs(dns->add_count));

				//point to the query portion
				qname = (Ui08 *)(pkt_data + sizeof(udp_headerAll) + sizeof(struct DNS_HEADER));

				//move ahead of the dns header and the query field
				reader = (Ui08 *)(pkt_data + sizeof(udp_headerAll) + sizeof(struct DNS_HEADER) + (strlen((const char*)qname) + 1) + sizeof(struct QUESTION));
				//Start reading answers
				stop = 0;

				for (i = 0; i < _HTONS(dns->ans_count); i++)
				{
					answer.name = ReadName(reader, (Ui08 *)(pkt_data + sizeof(udp_headerAll)), &stop);
					reader = reader + stop;

					answer.resource = (struct R_DATA*)(reader);
					reader = reader + sizeof(struct R_DATA);

					if (_HTONS(answer.resource->type) == 1) //if its an ipv4 address
					{
						answer.rdata = (unsigned char*)_AY_MallocMemory(_HTONS(answer.resource->data_len));

						for (j = 0; j < _HTONS(answer.resource->data_len); j++)
						{
							answer.rdata[j] = reader[j];
						}

						answer.rdata[_HTONS(answer.resource->data_len)] = '\0';

						reader = reader + _HTONS(answer.resource->data_len);

						printf("Name : %s ", answer.name);

						if (_HTONS(answer.resource->type) == T_A) //IPv4 address
						{
							printf("has IPv4 address : %s", AY_ConvertIPToStrRet(answer.rdata, Temp));
							if ((strstr((char *)DNS_Searching, (char *)answer.name) != nullptr)) {
#if CLIENT_DEMO
									SrvIP_Address.byte1 = 192;
									SrvIP_Address.byte2 = 168;
									SrvIP_Address.byte3 = 2;
									SrvIP_Address.byte4 = 149;
#else
								*((Ui32 *)&SrvIP_Address) = *((Ui32 *)answer.rdata);
#endif
									/* print ip addresses and udp ports */
									printf("\n\nSrvIP_Address = %d.%d.%d.%d\n",
										SrvIP_Address.byte1,
										SrvIP_Address.byte2,
										SrvIP_Address.byte3,
										SrvIP_Address.byte4);
									AY_Client_GetSrvIPadr = 1;
									return;
							}
						}

						if (_HTONS(answer.resource->type) == 5)
						{
							//Canonical name for an alias
							printf("has alias name : %s", answer.rdata);
						}

						printf("\n");
					}
					else
					{
						answer.rdata = ReadName(reader, (Ui08 *)(pkt_data + sizeof(udp_headerAll)), &stop);
						reader = reader + stop;
					}
				}
			}
		}
	}
	else if ((!AY_Client_RecvServer)) {
		int i;
		Ui08 Temp[45];
		AY_DeviceStartResp	*pRsp = (AY_DeviceStartResp *)(pkt_data + sizeof(udp_headerAll)); // 
		AY_DeviceRead		*pDev = (AY_DeviceRead *)(pkt_data + sizeof(udp_headerAll) + sizeof(AY_DeviceStartResp)); // 
		if ((pUDP->_udpHeader.sport == _HTONS(SERVER_Port))) {
			if ((pRsp->_Test0 == PACKET_TEST_DATA0) && (pRsp->_Test1 == PACKET_TEST_DATA1)) {
				AY_Ram.AY_DeviceCnt = pRsp->_DevCnt;
				if (AY_Ram.AY_DeviceCnt) {
					memcpy(&AY_Ram.AY_DeviceList[0], pDev, (AY_Ram.AY_DeviceCnt * sizeof(AY_DeviceRead)));
					AY_Decrypt_AES128((Ui08 *)&AY_Ram.AY_Sessionkey[0], (Ui08 *)&AY_Ram.AY_DeviceList[0], (AY_Ram.AY_DeviceCnt * sizeof(AY_DeviceRead)));
				}
				printf("\nDevice List Downloaded Packet Length = %d, DevCnt=%d\n", header->len, AY_Ram.AY_DeviceCnt);
				for (i = 0; i < AY_Ram.AY_DeviceCnt; i++) {
					printf("Device No:%d ID:%d Unq0:0x%08x Unq1:0x%08x  Unq2:0x%08x  Parent:%d Type:%d LocalIP:%s\n", i, AY_Ram.AY_DeviceList[i]._id, AY_Ram.AY_DeviceList[i]._Unique[0], AY_Ram.AY_DeviceList[i]._Unique[1], AY_Ram.AY_DeviceList[i]._Unique[2], AY_Ram.AY_DeviceList[i]._ParentId, AY_Ram.AY_DeviceList[i]._Type, AY_ConvertIPToStrRet((Ui08 *)&AY_Ram.AY_DeviceList[i]._LocalIp, (char*)&Temp[0]));
					//AYCMD_TakeThisIP((Ui08 *)&AY_Ram.AY_DeviceList[i]._LocalIp);
				}
				AY_Client_RecvServer = 1;
			}
		}
	}
	else {
		struct tm *ltime;
		char timestr[16];
		ip_header *ih;
		udp_header *uh;
		Ui32 ip_len;
		Ui16 sport, dport;
		time_t local_tv_sec;

		/* convert the timestamp to readable format */
		local_tv_sec = header->ts.tv_sec;
		ltime = localtime(&local_tv_sec);
		strftime(timestr, sizeof timestr, "%H:%M:%S", ltime);

		/* print timestamp and length of the packet */
		printf("%s.%.6d len:%d ", timestr, header->ts.tv_usec, header->len);

		/* retireve the position of the ip header */
		ih = (ip_header *)(pkt_data + 14); //length of ethernet header

		/* retireve the position of the udp header */
		ip_len = (ih->ver_ihl & 0xf) * 4;
		uh = (udp_header *)((ui08*)ih + ip_len);

		/* convert from network byte order to host byte order */
		sport = _HTONS(uh->sport);
		dport = _HTONS(uh->dport);

		/* print ip addresses and udp ports */
		printf("%d.%d.%d.%d.%d -> %d.%d.%d.%d.%d\n",
			ih->saddr.byte1,
			ih->saddr.byte2,
			ih->saddr.byte3,
			ih->saddr.byte4,
			sport,
			ih->daddr.byte1,
			ih->daddr.byte2,
			ih->daddr.byte3,
			ih->daddr.byte4,
			dport);
	}
}

int AY_DNS_Query(char *pDNS, ip_address *pDNS_SRV) {
	udp_headerAll		UDPheader;
	unsigned char		*qname;
	int					i;

	struct DNS_HEADER *dns = NULL;
	struct QUESTION *qinfo = NULL;

	//============= SET FILTER ==========================//
	AY_ClientFilterFreeA(_SLVS_SCKT);
	strcpy((char *)&MySocketBuff[0], "udp src port ");
	AY_ConvertUi32AddToStrRet( DNS_Port, (char *)&MySocketBuff[0] );
	strcat((char *)&MySocketBuff[0], " and udp dst port ");
	AY_ConvertUi32AddToStrRet((MyClientInstPort + 1), (char *)&MySocketBuff[0] );
	AY_ClientFilterSetA(_SLVS_SCKT, (char *)&MySocketBuff[0]);
	
	//============= SEND PACKET ==========================//
	UDP_header_init(&UDPheader);
	UDP_header_load(&UDPheader, SrvEth_Address/**((uip_eth_addr *)&DEMO_Mac[0])*/, *((ip_address *)&DNS_SRV_IP[0]), DNS_Port, MyEth_Address, MyIP_Address, (MyClientInstPort+1));

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
	AY_ClientFilterFreeA(_SLVS_SCKT);
	strcpy((char *)&MySocketBuff[0], "udp src port ");
	AY_ConvertUi32AddToStrRet(SERVER_Port, (char *)&MySocketBuff[0]);
	AY_ClientFilterSetA(_SLVS_SCKT, (char *)&MySocketBuff[0]);

	//------- LOAD
	memset(&DevStrt,0,sizeof(DevStrt));
	DevStrt._Test0 = PACKET_TEST_DATA0;
	DevStrt._Test1 = PACKET_TEST_DATA1;
	DevStrt._LocalCertNo = LOCAL_CertNo;
	DevStrt._ServerCertNo = SERVER_CertNo;
	memcpy(&DevStrt._Name[0], &MY_NAME, sizeof(MY_NAME));
	memcpy(&DevStrt._Pswd[0], &MY_PASSWORD, sizeof(MY_PASSWORD));
	memcpy(&DevStrt._MAC, &MyEth_Address, sizeof(MyEth_Address));
	memcpy(&DevStrt._Unique, &MyUnique_ID, sizeof(MyUnique_ID));
	//------ Generate AES Key
	AY_Generate_AES128(&DevStrt._SessionKey[0]);
	memcpy(&AY_Ram.AY_Sessionkey[0], &DevStrt._SessionKey[0],16);
	//------ ENCRPT
	AY_Crypt_RSAEncrpt((Ui08 *)&SERVER_PUB_KEY[0], (Ui08 *)&DevStrt._Input[0], 240, (Ui08 *)&OutData[0], &oLen);
	memcpy(&DevStrt._Input[0], &OutData[0], oLen);
	//------- SIGN
	AY_Crypt_RSASign((Ui08 *)&SIGNING_PR_KEY[0], (Ui08 *)&DevStrt._Input[0], 256, (Ui08 *)&DevStrt._Sign[0]);
	//------- SEND
	UDP_header_init(&UDPheader);
	UDP_header_load(&UDPheader, SrvEth_Address, SrvIP_Address, SERVER_Port, MyEth_Address, MyIP_Address, MyClientInstPort);
	oLen = sizeof(AY_DeviceStart);
	return ( UDP_packet_send(_MAIN_SCKT, &UDPheader, (Ui08 *)&DevStrt, oLen/*sizeof(AY_DeviceStart)*/) );

}

int main(void)//(int argc, char **argv)
{
	int i,j=0;
	Ui08 packet[114];
	char GetVal;

	MyClientInstPort = MyUDP_StartPort;
	AY_Client_Flags = 0;///< clear all flags
	AY_ClientInitLoop = 0;
	while (1) {
#if CLIENT_DEMO
		if (!AY_Client_Intro) {
			printf("============ CLIENT DEMO PROJECT =================\n\n");
			printf("Enter Client No (1-2)\n\n"); 
			GetVal = getchar();
			AY_Client_Intro = 1;
		}
		else if (!AY_Client_Initied) {
			if ((GetVal != '1') && (GetVal != '2')) {
				printf("ERROR !!!!\n\n\n\n");
				printf("Try Again\n\n\n\n");
				AY_Client_Intro = 0;
			}
			else {
				if (GetVal == '1') {
					memcpy(&SrvIP_Address.byte1, &DEMO_SRV_IP[0], 4);
					memcpy(&MyIP_Address.byte1, &DEMO_CLNT_IP[0][0], 4);
					memcpy(&MyEth_Address.addr[0], &DEMO_CLNT_MAC[0][0], 6);
					memcpy(&MyUnique_ID[0], &DEMO_CLNT_UNIQUE[0][0], 12);
					memcpy(&SrvEth_Address.addr[0], &DEMO_Mac[0], 6);
				} 
				else{
					memcpy(&SrvIP_Address.byte1, &DEMO_SRV_IP[1], 4);
					memcpy(&MyIP_Address.byte1, &DEMO_CLNT_IP[1][0], 4);
					memcpy(&MyEth_Address.addr[0], &DEMO_CLNT_MAC[1][0], 6);
					memcpy(&MyUnique_ID[0], &DEMO_CLNT_UNIQUE[1][0], 12);
					memcpy(&SrvEth_Address.addr[0], &DEMO_Mac[0], 6);
				}
				strcpy((char *)&MySocketBuff[0], "udp src port ");
				AY_ConvertUi32AddToStrRet(SERVER_Port, (char *)&MySocketBuff[0] );
				if (AY_ClientSocket_Init(_MAIN_SCKT, (Ui08 *)&MyMac[0], &MyIP_Address.byte1, SERVER_Port, 0, AY_MainSocket_CallBack, AY_ClientInitLoop) == 1) {
					AY_Client_Initied = 1;
					AY_Client_GetMACadr = 1;
					AY_Client_WaitMACadr = 1;
				}

			}
		}
		else if (!AY_Client_GetSrvIPadr) {
			strcpy((char *)&MySocketBuff[0], "udp src port ");
			AY_ConvertUi32AddToStrRet(DNS_Port, (char *)&MySocketBuff[0] );
			strcat((char *)&MySocketBuff[0], " and udp dst port ");
			AY_ConvertUi32AddToStrRet((MyClientInstPort + 1), (char *)&MySocketBuff[0] );
			if (AY_ClientSocket_Init(_SLVS_SCKT, (Ui08 *)&MyMac[0], &MyIP_Address.byte1, SERVER_Port, 0, AY_SocketRead_CallBack, AY_ClientInitLoop) == 1) {
				AY_Client_GetSrvIPadr = 1;
				AY_Client_WaitSrvIPadr = 1;
			}
		}
#else
		if (!AY_Client_Intro) {
			printf("============ CLIENT PROJECT =================\n\n");
			printf("================ START =================\n\n");
			AY_Client_Intro = 1;
		}
		else if (!AY_Client_Initied) {
			Ui08 Temp[45];
			MyIP_Address.byte1 = 0;	MyIP_Address.byte2 = 0;	MyIP_Address.byte3 = 0;	MyIP_Address.byte4 = 0;
			if (AY_ClientSocket_Init(_MAIN_SCKT, (Ui08 *)&MyMac[0], &MyIP_Address.byte1, SERVER_Port, 0, AY_MainSocket_CallBack, AY_ClientInitLoop) == 1) {

				printf("IP address: %s\n", AY_ConvertIPToStrRet((Ui08 *)(((Ui32 *)&MyIP_Address.byte1) + _IP_), (char*)&Temp[0]));
				printf("Subnet address: %s\n", AY_ConvertIPToStrRet((Ui08 *)(((Ui32 *)&MyIP_Address.byte1) + _SUBNET_), (char*)&Temp[0]));
				printf("Subnet mask: %s\n", AY_ConvertIPToStrRet((Ui08 *)(((Ui32 *)&MyIP_Address.byte1) + _MASK_), (char*)&Temp[0]));
				printf("Gateway address: %s\n", AY_ConvertIPToStrRet((Ui08 *)(((Ui32 *)&MyIP_Address.byte1) + _GW_), (char*)&Temp[0]));

				AY_Client_Initied = 1;
			}
		}
		else if (!AY_Client_GetMACadr) {
			if (!AY_Client_WaitMACadr) {
				if (AY_ClientSocket_Init(_SLVS_SCKT, (Ui08 *)&MyMac[0], &MyIP_Address.byte1, 0, 0, AY_SocketRead_CallBack, AY_ClientInitLoop) == 1) {
					AY_Client_WaitMACadr = 1; 
					j = 0;
				}
			}
			else {		
				if (++j < 10) {
					AY_Delay(1000);
				}
				else {
					AY_ClientInitLoop ++;
					AY_ClientFilterFreeB(_MAIN_SCKT);
					AY_ClientFilterFreeB(_SLVS_SCKT);
					AY_Client_WaitMACadr = 0;
					AY_Client_Initied = 0;
				}
			}
		}
		else if (!AY_Client_GetSrvIPadr) {
			if (!AY_Client_WaitSrvIPadr) {
				if (AY_DNS_Query((char *)SERVER_Dns, (ip_address *)&DNS_SRV_IP) == 1) {
					AY_Client_WaitSrvIPadr = 1;
					j = 0;
				}
			}
			else {
				if (++j < 5) {
					AY_Delay(1000);
				}
				else {
					AY_Client_WaitSrvIPadr = 0;
				}
			}
		}
#endif
		else if (!AY_Client_SendServer) {
			Ui08 Temp[45];

			AYFILE_TestConfigFile(1);
			AYFILE_ConfigFileReadComp((char *)&Temp[0], ServerDns);
			AYFILE_ConfigFileWriteComp((char *)"167", DNSPort);

			//AY_Ram.AY_DeviceCnt = 5;
			//for (i = 0; i < AY_Ram.AY_DeviceCnt; i++) {
			//	AY_Ram.AY_DeviceList[i]._id = 1;
			//	AY_Ram.AY_DeviceList[i]._Unique[0] = 1;
			//	AY_Ram.AY_DeviceList[i]._Unique[1] = 2;
			//	AY_Ram.AY_DeviceList[i]._Unique[2] = 3;
			//	AY_Ram.AY_DeviceList[i]._ParentId = 6;
			//	AY_Ram.AY_DeviceList[i]._Type = 1;
			//	AY_Ram.AY_DeviceList[i]._LocalIp = 0xD502a8c0+(0x01000000*i);///< 192.168.2.213
			//	printf("Device No:%d ID:%d Unq0:0x%08x Unq1:0x%08x  Unq2:0x%08x  Parent:%d Type:%d LocalIP:%s\n", i, AY_Ram.AY_DeviceList[i]._id, AY_Ram.AY_DeviceList[i]._Unique[0], AY_Ram.AY_DeviceList[i]._Unique[1], AY_Ram.AY_DeviceList[i]._Unique[2], AY_Ram.AY_DeviceList[i]._ParentId, AY_Ram.AY_DeviceList[i]._Type, AY_ConvertIPToStrRet((Ui08 *)&AY_Ram.AY_DeviceList[i]._LocalIp, (char*)&Temp[0]));
			//}
			//AYFILE_OpenFile((char *)AddIP_File);
			//AYFILE_ClearFile((char *)AddIP_File);
			//AYFILE_AddIPsFileStart((char *)AddIP_File, (char *)&MyInterface[0], 1);
			//for (i = 0; i < AY_Ram.AY_DeviceCnt; i++) {
			//	AYFILE_AddIPsToFile((char *)AddIP_File, (char *)&MyInterface[0], &AY_Ram.AY_DeviceList[i]._LocalIp, 1, AY_Ram.AY_DeviceList[i]._LocalIp, AY_Ram.AY_DeviceList[i]._LocalIp,1);
			//}
			//AYFILE_AddIPsFileStop((char *)AddIP_File, (char *)&MyInterface[0], 1);
			//AYFILE_CloseFile((char *)AddIP_File);

			////!AY_SendDeviceStartToServer();			
			
			AY_Client_SendServer = 1;
		}
		else if (!AY_Client_RecvServer) {
			//.. wait


			//AY_Client_RecvServer = 1;
		}
		else if (!AY_Client_GenerateRemoteDevs) {
			if (++j < 300) {
				AY_Delay(1000);
			}
			else {
				AY_Client_SendServer = 0;
			}
			
			
			
			/*j++;
			if (j > 1000) {
				j = 0;
			}*/
			//UDP_header_init(&AY_UDPheader);
			//UDP_header_load(&AY_UDPheader, *((uip_eth_addr *)&DEMO_Mac[0]), *((ip_address *)&DEMO_SRV_IP[0]), DEMO_SRV_Port, *((uip_eth_addr *)&DEMO_Mac[0]), MyIP_Address, MyClientInstPort);

			///* Fill the rest of the packet */
			//for (i = 0; i < 114; i++) {
			//	packet[i] = (Ui08)i;
			//}

			//while (1) {
			//	UDP_packet_send(pMainSocket, &AY_UDPheader, &packet[0], 114);
			//}
			//
			//
			//if (AY_SendDeviceStartToServer() == 1) {
			//	AY_Client_SendServer = 1;
			//}
		}

	}

	//===================================================//

	UDP_header_init(&AY_UDPheader);
	UDP_header_load(&AY_UDPheader, *((uip_eth_addr *)&DEMO_Mac[0]), *((ip_address *)&DEMO_SRV_IP[0]), DEMO_SRV_Port, *((uip_eth_addr *)&MyMac[0]), MyIP_Address, MyClientInstPort);

	/* Fill the rest of the packet */
	for (i = 0; i < 114; i++) {
		packet[i] = (Ui08)i;
	}

	while (1) {
		UDP_packet_send(_MAIN_SCKT, &AY_UDPheader, &packet[0], 114);
	}



	//AY_ClientSocket_main();
	return 0;
}


