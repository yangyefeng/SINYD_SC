// SINYD_SC_CoreApp.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "SINYD_SC_CoreApp.h"
#include "MainWorkThread.h"
#include "..\SINYD_SC_Register\SCRegisteTool.h"

#define MAX_LOADSTRING 100
#define PROCESS_RUNONCE_NAME  _T("SC_mutex_{A5794AB8-C211-4598-A3BC-F348DAC7B8A6}")

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


#define MY_WM_START (WM_USER + 100)
#define MY_WM_STOP (WM_USER + 101)
#define MY_WM_RESTART (WM_USER + 102)
#define MY_WM_INFOCOLLECT_START (WM_USER + 200)
#define MY_WM_INFOCOLLECT_STOP (WM_USER + 201)

CMainWorkThread* g_WorkThread = NULL;
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
			LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
			LoadStringW(hInstance, IDC_SINYD_SC_COREAPP, szWindowClass, MAX_LOADSTRING);
			HWND hWnd = FindWindow(szWindowClass, szTitle);
			if (hWnd)
			{
				SendMessage(hWnd, MY_WM_STOP, NULL, NULL);
			}

			return 0;
		}
	}

	HANDLE m_hMutex = CreateMutex(NULL, FALSE, PROCESS_RUNONCE_NAME);
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(m_hMutex);
		m_hMutex = NULL;
		return 0;
	}


    // TODO: 在此放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SINYD_SC_COREAPP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, SW_HIDE))
    {
        return FALSE;
    }

	g_WorkThread = new CMainWorkThread();
	g_WorkThread->Start();

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SINYD_SC_COREAPP));

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

	if (g_WorkThread)
	{
		g_WorkThread->Stop(10000);
		delete g_WorkThread;
		g_WorkThread = NULL;
		Sleep(5000);
	}

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SINYD_SC_COREAPP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SINYD_SC_COREAPP);
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
	case MY_WM_INFOCOLLECT_START:
		g_WorkThread->SetIsSaveCaliPNG(true);
		break;
	case  MY_WM_INFOCOLLECT_STOP:
		g_WorkThread->SetIsSaveCaliPNG(false);
		break;
	case MY_WM_RESTART:
		g_WorkThread->Restart();
		break;
	case MY_WM_STOP:
		return DefWindowProc(hWnd, WM_CLOSE, wParam, lParam);
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
