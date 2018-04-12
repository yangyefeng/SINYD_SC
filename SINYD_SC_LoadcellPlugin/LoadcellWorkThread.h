/////////////////////////////////////////////////////////////////////////////
/// @File name: LoadcellWorkThread.h
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
#include "ThreadUtil.h"
#include "LoadcellTask.h"

class CTaskListWaitingForWork
{
public:
	CTaskListWaitingForWork(const TaskList& listTask);
	~CTaskListWaitingForWork();
	void GetNextTask(bool* isLastTask, shared_ptr<CLoadcellTask>& spTask);
private:
	CCriticalSection m_TaskListWaitingWorkCS;
	TaskList m_listspTaskWaitingForWork;
};

class CLoadcellWorkThread : public CThreadBase
{
public:
	CLoadcellWorkThread();
	virtual ~CLoadcellWorkThread();

	virtual void Work(shared_ptr<CTaskListWaitingForWork> spTaskListForWork, IScreenReader* pIScreenAreaReader, IPluginExecuteCallback* pPluginExecuteCB, int packageID, const string& TimeStamp);

protected:
	virtual DWORD Execute() override;

private:
	HANDLE m_hWorkEvent;
	shared_ptr<CTaskListWaitingForWork> m_spTaskListForWork;
	IScreenReader* m_pIScreenAreaReader;
	IPluginExecuteCallback* m_pPluginExecuteCB;
	string m_TimeStamp;
	int m_packageID;
};

