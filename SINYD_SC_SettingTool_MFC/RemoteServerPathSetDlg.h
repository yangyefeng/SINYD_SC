#pragma once


// CRemoteServerPathSetDlg �Ի���

class CRemoteServerPathSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRemoteServerPathSetDlg)

public:
	CRemoteServerPathSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRemoteServerPathSetDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_RemoteSet };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonRemotetest();
};
