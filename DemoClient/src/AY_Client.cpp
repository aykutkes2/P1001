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
#include <AY_Client.h>
#include <AY_ClientPrjtBased.h>
#include <AY_Crypt.h>
#include <AY_Memory.h>
#include <AY_Command.h>
#include <AY_File.h>
#include <AY_ClientDev.h>
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
static Ui16				MyClientInstPort;
static Ui08				MySocketBuff[1536];
static char				DNS_Searching[40];
AY_DeviceRead			*pAY_Clients;
//static udp_headerAll	AY_UDPheader;
static tcp_headerAll	AY_TCPheader;
static Ui32				AY_ClientInitLoop;
static char				GetVal;

//struct timeval {
//	long    tv_sec;         /* seconds */
//	long    tv_usec;        /* and microseconds */
//};
struct pcap_pkthdr {
	struct timeval ts;	/* time stamp */
	Ui32 caplen;	/* length of portion present */
	Ui32 len;	/* length this packet (off wire) */
};


#define GW_DEVICE_LIST			((((AY_DeviceStartResp	*)pData)->_Test0 == PACKET_TEST_DATA0) && (((AY_DeviceStartResp	*)pData)->_Test1 == PACKET_TEST_DATA1))
#define GW_INFO_REQUEST			((((AY_GWINFORESP	*)pData)->_Test2 == PACKET_TEST_DATA2) && (((AY_GWINFORESP	*)pData)->_Test3 == PACKET_TEST_DATA3))
#define GW_PORT_RENT			((((AY_GWRENTRQST	*)pData)->_Test4 == PACKET_TEST_DATA4) && (((AY_GWRENTRQST	*)pData)->_Test5 == PACKET_TEST_DATA5))
#define NEW_REMOTE_PACKET		((((AY_GWDATAHDR	*)pData)->_Test6 == PACKET_TEST_DATA6) && (((AY_GWDATAHDR	*)pData)->_Test7 == PACKET_TEST_DATA7))
#define NEW_REMOTE_RESPONSE		((((AY_GWDATAHDR	*)pData)->_Test6 == PACKET_TEST_DATA8) && (((AY_GWDATAHDR	*)pData)->_Test7 == PACKET_TEST_DATA9))
//*****
#define DIRECT_SEND_REQUEST		((((AY_GWDRCTHDR	*)pData)->_Test10 == PACKET_TEST_DATA12) && (((AY_GWDRCTHDR	*)pData)->_Test11 == PACKET_TEST_DATA13))
#define DIRECT_SEND_RESPONSE	((((AY_GWDRCTHDR	*)pData)->_Test10 == PACKET_TEST_DATA14) && (((AY_GWDRCTHDR	*)pData)->_Test11 == PACKET_TEST_DATA15))

int AY_DirectSendToListed(Ui08 *pkt_data, Ui32 len, AY_DEVINFO *pInfom);
int AY_DirectSendToListedRcv(Ui08 *pkt_data, Ui32 len, AY_DEVINFO *pInfom);


