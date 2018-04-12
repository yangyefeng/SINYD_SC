#pragma once
#include "afxwin.h"


// CMachineCodeDlg 对话框

class CMachineCodeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMachineCodeDlg)

public:
	CMachineCodeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMachineCodeDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
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
