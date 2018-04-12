#include "DBOperator.h"
#include <time.h>


CDBOperator::CDBOperator(const DBInfo& dbInfo)
{
	int iRes = CreateDBController(&m_pDBCtrl);
	if (iRes == DBModule_SUCCESS)
	{
		char ctime[26] = { 0 };
		time_t st_timet = time(NULL);
		tm *systime = localtime(&st_timet);
		//sprintf(ctime, "%04d-%02d-%02d %02d-%02d-%02d.db", systime->tm_year + 1900, systime->tm_mon + 1, systime->tm_mday, systime->tm_hour, systime->tm_min, systime->tm_sec);
		strftime(ctime, sizeof(ctime), "_%Y-%m-%d %H_%M_%S.db", systime);
		string name = dbInfo.DBNamePrefix + string(ctime);

		iRes = m_pDBCtrl->CreateDBFile(name, false);
		if (iRes == DBModule_SUCCESS)
		{
			iRes = m_pDBCtrl->CreateTable(dbInfo, &m_pDBTable);
		}
	}

	if (iRes != DBModule_SUCCESS)
	{
		throw CDBModuleException(iRes);
	}
	if (!m_pDBCtrl || !m_pDBTable)
	{
		throw CDBModuleException(DBModule_NULLDBPTR);
	}

	m_dbInfo = dbInfo;
}

CDBOperator::CDBOperator(const string& dbName, bool isNameWithPath)
{
	int iRes = CreateDBController(&m_pDBCtrl);
	if (iRes == DBModule_SUCCESS)
	{
		iRes = m_pDBCtrl->CreateDBFile(dbName, isNameWithPath);
		if (iRes == DBModule_SUCCESS)
		{
			m_pDBTable = m_pDBCtrl->GetTable();
			if (m_pDBTable)
			{
				m_dbInfo = m_pDBTable->GetTableInfo();
				m_dbInfo.DBNamePrefix = dbName.substr(0, dbName.length() - 23);
			}
		}
	}

	if (iRes != DBModule_SUCCESS)
	{
		throw CDBModuleException(iRes);
	}
	if (!m_pDBCtrl || !m_pDBTable)
	{
		throw CDBModuleException(DBModule_NULLDBPTR);
	}
}


CDBOperator::~CDBOperator()
{
	ReleaseDBController(m_pDBCtrl);
	m_pDBCtrl = NULL;
}

int CDBOperator::Insert(const map<string, string>& datas)
{
	int ret = -1;
	int iRes = m_pDBTable->Insert(datas, &ret);
	if (iRes != DBModule_SUCCESS)
	{
		throw CDBModuleException(iRes);
	}

	return ret;
}

void CDBOperator::Update(int rowid, const map<string, string>& datas)
{
	int iRes = m_pDBTable->Update(rowid, datas);
	if (iRes != DBModule_SUCCESS)
	{
		throw CDBModuleException(iRes);
	}
}

IDBQueryRow* CDBOperator::Query(int startRowID, int endRowID)
{
	IDBTableQueryRow* pDBTQ = NULL;
	int iRes = m_pDBTable->Query(startRowID, endRowID , &pDBTQ);
	if (iRes != DBModule_SUCCESS)
	{
		throw CDBModuleException(iRes);
	}

	return new CDBQueryRow(pDBTQ);
}

void CDBOperator::QueryFinalize(IDBQueryRow* queryRows)
{
	if (queryRows)
	{
		delete queryRows;
		queryRows = NULL;
	}
	m_pDBTable->QueryFinalize(NULL);

}

const DBInfo& CDBOperator::GetDBInfo()
{
	return m_dbInfo;
}

CDBQueryRow::CDBQueryRow(IDBTableQueryRow* pDBTQ)
	: m_pDBTableQuery(pDBTQ)
{

}

CDBQueryRow::~CDBQueryRow()
{
	if (m_pDBTableQuery)
	{
		delete m_pDBTableQuery;
		m_pDBTableQuery = NULL;
	}
}

int CDBQueryRow::numDataFields()
{
	return m_pDBTableQuery->numDataFields();
}

bool CDBQueryRow::eof()
{
	return m_pDBTableQuery->eof();
}

void CDBQueryRow::nextRow()
{
	int iRes = m_pDBTableQuery->nextRow();
	if (iRes != DBModule_SUCCESS)
	{
		throw CDBModuleException(iRes);
	}
}

int CDBQueryRow::RowPK()
{
	int ret = -1;
	int iRes = m_pDBTableQuery->RowPK(ret);
	if (iRes != DBModule_SUCCESS)
	{
		throw CDBModuleException(iRes);
	}

	return ret;
}

const char* CDBQueryRow::DataFieldName(int nField)
{
	const char* ret = NULL;
	int iRes = m_pDBTableQuery->DataFieldName(nField, ret);
	if (iRes != DBModule_SUCCESS)
	{
		throw CDBModuleException(iRes);
	}

	return ret;
}

bool CDBQueryRow::DataFieldIsNull(int nField)
{
	bool ret;
	int iRes = m_pDBTableQuery->DataFieldIsNull(nField, ret);
	if (iRes != DBModule_SUCCESS)
	{
		throw CDBModuleException(iRes);
	}

	return ret;
}

const char * CDBQueryRow::GetStringDataField(int nField, const char* szNullValue /*= ""*/)
{
	const char* ret = NULL;
	int iRes = m_pDBTableQuery->GetStringDataField(nField, ret);
	if (iRes != DBModule_SUCCESS)
	{
		throw CDBModuleException(iRes);
	}

	return ret;
}
