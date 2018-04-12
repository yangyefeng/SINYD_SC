#pragma once
#include "ListCtrlEx.h"
#include "afxcmn.h"
#include "afxwin.h"
#include <list>
#include "DigitReader.h"
#include <map>
#include "..\SINYD_SC_CommonUtility\BitmapEx.h"

using namespace std;
// CDigitToolDlg �Ի���

class CDigitToolDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDigitToolDlg)

public:
	CDigitToolDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDigitToolDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SINYD_SC_DIGITTOOL_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	void InitAll();
	void InitImageList();
	void InitDigitList();
	void InitDigitViewList();
	void RefreshDigitViewList();
	void RefreshComboBoxDataList();
	void RefreshListCtrlData();
	void RefreshCurrentDigit(BOOL bClean = FALSE);
	void RefreshAll();

	string ConvertSpecialDigitValue(const string& value, bool isOrgToUI/*ԭʼ�ַ�UIתΪUI���ֵ�flag��trueΪԭʼתUI*/);
	void ListSelectDigit(int index);

protected:
	CListCtrlEx m_digit_list;
	CImageList m_image_list;		//��m_digits�е���ģһһ��Ӧ��indexʹ��m_digits��ÿ��digit���ڵ�index
	list<Digit> m_digits;			//���ļ��ж�ȡ��ȫ����ģ��Ϣ
	map<int, Digit*> m_digits_for_view;	//����UI��ʾ����ɸѡ�����ģ�б�����UI��ʾ��keyֵʹ��m_digits��ÿ��digit���ڵ�index����m_image_list��ÿ��image��index
	CStatic m_static_digitview;
	CToolTipCtrl m_EditValueToolTip;
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedCheckNotsetdigit();
	afx_msg void OnBnClickedCheckSetdigit();
	afx_msg void OnCbnSelchangeComboSetdigitres();
	afx_msg void OnBnClickedButtonDigitsavevalue();

	CButton m_check_not_set;
	CButton m_check_set;
	CComboBox m_select_set_value;

private:
	void ModelToBitmap(const Digit& d, CBitmapEx& bitmap);
public:
	afx_msg void OnNMClickListDigits(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnOK();

private:
	string m_xml_Name;
};
