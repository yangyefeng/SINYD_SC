// SCCaliAreaMeasureDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SINYD_SC_SettingTool_MFC.h"
#include "SCCaliAreaMeasureDlg.h"
#include "afxdialogex.h"
#include "..\SINYD_SC_CommonUtility\CCalibrationConfig.h"


// SCCaliAreaMeasureDlg �Ի���

IMPLEMENT_DYNAMIC(SCCaliAreaMeasureDlg, CDialogEx)

SCCaliAreaMeasureDlg::SCCaliAreaMeasureDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MEASURE_FUNCION_DIALOG, pParent)
	, m_measure_type(0)
	, m_v1_start(0)
	, m_v1_end(0)
	, m_v1_start_delay(0)
	, m_v1_end_delay(0)
	, m_v2_start(0)
	, m_v2_end(0)
	, m_v2_start_delay(0)
	, m_v2_end_delay(0)
	, m_v1_calc_type(0)
	, m_v2_calc_type(0)
{

}

SCCaliAreaMeasureDlg::~SCCaliAreaMeasureDlg()
{
}

void SCCaliAreaMeasureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_MEASURE_TYPE_1, m_measure_type);
	DDV_MinMaxInt(pDX, m_measure_type, 0, 1);
	DDX_CBIndex(pDX, IDC_COMBO_V1_S, m_v1_start);
	DDV_MinMaxInt(pDX, m_v1_start, 0, 3);
	DDX_CBIndex(pDX, IDC_COMBO_V1_E, m_v1_end);
	DDV_MinMaxInt(pDX, m_v1_end, 0, 3);
	DDX_Text(pDX, IDC_COMBO_V1_S_D, m_v1_start_delay);
	DDX_Text(pDX, IDC_EDIT_V1_E_D, m_v1_end_delay);
	DDX_CBIndex(pDX, IDC_COMBO_V2_S, m_v2_start);
	DDV_MinMaxInt(pDX, m_v2_start, 0, 3);
	DDX_CBIndex(pDX, IDC_COMBO_V2_E, m_v2_end);
	DDV_MinMaxInt(pDX, m_v2_end, 0, 3);
	DDX_Text(pDX, IDC_EDIT_V2_S_D, m_v2_start_delay);
	DDX_Text(pDX, IDC_EDIT_V2_E_D, m_v2_end_delay);
	DDX_CBIndex(pDX, IDC_COMBO1, m_v1_calc_type);
	DDV_MinMaxInt(pDX, m_v1_calc_type, 0, 4);
	DDX_CBIndex(pDX, IDC_COMBO2, m_v2_calc_type);
	DDV_MinMaxInt(pDX, m_v2_calc_type, 0, 4);
}


BEGIN_MESSAGE_MAP(SCCaliAreaMeasureDlg, CDialogEx)
	ON_BN_CLICKED(IDC_QUICK_DECREMENT, &SCCaliAreaMeasureDlg::OnBnClickedQuickDecrement)
	ON_BN_CLICKED(IDC_QUICK_INCREMENT, &SCCaliAreaMeasureDlg::OnBnClickedQuickIncrement)
	ON_BN_CLICKED(IDC_QUICK_CALC, &SCCaliAreaMeasureDlg::OnBnClickedQuickCalc)
	ON_BN_CLICKED(IDC_QUICK_SET, &SCCaliAreaMeasureDlg::OnBnClickedQuickSet)
	ON_BN_CLICKED(IDC_QUICK_TEMP, &SCCaliAreaMeasureDlg::OnBnClickedQuickTemp)
	ON_BN_CLICKED(IDOK, &SCCaliAreaMeasureDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &SCCaliAreaMeasureDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADIO_MEASURE_TYPE_1, &SCCaliAreaMeasureDlg::OnBnClickedRadioMeasureType1)
	ON_BN_CLICKED(IDC_RADIO_MEASURE_TYPE_2, &SCCaliAreaMeasureDlg::OnBnClickedRadioMeasureType2)
END_MESSAGE_MAP()


