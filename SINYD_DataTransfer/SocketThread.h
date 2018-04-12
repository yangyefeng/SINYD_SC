/////////////////////////////////////////////////////////////////////////////
/// @File name: SocketThread.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	传输模块主线程及传输模块主线程工作类
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef _Socket_Thread_h_
#define _Socket_Thread_h_

#include "SocketWrapper.h"
#include "ThreadTemplateCrossSystem/Thread.h"
#include "TransDataReader.h"
#include "ReturnCodeParser.h"
#include "TRWData.h"
#include "MeasureDef.h"
#include <typeinfo>
#include "log.h"
#if _WIN32
#include <memory>
#else
#include <memory>
#include <tr1/memory>
#endif
#include "TransConfig.h"

using namespace std;

class ISocketWorker
{
public:
	virtual ~ISocketWorker(){}
	///*******************************************************
	/// @name:   ISocketWorker::setIP
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][const char *]ip
	/// @param:  [in][int]port
	/// @brief:  设定传输服务器地址
	///*******************************************************
	virtual void setIP(const char* ip, int port) = 0;
	///*******************************************************
	/// @name:   ISocketWorker::preWorker
	/// @author: YaoDi
	/// @return: bool
	/// @param:  [in][const bool &]bStopForCheck
	/// @brief:  传输执行前准备处理
	///*******************************************************
	virtual bool preWorker(const bool& bStopForCheck) = 0;
	///*******************************************************
	/// @name:   ISocketWorker::runWorker
	/// @author: YaoDi
	/// @return: bool
	/// @param:  [in][const bool &]bStopForCheck
	/// @brief:  传输任务执行
	///*******************************************************
	virtual bool runWorker(const bool& bStopForCheck) = 0;
	///*******************************************************
	/// @name:   ISocketWorker::postWorker
	/// @author: YaoDi
	/// @return: bool
	/// @param:  [in][const bool &]bStopForCheck
	/// @brief:  传输执行完后续处理
	///*******************************************************
	virtual bool postWorker(const bool& bStopForCheck) = 0;
};


//template<class TDataReader>
class CSocketWorker : public ISocketWorker
{
public:
    CSocketWorker(std::tr1::shared_ptr<ITransDataReader> sp_reader, std::tr1::shared_ptr<IReturnCodeExecutor> sp_ret_exec)
		//: m_dataReader()		//delete:template class change to construct param
		: m_sp_dataReader(sp_reader)
		, m_bIsConnected(false)
		, m_curData()
		, m_curSendCode(0)
		, m_iniCS()
		, m_sp_returnCodeExc(sp_ret_exec)
	{
		m_sock.SetBlock(true);	//����Ϊ����ģʽ
		m_sock.SetTimeOut(TimeOut_Send, CTransConfig::GetSendTimeout(1000));	//���÷��ͳ�ʱʱ������λΪ����
		m_sock.SetTimeOut(TimeOut_Recv, CTransConfig::GetRecvTimeout(1000));	//���ý��ճ�ʱʱ������λΪ����
	}

	virtual ~CSocketWorker()
	{
		Disconnect();
	}

	virtual void setIP(const char* ip, int port)
	{
		m_ip = ip;
		m_port = port;
		//m_returnCodeExc.SetIP(m_ip, m_port);
		m_sp_returnCodeExc->SetIP(m_ip, m_port);
	}

