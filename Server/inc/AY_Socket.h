#pragma once
#include <AY_Type.h>

#define _THRDS_			2
#define THREAD_STACK_SIZE		(16*1024)

#pragma pack(push, 1)

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
typedef union ip_address
{
	uint32_t longip;
	struct {
		uint8_t byte1;
		uint8_t byte2;
		uint8_t byte3;
		uint8_t byte4;
	};
}ip_address;

/* 4 IP addresses */
typedef struct ip_addressAll
{
	ip_address	_ip;
	ip_address	_gateway;
	ip_address	_mask;
	ip_address	_subnet;
}ip_addressAll;

enum _IP_ALL{
	_IP_,
	_GW_,
	_MASK_,
	_SUBNET_
};

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

typedef struct tcp_header {
	uint16_t sport; // Source port
	uint16_t dport; // Destination port
	uint32_t seqnum; // Sequence Number
	uint32_t acknum; // Acknowledgement number
	uint8_t th_off; // Header length
	uint8_t flags; // packet flags
	uint16_t win; // Window size
	uint16_t crc; // Header Checksum
	uint16_t urgptr; // Urgent pointer...still don't know what this is...
}tcp_header;

/* UDP header complete */
typedef struct udp_headerAll
{
	uip_eth_hdr		_ethHeader;		// ETHERNET Header	14 bytes
	ip_header		_ipHeader;		// IP Header		20 bytes
	udp_header		_udpHeader;		// UDP Header		8 bytes
}udp_headerAll;						// total			42 bytes

/* TCP header complete */
typedef struct tcp_headerAll
{
	uip_eth_hdr		_ethHeader;		// ETHERNET Header	14 bytes
	ip_header		_ipHeader;		// IP Header		20 bytes
	tcp_header		_tcpHeader;		// TCP Header		20 bytes
}tcp_headerAll;						// total			54 bytes

/* ICMP header complete */
typedef struct icmp_headerAll
{
	uip_eth_hdr		_ethHeader;		// ETHERNET Header	14 bytes
	ip_header		_ipHeader;		// IP Header		20 bytes
}icmp_headerAll;						// total			34 bytes
/* ICMP header */
typedef struct _ihdr {
	uint8_t    i_type;
	uint8_t    i_code; /* type sub code */
	uint16_t    i_cksum;
	uint16_t    i_id;
	uint16_t    i_seq;
	/* This is not the std header, but we reserve space for time */
	//uint32_t    timestamp;
	uint8_t    data[32];///< abcdefghijklmnopqrstuvwabcdefghi
}icmp_data;

typedef struct _icmp_All
{
	uip_eth_hdr		_ethHeader;		// ETHERNET Header	14 bytes
	ip_header		_ipHeader;		// IP Header		20 bytes
	icmp_data		_data;			// Data				40 bytes
}icmp_All;							// total			74 bytes

/* ARP header */
typedef struct _arphdr {
	uip_eth_hdr		_ethHeader;		// ETHERNET Header	14 bytes
	uint16_t		hw_type;		///< 0x0001
	uint16_t		proto_type;		///< 0x0800 IPv4
	uint8_t			hw_size;		///< 0x0006
	uint8_t			proto_size;		///< 0x0004
	uint16_t		request;		///< 0x0001
	uip_eth_addr	SenderMac;		///< MyMAC
	ip_address		SenderIp;		///< MyIP
	uip_eth_addr	TargetMac;		///< 00:00:00:00:00:00
	ip_address		TargetIp;		///< 192.168.2.1
}arp_headerAll;

/* TCP/IP header info */
typedef struct ip_headerAll
{
	//struct uip_eth_addr		dest;
	//struct uip_eth_addr		src;
	ip_address				saddr;		// Source address
	ip_address				daddr;		// Destination address
	uint16_t				sport;		// Source port
	uint16_t				dport;		// Destination port
}ip_headerAll;						// total			24 bytes

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

extern int AYSCKT_Socket_Init(Ui08 idx, Ui08 *pMAC, Ui08 *pAdr, Ui16 rPort, char *pfilter, void *pCallBack, Ui32 _A);
extern int UDP_header_init(udp_headerAll * UDP_header);
extern int UDP_header_load(udp_headerAll * UDP_header, uip_eth_addr dest, ip_address	daddr, Ui16 dport, uip_eth_addr src, ip_address	saddr, Ui16 sport);
extern int UDP_packet_send(Ui08 idx, udp_headerAll * UDP_header, Ui08 *pBuff, int len);
extern int TCP_packet_send(Ui08 idx, tcp_headerAll * TCP_header, Ui08 *pBuff, int len);
extern int ICMP_packet_send(Ui08 idx, icmp_headerAll * ICMP_header, Ui08 *pBuff, int len);
extern int AYSCKT_PingIP(Ui08 idx, uip_eth_addr dest, ip_address	daddr, uip_eth_addr src, ip_address	saddr, Ui08 SeqNo);
extern int AYSCKT_WhoHasIP(Ui08 idx, uip_eth_addr dest, ip_address	daddr, uip_eth_addr src, ip_address	saddr, ip_address	search);
extern int UDP_packet_check(Ui08 *pBuff, int *pLen);

enum _AYSCKT_ETH_ {
	_ETH_NULL_,
	_ETH_SRC_,
	_ETH_DST_
};
extern int AY_ChngPacketDest(udp_headerAll *pUDP, uip_eth_addr *pEth, Ui08 SrcDst);
extern void AYPRINT_UDP_Header(udp_headerAll *pUDP);

extern int AYSCKT_FilterSetA(Ui08 idx, char *pfilter);
extern int AYSCKT_FilterSetB(Ui08 idx, char *pfilter);
extern int AYSCKT_FilterFreeA(Ui08 idx);
extern int AYSCKT_FilterFreeB(Ui08 idx);

extern int AYSCKT_StartThread(void *pCallBack);


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



