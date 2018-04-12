#pragma once
#include "afxcmn.h"
#include <vector>
#include <string>
#include "afxwin.h"


// CDBRecordViewDlg �Ի���
using namespace std;

class CDBRecordViewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDBRecordViewDlg)

public:
	CDBRecordViewDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDBRecordViewDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DBView };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_recordListCtrl;
	virtual BOOL OnInitDialog();
	void RefreshDBFileList();

private:
	CString m_curDBFileName;

public:
	afx_msg void OnBnClickedButtonRefreshrecord();
	CListCtrl m_dbFiles;
	afx_msg void OnBnClickedButtonRefreshfile();
	afx_msg void OnNMDblclkListDbfiles(NMHDR *pNMHDR, LRESULT *pResult);
};
