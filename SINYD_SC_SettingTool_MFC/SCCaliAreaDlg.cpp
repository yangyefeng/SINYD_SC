// SCCaliAreaDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SINYD_SC_SettingTool_MFC.h"
#include "SCCaliAreaDlg.h"
#include "afxdialogex.h"
#include "afxcmn.h"
#include "SCCaliRectConversion.h"
#include <algorithm>
#include "SCCaliAreaMeasureDlg.h"


// SCCaliAreaDlg �Ի���

IMPLEMENT_DYNAMIC(SCCaliAreaDlg, CDialogEx)

bool SCCaliAreaDlg::s_bCopyed = false;
vector<DWORD> SCCaliAreaDlg::s_listCopyColors;


SCCaliAreaDlg::SCCaliAreaDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CALIUI_AREA_MFC_DIALOG, pParent)
{
	areaID = 0;
	bGetRGB = false;
	measure_functin = { 0 };
}

SCCaliAreaDlg::~SCCaliAreaDlg()
{
	vRgbs.clear();
	vector<DWORD>(vRgbs).swap(vRgbs);
}

void SCCaliAreaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, comboAreaType);
	DDX_Control(pDX, IDC_PIC_AREA, picArea);
	DDX_Control(pDX, IDC_BUTTON7, btnDel);
	DDX_Control(pDX, IDC_LIST1, listAreaName);
	DDX_Control(pDX, IDC_BUTTON8, btnAddColor);
	DDX_Control(pDX, IDC_BUTTON3, btnDelColor);
	DDX_Control(pDX, IDC_LIST2, listCtrlColor);
	DDX_Control(pDX, IDC_EDIT1, edtAberration);
	DDX_Control(pDX, IDC_BUTTON2, btnCheck);
	DDX_Control(pDX, IDC_BUTTON6, m_btnPatse);
	DDX_Control(pDX, IDC_BUTTON4, m_btnCopy);
}


BEGIN_MESSAGE_MAP(SCCaliAreaDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &SCCaliAreaDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON7, &SCCaliAreaDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDCANCEL, &SCCaliAreaDlg::OnBnClickedCancel)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON2, &SCCaliAreaDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON8, &SCCaliAreaDlg::OnBnClickedButton8)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON3, &SCCaliAreaDlg::OnBnClickedButton3)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST2, &SCCaliAreaDlg::OnCustomdrawMyList)
	ON_CBN_SELCHANGE(IDC_COMBO2, &SCCaliAreaDlg::OnCbnSelchangeCombo2)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON4, &SCCaliAreaDlg::OnBnClickedButtonCopy)
	ON_BN_CLICKED(IDC_BUTTON6, &SCCaliAreaDlg::OnBnClickedButtonPaste)
	ON_BN_CLICKED(IDC_BUTTON9, &SCCaliAreaDlg::OnBnClickedButton9)
	//ON_BN_CLICKED(IDC_BUTTON_ListColor, &SCCaliAreaDlg::OnBnClickedButtonListcolor)
	ON_BN_CLICKED(IDC_BUTTON_Advanced, &SCCaliAreaDlg::OnBnClickedButtonAdvanced)
END_MESSAGE_MAP()


