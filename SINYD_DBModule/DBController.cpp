#include "DBController.h"
#include "DBTable.h"
#include <time.h>
#include "MeasureDef.h"
#include "DBHandleManager.h"

#define DEFAULT_TABLE_NAME "data"

int CreateDBController(IDBController** ppDBController)
{
	*ppDBController = new CDBController();
	if (*ppDBController == NULL)
	{
		return DBModule_FAIL;
	}
	return DBModule_SUCCESS;
}
void ReleaseDBController(IDBController* pDBController)
{
	if (pDBController != NULL)
	{
		delete pDBController;
		pDBController = NULL;
	}
}

CDBController::CDBController()
	: m_pIDBTable(NULL)
	, m_pSqlite3DB(NULL)
{
}

CDBController::~CDBController()
{
	ClearSelf();
}

int CDBController::CreateDBFile(const string& fileName, bool isNameWithPath)
{
	try
	{
		ClearSelf();
// 		string DBFilepath = CONFIG_DB_PATH + fileName;
// 		m_sqlite3DB.open(DBFilepath.c_str());
// 		m_sqlite3DB.execDML("PRAGMA synchronous = FULL;");

		if (isNameWithPath)
		{
			m_dbPath = fileName;
		}
		else
		{
			m_dbPath = CONFIG_DB_PATH + fileName;
		}
		
		m_pSqlite3DB = DBHanleMgrInstance::GetInstance()->CreateDB(m_dbPath);

		if (m_pSqlite3DB == NULL)
		{
			return DBModule_FAIL;
		}
	}
	catch (CppSQLite3Exception& e)
	{
		return SQLiteError2DBModuleError(e.errorCode());
	}
	
	return DBModule_SUCCESS;
}

int CDBController::CreateTable(const DBInfo& tconfig, IDBTable** ppDBTable)
{
	*ppDBTable = NULL;

	int iRes = DBModule_FAIL;

	CDBTable* pDBTable = new CDBTable(m_pSqlite3DB);
	try
	{
		iRes = pDBTable->Create(DEFAULT_TABLE_NAME, tconfig);
		if (DBModule_SUCCESS == iRes)
		{
			m_pIDBTable = pDBTable;
			*ppDBTable = pDBTable;
			iRes = DBModule_SUCCESS;
		}
		else
		{
			delete pDBTable;
			pDBTable = NULL;
		}
	}
	catch (CppSQLite3Exception& e)
	{
		delete pDBTable;
		pDBTable = NULL;

		iRes = SQLiteError2DBModuleError(e.errorCode());
	}
	catch (...)
	{
		delete pDBTable;
		pDBTable = NULL;

		iRes = DBModule_FAIL;
	}

	return iRes;
}

IDBTable* CDBController::GetTable()
{
	IDBTable* pRes = NULL;

	if (m_pIDBTable == NULL)
	{
		CDBTable* pDBTable = new CDBTable(m_pSqlite3DB);
		try
		{
			int iRes = pDBTable->Open(DEFAULT_TABLE_NAME);
			if (DBModule_SUCCESS == iRes)
			{
				m_pIDBTable = pDBTable;
				iRes = DBModule_SUCCESS;
			}
			else
			{
				delete pDBTable;
				pDBTable = NULL;
			}
		}
		catch (CppSQLite3Exception& e)
		{
			if (pDBTable)
			{
				delete pDBTable;
				pDBTable = NULL;
			}
			m_pIDBTable = NULL;
		}
		catch (...)
		{
			if (pDBTable)
			{
				delete pDBTable;
				pDBTable = NULL;
			}
			m_pIDBTable = NULL;
		}
	}

	return m_pIDBTable;
}

void CDBController::ClearSelf()
{
	if (m_pIDBTable)
	{
		delete m_pIDBTable;
		m_pIDBTable = NULL;
	}
	//m_sqlite3DB.close();
	if (m_pSqlite3DB)
	{
		DBHanleMgrInstance::GetInstance()->ReleaseDB(m_dbPath);
		m_pSqlite3DB = NULL;
	}
	m_dbPath = "";
}

