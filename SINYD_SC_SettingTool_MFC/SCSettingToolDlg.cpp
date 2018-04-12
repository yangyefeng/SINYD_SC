
// SCSettingToolDlg.cpp : ʵ���ļ�
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


// SCSettingToolDlg �Ի���



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


// SCSettingToolDlg ��Ϣ�������

BOOL SCSettingToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	UpdateRegStatText();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void SCSettingToolDlg::UpdateRegStatText()
{
	CSCRegisteTool tool;
	m_regStat = tool.GetSoftRegistStatus();
	if (m_regStat == UnRegisted)
	{
		SetDlgItemText(IDC_STATIC_REGSTAT, L"���δע�ᣬ����ϵ�����Ա����ע�ᣡ");
	}
	else if (m_regStat == HasRegisted)
	{

		string time = tool.GetSoftRegistDurationTime();
		if (time == INFINITE_KEY_STR)
		{
			time = "���ð�Ȩ";
		}
		CString text = L"�����ע�ᣬ����ʱ�䣺";
		text = text + CA2T(time.c_str());
		SetDlgItemText(IDC_STATIC_REGSTAT, text);
	}
	else if (m_regStat == OutTime)
	{
		string time = tool.GetSoftRegistDurationTime();
		CString text = L"����ѵ��ڣ�����ʱ�䣺";
		text = text + CA2T(time.c_str());
		text = text + L"\n����ϵ�����Ա���½���ע�ᣡ";
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void SCSettingToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR SCSettingToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void SCSettingToolDlg::OnBnClickedbtnsettingui()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SCSettingUIDlg uiDlg;
	uiDlg.DoModal();
}


void SCSettingToolDlg::OnBnClickedbtncalibration()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CaliTest test;
	//test.DoModal();
	SCCalibrationUIDlg cali;
	cali.DoModal();
}


void SCSettingToolDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMachineCodeDlg dlg;
	dlg.DoModal();

	UpdateRegStatText();
}


void SCSettingToolDlg::OnBnClickedButtonCollecmgr()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CSCAppMgrDlg dlg;

	dlg.DoModal();
}


void SCSettingToolDlg::OnBnClickedButtonRemoteinfo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CRemoteServerPathSetDlg dlg;
	dlg.DoModal();
}


void SCSettingToolDlg::OnBnClickedButtonRecordview()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDBRecordViewDlg dlg;
	dlg.DoModal();
}


void SCSettingToolDlg::OnBnClickedButtonDigitsetting()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDigitToolDlg dlg;
	dlg.DoModal();
}
