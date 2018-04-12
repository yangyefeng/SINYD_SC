
// SCSettingToolDlg.h : ͷ�ļ�
//

#pragma once
#include "..\SINYD_SC_Crypt\CryptTool.h"


// SCSettingToolDlg �Ի���
class SCSettingToolDlg : public CDialogEx
{
// ����
public:
	SCSettingToolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SINYD_SC_SETTINGTOOL_MFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
