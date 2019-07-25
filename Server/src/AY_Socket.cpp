//#include "pch.h"
#undef UNICODE

#define LOCATION_DTKM		1
#define GATEWAY_NO			1

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

#include<iostream>
#include<cstdio>
#include<fstream>
#include<sstream>
#include<string>
#include<cstdlib>
#include<conio.h>
#include<windows.h>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <AY_Printf.h>
#include <AY_Functions.h>
#include <AY_Socket.h>
#include <AY_Memory.h>
#include <process.h>
#include <pcap.h>
//

#define BUF   ((struct uip_tcpip_hdr *)&buff[UIP_LLH_LEN])

/*---------------------------------------------------------------------------*/
uint16_t mhtons(uint16_t val) {
	return _HTONS(val);
}
/*---------------------------------------------------------------------------*/
static uint16_t chksum(uint16_t sum, uint8_t *data, uint16_t len) {
	uint16_t t;
	uint8_t *dataptr;
	uint8_t *last_byte;

	dataptr = data;
	last_byte = data + len - 1;

	while (dataptr < last_byte) {	/* At least two more bytes */
		t = (dataptr[0] << 8) + dataptr[1];
		sum += t;
		if (sum < t) {
			sum++;		/* carry */
		}
		dataptr += 2;
	}

	if (dataptr == last_byte) {
		t = (dataptr[0] << 8) + 0;
		sum += t;
		if (sum < t) {
			sum++;		/* carry */
		}
	}

	/* Return sum in host byte order. */
	return sum;
}
/*---------------------------------------------------------------------------*/
uint16_t uip_chksum(uint16_t *data, uint16_t len) {
	return mhtons(chksum(0, (uint8_t *)data, len));
}
/*---------------------------------------------------------------------------*/
uint16_t uip_ipchksum(uint8_t *buff) {
	uint16_t sum;

	sum = chksum(0, (uint8_t *)&buff[UIP_LLH_LEN], UIP_IPH_LEN);
	return (sum == 0) ? 0xffff : mhtons(sum);
}
/*---------------------------------------------------------------------------*/
static uint16_t upper_layer_chksum(uint8_t proto, uint8_t *buff) {
	uint16_t upper_layer_len;
	uint16_t sum;

	upper_layer_len = mhtons(*(uint16_t *)&BUF->len[0]) - UIP_IPH_LEN;//			(((uint16_t)(BUF->len[0]) << 8) + BUF->len[1]) - UIP_IPH_LEN;

	/* First sum pseudoheader. */

	/* IP protocol and length fields. This addition cannot carry. */
	sum = upper_layer_len + proto;
	/* Sum IP source and destination addresses. */
	sum = chksum(sum, (uint8_t *)&BUF->srcipaddr[0], 2 * sizeof(ip_address));

	/* Sum TCP header and data. */
	sum = chksum(sum, (uint8_t *)&buff[UIP_IPH_LEN + UIP_LLH_LEN],
		upper_layer_len);

	return (sum == 0) ? 0xffff : mhtons(sum);
}
/*---------------------------------------------------------------------------*/
uint16_t uip_tcpchksum(uint8_t *buff) {
	return upper_layer_chksum(UIP_PROTO_TCP, buff);
}
/*---------------------------------------------------------------------------*/
uint16_t uip_udpchksum(uint8_t *buff) {
	return upper_layer_chksum(UIP_PROTO_UDP, buff);
}


/* prototype of the packet handler */
void _packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);

static pcap_if_t *alldevs;
static pcap_t *fs;
static Ui32 MyNetMask;
//static struct bpf_program My_fcode;
//static u_char SocketBuff[1500];
static udp_headerAll MyUDP_header;

typedef struct _AYSCKT_Thread {
	void *pMyHandler;
	pcap_t *pfp;
	struct bpf_program My_fcode;
}AYSCKT_Thread;
//AYSCKT_Thread *pThrd;
AYSCKT_Thread Thrd[_THRDS_];

