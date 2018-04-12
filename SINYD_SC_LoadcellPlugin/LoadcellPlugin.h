/////////////////////////////////////////////////////////////////////////////
/// @File name: LoadcellPlugin.h
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
#include "ISCPlugin.h"
#include "LoadcellTask.h"
#include "LoadcellWorkThreadMgr.h"


class CLoadcellPlugin : public IPlugin
{
public:
	CLoadcellPlugin();
	virtual ~CLoadcellPlugin();

	virtual SC_PLUGIN_ERROR GetCapability(int* piCapability) override;
	virtual SC_PLUGIN_ERROR Execute(IScreenReader* pIScreenAreaReader, IPluginExecuteCallback* pIPluginExecuteCB, int packageID, const string& TimeStamp) override;
	virtual SC_PLUGIN_ERROR Start() override;
	virtual SC_PLUGIN_ERROR Stop() override;
	virtual SC_PLUGIN_ERROR Suspend(unsigned long* plResult) override;
	virtual SC_PLUGIN_ERROR Resume(unsigned long* plResult) override;
	virtual SC_PLUGIN_ERROR AddTask(PTaskInfo pTaskInfo) override;
	virtual SC_PLUGIN_ERROR RemoveTask(PTaskInfo pTaskInfo) override;
	virtual SC_PLUGIN_ERROR GetTaskCount(int* piCount) override;
	virtual SC_PLUGIN_ERROR GetRunningTaskCount(int* piCount) override;

private:
	CCriticalSection m_listspTaskCS;
	TaskList m_listspTask;
	CLoadcellWorkThreadMgr m_WorkThreadMgr;
};

