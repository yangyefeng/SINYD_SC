/////////////////////////////////////////////////////////////////////////////
/// @File name: SYDDataTransferImpl.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	传输模块实现类
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
	/// @brief:  初始化传输模块
	///*******************************************************
	void Init(const SourceInfo& info);
	///*******************************************************
	/// @name:   CSYDDataTransferImpl::InitEx
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][const SourceInfo2 &]info
	/// @param:  [in][TransferFormatType]type
	/// @brief:  初始化传输模块
	///*******************************************************
	void Init2(const SourceInfo2& info);
	///*******************************************************
	/// @name:   CSYDDataTransferImpl::Start
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][const char *]ip
	/// @param:  [in][int]port
	/// @brief:  启动传输模块的传输
	///*******************************************************
	void Start(const char* ip, int port);
	///*******************************************************
	/// @name:   CSYDDataTransferImpl::Stop
	/// @author: YaoDi
	/// @return: void
	/// @brief:  停止传输模块的传输
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