#if (_THRDS_>0)
void Socket_Thread_0(void *pParams) {
	printf("THREAD=0 In thread function \n");
	pcap_loop(Thrd[0].pfp, 0, (pcap_handler)Thrd[0].pMyHandler, NULL);
	printf("THREAD=0 Thread function ends \n");
	_endthread();
}
#endif
#if (_THRDS_>1)
void Socket_Thread_1(void *pParams) {
	printf("THREAD=1 In thread function \n");
	pcap_loop(Thrd[1].pfp, 0, (pcap_handler)Thrd[1].pMyHandler, NULL);
	printf("THREAD=1 Thread function ends \n");
	_endthread();
}
#endif
#if (_THRDS_>2)
void Socket_Thread_2(void *pParams) {
	printf("THREAD=2 In thread function \n");
	pcap_loop(Thrd[2].pfp, 0, (pcap_handler)Thrd[2].pMyHandler, NULL);
	printf("THREAD=2 Thread function ends \n");
	_endthread();
}
#endif
#if (_THRDS_>3)
void Socket_Thread_3(void *pParams) {
	printf("THREAD=0 In thread function \n");
	pcap_loop(Thrd[3].pfp, 0, (pcap_handler)Thrd[3].pMyHandler, NULL);
	printf("THREAD=3 Thread function ends \n");
	_endthread();
}
#endif
#if (_THRDS_>4)
void Socket_Thread_4(void *pParams) {
	printf("THREAD=4 In thread function \n");
	pcap_loop(Thrd[4].pfp, 0, (pcap_handler)Thrd[4].pMyHandler, NULL);
	printf("THREAD=4 Thread function ends \n");
	_endthread();
}
#endif

int AYSCKT_FilterSet(pcap_t *fp, bpf_program *pfcode, char *pfilter, Ui32 netmask) {
	//struct bpf_program fcode;
	//compile the filter
	if (pcap_compile(fp, pfcode, pfilter, 1, netmask) < 0)
	{
		fprintf(stderr, "\nUnable to compile the packet filter. Check the syntax.\n");
		printf("\n %s\n", pfilter);
		/* Free the device list */
		return PCAP_ERROR;
	}

	printf("\n %s\n", pfilter);
	//set the filter
	if (pcap_setfilter(fp, pfcode) < 0)
	{
		fprintf(stderr, "\nError setting the filter.\n");
		/* Free the device list */
		return PCAP_ERROR;
	}
	return 1;
}
int AYSCKT_FilterFree(struct bpf_program *pfcode) {
	pcap_freecode(pfcode);
	return 1;
}

int AYSCKT_StartThread(void *pCallBack) {
	HANDLE hThread;

	hThread = (HANDLE)_beginthread((_beginthread_proc_type)pCallBack, THREAD_STACK_SIZE, 0);

	return 1;
}

int AYSCKT_StartThreadX(Ui08 idx) {
	switch (idx) {
#if (_THRDS_>1)
	case 1:		AYSCKT_StartThread((void *)Socket_Thread_1);	break;
#endif
#if (_THRDS_>2)
	case 2:		AYSCKT_StartThread((void *)Socket_Thread_2);	break;
#endif
#if (_THRDS_>3)
	case 3:		AYSCKT_StartThread((void *)Socket_Thread_3);	break;
#endif
#if (_THRDS_>4)
	case 4:		AYSCKT_StartThread((void *)Socket_Thread_4);	break;
#endif
#if (_THRDS_>0)
	default:		AYSCKT_StartThread((void *)Socket_Thread_0);	break;
#endif
	}
	return 1;
}
int AYSCKT_FilterSetA(Ui08 idx, char *pfilter) {
	AYSCKT_FilterSet(Thrd[idx].pfp, &Thrd[idx].My_fcode, pfilter, MyNetMask);
	AYSCKT_StartThreadX(idx);
	return 1;
}

int AYSCKT_FilterFreeA(Ui08 idx) {
	pcap_breakloop(Thrd[idx].pfp);
	return 1;
}

int AYSCKT_FilterFreeB(Ui08 idx) {
	pcap_breakloop(Thrd[idx].pfp);
	pcap_close(Thrd[idx].pfp);
	pcap_freecode(&Thrd[idx].My_fcode);
	if (idx == 0) {
		pcap_freealldevs(alldevs);
	}
	return 1;
}

