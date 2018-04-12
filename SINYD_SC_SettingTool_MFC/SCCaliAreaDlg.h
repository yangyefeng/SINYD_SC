#pragma once
#include "..\SINYD_SC_CommonUtility\CCalibrationConfig.h"
#include "..\SINYD_SC_CommonUtility\BitmapEx.h"
#include "SCOCR.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "SCWriteConfigDef.h"
#include "SCPluginDef.h"
#include <list>

#define AREA_DELETE 999  //用于删除area是返回成功值

// SCCaliAreaDlg 对话框

class SCCaliAreaDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SCCaliAreaDlg)

public:
	SCCaliAreaDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SCCaliAreaDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CALIUI_AREA_MFC_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void GetRectArea(RECT& rtSrcArea);

	void SetPCaliClass(CCalibrationConfig* pCaliClass);
	void SetRectArea(const RECT& rtSrcArea);
	void SetCBitmap(CBitmapEx& bitmapArea);
	void SetAreaInfo(int id);
	void SetCalcCoef(float coef);
	void SetHopperList(list<HopperStru>* HopperList);
	void InitComboBox();
	void BrushPicCtrl(CDC *pDC, CRect rctPic, DWORD rgb);
	void FillMonitorArea(MonitorArea *mArea);

	void CancelRectangle(CPoint POrigin, CPoint PEnd);

private:
	void MergeShowBMP(CBitmapEx& bitmapCali, CBitmapEx& bmpbinary);

private:
	int areaID;
	CCalibrationConfig* pCaliConfig;
	list<HopperStru>* pHopperList;

	bool bGetRGB;
	CPoint checkP;
	float pCoef;

	HBITMAP m_Screen;
	CBitmapEx bitmapCali, bitmapShow;
	RECT rtArea;
	vector<DWORD> vRgbs;

	//鼠标框选相关参数
	bool mouseSelect;
	CPoint PDown, PMove;

	static bool s_bCopyed;
	static vector<DWORD> s_listCopyColors;

	AreaMeasureFunction measure_functin;
	CString m_lastAreaType;

private:
	CComboBox comboAreaType;
	CStatic picArea;
	CButton btnDel;
	CListBox listAreaName;
	CButton btnAddColor;
	CButton btnDelColor;
	CListCtrl listCtrlColor;
	CEdit edtAberration;
	CButton btnCheck;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCustomdrawMyList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonCopy();
	afx_msg void OnBnClickedButtonPaste();
private:
	CButton m_btnPatse;
public:
	CButton m_btnCopy;
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButtonListcolor();
	afx_msg void OnBnClickedButtonAdvanced();

	static bool DefaultMeasureFunc(AreaMeasureFunction& func, const CString& areaType);
};