void SCCaliAreaMeasureDlg::SetAreaMeasureFunction(const AreaMeasureFunction& func)
{
	//ȡ�ñ궨������Ϣ���ָ��
	measure_func = func;
	m_measure_type = measure_func.method_type > 0 ? measure_func.method_type - 1 : measure_func.method_type;
	m_v1_start = measure_func.v1_start_at > 0 ? measure_func.v1_start_at - 1 : measure_func.v1_start_at;
	m_v1_start_delay = measure_func.v1_start_delay;
	m_v1_end = measure_func.v1_end_at > 0 ? measure_func.v1_end_at - 1 : measure_func.v1_end_at;
	m_v1_end_delay = measure_func.v1_end_delay;
	m_v1_calc_type = measure_func.v1_calc_method > 0 ? measure_func.v1_calc_method - 1 : measure_func.v1_calc_method;
	m_v2_start = measure_func.v2_start_at > 0 ? measure_func.v2_start_at - 1 : measure_func.v2_start_at;
	m_v2_start_delay = measure_func.v2_start_delay;
	m_v2_end = measure_func.v2_end_at > 0 ? measure_func.v2_end_at - 1 : measure_func.v2_end_at;
	m_v2_end_delay = measure_func.v2_end_delay;
	m_v2_calc_type = measure_func.v2_calc_method > 0 ? measure_func.v2_calc_method - 1 : measure_func.v2_calc_method;
}

// SCCaliAreaMeasureDlg ��Ϣ�������


void SCCaliAreaMeasureDlg::OnBnClickedQuickDecrement()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetAreaMeasureFunction(DefaultDecrementFunc());
	UpdateData(FALSE);
	ShowSecondValueCtrl(TRUE);
}


void SCCaliAreaMeasureDlg::OnBnClickedQuickIncrement()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetAreaMeasureFunction(DefaultIncrementFunc());
	UpdateData(FALSE);
	ShowSecondValueCtrl(TRUE);
}


void SCCaliAreaMeasureDlg::OnBnClickedQuickCalc()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetAreaMeasureFunction(DefaultCalcFunc());
	UpdateData(FALSE);
	ShowSecondValueCtrl(FALSE);
}


void SCCaliAreaMeasureDlg::OnBnClickedQuickSet()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetAreaMeasureFunction(DefaultSettingFunc());
	UpdateData(FALSE);
	ShowSecondValueCtrl(FALSE);
}


void SCCaliAreaMeasureDlg::OnBnClickedQuickTemp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetAreaMeasureFunction(DefaultTemperatureFunc());
	UpdateData(FALSE);
	ShowSecondValueCtrl(FALSE);
}

AreaMeasureFunction SCCaliAreaMeasureDlg::DefaultIncrementFunc()
{
	//��������Ĭ���㷨
	AreaMeasureFunction func = { 0 };
	func.method_type = 2;	//1:��ֵ  2:��ֵ
	func.v1_start_at = 1;	//1:���Ͽ�ʼ 2:���Ͻ��� 3:���Ͽ�ʼ 4:���Ͻ���
	func.v1_start_delay = 0; //ms
	func.v1_end_at = 3;		//1:���Ͽ�ʼ 2:���Ͻ��� 3:���Ͽ�ʼ 4:���Ͻ���
	func.v1_end_delay = 0;	//ms
	func.v1_calc_method = 3;	//1:��һ��ֵ 2:���һ��ֵ 3:���ֵ 4:��Сֵ 5:ƽ��ֵ
	func.v2_start_at = 1;	//1:���Ͽ�ʼ 2:���Ͻ��� 3:���Ͽ�ʼ 4:���Ͻ���
	func.v2_start_delay = 0;	//ms
	func.v2_end_at = 3;		//1:���Ͽ�ʼ 2:���Ͻ��� 3:���Ͽ�ʼ 4:���Ͻ���
	func.v2_end_delay = 0;	//ms
	func.v2_calc_method = 4;	//1:��һ��ֵ 2:���һ��ֵ 3:���ֵ 4:��Сֵ 5:ƽ��ֵ

	return func;
}

