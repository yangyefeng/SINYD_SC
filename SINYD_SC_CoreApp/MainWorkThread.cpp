#include "MainWorkThread.h"
#include "ConfigInitializer.h"
#include "CollectionManager.h"
#include "ScreenShotHolder.h"
#include "TargetManager.h"
#include "..\SINYD_SC_Register\SCRegisteTool.h"
#include <map>
#include <string>
#include "..\SINYD_DataTransfer\SCCustomDataTranser.h"
#include "TRWData.h"
#include "MeasureDef.h"


CMainWorkThread::CMainWorkThread()
{
	m_hRestartEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
}


CMainWorkThread::~CMainWorkThread()
{
	if (m_hRestartEvent)
	{
		::CloseHandle(m_hRestartEvent);
		m_hRestartEvent = NULL;
	}
}

void CMainWorkThread::Restart()
{
	::SetEvent(m_hRestartEvent);
}

void CMainWorkThread::SetIsSaveCaliPNG(bool bSave)
{
	ScreenShotHolderInstance::GetInstance()->SetIsSaveCaliPNG(bSave);
}

void CMainWorkThread::SetCurrentDirectoryToInstallDir()
{
	char fileFullPath[MAX_PATH] = "";
	::GetModuleFileNameA(::GetModuleHandle(NULL), fileFullPath, MAX_PATH);
	std::string strRes(fileFullPath);
	strRes = strRes.erase(strRes.find_last_of('\\'));

	::SetCurrentDirectoryA(strRes.c_str());
}

#define AppExeName ("SINYD_SC_CoreApp.exe")
#define AppWindowClassName _T("SINYD_SC_COREAPP")
#define AppWindowTitle _T("SINYD_SC_CoreApp")
#define MY_WM_STOP (WM_USER + 101)
bool CMainWorkThread::StopCoreAppAndCloseAutoStart(int stat)
{
	CSCRegisteTool regTool;
	regTool.CloseAppAutoStart();

	SYSTEMTIME systime;
	GetLocalTime(&systime);
	char time[24] = { 0 };
	sprintf_s(time, "%04d-%02d-%02d %02d:%02d:%02d.%03d", systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds);

// Delete when update to flume trans begin
// 	const char* ip = ConfiginitializerInstance::GetInstance()->SettingConfig()->GetSocketIP();
// 	int port = ConfiginitializerInstance::GetInstance()->SettingConfig()->GetSoecketPort();
// 
// 	if (ip != NULL)
// 	{
// 		map<string, string> data;
// 		char cStat[10] = { 0 };
// 		if (stat == UnRegisted)
// 		{
// 			stat = 1; //协议中定义的未注册数值
// 		}
// 		else if (stat == OutTime)
// 		{
// 			stat = 2; //协议中定义的已过期数值
// 		}
// 		else
// 		{
// 			stat = 0;
// 		}
// 		sprintf(cStat, "%d", stat);
// 		data["value"] = cStat;
// 		data["time"] = time;
// 
// 
// 		CTRWData rw;
// 
// 		string mixpath = CONFIG_BASE_PATH CONFIG_SYS_FILE;
// 		SourceInfo src;
// 		src.mix_id = rw._GetProfileInt("mix_machine_1", "mix_machine_device_id", 0, mixpath.c_str());
// 		src.mix_type = rw._GetProfileInt("mix_machine_1", "mix_machine_type", 0, mixpath.c_str());
// 		src.source_type = 1;	//界面采集
// 
// 		CSCCustomDataTranser trans(ip, port);
// 		trans.Init(src);
// 		trans.TransData(4/*信息数据类型*/, 2/*注册状态数据*/, data);
// 	}
// Delete when update to flume trans end

	bool bRes = false;
	HWND hWnd = ::FindWindow(AppWindowClassName, AppWindowTitle);
	if (hWnd != NULL)
	{
		PostMessage(hWnd, MY_WM_STOP, 0, 0);
		bRes = true;
	}

	FILE* file = fopen("SOFTREGERR", "a");
	if (file)
	{
		fprintf(file, "[%s %s]Software is not registed!\n", __DATE__, __TIME__);
		fclose(file);
	}

	return bRes;
}

DWORD CMainWorkThread::Execute()
{
	SetCurrentDirectoryToInstallDir();

	CSCRegisteTool regTool;
	VERIFIER_STATUS stat = regTool.GetSoftRegistStatus();
	if (stat != HasRegisted)
	{
		StopCoreAppAndCloseAutoStart(stat);
		return FALSE;
	}

	constexpr DWORD dwHandleCount = 2;
	HANDLE handles[dwHandleCount] = { m_hStopEvent, m_hRestartEvent };
	bool bContinue = true;
	while (bContinue)
	{
		ConfiginitializerInstance::GetInstance();
		CollectionMgrInstance::GetInstance();
		ScreenShotHolderInstance::GetInstance();
		TargetMgrInstance::GetInstance();

		DWORD dw = ::WaitForMultipleObjects(dwHandleCount, handles, FALSE, 1000 * 60 * 60 *24);		//Everyday check regist status
		if (WAIT_OBJECT_0 == dw)	//Stop Thread
		{
			bContinue = false;
		}
		else if (WAIT_OBJECT_0 + 1 == dw)		//Restart All Instance
		{
			//when restart , check regist status
			CSCRegisteTool regTool;
			VERIFIER_STATUS status = regTool.GetSoftRegistStatus();
			if (status == HasRegisted)
			{
				bContinue = true;
			}
			else if (status == UnRegisted)
			{
				StopCoreAppAndCloseAutoStart(status);
				bContinue = false;
			}
			else if (status == OutTime)
			{
				StopCoreAppAndCloseAutoStart(status);
				bContinue = false;
			}
			//bContinue = true;
		}
		else if (WAIT_TIMEOUT == dw)	//timeout to check regist status
		{
			CSCRegisteTool regTool;
			VERIFIER_STATUS status = regTool.GetSoftRegistStatus();
			if (status == HasRegisted)
			{
				bContinue = true;
			}
			else if (status == UnRegisted)
			{
				StopCoreAppAndCloseAutoStart(status);
				bContinue = false;
			}
			else if(status == OutTime)
			{
				StopCoreAppAndCloseAutoStart(status);
				bContinue = false;
			}
			continue;
		}
		else
		{
			bContinue = true;
			continue;
		}

		TargetMgrInstance::ReleaseInstance();
		ScreenShotHolderInstance::ReleaseInstance();
		CollectionMgrInstance::ReleaseInstance();
		ConfiginitializerInstance::ReleaseInstance();
	}

	return 0;
}
