#include "DBModule.h"
#include "CppSqlite3/sqlite3.h"
#include "DBOperator.h"

CDBModuleException::CDBModuleException(const int nErrCode)
	: mnErrCode(nErrCode)
{
}


CDBModuleException::CDBModuleException(const CDBModuleException&  e)
	: mnErrCode(e.mnErrCode)
{
}

CDBModuleException::~CDBModuleException()
{
}

const char* CDBModuleException::errorCodeAsString(int nErrCode)
{
	switch (nErrCode)
	{
	case DBModule_SUCCESS: return "DBModule_SUCCESS";
	case DBModule_FAIL: return "DBModule_FAIL";
	case DBModule_DBTableAlreadyExisted: return "DBModule_DBTableAlreadyExisted";
	case DBModule_DBTableNotExisted: return "DBModule_DBTableNotExisted";
	case DBModule_UNKNOWNDATATYPE: return "DBModule_UNKNOWNDATATYPE";
	case DBModule_WRONGDATACOUNT: return "DBModule_WRONGDATACOUNT";
	case DBModule_NULLDBPTR: return "DBModule_NULLDBPTR";
	case SQLiteError2DBModuleError(SQLITE_OK): return "SQLITE_OK";
	case SQLiteError2DBModuleError(SQLITE_ERROR): return "SQLITE_ERROR";
	case SQLiteError2DBModuleError(SQLITE_INTERNAL): return "SQLITE_INTERNAL";
	case SQLiteError2DBModuleError(SQLITE_PERM): return "SQLITE_PERM";
	case SQLiteError2DBModuleError(SQLITE_ABORT): return "SQLITE_ABORT";
	case SQLiteError2DBModuleError(SQLITE_BUSY): return "SQLITE_BUSY";
	case SQLiteError2DBModuleError(SQLITE_LOCKED): return "SQLITE_LOCKED";
	case SQLiteError2DBModuleError(SQLITE_NOMEM): return "SQLITE_NOMEM";
	case SQLiteError2DBModuleError(SQLITE_READONLY): return "SQLITE_READONLY";
	case SQLiteError2DBModuleError(SQLITE_INTERRUPT): return "SQLITE_INTERRUPT";
	case SQLiteError2DBModuleError(SQLITE_IOERR): return "SQLITE_IOERR";
	case SQLiteError2DBModuleError(SQLITE_CORRUPT): return "SQLITE_CORRUPT";
	case SQLiteError2DBModuleError(SQLITE_NOTFOUND): return "SQLITE_NOTFOUND";
	case SQLiteError2DBModuleError(SQLITE_FULL): return "SQLITE_FULL";
	case SQLiteError2DBModuleError(SQLITE_CANTOPEN): return "SQLITE_CANTOPEN";
	case SQLiteError2DBModuleError(SQLITE_PROTOCOL): return "SQLITE_PROTOCOL";
	case SQLiteError2DBModuleError(SQLITE_EMPTY): return "SQLITE_EMPTY";
	case SQLiteError2DBModuleError(SQLITE_SCHEMA): return "SQLITE_SCHEMA";
	case SQLiteError2DBModuleError(SQLITE_TOOBIG): return "SQLITE_TOOBIG";
	case SQLiteError2DBModuleError(SQLITE_CONSTRAINT): return "SQLITE_CONSTRAINT";
	case SQLiteError2DBModuleError(SQLITE_MISMATCH): return "SQLITE_MISMATCH";
	case SQLiteError2DBModuleError(SQLITE_MISUSE): return "SQLITE_MISUSE";
	case SQLiteError2DBModuleError(SQLITE_NOLFS): return "SQLITE_NOLFS";
	case SQLiteError2DBModuleError(SQLITE_AUTH): return "SQLITE_AUTH";
	case SQLiteError2DBModuleError(SQLITE_FORMAT): return "SQLITE_FORMAT";
	case SQLiteError2DBModuleError(SQLITE_RANGE): return "SQLITE_RANGE";
	case SQLiteError2DBModuleError(SQLITE_ROW): return "SQLITE_ROW";
	case SQLiteError2DBModuleError(SQLITE_DONE): return "SQLITE_DONE";
	default: return "UNKNOWN_ERROR";
	}
}




SC_DB_MODULE_API IDBOperator* SC_DB_MODULE_EXPORT_CALLCONV CreateDBOperator(const DBInfo& dbInfo)
{
	return new CDBOperator(dbInfo);
}
SC_DB_MODULE_API IDBOperator* SC_DB_MODULE_EXPORT_CALLCONV OpenDBOperator(const string& dbName, bool isNameWithPath)
{
	return new CDBOperator(dbName, isNameWithPath);
}
SC_DB_MODULE_API void SC_DB_MODULE_EXPORT_CALLCONV ReleaseDBOperator(IDBOperator* pDBOperator)
{
	if (pDBOperator != NULL)
	{
		delete pDBOperator;
		pDBOperator = NULL;
	}
}
