#include "DBTableQueryRow.h"

CDBTableQueryRow::CDBTableQueryRow(const CppSQLite3Query& query)
	: m_queryRes(query)
{
}

CDBTableQueryRow::~CDBTableQueryRow()
{
}

int CDBTableQueryRow::numFields()
{
	return m_queryRes.numFields();
}

int CDBTableQueryRow::numDataFields()
{
	return RealFieldNum2DataFieldNum(numFields());	// ºı»•PKA
}

bool CDBTableQueryRow::eof()
{
	return m_queryRes.eof();
}

int CDBTableQueryRow::nextRow()
{
	int iRes = DBModule_SUCCESS;
	try
	{
		m_queryRes.nextRow();
	}
	catch (CppSQLite3Exception& e)
	{
		iRes = SQLiteError2DBModuleError(e.errorCode());
	}
	return iRes;
}

int CDBTableQueryRow::RowPK(int & pk)
{
	int iRes = DBModule_SUCCESS;
	try
	{
		pk = m_queryRes.getIntField(0);
	}
	catch (CppSQLite3Exception& e)
	{
		iRes = SQLiteError2DBModuleError(e.errorCode());
	}
	return iRes;
}

int CDBTableQueryRow::DataFieldName(int nField, const char*& fieldName)
{
	int iRes = DBModule_SUCCESS;
	try
	{
		fieldName = m_queryRes.fieldName(DataFieldNum2RealFieldNum(nField)) + 4;
	}
	catch (CppSQLite3Exception& e)
	{
		iRes = SQLiteError2DBModuleError(e.errorCode());
	}
	return iRes;
}

int CDBTableQueryRow::DataFieldIsNull(int nField, bool & pIsNull)
{
	int iRes = DBModule_SUCCESS;
	try
	{
		pIsNull = m_queryRes.fieldIsNull(DataFieldNum2RealFieldNum(nField));
	}
	catch (CppSQLite3Exception& e)
	{
		iRes = SQLiteError2DBModuleError(e.errorCode());
	}
	return iRes;
}

int CDBTableQueryRow::GetStringDataField(int nField, const char *& value, const char * szNullValue)
{
	int iRes = DBModule_SUCCESS;
	try
	{
		value = m_queryRes.getStringField(DataFieldNum2RealFieldNum(nField), szNullValue);
	}
	catch (CppSQLite3Exception& e)
	{
		iRes = SQLiteError2DBModuleError(e.errorCode());
	}
	return iRes;
}

