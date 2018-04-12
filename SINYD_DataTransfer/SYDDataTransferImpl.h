/////////////////////////////////////////////////////////////////////////////
/// @File name: SYDDataTransferImpl.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	����ģ��ʵ����
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef _SYD_Data_Transfer_Impl_h_
#define _SYD_Data_Transfer_Impl_h_


#include "SocketThread.h"
#include "TransDataReader.h"
#include "SYDDataTransferDef.h"

class CSYDDataTransferImpl
{
public:
	CSYDDataTransferImpl();
	virtual ~CSYDDataTransferImpl();

	///*******************************************************
	/// @name:   CSYDDataTransferImpl::Init
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][const SourceInfo &]info
	/// @brief:  ��ʼ������ģ��
	///*******************************************************
	void Init(const SourceInfo& info);
	///*******************************************************
	/// @name:   CSYDDataTransferImpl::InitEx
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][const SourceInfo2 &]info
	/// @param:  [in][TransferFormatType]type
	/// @brief:  ��ʼ������ģ��
	///*******************************************************
	void Init2(const SourceInfo2& info);
	///*******************************************************
	/// @name:   CSYDDataTransferImpl::Start
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][const char *]ip
	/// @param:  [in][int]port
	/// @brief:  ��������ģ��Ĵ���
	///*******************************************************
	void Start(const char* ip, int port);
	///*******************************************************
	/// @name:   CSYDDataTransferImpl::Stop
	/// @author: YaoDi
	/// @return: void
	/// @brief:  ֹͣ����ģ��Ĵ���
	///*******************************************************
	void Stop();

private:
//     typedef CSocketThread<CSocketWorker<CRawDataReader> > Raw_Thread;
//     typedef CSocketThread<CSocketWorker<CMeasureDataReader> > Measure_Thread;
//     typedef CSocketThread<CSocketWorker<CDetailDataReader> > Detail_Thread;

// 	Raw_Thread m_rawThread;
// 	Measure_Thread m_measureThread;
// 	Detail_Thread m_detailThread;

    std::tr1::shared_ptr<CSocketThread> m_rawThread;
    std::tr1::shared_ptr<CSocketThread> m_measureThread;
    std::tr1::shared_ptr<CSocketThread> m_detailThread;
};


#endif	//_SYD_Data_Transfer_Impl_h_
