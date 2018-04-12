/////////////////////////////////////////////////////////////////////////////
/// @File name: SYDDataTransfer.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	���ݴ���ģ��ӿ���
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
	/// @brief:  ��ʼ������ģ��
	///*******************************************************
	void Init(const SourceInfo& info);	//��ͻ�ID���ɼ���Դ����ͻ�����
	///*******************************************************
	/// @name:   CSYDDataTransfer::InitEx
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][const SourceInfo2 &]info
	/// @param:  [in][TransferFormatType]type
	/// @brief:  ��ʼ������ģ��
	///*******************************************************
	void Init2(const SourceInfo2& info);
	///*******************************************************
	/// @name:   CSYDDataTransfer::Start
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][const char *]ip
	/// @param:  [in][int]port
	/// @brief:  ��������ģ��Ĵ���
	///*******************************************************
	void Start(const char* ip, int port);
	///*******************************************************
	/// @name:   CSYDDataTransfer::Stop
	/// @author: YaoDi
	/// @return: void
	/// @brief:  ֹͣ����ģ��Ĵ���
	///*******************************************************
	void Stop();

private:
	CSYDDataTransferImpl* m_pImpl;

};

#endif	//_SYD_Data_Transfer_h_

