
// SINYD_SC_keyGenDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxdtctl.h"


// CSINYD_SC_keyGenDlg 对话框
class CSINYD_SC_keyGenDlg : public CDialogEx
{
// 构造
public:
	CSINYD_SC_keyGenDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SINYD_SC_KEYGEN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editMachineID;
	CEdit m_editSerialKey;
	afx_msg void OnBnClickedBtngen();
	afx_msg void OnBnClickedBtnserialkeycopy();
	afx_msg void OnBnClickedCheck1();
	CButton m_INFINTECheck;
	//CEdit m_editDueTime;
	CDateTimeCtrl m_DTCtrl;
};
