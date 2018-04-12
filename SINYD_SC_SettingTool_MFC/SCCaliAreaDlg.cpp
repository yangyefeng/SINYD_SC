// SCCaliAreaDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SINYD_SC_SettingTool_MFC.h"
#include "SCCaliAreaDlg.h"
#include "afxdialogex.h"
#include "afxcmn.h"
#include "SCCaliRectConversion.h"
#include <algorithm>
#include "SCCaliAreaMeasureDlg.h"


// SCCaliAreaDlg 对话框

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


// SCCaliAreaDlg 消息处理程序
BOOL SCCaliAreaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitComboBox();
	btnDel.EnableWindow(areaID != 0);
	picArea.MoveWindow(20, 20, 250, 250);
	//picture控件修改type类型为bmp
	picArea.ModifyStyle(0, SS_BITMAP);

	//在新增区域时，未选区域类型，标定点的相关按钮控件全部禁用
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
		//修改操作，加载已有信息
		CString areaName, strTemp, areaType;
		strTemp = CA2T(pCaliConfig->GetMonitorArea(areaID)->GetMonitorAreaName());
		//在MonitorAreaName中截取name
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
			//截取type之前的最后一个name和type
			areaName = strTemp.Left(index);
			areaType = strTemp.Mid(index + 1);
			index = listAreaName.FindStringExact(0, areaName);
			if (index >= 0)
				listAreaName.SetSel(index, true);
			index = comboAreaType.FindStringExact(0, areaType);
			if (index >= 0)
				comboAreaType.SetCurSel(index);
			//根据区域类型设置颜色的增加和删除按钮是否可用
			index = areaType.Find(_T("阀门"));
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
		//加载checkpoint信息
		checkP.x = pCaliConfig->GetMonitorArea(areaID)->GetMonitorAreaX();
		checkP.y = pCaliConfig->GetMonitorArea(areaID)->GetMonitorAreaY();
		//加载识别点信息
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
					//如果是偏差类型的para，将其显示到色差控件上
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
					//其他情况均为颜色RGB值
					rgb = atoll((*pParameters)[i]->GetParameterValue());
					vRgbs.push_back(rgb);
					strRGB.Format(_T("0x%06X"), rgb);
					listCtrlColor.InsertItem(i, strRGB);
				}
			}
		}

		//加载计量算法
		measure_functin = pCaliConfig->GetMonitorArea(areaID)->GetMeasureFunction();
		if (measure_functin.method_type == 0 && measure_functin.v1_start_at == 0 && measure_functin.v1_end_at == 0)		//检查是否是空的计量算法
		{
			bool bSet = DefaultMeasureFunc(measure_functin, areaType);
			if (bSet)
			{
				//当成功设置为默认计量算法时，说明区域类型需要设置计量算法。
				//加载失败此时应是由于旧版本未进行设置的原因。
				//此时需要向区域中设置此次默认的计量算法。
				pCaliConfig->GetMonitorArea(areaID)->SetMeasureFunction(measure_functin);
			}
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void SCCaliAreaDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strType;
	//获取区域名称
	int nSel = listAreaName.GetSelCount();
	if (nSel == 0)
	{
		MessageBox(_T("请至少选择一个区域名称"), _T("提示"), MB_OK);
		return;
	}
	//获取区域类型
	comboAreaType.GetWindowTextW(strType);
	if (strType == "")
	{
		MessageBox(_T("区域类型不能为空"), _T("提示"), MB_OK);
		return;
	}
	//判断识别点数量
	if (vRgbs.size() <= 0)
	{
		MessageBox(_T("请至少识别一个关键点颜色"), _T("提示"), MB_OK);
		return;
	}
	//判断识别点是否已标定
	if (checkP.x == 0 && checkP.y == 0)
	{
		MessageBox(_T("请标定识别点坐标"), _T("提示"), MB_OK);
		return;
	}

	const vector<MonitorArea *>* areas = pCaliConfig->GetMonitorAreas();
	if (areaID == 0)
	{
		//新增区域
		int newID = 1;
		//根据当前区域情况获取新区域的ID，如果当前没有标定区域，新ID设为1
		if (areas->size() != 0)
		{
			//取得当前最大的MonitorAreaID
			for (int i = 0; i < areas->size(); i++)
			{
				if ((*areas)[i]->GetMonitorAreaID() >= newID)
					newID = (*areas)[i]->GetMonitorAreaID();
			}
			//在当前最大ID上加1作为新区域的ID
			newID = newID + 1;
		}
		//填充新区域的详细信息
		MonitorArea newM(newID);
		FillMonitorArea(&newM);
		//新区域加入标定配置类中
		pCaliConfig->AddMonitorArea(&newM);
	}
	else
	{
		//修改区域
		if (areas->size() != 0)
		{
			MonitorArea modifyM(areaID);
			FillMonitorArea(&modifyM);
			//删除当前id对应的标定配置类中的区域
			int result = pCaliConfig->DelMonitorArea(areaID);
			if (result == CONFIG_SUCCESS)
			{
				//删除成功，新区域加入标定配置类中
				pCaliConfig->AddMonitorArea(&modifyM);
			}
			else
			{
				//删除失败，提示用户没找到对应区域
				MessageBox(_T("传入的当前区域ID错误，没有找到匹配项"), _T("提示"), MB_OK);
				return;
			}
		}
		else
		{
			//当前标定配置类中没有区域，提示用户没找到对应区域
			MessageBox(_T("传入的当前区域ID错误，没有找到匹配项"), _T("提示"), MB_OK);
			return;
		}
	}
	EndDialog(IDOK);
}