/**************************************************************************************************************
									Main Server 1982 Call
***************************************************************************************************************
***************************************************************************************************************
			      **            **       ** 		**	    **	    **
			      ** **      ** **     **  **		**	    ****	**
			      **   **  **   **   **      **		**	    **	**  **
			      **     **     **   ********** 	**	    **	  ****
			      **            **   **********		**	    **	    **
			      **            **   **      **		**	    **	    **
			      **            **   **      **		**	    **	    **
***************************************************************************************************************
***************************************************************************************************************
***************************************************************************************************************/
void AY_MainSocket_CallBack(Ui08 *param, const struct pcap_pkthdr *header, const Ui08 *pkt_data) {
	tcp_headerAll	*pTCP;
	Ui08			*pData;

	printf("Main Socket Callback\n");

	/* retireve the position of the ethernet header */
	pTCP = (tcp_headerAll *)(pkt_data + 0); // tcp all header
	pData = (Ui08 *)(pkt_data + sizeof(tcp_headerAll)); // 
	
	if ( (pTCP->_tcpHeader.sport == _HTONS(CngFile.ServerPort)) && (memcmp(&pTCP->_ipHeader.daddr, &MyIP_Address.byte1, sizeof(ip_address)) == 0) ) {
		printf("Server Port Call\n");/* */
		if (((AY_GWDRCTHDR	*)pData)->_Test10 == PACKET_TEST_DATA11) {///< Direct Send Request
			AY_GWDRCTHDR		*pGwDH;
			Ui16				oLen, iLen;
			AY_DEVINFO			*pInfom;
			AY_CLNTQUEUE		*pQue;
			AY_LOCCONNINFO		LocConn0;
			int					i;
			Ui08				*pPckt;

			printf("AYCLNT--> ============ DIRECT SEND Test & Find Target =========\n ");
			pGwDH = (AY_GWDRCTHDR	*)pData;
			pInfom = pAY_FindDevInfoByDevNo(pGwDH->_DevNo);
			if (pInfom) {///< there is a valid target 
				if ((pInfom->DevRead._Type == _REAL_)) {///< target must be a real device					
					AY_DEVINFO		 *pInfom2;
					AY_GWINFO		*pGw0;
					tcp_headerAll	*pTCP2;
					ip_headerAll	IPA;
					AY_LOCCONNINFO	*pLocConn0 = nullptr;
					//---------------------------//
#if STEP_TEST == 1
					printf("********* STEP D3 *************\n********* STEP D3 *************\n********* STEP D3 *************\n");
					AYPRINT_TCP_Header(pTCP);
#endif	
					iLen = header->len;
					pPckt = (unsigned char*)_AY_MallocMemory(iLen);///< allocate memory for income data
					memcpy(pPckt, pkt_data, iLen);
					pData = (Ui08 *)(pPckt + sizeof(tcp_headerAll)); // 
					//------ DECRPT -------------------//
					oLen = iLen - sizeof(tcp_headerAll) - sizeof(AY_GWDRCTHDR);
					AY_Decrypt_AES128((Ui08 *)&AY_Ram.AY_Sessionkey[0], (Ui08 *)(pData + sizeof(AY_GWDRCTHDR)), oLen);
					pTCP2 = ((tcp_headerAll *)(pData + sizeof(AY_GWDRCTHDR)));
#if STEP_TEST == 1
					printf("********* STEP D3B *************\n********* STEP D3B *************\n********* STEP D3B *************\n");
					printf("AYCLNT--> SSK = "); AY_HexValPrint((Ui08 *)&AY_Ram.AY_Sessionkey[0], 16); printf("\r\n");
					AYPRINT_TCP_Header(pTCP2);
#endif	
					//___________________ LOCAL CONNECTION ____________________________________________//
					AY_LOCCONNINFO		LocConn0, *pLocConn;
					uip_eth_hdr			*pHdr;
					//tcp_headerAll		*pTCP;
					//--------------//
					LocConn0.IPA_Hdr.daddr = *((ip_address *)&pInfom->DevRead._LocalIp);
					LocConn0.IPA_Hdr.saddr = MyIP_Address; //ayk! pTCP2->_ipHeader.saddr;
					if ((oLen > (sizeof(uip_eth_hdr) + sizeof(ip_header))) && ((pTCP2->_ipHeader.proto == UIP_PROTO_UDP) || (pTCP2->_ipHeader.proto == UIP_PROTO_TCP))) {
						LocConn0.IPA_Hdr.dport = pTCP2->_tcpHeader.dport;
						LocConn0.IPA_Hdr.sport = pTCP2->_tcpHeader.sport;
					}
					else {
						LocConn0.IPA_Hdr.dport = 0;
						LocConn0.IPA_Hdr.sport = 0;
					}
					LocConn0.pDevInfo = pInfom;
					memcpy(&LocConn0.src, &pTCP2->_ethHeader.src, sizeof(uip_eth_addr));
					memcpy(&LocConn0.dest, &DefaultMac[0], sizeof(uip_eth_addr));
					pLocConn = pAYCLNT_TestAddOrUpdateLocConn(&LocConn0, 0, &LocConn0.src, _ETH_SRC_);
					memcpy(&pLocConn->src, &pTCP2->_ethHeader.src, sizeof(uip_eth_addr));
					pLocConn->Odaddr = pTCP2->_ipHeader.daddr;
					pLocConn->Osaddr = pTCP2->_ipHeader.saddr;
					pLocConn->ConM2M_Id = pGwDH->_ConM2M_Id;
					//______________________________________________________________________________________//
					pTCP2->_ethHeader.src = MyEth_Address; //ayk! LocConn0.src;
					pTCP2->_ethHeader.dest = pLocConn->dest;
					pTCP2->_ipHeader.saddr = MyIP_Address;
					pTCP2->_ipHeader.daddr = *((ip_address *)&pInfom->DevRead._LocalIp);
#if STEP_TEST == 1
						printf("********* STEP D4 *************\n********* STEP D4 *************\n********* STEP D4 *************\n");
						AYPRINT_TCP_Header(pTCP2);
#endif
					//----------- Re-Calculate CRC's -----------------//
					//----------- Send Packet --------------------------//
					switch (pTCP2->_ipHeader.proto) {
					case UIP_PROTO_UDP:
						pData = (Ui08 *)(((Ui08 *)pTCP2) + sizeof(udp_headerAll)); // 
						oLen = oLen - sizeof(udp_headerAll);
						UDP_packet_send(_MAIN_SCKT, (udp_headerAll*)pTCP2, pData, oLen);
						break;
					case UIP_PROTO_TCP:
						pData = (Ui08 *)(((Ui08 *)pTCP2) + sizeof(tcp_headerAll)); // 
						oLen = oLen - sizeof(tcp_headerAll);
						TCP_packet_send(_MAIN_SCKT, (tcp_headerAll*)pTCP2, pData, oLen);
						break;
					case UIP_PROTO_ICMP:
						pData = (Ui08 *)(((Ui08 *)pTCP2) + sizeof(icmp_headerAll)); // 
						oLen = oLen - sizeof(icmp_headerAll);
						ICMP_packet_send(_MAIN_SCKT, (icmp_headerAll*)pTCP2, pData, oLen);
						break;
					default:
						//...
						break;
					}
					free(pPckt);
				}
			}
		}
		else if (((AY_GWDRCTHDR	*)pData)->_Test10 == PACKET_TEST_DATA13) {///< Direct Response Request
			AY_GWDRCTHDR		*pGwDH;
			Ui16				oLen, iLen;
			AY_DEVINFO			*pInfom;
			AY_CLNTQUEUE		*pQue;
			AY_LOCCONNINFO		LocConn0;
			int					i;
			Ui08				*pPckt;

			printf("AYCLNT--> ============ DIRECT RCV Test & Find Target =========\n ");
			pGwDH = (AY_GWDRCTHDR	*)pData;
			pInfom = pAY_FindDevInfoByDevNo(pGwDH->_DevNo);
			if (pInfom) {///< there is a valid target 
				if ((pInfom->DevRead._Type == _MIRROR_) || (pInfom->DevRead._Type == _GUEST_)) {///< target must be a mirror device
					AY_DEVINFO		 *pInfom2;
					AY_GWINFO		*pGw0;
					tcp_headerAll	*pTCP2;
					ip_headerAll	IPA;
					//AY_LOCCONNINFO	*pLocConn0 = nullptr;
					//---------------------------//
#if STEP_TEST == 1
					printf("********* STEP D7 *************\n********* STEP D7 *************\n********* STEP D7 *************\n");
					AYPRINT_TCP_Header(pTCP);
#endif	
					iLen = header->len;
					pPckt = (unsigned char*)_AY_MallocMemory(iLen);///< allocate memory for income data
					memcpy(pPckt, pkt_data, iLen);
					pData = (Ui08 *)(pPckt + sizeof(tcp_headerAll)); // 
					//------ DECRPT -------------------//
					oLen = iLen - sizeof(tcp_headerAll) - sizeof(AY_GWDRCTHDR);
					AY_Decrypt_AES128((Ui08 *)&AY_Ram.AY_Sessionkey[0], (Ui08 *)(pData + sizeof(AY_GWDRCTHDR)), oLen);
					pTCP2 = ((tcp_headerAll *)(pData + sizeof(AY_GWDRCTHDR)));
#if STEP_TEST == 1
					printf("********* STEP D7B *************\n********* STEP D7B *************\n********* STEP D7B *************\n");
					printf("AYCLNT--> SSK = "); AY_HexValPrint((Ui08 *)&AY_Ram.AY_Sessionkey[0], 16); printf("\r\n");
					AYPRINT_TCP_Header(pTCP2);
#endif	
					////___________________ LOCAL CONNECTION ____________________________________________//
					//AY_LOCCONNINFO		LocConn0, *pLocConn;
					//uip_eth_hdr			*pHdr;
					//tcp_headerAll		*pTCP;
					////--------------//
					//LocConn0.IPA_Hdr.daddr = *((ip_address *)&pInfom->DevRead._LocalIp);
					//LocConn0.IPA_Hdr.saddr = MyIP_Address; //ayk! pTCP2->_ipHeader.saddr;
					//if ((header->len > (sizeof(uip_eth_hdr) + sizeof(ip_header))) && ((pTCP->_ipHeader.proto == UIP_PROTO_UDP) || (pTCP->_ipHeader.proto == UIP_PROTO_TCP))) {
					//	LocConn0.IPA_Hdr.dport = pTCP2->_tcpHeader.dport;
					//	LocConn0.IPA_Hdr.sport = pTCP2->_tcpHeader.sport;
					//}
					//else {
					//	LocConn0.IPA_Hdr.dport = 0;
					//	LocConn0.IPA_Hdr.sport = 0;
					//}
					//LocConn0.pDevInfo = pInfom;
					//memcpy(&LocConn0.src, &pTCP2->_ethHeader.src, sizeof(uip_eth_addr));
					//memcpy(&LocConn0.dest, &DefaultMac[0], sizeof(uip_eth_addr));
					//pLocConn = pAYCLNT_TestAddOrUpdateLocConn(&LocConn0, 0, &LocConn0.src, _ETH_SRC_);
					//memcpy(&pLocConn->src, &pTCP2->_ethHeader.src, sizeof(uip_eth_addr));
					//pLocConn->Odaddr = pTCP2->_ipHeader.daddr;
					//pLocConn->Osaddr = pTCP2->_ipHeader.saddr;
					//pLocConn->ConM2M_Id = pGwDH->_ConM2M_Id;
					////______________________________________________________________________________________//
					//pTCP2->_ethHeader.src = MyEth_Address; //ayk! LocConn0.src;
					//pTCP2->_ethHeader.dest = pLocConn->dest;
					//pTCP2->_ipHeader.saddr = MyIP_Address;
					//pTCP2->_ipHeader.daddr = *((ip_address *)&pInfom->DevRead._LocalIp);
#if STEP_TEST == 1
					printf("********* STEP D8 *************\n********* STEP D8 *************\n********* STEP D8 *************\n");
					AYPRINT_TCP_Header(pTCP2);
#endif
					//----------- Re-Calculate CRC's -----------------//
					//----------- Send Packet --------------------------//
					switch (pTCP2->_ipHeader.proto) {
					case UIP_PROTO_UDP:
						pData = (Ui08 *)(((Ui08 *)pTCP2) + sizeof(udp_headerAll)); // 
						oLen = oLen - sizeof(udp_headerAll);
						UDP_packet_send(_MAIN_SCKT, (udp_headerAll*)pTCP2, pData, oLen);
						break;
					case UIP_PROTO_TCP:
						pData = (Ui08 *)(((Ui08 *)pTCP2) + sizeof(tcp_headerAll)); // 
						oLen = oLen - sizeof(tcp_headerAll);
						TCP_packet_send(_MAIN_SCKT, (tcp_headerAll*)pTCP2, pData, oLen);
						break;
					case UIP_PROTO_ICMP:
						pData = (Ui08 *)(((Ui08 *)pTCP2) + sizeof(icmp_headerAll)); // 
						oLen = oLen - sizeof(icmp_headerAll);
						ICMP_packet_send(_MAIN_SCKT, (icmp_headerAll*)pTCP2, pData, oLen);
						break;
					default:
						//...
						break;
					}
					//-----------//
					free(pPckt);
				}
			}
		}
		else if (NEW_REMOTE_RESPONSE) {
			AY_GWDATAHDR		*pGwDH;
			Ui16				oLen, iLen;
			AY_DEVINFO			*pInfom;
			AY_CLNTQUEUE		*pQue;
			AY_LOCCONNINFO		LocConn0;
			int					i;
			Ui08				*pPckt;
			printf("AYCLNT--> ============ NEW REMOTE RESPONSE Test & Find Target =========\n ");
			pGwDH = (AY_GWDATAHDR	*)pData;
			pInfom = pAY_FindDevInfoByDevNo(pGwDH->_DevNoOnTrgt);
			if (pInfom) {///< there is a valid target 
				if ((pInfom->DevRead._Type == _MIRROR_)|| (pInfom->DevRead._Type == _GUEST_)) {///< target must be a mirror device
					AY_DEVINFO		 *pInfom2;
					AY_GWINFO		*pGw0;
					tcp_headerAll	*pTCP2;
					ip_headerAll	IPA;
					AY_LOCCONNINFO	*pLocConn0 = nullptr;
					//---------------------------//
#if STEP_TEST == 1
					printf("********* STEP 11 *************\n********* STEP 11 *************\n********* STEP 11 *************\n");
					AYPRINT_TCP_Header(pTCP);
#endif	
					iLen = header->len;
					pPckt = (unsigned char*)_AY_MallocMemory(iLen);///< allocate memory for income data
					memcpy(pPckt, pkt_data, iLen);
					pData = (Ui08 *)(pPckt + sizeof(tcp_headerAll)); // 
					//------ DECRPT -------------------//
					oLen = iLen - sizeof(tcp_headerAll) - sizeof(AY_GWDATAHDR);
					AY_Decrypt_AES128((Ui08 *)&AY_Ram.AY_Sessionkey[0], (Ui08 *)(pData + sizeof(AY_GWDATAHDR)), oLen);
					pTCP2 = ((tcp_headerAll *)(pData + sizeof(AY_GWDATAHDR)));
#if STEP_TEST == 1
					printf("********* STEP 11B *************\n********* STEP 11B *************\n********* STEP 11B *************\n");
					printf("AYCLNT--> SSK = "); AY_HexValPrint((Ui08 *)&AY_Ram.AY_Sessionkey[0], 16); printf("\r\n");
					AYPRINT_TCP_Header(pTCP2);
#endif	
					oLen = _HTONS(pTCP2->_ipHeader.tlen) + sizeof(uip_eth_hdr) + sizeof(AY_GWDATAHDR) + sizeof(tcp_headerAll);
					if (oLen <= iLen) {
						iLen = oLen;
					}
					//IPA
					IPA.daddr = *((ip_address *)&pInfom->DevRead._LocalIp);
					IPA.saddr = pTCP2->_ipHeader.daddr;
					if ((oLen > (sizeof(ip_header) + sizeof(uip_eth_hdr) + sizeof(AY_GWDATAHDR) + sizeof(tcp_headerAll)))&&((pTCP2->_ipHeader.proto== UIP_PROTO_UDP)|| (pTCP2->_ipHeader.proto == UIP_PROTO_TCP))) {
						IPA.sport = pTCP2->_tcpHeader.dport;
						IPA.dport = pTCP2->_tcpHeader.sport;
						//i = 1;
					}
					else {
						IPA.sport = 0;
						IPA.dport = 0;
						//i = 0;
					}
					pLocConn0 = pAYCLNT_FindLocConnByIPA/*_Rvs*/(&IPA, &i,0,0);///< find local connection 
					if (pLocConn0 != nullptr) {
						//------------ Change IP Addresses -----------//	
						pTCP2->_ethHeader.src = MyEth_Address;
						pTCP2->_ethHeader.dest = *((uip_eth_addr *)&DefaultMac[0]);
						pTCP2->_ipHeader.saddr = IPA.daddr;
						pTCP2->_ipHeader.daddr = IPA.saddr;
#if STEP_TEST == 1
						printf("********* STEP 12 *************\n********* STEP 12 *************\n********* STEP 12 *************\n");
						AYPRINT_TCP_Header(pTCP2);
#endif	
						//----------- Re-Calculate CRC's -----------------//
						//----------- Send Packet --------------------------//
						switch (pTCP2->_ipHeader.proto) {
						case UIP_PROTO_UDP:
							pData = (Ui08 *)(((Ui08 *)pTCP2) + sizeof(udp_headerAll)); // 
							oLen = oLen - sizeof(udp_headerAll) - sizeof(AY_GWDATAHDR) - sizeof(udp_headerAll);
							UDP_packet_send(_MAIN_SCKT, (udp_headerAll*)pTCP2, pData, oLen);
							break;
						case UIP_PROTO_TCP:
							pData = (Ui08 *)(((Ui08 *)pTCP2) + sizeof(tcp_headerAll)); // 
							oLen = oLen - sizeof(udp_headerAll) - sizeof(AY_GWDATAHDR) - sizeof(tcp_headerAll);
							TCP_packet_send(_MAIN_SCKT, (tcp_headerAll*)pTCP2, pData, oLen);
							break;
						case UIP_PROTO_ICMP:
							pData = (Ui08 *)(((Ui08 *)pTCP2) + sizeof(icmp_headerAll)); // 
							oLen = oLen - sizeof(udp_headerAll) - sizeof(AY_GWDATAHDR) - sizeof(icmp_headerAll);
							ICMP_packet_send(_MAIN_SCKT, (icmp_headerAll*)pTCP2, pData, oLen);
							break;
						default:
							//...
							break;
						}
					}
					//-----------//
					free(pPckt);
				}
			}

		}
		else if (NEW_REMOTE_PACKET) {
			AY_GWDATAHDR		*pGwDH;
			Ui16				oLen,iLen;
			AY_DEVINFO			*pInfom;
			AY_CLNTQUEUE		*pQue;
			AY_LOCCONNINFO		LocConn0;
			int					i;
			Ui08				*pPckt; 

#if STEP_TEST == 1
			printf("********* STEP 8 *************\n********* STEP 8 *************\n********* STEP 8 *************\n");
			AYPRINT_TCP_Header(pTCP);
#endif
			printf("AYCLNT--> ============ NEW REMOTE PACKET Test & Find Target =========\n ");

			pGwDH = (AY_GWDATAHDR	*)pData;
			pInfom = pAY_FindDevInfoByDevNo(pGwDH->_DevNoOnTrgt);
			if (pInfom) {///< there is a valid target 
				if ((pInfom->DevRead._Type == _REAL_)) {///< target must be a real device					
					//--------- Generate Connection -----//
					AY_DEVINFO		Inform2, *pInfom2;
					AY_GWINFO		*pGw0;
					tcp_headerAll	*pTCP2;
					pGw0 = pAYCLNT_FindGwByPortNo(pTCP->_tcpHeader.dport, &i);
					if (pGw0 != nullptr) {
						memset(&Inform2, 0, sizeof(AY_DEVINFO));
						//---------------------------//
						iLen = header->len;
						pPckt = (unsigned char*)_AY_MallocMemory(iLen);///< allocate memory for income data
						memcpy(pPckt, pkt_data, iLen);
						pData = (Ui08 *)(pPckt + sizeof(tcp_headerAll)); // 
						//------ DECRPT -------------------//
						oLen = iLen - sizeof(tcp_headerAll) - sizeof(AY_GWDATAHDR);
						AY_Decrypt_AES128((Ui08 *)&AY_Ram.AY_Sessionkey[0], (Ui08 *)(pPckt + sizeof(tcp_headerAll) + sizeof(AY_GWDATAHDR)), oLen);
						pTCP2 = ((tcp_headerAll *)(pData + sizeof(AY_GWDATAHDR)));
						oLen = _HTONS(pTCP2->_ipHeader.tlen) + sizeof(uip_eth_hdr) + sizeof(AY_GWDATAHDR) + sizeof(tcp_headerAll);
						if (oLen <= iLen) {
							iLen = oLen;
						}
						//---------- Generate Guest Device Info -------------//		
						if ((oLen > (sizeof(ip_header) + sizeof(uip_eth_hdr) + sizeof(AY_GWDATAHDR) + sizeof(tcp_headerAll))) && ((pTCP2->_ipHeader.proto == UIP_PROTO_UDP) || (pTCP2->_ipHeader.proto == UIP_PROTO_TCP))) {
							Inform2.DevRead._dport = pTCP2->_tcpHeader.dport;
							Inform2.DevRead._sport = pTCP2->_tcpHeader.sport;
						}
						else {
							Inform2.DevRead._dport = 0;
							Inform2.DevRead._sport = 0;
						}
						Inform2.DevRead._LocalIp = *((Ui32 *)&pTCP2->_ipHeader.saddr.byte1);
						Inform2.DevRead._ParentId = ((AY_GWDATAHDR	*)pData)->_DevNoOnTrgt;
						Inform2.DevRead._Type = _GUEST_;
						Inform2.DevF.Full_ = 1;
						memcpy(&Inform2.DevRead._Unique[0], &pGw0->_Unique[0], 12);
						//------------------------------------------------------------//
						pInfom2 = pAY_FindRmtDevInfoByAll(&Inform2);
						if (pInfom2 == nullptr) {
							pInfom2 = pAYCLNT_AddDevToList((Ui08 *)&pGw0->_Unique[0], 0x01000000 | (4096), _DEV_UNIQUE_ALL);///< burasý hatalý !!! sürekli yeni cihaz ekliyor.duzeltildi.
							memcpy(pInfom2, &Inform2, sizeof(AY_DEVINFO));
						}
						//------------ Change IP Addresses -----------//	
						pTCP2->_ethHeader.src = MyEth_Address;
						pTCP2->_ethHeader.dest = *((uip_eth_addr *)&DefaultMac[0]);
						pTCP2->_ipHeader.saddr = MyIP_Address;
						pTCP2->_ipHeader.daddr = *((ip_address *)&pInfom->DevRead._LocalIp);
						//------------ Generate Sub-Connection --------------------//
						LocConn0.pDevInfo = pInfom2;
						LocConn0.IPA_Hdr.daddr = pTCP2->_ipHeader.daddr;
						LocConn0.IPA_Hdr.saddr = pTCP2->_ipHeader.saddr;
						if ((oLen > (sizeof(ip_header) + sizeof(uip_eth_hdr) + sizeof(AY_GWDATAHDR) + sizeof(tcp_headerAll))) && ((pTCP2->_ipHeader.proto == UIP_PROTO_UDP) || (pTCP2->_ipHeader.proto == UIP_PROTO_TCP))) {
							LocConn0.IPA_Hdr.dport = pTCP2->_tcpHeader.dport;
							LocConn0.IPA_Hdr.sport = pTCP2->_tcpHeader.sport;
						}
						else {
							LocConn0.IPA_Hdr.dport = 0;
							LocConn0.IPA_Hdr.sport = 0;
						}
						pAYCLNT_TestAddOrUpdateLocConn(&LocConn0, 0,0,0);
#if STEP_TEST == 1
						printf("********* STEP 9 *************\n********* STEP 9 *************\n********* STEP 9 *************\n");
						AYPRINT_TCP_Header(pTCP2);
#endif
						//----------- Re-Calculate CRC's -----------------//
						//----------- Send Packet --------------------------//
						switch (pTCP2->_ipHeader.proto) {
						case UIP_PROTO_UDP:
							pData = (Ui08 *)(((Ui08 *)pTCP2) + sizeof(udp_headerAll)); // 
							oLen = oLen - sizeof(udp_headerAll) - sizeof(AY_GWDATAHDR) - sizeof(udp_headerAll);
							UDP_packet_send(_MAIN_SCKT, (udp_headerAll*)pTCP2, pData, oLen);
							break;
						case UIP_PROTO_TCP:
							pData = (Ui08 *)(((Ui08 *)pTCP2) + sizeof(tcp_headerAll)); // 
							oLen = oLen - sizeof(udp_headerAll) - sizeof(AY_GWDATAHDR) - sizeof(tcp_headerAll);
							TCP_packet_send(_MAIN_SCKT, (tcp_headerAll*)pTCP2, pData, oLen);
							break;
						case UIP_PROTO_ICMP:
							pData = (Ui08 *)(((Ui08 *)pTCP2) + sizeof(icmp_headerAll)); // 
							oLen = oLen - sizeof(udp_headerAll) - sizeof(AY_GWDATAHDR) - sizeof(icmp_headerAll);
							ICMP_packet_send(_MAIN_SCKT, (icmp_headerAll*)pTCP2, pData, oLen);
							break;
						default:
							//...
						break;
						}
						free(pPckt);
					}
				}
			}
		}
		else if (GW_PORT_RENT) {
			AY_GWRENTRQST *pGwRent = (AY_GWRENTRQST	*)pData;
			AY_GWINFO	*pGw0;
#if STEP_TEST==1
			printf("********* STEP 4 *************\n********* STEP 4 *************\n********* STEP 4 *************\n");
			AYPRINT_TCP_Header(pTCP);
#endif
			AY_Decrypt_AES128((Ui08 *)&AY_Ram.AY_Sessionkey[0], (Ui08 *)(&pGwRent->_InfoCont[0]), AY_GWRENTRQSTT_SIZE_OF_INFO_CONT);

			pGw0 = pAYCLNT_AddGwToList((Ui08 *)&pGwRent->_Unique[0], (Ui32*)&pGwRent->_Unique[0], _GW_UNQUE_ALL);///< add or update gw to list
			AYCLNT_UpdateGwInfo(pGw0, (Ui08 *)&pGwRent->_TCPh, _GW_TCPH);
			AYCLNT_UpdateGwInfo(pGw0, (Ui08 *)&pGwRent->_SessionKey[0], _GW_SSK);
			pGw0->MyPortNo = pTCP->_tcpHeader.dport;
			//----------------//
			MyClientInstPort++; if (MyClientInstPort < 81) { MyClientInstPort = 81; }
			AY_SendDeviceStartToServer(_USE_OLD);
		}
		else if (GW_INFO_REQUEST) {
			AY_GWINFORESP *pGwRsp = (AY_GWINFORESP	*)pData;
			AY_CLNTQUEUE *pQue = AYCLNT_QueueReadSlot(pGwRsp->_QueRowNo);
			AY_GWINFO	*pGw0;
			if (pQue != nullptr) {
#if STEP_TEST==1
				printf("********* STEP 6 *************\n********* STEP 6 *************\n********* STEP 6 *************\n");
				AYPRINT_TCP_Header(pTCP);
#endif
				AY_Decrypt_AES128((Ui08 *)&AY_Ram.AY_Sessionkey[0], (Ui08 *)(&pGwRsp->_InfoCont[0]), AY_GWINFORESP_SIZE_OF_INFO_CONT);

				pGw0 = pAYCLNT_AddGwToList((Ui08 *)&pQue->pInfo->DevRead._Unique[0], &pQue->pInfo->DevRead._Unique[0], _GW_UNQUE_ALL);///< add or update gw to list
				AYCLNT_UpdateGwInfo(pGw0, (Ui08 *)&pGwRsp->_TCPh, _GW_TCPH);
				AYCLNT_UpdateGwInfo(pGw0, (Ui08 *)&pGwRsp->_SessionKey[0], _GW_SSK);
				pGw0->MyPortNo = pTCP->_tcpHeader.dport;
#if STEP_TEST==1
				printf("********* STEP 6 *************\n********* STEP 6 *************\n********* STEP 6 *************\n");
				AYPRINT_TCP_Header(&pGwRsp->_TCPh);
#endif
				//----------------//
				pQue->pGw = pGw0;
				AY_Client_ChngServerConn = 1;
				pQue->Status = _CHNG_SERVER_CONN;
				//----------------//
				MyClientInstPort++; if (MyClientInstPort < 81) { MyClientInstPort = 81; }
				AY_SendDeviceStartToServer(_USE_OLD);
			}
		}
		else {
		AY_DeviceStartResp *pDevStrt = (AY_DeviceStartResp	*)pData;
			if (GW_DEVICE_LIST) {
				Ui32 i, j, k;
				Ui32 m, n;
				Ui08 Temp[45];
				AY_DeviceStartResp	*pRsp = (AY_DeviceStartResp *)(pkt_data + sizeof(tcp_headerAll)); // 
				AY_DeviceRead		*pDev = (AY_DeviceRead *)(pkt_data + sizeof(tcp_headerAll) + sizeof(AY_DeviceStartResp)); // 
#if REMOTE_TEST==1
				if (memcmp(&SrvEth_Address.addr[0], &DefaultMac[0], sizeof(uip_eth_addr)) == 0) {
					memcpy(&SrvEth_Address.addr[0], &pTCP->_ethHeader.src, 6);
				}
#endif
				if (AY_Client_SrvMAC_NotFound) {
					SrvEth_Address = pTCP->_ethHeader.src;
					AY_Client_SrvMAC_NotFound = 0;
				}
#if STEP_TEST==1
				if (AY_Client_ChngServerConn) {
					printf("********* STEP 7 *************\n********* STEP 7 *************\n********* STEP 7 *************\n");
				}
				else {
					printf("********* STEP 0 *************\n********* STEP 5 *************\n********* STEP 7 *************\n");
				}
				AYPRINT_TCP_Header(pTCP);
#endif
				if (AY_Ram.AY_DevPcktNo == pRsp->_DevPcktNo) {
					j = pRsp->_DevCnt;
					k = AY_Ram.AY_DeviceCnt;
					if (j) {
						AY_Decrypt_AES128((Ui08 *)&AY_Ram.AY_Sessionkey[0], (Ui08 *)pDev, header->len - (sizeof(tcp_headerAll) + sizeof(AY_DeviceStartResp))/*(j * sizeof(AY_DeviceRead))*/);
						memcpy(&AY_Ram.AY_DeviceList[0], pDev, (j * sizeof(AY_DeviceRead)));
					}

					//============== FILE========================//
					printf("AYDVSTRT--> SSK = "); AY_HexValPrint((Ui08 *)&AY_Ram.AY_Sessionkey[0], 16); printf("\r\n");
					printf("AYDVSTRT--> %d ============ FILE START =========\n ", (header->len - sizeof(tcp_headerAll)));
					m = sizeof(AY_DeviceStartResp) + (((Ui16)j) * sizeof(AY_DeviceRead));
					n = 0;
					while (m > 32) {
						AY_HexValPrint((((Ui08 *)pRsp) + n), 32);
						printf("\r\n ");
						n += 32;
						m -= 32;
					}
					if (m) {
						AY_HexValPrint((((Ui08 *)pRsp) + n), m);
						printf("\r\n ");
					}
					printf("AYDVSTRT--> ============ FILE END =========\n ");
					//===========================================//
					printf("\nDevice List Downloaded Packet Length = %d, DevCnt=%d\n", header->len, j);
					for (i = 0; i < j; i++) {
						printf("Device No:%d ID:%d Unq0:0x%08x Unq1:0x%08x  Unq2:0x%08x  Parent:%d Type:%d LocalIP:%s\n", i, AY_Ram.AY_DeviceList[i]._id, AY_Ram.AY_DeviceList[i]._Unique[0], AY_Ram.AY_DeviceList[i]._Unique[1], AY_Ram.AY_DeviceList[i]._Unique[2], AY_Ram.AY_DeviceList[i]._ParentId, AY_Ram.AY_DeviceList[i]._Type, AY_ConvertIPToStrRet((Ui08 *)&AY_Ram.AY_DeviceList[i]._LocalIp, (char*)&Temp[0]));
						pAYCLNT_AddDevToList((Ui08 *)&AY_Ram.AY_DeviceList[i], (k + i), _DEV_READ_ALL);
						if ((!AY_Client_RecvServer) && (AY_Ram.AY_DeviceList[i]._Type == _MIRROR_)) {
							printf("Remote Device found Device No:%d ID:%d Type:%d LocalIP:%s\n", i, AY_Ram.AY_DeviceList[i]._id, AY_Ram.AY_DeviceList[i]._Type, AY_ConvertIPToStrRet((Ui08 *)&AY_Ram.AY_DeviceList[i]._LocalIp, (char*)&Temp[0]));
							AYFILE_AddIPsToFile((char*)&AddIP_File[0], CngFile.NetInterfaceName, &AY_Ram.AY_DeviceList[i]._LocalIp, 1, *((Ui32*)&CngFile.NetworkSubnetMask[0]), *((Ui32*)&CngFile.NetworkGatewayIp[0]), 1);
						}
					}
					AY_Ram.AY_DeviceCnt += j;
					AY_Ram.AY_DevPcktNo++;
					if (j < 168) {
						AY_Client_RemoteDevListEnd = 1;
						AY_Client_RecvServer = 1;
						AY_Client_ChngServerConn = 0;
					}
				}
			}
		}
	}	
}

