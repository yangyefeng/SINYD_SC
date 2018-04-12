// SCAppMgrDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SINYD_SC_SettingTool_MFC.h"
#include "SCAppMgrDlg.h"
#include "afxdialogex.h"
#include <string>
#include "RWData.h"



// CSCAppMgrDlg �Ի���

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


// CSCAppMgrDlg ��Ϣ�������


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
		MessageBox(L"�����ɹ�");
	}
	else
	{
		MessageBox(L"����ʧ�ܣ���ȷ�Ͻ���״̬��ֱ�����а�װ·���µĳ���");
	}

	Sleep(500);
	updateAppState();
}


void CSCAppMgrDlg::OnBnClickedButtonStopApp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	bool bres = StopCoreApp();

	if (bres)
	{
		MessageBox(L"ֹͣ�ɹ�");
	}
	else
	{
		MessageBox(L"ֹͣʧ�ܣ���ȷ�Ͻ���״̬");
	}

	Sleep(300);
	updateAppState();
}


void CSCAppMgrDlg::OnBnClickedButtonStartPNG()
{
	bool bres = CoreAppStartCaliPng();

	if (bres)
	{
		MessageBox(L"��ʼ�ɹ�");
	}
	else
	{
		MessageBox(L"��ʼʧ�ܣ���ȷ�Ͻ���״̬");
	}

	Sleep(300);
	updateCapturePNGState();
}


void CSCAppMgrDlg::OnBnClickedButtonStopPNG()
{
	bool bres = CoreAppStopCaliPng();

	if (bres)
	{
		MessageBox(L"�رճɹ�");
	}
	else
	{
		MessageBox(L"�ر�ʧ�ܣ���ȷ�Ͻ���״̬");
	}

	Sleep(300);
	updateCapturePNGState();
}

void CSCAppMgrDlg::updateAppState()
{
	int res = CoreAppState();
	SetDlgItemTextW(IDC_EDIT_APPSTATE, res == 1 ? (L"������") : (L"δ����"));
}

void CSCAppMgrDlg::updateCapturePNGState()
{
	int res = CoreAppCapturePNGState();
	SetDlgItemTextW(IDC_EDIT_CapturePNGState, res == 1 ? (L"�ѿ�ʼ") : (L"δ��ʼ"));
}


void CSCAppMgrDlg::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

	/*ȷ��Ŀ¼��·����2��\0��β*/
	char tczFolder[MAX_PATH + 1];
	memset(tczFolder, 0, (MAX_PATH + 1)*sizeof(char));
	strcpy_s(tczFolder, pstrFolder);
	tczFolder[iPathLen] = _T('\0');
	tczFolder[iPathLen + 1] = _T('\0');

	SHFILEOPSTRUCTA FileOp;
	ZeroMemory(&FileOp, sizeof(SHFILEOPSTRUCT));
	FileOp.fFlags |= FOF_SILENT;        /*����ʾ����*/
	FileOp.fFlags |= FOF_NOERRORUI;        /*�����������Ϣ*/
	FileOp.fFlags |= FOF_NOCONFIRMATION;/*ֱ��ɾ����������ȷ��*/
	FileOp.hNameMappings = NULL;
	FileOp.hwnd = NULL;
	FileOp.lpszProgressTitle = NULL;
	FileOp.wFunc = FO_DELETE;
	FileOp.pFrom = tczFolder;            /*Ҫɾ����Ŀ¼��������2��\0��β*/
	FileOp.pTo = NULL;

	/*���ݴ��ݵ�bAllowUndo����ȷ���Ƿ�ɾ��������վ*/
	if (bAllowUndo)
	{
		FileOp.fFlags |= FOF_ALLOWUNDO; /*ɾ��������վ*/
	}
	else
	{
		FileOp.fFlags &= ~FOF_ALLOWUNDO; /*ֱ��ɾ�������������վ*/
	}

	/*ɾ��Ŀ¼*/
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

	MessageBox(L"������");
}
