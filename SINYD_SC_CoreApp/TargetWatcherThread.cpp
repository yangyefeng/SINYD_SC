/////////////////////////////////////////////////////////////////////////////
/// @File name: TargetWatcherThread.cpp
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 		
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#include "TargetWatcherThread.h"
#include "CollectionManager.h"
#include "ScreenShotHolder.h"
#include "ProcessNotification.h"
#include <windows.h>
#include <atlcomcli.h>
#include <TlHelp32.h>
#include "ConfigInitializer.h"
#include <atlstr.h>
#include <Psapi.h>
#include "log.h"

#pragma comment(lib, "Psapi.lib")

CTargetWatcherThread::CTargetWatcherThread()
	: m_dwTargetProcessID(0)
	, m_hTargetCreateEvent(nullptr)
	, m_hTargetTerminateEvent(nullptr)
	, m_hSearchprcessEvent(nullptr)
{
	m_hTargetCreateEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
	m_hTargetTerminateEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);

	m_hSearchprcessEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
	m_SearchProcessTimer.SetTimedEvent(this, &CTargetWatcherThread::SearchProcessTimerFunc);
}


CTargetWatcherThread::~CTargetWatcherThread()
{
	if (m_hTargetCreateEvent)
	{
		::CloseHandle(m_hTargetCreateEvent);
		m_hTargetCreateEvent = nullptr;
	}
	if (m_hTargetTerminateEvent)
	{
		::CloseHandle(m_hTargetTerminateEvent);
		m_hTargetTerminateEvent = nullptr;
	}
}

int EnableDebugPriv(const TCHAR * name)
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tp;
	LUID luid;
	//打开进程令牌环
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	//获得进程本地唯一ID
	LookupPrivilegeValue(NULL, name, &luid);

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	tp.Privileges[0].Luid = luid;
	//调整权限
	AdjustTokenPrivileges(hToken, 0, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
	return 0;
}

//*****************************************************************************************************************************

BOOL InjectDll(const char *DllFullPath, const DWORD dwRemoteProcessId)
{
	HANDLE hRemoteProcess;
	EnableDebugPriv(SE_DEBUG_NAME);
	//打开远程线程
	hRemoteProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwRemoteProcessId);

	char *pszLibFileRemote;

	//使用VirtualAllocEx函数在远程进程的内存地址空间分配DLL文件名空间
	pszLibFileRemote = (char *)VirtualAllocEx(hRemoteProcess, NULL, lstrlenA(DllFullPath) + 1, MEM_COMMIT, PAGE_READWRITE);


	//使用WriteProcessMemory函数将DLL的路径名写入到远程进程的内存空间
	BOOL bWrite = WriteProcessMemory(hRemoteProcess, pszLibFileRemote, (void *)DllFullPath, lstrlenA(DllFullPath) + 1, NULL);
	if (!bWrite)
	{
		return FALSE;
	}

	//计算LoadLibraryA的入口地址
	PTHREAD_START_ROUTINE pfnStartAddr = (PTHREAD_START_ROUTINE)
		GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryA");
	//(关于GetModuleHandle函数和GetProcAddress函数)

	//启动远程线程LoadLibraryA，通过远程线程调用创建新的线程
	HANDLE hRemoteThread;
	if ((hRemoteThread = CreateRemoteThread(hRemoteProcess, NULL, 0, pfnStartAddr, pszLibFileRemote, 0, NULL)) == NULL)
	{
		return FALSE;
	}

	// 释放句柄

	CloseHandle(hRemoteProcess);
	CloseHandle(hRemoteThread);

	return TRUE;
}

