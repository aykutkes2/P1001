
#include "pch.h"
#undef UNICODE

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
#include <process.h>

#include <pcap.h>

/* The TCP and IP headers. */
struct uip_tcpip_hdr {
	/* IPv4 header. */
	uint8_t vhl,
		tos,
		len[2],
		ipid[2],
		ipoffset[2],
		ttl,
		proto;
	uint16_t ipchksum;
	uint16_t srcipaddr[2],
		destipaddr[2];

	/* TCP header. */
	uint16_t srcport,
		destport;
	uint8_t seqno[4],
		ackno[4],
		tcpoffset,
		flags,
		wnd[2];
	uint16_t tcpchksum;
	uint8_t urgp[2];
	uint8_t optdata[4];
};

/* 4 bytes IP address */
typedef struct ip_address
{
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
}ip_address;

/**
 * Representation of a 48-bit Ethernet address.
 */
struct uip_eth_addr {
	uint8_t addr[6];
};

/**
 * The Ethernet header.
 */
struct uip_eth_hdr {
	struct uip_eth_addr dest;
	struct uip_eth_addr src;
	uint16_t type;
};

/* IPv4 header */
typedef struct ip_header
{
	u_char	ver_ihl;		// Version (4 bits) + Internet header length (4 bits)
	u_char	tos;			// Type of service 
	u_short tlen;			// Total length 
	u_short identification; // Identification
	u_short flags_fo;		// Flags (3 bits) + Fragment offset (13 bits)
	u_char	ttl;			// Time to live
	u_char	proto;			// Protocol
	u_short crc;			// Header checksum
	ip_address	saddr;		// Source address
	ip_address	daddr;		// Destination address
	//u_int	op_pad;			// Option + Padding
}ip_header;

/* UDP header*/
typedef struct udp_header
{
	u_short sport;			// Source port
	u_short dport;			// Destination port
	u_short len;			// Datagram length
	u_short crc;			// Checksum
}udp_header;

/* UDP header complete */
typedef struct udp_headerAll
{
	uip_eth_hdr		_ethHeader;		// ETHERNET Header
	ip_header		_ipHeader;		// IP Header
	udp_header		_udpHeader;		// UDP Header
}udp_headerAll;
//==============================================================//
struct ethip_hdr {
	struct uip_eth_hdr ethhdr;
	/* IP header. */
	uint8_t vhl,
		tos,
		len[2],
		ipid[2],
		ipoffset[2],
		ttl,
		proto;
	uint16_t ipchksum;
	uint16_t srcipaddr[2],
		destipaddr[2];
};

#define _HTONS(n) ((uint16_t)((((uint16_t)(n)) << 8) | (((uint16_t)(n)) >> 8)))
#define UIP_LLH_LEN     14	/* The link level header length. */
#define UIP_IPH_LEN    20    /* Size of IP header */
#define UIP_UDPH_LEN    8    /* Size of UDP header */
#define UIP_TCPH_LEN   20    /* Size of TCP header */
#define UIP_IPUDPH_LEN (UIP_UDPH_LEN + UIP_IPH_LEN)    /* Size of IP +  UDP  header */
#define UIP_IPTCPH_LEN (UIP_TCPH_LEN + UIP_IPH_LEN)    /* Size of IP +  TCP  header */
#define UIP_TCPIP_HLEN UIP_IPTCPH_LEN
#define UIP_PROTO_ICMP  1
#define UIP_PROTO_TCP   6
#define UIP_PROTO_UDP   17
#define UIP_ETHTYPE_ARP 0x0806
#define UIP_ETHTYPE_IP  0x0800
#define UIP_ETHTYPE_IP6 0x86dd

#define BUF   ((struct uip_tcpip_hdr *)&buff[UIP_LLH_LEN])

