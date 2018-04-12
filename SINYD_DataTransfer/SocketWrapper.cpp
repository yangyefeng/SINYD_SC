#include "SocketWrapper.h"
#include "memory.h"

//==============================================================================================================
//================================================================================================================
CSocketWrapper::CSocketWrapper(int tcpudp)
{
	socket_Init();

	memset(&m_stAddr, 0, sizeof(sockaddr_in));
	m_tcpudp = tcpudp;
	m_hSocket = INVALIDSOCKHANDLE;
	Reopen(false);
}


CSocketWrapper::~CSocketWrapper()
{
	Close();
	socket_Uninit();
}
void CSocketWrapper::Reopen(bool bForceClose)
{

	if (socket_Isvalid(m_hSocket) && bForceClose)
	{
		socket_Close(m_hSocket);
	}
	if (!socket_Isvalid(m_hSocket))
	{
		m_hSocket = socket_Open(m_tcpudp);
	}
}

void CSocketWrapper::Close()
{
	socket_Close(m_hSocket);
	m_hSocket = INVALIDSOCKHANDLE;
}

int CSocketWrapper::SetTimeOut(TimeOutType type, int iTimeout)   //in milliseconds
{
	return socket_SetTimeOut(m_hSocket, type, iTimeout);
}

int CSocketWrapper::SetBlock(bool bblock)
{
	return socket_SetBlock(m_hSocket, bblock);
}

int CSocketWrapper::Connnect(const char *ip, int port)
{
	socket_GetAddr_in(ip, port, &m_stAddr);
	return socket_Connect(m_hSocket, &m_stAddr);
}

TransResult CSocketWrapper::Send(const char *ptr, int nbytes)
{
	return socket_Send(m_hSocket, ptr, nbytes);
}
TransResult CSocketWrapper::Recv(char *ptr, int nbytes)
{
	return socket_Recv(m_hSocket, ptr, nbytes);
}

