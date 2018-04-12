#include "SocketAPI.h"

#include <stdio.h>
#include <string.h>
#include <fcntl.h>


#if defined(_WIN32_PLATFORM_)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define ISSOCKHANDLE(x)  (x!=INVALID_SOCKET)
#define BLOCKREADWRITE      0
#define NONBLOCKREADWRITE   0
#define SENDNOSIGNAL        0
#define ETRYAGAIN(x)     (x==WSAEWOULDBLOCK||x==WSAETIMEDOUT)
#define gxsprintf   sprintf_s

#pragma comment(lib, "ws2_32.lib")
#endif


#if defined(_LINUX_PLATFORM_)
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define ISSOCKHANDLE(x)    (x>0)
#define BLOCKREADWRITE      MSG_WAITALL
#define NONBLOCKREADWRITE   MSG_DONTWAIT
#define SENDNOSIGNAL        MSG_NOSIGNAL
#define ETRYAGAIN(x)        (x==EAGAIN||x==EWOULDBLOCK)
#define gxsprintf           snprintf
#endif

void socket_GetAddr_in(const char *ip, int port, sockaddr_in *addr)
{
	memset(addr, 0, sizeof(sockaddr_in));
	addr->sin_family = AF_INET;            /*地址类型为AF_INET*/
	if (ip)
	{
		addr->sin_addr.s_addr = inet_addr(ip);
	}
	else
	{
		/*网络地址为INADDR_ANY，这个宏表示本地的任意IP地址，因为服务器可能有多个网卡，每个网卡也可能绑定多个IP地址，
		这样设置可以在所有的IP地址上监听，直到与某个客户端建立了连接时才确定下来到底用哪个IP地址*/
		addr->sin_addr.s_addr = htonl(INADDR_ANY);
	}
	addr->sin_port = htons(port);   /*端口号*/
}
void socket_GetIp(sockaddr_in *addr, char *ip)
{
	unsigned char *p = (unsigned char *)(&(addr->sin_addr));
	gxsprintf(ip, 17, "%u.%u.%u.%u", *p, *(p + 1), *(p + 2), *(p + 3));
}

int socket_GetLastError()
{
#if defined(_WIN32_PLATFORM_)
	return WSAGetLastError();
#endif

#if defined(_LINUX_PLATFORM_)
	return errno;
#endif
}

bool socket_Isvalid(HSocket handle)
{
	return ISSOCKHANDLE(handle);
}

void socket_Close(HSocket &handle)
{
	if (ISSOCKHANDLE(handle))
	{
#if defined(_WIN32_PLATFORM_)
		closesocket(handle);
#endif

#if defined(_LINUX_PLATFORM_)
		close(handle);
#endif
		handle = INVALIDSOCKHANDLE;
	}
}

HSocket socket_Open(int tcpudp)
{
	int protocol = 0;
	HSocket hs;
#if defined(_WIN32_PLATFORM_)
	if (tcpudp == SOCK_STREAM) protocol = IPPROTO_TCP;
	else if (tcpudp == SOCK_DGRAM) protocol = IPPROTO_UDP;
#endif
	hs = socket(AF_INET, tcpudp, protocol);
	return hs;
}
int socket_Bind(HSocket hs, sockaddr_in *paddr)
{
	return bind(hs, (struct sockaddr *)paddr, sizeof(sockaddr_in));
}
int socket_Listen(HSocket hs, int maxconn)
{
	return listen(hs, maxconn);
}
HSocket socket_Accept(HSocket hs, sockaddr_in *paddr)
{
#if defined(_WIN32_PLATFORM_)
	int cliaddr_len = sizeof(sockaddr_in);
#endif
#if defined(_LINUX_PLATFORM_)
	socklen_t cliaddr_len = sizeof(sockaddr_in);
#endif
	return accept(hs, (struct sockaddr *)paddr, &cliaddr_len);
}

int socket_Connect(HSocket hs, sockaddr_in *addr)
{
	return connect(hs, (struct sockaddr *)addr, sizeof(sockaddr_in));
}

//
// if timeout occurs, nbytes=-1, nresult=1
// if socket error, nbyte=-1, nresult=-1
// if the other side has disconnected in either block mode or nonblock mode, nbytes=0, nresult=-1
// otherwise nbytes= the count of bytes sent , nresult=0
TransResult socket_Send(HSocket hs, const char *ptr, int nbytes)
{
	TransResult rt = { 0 };

	if (!ptr || nbytes < 1)
	{
		return rt;
	}

	//Linux: flag can be MSG_DONTWAIT, MSG_WAITALL, 使用MSG_WAITALL的时候, socket 必须是处于阻塞模式下，否则WAITALL不能起作用
	rt.nbytes = send(hs, ptr, nbytes, BLOCKREADWRITE | SENDNOSIGNAL);
	if (rt.nbytes > 0)
	{
		rt.nresult = (rt.nbytes == nbytes) ? 0 : 1;
	}
	else if (rt.nbytes == 0)
	{
		rt.nresult = -1;
	}
	else
	{
		rt.nresult = socket_GetLastError();
		rt.nresult = ETRYAGAIN(rt.nresult) ? 1 : -1;
	}

	return rt;
}



