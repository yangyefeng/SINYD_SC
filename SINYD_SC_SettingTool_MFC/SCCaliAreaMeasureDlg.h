#pragma once
#include "..\SINYD_SC_CommonUtility\CCalibrationConfig.h"


// SCCaliAreaMeasureDlg 对话框

class SCCaliAreaMeasureDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SCCaliAreaMeasureDlg)

public:
	SCCaliAreaMeasureDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SCCaliAreaMeasureDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MEASURE_FUNCION_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_measure_type;
	int m_v1_start;
	int m_v1_end;
	long m_v1_start_delay;
	long m_v1_end_delay;
	int m_v2_start;
	int m_v2_end;
	long m_v2_start_delay;
	long m_v2_end_delay;
	afx_msg void OnBnClickedQuickDecrement();
	afx_msg void OnBnClickedQuickIncrement();
	afx_msg void OnBnClickedQuickCalc();
	afx_msg void OnBnClickedQuickSet();
	afx_msg void OnBnClickedQuickTemp();

	static AreaMeasureFunction DefaultIncrementFunc();
	static AreaMeasureFunction DefaultDecrementFunc();
	static AreaMeasureFunction DefaultCalcFunc();
	static AreaMeasureFunction DefaultSettingFunc();
	static AreaMeasureFunction DefaultTemperatureFunc();


	void SetAreaMeasureFunction(const AreaMeasureFunction& func);
	AreaMeasureFunction GetAreaMeasureFunction();
private:
	AreaMeasureFunction measure_func;

private:
	void ShowSecondValueCtrl(BOOL bEnable);

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	int m_v1_calc_type;
	int m_v2_calc_type;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioMeasureType1();
	afx_msg void OnBnClickedRadioMeasureType2();
};