/*---------------------------------------------------------------------------*/
uint16_t mhtons(uint16_t val){
	return _HTONS(val);
}
/*---------------------------------------------------------------------------*/
static uint16_t chksum(uint16_t sum, uint8_t *data, uint16_t len){
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
uint16_t uip_chksum(uint16_t *data, uint16_t len){
	return mhtons(chksum(0, (uint8_t *)data, len));
}
/*---------------------------------------------------------------------------*/
uint16_t uip_ipchksum(uint8_t *buff){
	uint16_t sum;

	sum = chksum(0, (uint8_t *)&buff[UIP_LLH_LEN], UIP_IPH_LEN);
	return (sum == 0) ? 0xffff : mhtons(sum);
}
/*---------------------------------------------------------------------------*/
static uint16_t upper_layer_chksum(uint8_t proto, uint8_t *buff){
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
uint16_t uip_tcpchksum(uint8_t *buff){
	return upper_layer_chksum(UIP_PROTO_TCP, buff);
}
/*---------------------------------------------------------------------------*/
uint16_t uip_udpchksum(uint8_t *buff){
	return upper_layer_chksum(UIP_PROTO_UDP, buff);
}




/* prototype of the packet handler */
void _packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);

static pcap_t *fs;

void ListenSocket_Thread(void *param) {
	printf("In thread function \n");

	//printf("\nlistening on %s...\n", d->description);

	/* start the capture */
	pcap_loop(fs, 0, _packet_handler, NULL);

	printf("Thread function ends \n");
	_endthread();
}


int main(void)//(int argc, char **argv)
{
	HANDLE hThread;
	//==============================//
	pcap_if_t *alldevs;
	pcap_if_t *d;
	int inum;
	u_int netmask;
	char packet_filter[20] = "udp dst port ";// "ip and udp";
	struct bpf_program fcode;
	//==============================//
	pcap_t *fp;
	char errbuf[PCAP_ERRBUF_SIZE];
	u_char packet[114];
	int i=0;
	udp_headerAll	*pHdr;
	const u_char MAC[2][6] = { {0xff,0xff,0xff,0xff,0xff,0xff}, {0x4c,0xcc,0x6a,0xec,0x5d,0x94} };
	const u_char IPs[2][6] = { {0xc0,0xa8,0x02,0xac}, {0xff,0xff,0xff,0xff} };
	const uint16_t	PortNo = 1982;
	const uint16_t	SPortNo = 19820;
	//const char Adapter[] = "\Device\NPF_{9AA07D59-DB07-4870-9007-8D5069AD68DB}";
	//unsigned short crc1, crc2;
	//==============================//
	/* Retrieve the device list */
	if (pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);
		exit(1);
	}

	/* Print the list */
	for (d = alldevs; d; d = d->next)
	{
		printf("%d. %s", ++i, d->name);

		/*if (d->flags & PCAP_IF_CONNECTION_STATUS_CONNECTED) {
			printf(" (%s)\n", d->description);
			inum = i;
		}*/


		if (d->description) {
			printf(" (%s) ------ (%s)\n", d->description, d->name);
		}
		else
			printf(" (No description available)\n");
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
	if (pcap_compile(fs, &fcode, AY_ConvertUi32AddToStrRet(PortNo, packet_filter)/*packet_filter*/, 1, netmask) < 0)
	{
		fprintf(stderr, "\nUnable to compile the packet filter. Check the syntax.\n");
		printf("\n %s\n", packet_filter);
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}

	printf("\n %s\n", packet_filter);

	//set the filter
	if (pcap_setfilter(fs, &fcode) < 0)
	{
		fprintf(stderr, "\nError setting the filter.\n");
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}

	hThread = (HANDLE)_beginthread(ListenSocket_Thread, 16 * 1024/*0*/, NULL);
	//===================================================//


	// Ethernet Header
	pHdr = (udp_headerAll *)&packet[0];
	memcpy(&pHdr->_ethHeader.dest.addr[0], &MAC[0][0], 6);
	memcpy(&pHdr->_ethHeader.src.addr[0], &MAC[1][0], 6);
	pHdr->_ethHeader.type = mhtons(UIP_ETHTYPE_IP);
	// IP Header	
	pHdr->_ipHeader.ver_ihl						= 0x45;				///< Version:4		Length:20
	pHdr->_ipHeader.tos							= 0x00;				///< Not ECN-Capable Transport
	pHdr->_ipHeader.tlen						= mhtons(0x0064);	///< length 100 bytes
	pHdr->_ipHeader.identification				= mhtons(0x6f63);	///< identification
	pHdr->_ipHeader.flags_fo					= mhtons(0x0000);	///< Fragment offset
	pHdr->_ipHeader.ttl							= 0x80;				///< time to live 128
	pHdr->_ipHeader.proto						= 0x11;				///< UDP
	pHdr->_ipHeader.crc							= 0;				///< header checksum
	memcpy(&pHdr->_ipHeader.saddr, &IPs[0][0], 4);					///< sorce address
	memcpy(&pHdr->_ipHeader.daddr, &IPs[1][0], 4);					///< destination address
	// UDP Header	
	pHdr->_udpHeader.sport						= mhtons(SPortNo);	///< source port
	pHdr->_udpHeader.dport						= mhtons(PortNo);	///< destination port
	pHdr->_udpHeader.len						= mhtons(0x0050);	///< length 72 bytes
	pHdr->_udpHeader.crc						= 0;				///< checksum
	// DATA
	/* Fill the rest of the packet */
	for (i = 0; i < mhtons(pHdr->_udpHeader.len); i++){
		packet[i+ sizeof(udp_headerAll)] = (u_char)i;
	}
	pHdr->_ipHeader.crc = ~(uip_ipchksum(&packet[0])); 				///< header checksum
	pHdr->_udpHeader.crc = ~(uip_udpchksum(&packet[0]));  			///< checksum
	// SEND
	i = 0;
	while (1) {
		/* Send down the packet */
		if (pcap_sendpacket(fp,	// Adapter
			packet,				// buffer with the packet
			(mhtons(pHdr->_ipHeader.tlen) + 14)					// size
		) != 0)
		{
			fprintf(stderr, "\nError sending the packet: %s\n", pcap_geterr(fp));
			return 3;
		}
		i++;
		printf("Data Has been Sent !!!! Count = %d", i);
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