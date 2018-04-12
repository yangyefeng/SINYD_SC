/////////////////////////////////////////////////////////////////////////////
/// @File name: LoadcellWorkThreadMgr.h
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
#include "LoadcellWorkThread.h"
#include "SCPluginDef.h"
#include "LoadcellTask.h"
#include <list>

using namespace std;
class CLoadcellWorkThreadMgr
{
public:
	CLoadcellWorkThreadMgr();
	virtual ~CLoadcellWorkThreadMgr();
	SC_PLUGIN_ERROR Start();
	SC_PLUGIN_ERROR Stop();
	SC_PLUGIN_ERROR Suspend(unsigned long* plResult);
	SC_PLUGIN_ERROR Resume(unsigned long* plResult);
	SC_PLUGIN_ERROR Execute(const TaskList& m_listspTask, IScreenReader* pIScreenAreaReader, IPluginExecuteCallback* pPluginExecuteCB, int packageID, const string& TimeStamp);
private:
	CCriticalSection m_listpWorkThreadCS;
	list<CLoadcellWorkThread*> m_listpWorkThread;
};