/**************************************************************************************************************
***************************************************************************************************************
***************************************************************************************************************
			**                **                ******         **			**
			**              **   **           **              **   **		**
			**            **       **       **             **        **		**
			**            **       **       **             ************ 	**       
			**            **       **       **             ************		**
			********        **   **           **           **        **		********
			********          **                ******     **        **		********
***************************************************************************************************************
***************************************************************************************************************
***************************************************************************************************************/
void AY_SocketRead_CallBack(Ui08 *param, const struct pcap_pkthdr *header, const Ui08 *pkt_data) {
	uip_eth_hdr		*pHdr;
	ip_header		*ih;
	tcp_headerAll	*pTCP;
	struct DNS_HEADER *dns = NULL;

	//printf("Read Socket Callback\n");

	/* retireve the position of the ethernet header */
	pHdr = (uip_eth_hdr *)(pkt_data + 0); // ethernet header
	ih = (ip_header *)(pkt_data + 14); //length of ethernet header
	pTCP = (tcp_headerAll *)(pkt_data + 0); // tcp all header


	if ( pHdr->type == _HTONS(UIP_ETHTYPE_ARP) ) {
		if (!AY_Client_GetMACadr) {
			arp_headerAll		*pARP_Pckt = (arp_headerAll *)(pkt_data + 0); // ethernet header
			if (pARP_Pckt->request == _HTONS(2)) {///< reply
				if (memcmp(&pARP_Pckt->SenderIp.byte1, &MyIP_Addresses._gateway.byte1, sizeof(ip_address)) == 0) {
					printf("MAC Packet Received !!!\n\n\n===============================================\n\n\n===============================================\n\n\n");
					SrvEth_Address = pARP_Pckt->SenderMac;
					MyEth_Address = pARP_Pckt->TargetMac;
					AY_Client_GetMACadr = 1;
				}
			}
		}
	}
	else {
		if (!AY_Client_GetMACadr) {///< MAC Address Waiting ...

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
		else if ((!AY_Client_GetSrvIPadr)) {///< Server IP Waiting ...
			if ((pTCP->_tcpHeader.sport == _HTONS(CngFile.DNSPort))) {
				if ((pTCP->_tcpHeader.dport == _HTONS((MyClientInstPort + 1)))) {
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
		else if ((AY_Client_RecvServer)) {///< Ready ...
			AY_DEVINFO			*pInfom;
			AY_CLNTQUEUE		*pQue;
			AY_LOCCONNINFO		LocConn0;
			int					i;

			pInfom = pAY_FindLocDevInfoByIP(*((Ui32 *)&pTCP->_ipHeader.daddr));
			if (pInfom) {///< there is a valid target 
				printf("AYCLNT--> ============ NEW SIDE PACKET Test & Find Target =========\n ");
				if (pInfom->DevRead._Type == _MIRROR_) {///< target must be a mirror device
#if STEP_TEST==1
					printf("********* STEP 1 *************\n********* STEP 1 *************\n********* STEP 1 *************\n");
					AYPRINT_TCP_Header(pTCP);
#endif
					printf("AYCLNT--> PACKET (1)				DEMO1	--->	GHS1		(A request packet for Mirror(Reflection) Device)\n ");
#if (DIRECT_SEND==1)
					AY_DirectSendToListed((Ui08 *)pkt_data, header->len, pInfom);
#else
					pQue = pAYCLNT_FindFirstFreeQueueId(&i);///< find an empty queue row for outgoing packet
					if (pQue != nullptr) {
						pQue->pInfo = pInfom;
						i = sizeof(AY_GWDATAHDR) + ((header->len + 15) & 0xFFF0);
						pQue->DataIOLen = header->len;
						pQue->pDataIO = (unsigned char*)_AY_MallocMemory(i);///< allocate memory for outgoing data
						memcpy(pQue->pDataIO + sizeof(AY_GWDATAHDR), pkt_data, pQue->DataIOLen);
						pQue->Status = _SEND_TO_SRV;///< Vers 1.1 20191030 _FIND_GW;
						pQue->QueF.Full_ = 1;///< start core process
						//--------------//
						LocConn0.IPA_Hdr.daddr = pTCP->_ipHeader.daddr;
						LocConn0.IPA_Hdr.saddr = pTCP->_ipHeader.saddr;
						if ((header->len > (sizeof(uip_eth_hdr) + sizeof(ip_header))) && ((pTCP->_ipHeader.proto == UIP_PROTO_UDP) || (pTCP->_ipHeader.proto == UIP_PROTO_TCP))) {
							LocConn0.IPA_Hdr.dport = pTCP->_tcpHeader.dport;
							LocConn0.IPA_Hdr.sport = pTCP->_tcpHeader.sport;
						}
						else {
							LocConn0.IPA_Hdr.dport = 0;
							LocConn0.IPA_Hdr.sport = 0;
						}
						LocConn0.pDevInfo = pQue->pInfo;
						pQue->pLocConn = pAYCLNT_TestAddOrUpdateLocConn(&LocConn0, 0,0,0);
					}
#endif
				}
			}
			else {///< Check For Local Device
				pInfom = pAY_FindLocDevInfoByIP(*((Ui32 *)&pTCP->_ipHeader.saddr));
				if (pInfom) {///< there is a valid target 
					printf("AYCLNT--> ============ NEW SIDE PACKET Test & Find Target =========\n ");
					if (pInfom->DevRead._Type == _REAL_) {///< target must be a real device
#if (DIRECT_SEND==1)
#if STEP_TEST == 1
						printf("********* STEP 5 *************\n********* STEP 5 *************\n********* STEP 5 *************\n");
						AYPRINT_TCP_Header(pTCP);
#endif	
						AY_DirectSendToListedRcv((Ui08 *)pkt_data, header->len, pInfom);
#else
#if STEP_TEST == 1
						printf("********* STEP 10 *************\n********* STEP 10 *************\n********* STEP 10 *************\n");
						AYPRINT_TCP_Header(pTCP);
#endif	
						pQue = pAYCLNT_FindFirstFreeQueueId(&i);///< find an empty queue row for outgoing packet
						if (pQue != nullptr) {
							ip_headerAll	IPA;
							AY_LOCCONNINFO	*pLocConn0 = nullptr;
							IPA.daddr = pTCP->_ipHeader.saddr;
							IPA.saddr = pTCP->_ipHeader.daddr;
							if ((header->len > (sizeof(uip_eth_hdr) + sizeof(ip_header))) && ((pTCP->_ipHeader.proto == UIP_PROTO_UDP) || (pTCP->_ipHeader.proto == UIP_PROTO_TCP))) {
								IPA.dport = pTCP->_tcpHeader.sport;
								IPA.sport = pTCP->_tcpHeader.dport;
							}
							else {
								IPA.dport = 0;
								IPA.sport = 0;
							}
							pLocConn0 = pAYCLNT_FindLocConnByIPA/*_Rvs*/(&IPA, &i,0,0);///< find local connection 
							if (pLocConn0 != nullptr) {
								pQue->pInfo = pLocConn0->pDevInfo;//< start core process
								//--------------//							
								i = sizeof(AY_GWDATAHDR) + ((header->len + 15) & 0xFFF0);
								pQue->DataIOLen = header->len;
								pQue->pDataIO = (unsigned char*)_AY_MallocMemory(i);///< allocate memory for outgoing data
								memcpy(pQue->pDataIO + sizeof(AY_GWDATAHDR), pkt_data, pQue->DataIOLen);
								pQue->Status = _FIND_GW2;
								pQue->QueF.Full_ = 1;///< start core process
								//--------------//
								pQue->pLocConn = pLocConn0;
							}
						}
#endif
					}
				}
			}
		}
	}
}

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
	AY_ConvertUi32AddToStrRet((MyClientInstPort + 1), (char *)&MySocketBuff[0] );
	AYSCKT_FilterSetA(_SLVS_SCKT, (char *)&MySocketBuff[0]);
	
	//============= SEND PACKET ==========================//
	UDP_header_init(&UDPheader);
	UDP_header_load(&UDPheader, SrvEth_Address/**((uip_eth_addr *)&DEMO_Mac[0])*/, *((ip_address *)&CngFile.DNSIp[0]), CngFile.DNSPort, MyEth_Address, MyIP_Address, (MyClientInstPort+1));

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

int AY_SendDeviceStartToServer(Ui08 Filter) {
	tcp_headerAll		TCPheader;
	AY_DeviceStart		DevStrt;
	Ui08				OutData[256];
	Ui16				oLen;
	int					RetVal;

	//============= SET FILTER ==========================//
	if (Filter == _GNRT_NEW) {
		AYSCKT_FilterFreeA(_MAIN_SCKT);
		strcpy((char *)&MySocketBuff[0], "tcp src port ");
		AY_ConvertUi32AddToStrRet(CngFile.ServerPort, (char *)&MySocketBuff[0]);
		AYSCKT_FilterSetA(_MAIN_SCKT, (char *)&MySocketBuff[0]);
	}

	//------- LOAD
	memset(&DevStrt,0,sizeof(DevStrt));
	DevStrt._Test0 = PACKET_TEST_DATA0;
	DevStrt._Test1 = PACKET_TEST_DATA1;
	DevStrt._LocalCertNo = CLIENT_CERT_NO;
	DevStrt._ServerCertNo = CngFile.ServerID;
	memcpy(&DevStrt._Name[0], &CngFile.GatewayName, sizeof(CngFile.GatewayName));
	memcpy(&DevStrt._Pswd[0], &CngFile.GatewayPass, sizeof(CngFile.GatewayPass));
	memcpy(&DevStrt._MAC, &MyEth_Address, sizeof(MyEth_Address));
	memcpy(&DevStrt._Unique, &CngFile.UniqueID, sizeof(CngFile.UniqueID));
	//------ Generate AES Key
	if (Filter == _GNRT_NEW) {
		AY_Generate_AES128(&DevStrt._SessionKey[0]);
		memcpy(&AY_Ram.AY_Sessionkey[0], &DevStrt._SessionKey[0], 16);
	}
	else {
		memcpy(&DevStrt._SessionKey[0], &AY_Ram.AY_Sessionkey[0], 16);
	}
	//------ ENCRPT
	AY_Crypt_RSAEncrpt((Ui08 *)&/*SERVER_PUB_KEY*/CngFile.ServerPublicKey[0], (Ui08 *)&DevStrt._Input[0], 240, (Ui08 *)&OutData[0], &oLen);
	memcpy(&DevStrt._Input[0], &OutData[0], oLen);
	//------- SIGN
	AY_Crypt_RSASign((Ui08 *)&SIGNING_PR_KEY[0], (Ui08 *)&DevStrt._Input[0], 256, (Ui08 *)&DevStrt._Sign[0]);
	//------- SEND
#if (DIRECT_SEND==1)
	if (memcmp(&AY_TCPheader._ethHeader.src, &MyEth_Address, sizeof(uip_eth_addr)) == 0) {
		TCP_header_init(&AY_TCPheader);
		TCP_header_load(&AY_TCPheader, SrvEth_Address, SrvIP_Address, CngFile.ServerPort, MyEth_Address, MyIP_Address, MyClientInstPort, 1, 1, (_PSH | _ACK));
	}
	else {
		TCP_header_init(&AY_TCPheader);
		TCP_header_load(&AY_TCPheader, SrvEth_Address, SrvIP_Address, CngFile.ServerPort, MyEth_Address, MyIP_Address, MyClientInstPort, 1, 1, (_PSH | _ACK));
	}
	oLen = sizeof(AY_DeviceStart);
	RetVal = TCP_packet_send(_MAIN_SCKT, &AY_TCPheader, (Ui08 *)&DevStrt, oLen/*sizeof(AY_DeviceStart)*/);
#if STEP_TEST==1
	printf("********* STEP 0 *************\n********* STEP 5 *************\n********* STEP 7 *************\n");
	AYPRINT_TCP_Header(&AY_TCPheader);
#endif
#else
	TCP_header_init(&TCPheader);
	TCP_header_load(&TCPheader, SrvEth_Address, SrvIP_Address, CngFile.ServerPort, MyEth_Address, MyIP_Address, MyClientInstPort, 1, 1, (_PSH | _ACK));
	oLen = sizeof(AY_DeviceStart);
	RetVal = TCP_packet_send(_MAIN_SCKT, &TCPheader, (Ui08 *)&DevStrt, oLen/*sizeof(AY_DeviceStart)*/);
#if STEP_TEST==1
	printf("********* STEP 0 *************\n********* STEP 5 *************\n********* STEP 7 *************\n");
	AYPRINT_TCP_Header(&TCPheader);
#endif
#endif
	return RetVal;
}

int AY_StartSlaveListenMAC(uip_eth_addr MAC, Ui08 SrcDst) {
	//============= SET FILTER ==========================//
	AYSCKT_FilterFreeA(_SLVS_SCKT);	
	strcpy((char *)&MySocketBuff[0], "arp or icmp");
	AYSCKT_FilterSetA(_SLVS_SCKT, (char *)&MySocketBuff[0]);
	printf("_SLVS_SCKT filter: %s \r\n", (char *)&MySocketBuff[0]);
	return 1;
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

	strcpy((char *)&MySocketBuff[0], "ip dst host ");
	AY_ConvertIPAddToStrRet((Ui08*)&MyIP_Address.byte1, (char*)&MySocketBuff[0]);
	for (i = 0; i < AY_Ram.AY_DeviceCnt; i++) {
		pDevIfo = pAY_FindDevInfoByDevNo(i);
		if (pDevIfo) {
			if (pDevIfo->DevRead._Type == _MIRROR_) {///< remote
				strcat((char *)&MySocketBuff[0], " or ip dst host ");
				AY_ConvertIPAddToStrRet((Ui08*)&pDevIfo->DevRead._LocalIp, (char*)&MySocketBuff[0]);
				j++;
			}
		}
	}	
	if (j > 49) {
		AY_StartSlaveListenA();
	}
#endif
	AYSCKT_FilterSetA(_SLVS_SCKT, (char *)&MySocketBuff[0]);
	
	printf("_SLVS_SCKT filter: %s \r\n", (char *)&MySocketBuff[0]);
	return 1;
}

int AY_SendGwInfoRequest(AY_CLNTQUEUE *pQue, Si32 row) {
	AY_GWINFORQST		GwRqst;
	tcp_headerAll		TCPheader;
	Ui16				oLen;

	printf("AYCLNT--> PACKET (A)				GW1	--->	SRV				(GW info request)\n ");

	GwRqst._Test2 = PACKET_TEST_DATA2;
	GwRqst._Test3 = PACKET_TEST_DATA3;
	GwRqst._QueRowNo = row;
	//---------------------------//
	memcpy(&GwRqst._Unique[0], &pQue->pInfo->DevRead._Unique[0], sizeof(GwRqst._Unique));
	GwRqst.SendCnt = AY_SendCnt;
	GwRqst.ReadCnt = AY_ReadCnt;
	GwRqst.ErrCnt = AY_ErrCnt;
	//---------------------------//
	AY_Crypt_AES128((Ui08 *)&AY_Ram.AY_Sessionkey[0], (((Ui08 *)&GwRqst._InfoCont[0])), AY_GWINFORQST_SIZE_OF_INFO_CONT);
	//------- SEND
	TCP_header_init(&TCPheader);
	TCP_header_load(&TCPheader, SrvEth_Address, SrvIP_Address, CngFile.ServerPort, MyEth_Address, MyIP_Address, MyClientInstPort,1,1, (_PSH | _ACK));
	oLen = sizeof(AY_GWINFORQST);
#if STEP_TEST==1
	printf("********* STEP 3 *************\n********* STEP 3 *************\n********* STEP 3 *************\n");
	AYPRINT_TCP_Header(&TCPheader);
#endif
	return (TCP_packet_send(_MAIN_SCKT, &TCPheader, (Ui08 *)&GwRqst, oLen));
}

int AY_SendGwInfoSend(AY_CLNTQUEUE *pQue, Si32 row) {
	AY_GWDATAHDR		*pGwDH;
	AY_GWINFORQST		GwRqst;
	tcp_headerAll		TCPheader;
	Ui16				oLen;

	AY_ChngPacketDest_TCP(&pQue->pGw->TCP_Hdr, &SrvEth_Address, _ETH_SRC_);///< Ser Eth or My Eth which is true ???
	pGwDH = (AY_GWDATAHDR *)pQue->pDataIO;
	pGwDH->_Test6 = PACKET_TEST_DATA6;
	pGwDH->_Test7 = PACKET_TEST_DATA7;
	pGwDH->_DevNoOnTrgt = pQue->pInfo->DevRead._ParentId;///< row number on remote gateway for only remote type device
	//---------------------------//
	oLen = ((pQue->DataIOLen + 15) & 0xFFF0);
	AY_Crypt_AES128((Ui08 *)&pQue->pGw->Sessionkey[0], (Ui08 *)(pQue->pDataIO + sizeof(AY_GWDATAHDR)), oLen);
	//------- SEND
#if STEP_TEST==1
	printf("********* STEP 8 *************\n********* STEP 8 *************\n********* STEP 8 *************\n");
	AYPRINT_TCP_Header(&pQue->pGw->TCP_Hdr);
#endif
	return (TCP_packet_send(_MAIN_SCKT, &pQue->pGw->TCP_Hdr, (Ui08 *)pQue->pDataIO, oLen+ sizeof(AY_GWDATAHDR)));
}
int AY_SendGwInfoSend2(AY_CLNTQUEUE *pQue, Si32 row) {
	AY_GWDATAHDR		*pGwDH;
	AY_GWINFORQST		GwRqst;
	tcp_headerAll		TCPheader;
	Ui16				oLen;

	AY_ChngPacketDest_TCP(&pQue->pGw->TCP_Hdr, &SrvEth_Address, _ETH_SRC_);///< Ser Eth or My Eth which is true ???
	pGwDH = (AY_GWDATAHDR *)pQue->pDataIO;
	pGwDH->_Test6 = PACKET_TEST_DATA8;
	pGwDH->_Test7 = PACKET_TEST_DATA9;
	pGwDH->_DevNoOnTrgt = pQue->pInfo->DevRead._ParentId;///< row number on remote gateway for only remote type device
	
	//------------ Change IP Addresses -----------//	
	tcp_headerAll	*pTCP2;
	pTCP2 = (tcp_headerAll *)(pQue->pDataIO + sizeof(AY_GWDATAHDR));
	pTCP2->_ipHeader.daddr = *((ip_address *)&pQue->pInfo->DevRead._LocalIp);
#if STEP_TEST == 1
	printf("********* STEP 11B *************\n********* STEP 11B *************\n********* STEP 11B *************\n");
	printf("AYCLNT--> SSK = "); AY_HexValPrint((Ui08 *)&pQue->pGw->Sessionkey[0], 16); printf("\r\n");
	AYPRINT_TCP_Header(pTCP2);
#endif	
	//-----------------------------------//	

	oLen = ((pQue->DataIOLen + 15) & 0xFFF0);
	AY_Crypt_AES128((Ui08 *)&pQue->pGw->Sessionkey[0], (Ui08 *)(pQue->pDataIO + sizeof(AY_GWDATAHDR)), oLen);
	//------- SEND
#if STEP_TEST == 1
	printf("********* STEP 11 *************\n********* STEP 11 *************\n********* STEP 11 *************\n");
	AYPRINT_TCP_Header(&pQue->pGw->TCP_Hdr);
#endif	
	return (TCP_packet_send(_MAIN_SCKT, &pQue->pGw->TCP_Hdr, (Ui08 *)pQue->pDataIO, oLen + sizeof(AY_GWDATAHDR)));
}

int AY_DirectSendToListed(Ui08 *pkt_data, Ui32 len, AY_DEVINFO *pInfom) {
	unsigned char		*pPckt;
	AY_GWDRCTHDR		*pGwDH;
	Ui32				LenSend;
	Ui08				*pCont;
	//tcp_headerAll		TCPheader;

	//___________________ LOCAL CONNECTION ____________________________________________//
	AY_LOCCONNINFO		LocConn0, *pLocConn;
	uip_eth_hdr			*pHdr;
	tcp_headerAll		*pTCP;

	/* retireve the position of the ethernet header */
	pHdr = (uip_eth_hdr *)(pkt_data + 0); // ethernet header
	pTCP = (tcp_headerAll *)(pkt_data + 0); // tcp all header

	//--------------//
	LocConn0.IPA_Hdr.daddr = pTCP->_ipHeader.daddr;
	LocConn0.IPA_Hdr.saddr = pTCP->_ipHeader.saddr;
	if ((len > (sizeof(uip_eth_hdr) + sizeof(ip_header))) && ((pTCP->_ipHeader.proto == UIP_PROTO_UDP) || (pTCP->_ipHeader.proto == UIP_PROTO_TCP))) {
		LocConn0.IPA_Hdr.dport = pTCP->_tcpHeader.dport;
		LocConn0.IPA_Hdr.sport = pTCP->_tcpHeader.sport;
	}
	else {
		LocConn0.IPA_Hdr.dport = 0;
		LocConn0.IPA_Hdr.sport = 0;
	}
	LocConn0.pDevInfo = pInfom;
	memcpy(&LocConn0.src, &pTCP->_ethHeader.src, sizeof(uip_eth_addr));
	memcpy(&LocConn0.dest, &DefaultMac[0] /*ayk! &MyEth_Address*/, sizeof(uip_eth_addr));
	LocConn0.ConM2M_Id = 0xFFFFFFFF;
	pLocConn = pAYCLNT_TestAddOrUpdateLocConn(&LocConn0, 0, &LocConn0.src, _ETH_SRC_);
	pLocConn->Odaddr = pTCP->_ipHeader.daddr;
	pLocConn->Osaddr = pTCP->_ipHeader.saddr;
	//______________________________________________________________________________________//

	LenSend					= ((len + 15) & 0xFFF0) + sizeof(AY_GWDRCTHDR);
	pPckt					= (unsigned char*)_AY_MallocMemory(LenSend);///< allocate memory for income data
	pGwDH					= (AY_GWDRCTHDR *)((Ui08 *)pPckt);
	pGwDH->_Test10			= PACKET_TEST_DATA10;
	pGwDH->_Test11			= pLocConn->ConM2M_Id;
	pGwDH->_DevNo			= pInfom->DevRead._id;
	//pGwDH->_LocalIP			= *((Ui32 *)&((tcp_headerAll *)(pkt_data + 0))->_ipHeader.saddr);
	pCont					= pPckt + sizeof(AY_GWDRCTHDR);
	   	 
	memcpy(pCont, pkt_data, len);
	//---------------------------//
	AY_Crypt_AES128((Ui08 *)&AY_Ram.AY_Sessionkey[0], pCont, ((len + 15) & 0xFFF0));
	//------- SEND
	TCP_header_init(&AY_TCPheader);
	TCP_header_load(&AY_TCPheader, SrvEth_Address, SrvIP_Address, CngFile.ServerPort, MyEth_Address, MyIP_Address, MyClientInstPort, 1, 1, (_PSH | _ACK));
#if STEP_TEST==1
	printf("********* STEP D2 *************\n********* STEP D2 *************\n********* STEP D2 *************\n");
	printf("AYCLNT--> PACKET (1)				GW1	--->	SRV		(A request packet for Mirror(Reflection) Device)\n ");
	AYPRINT_TCP_Header(&AY_TCPheader);
#endif
	return (TCP_packet_send(_MAIN_SCKT, &AY_TCPheader, (Ui08 *)pPckt, LenSend));
}

int AY_DirectSendToListedRcv(Ui08 *pkt_data, Ui32 len, AY_DEVINFO *pInfom) {
	unsigned char		*pPckt;
	AY_GWDRCTHDR		*pGwDH;
	Ui32				LenSend;
	Ui08				*pCont;
	//tcp_headerAll		TCPheader;

	//___________________ LOCAL CONNECTION ____________________________________________//
	AY_LOCCONNINFO		LocConn0, *pLocConn;
	uip_eth_hdr			*pHdr;
	tcp_headerAll		*pTCP;

	/* retireve the position of the ethernet header */
	pHdr = (uip_eth_hdr *)(pkt_data + 0); // ethernet header
	pTCP = (tcp_headerAll *)(pkt_data + 0); // tcp all header

	//--------------//
	LocConn0.IPA_Hdr.daddr = pTCP->_ipHeader.daddr;
	LocConn0.IPA_Hdr.saddr = pTCP->_ipHeader.saddr;
	if ((len > (sizeof(uip_eth_hdr) + sizeof(ip_header))) && ((pTCP->_ipHeader.proto == UIP_PROTO_UDP) || (pTCP->_ipHeader.proto == UIP_PROTO_TCP))) {
		LocConn0.IPA_Hdr.dport = pTCP->_tcpHeader.dport;
		LocConn0.IPA_Hdr.sport = pTCP->_tcpHeader.sport;
	}
	else {
		LocConn0.IPA_Hdr.dport = 0;
		LocConn0.IPA_Hdr.sport = 0;
	}
	LocConn0.pDevInfo = pInfom;
	pLocConn = pAYCLNT_FindLocConnByIPA_Rvs(&LocConn0.IPA_Hdr, 0, 0, _ETH_NULL_);
	//______________________________________________________________________________________//
	if (pLocConn != nullptr) {
		memcpy(&pLocConn->dest, &pTCP->_ethHeader.src, sizeof(uip_eth_addr));

		LenSend = ((len + 15) & 0xFFF0) + sizeof(AY_GWDRCTHDR);
		pPckt = (unsigned char*)_AY_MallocMemory(LenSend);///< allocate memory for income data
		pGwDH = (AY_GWDRCTHDR *)((Ui08 *)pPckt);
		pGwDH->_Test10 = PACKET_TEST_DATA12;
		pGwDH->_ConM2M_Id = pLocConn->ConM2M_Id;
		pGwDH->_DevNo = pInfom->DevRead._id;
		pCont = pPckt + sizeof(AY_GWDRCTHDR);

		memcpy(pCont, pkt_data, len);
		//---------------------------//
		pTCP = (tcp_headerAll *)(pCont + 0); // tcp all header
		pTCP->_ethHeader.dest = pLocConn->src;
		pTCP->_ipHeader.daddr = pLocConn->Osaddr;
		pTCP->_ipHeader.saddr = pLocConn->Odaddr;
		//-----------------------------//
		AY_Crypt_AES128((Ui08 *)&AY_Ram.AY_Sessionkey[0], pCont, ((len + 15) & 0xFFF0));
		//------- SEND
		TCP_header_init(&AY_TCPheader);
		TCP_header_load(&AY_TCPheader, SrvEth_Address, SrvIP_Address, CngFile.ServerPort, MyEth_Address, MyIP_Address, MyClientInstPort, 1, 1, (_PSH | _ACK));
#if STEP_TEST==1
		printf("********* STEP D6 *************\n********* STEP D6 *************\n********* STEP D6 *************\n");
		printf("AYCLNT--> PACKET (1)				GW1	--->	SRV		(A request packet for Mirror(Reflection) Device)\n ");
		AYPRINT_TCP_Header(&AY_TCPheader);
#endif
		return (TCP_packet_send(_MAIN_SCKT, &AY_TCPheader, (Ui08 *)pPckt, LenSend));
	}
	return -1;
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

	AYCLNT_CoreInit();///< INIT !!!
	MyClientInstPort = MyTCP_StartPort;
	AY_Client_Flags = 0;///< clear all flags
	AY_ClientInitLoop = 0;
	while (1) {
#if CLIENT_DEMO
		if (!AY_Client_Intro) {
			printf("============ CLIENT DEMO PROJECT =================\n\n");
			printf("Enter Client No (1-2)\n\n"); 
			GetVal = getchar();
			//------------------------------//
			AYFILE_TestCertFile(1);
			AYFILE_ReadCertFile();
			AYFILE_TestConfigFile(1);
			AYFILE_ConfigFileUpdate();
			//------------------------------//
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
				strcpy((char *)&MySocketBuff[0], "tcp src port ");
				AY_ConvertUi32AddToStrRet(CngFile.ServerPort, (char *)&MySocketBuff[0] );
				if (AYSCKT_Socket_Init(_MAIN_SCKT, (Ui08 *)&MyMac[0], &MyIP_Address.byte1, CngFile.ServerPort, 0, AY_MainSocket_CallBack, AY_ClientInitLoop) == 1) {
					AY_Client_Initied = 1;
					AY_Client_GetMACadr = 1;
					AY_Client_WaitMACadr = 1;
				}

			}
		}
		else if (!AY_Client_GetSrvIPadr) {
			strcpy((char *)&MySocketBuff[0], "tcp src port ");
			AY_ConvertUi32AddToStrRet(CngFile.DNSPort, (char *)&MySocketBuff[0] );
			strcat((char *)&MySocketBuff[0], " and tcp dst port ");
			AY_ConvertUi32AddToStrRet((MyClientInstPort + 1), (char *)&MySocketBuff[0] );
			if (AYSCKT_Socket_Init(_SLVS_SCKT, (Ui08 *)&MyMac[0], &MyIP_Address.byte1, CngFile.ServerPort, 0, AY_SocketRead_CallBack, AY_ClientInitLoop) == 1) {
				AY_Client_GetSrvIPadr = 1;
				AY_Client_WaitSrvIPadr = 1;
			}
		}
#else
		if (!AY_Client_Intro) {
			GetVal = 0;
#if CLIENT_DEMO2
			printf("============ CLIENT DEMO PROJECT =================\n\n");
			printf("Enter Client No (1-2)\n\n");
			GetVal = getchar();
			if ((GetVal != '1') && (GetVal != '2')) {
				GetVal = 0;
			}
			else {
				GetVal -= '0';
			}

#endif
			printf("============ CLIENT PROJECT =================\n\n");
			printf("================ START =================\n\n");
			AYFILE_SelectConfigFile(GetVal);
			AYFILE_TestCertFile(1);
			AYFILE_ReadCertFile();
			AYFILE_TestConfigFile(1);
			AYFILE_ConfigFileUpdate();
			AY_Client_Intro = 1;
		}
		else if (!AY_Client_Initied) {
			Ui08 Temp[45];
			if (AY_Client_DynamicIP) {
				*((Ui32 *)&MyIP_Address) = 0;
			}
			else {
				*((Ui32 *)&MyIP_Address) = *((Ui32 *)&CngFile.NetIpAddress);
			}
			if (AYSCKT_Socket_Init(_MAIN_SCKT, (Ui08 *)&MyMac[0], &MyIP_Address.byte1, CngFile.ServerPort, 0, AY_MainSocket_CallBack, AY_ClientInitLoop) == 1) {

				printf("IP address: %s\n", AY_ConvertIPToStrRet((Ui08 *)(((Ui32 *)&MyIP_Address.byte1) + _IP_), (char*)&Temp[0]));
				printf("Subnet address: %s\n", AY_ConvertIPToStrRet((Ui08 *)(((Ui32 *)&MyIP_Address.byte1) + _SUBNET_), (char*)&Temp[0]));
				printf("Subnet mask: %s\n", AY_ConvertIPToStrRet((Ui08 *)(((Ui32 *)&MyIP_Address.byte1) + _MASK_), (char*)&Temp[0]));
				printf("Gateway address: %s\n", AY_ConvertIPToStrRet((Ui08 *)(((Ui32 *)&MyIP_Address.byte1) + _GW_), (char*)&Temp[0]));

				AY_Client_Initied = 1;
			}
		}
		else if (!AY_Client_GetMACadr) {
			if (!AY_Client_WaitMACadr) {
				if (AYSCKT_Socket_Init(_SLVS_SCKT, (Ui08 *)&MyMac[0], &MyIP_Address.byte1, 0, 0, AY_SocketRead_CallBack, AY_ClientInitLoop) == 1) {
					memcpy(&MyEth_Address.addr[0], &MyMac[0], sizeof(uip_eth_addr));
					AY_StartSlaveListenMAC(*(uip_eth_addr *)&MyMac[0], _ETH_DST_);
					AYSCKT_WhoHasIP(_SLVS_SCKT, *((uip_eth_addr *)&DefaultMac[0]), MyIP_Addresses._gateway, *((uip_eth_addr *)&MyMac[0]), MyIP_Address, MyIP_Addresses._gateway);
					AY_Client_WaitMACadr = 1; 
					AY_Client_MAC_NotFound = 0;
					j = 0;
				}
			}
			else {		
#if CLIENT_DEMO2
				if (GetVal > 0) {
					memcpy(&MyEth_Address.addr[0], &DEMO_CLNT_MAC[GetVal][0], 6);
#if REMOTE_TEST==1
					memcpy(&SrvEth_Address.addr[0], &DefaultMac[0], 6);
#else
					memcpy(&SrvEth_Address.addr[0], &MyMac[0], 6);
#endif
					AY_Client_GetMACadr = 1;
				}

#else
				if (++j < 10) {
					AY_Delay(30000);
					AYSCKT_WhoHasIP(_SLVS_SCKT, *((uip_eth_addr *)&DefaultMac[0]), MyIP_Addresses._gateway, *((uip_eth_addr *)&MyMac[0]), MyIP_Address, MyIP_Addresses._gateway);

				}
				else {					
					AY_ClientInitLoop ++;
					AYSCKT_FilterFreeB(_MAIN_SCKT);
					AYSCKT_FilterFreeB(_SLVS_SCKT);
					AY_Client_WaitMACadr = 0;
					AY_Client_Initied = 0;
				}
#endif
			}
		}
		else if (!AY_Client_GetSrvIPadr) {			
			if (AY_IsStringToIP((char *)&CngFile.ServerDns[0]) == 1) {
				p = (char *)CngFile.ServerDns;
				*((Ui32 *)&SrvIP_Address) = AY_ConvertStringToIP(&p);
				AY_Client_GetSrvIPadr = 1;
			}
			else {
				if (!AY_Client_WaitSrvIPadr) {
					if (AY_DNS_Query((char *)&CngFile.ServerDns[0], (ip_address *)&CngFile.DNSIp) == 1) {
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
		}
#endif
		else if (!AY_Client_SendServer) {
			AY_Ram.AY_DeviceCnt = 0;
			AY_Ram.AY_DevPcktNo = 0;
			AYFILE_ClearFile((char*)&AddIP_File[0]);
			AYFILE_AddIPsToFile((char*)&AddIP_File[0], CngFile.NetInterfaceName, 0, 0, 0, 0, 1);
			AY_SendDeviceStartToServer(_GNRT_NEW);
			
			AY_Client_SendServer = 1;
		}
		else if (!AY_Client_RecvServer) {
			//.. wait
		}
		else if (!AY_Client_GenerateRemoteDevs) {
			AYFILE_AddIPsToFile((char*)&AddIP_File[0], CngFile.NetInterfaceName, 0, 0xFFFF, 0, 0, 1);
			AYFILE_CloseFile((char*)&AddIP_File[0]);
			if (AY_Ram.AY_DevPcktNo >0) {
				AY_Client_GenerateRemoteDevs = 1;
				if (AY_Client_RemoteDevListEnd) {
					AY_Client_RemoteDevListEnd = 0;
					AY_Ram.AY_DevPcktNo = 0;
				}
			}
			else {
				AY_Client_SendServer = 0;
			}			
		}
		else if (!AY_Client_ListenThreads) {
			AY_StartSlaveListen();
			AY_Client_ListenThreads = 1;
		}
		else {
			AYCLNT_CoreDoTask();
			AYCLNT_RemoteDevTimeoutTest();
				///< check processes
		}
		Sleep(1);
	}

	//===================================================//

#endif
	return 0;
}



