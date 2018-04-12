#pragma once
#include "..\SINYD_SC_CommonUtility\CCalibrationConfig.h"
#include "..\SINYD_SC_CommonUtility\DialogUtility.h"
#include "afxwin.h"

#define WM_MYMSG_2 WM_USER + 111 //�Զ�����Ϣ�����ڴ�����깳�ӷ�������Ϣ

// SCCaliNewDlg �Ի���

class SCCaliNewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SCCaliNewDlg)

public:
	SCCaliNewDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SCCaliNewDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CALIUI_NEWDOC_MFC_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	void SetPCaliClass(CCalibrationConfig* pCaliClass);

private:
	CEdit edtSoftwareName;
	//CEdit edtSoftwarePath;
	CEdit edtVSoftwareVer;

	CCalibrationConfig* pCaliConfig;

public:
	//afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton1();
};
