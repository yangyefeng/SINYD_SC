/////////////////////////////////////////////////////////////////////////////
/// @File name: ISCPluginMgr.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 	Plugin Manager interface include
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "SCPluginDef.h"
#include "IScreenReader.h"
#include "ISCPlugin.h"

class IPluginMgrExecuteCallback
{
public:
	virtual ~IPluginMgrExecuteCallback(){}
	virtual void ExecuteResult(const string& result) = 0;
	virtual void ExecuteEndEvent() = 0;
};

class IPluginManager
{
public:
	virtual ~IPluginManager(){}
	virtual SC_PLUGIN_ERROR InitPlugins(bool bForce = false) = 0;
	virtual SC_PLUGIN_ERROR UnInitPlugins() = 0;
	virtual SC_PLUGIN_ERROR ExecutePlugins(IScreenReader* pIScreenAreaReader, IPluginMgrExecuteCallback* pPluginMgrExecuteCallback, int packageID, const string& TimeStamp) = 0;
	virtual SC_PLUGIN_ERROR StartPlugins() = 0;
	virtual SC_PLUGIN_ERROR StopPlugins() = 0;
	virtual SC_PLUGIN_ERROR SuspendPlugins() = 0;
	virtual SC_PLUGIN_ERROR ResumePlugins() = 0;
	virtual SC_PLUGIN_ERROR AddTask(PTaskInfo pTaskInfo) = 0;
	virtual SC_PLUGIN_ERROR RemoveTask(PTaskInfo pTaskInfo) = 0;
	virtual SC_PLUGIN_ERROR GetTaskCount(int* piCount) = 0;
	virtual SC_PLUGIN_ERROR GetRunningTaskCount(int* piCount) = 0;
};
