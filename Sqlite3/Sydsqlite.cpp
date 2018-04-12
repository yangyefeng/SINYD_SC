#include "sqlite3\Sydsqlite.h"
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

CSydsqlite::CSydsqlite():mp_syd_db(NULL)
{
}

CSydsqlite::~CSydsqlite()
{
	DBclose();
}

bool CSydsqlite::DBopen(char *db_name)
{
	if (db_name == NULL) {
		return false;
	}

	if ( SQLITE_OK != sqlite3_open(db_name, &mp_syd_db) )
	{
		printf("Open database %s failed!\n", db_name);
		return false;
	}
	sqlite3_exec(mp_syd_db, "PRAGMA synchronous = OFF; ", 0, 0, 0);
	return true;
}

void CSydsqlite::DBclose()
{
	if (mp_syd_db) {
		sqlite3_close(mp_syd_db);
		mp_syd_db = NULL;
	}
}

bool CSydsqlite::Execute(char *sql)
{
	char *eMsg = NULL;
	//SYSTEMTIME  time;
	////GetSystemTime(&time);
	//GetLocalTime(&time);
	//printf("[CSydsqlite::Execute]start---%d:%d:%d.%d[%lld]\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, time);

	if ( SQLITE_OK != sqlite3_exec(mp_syd_db, sql, NULL, NULL, &eMsg) )
	{
		printf("Execute sql: %s failed!\n", sql);
		printf("Faild eMsg:%s!\n", eMsg);
		return false;
	}
	//GetLocalTime(&time);
	//printf("[CSydsqlite::Execute]end---%d:%d:%d.%d[%lld]\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, time);
	return true;
}

bool CSydsqlite::ExcuteQuery(char *sql, QueryCallback pCallBack, void *p)
{
	if ( NULL == sql || NULL == pCallBack )
	{
		return false;
	}
	char *eMsg = NULL;

	if ( SQLITE_OK != sqlite3_exec(mp_syd_db, sql, pCallBack, p, &eMsg) )
	{
		printf("Execute sql %s failed!\n", sql);
		printf("Faild eMsg:%s!\n", eMsg);
		return false;
	}

	return true;
}

// 开始事务  
bool CSydsqlite::BeginTransaction()
{
	char * eMsg = NULL;
	if ( SQLITE_OK != sqlite3_exec(mp_syd_db, "BEGIN TRANSACTION;", NULL, NULL, &eMsg) )
	{
		printf("Begin transaction faild! eMsg:%s!\n", eMsg);
		return false;
	}
	return true;
}

// 提交事务  
bool CSydsqlite::CommitTransaction()
{
	char * eMsg = NULL;
	if ( SQLITE_OK != sqlite3_exec(mp_syd_db, "COMMIT TRANSACTION;", NULL, NULL, &eMsg) )
	{
		printf("Commit transaction faild! eMsg:%s!\n", eMsg);
		return false;
	}
	return true;
}

// 回滚事务  
bool CSydsqlite::RollbackTransaction()
{
	char * eMsg = NULL;
	if ( SQLITE_OK != sqlite3_exec(mp_syd_db, "ROLLBACK  TRANSACTION;", NULL, NULL, &eMsg) )
	{
		printf("RollBack transaction faild! eMsg:%s!\n", eMsg);
		return false;
	}
	return true;
}