DWORD CTargetWatcherThread::Execute()
{
	HRESULT hres;

	hres = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hres))
	{
		SCERROR("CTargetWatcherThread::Execute() CoInitializeEx hr = %x", hres);
		return 1;                  // Program has failed.
	}

	hres = CoInitializeSecurity(
		NULL,
		-1,                          // COM negotiates service
		NULL,                        // Authentication services
		NULL,                        // Reserved
		RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
		RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation  
		NULL,                        // Authentication info
		EOAC_NONE,                   // Additional capabilities 
		NULL                         // Reserved
		);


	if (FAILED(hres))
	{
		CoUninitialize();
		SCERROR("CTargetWatcherThread::Execute() CoInitializeSecurity hr = %x", hres);
		return 1;                      // Program has failed.
	}

	char InjectfileFullPath[MAX_PATH] = "";
	::GetModuleFileNameA(::GetModuleHandleA(NULL), InjectfileFullPath, MAX_PATH);

	std::string injectDllPath(InjectfileFullPath);
	injectDllPath = injectDllPath.erase(injectDllPath.find_last_of('\\'));
	char cpathend = *(injectDllPath.crbegin());
	if (cpathend != '\\')
	{
		injectDllPath = injectDllPath + '\\';
	}
	injectDllPath = injectDllPath + "SINYD_SC_RemoteScreenCapture.dll";

	CProcessNotification processNofity;
	CProcessNotification::TNotificationFunction CreationFunc = std::tr1::bind(&CTargetWatcherThread::ProcessCreationCallback, this, std::tr1::placeholders::_1);
	CProcessNotification::TNotificationFunction TerminationFunc = std::tr1::bind(&CTargetWatcherThread::ProcessTerminationCallback, this, std::tr1::placeholders::_1);
	try
	{
		processNofity.registerCreationCallback(CreationFunc);
		processNofity.registerTerminationCallback(TerminationFunc/*, m_dwTargetProcessID*/);

		if (SearchTargetProcessInAllStartedProcess(&m_dwTargetProcessID))
		{
			SCINFO("CTargetWatcherThread TargetProcess is Started ID = %d", m_dwTargetProcessID);

// 			processNofity.unregisterCreationCallback();
// 			processNofity.registerTerminationCallback(TerminationFunc, m_dwTargetProcessID);

			if (ConfiginitializerInstance::GetInstance()->CalibrationConfig()->GetBackgroundCaptureType() == 0)		//HOOK方式进行截图
			{
				InjectDll(injectDllPath.c_str(), m_dwTargetProcessID);
			}
			
			CollectionMgrInstance::GetInstance()->Run();
			ScreenShotHolderInstance::GetInstance()->Run(m_dwTargetProcessID);
		}
	}
	catch (const std::exception& e)
	{
		SCERROR("CTargetWatcherThread::Execute() registerCreationCallback exception = %s", e.what());
	}

	constexpr DWORD dwHandleCount = 4;
	HANDLE handles[dwHandleCount] = { m_hStopEvent, m_hTargetCreateEvent, m_hTargetTerminateEvent, m_hSearchprcessEvent };
	while (true)
	{
		DWORD dw = ::WaitForMultipleObjects(dwHandleCount, handles, FALSE, INFINITE);
		if (WAIT_OBJECT_0 == dw)
		{
			SCINFO("CTargetWatcherThread TargetWatcherThread to stop");

			ScreenShotHolderInstance::GetInstance()->Stop();
			CollectionMgrInstance::GetInstance()->Stop();

			processNofity.unregisterCreationCallback();
			processNofity.unregisterTerminationCallback();
			break;
		}
		else if (WAIT_OBJECT_0 + 1 == dw)
		{
			try
			{
				SCINFO("CTargetWatcherThread TargetProcess start ID = %d", m_dwTargetProcessID);

/*				processNofity.registerTerminationCallback(TerminationFunc, m_dwTargetProcessID);*/

				if (ConfiginitializerInstance::GetInstance()->CalibrationConfig()->GetBackgroundCaptureType() == 0)		//HOOK方式进行截图
				{
					InjectDll(injectDllPath.c_str(), m_dwTargetProcessID);
				}

				CollectionMgrInstance::GetInstance()->Run();
				ScreenShotHolderInstance::GetInstance()->Run(m_dwTargetProcessID);
			}
			catch (const std::exception& e)
			{
				SCERROR("CTargetWatcherThread::Execute() registerTerminationCallback exception = %s", e.what());
			}
		}
		else if (WAIT_OBJECT_0 + 2 == dw)
		{
			try
			{
				SCINFO("CTargetWatcherThread TargetProcess stop ID = %d", m_dwTargetProcessID);

/*				processNofity.registerCreationCallback(CreationFunc);*/

				ScreenShotHolderInstance::GetInstance()->Stop();
				CollectionMgrInstance::GetInstance()->Stop();
			}
			catch (const std::exception& e)
			{
				SCERROR("CTargetWatcherThread::Execute() registerCreationCallback exception = %s", e.what());
			}
		}
		else if (WAIT_OBJECT_0 + 3 == dw)		//¶¨Ê±¼ì²éÄ¿±ê½ø³Ì
		{
			DWORD pid = 0;
			if (SearchTargetProcessInAllStartedProcess(&pid))
			{
				if (pid != m_dwTargetProcessID)
				{
					m_dwTargetProcessID = pid;
					CollectionMgrInstance::GetInstance()->Run();
					ScreenShotHolderInstance::GetInstance()->Stop();
					ScreenShotHolderInstance::GetInstance()->Run(m_dwTargetProcessID);
				}
			}
		}
		else
		{
			continue;
		}
	}

	CoUninitialize();

	return 0;
}

