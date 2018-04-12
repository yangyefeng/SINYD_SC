// RemoteServerPathSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SINYD_SC_SettingTool_MFC.h"
#include "RemoteServerPathSetDlg.h"
#include "afxdialogex.h"


// CRemoteServerPathSetDlg 对话框

IMPLEMENT_DYNAMIC(CRemoteServerPathSetDlg, CDialogEx)

CRemoteServerPathSetDlg::CRemoteServerPathSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_RemoteSet, pParent)
{

}

CRemoteServerPathSetDlg::~CRemoteServerPathSetDlg()
{
}

void CRemoteServerPathSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRemoteServerPathSetDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CRemoteServerPathSetDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_RemoteTest, &CRemoteServerPathSetDlg::OnBnClickedButtonRemotetest)
END_MESSAGE_MAP()


// CRemoteServerPathSetDlg 消息处理程序


void CRemoteServerPathSetDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString path;
	GetDlgItemText(IDC_EDIT_ServerPath, path);
	path.TrimRight('/');
	path += '/';

	FILE* file = fopen(".\\cwRsync\\RemoteServerPath.txt", "w");
	if (file)
	{
		fprintf(file, CT2A(path));

		fclose(file);
		file = NULL;

		MessageBox(L"服务器路径设置成功");
	}
	else
	{
		MessageBox(L"服务器路径设置失败，请确认cwRsync文件夹是否存在");
		return;
	}


	CDialogEx::OnOK();
}


BOOL CRemoteServerPathSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	FILE* file = fopen(".\\cwRsync\\RemoteServerPath.txt", "r");
	if (file)
	{
		char cpath[MAX_PATH] = { 0 };
		fgets(cpath, MAX_PATH, file);

		CStringA path(cpath);
		path.Trim('\n');
		SetDlgItemTextA(this->GetSafeHwnd(), IDC_EDIT_ServerPath, path);

		fclose(file);
		file = NULL;
	}



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CRemoteServerPathSetDlg::OnBnClickedButtonRemotetest()
{
	// TODO: 在此添加控件通知处理程序代码

	//全新创建log文件
	SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES),NULL,TRUE };
	HANDLE cmdOutput = CreateFile(L".\\cwRsync\\rsynctest.log",
		GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		&sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (cmdOutput != INVALID_HANDLE_VALUE)
	{
		CloseHandle(cmdOutput);
	}

	//拼接测试命令
	CString cmd;
	GetDlgItemText(IDC_EDIT_ServerPath, cmd);
	cmd.TrimRight('/');
	cmd += '/';
	cmd = L".\\cwRsync\\rsync.exe " + cmd;
	cmd = cmd + L" --log-file .\\cwRsync\\rsynctest.log";

	//执行脚本命令
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);

	if (!CreateProcess(NULL,
		cmd.GetBuffer(),
		NULL,
		NULL,
		TRUE,
		CREATE_NO_WINDOW | CREATE_DEFAULT_ERROR_MODE,
		NULL,
		NULL,
		&si, &pi))
	{
		MessageBox(L"启动Rsync程序失败！");
	}
	else
	{
		WaitForSingleObject(pi.hProcess, INFINITE);

		DWORD exeCode = 0;
		GetExitCodeProcess(pi.hProcess, &exeCode);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		if (exeCode == 0)
		{
			MessageBox(L"测试成功！");
		}
		else
		{
			CString err;
			err.Format(L"测试失败，错误码 = %d\n", exeCode);

			FILE* log = fopen(".\\cwRsync\\rsynctest.log", "r,ccs=UTF-8");
			if (log)
			{
				while (!feof(log))
				{
					wchar_t buf[1024] = { 0 };
					fgetws(buf, 1024, log);
					err = err + buf;
				}
				fclose(log);
			}

			MessageBox(err);
		}
	}
}
