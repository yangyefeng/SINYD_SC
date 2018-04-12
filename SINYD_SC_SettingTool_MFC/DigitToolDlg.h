#pragma once
#include "ListCtrlEx.h"
#include "afxcmn.h"
#include "afxwin.h"
#include <list>
#include "DigitReader.h"
#include <map>
#include "..\SINYD_SC_CommonUtility\BitmapEx.h"

using namespace std;
// CDigitToolDlg 对话框

class CDigitToolDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDigitToolDlg)

public:
	CDigitToolDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDigitToolDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SINYD_SC_DIGITTOOL_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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

	string ConvertSpecialDigitValue(const string& value, bool isOrgToUI/*原始字符UI转为UI汉字的flag，true为原始转UI*/);
	void ListSelectDigit(int index);

protected:
	CListCtrlEx m_digit_list;
	CImageList m_image_list;		//与m_digits中的字模一一对应，index使用m_digits中每个digit所在的index
	list<Digit> m_digits;			//从文件中读取的全部字模信息
	map<int, Digit*> m_digits_for_view;	//根据UI显示条件筛选后的字模列表，用于UI显示，key值使用m_digits中每个digit所在的index，即m_image_list中每个image的index
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
