/////////////////////////////////////////////////////////////////////////////
/// @File name: SCPluginManager.cpp
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 		
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#include "SCPluginManager.h"
#include "PluginInfo.h"
#include "PluginFactory.h"
#include "CommonDef.h"
#include "MeasureDef.h"



CPluginExecuteCallback::CPluginExecuteCallback()
	: m_iExecutePluginCount(0)
	, m_pIPluginMgrExecuteCallback(nullptr)
{
}

CPluginExecuteCallback::~CPluginExecuteCallback()
{
}

void CPluginExecuteCallback::ExecuteResult(const string& result)
{
	m_pIPluginMgrExecuteCallback->ExecuteResult(result);
}

// void CPluginExecuteCallback::IncreasePluginCount()
// {
// 	CriticalSectionLock lock(m_CountLock);
// 	m_iExecutePluginCount++;
// }

void CPluginExecuteCallback::DecreasePluginCount()
{
	CriticalSectionLock lock(m_CountLock);
	m_iExecutePluginCount--;
	if (m_iExecutePluginCount == 0)
	{
		m_pIPluginMgrExecuteCallback->ExecuteEndEvent();
	}
}

void CPluginExecuteCallback::SetExecutepluginCount(int count)
{
	CriticalSectionLock lock(m_CountLock);
	m_iExecutePluginCount = count;
}

void CPluginExecuteCallback::SetMgrCB(IPluginMgrExecuteCallback * pIPluginMgrExecuteCallbac)
{
	m_pIPluginMgrExecuteCallback = pIPluginMgrExecuteCallbac;
}

CSCPluginManager::CSCPluginManager()
	: m_bIsPluginsInited(false)
	, m_listPluginCS()
	, m_listPlugin()
	, m_PluginExecuteCB()
{
}


CSCPluginManager::~CSCPluginManager()
{
	CriticalSectionLock lock(m_listPluginCS);
	if (m_listPlugin.size() > 0)
	{
		UnInitPlugins();
	}
}

