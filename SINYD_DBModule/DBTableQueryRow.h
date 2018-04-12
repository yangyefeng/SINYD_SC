/////////////////////////////////////////////////////////////////////////////
/// @File name: DBTableQueryRow.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	���ݴ洢���������
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
	/// @brief:  ��ȡ������ֶθ���
	///*******************************************************
	virtual int numDataFields();
	///*******************************************************
	/// @name:   CDBTableQueryRow::eof
	/// @author: YaoDi
	/// @return: bool
	/// @brief:  �Ƿ��ѵ��������Ľ���
	///*******************************************************
	virtual bool eof();
	///*******************************************************
	/// @name:   CDBTableQueryRow::nextRow
	/// @author: YaoDi
	/// @return: int
	/// @brief:  ����ƶ������������һ������
	///*******************************************************
	virtual int nextRow();
	///*******************************************************
	/// @name:   CDBTableQueryRow::RowPK
	/// @author: YaoDi
	/// @return: int
	/// @param:  [out][int &]pk
	/// @brief:  ���ص�ǰ�е�����ID
	///*******************************************************
	virtual int RowPK(int& pk);
	///*******************************************************
	/// @name:   CDBTableQueryRow::DataFieldName
	/// @author: YaoDi
	/// @return: int
	/// @param:  [in][int]nField
	/// @param:  [out][const char * &]fieldName
	/// @brief:  ��ȡ��N���ֶε��ֶ���
	///*******************************************************
	virtual int DataFieldName(int nField, const char*& fieldName);
	///*******************************************************
	/// @name:   CDBTableQueryRow::DataFieldIsNull
	/// @author: YaoDi
	/// @return: int
	/// @param:  [in][int]nField
	/// @param:  [out][bool &]pIsNull
	/// @brief:  �жϵ�N���ֶε������Ƿ�Ϊ��
	///*******************************************************
	virtual int DataFieldIsNull(int nField, bool& pIsNull);
	///*******************************************************
	/// @name:   CDBTableQueryRow::GetStringDataField
	/// @author: YaoDi
	/// @return: int
	/// @param:  [in][int]nField
	/// @param:  [in][const char * &]value
	/// @param:  [out][const char *]szNullValue
	/// @brief:  ��ȡ��N���ֶε��ַ�����ʽ������
	///*******************************************************
	virtual int GetStringDataField(int nField, const char*& value, const char* szNullValue = "");
	
protected:
	virtual int numFields();

protected:
	CppSQLite3Query m_queryRes;
};

#endif  //_CDBTABLEQUERYROW_H
