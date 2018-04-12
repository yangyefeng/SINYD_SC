/////////////////////////////////////////////////////////////////////////////
/// @File name: DBModule.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	���ݴ洢ģ��ӿ��ļ�
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
	/// @brief:  ���ؽ�������ֶθ���
	///*******************************************************
	virtual int numDataFields() = 0;
	///*******************************************************
	/// @name:   IDBQueryRow::eof
	/// @author: YaoDi
	/// @return: bool
	/// @brief:  ������Ƿ��Ѿ����������
	///*******************************************************
	virtual bool eof() = 0;
	///*******************************************************
	/// @name:   IDBQueryRow::nextRow
	/// @author: YaoDi
	/// @return: void
	/// @brief:  ����ƶ�����һ��
	///*******************************************************
	virtual void nextRow() = 0;
	///*******************************************************
	/// @name:   IDBQueryRow::RowPK
	/// @author: YaoDi
	/// @return: int
	/// @brief:  ���ص�ǰ�е�����
	///*******************************************************
	virtual int RowPK() = 0;
	///*******************************************************
	/// @name:   IDBQueryRow::DataFieldName
	/// @author: YaoDi
	/// @return: const char*
	/// @param:  [in][int]nField
	/// @brief:  ���ص�N���ֶε��ֶ���
	///*******************************************************
	virtual const char* DataFieldName(int nField) = 0;
	///*******************************************************
	/// @name:   IDBQueryRow::DataFieldIsNull
	/// @author: YaoDi
	/// @return: bool
	/// @param:  [in][int]nField
	/// @brief:  �жϵ�N���ֶε������Ƿ�Ϊ��
	///*******************************************************
	virtual bool DataFieldIsNull(int nField) = 0;
    ///*******************************************************
    /// @name:   IDBQueryRow::GetStringDataField
    /// @author: YaoDi
    /// @return: const char *
    /// @param:  [in][int]nField
    /// @param:  [in][const char *]szNullValue
    /// @brief:  ��ȡ��N���ֶε��ַ�����ʽ������
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
	/// @brief:  ����һ�����ݣ���������mapû��ĳһ�ֶΣ������ʱ��һ�ֶ�����Ϊ��
	///*******************************************************
	virtual int Insert(const map<string, string>& datas) = 0;
	virtual void Update(int rowid, const map<string, string>& datas) = 0;
	///*******************************************************
	/// @name:   IDBOperator::Query
	/// @author: YaoDi
	/// @return: IDBQueryRow*
	/// @param:  [in][int]startRowID
	/// @param:  [in][int]endRowID
	/// @brief:  ����������startRowID��endRowID������
	///          ���endRowIDΪ-1�����startRowID��ʼ����������
	///          ���StartRowIDΪ-1����ӵ�һ�����ݿ�ʼ������endRowID
	///*******************************************************
	virtual IDBQueryRow* Query(int startRowID, int endRowID) = 0;
	///*******************************************************
	/// @name:   IDBOperator::QueryFinalize
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][IDBQueryRow *]queryRows
	/// @brief:  �ͷż������
	///*******************************************************
	virtual void QueryFinalize(IDBQueryRow* queryRows) = 0;
	///*******************************************************
	/// @name:   IDBOperator::GetDBInfo
	/// @author: YaoDi
	/// @return: const DBInfo&
	/// @brief:  �������ݴ洢�ļ���Ϣ
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
	/// @brief:  �������ݴ洢���������ڲ������DBInfo�´���һ�����ݴ洢�ļ�
	///*******************************************************
	SC_DB_MODULE_API IDBOperator* SC_DB_MODULE_EXPORT_CALLCONV CreateDBOperator(const DBInfo& dbInfo);
	///*******************************************************
	/// @name:   OpenDBOperator
	/// @author: YaoDi
	/// @return: SC_DB_MODULE_API IDBOperator* SC_DB_MODULE_EXPORT_CALLCONV
	/// @param:  [in][const string &]dbName
	/// @param:  [in][bool]isNameWithPath
	/// @brief:  ���Ѿ����ڵ����ݴ洢�ļ���������
	///*******************************************************
	SC_DB_MODULE_API IDBOperator* SC_DB_MODULE_EXPORT_CALLCONV OpenDBOperator(const string& dbName, bool isNameWithPath);
	///*******************************************************
	/// @name:   ReleaseDBOperator
	/// @author: YaoDi
	/// @return: SC_DB_MODULE_API void SC_DB_MODULE_EXPORT_CALLCONV
	/// @param:  [in][IDBOperator *]pDBOperator
	/// @brief:  �ͷŴ����Ļ�򿪵����ݴ洢��������
	///*******************************************************
	SC_DB_MODULE_API void SC_DB_MODULE_EXPORT_CALLCONV ReleaseDBOperator(IDBOperator* pDBOperator);
}



#endif  //_DB_MODULE_H
