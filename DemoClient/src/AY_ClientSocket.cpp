

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
#include <AY_ClientSocket.h>
#include <AY_ClientPrjtBased.h>
#include <AY_Client.h>
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
static u_char SocketBuff[1500];
static udp_headerAll MyUDP_header;

typedef struct _AYSCKT_Thread{
	void *pMyHandler; 
	pcap_t *pfp;
	struct bpf_program My_fcode;
}AYSCKT_Thread;
//AYSCKT_Thread *pThrd;
AYSCKT_Thread Thrd[2];

void ListenSocket_Thread(void *pParams) {
	//pThrd = &Thrd;// (AYSCKT_Thread *)pParams;
	printf("In thread function \n");

	//printf("\nlistening on %s...\n", d->description);

	/* start the capture */
	pcap_loop(Thrd[0].pfp, 0, /*_packet_handler*/(pcap_handler)Thrd[0].pMyHandler, NULL);

	printf("Thread function ends \n");
	_endthread();
}

void ListenOtherIPs_Thread(void *pParams) {
	//pThrd = &Thrd;// (AYSCKT_Thread *)pParams;
	printf("In thread function \n");

	//printf("\nlistening on %s...\n", d->description);

	/* start the capture */
	pcap_loop(Thrd[1].pfp, 0, (pcap_handler)Thrd[1].pMyHandler, NULL);

	printf("Thread function ends \n");
	_endthread();
}

void ListenSocket_ThreadOrj(void *pParams) {
	printf("In thread function \n");

	//printf("\nlistening on %s...\n", d->description);

	/* start the capture */
	pcap_loop(fs, 0, _packet_handler, NULL);

	printf("Thread function ends \n");
	_endthread();
}

int AY_ClientFilterSet(pcap_t *fp, bpf_program *pfcode, char *pfilter, Ui32 netmask) {
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
int AY_ClientFilterFree(struct bpf_program *pfcode) {
	pcap_freecode(pfcode);	
	return 1;
}

int AY_ClientStartThread(void *pCallBack) {
	HANDLE hThread;
	
	hThread = (HANDLE)_beginthread((_beginthread_proc_type)pCallBack, 16 * 1024/*0*/, 0);

	return 1;
}

int AY_ClientFilterSetA(Ui08 idx, char *pfilter) {
	//pcap_t *fp = (pcap_t *)pFp;
	AY_ClientFilterSet(Thrd[idx].pfp, &Thrd[idx].My_fcode, pfilter, MyNetMask);
	if (idx == 0) {
		AY_ClientStartThread((void *)ListenSocket_Thread);
	}
	else {
		AY_ClientStartThread((void *)ListenOtherIPs_Thread);
	}
	return 1;
}

int AY_ClientFilterFreeA(Ui08 idx) {
	pcap_breakloop(Thrd[idx].pfp);
	//pcap_close(Thrd.pfp);
	//pcap_freecode(&My_fcode);
	return 1;
}

int AY_ClientFilterFreeB(Ui08 idx) {
	pcap_breakloop(Thrd[idx].pfp);
	pcap_close(Thrd[idx].pfp);
	pcap_freecode(&Thrd[idx].My_fcode);
	if (idx == 0) {
		pcap_freealldevs(alldevs);
	}
	return 1;
}

int AY_ClientSocket_Init(Ui08 idx, Ui08 *pMAC, Ui08 *pAdr, Ui16 rPort, char *pfilter, void *pCallBack, Ui32 _A) {
	//==============================//
	//pcap_if_t *alldevs;
	pcap_if_t *d;
	pcap_addr_t *a,*b;
	int i = 0;
	int ListenPort = 0;	
	char DevFound = 0;
	Ui32 j=0;
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
								//e = d;
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
				printf("\n\n_A = %d I will use this device !!!\n\n",_A); 
				printf(" %s \n\n", inet_ntoa(((struct sockaddr_in*)a->addr)->sin_addr));
				goto L_DevFound;
			}
		}
	}
