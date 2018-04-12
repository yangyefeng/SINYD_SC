/////////////////////////////////////////////////////////////////////////////
/// @File name: ScreenCaptureThread.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 		Thread for capture target window screen
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "ThreadUtil.h"
#include "IScreenWriter.h"
#include "ScreenWindowController.h"
#include "TemplateTimer.h"
#include "BitmapSaveThread.h"
#include "BitmapSaveThread_Cali.h"

class CScreenCaptureThread : public CThreadBase
{
public:
	CScreenCaptureThread(IScreenWriter* pSreenWriter);
	virtual ~CScreenCaptureThread();

	///*******************************************************
	/// @name:   CScreenCaptureThread::SetProcessID
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][unsigned long]ulProcessID
	/// @brief:  设置截图线程的目标进程ID
	///*******************************************************
	void SetProcessID(unsigned long ulProcessID);
	void SetIsSaveCaliPNG(bool bSave);
	bool GetIsSaveCaliPNG();

protected:
	virtual DWORD Execute() override;
	virtual DWORD StartImpl(DWORD dwCreationFlags) override;
	virtual void StopImpl(int nWaitTime) override;

private:
	void SetCaptureEventWithTimer();
	void SaveMinimizeStatus(int stat);
	void TransMinimizeStatus(int stat, const string& time);

private:
	IScreenWriter* m_pSreenWriter;
	CScreenWindowController m_WindowController;
	HANDLE m_CaptureEvent;
	TTimer<CScreenCaptureThread> m_CaptureTimer;
	bool m_bHasChangeTimer;
	CBitmapSaveThread m_saveThread;
	int m_TimerElapse;
	int m_TimerElapseShort;

	struct LastScreenShot
	{
		CBitmapEx bitmap;
		string time;
		bool isSaved;
	};
	LastScreenShot m_lastScreen;

	bool m_bSavePNG;
	CBitmapSaveThread_Cali m_PNGSaveThread;
	WINDOWSTATUS m_lastWindowStatus;
};