	virtual bool preWorker(const bool& bStopForCheck)
	{
		bool bRes = Connect();
		if (!bRes)
		{
			SCERROR("Trans::%s--Connect %s:%d failed\n", typeid(*m_sp_dataReader).name(), m_ip.c_str(), m_port);
		}
		if (bStopForCheck)
		{
			return false;
		}
		if (bRes)
		{
			try
			{
				//m_curData.pData = m_dataReader.getData();
				m_curData.pData = m_sp_dataReader->getData();
			}
			catch (CDBModuleException& e)
			{
				//need to write log
				SCERROR("Trans::%s--getData failed errorid = %d, errLog = %s\n", typeid(*m_sp_dataReader).name(), e.errorCode(), e.errorCodeAsString(e.errorCode()));
				bRes = false;
			}
			catch (...)
			{
				SCERROR("Trans::%s--getData failed\n", typeid(*m_sp_dataReader).name());
				bRes = false;
			}
			
		}

		return bRes;
	}
	virtual bool runWorker(const bool& bStopForCheck)
	{
		bool bRes = false;
		if (m_curData.pData != NULL && m_curData.pData->length() > 0)
		{
			int RetryCount = 0;
			TransResult tr = { 0 };
			do
			{
				tr = m_sock.Send(m_curData.pData->c_str() + tr.nbytes, m_curData.pData->length() - tr.nbytes);
				++RetryCount;

				if (bStopForCheck)
				{
					break;
				}
			} while (tr.nresult == 1 && RetryCount <= 3);
			if (tr.nresult == 0)
			{
				bRes = true;
			}
			else
			{
				SCERROR("Trans::%s--Send %s:%d failed\n", typeid(*m_sp_dataReader).name(), m_ip.c_str(), m_port);
				Disconnect();
			}

			if (bRes)
			{
				bRes = false;

				const int BufLen = 10240;
				string strBuf;
				bool bNeedGet = true;
				char recvBuf[BufLen] = { 0 };
				RetryCount = 0;
				do
				{
					tr = m_sock.Recv(recvBuf, BufLen);
					++RetryCount;

					if (tr.nbytes > 0)
					{
						strBuf = strBuf + recvBuf;
						memset(recvBuf, 0, 10240);
					}
					if (tr.nbytes > 0 && ( tr.nbytes < 10240 || *strBuf.rbegin() == '}' ) )
					{
						bNeedGet = false;
					}

					if (bStopForCheck)
					{
						break;
					}
				} while ( (bNeedGet || tr.nresult == 1) && RetryCount <= 3);
				if (tr.nresult == 0)
				{
					bRes = true;
					//m_curSendCode = m_returnCodeExc.Execute(strBuf);
					m_curSendCode = m_sp_returnCodeExc->Execute(strBuf);
				}
				else
				{
					SCERROR("Trans::%s--Recv %s:%d failed\n", typeid(*m_sp_dataReader).name(), m_ip.c_str(), m_port);
					Disconnect();
				}
			}
			
		}
		return bRes;
	}
	virtual bool postWorker(const bool& bStopForCheck)
	{
		bool bRes = false;

		if (m_curSendCode == 100 || m_curSendCode == 101)
		{
			try
			{
				//m_dataReader.next();
				m_sp_dataReader->next();
				bRes = true;
			}
			catch (CDBModuleException& e)
			{
				//need to write log
				SCERROR("Trans::%s--next failed errorid = %d, errLog = %s\n", typeid(*m_sp_dataReader).name(), e.errorCode(), e.errorCodeAsString(e.errorCode()));
				bRes = false;
			}
			catch (...)
			{
				SCERROR("Trans::%s--next failed\n", typeid(*m_sp_dataReader).name());
				bRes = false;
			}
		}
		else
		{
			SCERROR("Trans::%s--Recv return code %d\n", typeid(*m_sp_dataReader).name(), m_curSendCode);
		}

		return bRes;
	}

private:
	bool Connect()
	{
		if (!m_bIsConnected)
		{
/*#if !defined(_WIN32) */
			CTRWData rw;
			{
				m_iniCS.Lock();
				rw._CreateFile(CONFIG_NET_LOG_PATH "SocketState.ini");
				rw._WriteProfileString("Socket", "connect_state", "0", CONFIG_NET_LOG_PATH "SocketState.ini");
				m_iniCS.Unlock();
			}
/*#endif // !(_WIN32)*/
			m_sock.Reopen(true);
			if (m_sock.Connnect(m_ip.c_str(), m_port) == 0)
			{
				m_bIsConnected = true;
/*#if !defined(_WIN32) */
				{
					m_iniCS.Lock();
					rw._WriteProfileString("Socket", "connect_state", "1", CONFIG_NET_LOG_PATH "SocketState.ini");
					m_iniCS.Unlock();
				}
/*#endif // !(_WIN32)*/
			}
		}

		return m_bIsConnected;
	}
	void Disconnect()
	{
		if (m_bIsConnected)
		{
			m_sock.Close();
			m_bIsConnected = false;
/*#if !defined(_WIN32) */
		{
				CTRWData rw;
				m_iniCS.Lock();
				rw._CreateFile(CONFIG_NET_LOG_PATH "SocketState.ini");
				rw._WriteProfileString("Socket", "connect_state", "0", CONFIG_NET_LOG_PATH "SocketState.ini");
				m_iniCS.Unlock();
		}
/*#endif // !(_WIN32)*/

		}
	}

private:
	//TDataReader m_dataReader;			//delete:template class change to construct param
    std::tr1::shared_ptr<ITransDataReader> m_sp_dataReader;
	CSocketWrapper m_sock;
	bool m_bIsConnected;
	string m_ip;
	int m_port;
	struct SendData
	{
		SendData()
		{
			clear();
		}
		void clear()
		{
			pData = NULL;
		}
		const string* pData;
	};
	SendData m_curData;
	int m_curSendCode;
	Mutex m_iniCS;
	//CReturnCodeExecutor m_returnCodeExc;
    std::tr1::shared_ptr<IReturnCodeExecutor> m_sp_returnCodeExc;
};

