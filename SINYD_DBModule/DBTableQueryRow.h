/////////////////////////////////////////////////////////////////////////////
/// @File name: DBTableQueryRow.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	数据存储检索结果集
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#if !defined(_CDBTABLEQUERYROW_H)
#define _CDBTABLEQUERYROW_H

#include "DBModulePrivate.h"
#include "CppSqlite3/CppSQLite3.h"

#define DataFieldNum2RealFieldNum(dataFiledNum) ((dataFiledNum) + 1)
#define RealFieldNum2DataFieldNum(realFiledNum) ((realFiledNum) - 1)

class CDBTableQueryRow : public IDBTableQueryRow
{
public:
	CDBTableQueryRow(const CppSQLite3Query& query);
	virtual ~CDBTableQueryRow();

	///*******************************************************
	/// @name:   CDBTableQueryRow::numDataFields
	/// @author: YaoDi
	/// @return: int
	/// @brief:  获取结果的字段个数
	///*******************************************************
	virtual int numDataFields();
	///*******************************************************
	/// @name:   CDBTableQueryRow::eof
	/// @author: YaoDi
	/// @return: bool
	/// @brief:  是否已到达结果集的结束
	///*******************************************************
	virtual bool eof();
	///*******************************************************
	/// @name:   CDBTableQueryRow::nextRow
	/// @author: YaoDi
	/// @return: int
	/// @brief:  光标移动到结果集的下一行数据
	///*******************************************************
	virtual int nextRow();
	///*******************************************************
	/// @name:   CDBTableQueryRow::RowPK
	/// @author: YaoDi
	/// @return: int
	/// @param:  [out][int &]pk
	/// @brief:  返回当前行的主键ID
	///*******************************************************
	virtual int RowPK(int& pk);
	///*******************************************************
	/// @name:   CDBTableQueryRow::DataFieldName
	/// @author: YaoDi
	/// @return: int
	/// @param:  [in][int]nField
	/// @param:  [out][const char * &]fieldName
	/// @brief:  获取第N个字段的字段名
	///*******************************************************
	virtual int DataFieldName(int nField, const char*& fieldName);
	///*******************************************************
	/// @name:   CDBTableQueryRow::DataFieldIsNull
	/// @author: YaoDi
	/// @return: int
	/// @param:  [in][int]nField
	/// @param:  [out][bool &]pIsNull
	/// @brief:  判断第N个字段的数据是否为空
	///*******************************************************
	virtual int DataFieldIsNull(int nField, bool& pIsNull);
	///*******************************************************
	/// @name:   CDBTableQueryRow::GetStringDataField
	/// @author: YaoDi
	/// @return: int
	/// @param:  [in][int]nField
	/// @param:  [in][const char * &]value
	/// @param:  [out][const char *]szNullValue
	/// @brief:  获取第N个字段的字符串格式的数据
	///*******************************************************
	virtual int GetStringDataField(int nField, const char*& value, const char* szNullValue = "");
	
protected:
	virtual int numFields();

protected:
	CppSQLite3Query m_queryRes;
};

#endif  //_CDBTABLEQUERYROW_H
