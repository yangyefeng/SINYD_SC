/////////////////////////////////////////////////////////////////////////////
/// @File name: ReturnCodeParser.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	���䷵�����������������ִ��
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef _Return_Code_Parser_h_
#define _Return_Code_Parser_h_

#include <string>
#include <list>


using namespace std;

class IReturnCodeExecutor
{
public:
	virtual ~IReturnCodeExecutor() {}

	virtual int Execute(const string& doc) = 0;
	virtual void SetIP(const string& ip, int port) = 0;
};

class CReturnCodeExecutor : public IReturnCodeExecutor
{
public:
	CReturnCodeExecutor();
	virtual ~CReturnCodeExecutor();

	///*******************************************************
	/// @name:   CReturnCodeExecute::code_id
	/// @author: YaoDi
	/// @return: int
	/// @brief:  �������code_idֵ
	///*******************************************************
	virtual int Execute(const string& doc);

	virtual void SetIP(const string& ip, int port);
private:
	int m_code_id;
	string m_strIP;
	int m_iPort;
};

class CReturnCodeExecutor_Flume : public IReturnCodeExecutor
{
public:
	CReturnCodeExecutor_Flume();
	virtual ~CReturnCodeExecutor_Flume();

	virtual int Execute(const string& doc);

	virtual void SetIP(const string& ip, int port);

private:
	int m_code_id;
};

#endif	//_Return_Code_Parser_h_