int AYSCKT_Socket_Init(Ui08 idx, Ui08 *pMAC, Ui08 *pAdr, Ui16 rPort, char *pfilter, void *pCallBack, Ui32 _A) {
	//==============================//
	pcap_if_t *d;
	pcap_addr_t *a, *b;
	int i = 0;
	int ListenPort = 0;
	char DevFound = 0;
	Ui32 j = 0;
	//==============================//
	pcap_t *fp;
	char errbuf[PCAP_ERRBUF_SIZE];
	char packet_filter[40] = "udp dst port ";
	char packet_filterIP[40] = "ip src host ";

	d = NULL;
	a = NULL;
	b = NULL;
	/* Retrieve the device list */
	if (pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);
		return PCAP_ERROR;
	}

	for (pcap_if_t* pInterface(alldevs); pInterface != 0; pInterface = pInterface->next)
	{
		if ((pInterface->flags & PCAP_IF_LOOPBACK) != 0) // Skip loopback interfaces
		{
			continue;
		}

		for (d = alldevs; d != NULL; d = d->next) {
			printf("%s:", d->name);
			for (a = d->addresses; a != NULL; a = a->next) {
				if (a->addr->sa_family == AF_INET) {
					printf(" %s", inet_ntoa(((struct sockaddr_in*)a->addr)->sin_addr));
					if (*((Ui32 *)pAdr) == 0) {
						if ((Ui32)(((struct sockaddr_in*)a->addr)->sin_addr.S_un.S_addr)) {
							//if (j == _A) {
							//	printf("\n\n_A = %d I will use this device !!!\n\n",_A); 
							//	printf(" %s \n\n", inet_ntoa(((struct sockaddr_in*)a->addr)->sin_addr));
							b = a;
							DevFound = 1;
							//}
							j++;
						}
					}
					else if (*((Ui32 *)pAdr) == (Ui32)(((struct sockaddr_in*)a->addr)->sin_addr.S_un.S_addr)) {
						b = a;
						DevFound = 1;
					}
				}
			}
			printf("\n");
			if (DevFound) {
				a = b;
				printf("\n\n_A = %d I will use this device !!!\n\n", _A);
				printf(" %s \n\n", inet_ntoa(((struct sockaddr_in*)a->addr)->sin_addr));
				goto L_DevFound;
			}
		}
	}
L_DevFound:
	if ((a != NULL) && (d != NULL)) {
		*((Ui32 *)pAdr) = (Ui32)(((struct sockaddr_in*)a->addr)->sin_addr.S_un.S_addr);
		/* Open the adapter */
		if ((fp = pcap_open_live(d->name/*argv[1]*/,		// name of the device
			65536,			// portion of the packet to capture. It doesn't matter in this case 
			1,				// promiscuous mode (nonzero means promiscuous)
			1000,			// read timeout
			errbuf			// error buffer
		)) == NULL)
		{
			fprintf(stderr, "\nUnable to open the adapter. %s is not supported by WinPcap\n", d->name/*argv[1]*/);
			return PCAP_ERROR;
		}
		//*pFp = (void *)fp;
		if (d->addresses != NULL)
			/* Retrieve the mask of the first address of the interface */
			MyNetMask = ((struct sockaddr_in *)(d->addresses->netmask))->sin_addr.S_un.S_addr;
		else
			/* If the interface is without addresses we suppose to be in a C class network */
			MyNetMask = 0xffffff;

		// select filter type
		if (pfilter != nullptr) {
			ListenPort = 0;
			//strcpy(packet_filter, pfilter);
		}
		else if (rPort != 0) {
			ListenPort = 1;
			AY_ConvertUi32AddToStrRet(rPort, packet_filter);
			pfilter = &packet_filter[0];
		}
		else {
			ListenPort = 0;
			strcat(packet_filterIP, inet_ntoa(((struct sockaddr_in*)a->addr)->sin_addr));
			pfilter = &packet_filterIP[0];
		}

		if (AYSCKT_FilterSet(fp, &Thrd[idx].My_fcode, pfilter, MyNetMask) < 0) {
			pcap_freealldevs(alldevs);
			return PCAP_ERROR;
		}

		Thrd[idx].pfp = fp;
		Thrd[idx].pMyHandler = pCallBack;

		AYSCKT_StartThreadX(idx);
		//===================================================//
	}
	else {
		//pcap_freealldevs(alldevs);
		return PCAP_ERROR;
	}

	//pcap_freealldevs(alldevs);
	return 1;
}