void SCCaliAreaDlg::SetPCaliClass(CCalibrationConfig* pCaliClass)
{
	//取得标定配置信息类的指针
	pCaliConfig = pCaliClass;
}

void SCCaliAreaDlg::SetHopperList(list<HopperStru>* HopperList)
{
	//取得用于配置文件的HopperList的指针
	pHopperList = HopperList;
}

void SCCaliAreaDlg::SetRectArea(const RECT& rtSrcArea)
{
	//取得相对整体截图的标定区域的四角坐标
	rtArea.left = rtSrcArea.left;
	rtArea.top = rtSrcArea.top;
	rtArea.right = rtSrcArea.right;
	rtArea.bottom = rtSrcArea.bottom;
}

void SCCaliAreaDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	//id为0，表示不在删除区域的状态
	if (areaID == 0)
		return;

	//提示失败，不关闭对话框
	int yesno = MessageBox(_T("确定删除此标定区域？"), _T(""), MB_YESNO);
	if (yesno == IDNO)
	{
		return;
	}

	int result  = pCaliConfig->DelMonitorArea(areaID);
	//删除area成功，结束对话框，返回自定义的AREA_DELETE
	if (result == CONFIG_SUCCESS)
	{
		//提示成功，关闭对话框
		MessageBox(_T("删除成功"), _T("提示"), MB_OK);
		EndDialog(AREA_DELETE);
	}
	else
	{
		//提示失败，不关闭对话框
		MessageBox(_T("标定信息中没有对应ID的标定区域，删除失败！"), _T("提示"), MB_OK);
		return;
	}
}

