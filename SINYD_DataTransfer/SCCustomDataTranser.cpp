#include "SCCustomDataTranser.h"
#include "SocketWrapper.h"
#include "JsonRW.h"

using namespace std;

CSCCustomDataTranser::CSCCustomDataTranser(const string& ip, int port)
	: m_ip(ip)
	, m_port(port)
{
}


CSCCustomDataTranser::~CSCCustomDataTranser()
{
}

void CSCCustomDataTranser::Init(const SourceInfo & info)
{
	CJsonRW::initSourceInfo(info);
}

bool CSCCustomDataTranser::TransData(int contentType, int dataType, const map<string, string>& data)
{
	bool bRes = false;

	CSocketWrapper sock;
	if (sock.Connnect(m_ip.c_str(), m_port) == 0)
	{
		CJsonRW jsonRW;
		int contentIndex = jsonRW.CreateContent(contentType);	
		int contentDataIndex = jsonRW.CreateContentDataIntoContent(contentIndex, dataType);
		int dataIndex = jsonRW.CreateDataIntoContentData(contentDataIndex);

		map<string, string>::const_iterator itEnd = data.end();
		for (map<string, string>::const_iterator it = data.begin(); it != itEnd; ++it)
		{
			jsonRW.AddDataInfoIntoData(dataIndex, it->first, it->second);
		}

		int RetryCount = 0;
		TransResult tr = { 0 };
		const string* data = jsonRW.GetFinal();
		do
		{
			tr = sock.Send(data->c_str() + tr.nbytes, data->length() - tr.nbytes);
			++RetryCount;
		} while (tr.nresult == 1 && RetryCount <= 3);

		if (tr.nresult == 0)
		{
			bRes = true;
		}

		sock.Close();
	}
	return bRes;
}

// bool CSCCustomDataTranser::TransData(int contentType, int dataType, const list<map<string, string>>& data)
// {
// 	return false;
// }
