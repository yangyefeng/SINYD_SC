
// SCSettingToolDlg.h : 头文件
//

#pragma once
#include "..\SINYD_SC_Crypt\CryptTool.h"


// SCSettingToolDlg 对话框
class SCSettingToolDlg : public CDialogEx
{
// 构造
public:
	SCSettingToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SINYD_SC_SETTINGTOOL_MFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	void UpdateRegStatText();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedbtnsettingui();
	afx_msg void OnBnClickedbtncalibration();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonCollecmgr();

private:
	VERIFIER_STATUS m_regStat;
public:
	afx_msg void OnBnClickedButtonRemoteinfo();
	afx_msg void OnBnClickedButtonRecordview();
	afx_msg void OnBnClickedButtonDigitsetting();
};
