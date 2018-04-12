/////////////////////////////////////////////////////////////////////////////
/// @File name: ScreenShotHolder.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 		截图持有对象
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ThreadUtil.h"
#include "ScreenBitmaphHolder.h"
#include "IScreenReader.h"
#include "ScreenCaptureThread.h"


class CScreenShotHolder
{
	DECLARE_SINGLETON_CLASS(CScreenShotHolder);

private:
	CScreenShotHolder(const CScreenShotHolder&);
	CScreenShotHolder& operator=(const CScreenShotHolder&);

	CScreenShotHolder();
	virtual ~CScreenShotHolder();

public:
	///*******************************************************
	/// @name:   CScreenShotHolder::Run
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][unsigned long]ulProcessID
	/// @brief:  启动截图线程
	///*******************************************************
	void Run(unsigned long ulProcessID);
	///*******************************************************
	/// @name:   CScreenShotHolder::Suspend
	/// @author: YaoDi
	/// @return: void
	/// @brief:  挂起截图线程
	///*******************************************************
	void Suspend();
	///*******************************************************
	/// @name:   CScreenShotHolder::Resume
	/// @author: YaoDi
	/// @return: void
	/// @brief:  恢复截图线程
	///*******************************************************
	void Resume();
	///*******************************************************
	/// @name:   CScreenShotHolder::Stop
	/// @author: YaoDi
	/// @return: void
	/// @brief:  停止截图线程
	///*******************************************************
	void Stop();

	///*******************************************************
	/// @name:   CScreenShotHolder::GetScreenReader
	/// @author: YaoDi
	/// @return: IScreenReader*
	/// @brief:  获取截图数据读取对象
	///*******************************************************
	IScreenReader* GetScreenReader();

	void SetIsSaveCaliPNG(bool bSave);

private:
	CScreenBitmaphHolder m_BitmapHolder;
	CScreenCaptureThread m_Capturethread;
};
typedef CSingleton<CScreenShotHolder> ScreenShotHolderInstance;

