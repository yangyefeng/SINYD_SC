#include "DBHandleManager.h"



CDBHandleManager::CDBHandleManager()
{
}


CDBHandleManager::~CDBHandleManager()
{
	m_csHandles.Lock();

	map<string, DBHandle>::iterator itEnd = m_handles.end();
	for (map<string, DBHandle>::iterator it = m_handles.begin(); it != itEnd; ++it)
	{
		if (it->second.pDB)
		{
			delete it->second.pDB;
			it->second.pDB = NULL;
			it->second.count = 0;
		}
	}
	m_handles.clear();

	m_csHandles.Unlock();
}

CppSQLite3DB * CDBHandleManager::CreateDB(const string & path)
{
	m_csHandles.Lock();

	CppSQLite3DB* pRes = NULL;

	map<string, DBHandle>::iterator itFind = m_handles.find(path);
	if (itFind != m_handles.end())
	{
		pRes = itFind->second.pDB;
		itFind->second.count++;
	}
	else
	{
		DBHandle handle;
		try
		{
			handle.pDB = new CppSQLite3DB();
			handle.pDB->open(path.c_str());
			handle.pDB->execDML("PRAGMA synchronous = FULL;");
			handle.count = 1;
			m_handles[path] = handle;

			pRes = handle.pDB;
		}
		catch (CppSQLite3Exception& e)
		{
			if (handle.pDB)
			{
				delete handle.pDB;
				handle.pDB = NULL;
			}
		}
	}

	m_csHandles.Unlock();

	return pRes;
}

void CDBHandleManager::ReleaseDB(const string & path)
{
	m_csHandles.Lock();

	map<string, DBHandle>::iterator itFind = m_handles.find(path);
	if (itFind != m_handles.end())
	{
		itFind->second.count--;
		if (itFind->second.count == 0)
		{
			delete itFind->second.pDB;
			itFind->second.pDB = NULL;
			m_handles.erase(itFind);
		}
	}

	m_csHandles.Unlock();
}