int UDP_header_init(udp_headerAll * UDP_header) {
	// Ethernet Header
	UDP_header->_ethHeader.type = mhtons(UIP_ETHTYPE_IP);
	// IP Header	
	UDP_header->_ipHeader.ver_ihl = 0x45;				///< Version:4		Length:20
	UDP_header->_ipHeader.tos = 0x00;				///< Not ECN-Capable Transport
	UDP_header->_ipHeader.identification = mhtons(0x6f63);	///< identification
	UDP_header->_ipHeader.flags_fo = mhtons(0x0000);	///< Fragment offset
	UDP_header->_ipHeader.ttl = 0x80;				///< time to live 128
	UDP_header->_ipHeader.proto = 0x11;				///< UDP
	// UDP Header	
	// END
	return 1;
}

int UDP_header_load(udp_headerAll * UDP_header, uip_eth_addr dest, ip_address	daddr, Ui16 dport, uip_eth_addr src, ip_address	saddr, Ui16 sport) {
	// Ethernet Header
	memcpy(&UDP_header->_ethHeader.dest.addr[0], &dest.addr[0], 6);
	memcpy(&UDP_header->_ethHeader.src.addr[0], &src.addr[0], 6);
	// IP Header	
	memcpy(&UDP_header->_ipHeader.saddr, (u_char *)&saddr, 4);					///< sorce address
	memcpy(&UDP_header->_ipHeader.daddr, (u_char *)&daddr, 4);					///< destination address
	// UDP Header	
	UDP_header->_udpHeader.sport = mhtons(sport);	///< source port
	UDP_header->_udpHeader.dport = mhtons(dport);	///< destination port	
	// END	
	return 1;
}

int UDP_packet_send(Ui08 idx, udp_headerAll * UDP_header, Ui08 *pBuff, int len) {
	int i = sizeof(udp_headerAll);
	Ui08 *ptr = (Ui08	*)_AY_MallocMemory(len+sizeof(udp_headerAll));///< max packet size
	udp_headerAll	*pHdr = (udp_headerAll *)&ptr[0];
	//pcap_t *fp = (pcap_t *)pFp;
	//i = sizeof(udp_headerAll);
	memcpy(&ptr[0], UDP_header, i);
	memcpy(&ptr[i], pBuff, len);
	i += len;
	pHdr->_ipHeader.tlen = mhtons(i - 14);	///< length 100 bytes
	pHdr->_ipHeader.crc = 0;				///< header checksum
	pHdr->_udpHeader.len = mhtons(len + 8);	///< length 72 + 8 bytes
	pHdr->_udpHeader.crc = 0;				///< checksum
	pHdr->_ipHeader.crc = ~(uip_ipchksum(&ptr[0])); 				///< header checksum
	pHdr->_udpHeader.crc = ~(uip_udpchksum(&ptr[0]));  			///< checksum

	/* Send down the packet */
	if (pcap_sendpacket(Thrd[idx].pfp, ptr, i) != 0)
	{
		fprintf(stderr, "\nError sending the packet: %s\n", pcap_geterr(Thrd[idx].pfp));
		_AY_FreeMemory((unsigned char*)ptr);
		return PCAP_ERROR;
	}
	printf("Data Has been Sent !!!! Count = %d", i);
	_AY_FreeMemory((unsigned char*)ptr);
	return 1;
}

int UDP_packet_check(Ui08 *pBuff, int *pLen) {
	udp_headerAll * pHdr;
	//int len;

	Ui16 ipCrc0, ipCrc1, ipCrc2;
	Ui16 udpCrc0, udpCrc1, udpCrc2;

	pHdr = (udp_headerAll *)pBuff;

	ipCrc0 = pHdr->_ipHeader.crc;
	ipCrc1 = uip_ipchksum(pBuff);
	ipCrc2 = ~(uip_ipchksum(pBuff));

	udpCrc0 = pHdr->_udpHeader.crc;
	udpCrc1 = uip_udpchksum(pBuff);
	udpCrc2 = ~(uip_udpchksum(pBuff));


	if ( (uip_ipchksum(pBuff)==0xFFFF) && (uip_udpchksum(pBuff)==0xFFFF) ) {
		*pLen = mhtons(pHdr->_udpHeader.len) - 8;
		return 1;
	}
	else {
		*pLen = 0;
		return -1;
	}
}

