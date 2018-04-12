/////////////////////////////////////////////////////////////////////////////
/// @File name: SocketAPI.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	跨平台socket接口封装
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef _SOCK_API_H_
#define _SOCK_API_H_

#ifdef _WIN32
#define _WIN32_PLATFORM_
#else
#define _LINUX_PLATFORM_
#endif

#if defined(_WIN32_PLATFORM_)
#include <winsock2.h>
typedef SOCKET HSocket;


#endif

#if defined(_LINUX_PLATFORM_)
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

typedef int HSocket;
#define SOCKET_ERROR  (-1)
#define INVALID_SOCKET  0
#endif


typedef struct TransResult
{
	int nbytes;
	int nresult;
} TRANSRESULT, *PTRANSRESULT;

enum TimeOutType
{
	TimeOut_Recv,
	TimeOut_Send,
	TimeOut_Linger
};


#define INVALIDSOCKHANDLE   INVALID_SOCKET

int socket_Init();
void socket_Uninit();

HSocket socket_Open(int tcpudp);
HSocket socket_Accept(HSocket hs, sockaddr_in *addr);
int socket_Bind(HSocket hs, sockaddr_in *addr);
int socket_Listen(HSocket hs, int maxconn);
int socket_Connect(HSocket hs, sockaddr_in *addr);
void socket_Close(HSocket &handle);
TransResult socket_Send(HSocket hs, const char *ptr, int nbytes);
TransResult socket_Recv(HSocket hs, char *ptr, int nbytes);

int socket_SetBlock(HSocket hs, bool bblock);
int socket_SetTimeOut(HSocket hs, TimeOutType type, int iTimeout);
int socket_GetLastError();
void socket_GetAddr_in(const char *ip, int port, sockaddr_in *addr);
void socket_GetIp(sockaddr_in *addr, char *ip);
bool socket_Isvalid(HSocket handle);


#endif	//_SOCK_API_H_
