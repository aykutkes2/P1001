#pragma once
#include <AY_Type.h>

#pragma pack(push, 8)

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
	uint8_t byte1;
	uint8_t byte2;
	uint8_t byte3;
	uint8_t byte4;
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
	uint8_t	ver_ihl;		// Version (4 bits) + Internet header length (4 bits)
	uint8_t	tos;			// Type of service 
	uint16_t tlen;			// Total length 
	uint16_t identification; // Identification
	uint16_t flags_fo;		// Flags (3 bits) + Fragment offset (13 bits)
	uint8_t	ttl;			// Time to live
	uint8_t	proto;			// Protocol
	uint16_t crc;			// Header checksum
	ip_address	saddr;		// Source address
	ip_address	daddr;		// Destination address
	//u_int	op_pad;			// Option + Padding
}ip_header;

/* UDP header*/
typedef struct udp_header
{
	uint16_t sport;			// Source port
	uint16_t dport;			// Destination port
	uint16_t len;			// Datagram length
	uint16_t crc;			// Checksum
}udp_header;

/* UDP header complete */
typedef struct udp_headerAll
{
	uip_eth_hdr		_ethHeader;		// ETHERNET Header	14 bytes
	ip_header		_ipHeader;		// IP Header		20 bytes
	udp_header		_udpHeader;		// UDP Header		8 bytes
}udp_headerAll;						// total			42 bytes
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

#pragma pack(pop)

extern int AY_ClientSocket_Init(Ui08 idx, Ui08 *pMAC, Ui08 *pAdr, Ui16 rPort, char *pfilter, void *pCallBack, Ui32 _A);
extern int UDP_header_init(udp_headerAll * UDP_header);
extern int UDP_header_load(udp_headerAll * UDP_header, uip_eth_addr dest, ip_address	daddr, Ui16 dport, uip_eth_addr src, ip_address	saddr, Ui16 sport);
extern int UDP_packet_send(Ui08 idx, udp_headerAll * UDP_header, Ui08 *pBuff, int len);


extern int AY_ClientFilterSetA(Ui08 idx, char *pfilter);
extern int AY_ClientFilterFreeA(Ui08 idx);
extern int AY_ClientFilterFreeB(Ui08 idx);


extern int AY_ClientSocket_main(void);


//=================== DNS ===========
//DNS header structure
struct DNS_HEADER
{
	Ui16 id; // identification number

	union {
		Ui08 __xcv;
		struct {
			Ui08 rd : 1; // recursion desired
			Ui08 tc : 1; // truncated message
			Ui08 aa : 1; // authoritive answer
			Ui08 opcode : 4; // purpose of message
			Ui08 qr : 1; // query/response flag
		};
	};
	union {
		Ui08 __ycv;
		struct {
			Ui08 rcode : 4; // response code
			Ui08 cd : 1; // checking disabled
			Ui08 ad : 1; // authenticated data
			Ui08 z : 1; // its z! reserved
			Ui08 ra : 1; // recursion available
		};
	};	

	Ui16 q_count; // number of question entries
	Ui16 ans_count; // number of answer entries
	Ui16 auth_count; // number of authority entries
	Ui16 add_count; // number of resource entries
};

//Constant sized fields of query structure
struct QUESTION
{
	Ui16 qtype;
	Ui16 qclass;
};

//Constant sized fields of the resource record structure
#pragma pack(push, 1)
struct R_DATA
{
	Ui16 type;
	Ui16 _class;
	Ui32 ttl;
	Ui16 data_len;
};
#pragma pack(pop)

//Pointers to resource record contents
struct RES_RECORD
{
	Ui08 *name;
	struct R_DATA *resource;
	Ui08 *rdata;
};

//Structure of a Query
typedef struct
{
	Ui08 *name;
	struct QUESTION *ques;
} QUERY;


//Types of DNS resource records 
#define T_A 1 //Ipv4 address
#define T_NS 2 //Nameserver
#define T_CNAME 5 // canonical name
#define T_SOA 6 /* start of authority zone */
#define T_PTR 12 /* domain name pointer */
#define T_MX 15 //Mail server



