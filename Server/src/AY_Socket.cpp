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
#if DEVICE_CLIENT
#include <AY_Client.h>
#include <AY_ClientPrjtBased.h>
#else
#include <AY_Server.h>
#include <AY_ServerPrjtBased.h>
#endif
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
/*---------------------------------------------------------------------------*/
uint16_t icmp_udpchksum(uint8_t *buff) {
	return upper_layer_chksum(UIP_PROTO_ICMP, buff);
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
	pcap_if_t *d;
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

int AYSCKT_FilterSetB(Ui08 idx, char *pfilter) {
	pcap_t *fp;
	AYSCKT_Thread ThrdBuff;
	char errbuf[PCAP_ERRBUF_SIZE];
	char EmptyFilter[] = "";

	ThrdBuff = Thrd[idx];

	//AYSCKT_FilterFreeB(idx);
	pcap_breakloop(Thrd[idx].pfp);
	pcap_close(Thrd[idx].pfp);
	pcap_freecode(&Thrd[idx].My_fcode);

	Thrd[idx] = ThrdBuff;

	/* Open the adapter */
	if ((fp = pcap_open_live(Thrd[idx].d->name/*argv[1]*/,		// name of the device
		65536,			// portion of the packet to capture. It doesn't matter in this case 
		1,				// promiscuous mode (nonzero means promiscuous)
		1000,			// read timeout
		errbuf			// error buffer
	)) == NULL)
	{
		fprintf(stderr, "\nUnable to open the adapter. %s is not supported by WinPcap\n", Thrd[idx].d->name/*argv[1]*/);
		return PCAP_ERROR;
	}

	if (AYSCKT_FilterSet(fp, &Thrd[idx].My_fcode, &EmptyFilter[0], MyNetMask) < 0) {
		pcap_freealldevs(alldevs);
		return PCAP_ERROR;
	}

	if (AYSCKT_FilterSet(fp, &Thrd[idx].My_fcode, pfilter, MyNetMask) < 0) {
		pcap_freealldevs(alldevs);
		return PCAP_ERROR;
	}

	Thrd[idx].pfp = fp;

	AYSCKT_StartThreadX(idx);
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
	//========================================================================//
	if (AY_DynamicIP) {
		bpf_u_int32 ip_raw; /* IP address as integer */
		bpf_u_int32 subnet_mask_raw; /* Subnet mask as integer */

		/* Get device info */
		if (pcap_lookupnet(d->name, &ip_raw, &subnet_mask_raw, errbuf) < 0) {
			fprintf(stderr, "Error in pcap_lookupnet: %s\n", errbuf);
			return PCAP_ERROR;
		}

		*(((Ui32 *)pAdr) + _SUBNET_) = subnet_mask_raw;
		*(((Ui32 *)pAdr) + _MASK_) = ip_raw;
		*(((Ui32 *)pAdr) + _GW_) = ip_raw + 0x01000000;///< not good!
	}
	else {
		*(((Ui32 *)pAdr) + _MASK_) = *((Ui32 *)&AY_NetworkSubnetMask);
		*(((Ui32 *)pAdr) + _GW_) = *((Ui32 *)&AY_NetworkGatewayIp);
		*(((Ui32 *)pAdr) + _SUBNET_) = *((Ui32 *)&AY_NetSubnetIp);
	}
	printf("Subnet address: %s\n", AY_ConvertIPToStrRet((Ui08 *)(((Ui32 *)pAdr) + _SUBNET_), &errbuf[0]));
	printf("Subnet mask: %s\n", AY_ConvertIPToStrRet((Ui08 *)(((Ui32 *)pAdr) + _MASK_), &errbuf[0]));
	printf("Gateway address: %s\n", AY_ConvertIPToStrRet((Ui08 *)(((Ui32 *)pAdr) + _GW_), &errbuf[0]));
//==========================================================================================================//
	if ((a != NULL)&& (d != NULL)) {
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
		if (d->addresses != NULL) {
			/* Retrieve the mask of the first address of the interface */
			MyNetMask = ((struct sockaddr_in *)(d->addresses->netmask))->sin_addr.S_un.S_addr;
		}
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
		Thrd[idx].d = d;
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

Ui08 AY_SocketBuff[4096];
int UDP_packet_send(Ui08 idx, udp_headerAll * UDP_header, Ui08 *pBuff, int len) {
	int i = sizeof(udp_headerAll);
	Ui08 *ptr = &AY_SocketBuff[0];// (Ui08	*)_AY_MallocMemory(len + sizeof(udp_headerAll) + 64);///< max packet size
	udp_headerAll	*pHdr = (udp_headerAll *)&ptr[0];
	
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
		//_AY_FreeMemory((unsigned char*)ptr);
		return PCAP_ERROR;
	}
	printf("Data Has been Sent !!!! Count = %d", i);
	AY_SendCnt += i;
	//_AY_FreeMemory((unsigned char*)ptr);
	return 1;
}

int TCP_packet_send(Ui08 idx, tcp_headerAll * TCP_header, Ui08 *pBuff, int len) {
	int i = sizeof(tcp_headerAll);
	Ui08 *ptr = (Ui08	*)_AY_MallocMemory(len + sizeof(tcp_headerAll));///< max packet size
	tcp_headerAll	*pHdr = (tcp_headerAll *)&ptr[0];

	memcpy(&ptr[0], TCP_header, i);
	memcpy(&ptr[i], pBuff, len);
	i += len;
	pHdr->_ipHeader.tlen = mhtons(i - 14);	///< length 100 bytes
	pHdr->_ipHeader.crc = 0;				///< header checksum
	//pHdr->_tcpHeader.th_off = mhtons(20);	///< Header length
	pHdr->_tcpHeader.crc = 0;				///< checksum
	pHdr->_ipHeader.crc = ~(uip_ipchksum(&ptr[0])); 				///< header checksum
	pHdr->_tcpHeader.crc = ~(uip_tcpchksum(&ptr[0]));  			///< checksum

	/* Send down the packet */
	if (pcap_sendpacket(Thrd[idx].pfp, ptr, i) != 0)
	{
		fprintf(stderr, "\nError sending the packet: %s\n", pcap_geterr(Thrd[idx].pfp));
		_AY_FreeMemory((unsigned char*)ptr);
		return PCAP_ERROR;
	}
	printf("Data Has been Sent !!!! Count = %d", i);
	AY_SendCnt += i;
	_AY_FreeMemory((unsigned char*)ptr);
	return 1;
}

/*
 *			I N _ C K S U M
 *
 * Checksum routine for Internet Protocol family headers (C Version)
 *
 */
Ui16 in_cksum(Ui16 *addr, Ui32 len)
{
	Ui32 nleft = len;
	Ui16 *w = addr;
	Ui16 answer;
	Ui32 sum = 0;

	/*
	 *  Our algorithm is simple, using a 32 bit accumulator (sum),
	 *  we add sequential 16 bit words to it, and at the end, fold
	 *  back all the carry bits from the top 16 bits into the lower
	 *  16 bits.
	 */
	while (nleft > 1) {
		sum += *w++;
		nleft -= 2;
	}

	/* mop up an odd byte, if necessary */
	if (nleft == 1) {
		Ui16	u = 0;

		*(Ui08 *)(&u) = *(Ui08 *)w;
		sum += u;
	}

	/*
	 * add back carry outs from top 16 bits to low 16 bits
	 */
	sum = (sum >> 16) + (sum & 0xffff);	/* add hi 16 to low 16 */
	sum += (sum >> 16);			/* add carry */
	answer = ~sum;				/* truncate to 16 bits */
	return (answer);
}

int ICMP_packet_send(Ui08 idx, icmp_headerAll * ICMP_header, Ui08 *pBuff, int len) {
	int i = sizeof(icmp_headerAll);
	Ui08 *ptr = (Ui08	*)_AY_MallocMemory(len + sizeof(icmp_headerAll));///< max packet size
	icmp_headerAll	*pHdr = (icmp_headerAll *)&ptr[0];

	memcpy(&ptr[0], ICMP_header, i);
	memcpy(&ptr[i], pBuff, len);
	i += len;
	pHdr->_ipHeader.tlen = mhtons(i - 14);	///< length 100 bytes
	pHdr->_ipHeader.crc = 0;				///< header checksum
	pHdr->_ipHeader.crc = ~(uip_ipchksum(&ptr[0])); 				///< header checksum

	/* Send down the packet */
	if (pcap_sendpacket(Thrd[idx].pfp, ptr, i) != 0)
	{
		fprintf(stderr, "\nError sending the packet: %s\n", pcap_geterr(Thrd[idx].pfp));
		_AY_FreeMemory((unsigned char*)ptr);
		return PCAP_ERROR;
	}
	printf("Data Has been Sent !!!! Count = %d", i);
	AY_SendCnt += i;
	_AY_FreeMemory((unsigned char*)ptr);
	return 1;
}

int ICMP_header_init(icmp_headerAll * ICMP_header ) {
	// Ethernet Header
	ICMP_header->_ethHeader.type = mhtons(UIP_ETHTYPE_IP);
	// IP Header	
	ICMP_header->_ipHeader.ver_ihl = 0x45;				///< Version:4		Length:20
	ICMP_header->_ipHeader.tos = 0x00;				///< Not ECN-Capable Transport
	ICMP_header->_ipHeader.identification = mhtons(0x6f63);	///< identification
	ICMP_header->_ipHeader.flags_fo = mhtons(0x0000);	///< Fragment offset
	ICMP_header->_ipHeader.ttl = 0x80;				///< time to live 128
	ICMP_header->_ipHeader.proto = 0x01;				///< ICMP
	// END
	return 1;
}

int AYSCKT_PingIP(Ui08 idx, uip_eth_addr dest, ip_address	daddr, uip_eth_addr src, ip_address	saddr, Ui08 SeqNo) {
	icmp_data		ICMP_Data;
	icmp_headerAll	ICMP_header;
	icmp_headerAll	*pICMP_header = (icmp_headerAll	*)&ICMP_header;
	//Ui08			*ptr = (Ui08*)&ICMP;
	//Ui16			i = sizeof(icmp_All);

	char _PingData[] = "abcdefghijklmnopqrstuvwabcdefghi";

	ICMP_header_init(pICMP_header);
	pICMP_header->_ethHeader.dest = dest;
	pICMP_header->_ethHeader.src = src;
	pICMP_header->_ipHeader.daddr = daddr;
	pICMP_header->_ipHeader.saddr = saddr;
	//----------------------------------------//
	ICMP_Data.i_cksum = 0;
	ICMP_Data.i_code = 0;
	ICMP_Data.i_type = 0;
	ICMP_Data.i_id = mhtons(1);
	ICMP_Data.i_seq = mhtons(SeqNo);
	memcpy(&ICMP_Data.data[0], &_PingData[0], 32);
	//----------------------------------------//
	ICMP_Data.i_cksum = in_cksum((Ui16 *)&ICMP_Data, sizeof(icmp_data));//
	//----------------------------------------//
	return ICMP_packet_send(idx, pICMP_header, (Ui08 *)&ICMP_Data, sizeof(icmp_data));
	
}

int AYSCKT_WhoHasIP(Ui08 idx, uip_eth_addr dest, ip_address	daddr, uip_eth_addr src, ip_address	saddr, ip_address	search) {
	arp_headerAll		ARP_Pckt;
	
	// Ethernet Header
	ARP_Pckt._ethHeader.dest = dest;
	ARP_Pckt._ethHeader.src = src;
	ARP_Pckt._ethHeader.type = mhtons(UIP_ETHTYPE_ARP);
	// ARP
	ARP_Pckt.hw_type = mhtons(0x0001);		///< 0x0001
	ARP_Pckt.proto_type = mhtons(0x0800);		///< 0x0800 IPv4
	ARP_Pckt.hw_size = 0x06;		///< 0x06
	ARP_Pckt.proto_size = 0x04;		///< 0x04
	ARP_Pckt.request = mhtons(0x0001);		///< 0x0001
	ARP_Pckt.SenderMac = src;		///< MyMAC
	ARP_Pckt.SenderIp = saddr;		///< MyIP
	memset(&ARP_Pckt.TargetMac, 0, sizeof(uip_eth_addr));///< 00:00:00:00:00:00
	ARP_Pckt.TargetIp = search;		///< 192.168.2.1
	// SEND
	/* Send down the packet */
	if (pcap_sendpacket(Thrd[idx].pfp, (Ui08 *)&ARP_Pckt, sizeof(arp_headerAll)) != 0)
	{
		fprintf(stderr, "\nARP - Error sending the packet: %s\n", pcap_geterr(Thrd[idx].pfp));
		return PCAP_ERROR;
	}
	printf("ARP - Data Has been Sent !!!! Count = %d", sizeof(arp_headerAll));
	AY_SendCnt += sizeof(arp_headerAll);
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


int AY_ChngPacketDest(udp_headerAll *pUDP, uip_eth_addr *pEth, Ui08 SrcDst) {
	udp_headerAll UDP0 = *pUDP;

	pUDP->_ethHeader.dest = UDP0._ethHeader.src;
	pUDP->_ethHeader.src = UDP0._ethHeader.dest;
	if (pEth != 0) {
		if (SrcDst == _ETH_SRC_) {
			pUDP->_ethHeader.src = *pEth;
		}
		else if (SrcDst == _ETH_DST_) {
			pUDP->_ethHeader.dest = *pEth;
		}
	}

	pUDP->_ipHeader.daddr = UDP0._ipHeader.saddr;
	pUDP->_ipHeader.saddr = UDP0._ipHeader.daddr;

	pUDP->_udpHeader.dport = UDP0._udpHeader.sport;
	pUDP->_udpHeader.sport = UDP0._udpHeader.dport;

	return 1;
}

void AYPRINT_UDP_Header(udp_headerAll *pUDP) {
	char Buff0[32], Buff1[32];
	printf("\t\t***************\tPACKET HEADER\t******************\n");
	printf("\t\tPROTOCOL:\t\t%d\n", pUDP->_ipHeader.proto);
	printf("\t- SOURCE -\t\t\t- DESTINATION -\n");
	Buff0[0] = 0; AY_HexToStr(&Buff0[0], &pUDP->_ethHeader.src.addr[0], 6, 3);
	Buff1[0] = 0; AY_HexToStr(&Buff1[0], &pUDP->_ethHeader.dest.addr[0], 6, 3);
	printf("\tETH:\t%s\tETH:\t%s\n", Buff0, Buff1);
	Buff0[0] = 0; AY_ConvertIPToStr(&pUDP->_ipHeader.saddr.byte1 , &Buff0[0]);
	Buff1[0] = 0; AY_ConvertIPToStr(&pUDP->_ipHeader.daddr.byte1, &Buff1[0]);
	printf("\tIP:\t%s\t\tIP:\t%s\n", Buff0, Buff1);
	printf("\tPORT:   \t%d\t\tPORT:   \t%d\n", mhtons(pUDP->_udpHeader.sport), mhtons(pUDP->_udpHeader.dport));
}
