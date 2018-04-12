/////////////////////////////////////////////////////////////////////////////
/// @File name: ValveWorkThread.h
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
#include "ValveTask.h"

class CTaskListWaitingForWork
{
public:
	CTaskListWaitingForWork(const TaskList& listTask);
	~CTaskListWaitingForWork();
	void GetNextTask(bool* isLastTask, shared_ptr<CValveTask>& spTask);
private:
	CCriticalSection m_TaskListWaitingWorkCS;
	TaskList m_listspTaskWaitingForWork;
};

class CValveWorkThread : public CThreadBase
{
public:
	CValveWorkThread();
	virtual ~CValveWorkThread();

	virtual void Work(shared_ptr<CTaskListWaitingForWork> spTaskListForWork, IScreenReader* pIScreenAreaReader, IPluginExecuteCallback* pIPluginExecuteCB, int packageID, const string& TimeStamp);

protected:
	virtual DWORD Execute() override;

private:
	HANDLE m_hWorkEvent;
	shared_ptr<CTaskListWaitingForWork> m_spTaskListForWork;
	IScreenReader* m_pIScreenAreaReader;
	IPluginExecuteCallback* m_pIPluginExecuteCB;
	string m_TimeStamp;
	int m_packageID;
};