SC_PLUGIN_ERROR CSCPluginManager::InitPlugins(bool bForce)
{
	SC_PLUGIN_ERROR res = SC_PLUGIN_FAIL;

	if (m_bIsPluginsInited && !bForce)
	{
		res = SC_PLUGIN_SUCCESS;
	}
	else
	{
		CPluginInfoLoader infoLoader;
		//string filePath = CONFIG_BACKUP_PATH + string(ConfigFileName);
		int iRes = infoLoader.Load(CONFIG_BACKUP_PATH ConfigFileName);
		if (iRes == 0)
		{
			long lCount = infoLoader.GetPluginCount();

			CriticalSectionLock lock(m_listPluginCS);
			for (long l = 0; l < lCount; l++)
			{
				PluginInfo info;
				if (infoLoader.GetPluginInfo(l, &info))
				{
					IPlugin* pPlugin = nullptr;
					iRes = CPluginFactory::CreatePlugin(info, &pPlugin);
					if (iRes == 0 && pPlugin != nullptr)
					{
						m_listPlugin.push_back(pPlugin);
					}
				}
			}

			if (m_listPlugin.size() > 0)
			{
				m_bIsPluginsInited = true;
				res = SC_PLUGIN_SUCCESS;
			}
		}
	}

	return res;
}
SC_PLUGIN_ERROR CSCPluginManager::UnInitPlugins()
{
	SC_PLUGIN_ERROR err = SC_PLUGIN_FAIL;
	if (m_bIsPluginsInited)
	{
		CriticalSectionLock lock(m_listPluginCS);
		list<IPlugin*>::iterator itBegin = m_listPlugin.begin();
		list<IPlugin*>::iterator itEnd = m_listPlugin.end();
		for (list<IPlugin*>::iterator it = itBegin; it != itEnd; ++it)
		{
			CPluginFactory::ReleasePlugin(*it);
		}
		m_listPlugin.clear();
		m_bIsPluginsInited = false;
	}

	return SC_PLUGIN_ERROR();
}
SC_PLUGIN_ERROR CSCPluginManager::StartPlugins()
{
	SC_PLUGIN_ERROR err = SC_PLUGIN_SUCCESS;

	CriticalSectionLock lock(m_listPluginCS);
	if (m_listPlugin.size() > 0)
	{
		list<IPlugin*>::iterator itBegin = m_listPlugin.begin();
		list<IPlugin*>::iterator itEnd = m_listPlugin.end();
		for (list<IPlugin*>::iterator it = itBegin; it != itEnd; ++it)
		{
			if (*it != nullptr)
			{
				SC_PLUGIN_ERROR errPlugin = (*it)->Start();
				if (errPlugin != SC_PLUGIN_SUCCESS)
				{
					err = errPlugin;
				}
			}
		}

	}
	return err;
}
SC_PLUGIN_ERROR CSCPluginManager::StopPlugins()
{
	SC_PLUGIN_ERROR err = SC_PLUGIN_SUCCESS;

	CriticalSectionLock lock(m_listPluginCS);
	if (m_listPlugin.size() > 0)
	{
		list<IPlugin*>::iterator itBegin = m_listPlugin.begin();
		list<IPlugin*>::iterator itEnd = m_listPlugin.end();
		for (list<IPlugin*>::iterator it = itBegin; it != itEnd; ++it)
		{
			if (*it != nullptr)
			{
				SC_PLUGIN_ERROR errPlugin = (*it)->Stop();
				if (errPlugin != SC_PLUGIN_SUCCESS)
				{
					err = errPlugin;
				}
			}
		}

	}
	return err;
}
SC_PLUGIN_ERROR CSCPluginManager::ExecutePlugins(IScreenReader* pIScreenAreaReader, IPluginMgrExecuteCallback* pPluginMgrExecuteCallback, int packageID, const string& TimeStamp)
{

	if (pIScreenAreaReader == nullptr || pPluginMgrExecuteCallback == nullptr)
	{
		return SC_PLUGIN_NULLPTR;
	}

	SC_PLUGIN_ERROR err = SC_PLUGIN_SUCCESS;

	m_PluginExecuteCB.SetMgrCB(pPluginMgrExecuteCallback);

	CriticalSectionLock lock(m_listPluginCS);
	if (m_listPlugin.size() > 0)
	{
		m_PluginExecuteCB.SetExecutepluginCount(m_listPlugin.size());

		list<IPlugin*>::iterator itBegin = m_listPlugin.begin();
		list<IPlugin*>::iterator itEnd = m_listPlugin.end();
		for (list<IPlugin*>::iterator it = itBegin; it != itEnd; ++it)
		{
			int taskCount = 0;
			if (*it != nullptr && (*it)->GetTaskCount(&taskCount) == SC_PLUGIN_SUCCESS && taskCount > 0)
			{
				SC_PLUGIN_ERROR errPlugin = (*it)->Execute(pIScreenAreaReader, &m_PluginExecuteCB, packageID, TimeStamp);
				if (errPlugin != SC_PLUGIN_SUCCESS)
				{
					err = errPlugin;
				}
			}
			else
			{
				m_PluginExecuteCB.DecreasePluginCount();
			}
		}

	}
	return err;
}
SC_PLUGIN_ERROR CSCPluginManager::SuspendPlugins()
{
	SC_PLUGIN_ERROR err = SC_PLUGIN_SUCCESS;

	CriticalSectionLock lock(m_listPluginCS);
	if (m_listPlugin.size() > 0)
	{
		list<IPlugin*>::iterator itBegin = m_listPlugin.begin();
		list<IPlugin*>::iterator itEnd = m_listPlugin.end();
		for (list<IPlugin*>::iterator it = itBegin; it != itEnd; ++it)
		{
			if (*it != nullptr)
			{
				unsigned long lResult = 0;
				SC_PLUGIN_ERROR errPlugin = (*it)->Suspend(&lResult);
				if (errPlugin != SC_PLUGIN_SUCCESS)
				{
					err = errPlugin;
				}
			} 
		}

	}
	return err;
}
SC_PLUGIN_ERROR CSCPluginManager::ResumePlugins()
{
	SC_PLUGIN_ERROR err = SC_PLUGIN_SUCCESS;

	CriticalSectionLock lock(m_listPluginCS);
	if (m_listPlugin.size() > 0)
	{
		list<IPlugin*>::iterator itBegin = m_listPlugin.begin();
		list<IPlugin*>::iterator itEnd = m_listPlugin.end();
		for (list<IPlugin*>::iterator it = itBegin; it != itEnd; ++it)
		{
			if (*it != nullptr)
			{
				unsigned long lResult = 0;
				SC_PLUGIN_ERROR errPlugin = (*it)->Resume(&lResult);
				if (errPlugin == SC_PLUGIN_SUCCESS)
				{
					while (lResult > 0)
					{
						errPlugin = (*it)->Resume(&lResult);
					}
				}
				else
				{
					err = errPlugin;
				}
			}
		}

	}
	return err;
}
SC_PLUGIN_ERROR CSCPluginManager::AddTask(PTaskInfo pTaskInfo)
{
	if (pTaskInfo == nullptr)
	{
		return SC_PLUGIN_NULLPTR;
	}

	SC_PLUGIN_ERROR err = SC_PLUGIN_SUCCESS;

	CriticalSectionLock lock(m_listPluginCS);
	if (m_listPlugin.size() > 0)
	{
		list<IPlugin*>::iterator itBegin = m_listPlugin.begin();
		list<IPlugin*>::iterator itEnd = m_listPlugin.end();
		for (list<IPlugin*>::iterator it = itBegin; it != itEnd; ++it)
		{
			if (*it != nullptr)
			{
				int cap = 0;
				(*it)->GetCapability(&cap);
				if (pTaskInfo->type == cap)
				{
					err = (*it)->AddTask(pTaskInfo);
					break;
				}
			}
		}
	}

	return err;
}
SC_PLUGIN_ERROR CSCPluginManager::RemoveTask(PTaskInfo pTaskInfo)
{
	if (pTaskInfo == nullptr)
	{
		return SC_PLUGIN_NULLPTR;
	}

	SC_PLUGIN_ERROR err = SC_PLUGIN_SUCCESS;

	CriticalSectionLock lock(m_listPluginCS);
	if (m_listPlugin.size() > 0)
	{
		list<IPlugin*>::iterator itBegin = m_listPlugin.begin();
		list<IPlugin*>::iterator itEnd = m_listPlugin.end();
		for (list<IPlugin*>::iterator it = itBegin; it != itEnd; ++it)
		{
			if (*it != nullptr)
			{
				int cap = 0;
				(*it)->GetCapability(&cap);
				if (pTaskInfo->type == cap)
				{
					err = (*it)->RemoveTask(pTaskInfo);
					break;
				}
			}
		}
	}

	return err;
}
SC_PLUGIN_ERROR CSCPluginManager::GetTaskCount(int* piCount)
{
	if (piCount == nullptr)
	{
		return SC_PLUGIN_NULLPTR;
	}

	SC_PLUGIN_ERROR err = SC_PLUGIN_SUCCESS;

	CriticalSectionLock lock(m_listPluginCS);
	if (m_listPlugin.size() > 0)
	{
		*piCount = 0;
		list<IPlugin*>::iterator itBegin = m_listPlugin.begin();
		list<IPlugin*>::iterator itEnd = m_listPlugin.end();
		for (list<IPlugin*>::iterator it = itBegin; it != itEnd; ++it)
		{
			if (*it != nullptr)
			{
				int count = 0;
				SC_PLUGIN_ERROR errPlugin = (*it)->GetTaskCount(&count);
				if (errPlugin != SC_PLUGIN_SUCCESS)
				{
					err = errPlugin;
				}
				else
				{
					*piCount = *piCount + count;
				}
			}
		}

	}
	return err;
}
SC_PLUGIN_ERROR CSCPluginManager::GetRunningTaskCount(int* piCount)
{
	if (piCount == nullptr)
	{
		return SC_PLUGIN_NULLPTR;
	}

	SC_PLUGIN_ERROR err = SC_PLUGIN_SUCCESS;

	CriticalSectionLock lock(m_listPluginCS);
	if (m_listPlugin.size() > 0)
	{
		*piCount = 0;
		list<IPlugin*>::iterator itBegin = m_listPlugin.begin();
		list<IPlugin*>::iterator itEnd = m_listPlugin.end();
		for (list<IPlugin*>::iterator it = itBegin; it != itEnd; ++it)
		{
			if (*it != nullptr)
			{
				int count = 0;
				SC_PLUGIN_ERROR errPlugin = (*it)->GetRunningTaskCount(&count);
				if (errPlugin != SC_PLUGIN_SUCCESS)
				{
					err = errPlugin;
				}
				else
				{
					*piCount = *piCount + count;
				}
			}
		}
	}
	return err;
}

