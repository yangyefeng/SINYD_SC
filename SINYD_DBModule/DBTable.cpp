#include "DBTable.h"
#include "DBTableConfigTool.h"
#include "DBTableQueryRow.h"

CDBTable::CDBTable(CppSQLite3DB * pSqliteDB)
	: m_sqlite3DB(pSqliteDB)
{
}

CDBTable::~CDBTable()
{
}

int CDBTable::Create(const string& name, const DBInfo& info)
{
	if (!m_sqlite3DB)
	{
		return DBModule_NULLDBPTR;
	}

	int iRes = DBModule_SUCCESS;

	m_tableName = name;
	m_ColumnCount = info.fields.size();
	m_TableInfo = info;

	try
	{
		CreateTable();
		CreateSTMT();
	}
	catch (CppSQLite3Exception& e)
	{
		iRes = SQLiteError2DBModuleError(e.errorCode());
	}
	catch (int& e)
	{
		iRes = e;
	}
	catch (...)
	{
		iRes = DBModule_FAIL;
	}
	
	return iRes;
}

int CDBTable::Open(const string& name)
{
	if (!m_sqlite3DB)
	{
		return DBModule_NULLDBPTR;
	}

	int iRes = DBModule_SUCCESS;
	
	m_tableName = name;

	CDBTableConfigTool configTool(m_sqlite3DB);
	configTool.GetTableConfigFromDB(m_tableName, &m_TableInfo);
	m_TableInfo.configInfo = configTool.GetDBConfigPath();
	m_ColumnCount = m_TableInfo.fields.size();

	list<FieldInfo>::const_iterator itE = m_TableInfo.fields.end();
	int iColumnIndex = 1;
	for (list<FieldInfo>::const_iterator it = m_TableInfo.fields.begin(); it != itE; ++it)
	{
		m_columnIndexsMap[iColumnIndex] = it->fieldName;
		iColumnIndex++;
	}

	try
	{
		CreateSTMT();
	}
	catch (CppSQLite3Exception& e)
	{
		iRes = SQLiteError2DBModuleError(e.errorCode());
	}
	catch (int& e)
	{
		iRes = e;
	}
	catch (...)
	{
		iRes = DBModule_FAIL;
	}

	return iRes;
}

void CDBTable::CreateSTMT()
{
	string insertSQL = "insert into " + m_tableName + " values(NULL";
	for (int i = 0; i < m_TableInfo.fields.size(); i++)
	{
		insertSQL = insertSQL + "," + "?";
	}
	insertSQL += ")";
	//string querySQL = "select * from " + m_tableName + " where PKA >= ? and PKA <= ?;";

	m_InsertStmt = m_sqlite3DB->compileStatement(insertSQL.c_str());
	//m_QueryStmt = m_sqlite3DB->compileStatement(querySQL.c_str());
}

void CDBTable::CreateTable()
{
	string createTableSQL = "CREATE TABLE " + m_tableName + "(PKA INTEGER  PRIMARY KEY AUTOINCREMENT";

	list<FieldInfo>::const_iterator itE = m_TableInfo.fields.end();
	string dataType;
	int iColumnIndex = 1;
	for (list<FieldInfo>::const_iterator it = m_TableInfo.fields.begin(); it != itE; ++it)
	{
		switch (it->dataType)
		{
        case DATATYPE_INTEGER:
			dataType = "INTEGER";
			break;
        case DATATYPE_NUMERIC:
			dataType = "NUMERIC";
			break;
        case DATATYPE_REAL:
			dataType = "REAL";
			break;
        case DATATYPE_TEXT:
			dataType = "TEXT";
			break;
        case DATATYPE_FLOAT:
			dataType = "FLOAT";
			break;
        case DATATYPE_BLOB:
			dataType = "BLOB";
			break;
        case DATATYPE_BOOLEAN:
			dataType = "BOOLEAN";
			break;
        case DATATYPE_TIMESTAMP:
			dataType = "TIMESTAMP";
			break;
        case DATATYPE_CHAR:
			dataType = "CHAR";
        case DATATYPE_DATETIME:
			dataType = "DATETIME";
			break;
		default:
			dataType = "";
			break;
		}
		if (dataType == "")
		{
			throw DBModule_UNKNOWNDATATYPE;
		}
		createTableSQL = createTableSQL + "," + "col_" + it->fieldName + " " + dataType;

		m_columnIndexsMap[iColumnIndex] = it->fieldName;
		iColumnIndex++;
	}

	createTableSQL += ");";

	CDBTableConfigTool configTool(m_sqlite3DB);
	configTool.SetTableConfigToDB(m_tableName, m_TableInfo);
	configTool.SetDBConfigPath(m_TableInfo.configInfo);

	m_sqlite3DB->execDML(createTableSQL.c_str());
}