// SCCaliAreaDlg ��Ϣ�������
BOOL SCCaliAreaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitComboBox();
	btnDel.EnableWindow(areaID != 0);
	picArea.MoveWindow(20, 20, 250, 250);
	//picture�ؼ��޸�type����Ϊbmp
	picArea.ModifyStyle(0, SS_BITMAP);

	//����������ʱ��δѡ�������ͣ��궨�����ذ�ť�ؼ�ȫ������
	btnAddColor.EnableWindow(FALSE);
	btnDelColor.EnableWindow(FALSE);
	edtAberration.EnableWindow(FALSE);
	btnCheck.EnableWindow(FALSE);
	m_btnPatse.EnableWindow(FALSE);
	m_btnCopy.EnableWindow(FALSE);
	GetDlgItem(IDC_ColorInvert)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_Advanced)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_Advanced)->ShowWindow(SW_HIDE);

	if (areaID != 0)
	{
		btnCheck.EnableWindow(TRUE);
		USES_CONVERSION;
		//�޸Ĳ���������������Ϣ
		CString areaName, strTemp, areaType;
		strTemp = CA2T(pCaliConfig->GetMonitorArea(areaID)->GetMonitorAreaName());
		//��MonitorAreaName�н�ȡname
		int index = strTemp.Find(_T("_"));
		while (index > 0)
		{
			areaName = strTemp.Left(index);
			strTemp = strTemp.Mid(index + 1);

			index = listAreaName.FindStringExact(0, areaName);
			if (index >= 0)
				listAreaName.SetSel(index, true);			

			index = strTemp.Find(_T("_"));
		}
		index = strTemp.Find(_T("@"));
		if (index > 0)
		{
			//��ȡtype֮ǰ�����һ��name��type
			areaName = strTemp.Left(index);
			areaType = strTemp.Mid(index + 1);
			index = listAreaName.FindStringExact(0, areaName);
			if (index >= 0)
				listAreaName.SetSel(index, true);
			index = comboAreaType.FindStringExact(0, areaType);
			if (index >= 0)
				comboAreaType.SetCurSel(index);
			//������������������ɫ�����Ӻ�ɾ����ť�Ƿ����
			index = areaType.Find(_T("����"));
			btnAddColor.EnableWindow(index >= 0);
			btnDelColor.EnableWindow(index >= 0);
			m_btnCopy.EnableWindow(index >= 0);
			GetDlgItem(IDC_ColorInvert)->EnableWindow(index >= 0);
			m_btnPatse.EnableWindow(index >= 0 && s_bCopyed);
			edtAberration.EnableWindow(index < 0);
			GetDlgItem(IDC_BUTTON_Advanced)->EnableWindow(index < 0);
			GetDlgItem(IDC_BUTTON_Advanced)->ShowWindow(index < 0 ? SW_SHOWDEFAULT : SW_HIDE);
			m_lastAreaType = areaType;
		}
		//����checkpoint��Ϣ
		checkP.x = pCaliConfig->GetMonitorArea(areaID)->GetMonitorAreaX();
		checkP.y = pCaliConfig->GetMonitorArea(areaID)->GetMonitorAreaY();
		//����ʶ�����Ϣ
		if (pCaliConfig->GetMonitorArea(areaID)->GetMonitorParaCount() > 0)
		{
			const vector<Parameter*>* pParameters = pCaliConfig->GetMonitorArea(areaID)->GetParameters();
			DWORD rgb;
			CString strRGB;
			for (int i = 0; i < pParameters->size(); i++)
			{
				CString strA = CA2T((*pParameters)[i]->GetParameterName());
				if (strA == _T(SC_COMMON_COLOR_THRESHOLD_PARAMNAME))
				{
					//�����ƫ�����͵�para��������ʾ��ɫ��ؼ���
					edtAberration.SetWindowTextW(CA2T((*pParameters)[i]->GetParameterValue()));
				}
				else if (strA == _T(SC_VALVE_IS_COLOR_INVERT))
				{
					const char* c = (*pParameters)[i]->GetParameterValue();
					if (c)
					{
						if (0 == strcmp(c, "1"))
						{
							((CButton*)GetDlgItem(IDC_ColorInvert))->SetCheck(TRUE);
						}
					}
				}
				else
				{
					//���������Ϊ��ɫRGBֵ
					rgb = atoll((*pParameters)[i]->GetParameterValue());
					vRgbs.push_back(rgb);
					strRGB.Format(_T("0x%06X"), rgb);
					listCtrlColor.InsertItem(i, strRGB);
				}
			}
		}

		//���ؼ����㷨
		measure_functin = pCaliConfig->GetMonitorArea(areaID)->GetMeasureFunction();
		if (measure_functin.method_type == 0 && measure_functin.v1_start_at == 0 && measure_functin.v1_end_at == 0)		//����Ƿ��ǿյļ����㷨
		{
			bool bSet = DefaultMeasureFunc(measure_functin, areaType);
			if (bSet)
			{
				//���ɹ�����ΪĬ�ϼ����㷨ʱ��˵������������Ҫ���ü����㷨��
				//����ʧ�ܴ�ʱӦ�����ھɰ汾δ�������õ�ԭ��
				//��ʱ��Ҫ�����������ô˴�Ĭ�ϵļ����㷨��
				pCaliConfig->GetMonitorArea(areaID)->SetMeasureFunction(measure_functin);
			}
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void SCCaliAreaDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strType;
	//��ȡ��������
	int nSel = listAreaName.GetSelCount();
	if (nSel == 0)
	{
		MessageBox(_T("������ѡ��һ����������"), _T("��ʾ"), MB_OK);
		return;
	}
	//��ȡ��������
	comboAreaType.GetWindowTextW(strType);
	if (strType == "")
	{
		MessageBox(_T("�������Ͳ���Ϊ��"), _T("��ʾ"), MB_OK);
		return;
	}
	//�ж�ʶ�������
	if (vRgbs.size() <= 0)
	{
		MessageBox(_T("������ʶ��һ���ؼ�����ɫ"), _T("��ʾ"), MB_OK);
		return;
	}
	//�ж�ʶ����Ƿ��ѱ궨
	if (checkP.x == 0 && checkP.y == 0)
	{
		MessageBox(_T("��궨ʶ�������"), _T("��ʾ"), MB_OK);
		return;
	}

	const vector<MonitorArea *>* areas = pCaliConfig->GetMonitorAreas();
	if (areaID == 0)
	{
		//��������
		int newID = 1;
		//���ݵ�ǰ���������ȡ�������ID�������ǰû�б궨������ID��Ϊ1
		if (areas->size() != 0)
		{
			//ȡ�õ�ǰ����MonitorAreaID
			for (int i = 0; i < areas->size(); i++)
			{
				if ((*areas)[i]->GetMonitorAreaID() >= newID)
					newID = (*areas)[i]->GetMonitorAreaID();
			}
			//�ڵ�ǰ���ID�ϼ�1��Ϊ�������ID
			newID = newID + 1;
		}
		//������������ϸ��Ϣ
		MonitorArea newM(newID);
		FillMonitorArea(&newM);
		//���������궨��������
		pCaliConfig->AddMonitorArea(&newM);
	}
	else
	{
		//�޸�����
		if (areas->size() != 0)
		{
			MonitorArea modifyM(areaID);
			FillMonitorArea(&modifyM);
			//ɾ����ǰid��Ӧ�ı궨�������е�����
			int result = pCaliConfig->DelMonitorArea(areaID);
			if (result == CONFIG_SUCCESS)
			{
				//ɾ���ɹ������������궨��������
				pCaliConfig->AddMonitorArea(&modifyM);
			}
			else
			{
				//ɾ��ʧ�ܣ���ʾ�û�û�ҵ���Ӧ����
				MessageBox(_T("����ĵ�ǰ����ID����û���ҵ�ƥ����"), _T("��ʾ"), MB_OK);
				return;
			}
		}
		else
		{
			//��ǰ�궨��������û��������ʾ�û�û�ҵ���Ӧ����
			MessageBox(_T("����ĵ�ǰ����ID����û���ҵ�ƥ����"), _T("��ʾ"), MB_OK);
			return;
		}
	}
	EndDialog(IDOK);
}

void SCCaliAreaDlg::SetPCaliClass(CCalibrationConfig* pCaliClass)
{
	//ȡ�ñ궨������Ϣ���ָ��
	pCaliConfig = pCaliClass;
}

void SCCaliAreaDlg::SetHopperList(list<HopperStru>* HopperList)
{
	//ȡ�����������ļ���HopperList��ָ��
	pHopperList = HopperList;
}

void SCCaliAreaDlg::SetRectArea(const RECT& rtSrcArea)
{
	//ȡ����������ͼ�ı궨������Ľ�����
	rtArea.left = rtSrcArea.left;
	rtArea.top = rtSrcArea.top;
	rtArea.right = rtSrcArea.right;
	rtArea.bottom = rtSrcArea.bottom;
}

void SCCaliAreaDlg::OnBnClickedButton7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//idΪ0����ʾ����ɾ�������״̬
	if (areaID == 0)
		return;

	//��ʾʧ�ܣ����رնԻ���
	int yesno = MessageBox(_T("ȷ��ɾ���˱궨����"), _T(""), MB_YESNO);
	if (yesno == IDNO)
	{
		return;
	}

	int result  = pCaliConfig->DelMonitorArea(areaID);
	//ɾ��area�ɹ��������Ի��򣬷����Զ����AREA_DELETE
	if (result == CONFIG_SUCCESS)
	{
		//��ʾ�ɹ����رնԻ���
		MessageBox(_T("ɾ���ɹ�"), _T("��ʾ"), MB_OK);
		EndDialog(AREA_DELETE);
	}
	else
	{
		//��ʾʧ�ܣ����رնԻ���
		MessageBox(_T("�궨��Ϣ��û�ж�ӦID�ı궨����ɾ��ʧ�ܣ�"), _T("��ʾ"), MB_OK);
		return;
	}
}

