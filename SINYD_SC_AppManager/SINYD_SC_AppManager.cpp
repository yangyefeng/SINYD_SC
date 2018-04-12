// SINYD_SC_AppManager.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "SINYD_SC_AppManager.h"
#include "TRWData.h"
#include <string>
#include "..\Common\log.h"
#include "..\Common\TemplateTimer.h"
#include "..\SINYD_SC_Crypt\CryptTool.h"
#include <TlHelp32.h>
#include <atlstr.h>
#include <Psapi.h>
#include <imagehlp.h>

#pragma comment(lib, "imagehlp.lib")
#pragma comment(lib, "Psapi.lib")

#define MAX_LOADSTRING 100

#define MGR_PROCESS_RUNONCE_NAME  _T("SC_APPMGR_mutex_{46EBE513-541A-4CC2-8E43-31959EFAE7DE}")

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

string SetCurrentDirectoryToInstallDir()
{
	char fileFullPath[MAX_PATH] = "";
	::GetModuleFileNameA(::GetModuleHandle(NULL), fileFullPath, MAX_PATH);
	std::string strRes(fileFullPath);
	strRes = strRes.erase(strRes.find_last_of('\\'));

	::SetCurrentDirectoryA(strRes.c_str());

	return strRes;
}

#define ServiceConfigFile ".\\ConfigFile_RunTime\\AppManager.conf"
#define AppExeName ("SINYD_SC_CoreApp.exe")
#define AppWindowClassName _T("SINYD_SC_COREAPP")
#define AppWindowTitle _T("SINYD_SC_CoreApp")

#define MY_WM_START (WM_USER + 100)
#define MY_WM_STOP (WM_USER + 101)
#define MY_WM_RESTART (WM_USER + 102)
#define MY_WM_INFOCOLLECT_START (WM_USER + 200)
#define MY_WM_INFOCOLLECT_STOP (WM_USER + 201)


