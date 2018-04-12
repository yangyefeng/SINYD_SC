#include "DBTableConfigTool.h"

#define ConfigTable_postfix "_Config"

CDBTableConfigTool::CDBTableConfigTool(CppSQLite3DB * pSqliteDB)
	: m_sqlite3DB(pSqliteDB)
{
}

CDBTableConfigTool::~CDBTableConfigTool()
{
}

void CDBTableConfigTool::RenameTableConfig(const string& orgTableName, const string& newTableName)
{
	if (!m_sqlite3DB)
	{
		return;
	}

	string renameTableSQL = "ALTER TABLE " + orgTableName + ConfigTable_postfix + " RENAME TO " + newTableName + ConfigTable_postfix + ";";
	m_sqlite3DB->execDML(renameTableSQL.c_str());
}

void CDBTableConfigTool::SetTableConfigToDB(const string& tableName, const DBInfo& info)
{
	if (!m_sqlite3DB)
	{
		return;
	}

	string createTableSQL = "CREATE TABLE " + tableName + ConfigTable_postfix + "(ColumnName TEXT, ColumnOriginalName TEXT, ColumnDataType INTEGER);";
	m_sqlite3DB->execDML(createTableSQL.c_str());

	string insertSQL = "insert into " + tableName + ConfigTable_postfix + " values(?,?,?);";
	CppSQLite3Statement stmt = m_sqlite3DB->compileStatement(insertSQL.c_str());
	
	m_sqlite3DB->execDML("begin transaction;");
	try
	{
		list<FieldInfo>::const_iterator itE = info.fields.end();
		for (list<FieldInfo>::const_iterator it = info.fields.begin(); it != itE; ++it)
		{
			stmt.bind(1, it->fieldName.c_str());
			stmt.bind(2, it->fieldOriginalName.c_str());
			stmt.bind(3, (int)it->dataType);
			stmt.execDML();
			stmt.reset();
		}
		m_sqlite3DB->execDML("commit transaction;");
	}
	catch (CppSQLite3Exception& e)
	{
		m_sqlite3DB->execDML("rollback transaction;");
		throw e;
	}
}

void CDBTableConfigTool::GetTableConfigFromDB(const string& tableName, DBInfo* pInfo)
{
	if (!m_sqlite3DB)
	{
		return;
	}

	pInfo->DBNamePrefix = tableName;
    string querySQL = "select * from " + tableName + ConfigTable_postfix +";";
	CppSQLite3Query query = m_sqlite3DB->execQuery(querySQL.c_str());
	FieldInfo columnInfo;
	while (!query.eof())
	{
		columnInfo.fieldName = query.getStringField(0);
		columnInfo.fieldOriginalName = query.getStringField(1);
		columnInfo.dataType = (FieldDataType)query.getIntField(2);
		pInfo->fields.push_back(columnInfo);

		query.nextRow();
	}
}

void CDBTableConfigTool::SetDBConfigPath(const ConfigInfo & configInfo)
{
	if (!m_sqlite3DB)
	{
		return;
	}

	const char* createTableSQL = "CREATE TABLE ConfigPath(Path TEXT, time TEXT);";
	m_sqlite3DB->execDML(createTableSQL);
	string insertSQL = "insert into ConfigPath values(\"" + configInfo.DBConfigPath + "\",\"" + configInfo.ConfigTimeStamp + "\");";
	m_sqlite3DB->execDML(insertSQL.c_str());
}

ConfigInfo CDBTableConfigTool::GetDBConfigPath()
{
	ConfigInfo res;

	string querySQL = "select * from ConfigPath";
	CppSQLite3Query query = m_sqlite3DB->execQuery(querySQL.c_str());
	if (!query.eof())
	{
		res.DBConfigPath = query.getStringField(0);
		res.ConfigTimeStamp = query.getStringField(1);
	}

	return res;
}

