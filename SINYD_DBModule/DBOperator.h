/////////////////////////////////////////////////////////////////////////////
/// @File name: DBOperator.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	数据存储操作类
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#if !defined(_DB_OPERATOR_H)
#define _DB_OPERATOR_H

#include "DBModule.h"
#include "DBModulePrivate.h"

class CDBQueryRow : public IDBQueryRow
{
public:
	CDBQueryRow(IDBTableQueryRow* pDBTQ);
	virtual ~CDBQueryRow();

	virtual int numDataFields();
	virtual bool eof();
	virtual void nextRow();
	virtual int RowPK();
	virtual const char* DataFieldName(int nField);
	virtual bool DataFieldIsNull(int nField);
	virtual const char * GetStringDataField(int nField, const char* szNullValue = "");

protected:
	IDBTableQueryRow* m_pDBTableQuery;
};

class CDBOperator : public IDBOperator
{
public:
	CDBOperator(const DBInfo& dbInfo);
	CDBOperator(const string& dbPath, bool isNameWithPath);
	virtual ~CDBOperator();

	virtual int Insert(const map<string, string>& datas);
	virtual void Update(int rowid, const map<string, string>& datas);
	virtual IDBQueryRow* Query(int startRowID, int endRowID);
	virtual void QueryFinalize(IDBQueryRow* queryRows);
	virtual const DBInfo& GetDBInfo();

protected:
	DBInfo m_dbInfo;
	IDBController* m_pDBCtrl;
	IDBTable* m_pDBTable;
};

#endif	//_DB_OPERATOR_H

