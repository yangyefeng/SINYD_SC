/////////////////////////////////////////////////////////////////////////////
/// @File name: TargetManager.cpp
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 		
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#include "TargetManager.h"



CTargetManager::CTargetManager()
{
	this->Run();
}


CTargetManager::~CTargetManager()
{
	this->Stop();
}

int CTargetManager::Run()
{
	m_WatcherThread.Start();
	return 0;
}

int CTargetManager::Stop()
{
	m_WatcherThread.Stop();
	return 0;
}
