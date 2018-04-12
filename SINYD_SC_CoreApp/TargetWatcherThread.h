/////////////////////////////////////////////////////////////////////////////
/// @File name: TargetWatcherThread.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 		目标进程启动退出监视线程
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "ThreadUtil.h"
#include <wbemcli.h>
#include <atlstr.h>
#include "TemplateTimer.h"

class CTargetWatcherThread : public CThreadBase
{
public:
	CTargetWatcherThread();
	virtual ~CTargetWatcherThread();

protected:
	virtual DWORD Execute() override;
	virtual DWORD StartImpl(DWORD dwCreationFlags);
	virtual void StopImpl(int nWaitTime);

private:
	bool SearchTargetProcessInAllStartedProcess(DWORD* dwProcessID);

	void GetProcessFilePath(IN HANDLE hProcess, CString & sFilePath);

	void GetProcessPath(DWORD processID, CString & str);
	
	bool ProcessCreationCallback(IWbemClassObject* ptr);
	bool ProcessTerminationCallback(IWbemClassObject* ptr);

	void SearchProcessTimerFunc();

private:
	DWORD m_dwTargetProcessID;
	HANDLE m_hTargetCreateEvent;
	HANDLE m_hTargetTerminateEvent;
	HANDLE m_hSearchprcessEvent;
	TTimer<CTargetWatcherThread> m_SearchProcessTimer;
};