void SCCaliAreaDlg::SetCBitmap(CBitmapEx& bitmapArea)
{
	//�������ڱ궨��ԭ��ͼBMP
	bitmapCali.Create(bitmapArea);
	bitmapCali.SetResampleMode(RM_NEARESTNEIGHBOUR);
	//���ݴ�������λ�ô������ڷŴ�չʾ�Ľ�ͼBMP
	CBitmapEx bitbinary;
	bitbinary.Create(rtArea.right - rtArea.left + 1, rtArea.bottom - rtArea.top + 1);
	bitbinary.Draw2(0, 0, bitbinary.GetWidth(), bitbinary.GetHeight(), bitmapCali, rtArea.left, rtArea.top, bitbinary.GetWidth(), bitbinary.GetHeight());

	if (areaID != 0)//ָ������ɫ
	{
		//����ʶ�����Ϣ
		if (pCaliConfig->GetMonitorArea(areaID)->GetMonitorParaCount() > 0)
		{
			const vector<Parameter*>* pParameters = pCaliConfig->GetMonitorArea(areaID)->GetParameters();
			DWORD rgb = NULL;
			int iThreshold = 0;
			for (int i = 0; i < pParameters->size(); i++)
			{
				CString strA = CA2T((*pParameters)[i]->GetParameterName());
				if (strA == _T(SC_COMMON_COLOR_THRESHOLD_PARAMNAME))
				{
					iThreshold = _ttoi(CA2T((*pParameters)[i]->GetParameterValue()));
				}
				else if (rgb == NULL)
				{
					//���������Ϊ��ɫRGBֵ
					rgb = atoll((*pParameters)[i]->GetParameterValue());
				}
			}
			bitbinary.Binary_RBG(rgb, iThreshold);
		}
	}
	else
	{
		bitbinary.Grayscale();
		int iThresold = 0;
		bitbinary.Binary_OTSU(iThresold);
	}

// 	bitmapShow.Create(rtArea.right - rtArea.left + 1, (rtArea.bottom - rtArea.top + 1) * 2);
// 	bitmapShow.SetResampleMode(RM_NEARESTNEIGHBOUR);
// 	bitmapShow.Draw2(0, 0, rtArea.right - rtArea.left + 1, rtArea.bottom - rtArea.top + 1, bitmapCali, rtArea.left, rtArea.top, rtArea.right - rtArea.left + 1, rtArea.bottom - rtArea.top + 1);
// 	CBitmapEx line;
// 	line.Create(bitmapShow.GetWidth(), 1);
// 	line.Clear(0);
// 	bitmapShow.Draw2(0, rtArea.bottom - rtArea.top + 1, bitmapShow.GetWidth(), 1, line, 0, 0, line.GetWidth(), 1);
// 	bitmapShow.Draw2(0, rtArea.bottom - rtArea.top + 1 + 1, rtArea.right - rtArea.left + 1, rtArea.bottom - rtArea.top + 1, bitbinary, 0,0, bitbinary.GetWidth(), bitbinary.GetHeight());
// 	bitmapShow.Scale(100 * pCoef, 100 * pCoef);
	MergeShowBMP(bitmapCali, bitbinary);
}

void SCCaliAreaDlg::SetAreaInfo(int id)
{
	areaID = id;
}

void SCCaliAreaDlg::SetCalcCoef(float coef)
{
	pCoef = coef;
}

void SCCaliAreaDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���ָ��ظ�ԭ״
	SetClassLong(this->GetSafeHwnd(), GCLP_HCURSOR, (LONG)LoadCursor(NULL, IDC_ARROW));
	bitmapCali.Clear();
	bitmapShow.Clear();
	CDialogEx::OnCancel();
}

void SCCaliAreaDlg::InitComboBox()
{
	//�����������ѡ��
	listAreaName.ResetContent();
	comboAreaType.ResetContent();
	//��ʼ��������
	comboAreaType.InsertString(0, _T("���Ϸ���1"));
	comboAreaType.InsertString(1, _T("���Ϸ���2"));
	comboAreaType.InsertString(2, _T("���Ϸ���1"));
	comboAreaType.InsertString(3, _T("���Ϸ���2"));
	comboAreaType.InsertString(4, _T("�д�������"));
	comboAreaType.InsertString(5, _T("��"));
	comboAreaType.InsertString(6, _T("����ֵ"));
	comboAreaType.InsertString(7, _T("�趨ֵ"));
	comboAreaType.InsertString(8, _T("�¶�"));

	listAreaName.InsertString(0, _T("����1"));
	listAreaName.InsertString(1, _T("����2"));
	listAreaName.InsertString(2, _T("����3"));
	listAreaName.InsertString(3, _T("����4"));
	listAreaName.InsertString(4, _T("����5"));
	listAreaName.InsertString(5, _T("����6"));
	listAreaName.InsertString(6, _T("����1"));
	listAreaName.InsertString(7, _T("����2"));
	listAreaName.InsertString(8, _T("����3"));
	listAreaName.InsertString(9, _T("����4"));
	listAreaName.InsertString(10, _T("����5"));
	listAreaName.InsertString(11, _T("����6"));
	listAreaName.InsertString(12, _T("��μ�1"));
	listAreaName.InsertString(13, _T("��μ�2"));
	listAreaName.InsertString(14, _T("ľ����"));
	listAreaName.InsertString(15, _T("����"));
	listAreaName.InsertString(16, _T("ˮ"));
	listAreaName.InsertString(17, _T("�����"));

	//��ʼ��listCtrlColor
	DWORD dwStyle = listCtrlColor.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;// ѡ��ĳ��ʹ���и�����ֻ������report ����listctrl ��   
	listCtrlColor.SetExtendedStyle(dwStyle); // ������չ���   
	listCtrlColor.InsertColumn(0, _T("��ɫ"), LVCFMT_LEFT, 100);
	listCtrlColor.DeleteAllItems();
}

