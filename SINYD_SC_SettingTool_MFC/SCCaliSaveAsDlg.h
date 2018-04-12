#pragma once
#include "afxwin.h"
#include "..\SINYD_SC_CommonUtility\CCalibrationConfig.h"


// SCCaliSaveAsDlg �Ի���

class SCCaliSaveAsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SCCaliSaveAsDlg)

public:
	SCCaliSaveAsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SCCaliSaveAsDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CALIUI_SAVEAS_MFC_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CEdit edtNewName;
	CCalibrationConfig* pCaliConfig;

	/*string ws2s(const wstring& ws, const char* clocale = "");
	wstring s2ws(const string& s, const char* clocale = "");*/
	string strToLower(const string &str);
	bool compareNoCase(const string &strA, const string &strB);

public:
	void SetPCaliClass(CCalibrationConfig* pCaliClass);

	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
};
