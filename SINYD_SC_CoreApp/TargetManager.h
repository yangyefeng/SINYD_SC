/////////////////////////////////////////////////////////////////////////////
/// @File name: TargetManager.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 		目标进程状态管理
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ThreadUtil.h"
#include "TargetWatcherThread.h"


class CTargetManager
{
	DECLARE_SINGLETON_CLASS(CTargetManager);

private:
	CTargetManager(const CTargetManager&);
	CTargetManager& operator=(const CTargetManager&);

	CTargetManager();
	virtual ~CTargetManager();

protected:
	int Run();
	int Stop();

private:
	CTargetWatcherThread m_WatcherThread;
};
typedef CSingleton<CTargetManager> TargetMgrInstance;

