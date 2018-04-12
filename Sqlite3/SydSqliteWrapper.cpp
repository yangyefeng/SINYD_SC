#include "sqlite3/SydSqliteWrapper.h"
#include <stdio.h>


CSydSqliteWrapper::CSydSqliteWrapper():mp_syd_sqlite(NULL)
{
	mp_syd_sqlite = new CSydsqlite();
}


CSydSqliteWrapper::~CSydSqliteWrapper()
{
	if (NULL != mp_syd_sqlite)
	{
		delete mp_syd_sqlite;
		mp_syd_sqlite = NULL;
	}
}

bool CSydSqliteWrapper::InsertDB(char * sql)
{
	if (!mp_syd_sqlite->DBopen(SYD_SQLITE3_DBNAME))
	{
		mp_syd_sqlite->DBclose();
		return false;
	}

	if (!mp_syd_sqlite->BeginTransaction())
	{
		mp_syd_sqlite->DBclose();
		return false;
	}

	if (!mp_syd_sqlite->Execute(sql)) {
		mp_syd_sqlite->RollbackTransaction();
		mp_syd_sqlite->DBclose();
		return false;
	}

	mp_syd_sqlite->CommitTransaction();
	mp_syd_sqlite->DBclose();
	return true;
}

bool CSydSqliteWrapper::SelectDB(char * sql, QueryCallback qcb, void *p)
{
	if (!mp_syd_sqlite->DBopen(SYD_SQLITE3_DBNAME))
	{
		mp_syd_sqlite->DBclose();
		return false;
	}

	//QueryCallback qcb = SelectdbCB;
	if (!mp_syd_sqlite->ExcuteQuery(sql, qcb, p)) {
		mp_syd_sqlite->DBclose();
		return false;
	}

	mp_syd_sqlite->DBclose();
	return true;
}

int CSydSqliteWrapper::SelectdbCB(void *para, int n_column, char **column_value, char **column_name)
{
	printf("test!!!!!!!!!!!!!!!\n");
	for (int i = 0; i < n_column; i++) {
		if (column_value[i] == '\0') {
			printf("=====%s[%s]=====\n", column_name[i], column_value[i]);
		}
		printf("[name]:%s, value:%s \n",column_name[i],column_value[i]);
	}
	return 0;
}