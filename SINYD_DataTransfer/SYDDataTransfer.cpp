#include "SYDDataTransferImpl.h"
#include "SYDDataTransfer.h"



CSYDDataTransfer::CSYDDataTransfer()
{
	m_pImpl = new CSYDDataTransferImpl();
}


CSYDDataTransfer::~CSYDDataTransfer()
{
	if (m_pImpl)
	{
		delete m_pImpl;
		m_pImpl = NULL;
	}
}

void CSYDDataTransfer::Init(const SourceInfo & info)
{
	if (m_pImpl)
	{
		m_pImpl->Init(info);
	}
}

void CSYDDataTransfer::Init2(const SourceInfo2 & info)
{
	if (m_pImpl)
	{
		m_pImpl->Init2(info);
	}
}

void CSYDDataTransfer::Start(const char* ip, int port)
{
	if (m_pImpl)
	{
		m_pImpl->Start(ip, port);
	}
}

void CSYDDataTransfer::Stop()
{
	if (m_pImpl)
	{
		m_pImpl->Stop();
	}
}
