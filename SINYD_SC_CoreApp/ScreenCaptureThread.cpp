/////////////////////////////////////////////////////////////////////////////
/// @File name: ScreenCaptureThread.cpp
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 		
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#include "ScreenCaptureThread.h"
#include "CollectionManager.h"
#include "ConfigInitializer.h"
#include "log.h"
#include "..\SINYD_DataTransfer\SCCustomDataTranser.h"
#include <map>
#include "..\SINYD_SC_AppManager\TRWData.h"

constexpr unsigned int TimerElapse = 500;
constexpr unsigned int TimerElapseShort = 100;

CScreenCaptureThread::CScreenCaptureThread(IScreenWriter* pSreenWriter)
	: m_pSreenWriter(pSreenWriter)
	, m_WindowController()
	, m_CaptureEvent(NULL)
	, m_CaptureTimer()
	, m_bHasChangeTimer(false)
	, m_saveThread()
	, m_TimerElapse(TimerElapse)
	, m_TimerElapseShort(TimerElapseShort)
	, m_bSavePNG(false)
	, m_PNGSaveThread()
	, m_lastWindowStatus(STATUS_UNKNOWN)
{
	int iHz = ConfiginitializerInstance::GetInstance()->SettingConfig()->GetCollFreq();
	if (iHz > 0)
	{
		m_TimerElapse = 1000 / iHz;		//1000毫秒除以每秒帧数，获得每帧时长
		m_TimerElapseShort = m_TimerElapse < TimerElapseShort ? m_TimerElapse : TimerElapseShort;
	}

	m_CaptureEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
	m_CaptureTimer.SetTimedEvent(this, &CScreenCaptureThread::SetCaptureEventWithTimer);
}


CScreenCaptureThread::~CScreenCaptureThread()
{
	if (m_CaptureEvent)
	{
		::CloseHandle(m_CaptureEvent);
		m_CaptureEvent = NULL;
	}
}

void CScreenCaptureThread::SetProcessID(unsigned long ulProcessID)
{
	m_WindowController.SetProcessID(ulProcessID);
}

#define ServiceConfigFile ".\\ConfigFile_RunTime\\AppManager.conf"

void CScreenCaptureThread::SetIsSaveCaliPNG(bool bSave)
{
	m_bSavePNG = bSave;
	CTRWData rw;
	bool bCreate = rw._CreateFile(ServiceConfigFile);
	rw._WriteProfileInt("AppState", "PNGCaptureState", m_bSavePNG?1:0, ServiceConfigFile);

	if (bCreate)
	{
		rw._WriteDebugForLog("\n\n\n1:StartCoreApp   ", ServiceConfigFile);
		rw._WriteDebugForLog("2:StopCoreApp   ", ServiceConfigFile);
		rw._WriteDebugForLog("3:RestartCoreApp   ", ServiceConfigFile);
		rw._WriteDebugForLog("4:CoreAppStartCaliPng   ", ServiceConfigFile);
		rw._WriteDebugForLog("5:CoreAppStopCaliPng   ", ServiceConfigFile);
	}
}

bool CScreenCaptureThread::GetIsSaveCaliPNG()
{
	CTRWData rw;
	int iRes = rw._GetProfileInt("AppState", "PNGCaptureState", 0, ServiceConfigFile);
	return iRes == 1 ? true : false;
}

DWORD CScreenCaptureThread::StartImpl(DWORD dwCreationFlags)
{
	m_saveThread.Start();
	DWORD dw = CThreadBase::StartImpl(dwCreationFlags);
	m_CaptureTimer.Start(m_TimerElapse);

	return dw;
}

void CScreenCaptureThread::StopImpl(int nWaitTime)
{
	m_CaptureTimer.Stop();
	CThreadBase::StopImpl(nWaitTime);
	m_saveThread.Stop();
}

