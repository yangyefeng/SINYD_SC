#include "ReturnCodeParser.h"
#include "SocketWrapper.h"
#include "json/json.h"
#include "ProtocolDef.h"
#include "ThreadTemplateCrossSystem/Thread.h"


#if _WIN32
#include "SCOCR.h"
#include "JsonRW.h"

using namespace Json;

struct DigitThreadParam
{
	string m_strIP;
	int m_iPort;
	list<int> m_listDigitID;
};

void ThreadDigitSocket(DigitThreadParam& param)
{
	CSCDigitOCR ocr;
	list<DIGITMODEL> digits;
	ocr.GetDigitModelbyID(param.m_listDigitID, digits);
	if (digits.size() > 0)
	{
		CSocketWrapper sock;
		sock.SetBlock(true);
		sock.SetTimeOut(TimeOut_Send, 1000);
		sock.SetTimeOut(TimeOut_Recv, 1000);
		if (sock.Connnect(param.m_strIP.c_str(), param.m_iPort) == 0)
		{
			CJsonRW jsonRW;
			int contentIndex = jsonRW.CreateContent(2);	//2 = 配置信息类型
			int contentDataIndex = jsonRW.CreateContentDataIntoContent(contentIndex, 3);	 //3 = 字模配置信息

			list<DIGITMODEL>::iterator itEnd = digits.end();
			for (list<DIGITMODEL>::iterator it = digits.begin(); it != itEnd; ++it)
			{
				int dataIndex = jsonRW.CreateDataIntoContentData(contentDataIndex);
				jsonRW.AddDataInfoIntoData(dataIndex, "id", it->id);
				jsonRW.AddDataInfoIntoData(dataIndex, "width", it->width);
				jsonRW.AddDataInfoIntoData(dataIndex, "height", it->height);
				jsonRW.AddDataInfoIntoData(dataIndex, "matrix", it->m);
			}

			int RetryCount = 0;
			TransResult tr = { 0 };
			const string* data = jsonRW.GetFinal();
			do
			{
				tr = sock.Send(data->c_str() + tr.nbytes, data->length() - tr.nbytes);
				++RetryCount;
			} while (tr.nresult == 1 && RetryCount <= 3);

			sock.Close();
		}
	}
	ocr.ClearDigitModelResList(digits);
}

#endif
#include "log.h"

CReturnCodeExecutor::CReturnCodeExecutor()
	: m_code_id(0)
	, m_strIP()
	, m_iPort(0)
{

}


CReturnCodeExecutor::~CReturnCodeExecutor()
{
}

int CReturnCodeExecutor::Execute(const string& doc)
{
	m_code_id = 0;
	if (!doc.empty())
	{
		try
		{
			Json::Value root;
			Json::Reader reader;
			if (reader.parse(doc, root))
			{
				m_code_id = root.get(CODE_ID, 0).asInt();
//#if _WIN32
#if 0		//取消上传字模的功能
				if (!root[NUM_ID].isNull())
				{
				 	int size = root[NUM_ID].size();  // 得到"files"的数组个数  
					DigitThreadParam param;
					param.m_strIP = m_strIP;
					param.m_iPort = m_iPort;
				 	for (int i = 0; i < size; ++i)  // 遍历数组  
				 	{
						param.m_listDigitID.push_back(root[NUM_ID][i].asInt());
				 	}

					Thread<DigitThreadParam>::Create((Thread<DigitThreadParam>::Handler)ThreadDigitSocket, param, 0, true);
				}
#endif
			}
		}
		catch (...)
		{
			m_code_id = 999;
		}
	}

	return m_code_id;
}

void CReturnCodeExecutor::SetIP(const string & ip, int port)
{
	m_strIP = ip;
	m_iPort = port;
}

CReturnCodeExecutor_Flume::CReturnCodeExecutor_Flume()
	: m_code_id(0)
{

}

CReturnCodeExecutor_Flume::~CReturnCodeExecutor_Flume()
{

}

int CReturnCodeExecutor_Flume::Execute(const string& doc)
{
	m_code_id = 0;
	if (doc.substr(0, 2) == "OK")
	{
		m_code_id = 100;
	}
	else
	{
		SCERROR("Trans::ReturnCodeExecute--Code %s\n", doc.c_str());
	}

	return m_code_id;
}

void CReturnCodeExecutor_Flume::SetIP(const string& ip, int port)
{
	return;
}
