/////////////////////////////////////////////////////////////////////////////
/// @File name: SYDDataTransfer.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	数据传输模块接口类
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef _SYD_Data_Transfer_h_
#define _SYD_Data_Transfer_h_

#ifdef _WIN32
#ifdef SC_DATA_TRANSFER_DLL_EXPORT
#define SC_DATA_TRANSFER __declspec(dllexport)
#else 
#define SC_DATA_TRANSFER __declspec(dllimport)
#endif
#else
#define SC_DATA_TRANSFER
#endif

#include "SYDDataTransferDef.h"

class CSYDDataTransferImpl;
class SC_DATA_TRANSFER CSYDDataTransfer
{
public:
	CSYDDataTransfer();
	virtual ~CSYDDataTransfer();

	///*******************************************************
	/// @name:   CSYDDataTransfer::Init
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][const SourceInfo &]info
	/// @brief:  初始化传输模块
	///*******************************************************
	void Init(const SourceInfo& info);	//拌和机ID、采集来源、拌和机类型
	///*******************************************************
	/// @name:   CSYDDataTransfer::InitEx
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][const SourceInfo2 &]info
	/// @param:  [in][TransferFormatType]type
	/// @brief:  初始化传输模块
	///*******************************************************
	void Init2(const SourceInfo2& info);
	///*******************************************************
	/// @name:   CSYDDataTransfer::Start
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][const char *]ip
	/// @param:  [in][int]port
	/// @brief:  启动传输模块的传输
	///*******************************************************
	void Start(const char* ip, int port);
	///*******************************************************
	/// @name:   CSYDDataTransfer::Stop
	/// @author: YaoDi
	/// @return: void
	/// @brief:  停止传输模块的传输
	///*******************************************************
	void Stop();

private:
	CSYDDataTransferImpl* m_pImpl;

};

#endif	//_SYD_Data_Transfer_h_

