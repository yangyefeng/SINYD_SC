
// SCSettingToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SINYD_SC_SettingTool_MFC.h"
#include "SCSettingToolDlg.h"
#include "SCSettingUIDlg.h"
#include "afxdialogex.h"
#include "SCCalibrationUIDlg.h"
#include "MachineCodeDlg.h"
#include "..\SINYD_SC_Register\SCRegisteTool.h"
#include <atlconv.h>
#include "SCAppMgrDlg.h"
#include "RemoteServerPathSetDlg.h"
#include "DBRecordViewDlg.h"
#include "DigitToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// SCSettingToolDlg 对话框



SCSettingToolDlg::SCSettingToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SINYD_SC_SETTINGTOOL_MFC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void SCSettingToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(SCSettingToolDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(btnSettingUI, &SCSettingToolDlg::OnBnClickedbtnsettingui)
	ON_BN_CLICKED(btnCalibration, &SCSettingToolDlg::OnBnClickedbtncalibration)
	ON_BN_CLICKED(IDC_BUTTON1, &SCSettingToolDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_CollecMgr, &SCSettingToolDlg::OnBnClickedButtonCollecmgr)
	ON_BN_CLICKED(IDC_BUTTON_RemoteInfo, &SCSettingToolDlg::OnBnClickedButtonRemoteinfo)
	ON_BN_CLICKED(IDC_BUTTON_RecordView, &SCSettingToolDlg::OnBnClickedButtonRecordview)
	ON_BN_CLICKED(IDC_BUTTON_DigitSetting, &SCSettingToolDlg::OnBnClickedButtonDigitsetting)
END_MESSAGE_MAP()


// SCSettingToolDlg 消息处理程序

BOOL SCSettingToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	UpdateRegStatText();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void SCSettingToolDlg::UpdateRegStatText()
{
	CSCRegisteTool tool;
	m_regStat = tool.GetSoftRegistStatus();
	if (m_regStat == UnRegisted)
	{
		SetDlgItemText(IDC_STATIC_REGSTAT, L"软件未注册，请联系相关人员进行注册！");
	}
	else if (m_regStat == HasRegisted)
	{

		string time = tool.GetSoftRegistDurationTime();
		if (time == INFINITE_KEY_STR)
		{
			time = "永久版权";
		}
		CString text = L"软件已注册，到期时间：";
		text = text + CA2T(time.c_str());
		SetDlgItemText(IDC_STATIC_REGSTAT, text);
	}
	else if (m_regStat == OutTime)
	{
		string time = tool.GetSoftRegistDurationTime();
		CString text = L"软件已到期，到期时间：";
		text = text + CA2T(time.c_str());
		text = text + L"\n请联系相关人员重新进行注册！";
		SetDlgItemText(IDC_STATIC_REGSTAT, text);
	}

	if (m_regStat == HasRegisted)
	{
		GetDlgItem(IDC_BUTTON_CollecMgr)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_CollecMgr)->EnableWindow(FALSE);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void SCSettingToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR SCSettingToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void SCSettingToolDlg::OnBnClickedbtnsettingui()
{
	// TODO: 在此添加控件通知处理程序代码
	SCSettingUIDlg uiDlg;
	uiDlg.DoModal();
}


void SCSettingToolDlg::OnBnClickedbtncalibration()
{
	// TODO: 在此添加控件通知处理程序代码
	//CaliTest test;
	//test.DoModal();
	SCCalibrationUIDlg cali;
	cali.DoModal();
}


void SCSettingToolDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CMachineCodeDlg dlg;
	dlg.DoModal();

	UpdateRegStatText();
}


void SCSettingToolDlg::OnBnClickedButtonCollecmgr()
{
	// TODO: 在此添加控件通知处理程序代码
	CSCAppMgrDlg dlg;

	dlg.DoModal();
}


void SCSettingToolDlg::OnBnClickedButtonRemoteinfo()
{
	// TODO: 在此添加控件通知处理程序代码

	CRemoteServerPathSetDlg dlg;
	dlg.DoModal();
}


void SCSettingToolDlg::OnBnClickedButtonRecordview()
{
	// TODO: 在此添加控件通知处理程序代码
	CDBRecordViewDlg dlg;
	dlg.DoModal();
}


void SCSettingToolDlg::OnBnClickedButtonDigitsetting()
{
	// TODO: 在此添加控件通知处理程序代码
	CDigitToolDlg dlg;
	dlg.DoModal();
}
