#pragma once
#include "..\SINYD_SC_CommonUtility\CCalibrationConfig.h"
#include "afxwin.h"


// SCCaliLoadOldDlg 对话框

class SCCaliLoadOldDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SCCaliLoadOldDlg)

public:
	SCCaliLoadOldDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SCCaliLoadOldDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CALIUI_LOADDOC_MFC_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	void SetPCaliClass(CCalibrationConfig* pCaliClass);

private:
	CComboBox comboCaliName;
	CCalibrationConfig* pCaliConfig;

	void InitCaliNameCombo();
	/*string ws2s(const wstring& ws, const char* clocale = "");
	wstring s2ws(const string& s, const char* clocale = "");*/

public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();
};