int CDBTable::Insert(const map<string, string>& datas, int* rowid)
{
	if (!m_sqlite3DB)
	{
		return DBModule_NULLDBPTR;
	}

	int iRes = DBModule_SUCCESS;
	
	try
	{
		map<int, string>::iterator itE = m_columnIndexsMap.end();
		for (map<int, string>::iterator it = m_columnIndexsMap.begin(); it != itE; ++it)
		{
			map<string, string>::const_iterator itFind = datas.find(it->second);
            if (itFind != datas.end())
			{
				m_InsertStmt.bind(it->first, (itFind->second).c_str());
			}
			else
			{
				m_InsertStmt.bindNull(it->first);
			}
			
		}
		m_InsertStmt.execDML();
		*rowid = m_sqlite3DB->lastRowId();
		m_InsertStmt.reset();
	}
	catch (CppSQLite3Exception& e)
	{
		iRes = SQLiteError2DBModuleError(e.errorCode());
	}
	catch (...)
	{
		iRes = DBModule_FAIL;
	}
	return iRes;
}

int CDBTable::Update(int rowid, const map<string, string>& datas)
{
	if (!m_sqlite3DB)
	{
		return DBModule_NULLDBPTR;
	}

	int iRes = DBModule_SUCCESS;

	try
	{
		string updateSQL = "update " + m_tableName + " set";
		map<string, string>::const_iterator itE = datas.end();
		for (map<string, string>::const_iterator it = datas.begin(); it != itE; ++it)
		{
			updateSQL = updateSQL + " col_" + it->first + " = " + it->second + ",";
		}
		updateSQL = updateSQL.substr(0, updateSQL.size() - 1);
		char cRowId[10] = { 0 };
		sprintf(cRowId, "%d", rowid);
		updateSQL = updateSQL + " where PKA = " + cRowId + ";";

		m_sqlite3DB->execDML(updateSQL.c_str());
	}
	catch (CppSQLite3Exception& e)
	{
		iRes = SQLiteError2DBModuleError(e.errorCode());
	}
	catch (...)
	{
		iRes = DBModule_FAIL;
	}
	return iRes;

}

int CDBTable::Query(int startRowID, int endRowID, IDBTableQueryRow** queryRows)
{
	if (!m_sqlite3DB)
	{
		return DBModule_NULLDBPTR;
	}

	int iRes = DBModule_SUCCESS;

	try
	{
		if (startRowID == -1)
		{
			string querySQL = "select * from " + m_tableName + " where PKA <= ?;";
			m_QueryStmt = m_sqlite3DB->compileStatement(querySQL.c_str());
			m_QueryStmt.bind(1, endRowID);
			*queryRows = new CDBTableQueryRow(m_QueryStmt.execQuery());
		}
		else if (endRowID == -1)
		{
			string querySQL = "select * from " + m_tableName + " where PKA >= ?;";
			m_QueryStmt = m_sqlite3DB->compileStatement(querySQL.c_str());
			m_QueryStmt.bind(1, startRowID);
			*queryRows = new CDBTableQueryRow(m_QueryStmt.execQuery());
		}
		else
		{
			string querySQL = "select * from " + m_tableName + " where PKA >= ? and PKA <= ?;";
			m_QueryStmt = m_sqlite3DB->compileStatement(querySQL.c_str());
			m_QueryStmt.bind(1, startRowID);
			m_QueryStmt.bind(2, endRowID);
			*queryRows = new CDBTableQueryRow(m_QueryStmt.execQuery());
		}

		//m_QueryStmt.bind(1, startRowID);
		//m_QueryStmt.bind(2, endRowID);
		//*queryRows = new CDBTableQueryRow(m_QueryStmt.execQuery());
	}
	catch (CppSQLite3Exception& e)
	{
		iRes = SQLiteError2DBModuleError(e.errorCode());
	}
	catch (...)
	{
		iRes = DBModule_FAIL;
	}
	return iRes;

}

void CDBTable::QueryFinalize(IDBTableQueryRow * queryRows)
{
	if (queryRows)
	{
		delete queryRows;
		queryRows = NULL;
	}
	m_QueryStmt.reset();
}

const string& CDBTable::getTableName()
{
	return m_tableName;
}

int CDBTable::RenameTable(const string& orgTableName, const string& newTableName)
{
	if (!m_sqlite3DB)
	{
		return DBModule_NULLDBPTR;
	}

	int iRes = DBModule_SUCCESS;

	try
	{
		CDBTableConfigTool configTool(m_sqlite3DB);
		configTool.RenameTableConfig(orgTableName, newTableName);
		string renameTableSQL = "ALTER TABLE " + orgTableName + " RENAME TO " + newTableName + ";";
		m_sqlite3DB->execDML(renameTableSQL.c_str());
		m_tableName = newTableName;
		CreateSTMT();
	}
	catch (CppSQLite3Exception& e)
	{
		iRes = SQLiteError2DBModuleError(e.errorCode());
	}
	catch (...)
	{
		iRes = DBModule_FAIL;
	}

	return iRes;
}

const DBInfo&  CDBTable::GetTableInfo()
{
	return m_TableInfo;
}


