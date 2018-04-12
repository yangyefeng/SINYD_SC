/////////////////////////////////////////////////////////////////////////////
/// @File name: DBTable.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	���ݴ洢�����
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#if !defined(_CDBTABLE_H)
#define _CDBTABLE_H

#include "DBModulePrivate.h"
#include "CppSqlite3/CppSQLite3.h"

class CDBTable : public IDBTable
{
public:
	CDBTable(CppSQLite3DB* pSqliteDB);
	virtual ~CDBTable();

	///*******************************************************
	/// @name:   CDBTable::Create
	/// @author: YaoDi
	/// @return: int
	/// @param:  [in][const string &]name
	/// @param:  [in][const DBInfo &]info
	/// @brief:  ������
	///*******************************************************
	virtual int Create(const string& name, const DBInfo& info);
	///*******************************************************
	/// @name:   CDBTable::Open
	/// @author: YaoDi
	/// @return: int
	/// @param:  [in][const string &]name
	/// @brief:  ���Ѵ����ı�
	///*******************************************************
	virtual int Open(const string& name);
	///*******************************************************
	/// @name:   CDBTable::Insert
	/// @author: YaoDi
	/// @return: int
	/// @param:  [in][const]map<string
	/// @param:  [in][string> &]datas
	/// @param:  [out][int *]rowid
	/// @brief:  ����в������ݣ��������ز����е�����ID
	///*******************************************************
	virtual int Insert(const map<string, string>& datas, int* rowid);
	virtual int Update(int rowid, const map<string, string>& datas);
	///*******************************************************
	/// @name:   CDBTable::Query
	/// @author: YaoDi
	/// @return: int
	/// @param:  [in][int]startRowID
	/// @param:  [in][int]endRowID
	/// @param:  [out][IDBTableQueryRow * *]queryRows
	/// @brief:  �������е����ݣ��������ؼ��������
	///*******************************************************
	virtual int Query(int startRowID, int endRowID, IDBTableQueryRow** queryRows);
	///*******************************************************
	/// @name:   CDBTable::QueryFinalize
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][IDBTableQueryRow *]queryRows
	/// @brief:  �ͷż��������
	///*******************************************************
	virtual void QueryFinalize(IDBTableQueryRow* queryRows);
	///*******************************************************
	/// @name:   CDBTable::getTableName
	/// @author: YaoDi
	/// @return: const string&
	/// @brief:  ���ر���
	///*******************************************************
	virtual const string& getTableName();
	///*******************************************************
	/// @name:   CDBTable::RenameTable
	/// @author: YaoDi
	/// @return: int
	/// @param:  [in][const string &]orgTableName
	/// @param:  [in][const string &]newTableName
	/// @brief:  ����������
	///*******************************************************
	virtual int RenameTable(const string& orgTableName, const string& newTableName);
	///*******************************************************
	/// @name:   CDBTable::GetTableInfo
	/// @author: YaoDi
	/// @return: const DBInfo&
	/// @brief:  ���ر���Ϣ
	///*******************************************************
	virtual const DBInfo& GetTableInfo();

protected:
	void CreateSTMT();
	void CreateTable();

protected:
	CppSQLite3Statement m_InsertStmt;
	CppSQLite3Statement m_QueryStmt;
	CppSQLite3DB* m_sqlite3DB;
	int m_ColumnCount;
	string m_tableName;
	map<int, string> m_columnIndexsMap;
	DBInfo m_TableInfo;
};

#endif  //_CDBTABLE_H