DWORD CTargetWatcherThread::StartImpl(DWORD dwCreationFlags)
{
	DWORD dw = CThreadBase::StartImpl(dwCreationFlags);
	m_SearchProcessTimer.Start(5000);

	return dw;
}

void CTargetWatcherThread::StopImpl(int nWaitTime)
{
	m_SearchProcessTimer.Stop();
	CThreadBase::StopImpl(nWaitTime);
}

bool CTargetWatcherThread::SearchTargetProcessInAllStartedProcess(DWORD* dwProcessID)
{
	bool bRes = false;

	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 pe32;
		pe32.dwSize = sizeof(PROCESSENTRY32);

		BOOL bRet = Process32First(hProcessSnap, &pe32);
		while (bRet)
		{
			CString strFullPath;
			GetProcessPath(pe32.th32ProcessID, strFullPath);
			CComBSTR bstr(ConfiginitializerInstance::GetInstance()->CalibrationConfig()->GetSoftwarePath());
			if (bstr != NULL && 0 == strFullPath.CompareNoCase(bstr))
			{
				SCINFO("ProID = %d", pe32.th32ProcessID);
				*dwProcessID = pe32.th32ProcessID;
				bRes = true;
				break;
			}
			bRet = Process32Next(hProcessSnap, &pe32);
		}
		::CloseHandle(hProcessSnap);
	}

	return bRes;
}

void CTargetWatcherThread::GetProcessFilePath(IN HANDLE hProcess, CString& sFilePath)
{
	sFilePath = _T("");

	TCHAR tsFileDosPath[MAX_PATH + 1];
	ZeroMemory(tsFileDosPath, sizeof(TCHAR)*(MAX_PATH + 1));
	if (0 == GetProcessImageFileName(hProcess, tsFileDosPath, MAX_PATH + 1))
	{
		return;
	}

	UINT uiLen = GetLogicalDriveStrings(0, NULL);
	if (0 == uiLen)
	{
		return;
	}

	PTSTR pLogicDriveString = new TCHAR[uiLen + 1];
	ZeroMemory(pLogicDriveString, uiLen + 1);
	uiLen = GetLogicalDriveStrings(uiLen, pLogicDriveString);
	if (0 == uiLen)
	{
		delete[]pLogicDriveString;
		return;
	}

	TCHAR szDrive[3] = TEXT(" :");
	PTSTR pDosDriveName = new TCHAR[MAX_PATH];
	PTSTR pLogicIndex = pLogicDriveString;

	do
	{
		szDrive[0] = *pLogicIndex;
		uiLen = QueryDosDevice(szDrive, pDosDriveName, MAX_PATH);
		if (0 == uiLen)
		{
			if (ERROR_INSUFFICIENT_BUFFER != GetLastError())
			{
				break;
			}

			delete[]pDosDriveName;
			pDosDriveName = new TCHAR[uiLen + 1];
			uiLen = QueryDosDevice(szDrive, pDosDriveName, uiLen + 1);
			if (0 == uiLen)
			{
				break;
			}
		}

		uiLen = _tcslen(pDosDriveName);
		if (0 == _tcsnicmp(tsFileDosPath, pDosDriveName, uiLen))
		{
			sFilePath.Format(_T("%s%s"), szDrive, tsFileDosPath + uiLen);
			break;
		}

		while (*pLogicIndex++);
	} while (*pLogicIndex);

	delete[]pLogicDriveString;
	delete[]pDosDriveName;
}

