// SCAppMgrDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SINYD_SC_SettingTool_MFC.h"
#include "SCAppMgrDlg.h"
#include "afxdialogex.h"
#include <string>
#include "RWData.h"



// CSCAppMgrDlg 对话框

IMPLEMENT_DYNAMIC(CSCAppMgrDlg, CDialogEx)

CSCAppMgrDlg::CSCAppMgrDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG4, pParent)
{

}

CSCAppMgrDlg::~CSCAppMgrDlg()
{
}

void CSCAppMgrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSCAppMgrDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CSCAppMgrDlg::OnBnClickedButtonStartApp)
	ON_BN_CLICKED(IDC_BUTTON2, &CSCAppMgrDlg::OnBnClickedButtonStopApp)
	ON_BN_CLICKED(IDC_BUTTON3, &CSCAppMgrDlg::OnBnClickedButtonStartPNG)
	ON_BN_CLICKED(IDC_BUTTON4, &CSCAppMgrDlg::OnBnClickedButtonStopPNG)
	ON_BN_CLICKED(IDC_BUTTON5, &CSCAppMgrDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON_CLearPNG, &CSCAppMgrDlg::OnBnClickedButtonClearpng)
END_MESSAGE_MAP()


// CSCAppMgrDlg 消息处理程序


#define ServiceConfigFile ".\\ConfigFile_RunTime\\AppManager.conf"
#define AppExeName ("SINYD_SC_CoreApp.exe")
#define AppWindowClassName _T("SINYD_SC_COREAPP")
#define AppWindowTitle _T("SINYD_SC_CoreApp")

#define MY_WM_START (WM_USER + 100)
#define MY_WM_STOP (WM_USER + 101)
#define MY_WM_RESTART (WM_USER + 102)
#define MY_WM_INFOCOLLECT_START (WM_USER + 200)
#define MY_WM_INFOCOLLECT_STOP (WM_USER + 201)

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

		bRes = true;
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

int CoreAppCapturePNGState()
{
	CRWData rw;
	return rw._GetProfileInt("AppState", "PNGCaptureState", 0, ServiceConfigFile);
}




BOOL CSCAppMgrDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	updateAppState();
	updateCapturePNGState();

	return TRUE;
}

void CSCAppMgrDlg::OnBnClickedButtonStartApp()
{
	bool bres = StartCoreApp();

	Sleep(500);
	int iRes = CoreAppState();
	if (bres && iRes == 1)
	{
		MessageBox(L"启动成功");
	}
	else
	{
		MessageBox(L"启动失败，请确认进程状态或直接运行安装路径下的程序");
	}

	Sleep(500);
	updateAppState();
}


void CSCAppMgrDlg::OnBnClickedButtonStopApp()
{
	// TODO: 在此添加控件通知处理程序代码
	bool bres = StopCoreApp();

	if (bres)
	{
		MessageBox(L"停止成功");
	}
	else
	{
		MessageBox(L"停止失败，请确认进程状态");
	}

	Sleep(300);
	updateAppState();
}


void CSCAppMgrDlg::OnBnClickedButtonStartPNG()
{
	bool bres = CoreAppStartCaliPng();

	if (bres)
	{
		MessageBox(L"开始成功");
	}
	else
	{
		MessageBox(L"开始失败，请确认进程状态");
	}

	Sleep(300);
	updateCapturePNGState();
}


void CSCAppMgrDlg::OnBnClickedButtonStopPNG()
{
	bool bres = CoreAppStopCaliPng();

	if (bres)
	{
		MessageBox(L"关闭成功");
	}
	else
	{
		MessageBox(L"关闭失败，请确认进程状态");
	}

	Sleep(300);
	updateCapturePNGState();
}

void CSCAppMgrDlg::updateAppState()
{
	int res = CoreAppState();
	SetDlgItemTextW(IDC_EDIT_APPSTATE, res == 1 ? (L"已启动") : (L"未启动"));
}

void CSCAppMgrDlg::updateCapturePNGState()
{
	int res = CoreAppCapturePNGState();
	SetDlgItemTextW(IDC_EDIT_CapturePNGState, res == 1 ? (L"已开始") : (L"未开始"));
}


void CSCAppMgrDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	updateAppState();
	updateCapturePNGState();
}

BOOL SHDeleteFolderA(LPCSTR pstrFolder, BOOL bAllowUndo)
{
	if ((NULL == pstrFolder))
	{
		return FALSE;
	}

	int iPathLen = strlen(pstrFolder);
	if (iPathLen >= MAX_PATH)
	{
		return FALSE;
	}

	/*确保目录的路径以2个\0结尾*/
	char tczFolder[MAX_PATH + 1];
	memset(tczFolder, 0, (MAX_PATH + 1)*sizeof(char));
	strcpy_s(tczFolder, pstrFolder);
	tczFolder[iPathLen] = _T('\0');
	tczFolder[iPathLen + 1] = _T('\0');

	SHFILEOPSTRUCTA FileOp;
	ZeroMemory(&FileOp, sizeof(SHFILEOPSTRUCT));
	FileOp.fFlags |= FOF_SILENT;        /*不显示进度*/
	FileOp.fFlags |= FOF_NOERRORUI;        /*不报告错误信息*/
	FileOp.fFlags |= FOF_NOCONFIRMATION;/*直接删除，不进行确认*/
	FileOp.hNameMappings = NULL;
	FileOp.hwnd = NULL;
	FileOp.lpszProgressTitle = NULL;
	FileOp.wFunc = FO_DELETE;
	FileOp.pFrom = tczFolder;            /*要删除的目录，必须以2个\0结尾*/
	FileOp.pTo = NULL;

	/*根据传递的bAllowUndo参数确定是否删除到回收站*/
	if (bAllowUndo)
	{
		FileOp.fFlags |= FOF_ALLOWUNDO; /*删除到回收站*/
	}
	else
	{
		FileOp.fFlags &= ~FOF_ALLOWUNDO; /*直接删除，不放入回收站*/
	}

	/*删除目录*/
	if (0 == SHFileOperationA(&FileOp))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


void CSCAppMgrDlg::OnBnClickedButtonClearpng()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR workDir[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH, workDir);
	CString pngFolder = workDir;
	pngFolder.TrimRight('\\');
	pngFolder = pngFolder + L"\\CalibrationInfo";
	BOOL bRes = SHDeleteFolderA(CT2A(pngFolder), FALSE);

	FILE* file = fopen(".\\RemoteLog\\pnglist.log", "w");
	if (file)
	{
		fclose(file);
		file = NULL;
	}

	MessageBox(L"清空完毕");
}
