/////////////////////////////////////////////////////////////////////////////
/// @File name: DBModule.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	数据存储模块接口文件
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#if !defined(_DB_MODULE_H)
#define _DB_MODULE_H

#include <string>
#include <list>
#include "DBModuleDef.h"
#include <map>

using namespace std;

typedef struct FieldInfo
{
public:
	string fieldName;
	FieldDataType dataType;
	string fieldOriginalName;
}FIELDINFO, *PFIELDINFO;

typedef struct ConfigInfo
{
	string DBConfigPath;
	string ConfigTimeStamp;
}CONFIGINFO, *PCONFIGINFO;

typedef struct DBInfo
{
	string DBNamePrefix;
	list<FIELDINFO> fields;
	ConfigInfo configInfo;
}TABLEINFO, *PTABLEINFO;

class SC_DB_MODULE_API CDBModuleException
{
public:
	CDBModuleException(const int nErrCode);
	CDBModuleException(const CDBModuleException& e);
	virtual ~CDBModuleException();
	const int errorCode() { return mnErrCode; }
	static const char* errorCodeAsString(int nErrCode);
private:
	int mnErrCode;
};

class IDBQueryRow
{
public:
	virtual ~IDBQueryRow(){}
	///*******************************************************
	/// @name:   IDBQueryRow::numDataFields
	/// @author: YaoDi
	/// @return: int
	/// @brief:  返回结果集的字段个数
	///*******************************************************
	virtual int numDataFields() = 0;
	///*******************************************************
	/// @name:   IDBQueryRow::eof
	/// @author: YaoDi
	/// @return: bool
	/// @brief:  结果集是否已经到达结束行
	///*******************************************************
	virtual bool eof() = 0;
	///*******************************************************
	/// @name:   IDBQueryRow::nextRow
	/// @author: YaoDi
	/// @return: void
	/// @brief:  光标移动到下一行
	///*******************************************************
	virtual void nextRow() = 0;
	///*******************************************************
	/// @name:   IDBQueryRow::RowPK
	/// @author: YaoDi
	/// @return: int
	/// @brief:  返回当前行的主键
	///*******************************************************
	virtual int RowPK() = 0;
	///*******************************************************
	/// @name:   IDBQueryRow::DataFieldName
	/// @author: YaoDi
	/// @return: const char*
	/// @param:  [in][int]nField
	/// @brief:  返回第N个字段的字段名
	///*******************************************************
	virtual const char* DataFieldName(int nField) = 0;
	///*******************************************************
	/// @name:   IDBQueryRow::DataFieldIsNull
	/// @author: YaoDi
	/// @return: bool
	/// @param:  [in][int]nField
	/// @brief:  判断第N个字段的数据是否为空
	///*******************************************************
	virtual bool DataFieldIsNull(int nField) = 0;
    ///*******************************************************
    /// @name:   IDBQueryRow::GetStringDataField
    /// @author: YaoDi
    /// @return: const char *
    /// @param:  [in][int]nField
    /// @param:  [in][const char *]szNullValue
    /// @brief:  获取第N个字段的字符串格式的数据
    ///*******************************************************
    virtual const char * GetStringDataField(int nField, const char* szNullValue = NULL) = 0;
};

class IDBOperator
{
public:
	virtual ~IDBOperator(){}
	///*******************************************************
	/// @name:   IDBOperator::Insert
	/// @author: YaoDi
	/// @return: int
	/// @param:  [in][const]map<string string> &]datas
	/// @brief:  插入一条数据，如果传入的map没有某一字段，则插入时这一字段数据为空
	///*******************************************************
	virtual int Insert(const map<string, string>& datas) = 0;
	virtual void Update(int rowid, const map<string, string>& datas) = 0;
	///*******************************************************
	/// @name:   IDBOperator::Query
	/// @author: YaoDi
	/// @return: IDBQueryRow*
	/// @param:  [in][int]startRowID
	/// @param:  [in][int]endRowID
	/// @brief:  检索主键从startRowID到endRowID的数据
	///          如果endRowID为-1，则从startRowID开始检索到结束
	///          如果StartRowID为-1，则从第一条数据开始检索到endRowID
	///*******************************************************
	virtual IDBQueryRow* Query(int startRowID, int endRowID) = 0;
	///*******************************************************
	/// @name:   IDBOperator::QueryFinalize
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][IDBQueryRow *]queryRows
	/// @brief:  释放检索结果
	///*******************************************************
	virtual void QueryFinalize(IDBQueryRow* queryRows) = 0;
	///*******************************************************
	/// @name:   IDBOperator::GetDBInfo
	/// @author: YaoDi
	/// @return: const DBInfo&
	/// @brief:  返回数据存储文件信息
	///*******************************************************
	virtual const DBInfo& GetDBInfo() = 0;
};

extern "C"
{
	///*******************************************************
	/// @name:   CreateDBOperator
	/// @author: YaoDi
	/// @return: SC_DB_MODULE_API IDBOperator* SC_DB_MODULE_EXPORT_CALLCONV
	/// @param:  [in][const DBInfo &]dbInfo
	/// @brief:  创建数据存储操作对象，内部会根据DBInfo新创建一个数据存储文件
	///*******************************************************
	SC_DB_MODULE_API IDBOperator* SC_DB_MODULE_EXPORT_CALLCONV CreateDBOperator(const DBInfo& dbInfo);
	///*******************************************************
	/// @name:   OpenDBOperator
	/// @author: YaoDi
	/// @return: SC_DB_MODULE_API IDBOperator* SC_DB_MODULE_EXPORT_CALLCONV
	/// @param:  [in][const string &]dbName
	/// @param:  [in][bool]isNameWithPath
	/// @brief:  打开已经存在的数据存储文件操作对象
	///*******************************************************
	SC_DB_MODULE_API IDBOperator* SC_DB_MODULE_EXPORT_CALLCONV OpenDBOperator(const string& dbName, bool isNameWithPath);
	///*******************************************************
	/// @name:   ReleaseDBOperator
	/// @author: YaoDi
	/// @return: SC_DB_MODULE_API void SC_DB_MODULE_EXPORT_CALLCONV
	/// @param:  [in][IDBOperator *]pDBOperator
	/// @brief:  释放创建的或打开的数据存储操作对象
	///*******************************************************
	SC_DB_MODULE_API void SC_DB_MODULE_EXPORT_CALLCONV ReleaseDBOperator(IDBOperator* pDBOperator);
}



#endif  //_DB_MODULE_H