L_DevFound:
	//========================================================================//
	if (AY_Client_DynamicIP) {
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
		*(((Ui32 *)pAdr) + _MASK_) = *((Ui32 *)&CngFile.NetworkSubnetMask);
		*(((Ui32 *)pAdr) + _GW_) = *((Ui32 *)&CngFile.NetworkGatewayIp);
		*(((Ui32 *)pAdr) + _SUBNET_) = *((Ui32 *)&CngFile.NetSubnetIp);
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
		else if(rPort != 0) {
			ListenPort = 1;
			AY_ConvertUi32AddToStrRet(rPort, packet_filter);
			pfilter = &packet_filter[0];
		}
		else {
			ListenPort = 0;
			strcat(packet_filterIP, inet_ntoa(((struct sockaddr_in*)a->addr)->sin_addr));
			pfilter = &packet_filterIP[0];
		}

		if (AY_ClientFilterSet(fp, &Thrd[idx].My_fcode, pfilter, MyNetMask) < 0) {
			pcap_freealldevs(alldevs);
			return PCAP_ERROR;
		}

		Thrd[idx].pfp = fp;
		Thrd[idx].pMyHandler = pCallBack;
		
		if (idx == 0) {
			AY_ClientStartThread((void *)ListenSocket_Thread);
		}
		else {
			AY_ClientStartThread((void *)ListenOtherIPs_Thread);
		}
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
	Ui08 *ptr = (Ui08	*)_AY_MallocMemory(len + sizeof(udp_headerAll));///< max packet size
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





int AY_ClientSocket_main(void)//(int argc, char **argv)
{
	HANDLE hThread;
	//==============================//
	pcap_if_t *alldevs;
	pcap_if_t *d;
	int inum;
	u_int netmask;
	char packet_filter[20] = "udp dst port ";// "ip and udp";
	//struct bpf_program fcode;
	//==============================//
	pcap_t *fp;
	char errbuf[PCAP_ERRBUF_SIZE];
	u_char packet[114];
	int i = 0;
	//udp_headerAll	*pHdr;
	const u_char MAC[2][6] = { {0xff,0xff,0xff,0xff,0xff,0xff}, {0x4c,0xcc,0x6a,0xec,0x5d,0x94} };
	const u_char IPs[2][6] = { {0xc0,0xa8,0x02,0xac}, {0xff,0xff,0xff,0xff} };
	const uint16_t	PortNo = 1982;
	const uint16_t	SPortNo = 19820;
	//==============================//
	/* Retrieve the device list */
	if (pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);
		exit(1);
	}

	for (pcap_if_t* pInterface(alldevs); pInterface != 0; pInterface = pInterface->next)
	{
		if ((pInterface->flags & PCAP_IF_LOOPBACK) != 0) // Skip loopback interfaces
		{
			continue;
		}

		for (pcap_if_t *d = alldevs; d != NULL; d = d->next) {
			printf("%s:", d->name);
			for (pcap_addr_t *a = d->addresses; a != NULL; a = a->next) {
				if (a->addr->sa_family == AF_INET)
					printf(" %s", inet_ntoa(((struct sockaddr_in*)a->addr)->sin_addr));
			}
			printf("\n");
		}
	}

	/* Print the list */
	for (d = alldevs; d; d = d->next)
	{
		printf("%d. %s", ++i, d->name);

		if (d->description) {
			printf(" (%s) ------ (%s)\n", d->description, d->name);
		}
		else
			printf(" (No description available)\n");

		if ((d->flags & PCAP_IF_LOOPBACK)) {
			printf(" LOOPBACK !!!\n");
		}

		if ((d->flags & PCAP_IF_UP)) {
			printf(" UP !!!\n");
		}

		if ((d->flags & PCAP_IF_RUNNING)) {
			printf(" RUNNING !!!\n");
		}
		switch ((d->flags & PCAP_IF_CONNECTION_STATUS)) {
		case PCAP_IF_CONNECTION_STATUS_UNKNOWN:
			printf(" CONNECTION_STATUS_UNKNOWN !!!\n");
		break;
		case PCAP_IF_CONNECTION_STATUS_CONNECTED:
			printf(" CONNECTION_STATUS_CONNECTED !!!\n");
			break;
		case PCAP_IF_CONNECTION_STATUS_DISCONNECTED:
			printf(" CONNECTION_STATUS_DISCONNECTED !!!\n");
			break;
		case PCAP_IF_CONNECTION_STATUS_NOT_APPLICABLE:
			printf(" CONNECTION_STATUS_NOT_APPLICABLE !!!\n");
			break;
		}

	}

	if (i == 0)
	{
		printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
		return -1;
	}

	printf("Enter the interface number (1-%d):", i);
	scanf_s("%d", &inum);

	/* Check if the user specified a valid adapter */
	if (inum < 1 || inum > i)
	{
		printf("\nAdapter number out of range.\n");

		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}

	/* Jump to the selected adapter */
	for (d = alldevs, i = 0; i < inum - 1; d = d->next, i++);
	//==============================//

	/* Open the adapter */
	if ((fp = pcap_open_live(d->name/*argv[1]*/,		// name of the device
		65536,			// portion of the packet to capture. It doesn't matter in this case 
		1,				// promiscuous mode (nonzero means promiscuous)
		1000,			// read timeout
		errbuf			// error buffer
	)) == NULL)
	{
		fprintf(stderr, "\nUnable to open the adapter. %s is not supported by WinPcap\n", d->name/*argv[1]*/);
		return 2;
	}
	//============== RECEIVE =================================//

	/* Open the adapter */
	if ((fs = pcap_open_live(d->name/*argv[1]*/,		// name of the device
		65536,			// portion of the packet to capture. It doesn't matter in this case 
		1,				// promiscuous mode (nonzero means promiscuous)
		1000,			// read timeout
		errbuf			// error buffer
	)) == NULL)
	{
		fprintf(stderr, "\nUnable to open the adapter. %s is not supported by WinPcap\n", d->name/*argv[1]*/);
		return 2;
	}
	if (d->addresses != NULL)
		/* Retrieve the mask of the first address of the interface */
		netmask = ((struct sockaddr_in *)(d->addresses->netmask))->sin_addr.S_un.S_addr;
	else
		/* If the interface is without addresses we suppose to be in a C class network */
		netmask = 0xffffff;


	//compile the filter
	if (pcap_compile(fs, &Thrd[0].My_fcode, AY_ConvertUi32AddToStrRet(PortNo, packet_filter)/*packet_filter*/, 1, netmask) < 0)
	{
		fprintf(stderr, "\nUnable to compile the packet filter. Check the syntax.\n");
		printf("\n %s\n", packet_filter);
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}

	printf("\n %s\n", packet_filter);

	//set the filter
	if (pcap_setfilter(fs, &Thrd[0].My_fcode) < 0)
	{
		fprintf(stderr, "\nError setting the filter.\n");
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}

	hThread = (HANDLE)_beginthread(ListenSocket_ThreadOrj, 16 * 1024/*0*/, NULL);
	//===================================================//

	UDP_header_init(&MyUDP_header);
	UDP_header_load(&MyUDP_header, *((uip_eth_addr *)&MAC[0][0]), *((ip_address *)&IPs[1][0]), PortNo, *((uip_eth_addr *)&MAC[1][0]), *((ip_address *)&IPs[0][0]), SPortNo);
	
	/* Fill the rest of the packet */
	for (i = 0; i < 114; i++) {
		packet[i] = (u_char)i;
	}
	Thrd[0].pfp = fp;
	Thrd[0].pMyHandler = _packet_handler;
	while (1) {
		UDP_packet_send(0, &MyUDP_header, &packet[0], 114);
	}

	pcap_close(fp);
	return 0;
}

/* Callback function invoked by libpcap for every incoming packet */
void _packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
	struct tm *ltime;
	char timestr[16];
	ip_header *ih;
	udp_header *uh;
	u_int ip_len;
	u_short sport, dport;
	time_t local_tv_sec;

	/*
	 * unused parameter
	 */
	(VOID)(param);

	/* convert the timestamp to readable format */
	local_tv_sec = header->ts.tv_sec;
	ltime = localtime(&local_tv_sec);
	strftime(timestr, sizeof timestr, "%H:%M:%S", ltime);

	/* print timestamp and length of the packet */
	printf("%s.%.6d len:%d ", timestr, header->ts.tv_usec, header->len);

	/* retireve the position of the ip header */
	ih = (ip_header *)(pkt_data +
		14); //length of ethernet header

	/* retireve the position of the udp header */
	ip_len = (ih->ver_ihl & 0xf) * 4;
	uh = (udp_header *)((u_char*)ih + ip_len);

	/* convert from network byte order to host byte order */
	sport = ntohs(uh->sport);
	dport = ntohs(uh->dport);

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