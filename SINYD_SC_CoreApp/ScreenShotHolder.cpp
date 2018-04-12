/////////////////////////////////////////////////////////////////////////////
/// @File name: ScreenShotHolder.cpp
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 		
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#include "ScreenShotHolder.h"



CScreenShotHolder::CScreenShotHolder()
	: m_BitmapHolder()
	, m_Capturethread(&m_BitmapHolder)
{
}


CScreenShotHolder::~CScreenShotHolder()
{
	this->Stop();
}

void CScreenShotHolder::Run(unsigned long ulProcessID)
{
	m_Capturethread.SetProcessID(ulProcessID);
	m_Capturethread.Start();
}

void CScreenShotHolder::Suspend()
{
	m_Capturethread.Suspend();
}

void CScreenShotHolder::Resume()
{
	m_Capturethread.Resume();
}

void CScreenShotHolder::Stop()
{
	m_Capturethread.Stop();
	//m_Capturethread.SetProcessID(0);
}

IScreenReader * CScreenShotHolder::GetScreenReader()
{
	return &m_BitmapHolder;
}

void CScreenShotHolder::SetIsSaveCaliPNG(bool bSave)
{
	m_Capturethread.SetIsSaveCaliPNG(bSave);
}
