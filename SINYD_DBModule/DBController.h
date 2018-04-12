/////////////////////////////////////////////////////////////////////////////
/// @File name: DBController.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	���ݴ洢ʵ�ʿ�����
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
	/// @brief:  �������ݴ洢�ļ�
	///*******************************************************
	virtual int CreateDBFile(const string& fileName, bool isNameWithPath);
	///*******************************************************
	/// @name:   CDBController::CreateTable
	/// @author: YaoDi
	/// @return: int
	/// @param:  [int][const DBInfo &]tconfig
	/// @param:  [in][IDBTable * *]ppDBTable
	/// @brief:  �������ݴ洢��
	///*******************************************************
	virtual int CreateTable(const DBInfo& tconfig, IDBTable** ppDBTable);
	///*******************************************************
	/// @name:   CDBController::GetTable
	/// @author: YaoDi
	/// @return: IDBTable*
	/// @brief:  ��ȡ���ݴ洢��
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