void CTargetWatcherThread::GetProcessPath(DWORD processID, CString& str)
{
	HMODULE hMods[1024];
	HANDLE hProcess;
	DWORD cbNeeded;
	unsigned int i;

	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID);
	if (NULL == hProcess)
		return;

	TCHAR szModName[MAX_PATH] = { 0 };
	GetProcessFilePath(hProcess, str);

	CloseHandle(hProcess);
}

bool CTargetWatcherThread::ProcessCreationCallback(IWbemClassObject * ptr)
{
	bool bRes = false;
	if (ptr)
	{
		CComVariant vtInstanceObject;
		HRESULT hRet = ptr->Get(_T("TargetInstance"), 0, &vtInstanceObject, NULL, NULL);
		if (SUCCEEDED(hRet) && vtInstanceObject.vt == VT_UNKNOWN && vtInstanceObject.punkVal != NULL)
		{
			CComPtr<IWbemClassObject> pTargetInstance = (IWbemClassObject*)vtInstanceObject.punkVal;

			CComVariant vtProcessID/*, vtName*/, vtExecutablePath;
			HRESULT h1 = pTargetInstance->Get(_T("ProcessID"), 0, &vtProcessID, NULL, NULL);
			//HRESULT h2 = pTargetInstance->Get(_T("Name"), 0, &vtName, NULL, NULL);
			HRESULT h3 = pTargetInstance->Get(_T("ExecutablePath"), 0, &vtExecutablePath, NULL, NULL);
			if (SUCCEEDED(h1)/* && SUCCEEDED(h2)*/ && SUCCEEDED(h3))
			{
				CComBSTR bstrTargetName(ConfiginitializerInstance::GetInstance()->CalibrationConfig()->GetSoftwarePath());
				if (bstrTargetName != NULL)
				{
					bstrTargetName.ToLower();
					CComBSTR bstrGetName(vtExecutablePath.bstrVal);
					bstrGetName.ToLower();
					if (bstrTargetName == bstrGetName)
					{
						m_dwTargetProcessID = vtProcessID.ulVal;
						::SetEvent(m_hTargetCreateEvent);
						bRes = true;
					}
				}
			}
		}
	}
	return bRes;
}

bool CTargetWatcherThread::ProcessTerminationCallback(IWbemClassObject * ptr)
{
// 	m_dwTargetProcessID = 0;
// 	::SetEvent(m_hTargetTerminateEvent);
// 	return true;

	bool bRes = false;
	if (ptr)
	{
		CComVariant vtInstanceObject;
		HRESULT hRet = ptr->Get(_T("TargetInstance"), 0, &vtInstanceObject, NULL, NULL);
		if (SUCCEEDED(hRet) && vtInstanceObject.vt == VT_UNKNOWN && vtInstanceObject.punkVal != NULL)
		{
			CComPtr<IWbemClassObject> pTargetInstance = (IWbemClassObject*)vtInstanceObject.punkVal;

			CComVariant vtProcessID/*, vtName*/, vtExecutablePath;
			HRESULT h1 = pTargetInstance->Get(_T("ProcessID"), 0, &vtProcessID, NULL, NULL);
			//HRESULT h2 = pTargetInstance->Get(_T("Name"), 0, &vtName, NULL, NULL);
			HRESULT h3 = pTargetInstance->Get(_T("ExecutablePath"), 0, &vtExecutablePath, NULL, NULL);
			if (SUCCEEDED(h1)/* && SUCCEEDED(h2)*/ && SUCCEEDED(h3))
			{
				CComBSTR bstrTargetName(ConfiginitializerInstance::GetInstance()->CalibrationConfig()->GetSoftwarePath());
				if (bstrTargetName != NULL)
				{
					bstrTargetName.ToLower();
					CComBSTR bstrGetName(vtExecutablePath.bstrVal);
					bstrGetName.ToLower();
					if (bstrTargetName == bstrGetName)
					{
						m_dwTargetProcessID = 0;
						::SetEvent(m_hTargetTerminateEvent);
						bRes = true;
					}
				}
			}
		}
	}
	return bRes;
}

void CTargetWatcherThread::SearchProcessTimerFunc()
{
	if (m_hSearchprcessEvent)
	{
		::SetEvent(m_hSearchprcessEvent);
	}
}