int CoreAppState()
{
	HWND hWnd = ::FindWindow(AppWindowClassName, AppWindowTitle);
	if (NULL == hWnd)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

bool StartCoreApp()
{
	bool bRes = false;

	char fileFullPath[MAX_PATH] = "";
	::GetModuleFileNameA(::GetModuleHandle(NULL), fileFullPath, MAX_PATH);
	std::string strAppFullPath(fileFullPath);
	strAppFullPath = strAppFullPath.erase(strAppFullPath.find_last_of('\\'));
	strAppFullPath = strAppFullPath + "\\" + AppExeName;
	
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	if (CreateProcessA(strAppFullPath.c_str(), "", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		Sleep(100);
		int iRes = CoreAppState();
		if (iRes == 1)
		{
			bRes = true;
		}
	
	}

	return bRes;
}

bool StopCoreApp()
{
	bool bRes = false;
	HWND hWnd = ::FindWindow(AppWindowClassName, AppWindowTitle);
	if (hWnd != NULL)
	{
		SendMessage(hWnd, MY_WM_STOP, 0, 0);
		bRes = true;
	}

	return bRes;
}

bool RestartCoreApp()
{
	bool bRes = false;
	HWND hWnd = ::FindWindow(AppWindowClassName, AppWindowTitle);
	if (hWnd != NULL)
	{
		SendMessage(hWnd, MY_WM_RESTART, 0, 0);
		bRes = true;
	}

	return bRes;
}

bool CoreAppStartCaliPng()
{
	bool bRes = false;
	HWND hWnd = ::FindWindow(AppWindowClassName, AppWindowTitle);
	if (hWnd != NULL)
	{
		SendMessage(hWnd, MY_WM_INFOCOLLECT_START, 0, 0);
		bRes = true;
	}

	return bRes;
}

bool CoreAppStopCaliPng()
{
	bool bRes = false;
	HWND hWnd = ::FindWindow(AppWindowClassName, AppWindowTitle);
	if (hWnd != NULL)
	{
		SendMessage(hWnd, MY_WM_INFOCOLLECT_STOP, 0, 0);
		bRes = true;
	}

	return bRes;
}

HANDLE g_ServiceStopEvent = NULL;
DWORD WINAPI WorkerThread(LPVOID lpParam)
{
	MakeSureDirectoryPathExists(ServiceConfigFile);

	CTRWData rw;
	bool bCreate = rw._CreateFile(ServiceConfigFile);
	int cmd = rw._WriteProfileInt("AppControl", "Command", 0, ServiceConfigFile);
	rw._WriteProfileInt("AppState", "CollectionAppState", CoreAppState(), ServiceConfigFile);

	if (bCreate)
	{
		rw._WriteDebugForLog("\n\n\n1:StartCoreApp   ", ServiceConfigFile);
		rw._WriteDebugForLog("2:StopCoreApp   ", ServiceConfigFile);
		rw._WriteDebugForLog("3:RestartCoreApp   ", ServiceConfigFile);
		rw._WriteDebugForLog("4:CoreAppStartCaliPng   ", ServiceConfigFile);
		rw._WriteDebugForLog("5:CoreAppStopCaliPng   ", ServiceConfigFile);
	}

	while (1)
	{
		DWORD dw = WaitForSingleObject(g_ServiceStopEvent, 1000);
		if (dw == WAIT_OBJECT_0)
		{
			break;
		}
		else if (dw == WAIT_TIMEOUT)
		{
			int cmd = rw._GetProfileInt("AppControl", "Command", 0, ServiceConfigFile);

			bool bRes = FALSE;
			if (cmd == 1)
			{
				bRes = StartCoreApp();
			}
			else if (cmd == 2)
			{
				bRes = StopCoreApp();
			}
			else if (cmd == 3)
			{
				bRes = RestartCoreApp();
			}
			else if (cmd == 4)
			{
				bRes = CoreAppStartCaliPng();
			}
			else if (cmd == 5)
			{
				bRes = CoreAppStopCaliPng();
			}
			else
			{
				bRes = true;
			}

			if (bRes)
			{
				rw._WriteProfileInt("AppControl", "Command", 0, ServiceConfigFile);
			}

			rw._WriteProfileInt("AppState", "CollectionAppState", CoreAppState(), ServiceConfigFile);
		}
		else
		{
			continue;
		}
	}

	return ERROR_SUCCESS;
}

#define ManagerConfig L".\\ConfigFile\\ManagerConfig.conf"

DWORD WINAPI RemoteSyncThread(LPVOID lpParam)
{
	string batPath = SetCurrentDirectoryToInstallDir();
	batPath = batPath + "\\cwRsync\\SyncAll.bat";

	int waitTime = GetPrivateProfileInt(L"Time", L"SyncIntervalTime", 5 * 60, ManagerConfig) * 1000;
	while (1)
	{
		DWORD dw = WaitForSingleObject(g_ServiceStopEvent, waitTime);
		if (dw == WAIT_OBJECT_0)
		{
			break;
		}
		else if (dw == WAIT_TIMEOUT)
		{
			//检查服务器路径是否已设置
			FILE* pathFile = fopen(".\\cwRsync\\RemoteServerPath.txt", "r");
			if (pathFile)
			{
				char ch = fgetc(pathFile);
				if (ch == EOF || ch == '\n' || ch == '\r')
				{
					fclose(pathFile);
					continue;
				}

				fclose(pathFile);
			}
			else
			{
				continue;
			}

			//执行全部同步
			PROCESS_INFORMATION pi;
			STARTUPINFOA si;
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);
			si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
// 			si.dwXCountChars = 500;
// 			si.dwYCountChars = 500;
// 			si.dwFlags = STARTF_USESHOWWINDOW;
// 			si.wShowWindow = SW_HIDE;
			
			if (!CreateProcessA(NULL,
				(LPSTR)batPath.c_str(),
				NULL,
				NULL,
				FALSE,
				CREATE_NO_WINDOW,
				NULL,
				NULL,
				&si, &pi))
			{
				SCERROR("[AppManager]%s run Fail", batPath.c_str());
			}
			else
			{
				HANDLE h[2] = { g_ServiceStopEvent, pi.hProcess };
				DWORD d = WaitForMultipleObjects(2, h, FALSE, INFINITE);
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);
				if (WAIT_OBJECT_0 == d)
				{
					break;
				}
			}
		}
		else
		{
			//continue;
		}

		waitTime = GetPrivateProfileInt(L"Time", L"SyncIntervalTime", 5 * 60, ManagerConfig) * 1000;
	}

	return ERROR_SUCCESS;
}
DWORD WINAPI RemoteScriptThread(LPVOID lpParam)
{
	string curDir = SetCurrentDirectoryToInstallDir();
	string batPath = curDir + "\\RemoteScript\\cmd_list.bat";
	string logbatPath = curDir + "\\cwRsync\\UploadLogAndScript.bat";

	int waitTime = GetPrivateProfileInt(L"Time", L"ScriptIntervalTime", 5 * 60, ManagerConfig) * 1000;
	while (1)
	{
		DWORD dw = WaitForSingleObject(g_ServiceStopEvent, waitTime);
		if (dw == WAIT_OBJECT_0)
		{
			break;
		}
		else if (dw == WAIT_TIMEOUT)
		{
			//检查服务器路径是否已设置
			FILE* pathFile = fopen(".\\cwRsync\\RemoteServerPath.txt", "r");
			if (pathFile)
			{
				char ch = fgetc(pathFile);
				if (ch == EOF || ch == '\n' || ch == '\r')
				{
					fclose(pathFile);
					continue;
				}

				fclose(pathFile);
			}
			else
			{
				continue;
			}

			//执行脚本命令
			PROCESS_INFORMATION pi;
			STARTUPINFOA si;
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);
			si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
			if (!CreateProcessA(NULL,
				(LPSTR)batPath.c_str(),
				NULL,
				NULL,
				FALSE,
				CREATE_NO_WINDOW,
				NULL,
				NULL,
				&si, &pi))
			{
				SCERROR("[AppManager]%s run Fail", batPath.c_str());
			}
			else
			{
				HANDLE h[2] = { g_ServiceStopEvent, pi.hProcess };
				DWORD d = WaitForMultipleObjects(2, h, FALSE, INFINITE);
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);
				
				if (WAIT_OBJECT_0 == d)
				{
					break;
				}

				FILE* batClear = fopen(batPath.c_str(), "w");
				if (batClear)
				{
					fclose(batClear);
				}

				PROCESS_INFORMATION piLog;
				STARTUPINFOA siLog;
				ZeroMemory(&siLog, sizeof(si));
				siLog.cb = sizeof(si);
				siLog.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
				if (!CreateProcessA(NULL,
					(LPSTR)logbatPath.c_str(),
					NULL,
					NULL,
					FALSE,
					CREATE_NO_WINDOW,
					NULL,
					NULL,
					&siLog, &piLog))
				{
					SCERROR("[AppManager]%s run Fail", logbatPath.c_str());
				}
			}
		}
		else
		{
			//continue;
		}

		waitTime = GetPrivateProfileInt(L"Time", L"ScriptIntervalTime", 5 * 60, ManagerConfig) * 1000;
	}


	return ERROR_SUCCESS;
}