void SCCaliAreaDlg::OnPaint()
{
	CDialogEx::OnPaint();
	CDialogEx::UpdateWindow();
	//չʾ��ͼ
	bitmapShow.SetResampleMode(RM_NEARESTNEIGHBOUR);
	CDC* pDC = picArea.GetDC();		// add by yaodi for DC release
	bitmapShow.Draw(pDC->m_hDC);
	ReleaseDC(pDC);				// add by yaodi for DC release
}

void SCCaliAreaDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//ȡ���ʶ�������ı������̬Ϊʮ����
	bGetRGB = true;
	SetClassLong(this->GetSafeHwnd(), GCLP_HCURSOR, (LONG)LoadCursor(NULL, IDC_CROSS));
}


void SCCaliAreaDlg::OnBnClickedButton8()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (vRgbs.size() > 0)
	{
		//ȡ�õ�ǰʶ����rgb������rgb����
		DWORD rgb = bitmapCali.GetPixel(checkP.x, checkP.y);
		vRgbs.push_back(rgb);
		CString strRGB;
		strRGB.Format(_T("0x%06X"), rgb);
		//ȡ���б�ǰ��������������Ϊ�кŲ����µ�rgbֵ��Ϊ���У�Ĭ���б�ؼ����кŰ���˳���0��ʼ
		//ͨ��ɾ��ѡ����ʱ������ɾ�����²���ķ�ʽ����֤�кŵ�����ά��
		int colorNum = listCtrlColor.GetItemCount();
		listCtrlColor.InsertItem(colorNum, strRGB);
	}
}

void SCCaliAreaDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (bGetRGB)
	{
		RECT rtPicCtrl, rtPic;
		//ȡ��ͼƬ�ؼ����ڵ�ǰ���ڵ���Է�Χ��������ͼƬ��Χ��ȷ��ͼƬ��Χ��top��left
		picArea.GetWindowRect(&rtPicCtrl);
		ScreenToClient(&rtPicCtrl);
		rtPic = rtPicCtrl;
		//ͨ������ϵ��ȷ��ͼƬ��Χ�Ŀ�͸ߣ�����ͼƬ�ؼ���left��top����ȷ�����յ�ʵ��ͼƬ�ķ�Χbottom��right
		rtPic.right = pCoef * (rtArea.right - rtArea.left) + rtPic.left;
		rtPic.bottom = pCoef * (rtArea.bottom - rtArea.top) + rtPic.top;
		if (!PtInRect(&rtPic, point))
		{
			//�ж������������ͼƬ��Χ�ڣ�����ʶ��ؼ��㹦�ܽ���������ʾ�û�
			bGetRGB = false;
			SetClassLong(this->GetSafeHwnd(), GCLP_HCURSOR, (LONG)LoadCursor(NULL, IDC_ARROW));
			MessageBox(_T("����ͼƬ�ڱ궨�ؼ���"), _T("��ʾ"), MB_OK);
			CDialogEx::OnLButtonUp(nFlags, point);
			return;
		}
		//�궨ʶ���
		if (bGetRGB)
		{
			//���������ͼƬ��Χ�ڣ�ͨ������ϵ������ؼ�����������ͼ�е�����
			checkP.x = (point.x - rtPic.left) / pCoef + rtArea.left;
			checkP.y = (point.y - rtPic.top) / pCoef + rtArea.top;
			//ȡ�����������ԭ��ͼRGB������rgb���У�ֻ�����ڵ�һ��
			DWORD rgb = bitmapCali.GetPixel(checkP.x, checkP.y);
			if (vRgbs.size() == 0)
			{
				vRgbs.push_back(rgb);
			}
			else
			{
				vRgbs[0] = bitmapCali.GetPixel(checkP.x, checkP.y);
			}
			//listctrl�ؼ��л�û���κ��У������һ�У���������һ�У����µ�һ��
			CString strRGB;
			strRGB.Format(_T("0x%06X"), vRgbs[0]);
			CString strTemp = listCtrlColor.GetItemText(0, 0);
			if (strTemp == _T(""))
			{
				listCtrlColor.InsertItem(0, strRGB);
			}
			else
			{
				listCtrlColor.SetItemText(0, 0, strRGB);
			}
			CString strType;
			comboAreaType.GetWindowTextW(strType);
			if (strType.Find(_T("����")) < 0)
			{
				//�Ƿ��ŵ�����£�����OCR�ṩ�ķ���������ɫƫ����ֵ
				USES_CONVERSION;
				long outwidth, outheight;
				long bufLen = (rtArea.right - rtArea.left) * (rtArea.bottom - rtArea.top);
				DWORD* buf = new DWORD[bufLen];
				bitmapCali.GetPixels(rtArea.left, rtArea.top, rtArea.right - rtArea.left, rtArea.bottom - rtArea.top, buf, &bufLen, &outwidth, &outheight);
				CSCColorOCR ocr;
				int aberration = ocr.GetOTSUThreshold(rgb, buf, bufLen);
				CString strAberration;
				strAberration.Format(_T("%d"), aberration);
				edtAberration.SetWindowTextW(strAberration);

				if (buf)
				{
					delete[] buf;
					buf = nullptr;
				}

				//ѡȡ��ɫ�� ����ָ����RGBֵ��ѡ������ͼ����ж�ֵ��
				CBitmapEx bitbinary;
				bitbinary.Create(rtArea.right - rtArea.left + 1, rtArea.bottom - rtArea.top + 1);
				bitbinary.Draw(0, 0, bitbinary.GetWidth(), bitbinary.GetHeight(), bitmapCali, rtArea.left, rtArea.top, bitbinary.GetWidth(), bitbinary.GetHeight());
				bitbinary.Binary_RBG(vRgbs[0], aberration);

				CDC* pDC = picArea.GetDC();
				bitmapShow.Clear(RGB(240, 240, 240));
// 				bitmapShow.Draw(pDC->m_hDC);
// 				bitmapShow.Create(rtArea.right - rtArea.left + 1, (rtArea.bottom - rtArea.top + 1) * 2);
// 				bitmapShow.SetResampleMode(RM_NEARESTNEIGHBOUR);
// 				bitmapShow.Draw2(0, 0, bitbinary.GetWidth(), bitbinary.GetHeight(), bitmapCali, rtArea.left, rtArea.top, bitbinary.GetWidth(), bitbinary.GetHeight());
// 				bitmapShow.Draw2(0, bitbinary.GetHeight(), bitbinary.GetWidth(), bitbinary.GetHeight(), bitbinary, 0, 0, bitbinary.GetWidth(), bitbinary.GetHeight());
// 				bitmapShow.Scale(100 * pCoef, 100 * pCoef);
				MergeShowBMP(bitmapCali, bitbinary);
				bitmapShow.Draw(pDC->m_hDC);
				ReleaseDC(pDC);


			}

			//��ȡʶ�����̽��������ָ��ظ�ԭ״
			bGetRGB = false;
			SetClassLong(this->GetSafeHwnd(), GCLP_HCURSOR, (LONG)LoadCursor(NULL, IDC_ARROW));
		}
	}
	else
	{
		//�ڷ�ѡȡʶ����ڼ�Ž�����껭��Ĳ���
		RECT rtPic;
		picArea.GetWindowRect(&rtPic);
		ScreenToClient(&rtPic);
		RECT curRect;
		SCCaliRectConversion rectConv;
		curRect = rectConv.MakeRect(PDown, point);
		//�����µĻ����������������ͼƬ������ȡ������
		if (curRect.right - rtPic.left > bitmapShow.GetWidth() || curRect.bottom - rtPic.top > bitmapShow.GetHeight())
		{
			mouseSelect = false;
			CancelRectangle(PDown, PMove);
			CDialogEx::OnLButtonUp(nFlags, point);
			return;
		}
		//���ڻ���״̬
		if (mouseSelect)
		{
			//���ѡȡ��־��Ϊfalse
			mouseSelect = false;
			//�»���Ŀ�͸���һ����ڿؼ�ͼƬ��50%������Ϊ����Ч��ѡ
			if (fabs(point.x - PDown.x) > bitmapShow.GetWidth()*0.5/*(rtPic.right - rtPic.left) * 0.5*/ || fabs(point.y - PDown.y) > bitmapShow.GetHeight() / 2*0.5/*(rtPic.bottom - rtPic.top) * 0.5*/)
			{
				//�õ�ǰ������������ʵ��ͼ�ϵ����귶Χ
				//�»���Ŀ�͸�����ʵ��ͼ��pCoef�����������������õ������ʵ��ͼ�Ŀ�͸�
				int trueWidth = floor((curRect.right - curRect.left) / pCoef * 10 + 0.5) / 10;
				int trueHeight = floor((curRect.bottom - curRect.top) / pCoef * 10 + 0.5) / 10;
				//�»����left��ȥͼƬ�ؼ���left�����»��������ԭʼ��ͼ��ƫ��left��pCoef��
				//����pCoef������õ���ԭʼ��ͼ����ʵƫ�����ԭʼ��ͼ��left����������õ���ʵleft
				//ͬ��õ���ʵtop
				int newTrueLeft = floor(((curRect.left - rtPic.left) / pCoef + rtArea.left) + 0.5);
				int newTrueTop = floor(((curRect.top - rtPic.top) / pCoef + rtArea.top) + 0.5);
				//������ʵ��ͼ���»���������Ϣ
				rtArea.left = newTrueLeft;
				rtArea.top = newTrueTop;
				rtArea.right = newTrueLeft + trueWidth;
				rtArea.bottom = newTrueTop + trueHeight;
				//���¼���궨��ѡͼ�Ŵ�ϵ��
				pCoef = rectConv.CalcAreaCoef(rectConv.areaPicSide, rectConv.areaPicSide, trueHeight, trueWidth);
				//ɾ����ѡ��ͼ
				CancelRectangle(PDown, PMove);
				//�����µĿ�ѡͼˢ��ͼƬ�ؼ�
				CDC* pDC = picArea.GetDC();
				//�Ƚ�ͼƬ�ؼ�ˢ�ɻ�ɫ����ˢ���µķŴ�ͼƬ
				bitmapShow.Clear(RGB(240, 240, 240));
				bitmapShow.Draw(pDC->m_hDC);

				CBitmapEx bitbinary;
				bitbinary.Create(rtArea.right - rtArea.left + 1, rtArea.bottom - rtArea.top + 1);
				bitbinary.Draw2(0, 0, bitbinary.GetWidth(), bitbinary.GetHeight(), bitmapCali, rtArea.left, rtArea.top, bitbinary.GetWidth(), bitbinary.GetHeight());
				if (0 == vRgbs.size())//��δָ����ɫ
				{
					bitbinary.Grayscale();
					int iThresold = 0;
					bitbinary.Binary_OTSU(iThresold);
				}
				else//ָ������ɫ
				{
					CString sRGB;
					edtAberration.GetWindowTextW(sRGB);
					int iThreshold = _ttoi(sRGB);

					bitbinary.Binary_RBG(vRgbs[0], iThreshold);
				}

// 				bitmapShow.Create(rtArea.right - rtArea.left + 1, (rtArea.bottom - rtArea.top + 1) * 2);
// 				bitmapShow.SetResampleMode(RM_NEARESTNEIGHBOUR);
// 				bitmapShow.Draw2(0, 0, rtArea.right - rtArea.left + 1, rtArea.bottom - rtArea.top + 1, bitmapCali, rtArea.left, rtArea.top, rtArea.right - rtArea.left + 1, rtArea.bottom - rtArea.top + 1);
// 				bitmapShow.Draw2(0, rtArea.bottom - rtArea.top + 1, rtArea.right - rtArea.left + 1, rtArea.bottom - rtArea.top + 1, bitbinary, 0, 0, bitbinary.GetWidth(), bitbinary.GetHeight());
// 				bitmapShow.Scale(100 * pCoef, 100 * pCoef);

				MergeShowBMP(bitmapCali, bitbinary);
				bitmapShow.Draw(pDC->m_hDC);
				ReleaseDC(pDC);
			}
			else
			{
				//ɾ����ѡ��ͼ
				CancelRectangle(PDown, PMove);
			}
		}
	}
	
	CDialogEx::OnLButtonUp(nFlags, point);
}

void SCCaliAreaDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (bGetRGB)
	{
		//ȡ���ʶ����ȡ���������ڼ䣬����ƶ�ʱ����ʮ������״
		HCURSOR hCur = LoadCursor(NULL, IDC_CROSS);
		::SetCursor(hCur);
		SetSystemCursor(hCur, (DWORD)IDC_CROSS);
	}
	else
	{
		if (mouseSelect)
		{
			CClientDC dcMouse(this);//��ǰDC
			int oldmode = dcMouse.SetROP2(R2_NOTXORPEN);//���û�ͼģʽ
			CPen pen(0, 0, RGB(255, 0, 0));//���廭��
			CPen * oldpen = dcMouse.SelectObject(&pen);//ѡ�񻭱ʵ���ǰDC

			dcMouse.Rectangle(CRect(PDown, PMove));

			PMove = point;

			dcMouse.Rectangle(CRect(PDown, PMove));

			dcMouse.SelectObject(oldpen);
			dcMouse.SetROP2(oldmode);
			ReleaseDC(&dcMouse);
		}
	}
	CDialogEx::OnMouseMove(nFlags, point);
}

void SCCaliAreaDlg::BrushPicCtrl(CDC *pDC, CRect rctPic, DWORD rgb)
{
	CBrush brs;
	brs.CreateSolidBrush(rgb);
	CRect picrct;
	picrct.top = 0;
	picrct.left = 0;
	picrct.bottom = rctPic.Height();
	picrct.right = rctPic.Width();
	pDC->FillRect(&picrct, &brs);
}

void SCCaliAreaDlg::FillMonitorArea(MonitorArea *mArea)
{
	USES_CONVERSION;
	CString strName, strNameTemp, strType;
	//��ȡ��������
	int nSel = listAreaName.GetSelCount();
	CArray< int, int& > arrayListSel;
	arrayListSel.SetSize(nSel);
	listAreaName.GetSelItems(nSel, arrayListSel.GetData());
	strName = strNameTemp = _T("");
	for (int i = 0; i < nSel; i++)
	{
		listAreaName.GetText(arrayListSel[i], strNameTemp);
		if (i == 0)
		{
			strName = strNameTemp;
		}
		else
		    strName = strName + _T("_") + strNameTemp;
	}
	//��ȡ��������
	comboAreaType.GetWindowTextW(strType);
	int strLen, areaType, valueType, checkType;
	//ƴ�Ӳ�������������
	strLen = strlen(CT2A(strName + _T("@") + strType));
	mArea->SetMonitorAreaName(CT2A(strName + _T("@") + strType), strLen);
	//����������������areaType��valueType��checkType
	if (strType.Find(_T("����")) >= 0)
	{
		//areaType��1��ʾ�����������ţ���2��ʾ�����������֣�
		areaType = 1;
		//valueType��1��ʾ���֣�2��ʾBOOL��3��ʾ���֣���ʱû��ʹ�ú��ֵ����
		valueType = 2;
		//checkType��0��ʾһֱ���ӣ����ȽϹؼ��㣬1��ʾ��Ҫ�ȽϹؼ���
		checkType = 1;
	}
	else
	{
		areaType = 2;
		valueType = 1;
		checkType = 0;
	}
	mArea->SetMonitorAreaType(areaType);
	mArea->SetMonitorAreaValueType(valueType);
	mArea->SetMonitorIsMonitor(1);//Ŀǰ������Ϊ1
	mArea->SetMonitorCheckPointX(checkP.x);
	mArea->SetMonitorCheckPointY(checkP.y);
	mArea->SetMonitorCheckPointType(checkType);
	
	CString strRGB;
	if (strType.Find(_T("����")) >= 0)
	{
		//����ʱ��area��Χֻȡһ����
		mArea->SetMonitorAreaX(checkP.x);
		mArea->SetMonitorAreaY(checkP.y);
		mArea->SetMonitorAreaHeight(0);
		mArea->SetMonitorAreaWidth(0);
		//����ʱ��parameter name="OpenColor"����ɫ���ܴ��ڶ��
		if (vRgbs.size() > 0)
		{
			for (int i = 0; i < vRgbs.size(); i++)
			{
				strRGB.Format(_T("%ul"), vRgbs[i]);
				mArea->AddParameter(SC_VALVE_OPEN_COLOR_PARAMNAME, CT2A(strRGB));
			}
		}
		if (((CButton*)GetDlgItem(IDC_ColorInvert))->GetCheck() == BST_CHECKED)
		{
			mArea->AddParameter(SC_VALVE_IS_COLOR_INVERT, "1");
		}
	}
	else
	{
		mArea->SetMonitorAreaX(rtArea.left);
		mArea->SetMonitorAreaY(rtArea.top);
		mArea->SetMonitorAreaHeight(rtArea.bottom - rtArea.top);
		mArea->SetMonitorAreaWidth(rtArea.right - rtArea.left);
		//���Ƿ���ʱ��parameter name="FontColor"����ɫֻ����һ��
		if (vRgbs.size() > 0)
		{
			strRGB.Format(_T("%ul"), vRgbs[0]);
			mArea->AddParameter(SC_LOADCELL_FONT_COLOR_PARAMNAME, CT2A(strRGB));
		}
		CString strTemp;
		//���Ƿ���ʱ���жϼ�¼��ɫƫ��Ŀؼ��Ƿ���ֵ���еĻ�ʹ��parameter name="ColorThreshold"������ɫƫ��
		edtAberration.GetWindowTextW(strTemp);
		if (strTemp != "")
		{
			mArea->AddParameter(SC_COMMON_COLOR_THRESHOLD_PARAMNAME, CT2A(strTemp));
		}
	}

	mArea->SetMeasureFunction(measure_functin);
}

