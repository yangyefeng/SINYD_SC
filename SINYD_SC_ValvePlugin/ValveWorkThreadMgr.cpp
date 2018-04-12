/////////////////////////////////////////////////////////////////////////////
/// @File name: ValveWorkThreadMgr.cpp
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 		
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#include "ValveWorkThreadMgr.h"

const int ThreadCount = 2;

CValveWorkThreadMgr::CValveWorkThreadMgr()
{
	CriticalSectionLock lock(m_listpWorkThreadCS);
	for (int i = 0; i < ThreadCount; i++)
	{
		m_listpWorkThread.push_back(new CValveWorkThread());
	}
}


CValveWorkThreadMgr::~CValveWorkThreadMgr()
{
	CriticalSectionLock lock(m_listpWorkThreadCS);
	while (m_listpWorkThread.size() > 0)
	{
		CValveWorkThread* pWorkThread = m_listpWorkThread.back();
		if (pWorkThread != nullptr)
		{
			pWorkThread->Stop();
			delete pWorkThread;
			pWorkThread = nullptr;
		}
		m_listpWorkThread.pop_back();
	}
}

SC_PLUGIN_ERROR CValveWorkThreadMgr::Start()
{
	SC_PLUGIN_ERROR err = SC_PLUGIN_SUCCESS;

	CriticalSectionLock lock(m_listpWorkThreadCS);
	list<CValveWorkThread*>::iterator itBegin = m_listpWorkThread.begin();
	list<CValveWorkThread*>::iterator itEnd = m_listpWorkThread.end();
	for (list<CValveWorkThread*>::iterator it = itBegin; it != itEnd; ++it)
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

SC_PLUGIN_ERROR CValveWorkThreadMgr::Stop()
{
	SC_PLUGIN_ERROR err = SC_PLUGIN_SUCCESS;

	CriticalSectionLock lock(m_listpWorkThreadCS);
	list<CValveWorkThread*>::iterator itBegin = m_listpWorkThread.begin();
	list<CValveWorkThread*>::iterator itEnd = m_listpWorkThread.end();
	for (list<CValveWorkThread*>::iterator it = itBegin; it != itEnd; ++it)
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

SC_PLUGIN_ERROR CValveWorkThreadMgr::Suspend(unsigned long* plResult)
{
	SC_PLUGIN_ERROR err = SC_PLUGIN_SUCCESS;

	*plResult = 0;
	CriticalSectionLock lock(m_listpWorkThreadCS);
	list<CValveWorkThread*>::iterator itBegin = m_listpWorkThread.begin();
	list<CValveWorkThread*>::iterator itEnd = m_listpWorkThread.end();
	for (list<CValveWorkThread*>::iterator it = itBegin; it != itEnd; ++it)
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

SC_PLUGIN_ERROR CValveWorkThreadMgr::Resume(unsigned long* plResult)
{
	SC_PLUGIN_ERROR err = SC_PLUGIN_SUCCESS;

	*plResult = 0;
	CriticalSectionLock lock(m_listpWorkThreadCS);
	list<CValveWorkThread*>::iterator itBegin = m_listpWorkThread.begin();
	list<CValveWorkThread*>::iterator itEnd = m_listpWorkThread.end();
	for (list<CValveWorkThread*>::iterator it = itBegin; it != itEnd; ++it)
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

SC_PLUGIN_ERROR CValveWorkThreadMgr::Execute(const TaskList& m_listspTask, 
	IScreenReader* pIScreenAreaReader, 
	IPluginExecuteCallback* pPluginExecuteCB, 
	int packageID, 
	const string& TimeStamp)
{
	SC_PLUGIN_ERROR err = SC_PLUGIN_SUCCESS;

	shared_ptr<CTaskListWaitingForWork> spTaskListForWork(new CTaskListWaitingForWork(m_listspTask));

	CriticalSectionLock lock(m_listpWorkThreadCS);
	list<CValveWorkThread*>::iterator itBegin = m_listpWorkThread.begin();
	list<CValveWorkThread*>::iterator itEnd = m_listpWorkThread.end();
	for (list<CValveWorkThread*>::iterator it = itBegin; it != itEnd; ++it)
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
