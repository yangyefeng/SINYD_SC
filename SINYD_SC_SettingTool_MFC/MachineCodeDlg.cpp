// MachineCodeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SINYD_SC_SettingTool_MFC.h"
#include "MachineCodeDlg.h"
#include "afxdialogex.h"
#include "..\SINYD_SC_Crypt\CryptTool.h"
#include <string>
#include "..\SINYD_SC_Register\SCRegisteTool.h"

// CMachineCodeDlg 对话框

IMPLEMENT_DYNAMIC(CMachineCodeDlg, CDialogEx)

CMachineCodeDlg::CMachineCodeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{
	CCryptTool tool;
	string machineCode = tool.GetSecretMachineCodeWithTimeFromLocal();
	m_strMachineCode = CA2T(machineCode.c_str());

}

CMachineCodeDlg::~CMachineCodeDlg()
{
}

void CMachineCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editMCode);
}

BOOL CMachineCodeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetDlgItemText(IDC_EDIT1, m_strMachineCode);
	return 0;
}


BEGIN_MESSAGE_MAP(CMachineCodeDlg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &CMachineCodeDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMachineCodeDlg::OnBnClickedButtonRegist)
END_MESSAGE_MAP()


// CMachineCodeDlg 消息处理程序


void CMachineCodeDlg::OnPaint()
{

	CDialogEx::OnPaint();
}

void CMachineCodeDlg::OnBnClickedButton1()
{
	if (m_strMachineCode.GetLength() <= 0)
	{
		MessageBox(L"未生成机器码，无法复制", L"提示", MB_OK);
	}
	else
	{
		if (!OpenClipboard())
		{
			AfxMessageBox(_T("Cannot open the Clipboard"));
			return;
		}
		// Remove the current Clipboard contents   
		if (!EmptyClipboard())
		{
			AfxMessageBox(_T("Cannot empty the Clipboard"));
			return;
		}

		// global memory of data
		size_t cbStr = (m_strMachineCode.GetLength() + 1) * sizeof(TCHAR);
		HGLOBAL hData = GlobalAlloc(GMEM_MOVEABLE, cbStr);
		memcpy_s(GlobalLock(hData), cbStr, m_strMachineCode.LockBuffer(), cbStr);
		GlobalUnlock(hData);
		m_strMachineCode.UnlockBuffer();

		// For the appropriate data formats...
		UINT uiFormat = (sizeof(TCHAR) == sizeof(WCHAR)) ? CF_UNICODETEXT : CF_TEXT;
		if (::SetClipboardData(uiFormat, hData) == NULL)
		{
			AfxMessageBox(_T("Unable to set Clipboard data"));
			CloseClipboard();
			return;
		}

		CloseClipboard();
	}
}


void CMachineCodeDlg::OnBnClickedButtonRegist()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strKey;
	GetDlgItemText(IDC_EDIT2, strKey);

	if (strKey.GetLength() != SERIAL_KEY_STR_LEN)
	{
		MessageBox(L"注册码无效，请输入有效的注册码", L"提示", MB_OK);
	}
	else
	{
		CSCRegisteTool reg;
		VERIFIER_STATUS stat = reg.RegistSoft(string(CT2A(strKey)));
		if (stat == HasRegisted)
		{
			MessageBox(L"注册成功", L"提示", MB_OK);
		}
		else
		{
			MessageBox(L"注册码无效，请输入有效的注册码", L"提示", MB_OK);
		}
	}
}
