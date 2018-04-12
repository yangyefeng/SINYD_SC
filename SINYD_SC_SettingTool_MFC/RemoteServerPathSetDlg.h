#pragma once


// CRemoteServerPathSetDlg 对话框

class CRemoteServerPathSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRemoteServerPathSetDlg)

public:
	CRemoteServerPathSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRemoteServerPathSetDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_RemoteSet };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonRemotetest();
};