void SCCaliAreaDlg::SetCBitmap(CBitmapEx& bitmapArea)
{
	//创建用于标定的原大图BMP
	bitmapCali.Create(bitmapArea);
	bitmapCali.SetResampleMode(RM_NEARESTNEIGHBOUR);
	//根据传入区域位置创建用于放大展示的截图BMP
	CBitmapEx bitbinary;
	bitbinary.Create(rtArea.right - rtArea.left + 1, rtArea.bottom - rtArea.top + 1);
	bitbinary.Draw2(0, 0, bitbinary.GetWidth(), bitbinary.GetHeight(), bitmapCali, rtArea.left, rtArea.top, bitbinary.GetWidth(), bitbinary.GetHeight());

	if (areaID != 0)//指定了颜色
	{
		//加载识别点信息
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
					//其他情况均为颜色RGB值
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
	// TODO: 在此添加控件通知处理程序代码
	//鼠标指针回复原状
	SetClassLong(this->GetSafeHwnd(), GCLP_HCURSOR, (LONG)LoadCursor(NULL, IDC_ARROW));
	bitmapCali.Clear();
	bitmapShow.Clear();
	CDialogEx::OnCancel();
}

void SCCaliAreaDlg::InitComboBox()
{
	//清除所有下拉选项
	listAreaName.ResetContent();
	comboAreaType.ResetContent();
	//初始化下拉框
	comboAreaType.InsertString(0, _T("上料阀门1"));
	comboAreaType.InsertString(1, _T("上料阀门2"));
	comboAreaType.InsertString(2, _T("放料阀门1"));
	comboAreaType.InsertString(3, _T("放料阀门2"));
	comboAreaType.InsertString(4, _T("中储斗阀门"));
	comboAreaType.InsertString(5, _T("秤"));
	comboAreaType.InsertString(6, _T("计算值"));
	comboAreaType.InsertString(7, _T("设定值"));
	comboAreaType.InsertString(8, _T("温度"));

	listAreaName.InsertString(0, _T("骨料1"));
	listAreaName.InsertString(1, _T("骨料2"));
	listAreaName.InsertString(2, _T("骨料3"));
	listAreaName.InsertString(3, _T("骨料4"));
	listAreaName.InsertString(4, _T("骨料5"));
	listAreaName.InsertString(5, _T("骨料6"));
	listAreaName.InsertString(6, _T("粉料1"));
	listAreaName.InsertString(7, _T("粉料2"));
	listAreaName.InsertString(8, _T("粉料3"));
	listAreaName.InsertString(9, _T("粉料4"));
	listAreaName.InsertString(10, _T("粉料5"));
	listAreaName.InsertString(11, _T("粉料6"));
	listAreaName.InsertString(12, _T("外参剂1"));
	listAreaName.InsertString(13, _T("外参剂2"));
	listAreaName.InsertString(14, _T("木质素"));
	listAreaName.InsertString(15, _T("沥青"));
	listAreaName.InsertString(16, _T("水"));
	listAreaName.InsertString(17, _T("搅拌机"));

	//初始化listCtrlColor
	DWORD dwStyle = listCtrlColor.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;// 选中某行使整行高亮（只适用与report 风格的listctrl ）   
	listCtrlColor.SetExtendedStyle(dwStyle); // 设置扩展风格   
	listCtrlColor.InsertColumn(0, _T("颜色"), LVCFMT_LEFT, 100);
	listCtrlColor.DeleteAllItems();
}

void SCCaliAreaDlg::OnPaint()
{
	CDialogEx::OnPaint();
	CDialogEx::UpdateWindow();
	//展示截图
	bitmapShow.SetResampleMode(RM_NEARESTNEIGHBOUR);
	CDC* pDC = picArea.GetDC();		// add by yaodi for DC release
	bitmapShow.Draw(pDC->m_hDC);
	ReleaseDC(pDC);				// add by yaodi for DC release
}

void SCCaliAreaDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	//取点标识开启，改变鼠标形态为十字星
	bGetRGB = true;
	SetClassLong(this->GetSafeHwnd(), GCLP_HCURSOR, (LONG)LoadCursor(NULL, IDC_CROSS));
}


void SCCaliAreaDlg::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	if (vRgbs.size() > 0)
	{
		//取得当前识别点的rgb，加入rgb队列
		DWORD rgb = bitmapCali.GetPixel(checkP.x, checkP.y);
		vRgbs.push_back(rgb);
		CString strRGB;
		strRGB.Format(_T("0x%06X"), rgb);
		//取得列表当前行数，以行数作为行号插入新的rgb值作为新行，默认列表控件中行号按照顺序从0开始
		//通过删除选中行时，整体删除重新插入的方式，保证行号的序列维护
		int colorNum = listCtrlColor.GetItemCount();
		listCtrlColor.InsertItem(colorNum, strRGB);
	}
}

void SCCaliAreaDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (bGetRGB)
	{
		RECT rtPicCtrl, rtPic;
		//取得图片控件的在当前窗口的相对范围，并赋给图片范围，确定图片范围的top和left
		picArea.GetWindowRect(&rtPicCtrl);
		ScreenToClient(&rtPicCtrl);
		rtPic = rtPicCtrl;
		//通过缩放系数确定图片范围的宽和高，加上图片控件的left和top可以确定最终的实际图片的范围bottom和right
		rtPic.right = pCoef * (rtArea.right - rtArea.left) + rtPic.left;
		rtPic.bottom = pCoef * (rtArea.bottom - rtArea.top) + rtPic.top;
		if (!PtInRect(&rtPic, point))
		{
			//判断鼠标点击处不在图片范围内，本次识别关键点功能结束，并提示用户
			bGetRGB = false;
			SetClassLong(this->GetSafeHwnd(), GCLP_HCURSOR, (LONG)LoadCursor(NULL, IDC_ARROW));
			MessageBox(_T("请在图片内标定关键点"), _T("提示"), MB_OK);
			CDialogEx::OnLButtonUp(nFlags, point);
			return;
		}
		//标定识别点
		if (bGetRGB)
		{
			//鼠标点击处在图片范围内，通过缩放系数计算关键点在整个截图中的坐标
			checkP.x = (point.x - rtPic.left) / pCoef + rtArea.left;
			checkP.y = (point.y - rtPic.top) / pCoef + rtArea.top;
			//取得鼠标点击处的原大图RGB，存入rgb队列，只保存在第一点
			DWORD rgb = bitmapCali.GetPixel(checkP.x, checkP.y);
			if (vRgbs.size() == 0)
			{
				vRgbs.push_back(rgb);
			}
			else
			{
				vRgbs[0] = bitmapCali.GetPixel(checkP.x, checkP.y);
			}
			//listctrl控件中还没有任何行，插入第一行，已有至少一行，更新第一行
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
			if (strType.Find(_T("阀门")) < 0)
			{
				//非阀门的情况下，调用OCR提供的方法计算颜色偏差阈值
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

				//选取颜色后 根据指定的RGB值对选定区域图像进行二值化
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

			//获取识别点过程结束，鼠标指针回复原状
			bGetRGB = false;
			SetClassLong(this->GetSafeHwnd(), GCLP_HCURSOR, (LONG)LoadCursor(NULL, IDC_ARROW));
		}
	}
	else
	{
		//在非选取识别点期间才进行鼠标画框的操作
		RECT rtPic;
		picArea.GetWindowRect(&rtPic);
		ScreenToClient(&rtPic);
		RECT curRect;
		SCCaliRectConversion rectConv;
		curRect = rectConv.MakeRect(PDown, point);
		//构造新的画框区域，新区域大于图片区域则取消画框
		if (curRect.right - rtPic.left > bitmapShow.GetWidth() || curRect.bottom - rtPic.top > bitmapShow.GetHeight())
		{
			mouseSelect = false;
			CancelRectangle(PDown, PMove);
			CDialogEx::OnLButtonUp(nFlags, point);
			return;
		}
		//处于画框状态
		if (mouseSelect)
		{
			//鼠标选取标志置为false
			mouseSelect = false;
			//新画框的宽和高有一项大于控件图片的50%，才认为是有效新选
			if (fabs(point.x - PDown.x) > bitmapShow.GetWidth()*0.5/*(rtPic.right - rtPic.left) * 0.5*/ || fabs(point.y - PDown.y) > bitmapShow.GetHeight() / 2*0.5/*(rtPic.bottom - rtPic.top) * 0.5*/)
			{
				//用当前画框区域反算真实截图上的坐标范围
				//新画框的宽和高是真实截图的pCoef倍，反算后四舍五入得到相对真实截图的宽和高
				int trueWidth = floor((curRect.right - curRect.left) / pCoef * 10 + 0.5) / 10;
				int trueHeight = floor((curRect.bottom - curRect.top) / pCoef * 10 + 0.5) / 10;
				//新画框的left减去图片控件的left，是新画框相对于原始截图的偏差left的pCoef倍
				//除以pCoef倍数后得到与原始截图的真实偏差，加上原始截图的left，四舍五入得到真实left
				//同理得到真实top
				int newTrueLeft = floor(((curRect.left - rtPic.left) / pCoef + rtArea.left) + 0.5);
				int newTrueTop = floor(((curRect.top - rtPic.top) / pCoef + rtArea.top) + 0.5);
				//更新真实截图的新画框区域信息
				rtArea.left = newTrueLeft;
				rtArea.top = newTrueTop;
				rtArea.right = newTrueLeft + trueWidth;
				rtArea.bottom = newTrueTop + trueHeight;
				//重新计算标定框选图放大系数
				pCoef = rectConv.CalcAreaCoef(rectConv.areaPicSide, rectConv.areaPicSide, trueHeight, trueWidth);
				//删除框选画图
				CancelRectangle(PDown, PMove);
				//根据新的框选图刷新图片控件
				CDC* pDC = picArea.GetDC();
				//先将图片控件刷成灰色，再刷上新的放大图片
				bitmapShow.Clear(RGB(240, 240, 240));
				bitmapShow.Draw(pDC->m_hDC);

				CBitmapEx bitbinary;
				bitbinary.Create(rtArea.right - rtArea.left + 1, rtArea.bottom - rtArea.top + 1);
				bitbinary.Draw2(0, 0, bitbinary.GetWidth(), bitbinary.GetHeight(), bitmapCali, rtArea.left, rtArea.top, bitbinary.GetWidth(), bitbinary.GetHeight());
				if (0 == vRgbs.size())//还未指定颜色
				{
					bitbinary.Grayscale();
					int iThresold = 0;
					bitbinary.Binary_OTSU(iThresold);
				}
				else//指定了颜色
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
				//删除框选画图
				CancelRectangle(PDown, PMove);
			}
		}
	}
	
	CDialogEx::OnLButtonUp(nFlags, point);
}

void SCCaliAreaDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (bGetRGB)
	{
		//取点标识或者取背景开启期间，鼠标移动时保持十字星形状
		HCURSOR hCur = LoadCursor(NULL, IDC_CROSS);
		::SetCursor(hCur);
		SetSystemCursor(hCur, (DWORD)IDC_CROSS);
	}
	else
	{
		if (mouseSelect)
		{
			CClientDC dcMouse(this);//当前DC
			int oldmode = dcMouse.SetROP2(R2_NOTXORPEN);//设置绘图模式
			CPen pen(0, 0, RGB(255, 0, 0));//定义画笔
			CPen * oldpen = dcMouse.SelectObject(&pen);//选择画笔到当前DC

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
	//获取区域名称
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
	//获取区域类型
	comboAreaType.GetWindowTextW(strType);
	int strLen, areaType, valueType, checkType;
	//拼接并设置区域名称
	strLen = strlen(CT2A(strName + _T("@") + strType));
	mArea->SetMonitorAreaName(CT2A(strName + _T("@") + strType), strLen);
	//根据区域类型设置areaType、valueType和checkType
	if (strType.Find(_T("阀门")) >= 0)
	{
		//areaType，1表示开关量（阀门），2表示数字量（数字）
		areaType = 1;
		//valueType，1表示数字，2表示BOOL，3表示汉字，暂时没有使用汉字的情况
		valueType = 2;
		//checkType，0表示一直监视，不比较关键点，1表示需要比较关键点
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
	mArea->SetMonitorIsMonitor(1);//目前都设置为1
	mArea->SetMonitorCheckPointX(checkP.x);
	mArea->SetMonitorCheckPointY(checkP.y);
	mArea->SetMonitorCheckPointType(checkType);
	
	CString strRGB;
	if (strType.Find(_T("阀门")) >= 0)
	{
		//阀门时，area范围只取一个点
		mArea->SetMonitorAreaX(checkP.x);
		mArea->SetMonitorAreaY(checkP.y);
		mArea->SetMonitorAreaHeight(0);
		mArea->SetMonitorAreaWidth(0);
		//阀门时，parameter name="OpenColor"，颜色可能存在多个
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
		//不是阀门时，parameter name="FontColor"，颜色只保存一个
		if (vRgbs.size() > 0)
		{
			strRGB.Format(_T("%ul"), vRgbs[0]);
			mArea->AddParameter(SC_LOADCELL_FONT_COLOR_PARAMNAME, CT2A(strRGB));
		}
		CString strTemp;
		//不是阀门时，判断记录颜色偏差的控件是否有值，有的话使用parameter name="ColorThreshold"保存颜色偏差
		edtAberration.GetWindowTextW(strTemp);
		if (strTemp != "")
		{
			mArea->AddParameter(SC_COMMON_COLOR_THRESHOLD_PARAMNAME, CT2A(strTemp));
		}
	}

	mArea->SetMeasureFunction(measure_functin);
}

//ListCtrl加颜色
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
		//取序号为0的列的text，实际是RGB的值
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
	// TODO: 在此添加控件通知处理程序代码
	//列表中不存在颜色时，不处理
	int itemp = vRgbs.size();
	if (itemp > 0)
	{
		int nItem = 0;
		CString strRGB;
		//遍历listCtrlColor，找到选中行的序号和RGB值
		for (nItem; nItem < listCtrlColor.GetItemCount(); nItem++)
		{
			if (listCtrlColor.GetItemState(nItem, LVNI_ALL | LVNI_SELECTED) == LVIS_SELECTED)
			{
				strRGB = listCtrlColor.GetItemText(nItem, 0);
				break;
			}
		}
		//找到选中行，在RGB队列中删除对应的RGB值，listCtrlColor删除全部行之后重新插入全部RGB队列的值
		//通过这种整体删除重新插入的方式，保证行号的序列维护
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
	// TODO: 在此添加控件通知处理程序代码
	CString areaType;
	comboAreaType.GetLBText(comboAreaType.GetCurSel(), areaType);
	int index = areaType.Find(_T("阀门"));
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
	if (areaID != 0)	//如果不是新建区域
	{
		CString strAreaName = CA2T(pCaliConfig->GetMonitorArea(areaID)->GetMonitorAreaName());
		//如果既有的区域类型和本次选择的是一个，则不需要置为默认值，而是保留当前状态不变更
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (!bGetRGB)
	{
		//在非选取识别点期间才进行鼠标画框的操作
		RECT rtPic;
		picArea.GetWindowRect(&rtPic);
		ScreenToClient(&rtPic);
		//判断左键按下时坐标是否处于图片内部
		if (PtInRect(&rtPic, point))
		{
			//是则开启鼠标画框
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
	//取消矩形绘制
	CClientDC dcMouse(this);//当前DC
	int oldmode = dcMouse.SetROP2(R2_NOTXORPEN);//设置绘图模式
	CPen pen(0, 0, RGB(255, 0, 0));//定义画笔
	CPen * oldpen = dcMouse.SelectObject(&pen);//选择画笔到当前DC

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
	//取得相对整体截图的标定区域的四角坐标
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


void SCCaliAreaDlg::OnBnClickedButton9()//OK按钮 点击后根据指定阈值显示二值化图片
{
	// TODO: 在此添加控件通知处理程序代码


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
// 	// TODO: 在此添加控件通知处理程序代码
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
// 		//取得列表当前行数，以行数作为行号插入新的rgb值作为新行，默认列表控件中行号按照顺序从0开始
// 		//通过删除选中行时，整体删除重新插入的方式，保证行号的序列维护
// 		int colorNum = listCtrlColor.GetItemCount();
// 		listCtrlColor.InsertItem(colorNum, strRGB);
// 	}
// }


void SCCaliAreaDlg::OnBnClickedButtonAdvanced()
{
	// TODO: 在此添加控件通知处理程序代码
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
	if (areaType.Find(_T("计算值")) >= 0)
	{
		func = SCCaliAreaMeasureDlg::DefaultCalcFunc();
		bSet = true;
	}
	else if (areaType.Find(_T("设定值")) >= 0)
	{
		func = SCCaliAreaMeasureDlg::DefaultSettingFunc();
		bSet = true;
	}
	else if (areaType.Find(_T("秤")) >= 0)
	{
		func = SCCaliAreaMeasureDlg::DefaultDecrementFunc();
		bSet = true;
	}
	else if (areaType.Find(_T("温度")) >= 0)
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
