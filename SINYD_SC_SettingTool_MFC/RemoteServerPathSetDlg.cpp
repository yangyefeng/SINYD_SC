// RemoteServerPathSetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SINYD_SC_SettingTool_MFC.h"
#include "RemoteServerPathSetDlg.h"
#include "afxdialogex.h"


// CRemoteServerPathSetDlg �Ի���

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


// CRemoteServerPathSetDlg ��Ϣ�������


void CRemoteServerPathSetDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

		MessageBox(L"������·�����óɹ�");
	}
	else
	{
		MessageBox(L"������·������ʧ�ܣ���ȷ��cwRsync�ļ����Ƿ����");
		return;
	}


	CDialogEx::OnOK();
}


BOOL CRemoteServerPathSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CRemoteServerPathSetDlg::OnBnClickedButtonRemotetest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//ȫ�´���log�ļ�
	SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES),NULL,TRUE };
	HANDLE cmdOutput = CreateFile(L".\\cwRsync\\rsynctest.log",
		GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		&sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (cmdOutput != INVALID_HANDLE_VALUE)
	{
		CloseHandle(cmdOutput);
	}

	//ƴ�Ӳ�������
	CString cmd;
	GetDlgItemText(IDC_EDIT_ServerPath, cmd);
	cmd.TrimRight('/');
	cmd += '/';
	cmd = L".\\cwRsync\\rsync.exe " + cmd;
	cmd = cmd + L" --log-file .\\cwRsync\\rsynctest.log";

	//ִ�нű�����
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
		MessageBox(L"����Rsync����ʧ�ܣ�");
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
			MessageBox(L"���Գɹ���");
		}
		else
		{
			CString err;
			err.Format(L"����ʧ�ܣ������� = %d\n", exeCode);

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
