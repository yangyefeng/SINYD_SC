#pragma once
#include "afxwin.h"


// CMachineCodeDlg �Ի���

class CMachineCodeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMachineCodeDlg)

public:
	CMachineCodeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMachineCodeDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedButton1();

private:
	CString m_strMachineCode;
public:
	CEdit m_editMCode;
	afx_msg void OnBnClickedButtonRegist();
};
