/////////////////////////////////////////////////////////////////////////////
/// @File name: DBHandleManager.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	数据存储文件句柄管理，保证同一文件只打开一次给多个线程共享使用
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "ThreadTemplateCrossSystem/Singleton.h"
#include "CppSqlite3/CppSQLite3.h"
#include <string>
#include <map>
using namespace std;

class CDBHandleManager
{
	DECLARE_SINGLETON_CLASS(CDBHandleManager);
private:
	CDBHandleManager(const CDBHandleManager&);
	CDBHandleManager& operator=(const CDBHandleManager&);

	CDBHandleManager();
	virtual ~CDBHandleManager();

public:
	///*******************************************************
	/// @name:   CDBHandleManager::CreateDB
	/// @author: YaoDi
	/// @return: CppSQLite3DB*
	/// @param:  [int][const string &]path
	/// @brief:  创建数据文件句柄
	///*******************************************************
	CppSQLite3DB* CreateDB(const string& path);
	///*******************************************************
	/// @name:   CDBHandleManager::ReleaseDB
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][const string &]path
	/// @brief:  释放数据文件句柄
	///*******************************************************
	void ReleaseDB(const string& path);

private:
	struct DBHandle
	{
		CppSQLite3DB* pDB;
		int count;
	};
	map<string, DBHandle> m_handles;
	Mutex m_csHandles;
};

typedef CSingleton<CDBHandleManager> DBHanleMgrInstance;