//template<class TWorker>
class CSocketThread : private Thread<void>
{
public:
    CSocketThread(std::tr1::shared_ptr<ISocketWorker> sp_worker)
		: m_bStop(false)
		, m_TH(InvalidHandle)
		, m_sp_worker(sp_worker)			//add:template class change to construct param
	{
	}
	virtual ~CSocketThread()
	{
	}

	virtual void Start(const char* ip, int port)
	{
		//change:template class change to construct param begin
		if (m_sp_worker)
		{
			m_sp_worker->setIP(ip, port);
			if (m_TH == InvalidHandle)
			{
				Create(&m_TH);
			}
		}
// 		m_worker.setIP(ip, port);
// 		if (m_TH == InvalidHandle)
// 		{
// 			Create(&m_TH);
// 		}
		//change:template class change to construct param end
	}
	virtual void Stop()
	{
		m_bStop = true;
		if (m_TH != InvalidHandle)
		{
			Join(m_TH);
			m_TH = InvalidHandle;
		}
	}

protected:
	virtual void ThreadMain(void)
	{
		while (1)
		{
			//bool bContinue = m_worker.preWorker(m_bStop);	//delete:template class change to construct param
			bool bContinue = m_sp_worker->preWorker(m_bStop);

			//check for thread stop
			if (m_bStop)
			{
				break;
			}

			if (bContinue)
			{
				//bContinue = m_worker.runWorker(m_bStop);	//delete:template class change to construct param
				bContinue = m_sp_worker->runWorker(m_bStop);
			}

			if (bContinue)
			{
				//bContinue = m_worker.postWorker(m_bStop);	//delete:template class change to construct param
				bContinue = m_sp_worker->postWorker(m_bStop);
			}

			if (bContinue)
			{
				continue;
			} 
			//��workerִ�з�������ʱ���̵߳ȴ�3���ٽ�����һ��ִ��
			else
			{
				SLEEP(3000);
			}

			//check for thread stop
			if (m_bStop)
			{
				break;
			}

		}
	}

protected:
	bool m_bStop;
	Thread<void>::Handle m_TH;
	//TWorker m_worker;			//delete:template class change to construct param
    std::tr1::shared_ptr<ISocketWorker> m_sp_worker;
};




#endif	//_Socket_Thread_h_
