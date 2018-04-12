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

struct IBitmapSaveTask
{
	virtual void Execute() = 0;
};

class CBitmapSaveThread : public CThreadBase
{
public:
	CBitmapSaveThread();
	virtual ~CBitmapSaveThread();

	///*******************************************************
	/// @name:   CBitmapSaveThread::AddTask
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][const HBITMAP &]hbitmap
	/// @param:  [in][const string &]time
	/// @brief:  添加截图保存任务
	///*******************************************************
	void AddTask(const HBITMAP& hbitmap, const string& time, bool bSaveFull = false);
	void AddTask(CBitmapEx & hbitmap, const string& time, bool bSaveFull = false);
	///*******************************************************
	/// @name:   CBitmapSaveThread::AddTimeCleanTask
	/// @author: YaoDi
	/// @return: void
	/// @brief:  添加保存时间清理任务
	///*******************************************************
	void AddTimeCleanTask();
	///*******************************************************
	/// @name:   CBitmapSaveThread::AddDiskCleanTask
	/// @author: YaoDi
	/// @return: void
	/// @brief:  添加磁盘占用空间清理任务
	///*******************************************************
	void AddDiskCleanTask();

protected:
	virtual DWORD Execute() override;
	virtual DWORD StartImpl(DWORD dwCreationFlags) override;
	virtual void StopImpl(int nWaitTime) override;

private:
	void PushTask(shared_ptr<IBitmapSaveTask> spTask);
	void PopTask(list<shared_ptr<IBitmapSaveTask>>& taskList);
	void CleanTimerFunc();

private:
	HANDLE m_hTaskEvent;
	CCriticalSection m_lisTaskCS;
	list<shared_ptr<IBitmapSaveTask>> m_lisTask;
	TTimer<CBitmapSaveThread> m_CleanTaskTimer;
};