class SaveMachineCodeTimer : public CSCTimer
{
public:
	SaveMachineCodeTimer() {}
	~SaveMachineCodeTimer() {}

	virtual void OnTimedEvent()
	{
		string mc = m_tool.GetSecretMachineCodeWithTimeFromLocal();
		FILE* file = fopen(".\\RemoteLog\\MachineCode.txt", "w");
		if (file)
		{
			fprintf(file, mc.c_str());
			fclose(file);
		}
	}

private:
	CCryptTool m_tool;
};


void GetProcessFilePath(IN HANDLE hProcess, CString& sFilePath)
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

void GetProcessPath(DWORD processID, CString& str)
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

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	if (lpCmdLine != NULL)
	{
		if (_tcsicmp(lpCmdLine, _T("/run")) == 0)
		{
			char filePath[MAX_PATH] = { 0 };
			GetModuleFileNameA(::GetModuleHandle(NULL), filePath, MAX_PATH);

			STARTUPINFOA si;
			PROCESS_INFORMATION pi;
			ZeroMemory(&pi, sizeof(pi));
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);
			if (CreateProcessA(filePath, "", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
			{
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);
			}

			return 0;
		}
		if (_tcsicmp(lpCmdLine, _T("/stop")) == 0)
		{
			TCHAR filePath[MAX_PATH] = { 0 };
			GetModuleFileName(::GetModuleHandle(NULL), filePath, MAX_PATH);

			HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			if (hProcessSnap != INVALID_HANDLE_VALUE)
			{
				PROCESSENTRY32 pe32;
				pe32.dwSize = sizeof(PROCESSENTRY32);

				BOOL bRet = Process32First(hProcessSnap, &pe32);
				while (bRet)
				{
					if (GetCurrentProcessId() != pe32.th32ProcessID)
					{
						CString strFullPath;
						GetProcessPath(pe32.th32ProcessID, strFullPath);
						if (0 == strFullPath.CompareNoCase(filePath))
						{
							HANDLE hProcessHandle = ::OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
							TerminateProcess(hProcessHandle, 0);
							CloseHandle(hProcessHandle);
							break;
						}
					}

					bRet = Process32Next(hProcessSnap, &pe32);
				}
				::CloseHandle(hProcessSnap);
			}

			return 0;
		}
	}

	HANDLE m_hMutex = CreateMutex(NULL, FALSE, MGR_PROCESS_RUNONCE_NAME);
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(m_hMutex);
		m_hMutex = NULL;
		return 0;
	}

	SetCurrentDirectoryToInstallDir();

    // TODO: 在此放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SINYD_SC_APPMANAGER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

//     // 执行应用程序初始化: 
//     if (!InitInstance (hInstance, SW_HIDE))
//     {
//         return FALSE;
//     }

	g_ServiceStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	HANDLE hThread = CreateThread(NULL, 0, WorkerThread, NULL, 0, NULL);
	HANDLE hSyncThread = CreateThread(NULL, 0, RemoteSyncThread, NULL, 0, NULL);
	HANDLE hScriptThread = CreateThread(NULL, 0, RemoteScriptThread, NULL, 0, NULL);

	SaveMachineCodeTimer mctimer;
	mctimer.Start(60 * 60 * 1000, true);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SINYD_SC_APPMANAGER));

    MSG msg;

    // 主消息循环: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

	mctimer.Stop();

	WaitForSingleObject(hThread, INFINITE);
	WaitForSingleObject(hSyncThread, INFINITE);
	WaitForSingleObject(hScriptThread, INFINITE);
	CloseHandle(g_ServiceStopEvent);


	if (m_hMutex)
	{
		CloseHandle(m_hMutex);
	}

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SINYD_SC_APPMANAGER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SINYD_SC_APPMANAGER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择: 
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case WM_ENDSESSION:
				SetEvent(g_ServiceStopEvent);
				break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
