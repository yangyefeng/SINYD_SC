
// SINYD_SC_keyGenDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxdtctl.h"


// CSINYD_SC_keyGenDlg �Ի���
class CSINYD_SC_keyGenDlg : public CDialogEx
{
// ����
public:
	CSINYD_SC_keyGenDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SINYD_SC_KEYGEN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
