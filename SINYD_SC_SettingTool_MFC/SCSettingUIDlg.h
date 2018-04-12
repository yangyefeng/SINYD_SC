
// SCSettingUIDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "..\SINYD_SC_CommonUtility\CSettingConfig.h"
#include "..\SINYD_SC_CommonUtility\DialogUtility.h"
//#include "..\InterfaceInclude\Measurelogic.h"
//#include "..\InterfaceInclude\Measure.h"
#include "SCWriteConfigDef.h"

// SCSettingUIDlg �Ի���
class SCSettingUIDlg : public CDialogEx
{
	// ����
public:
	SCSettingUIDlg(CWnd* pParent = NULL);	// ��׼���캯��

											// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SINYD_SC_SETTINGUI_MFC_DIALOG };
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
	afx_msg void OnBnClickedButton1();
	//afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
private:
	CIPAddressCtrl SocketIP_Ctrl;
	CEdit CacheAdressEdit_Ctrl;
	//CEdit TargetSoftwarePathEdit_Ctrl;
	CEdit SocketPortEdit_Ctrl;
	CEdit UsedSpaceEdit_Ctrl;
	CEdit SaveTimeEdit_Ctrl;
	//CEdit SoftwareNameEdit_Ctrl;
	//CEdit ExeNameEdit_Ctrl;
	CEdit StationIDEdit_Ctrl;
	CEdit SectionIDEdit_Ctrl;
	CEdit ProjectIDEdit_Ctrl;
	CButton btnCancel;
	CButton btnSave;
	CComboBox comboMixType;
	CEdit edtThreadCount;
	CEdit edtFrequency;

	bool InitSettingConfig();
	void InitComboBox();
	void ShowDlg(const CSettingConfig& settingConfig);
	bool ReadDlgToEntry(CSettingConfig* settingConfig);
	void WriteConfigFile(const CSettingConfig& settingConfig);
	
public:
	afx_msg void OnBnClickedButtonIptest();
	int m_trans_protocal;
};


