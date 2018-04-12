/////////////////////////////////////////////////////////////////////////////
/// @File name: SCPluginManager.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 		
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "ISCPluginMgr.h"
#include "ISCPlugin.h"
#include "ThreadUtil.h"
#include <list>

using namespace std;


class CPluginExecuteCallback : public IPluginExecuteCallback
{
public:
	CPluginExecuteCallback();
	~CPluginExecuteCallback();

	virtual void ExecuteResult(const string& result) override;
	//virtual void IncreasePluginCount() override;
	virtual void DecreasePluginCount() override;
	virtual void SetExecutepluginCount(int count);

	void SetMgrCB(IPluginMgrExecuteCallback* pIPluginMgrExecuteCallbac);

private:
	CCriticalSection m_CountLock;
	int m_iExecutePluginCount;
	IPluginMgrExecuteCallback* m_pIPluginMgrExecuteCallback;
};



class CSCPluginManager : public IPluginManager
{
public:
	CSCPluginManager();
	virtual ~CSCPluginManager();

	virtual SC_PLUGIN_ERROR InitPlugins(bool bForce = false) override;
	virtual SC_PLUGIN_ERROR UnInitPlugins() override;
	virtual SC_PLUGIN_ERROR StartPlugins() override;
	virtual SC_PLUGIN_ERROR StopPlugins() override;
	virtual SC_PLUGIN_ERROR ExecutePlugins(IScreenReader* pIScreenAreaReader, IPluginMgrExecuteCallback* pPluginMgrExecuteCallback, int packageID, const string& TimeStamp) override;
	virtual SC_PLUGIN_ERROR SuspendPlugins() override;
	virtual SC_PLUGIN_ERROR ResumePlugins() override;
	virtual SC_PLUGIN_ERROR AddTask(PTaskInfo pTaskInfo) override;
	virtual SC_PLUGIN_ERROR RemoveTask(PTaskInfo pTaskInfo) override;
	virtual SC_PLUGIN_ERROR GetTaskCount(int* piCount) override;
	virtual SC_PLUGIN_ERROR GetRunningTaskCount(int* piCount) override;

private:
	bool m_bIsPluginsInited;
	CCriticalSection m_listPluginCS;
	list<IPlugin*> m_listPlugin;
	CPluginExecuteCallback m_PluginExecuteCB;
};

