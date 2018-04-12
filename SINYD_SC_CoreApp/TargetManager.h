/////////////////////////////////////////////////////////////////////////////
/// @File name: TargetManager.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 		Ŀ�����״̬����
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