//ListCtrl����ɫ
void SCCaliAreaDlg::OnCustomdrawMyList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	// Take the default processing unless we set this to something else below.  
	*pResult = CDRF_DODEFAULT;

	// First thing - check the draw stage. If it's the control's prepaint  
	// stage, then tell Windows we want messages for every item.  
	if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		// This is the notification message for an item.  We'll request  
		// notifications before each subitem's prepaint stage.  
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if ((CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage)
	{
		DWORD clrNewTextColor, clrNewBkColor;

		int    nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec);

		USES_CONVERSION;
		//ȡ���Ϊ0���е�text��ʵ����RGB��ֵ
		CString strTemp = listCtrlColor.GetItemText(nItem, 0);
		//clrNewTextColor = atoll(CT2A(strTemp)); //Set the text
		clrNewTextColor = strtoul(CT2A(strTemp), NULL, 16);
		//clrNewBkColor = atoll(CT2A(strTemp));

		pLVCD->clrText = RGB(_GetRValue(clrNewTextColor), _GetGValue(clrNewTextColor), _GetBValue(clrNewTextColor));
		//pLVCD->clrTextBk = RGB(_GetRValue(clrNewBkColor), _GetGValue(clrNewBkColor), _GetBValue(clrNewBkColor));

		// Tell Windows to paint the control itself.  
		*pResult = CDRF_DODEFAULT;
	}
}

void SCCaliAreaDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�б��в�������ɫʱ��������
	int itemp = vRgbs.size();
	if (itemp > 0)
	{
		int nItem = 0;
		CString strRGB;
		//����listCtrlColor���ҵ�ѡ���е���ź�RGBֵ
		for (nItem; nItem < listCtrlColor.GetItemCount(); nItem++)
		{
			if (listCtrlColor.GetItemState(nItem, LVNI_ALL | LVNI_SELECTED) == LVIS_SELECTED)
			{
				strRGB = listCtrlColor.GetItemText(nItem, 0);
				break;
			}
		}
		//�ҵ�ѡ���У���RGB������ɾ����Ӧ��RGBֵ��listCtrlColorɾ��ȫ����֮�����²���ȫ��RGB���е�ֵ
		//ͨ����������ɾ�����²���ķ�ʽ����֤�кŵ�����ά��
		if (nItem != listCtrlColor.GetItemCount())
		{
			std::vector<DWORD>::iterator it = vRgbs.begin() + nItem;
			vRgbs.erase(it);

			listCtrlColor.DeleteAllItems();
			for (int i = 0; i < vRgbs.size(); i++)
			{
				strRGB.Format(_T("0x%06X"), vRgbs[i]);
				listCtrlColor.InsertItem(i, strRGB);
			}
		}
	}	
}

void SCCaliAreaDlg::OnCbnSelchangeCombo2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString areaType;
	comboAreaType.GetLBText(comboAreaType.GetCurSel(), areaType);
	int index = areaType.Find(_T("����"));
	btnAddColor.EnableWindow(index >= 0);
	btnDelColor.EnableWindow(index >= 0);
	m_btnCopy.EnableWindow(index >= 0);
	GetDlgItem(IDC_ColorInvert)->EnableWindow(index >= 0);
	m_btnPatse.EnableWindow(index >= 0 && s_bCopyed);
	edtAberration.EnableWindow(index < 0);
	btnCheck.EnableWindow(comboAreaType.GetCurSel() >= 0);
	GetDlgItem(IDC_BUTTON_Advanced)->EnableWindow(index < 0);
	GetDlgItem(IDC_BUTTON_Advanced)->ShowWindow(index < 0 ? SW_SHOWDEFAULT : SW_HIDE);

	bool bNeedToDefault = true;
	if (areaID != 0)	//��������½�����
	{
		CString strAreaName = CA2T(pCaliConfig->GetMonitorArea(areaID)->GetMonitorAreaName());
		//������е��������ͺͱ���ѡ�����һ��������Ҫ��ΪĬ��ֵ�����Ǳ�����ǰ״̬�����
		if (strAreaName.Find(areaType) >= 0) {
			bNeedToDefault = false;
			measure_functin = pCaliConfig->GetMonitorArea(areaID)->GetMeasureFunction();
		}
	}
	else
	{
		if (m_lastAreaType == areaType)
		{
			bNeedToDefault = false;
		}
	}
	if (bNeedToDefault)
	{
		DefaultMeasureFunc(measure_functin, areaType);
	}

	m_lastAreaType = areaType;
}

void SCCaliAreaDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (!bGetRGB)
	{
		//�ڷ�ѡȡʶ����ڼ�Ž�����껭��Ĳ���
		RECT rtPic;
		picArea.GetWindowRect(&rtPic);
		ScreenToClient(&rtPic);
		//�ж��������ʱ�����Ƿ���ͼƬ�ڲ�
		if (PtInRect(&rtPic, point))
		{
			//��������껭��
			mouseSelect = true;
			PDown = PMove = point;
		}
		else
		{
			PDown = PMove = point;
		}
	}
	
	CDialogEx::OnLButtonDown(nFlags, point);
}

void SCCaliAreaDlg::CancelRectangle(CPoint POrigin, CPoint PEnd)
{
	//ȡ�����λ���
	CClientDC dcMouse(this);//��ǰDC
	int oldmode = dcMouse.SetROP2(R2_NOTXORPEN);//���û�ͼģʽ
	CPen pen(0, 0, RGB(255, 0, 0));//���廭��
	CPen * oldpen = dcMouse.SelectObject(&pen);//ѡ�񻭱ʵ���ǰDC

	dcMouse.Rectangle(CRect(POrigin, PEnd));

	dcMouse.SelectObject(oldpen);
	dcMouse.SetROP2(oldmode);
	ReleaseDC(&dcMouse);
}

