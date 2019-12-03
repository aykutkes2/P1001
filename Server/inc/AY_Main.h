#pragma once

#pragma pack(push, 1)

#pragma pack(pop)


#define _MAC_OS_		( __APPLE__ | __MACH__ )
#if (!_MAC_OS_)
#define _LINUX_OS_		( __linux__ | linux | __linux )
#if (!_LINUX_OS_)
#define _WINDOWS_OS_	( _WIN32 | _WIN64 )	
#endif
#endif


#if _LINUX_OS_
#include <stdio.h> 
#include <stdlib.h> 
#include <cstdio>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <chrono>
#include <unistd.h>
//#include <arpa/inet.h>

#include <netpacket/packet.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <netinet/if_ether.h>
#else
#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#include<winsock2.h>
#include<stdio.h>
#pragma comment(lib, "Ws2_32.lib")
#include <process.h>
#include <windows.h>
#endif

#include <AY_Type.h>
#include <AY_Socket.h>

#if _LINUX_OS_
#define Sleep(mSec)											usleep((mSec*1000))
#define closesocket(_Sckt_)									close(_Sckt_)

#endif




