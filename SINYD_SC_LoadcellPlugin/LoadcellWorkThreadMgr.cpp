/////////////////////////////////////////////////////////////////////////////
/// @File name: LoadcellWorkThreadMgr.cpp
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 		
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#include "LoadcellWorkThreadMgr.h"

const int ThreadCount = 2;

CLoadcellWorkThreadMgr::CLoadcellWorkThreadMgr()
{
	CriticalSectionLock lock(m_listpWorkThreadCS);
	for (int i = 0; i < ThreadCount; i++)
	{
		m_listpWorkThread.push_back(new CLoadcellWorkThread());
	}
}


CLoadcellWorkThreadMgr::~CLoadcellWorkThreadMgr()
{
	CriticalSectionLock lock(m_listpWorkThreadCS);
	while (m_listpWorkThread.size() > 0)
	{
		CLoadcellWorkThread* pWorkThread = m_listpWorkThread.back();
		if (pWorkThread != nullptr)
		{
			pWorkThread->Stop();
			delete pWorkThread;
			pWorkThread = nullptr;
		}
		m_listpWorkThread.pop_back();
	}
}

SC_PLUGIN_ERROR CLoadcellWorkThreadMgr::Start()
{
	SC_PLUGIN_ERROR err = SC_PLUGIN_SUCCESS;

	CriticalSectionLock lock(m_listpWorkThreadCS);
	list<CLoadcellWorkThread*>::iterator itBegin = m_listpWorkThread.begin();
	list<CLoadcellWorkThread*>::iterator itEnd = m_listpWorkThread.end();
	for (list<CLoadcellWorkThread*>::iterator it = itBegin; it != itEnd; ++it)
	{
		if (*it != nullptr)
		{
			DWORD dw = (*it)->Start();
			if (dw != 0)
			{
				err = SC_PLUGIN_THREADSTARTFAIL;
			}
		}
		else
		{
			err = SC_PLUGIN_NULLPTR;
		}
	}
	return err;
}

SC_PLUGIN_ERROR CLoadcellWorkThreadMgr::Stop()
{
	SC_PLUGIN_ERROR err = SC_PLUGIN_SUCCESS;

	CriticalSectionLock lock(m_listpWorkThreadCS);
	list<CLoadcellWorkThread*>::iterator itBegin = m_listpWorkThread.begin();
	list<CLoadcellWorkThread*>::iterator itEnd = m_listpWorkThread.end();
	for (list<CLoadcellWorkThread*>::iterator it = itBegin; it != itEnd; ++it)
	{
		if (*it != nullptr)
		{
			(*it)->Stop();
		}
		else
		{
			err = SC_PLUGIN_NULLPTR;
		}
	}
	return err;
}

SC_PLUGIN_ERROR CLoadcellWorkThreadMgr::Suspend(unsigned long* plResult)
{
	SC_PLUGIN_ERROR err = SC_PLUGIN_SUCCESS;

	*plResult = 0;
	CriticalSectionLock lock(m_listpWorkThreadCS);
	list<CLoadcellWorkThread*>::iterator itBegin = m_listpWorkThread.begin();
	list<CLoadcellWorkThread*>::iterator itEnd = m_listpWorkThread.end();
	for (list<CLoadcellWorkThread*>::iterator it = itBegin; it != itEnd; ++it)
	{
		if (*it != nullptr)
		{
			*plResult += (*it)->Suspend();
		}
		else
		{
			err = SC_PLUGIN_NULLPTR;
		}
	}
	return err;
}

SC_PLUGIN_ERROR CLoadcellWorkThreadMgr::Resume(unsigned long* plResult)
{
	SC_PLUGIN_ERROR err = SC_PLUGIN_SUCCESS;

	*plResult = 0;
	CriticalSectionLock lock(m_listpWorkThreadCS);
	list<CLoadcellWorkThread*>::iterator itBegin = m_listpWorkThread.begin();
	list<CLoadcellWorkThread*>::iterator itEnd = m_listpWorkThread.end();
	for (list<CLoadcellWorkThread*>::iterator it = itBegin; it != itEnd; ++it)
	{
		if (*it != nullptr)
		{
			*plResult += (*it)->Resume();
		}
		else
		{
			err = SC_PLUGIN_NULLPTR;
		}
	}
	return err;
}

SC_PLUGIN_ERROR CLoadcellWorkThreadMgr::Execute(const TaskList& m_listspTask, 
												IScreenReader* pIScreenAreaReader, 
												IPluginExecuteCallback* pPluginExecuteCB, 
												int packageID, 
												const string& TimeStamp)
{
	SC_PLUGIN_ERROR err = SC_PLUGIN_SUCCESS;

	shared_ptr<CTaskListWaitingForWork> spTaskListForWork(new CTaskListWaitingForWork(m_listspTask));

	CriticalSectionLock lock(m_listpWorkThreadCS);
	list<CLoadcellWorkThread*>::iterator itBegin = m_listpWorkThread.begin();
	list<CLoadcellWorkThread*>::iterator itEnd = m_listpWorkThread.end();
	for (list<CLoadcellWorkThread*>::iterator it = itBegin; it != itEnd; ++it)
	{
		if (*it != nullptr)
		{
			(*it)->Work(spTaskListForWork, pIScreenAreaReader, pPluginExecuteCB, packageID, TimeStamp);
		}
		else
		{
			err = SC_PLUGIN_NULLPTR;
		}
	}
	return err;
}