AreaMeasureFunction SCCaliAreaMeasureDlg::DefaultDecrementFunc()
{
	//��������Ĭ���㷨
	AreaMeasureFunction func = { 0 };
	func.method_type = 2;	//1:��ֵ  2:��ֵ
	func.v1_start_at = 1;	//1:���Ͽ�ʼ 2:���Ͻ��� 3:���Ͽ�ʼ 4:���Ͻ���
	func.v1_start_delay = 0; //ms
	func.v1_end_at = 3;		//1:���Ͽ�ʼ 2:���Ͻ��� 3:���Ͽ�ʼ 4:���Ͻ���
	func.v1_end_delay = 0;	//ms
	func.v1_calc_method = 3;	//1:��һ��ֵ 2:���һ��ֵ 3:���ֵ 4:��Сֵ 5:ƽ��ֵ
	func.v2_start_at = 2;	//1:���Ͽ�ʼ 2:���Ͻ��� 3:���Ͽ�ʼ 4:���Ͻ���
	func.v2_start_delay = 0;	//ms
	func.v2_end_at = 4;		//1:���Ͽ�ʼ 2:���Ͻ��� 3:���Ͽ�ʼ 4:���Ͻ���
	func.v2_end_delay = 0;	//ms
	func.v2_calc_method = 4;	//1:��һ��ֵ 2:���һ��ֵ 3:���ֵ 4:��Сֵ 5:ƽ��ֵ

	return func;
}

AreaMeasureFunction SCCaliAreaMeasureDlg::DefaultCalcFunc()
{
	//����ֵ����Ĭ���㷨
	AreaMeasureFunction func = {0};
	func.method_type = 1;	//1:��ֵ  2:��ֵ
	func.v1_start_at = 1;	//1:���Ͽ�ʼ 2:���Ͻ��� 3:���Ͽ�ʼ 4:���Ͻ���
	func.v1_start_delay = 0; //ms
	func.v1_end_at = 4;		//1:���Ͽ�ʼ 2:���Ͻ��� 3:���Ͽ�ʼ 4:���Ͻ���
	func.v1_end_delay = 0;	//ms
	func.v1_calc_method = 3;	//1:��һ��ֵ 2:���һ��ֵ 3:���ֵ 4:��Сֵ 5:ƽ��ֵ

	return func;
}

AreaMeasureFunction SCCaliAreaMeasureDlg::DefaultSettingFunc()
{
	//�趨ֵ����Ĭ���㷨
	AreaMeasureFunction func = { 0 };
	func.method_type = 1;	//1:��ֵ  2:��ֵ
	func.v1_start_at = 1;	//1:���Ͽ�ʼ 2:���Ͻ��� 3:���Ͽ�ʼ 4:���Ͻ���
	func.v1_start_delay = 0; //ms
	func.v1_end_at = 2;		//1:���Ͽ�ʼ 2:���Ͻ��� 3:���Ͽ�ʼ 4:���Ͻ���
	func.v1_end_delay = 0;	//ms
	func.v1_calc_method = 2;	//1:��һ��ֵ 2:���һ��ֵ 3:���ֵ 4:��Сֵ 5:ƽ��ֵ

	return func;
}

AreaMeasureFunction SCCaliAreaMeasureDlg::DefaultTemperatureFunc()
{
	//�¶�Ĭ���㷨
	AreaMeasureFunction func = { 0 };
	func.method_type = 1;	//1:��ֵ  2:��ֵ
	func.v1_start_at = 1;	//1:���Ͽ�ʼ 2:���Ͻ��� 3:���Ͽ�ʼ 4:���Ͻ���
	func.v1_start_delay = 0; //ms
	func.v1_end_at = 3;		//1:���Ͽ�ʼ 2:���Ͻ��� 3:���Ͽ�ʼ 4:���Ͻ���
	func.v1_end_delay = 0;	//ms
	func.v1_calc_method = 3;	//1:��һ��ֵ 2:���һ��ֵ 3:���ֵ 4:��Сֵ 5:ƽ��ֵ

	return func;
}