DWORD CScreenCaptureThread::Execute()
{
	m_lastScreen.isSaved = false;

	SetIsSaveCaliPNG(GetIsSaveCaliPNG());

	int configScreenW = ConfiginitializerInstance::GetInstance()->CalibrationConfig()->GetWindowWidth();
	int configScreenH = ConfiginitializerInstance::GetInstance()->CalibrationConfig()->GetWindowHeight();
	constexpr DWORD dwHandleCount = 2;
	HANDLE handles[dwHandleCount] = { m_hStopEvent, m_CaptureEvent };
	while (true)
	{
		DWORD dw = ::WaitForMultipleObjects(dwHandleCount, handles, FALSE, INFINITE);
		if (WAIT_OBJECT_0 == dw)
		{
			break;
		}
		else if (WAIT_OBJECT_0 + 1 == dw)
		{
			if (m_WindowController.GetWindowHandle() == NULL)
			{
				SCINFO("CScreenCaptureThread GetWindowHandle NULL");
				m_WindowController.RefreshWindowHandle();
			}
			else
			{
				SCINFO("CScreenCaptureThread GetWindowHandle %x   Status %d", m_WindowController.GetWindowHandle(), m_WindowController.GetWindowStatus());
			}

			//check window status and do
			WINDOWSTATUS curStat = m_WindowController.GetWindowStatus();
			if (curStat == STATUS_MINIMIZE)
			{
				if (CollectionMgrInstance::GetInstance()->IsProducing())
				{
					SYSTEMTIME systime;
					GetLocalTime(&systime);
					char time[24] = { 0 };
					sprintf_s(time, "%04d-%02d-%02d %02d:%02d:%02d.%03d", systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds);

					//print desktop screen
					CBitmapEx bitmap;
					bool bPrint = m_WindowController.GetDesktopScreen(bitmap);
					if (bPrint)
					{
						m_saveThread.AddTask(bitmap, time, true);
					}

					//save and trans minimize status when app change to minimize
					if (m_lastWindowStatus != STATUS_MINIMIZE)
					{
						SaveMinimizeStatus(1);
					}
				}
			}
			else
			{
				if (CollectionMgrInstance::GetInstance()->IsProducing() && m_lastWindowStatus == STATUS_MINIMIZE)
				{
					SaveMinimizeStatus(2);
				}
			}
			m_lastWindowStatus = curStat;

			//capture screen and do
			HBITMAP hBitmap = m_WindowController.GetScreenShot();
			if (hBitmap)
			{
				SCINFO("CScreenCaptureThread GetScreenShot %x", hBitmap);
				if (m_pSreenWriter)
				{
					WINDOWRESOLUTION resolution = m_WindowController.GetWindowClientResolution();
					m_pSreenWriter->SetScreenRatio((float)resolution.width / (float)configScreenW, (float)resolution.height / (float)configScreenH);
					m_pSreenWriter->SetScreen(hBitmap);

					SYSTEMTIME systime;
					GetLocalTime(&systime);
					char time[24] = { 0 };
					sprintf_s(time, "%04d-%02d-%02d %02d:%02d:%02d.%03d", systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds);

					CollectionMgrInstance::GetInstance()->Collect(time);
					if (CollectionMgrInstance::GetInstance()->IsProducing())
					{
						SCINFO("CScreenCaptureThreadIsProducing true to save jpg");
						m_saveThread.AddTask(hBitmap, time);
						if (m_lastScreen.isSaved)
						{
							m_saveThread.AddTask(m_lastScreen.bitmap, m_lastScreen.time);
							m_lastScreen.isSaved = false;
							m_lastScreen.bitmap.Clear();
						}
					}
					else
					{
						SCINFO("CScreenCaptureThread IsProducing false not save jpg");
						m_lastScreen.bitmap.Load(hBitmap);
						m_lastScreen.time = time;
						m_lastScreen.isSaved = true;
					}

					if (m_bSavePNG)
					{
						if (!m_PNGSaveThread.IsRunning())
						{
							m_PNGSaveThread.Start();
						}
						m_PNGSaveThread.AddTask(hBitmap, time);
					}
					else
					{
						if (m_PNGSaveThread.IsRunning())
						{
							m_PNGSaveThread.Stop();
						}
					}
				}
			}
			else
			{
				SCINFO("CScreenCaptureThread GetScreenShot NULL");
			}
		}
		else
		{
			continue;
		}
	}

	return 0;
}

void CScreenCaptureThread::SetCaptureEventWithTimer()
{
	if (!CollectionMgrInstance::GetInstance()->IsCollecting())
	{
		::SetEvent(m_CaptureEvent);
		if (m_bHasChangeTimer)
		{
			m_CaptureTimer.Change(m_TimerElapse);
			m_bHasChangeTimer = false;
		}
	}
	else
	{
		if (!m_bHasChangeTimer)
		{
			m_CaptureTimer.Change(m_TimerElapseShort);
			m_bHasChangeTimer = true;
		}
	}
}

#define WINDOWSTATUS_LOG_PATH ".\\DB\\WINDOWSTATUS.log"
void CScreenCaptureThread::SaveMinimizeStatus(int stat)
{
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	char time[24] = { 0 };
	sprintf_s(time, "%04d-%02d-%02d %02d:%02d:%02d.%03d", systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds);

	FILE* file = fopen(WINDOWSTATUS_LOG_PATH, "a");
	if (file)
	{
		fprintf(file, "[%s][MINIMIZE][%d]\n", time, stat);
		fclose(file);
	}

	TransMinimizeStatus(stat, time);
}

void CScreenCaptureThread::TransMinimizeStatus(int stat, const string& time)
{
// Delete when update to flume trans begin
// 	const char* ip = ConfiginitializerInstance::GetInstance()->SettingConfig()->GetSocketIP();
// 	int port = ConfiginitializerInstance::GetInstance()->SettingConfig()->GetSoecketPort();
// 
// 	if (ip != NULL)
// 	{
// 		map<string, string> data;
// 		char cStat[10] = { 0 };
// 		sprintf(cStat, "%d", stat);
// 		data["value"] = cStat;
// 		data["time"] = time;
// 		CSCCustomDataTranser trans(ip, port);
// 		trans.TransData(4/*信息数据类型*/, 1/*窗口状态数据*/, data);
// 	}
// Delete when update to flume trans end

}




