#pragma once


// CSCAppMgrDlg �Ի���

class CSCAppMgrDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSCAppMgrDlg)

public:
	CSCAppMgrDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSCAppMgrDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonStartApp();
	afx_msg void OnBnClickedButtonStopApp();
	afx_msg void OnBnClickedButtonStartPNG();
	afx_msg void OnBnClickedButtonStopPNG();

	void updateAppState();
	void updateCapturePNGState();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButtonClearpng();
};