// if timeout occurs, nbytes=-1, nresult=1
// if socket error, nbyte=-1, nresult=-1
// if the other side has disconnected in either block mode or nonblock mode, nbytes=0, nresult=-1
TransResult socket_Recv(HSocket hs, char *ptr, int nbytes)
{
	TransResult rt = { 0 };

	if (!ptr || nbytes < 1)
	{
		return rt;
	}

    //by lixf: BLOCKREADWRITE-->0
    //rt.nbytes = recv(hs, ptr, nbytes, BLOCKREADWRITE);
    rt.nbytes = recv(hs, ptr, nbytes, 0);
	if (rt.nbytes > 0)
	{
		return rt;
	}
	else if (rt.nbytes == 0)
	{
		rt.nresult = -1;
	}
	else
	{
		rt.nresult = socket_GetLastError();
		rt.nresult = ETRYAGAIN(rt.nresult) ? 1 : -1;
	}

	return rt;
}

int socket_SetBlock(HSocket hs, bool bblock)
{
	unsigned long mode;
	if (ISSOCKHANDLE(hs))
	{
#if defined(_WIN32_PLATFORM_)
		mode = bblock ? 0 : 1;
		return ioctlsocket(hs, FIONBIO, &mode);
#endif

#if defined(_LINUX_PLATFORM_)
		mode = fcntl(hs, F_GETFL, 0);                  //获取文件的flags值。
													   //设置成阻塞模式      非阻塞模式
		return bblock ? fcntl(hs, F_SETFL, mode&~O_NONBLOCK) : fcntl(hs, F_SETFL, mode | O_NONBLOCK);
#endif
	}
	return -1;
}

int socket_SetTimeOut(HSocket hs, TimeOutType type, int iTimeout)   //in milliseconds
{
	int rt = -1;
	if (ISSOCKHANDLE(hs))
	{
		rt = 0;
		if (type == TimeOut_Recv)
		{
#if defined(_WIN32_PLATFORM_)
			if (iTimeout > 0 && rt == 0)
			{
				rt = rt | (setsockopt(hs, SOL_SOCKET, SO_RCVTIMEO, (char *)&iTimeout, sizeof(int)) == 0 ? 0 : 0x2);
			}
#endif
#if defined(_LINUX_PLATFORM_)
			struct timeval timeout;
			if (iTimeout > 0 && rt == 0)
			{
				timeout.tv_sec = iTimeout / 1000;
				timeout.tv_usec = (iTimeout % 1000) * 1000;
				rt = rt | (setsockopt(hs, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeval)) == 0 ? 0 : 0x2);
			}
#endif
		}
		else if (type == TimeOut_Send)
		{
#if defined(_WIN32_PLATFORM_)
			if (iTimeout > 0 && rt == 0)
			{
				rt = rt | (setsockopt(hs, SOL_SOCKET, SO_SNDTIMEO, (char *)&iTimeout, sizeof(int)) == 0 ? 0 : 0x4);
			}
#endif
#if defined(_LINUX_PLATFORM_)
			struct timeval timeout;
			if (iTimeout > 0 && rt == 0)
			{
				timeout.tv_sec = iTimeout / 1000;
				timeout.tv_usec = (iTimeout % 1000) * 1000;
				rt = rt | (setsockopt(hs, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeval)) == 0 ? 0 : 0x4);
			}
#endif
		}
		else if (type == TimeOut_Linger)
		{
#if defined(_WIN32_PLATFORM_)
			if (iTimeout > -1)
			{
				struct linger  lin;
				lin.l_onoff = iTimeout;
				lin.l_linger = iTimeout;
				rt = setsockopt(hs, SOL_SOCKET, SO_DONTLINGER, (const char*)&lin, sizeof(linger)) == 0 ? 0 : 0x1;
			}
#endif
#if defined(_LINUX_PLATFORM_)
			struct timeval timeout;
			if (iTimeout > -1)
			{
				struct linger  lin;
				lin.l_onoff = iTimeout > 0 ? 1 : 0;
				lin.l_linger = iTimeout / 1000;
				rt = setsockopt(hs, SOL_SOCKET, SO_LINGER, (const char*)&lin, sizeof(linger)) == 0 ? 0 : 0x1;
			}
#endif
		}
	}
	return rt;
}


int socket_Init()
{
#if defined(_WIN32_PLATFORM_)
	WSADATA  Ws;
	//Init Windows Socket
	if (WSAStartup(MAKEWORD(2, 2), &Ws) != 0)
	{
		return -1;
	}
#endif
	return 0;
}
void socket_Uninit()
{
#if defined(_WIN32_PLATFORM_)
	WSACleanup();
#endif
}