AreaMeasureFunction SCCaliAreaMeasureDlg::GetAreaMeasureFunction()
{
	return measure_func;
}


void SCCaliAreaMeasureDlg::ShowSecondValueCtrl(BOOL bEnable)
{
	GetDlgItem(IDC_COMBO_V2_S)->ShowWindow(bEnable ? SW_SHOWDEFAULT : SW_HIDE);
	GetDlgItem(IDC_COMBO_V2_E)->ShowWindow(bEnable ? SW_SHOWDEFAULT : SW_HIDE);
	GetDlgItem(IDC_EDIT_V2_S_D)->ShowWindow(bEnable ? SW_SHOWDEFAULT : SW_HIDE);
	GetDlgItem(IDC_EDIT_V2_E_D)->ShowWindow(bEnable ? SW_SHOWDEFAULT : SW_HIDE);
	GetDlgItem(IDC_COMBO2)->ShowWindow(bEnable ? SW_SHOWDEFAULT : SW_HIDE);

	GetDlgItem(IDC_STATIC_V2_1)->ShowWindow(bEnable ? SW_SHOWDEFAULT : SW_HIDE);
	GetDlgItem(IDC_STATIC_V2_2)->ShowWindow(bEnable ? SW_SHOWDEFAULT : SW_HIDE);
	GetDlgItem(IDC_STATIC_V2_3)->ShowWindow(bEnable ? SW_SHOWDEFAULT : SW_HIDE);
	GetDlgItem(IDC_STATIC_V2_4)->ShowWindow(bEnable ? SW_SHOWDEFAULT : SW_HIDE);
	GetDlgItem(IDC_STATIC_V2_5)->ShowWindow(bEnable ? SW_SHOWDEFAULT : SW_HIDE);
	GetDlgItem(IDC_STATIC_V2_6)->ShowWindow(bEnable ? SW_SHOWDEFAULT : SW_HIDE);
	GetDlgItem(IDC_STATIC_V2_7)->ShowWindow(bEnable ? SW_SHOWDEFAULT : SW_HIDE);
	GetDlgItem(IDC_STATIC_V2_8)->ShowWindow(bEnable ? SW_SHOWDEFAULT : SW_HIDE);
}

void SCCaliAreaMeasureDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();

	UpdateData(TRUE);
	measure_func.method_type = m_measure_type + 1;
	measure_func.v1_calc_method = m_v1_calc_type + 1;
	measure_func.v1_start_at = m_v1_start + 1;
	measure_func.v1_start_delay = m_v1_start_delay;
	measure_func.v1_end_at = m_v1_end + 1;
	measure_func.v1_end_delay = m_v1_end_delay;
	measure_func.v2_calc_method = m_v2_calc_type + 1;
	measure_func.v2_start_at = m_v2_start + 1;
	measure_func.v2_start_delay = m_v2_start_delay;
	measure_func.v2_end_at = m_v2_end + 1;
	measure_func.v2_end_delay = m_v2_end_delay;
}


void SCCaliAreaMeasureDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


BOOL SCCaliAreaMeasureDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	UpdateData(FALSE);
	if (m_measure_type == 0)	//��ֵ
	{
		ShowSecondValueCtrl(FALSE);
	}
	else	//��ֵ
	{
		ShowSecondValueCtrl(TRUE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}



void SCCaliAreaMeasureDlg::OnBnClickedRadioMeasureType1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (m_measure_type == 0)	//��ֵ
	{
		ShowSecondValueCtrl(FALSE);
	}
	else	//��ֵ
	{
		ShowSecondValueCtrl(TRUE);
	}
}


void SCCaliAreaMeasureDlg::OnBnClickedRadioMeasureType2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (m_measure_type == 0)	//��ֵ
	{
		ShowSecondValueCtrl(FALSE);
	}
	else	//��ֵ
	{
		ShowSecondValueCtrl(TRUE);
	}
}