void SCCaliAreaDlg::MergeShowBMP(CBitmapEx & bitmapCali, CBitmapEx & bmpbinary)
{
	bitmapShow.Create(rtArea.right - rtArea.left + 1, (rtArea.bottom - rtArea.top + 1) * 2);
	bitmapShow.SetResampleMode(RM_NEARESTNEIGHBOUR);
	bitmapShow.Draw2(0, 0, rtArea.right - rtArea.left + 1, rtArea.bottom - rtArea.top + 1, bitmapCali, rtArea.left, rtArea.top, rtArea.right - rtArea.left + 1, rtArea.bottom - rtArea.top + 1);
// 	CBitmapEx line;
// 	line.Create(bitmapShow.GetWidth(), 1);
// 	line.Clear(0);
// 	bitmapShow.Draw2(0, rtArea.bottom - rtArea.top + 1, bitmapShow.GetWidth(), 1, line, 0, 0, line.GetWidth(), 1);
	bitmapShow.Draw2(0, rtArea.bottom - rtArea.top + 1, rtArea.right - rtArea.left + 1, rtArea.bottom - rtArea.top + 1, bmpbinary, 0, 0, bmpbinary.GetWidth(), bmpbinary.GetHeight());
	bitmapShow.Scale(100 * pCoef, 100 * pCoef);
}

void SCCaliAreaDlg::GetRectArea(RECT& rtSrcArea)
{
	//ȡ����������ͼ�ı궨������Ľ�����
	rtSrcArea.left = rtArea.left;
	rtSrcArea.top = rtArea.top;
	rtSrcArea.right = rtArea.right;
	rtSrcArea.bottom = rtArea.bottom;
}


void SCCaliAreaDlg::OnBnClickedButtonCopy()
{
	s_listCopyColors = vRgbs;
	s_bCopyed = true;

	m_btnPatse.EnableWindow(TRUE);
}


void SCCaliAreaDlg::OnBnClickedButtonPaste()
{
	if (s_bCopyed)
	{
		vRgbs = s_listCopyColors;
		listCtrlColor.DeleteAllItems();
		CString strRGB;
		for (int i = 0; i < vRgbs.size(); i++)
		{
			strRGB.Format(_T("0x%06X"), vRgbs[i]);
			listCtrlColor.InsertItem(i, strRGB);
		}
	}
}


void SCCaliAreaDlg::OnBnClickedButton9()//OK��ť ��������ָ����ֵ��ʾ��ֵ��ͼƬ
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������


	CString sRGB;
	edtAberration.GetWindowTextW(sRGB);
	int iThreshold = _ttoi(sRGB);

	CBitmapEx bitbinary;
	bitbinary.Create(rtArea.right - rtArea.left + 1, rtArea.bottom - rtArea.top + 1);
	bitbinary.Draw(0, 0, bitbinary.GetWidth(), bitbinary.GetHeight(), bitmapCali, rtArea.left, rtArea.top, bitbinary.GetWidth(), bitbinary.GetHeight());
	bitbinary.Binary_RBG(vRgbs[0], iThreshold);

	CDC* pDC = picArea.GetDC();
	bitmapShow.Clear(RGB(240, 240, 240));
// 	bitmapShow.Draw(pDC->m_hDC);
// 	bitmapShow.Create(rtArea.right - rtArea.left + 1, (rtArea.bottom - rtArea.top + 1) * 2);
// 	bitmapShow.SetResampleMode(RM_NEARESTNEIGHBOUR);
// 	bitmapShow.Draw2(0, 0, bitbinary.GetWidth(), bitbinary.GetHeight(), bitmapCali, rtArea.left, rtArea.top, bitbinary.GetWidth(), bitbinary.GetHeight());
// 	bitmapShow.Draw2(0, bitbinary.GetHeight(), bitbinary.GetWidth(), bitbinary.GetHeight(), bitbinary, 0, 0, bitbinary.GetWidth(), bitbinary.GetHeight());
// 	bitmapShow.Scale(100 * pCoef, 100 * pCoef);
	MergeShowBMP(bitmapCali, bitbinary);
	bitmapShow.Draw(pDC->m_hDC);
	ReleaseDC(pDC);
}


// void SCCaliAreaDlg::OnBnClickedButtonListcolor()
// {
// 	// TODO: �ڴ���ӿؼ�֪ͨ����������
// 	vRgbs.clear();
// 	for (int x = rtArea.left; x < rtArea.right; x++)
// 	{
// 		for (int y = rtArea.top; y < rtArea.bottom; y++)
// 		{
// 			vRgbs.push_back(bitmapCali.GetPixel(x, y));
// 		}
// 	}
// 	sort(vRgbs.begin(), vRgbs.end());
// 	unique(vRgbs.begin(), vRgbs.end());
// 
// 	for (int i = 0; i < vRgbs.size(); i++)
// 	{
// 		CString strRGB;
// 		strRGB.Format(_T("0x%06X"), vRgbs[i]);
// 		//ȡ���б�ǰ��������������Ϊ�кŲ����µ�rgbֵ��Ϊ���У�Ĭ���б�ؼ����кŰ���˳���0��ʼ
// 		//ͨ��ɾ��ѡ����ʱ������ɾ�����²���ķ�ʽ����֤�кŵ�����ά��
// 		int colorNum = listCtrlColor.GetItemCount();
// 		listCtrlColor.InsertItem(colorNum, strRGB);
// 	}
// }


void SCCaliAreaDlg::OnBnClickedButtonAdvanced()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SCCaliAreaMeasureDlg dlg;
	dlg.SetAreaMeasureFunction(measure_functin);
	if (dlg.DoModal() == IDOK )
	{
		measure_functin = dlg.GetAreaMeasureFunction();
	}
	
}

bool SCCaliAreaDlg::DefaultMeasureFunc(AreaMeasureFunction& func, const CString& areaType)
{
	bool bSet = false;
	if (areaType.Find(_T("����ֵ")) >= 0)
	{
		func = SCCaliAreaMeasureDlg::DefaultCalcFunc();
		bSet = true;
	}
	else if (areaType.Find(_T("�趨ֵ")) >= 0)
	{
		func = SCCaliAreaMeasureDlg::DefaultSettingFunc();
		bSet = true;
	}
	else if (areaType.Find(_T("��")) >= 0)
	{
		func = SCCaliAreaMeasureDlg::DefaultDecrementFunc();
		bSet = true;
	}
	else if (areaType.Find(_T("�¶�")) >= 0)
	{
		func = SCCaliAreaMeasureDlg::DefaultTemperatureFunc();
		bSet = true;
	}
	else
	{
		bSet = false;
	}

	return bSet;
}
