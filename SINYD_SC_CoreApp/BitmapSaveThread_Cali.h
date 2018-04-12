/////////////////////////////////////////////////////////////////////////////
/// @File name: BitmapSaveThread.h
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
#include <list>
#include "..\SINYD_SC_CommonUtility\BitmapEx.h"
#include <memory>
#include "TemplateTimer.h"
using namespace std;

struct IBitmapSaveTask_Cali
{
	virtual void Execute() = 0;
};

class CBitmapSaveThread_Cali : public CThreadBase
{
public:
	CBitmapSaveThread_Cali();
	virtual ~CBitmapSaveThread_Cali();

	void AddTask(const HBITMAP& hbitmap, const string& time);
	void AddDiskCheckTask();
	void ClearTask();

protected:
	virtual DWORD Execute() override;
	virtual DWORD StartImpl(DWORD dwCreationFlags) override;
	virtual void StopImpl(int nWaitTime) override;

private:
	void PushTask(shared_ptr<IBitmapSaveTask_Cali> spTask);
	void PopTask(list<shared_ptr<IBitmapSaveTask_Cali>>& taskList);
	void CleanTimerFunc();

private:
	HANDLE m_hTaskEvent;
	CCriticalSection m_lisTaskCS;
	list<shared_ptr<IBitmapSaveTask_Cali>> m_lisTask;
	TTimer<CBitmapSaveThread_Cali> m_CleanTaskTimer;
};

