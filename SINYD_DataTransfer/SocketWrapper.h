/////////////////////////////////////////////////////////////////////////////
/// @File name: SocketWrapper.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	跨平台Socket传输封装类
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef _SOCK_WRAPPER_H_
#define _SOCK_WRAPPER_H_

#include "SocketAPI.h"

class CSocketWrapper
{
public:
	CSocketWrapper(int tcpudp = SOCK_STREAM);
	~CSocketWrapper();
	///*******************************************************
	/// @name:   CSocketWrapper::SetTimeOut
	/// @author: YaoDi
	/// @return: int
	/// @param:  [in][TimeOutType]type
	/// @param:  [in][int]iTimeout
	/// @brief:  设定Socket超时属性
	///*******************************************************
	int SetTimeOut(TimeOutType type, int iTimeout);
	///*******************************************************
	/// @name:   CSocketWrapper::SetBlock
	/// @author: YaoDi
	/// @return: int
	/// @param:  [in][bool]bblock
	/// @brief:  设定Socket阻塞模式
	///*******************************************************
	int SetBlock(bool bblock);

	///*******************************************************
	/// @name:   CSocketWrapper::Reopen
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][bool]bForceClose
	/// @brief:  打开及重新打开Socket对象
	///*******************************************************
	void Reopen(bool bForceClose);
	///*******************************************************
	/// @name:   CSocketWrapper::Close
	/// @author: YaoDi
	/// @return: void
	/// @brief:  关闭Socket对象
	///*******************************************************
	void Close();
	///*******************************************************
	/// @name:   CSocketWrapper::Connnect
	/// @author: YaoDi
	/// @return: int
	/// @param:  [in][const char *]ip
	/// @param:  [in][int]port
	/// @brief:  连接Socket服务器
	///*******************************************************
	int Connnect(const char *ip, int port);
	///*******************************************************
	/// @name:   CSocketWrapper::Send
	/// @author: YaoDi
	/// @return: TransResult
	/// @param:  [in][const char *]ptr
	/// @param:  [in][int]nbytes
	/// @brief:  发送数据
	///*******************************************************
	TransResult Send(const char *ptr, int nbytes);
	///*******************************************************
	/// @name:   CSocketWrapper::Recv
	/// @author: YaoDi
	/// @return: TransResult
	/// @param:  [in][char *]ptr
	/// @param:  [in][int]nbytes
	/// @brief:  接收数据
	///*******************************************************
	TransResult Recv(char *ptr, int nbytes);

protected:
	HSocket  m_hSocket;
	sockaddr_in m_stAddr;
	int m_tcpudp;
};


#endif	//_SOCK_WRAPPER_H_

