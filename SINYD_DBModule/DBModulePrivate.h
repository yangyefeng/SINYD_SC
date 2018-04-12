/////////////////////////////////////////////////////////////////////////////
/// @File name: DBModulePrivate.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	数据存储模块内部接口定义
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#if !defined(_DB_MODULE_PRIVATE_H)
#define _DB_MODULE_PRIVATE_H

#include <string>
#include <list>
#include "DBModule.h"
#include <map>

using namespace std;

class IDBTableQueryRow
{
public:
	virtual ~IDBTableQueryRow(){}
	virtual int numDataFields() = 0;
	virtual bool eof() = 0;
	virtual int nextRow() = 0;
	virtual int RowPK(int& pk) = 0;
	virtual int DataFieldName(int nField, const char*& fieldName) = 0;
	virtual int DataFieldIsNull(int nField, bool& pIsNull) = 0;
	virtual int GetStringDataField(int nField, const char*& value, const char* szNullValue = "") = 0;
};

class IDBTable
{
public:
	virtual ~IDBTable(){}
	virtual int Insert(const map<string, string>& datas, int* rowid) = 0;
	virtual int Update(int rowid, const map<string, string>& datas) = 0;
	virtual int Query(int startRowID, int endRowID, IDBTableQueryRow** queryRows) = 0;
	virtual void QueryFinalize(IDBTableQueryRow* queryRows) = 0;
	virtual const string& getTableName() = 0;
	virtual int RenameTable(const string& orgTableName, const string& newTableName) = 0;
	virtual const DBInfo& GetTableInfo() = 0;
};

class IDBController
{
public:
	virtual ~IDBController(){}
	virtual int CreateDBFile(const string& fileName, bool isNameWithPath) = 0;
	virtual int CreateTable(const DBInfo& tconfig, IDBTable** ppDBTable) = 0;
	virtual IDBTable* GetTable() = 0;
};

int CreateDBController(IDBController** ppDBController);
void ReleaseDBController(IDBController* pDBController);



#endif  //_DB_MODULE_PRIVATE_H
