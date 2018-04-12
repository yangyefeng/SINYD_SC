/////////////////////////////////////////////////////////////////////////////
/// @File name: ValveWorkThread.cpp
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 		
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#include "ValveWorkThread.h"

CTaskListWaitingForWork::CTaskListWaitingForWork(const TaskList& listTask)
	: m_listspTaskWaitingForWork(listTask)
{
}

CTaskListWaitingForWork::~CTaskListWaitingForWork()
{
	CriticalSectionLock lock(m_TaskListWaitingWorkCS);
	m_listspTaskWaitingForWork.clear();
}

void CTaskListWaitingForWork::GetNextTask(bool* isLastTask, shared_ptr<CValveTask>& spTask)
{
	CriticalSectionLock lock(m_TaskListWaitingWorkCS);

/*	shared_ptr<CValveTask> spTask(nullptr);*/
	if (m_listspTaskWaitingForWork.size() > 0)
	{
		spTask = m_listspTaskWaitingForWork.front();
		m_listspTaskWaitingForWork.pop_front();
		if (m_listspTaskWaitingForWork.size() == 0)
		{
			*isLastTask = true;
		}
		else
		{
			*isLastTask = false;
		}
	}
	else
	{
		spTask = nullptr;
	}

}

CValveWorkThread::CValveWorkThread()
	: m_hWorkEvent(nullptr)
	, m_spTaskListForWork(nullptr)
	, m_pIScreenAreaReader(nullptr)
	, m_pIPluginExecuteCB(nullptr)
{
	m_hWorkEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
}


CValveWorkThread::~CValveWorkThread()
{
	if (m_hWorkEvent)
	{
		::CloseHandle(m_hWorkEvent);
		m_hWorkEvent = nullptr;
	}
}

DWORD CValveWorkThread::Execute()
{
	constexpr DWORD dwHandleCount = 2;
	HANDLE handles[dwHandleCount] = { m_hStopEvent, m_hWorkEvent };
	while (true)
	{
		DWORD dw = ::WaitForMultipleObjects(dwHandleCount, handles, FALSE, INFINITE);
		if (WAIT_OBJECT_0 == dw)
		{
			break;
		}
		else if (WAIT_OBJECT_0 + 1 == dw)
		{
			bool isLastTask = false;
			shared_ptr<CValveTask> spTask;
			m_spTaskListForWork->GetNextTask(&isLastTask, spTask);
			while (spTask != nullptr)
			{
				spTask->Execute(m_pIScreenAreaReader, m_pIPluginExecuteCB, m_packageID, m_TimeStamp);
				if (isLastTask)
				{
					if (m_pIPluginExecuteCB)
					{
						m_pIPluginExecuteCB->DecreasePluginCount();
					}
				}
				m_spTaskListForWork->GetNextTask(&isLastTask, spTask);
			}
		}
		else
		{
			continue;
		}
	}

	return 0;
}

void CValveWorkThread::Work(shared_ptr<CTaskListWaitingForWork> spTaskListForWork, IScreenReader* pIScreenAreaReader, IPluginExecuteCallback* pIPluginExecuteCB, int packageID, const string& TimeStamp)
{
	m_spTaskListForWork = spTaskListForWork;
	m_pIScreenAreaReader = pIScreenAreaReader;
	m_pIPluginExecuteCB = pIPluginExecuteCB;
	m_TimeStamp = TimeStamp;
	m_packageID = packageID;
	::SetEvent(m_hWorkEvent);
}


