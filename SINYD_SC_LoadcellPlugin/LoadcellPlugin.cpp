/////////////////////////////////////////////////////////////////////////////
/// @File name: LoadcellPlugin.cpp
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 		
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#include "LoadcellPlugin.h"

static const int thisCapability = SC_PLUGIN_CAPABILITY_TYPE_LOADCELL;

CLoadcellPlugin::CLoadcellPlugin()
	: m_listspTaskCS()
	, m_listspTask()
	, m_WorkThreadMgr()
{
}


CLoadcellPlugin::~CLoadcellPlugin()
{
}

SC_PLUGIN_ERROR CLoadcellPlugin::GetCapability(int* piCapability)
{
	if (piCapability == nullptr)
	{
		return SC_PLUGIN_NULLPTR;
	}

	*piCapability = thisCapability;
	return SC_PLUGIN_SUCCESS;
}
SC_PLUGIN_ERROR CLoadcellPlugin::Execute(IScreenReader* pIScreenAreaReader, IPluginExecuteCallback* pIPluginExecuteCB, int packageID, const string& TimeStamp)
{
	if (pIScreenAreaReader == nullptr || pIPluginExecuteCB == nullptr)
	{
		return SC_PLUGIN_NULLPTR;
	}

	CriticalSectionLock lock(m_listspTaskCS);
	SC_PLUGIN_ERROR err = SC_PLUGIN_SUCCESS;
	if (m_listspTask.size() == 0)
	{
		err = SC_PLUGIN_ZEROTASKCOUNT;
	}
	else
	{
		//pIPluginExecuteCB->IncreasePluginCount();		// ���Ϊ��PluginManager��һ������
		err = m_WorkThreadMgr.Execute(m_listspTask, pIScreenAreaReader, pIPluginExecuteCB, packageID, TimeStamp);
	}
	return err;
}
SC_PLUGIN_ERROR CLoadcellPlugin::Start()
{
	return m_WorkThreadMgr.Start();
}
SC_PLUGIN_ERROR CLoadcellPlugin::Stop()
{
	return m_WorkThreadMgr.Stop();
}
SC_PLUGIN_ERROR CLoadcellPlugin::Suspend(unsigned long* plResult)
{
	if (plResult == nullptr)
	{
		return SC_PLUGIN_NULLPTR;
	}

	return m_WorkThreadMgr.Suspend(plResult);
}
SC_PLUGIN_ERROR CLoadcellPlugin::Resume(unsigned long* plResult)
{
	if (plResult == nullptr)
	{
		return SC_PLUGIN_NULLPTR;
	}

	return m_WorkThreadMgr.Resume(plResult);
}
SC_PLUGIN_ERROR CLoadcellPlugin::AddTask(PTaskInfo pTaskInfo)
{
	if (pTaskInfo == nullptr)
	{
		return SC_PLUGIN_NULLPTR;
	}

	shared_ptr<CLoadcellTask> spTask(new CLoadcellTask(pTaskInfo));
	CriticalSectionLock lock(m_listspTaskCS);
	m_listspTask.push_back(spTask);

	return SC_PLUGIN_SUCCESS;
}
SC_PLUGIN_ERROR CLoadcellPlugin::RemoveTask(PTaskInfo pTaskInfo)
{
	if (pTaskInfo == nullptr)
	{
		return SC_PLUGIN_NULLPTR;
	}

	SC_PLUGIN_ERROR err = SC_PLUGIN_SUCCESS;

	bool bFound = false;
	CriticalSectionLock lock(m_listspTaskCS);
	TaskList::iterator itBegin = m_listspTask.begin();
	TaskList::iterator itEnd = m_listspTask.end();
	for (TaskList::iterator it = itBegin; it != itEnd; ++it)
	{
		if ((*it)->GetID() == pTaskInfo->id)
		{
			bFound = true;
			m_listspTask.erase(it);
			break;
		}
	}
	if (!bFound)
	{
		err = SC_PLUGIN_TASKNOTFOUND;
	}

	return err;
}
SC_PLUGIN_ERROR CLoadcellPlugin::GetTaskCount(int* piCount)
{
	if (piCount == nullptr)
	{
		return SC_PLUGIN_NULLPTR;

	}

	CriticalSectionLock lock(m_listspTaskCS);
	*piCount = m_listspTask.size();
	return SC_PLUGIN_SUCCESS;
}
SC_PLUGIN_ERROR CLoadcellPlugin::GetRunningTaskCount(int* piCount)
{
	if (piCount == nullptr)
	{
		return SC_PLUGIN_NULLPTR;

	}

	int iCount = 0;
	CriticalSectionLock lock(m_listspTaskCS);
	TaskList::iterator itBegin = m_listspTask.begin();
	TaskList::iterator itEnd = m_listspTask.end();
	for (TaskList::iterator it = itBegin; it != itEnd; ++it)
	{
		if ((*it)->IsRunning())
		{
			iCount++;
		}
	}
	*piCount = iCount;
	return SC_PLUGIN_SUCCESS;
}
