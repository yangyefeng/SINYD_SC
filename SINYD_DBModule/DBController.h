/////////////////////////////////////////////////////////////////////////////
/// @File name: DBController.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	数据存储实际控制类
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#if !defined(_CDBCONTROLLER_H)
#define _CDBCONTROLLER_H

#include "DBModulePrivate.h"
#include "CppSqlite3/CppSQLite3.h"

class CDBController : public IDBController
{
public:
	CDBController();
	virtual ~CDBController();

	///*******************************************************
	/// @name:   CDBController::CreateDBFile
	/// @author: YaoDi
	/// @return: int
	/// @param:  [int][const string &]fileName
	/// @param:  [in][bool]isNameWithPath
	/// @brief:  创建数据存储文件
	///*******************************************************
	virtual int CreateDBFile(const string& fileName, bool isNameWithPath);
	///*******************************************************
	/// @name:   CDBController::CreateTable
	/// @author: YaoDi
	/// @return: int
	/// @param:  [int][const DBInfo &]tconfig
	/// @param:  [in][IDBTable * *]ppDBTable
	/// @brief:  创建数据存储表
	///*******************************************************
	virtual int CreateTable(const DBInfo& tconfig, IDBTable** ppDBTable);
	///*******************************************************
	/// @name:   CDBController::GetTable
	/// @author: YaoDi
	/// @return: IDBTable*
	/// @brief:  获取数据存储表
	///*******************************************************
	virtual IDBTable* GetTable();


protected:
	void ClearSelf();

protected:
	IDBTable* m_pIDBTable;
	CppSQLite3DB* m_pSqlite3DB;
	string m_dbPath;
};

#endif  //_CDBCONTROLLER_H
