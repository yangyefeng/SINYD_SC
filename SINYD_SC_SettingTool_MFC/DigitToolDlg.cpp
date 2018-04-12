// DigitToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SINYD_SC_SettingTool_MFC.h"
#include "DigitToolDlg.h"
#include "afxdialogex.h"
#include "DigitReader.h"
#include "..\SINYD_SC_CommonUtility\BitmapEx.h"
#include <set>


// CDigitToolDlg 对话框

#define ID_COL_NUM 2
#define VLAUE_COL_NUM 1
#define IMAGE_COL_NUM 0

IMPLEMENT_DYNAMIC(CDigitToolDlg, CDialogEx)

CDigitToolDlg::CDigitToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SINYD_SC_DIGITTOOL_DIALOG, pParent)
{
	m_xml_Name = ".\\ConfigFile\\SCDTMList.xml";
}

CDigitToolDlg::~CDigitToolDlg()
{
}

void CDigitToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DIGITS, m_digit_list);
	DDX_Control(pDX, IDC_STATIC_DIGITIMGEVIEW, m_static_digitview);
	DDX_Control(pDX, IDC_CHECK_NOTSETDIGIT, m_check_not_set);
	DDX_Control(pDX, IDC_CHECK_SETDIGIT, m_check_set);
	DDX_Control(pDX, IDC_COMBO_SETDIGITRES, m_select_set_value);
}


BEGIN_MESSAGE_MAP(CDigitToolDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_NOTSETDIGIT, &CDigitToolDlg::OnBnClickedCheckNotsetdigit)
	ON_BN_CLICKED(IDC_CHECK_SETDIGIT, &CDigitToolDlg::OnBnClickedCheckSetdigit)
	ON_CBN_SELCHANGE(IDC_COMBO_SETDIGITRES, &CDigitToolDlg::OnCbnSelchangeComboSetdigitres)
	ON_BN_CLICKED(IDC_BUTTON_DIGITSAVEVALUE, &CDigitToolDlg::OnBnClickedButtonDigitsavevalue)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DIGITS, &CDigitToolDlg::OnNMClickListDigits)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDigitToolDlg 消息处理程序


BOOL CDigitToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_check_set.SetCheck(BST_UNCHECKED);
	m_check_not_set.SetCheck(BST_CHECKED);
	m_select_set_value.InsertString(0, L"全部");
	m_select_set_value.SetCurSel(0);

	m_EditValueToolTip.Create(this);
	m_EditValueToolTip.AddTool(GetDlgItem(IDC_EDIT_DIGITVALUE), _T("输入' '(空格)为废弃字模\r\n输入'x'或不输入任何内容为置为未标定"));
	m_EditValueToolTip.SetMaxTipWidth(250);
	m_EditValueToolTip.Activate(TRUE);

	InitAll();
	RefreshAll();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CDigitToolDlg::InitImageList()
{
	m_image_list.Create(30, 30, ILC_COLOR32, m_digits.size(), m_digits.size());
	for (auto& d : m_digits)
	{
		CBitmapEx bitmap;
		ModelToBitmap(d, bitmap);
		//保持比例变更大小到30*30
		if (d.weight > d.height)
		{
			float radio = 30.0 / (float)d.weight *100.0;
			long lRadio = (long)radio;
			bitmap.Scale(lRadio, lRadio);
		}
		else
		{
			float radio = 30.0 / (float)d.height *100.0;
			long lRadio = (long)radio;
			bitmap.Scale(lRadio, lRadio);
		}
		//非等比部分用白色填充
		CBitmapEx bit3030;
		bit3030.Create(30, 30);
		bit3030.Clear(0xFFFFFF);
		bit3030.Draw((30 - bitmap.GetWidth()) / 2, (30 - bitmap.GetHeight()) / 2, bitmap.GetWidth(), bitmap.GetHeight(), bitmap, 0, 0, bitmap.GetWidth(), bitmap.GetHeight());
		//

		HBITMAP hbit = NULL;
		bit3030.Save(hbit);
		CBitmap cbit;
		cbit.Attach(hbit);
		m_image_list.Add(&cbit, RGB(0, 0, 0));
		DeleteObject(hbit);
	}

}

void CDigitToolDlg::InitDigitList()
{
	DigitsRW drw;
	m_digits = drw.ReadFromXML(m_xml_Name);
}

void CDigitToolDlg::InitDigitViewList()
{
	WORD dwStyle = m_digit_list.GetExtendedStyle();                    //添加列表框的网格线！！！
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	dwStyle != LVS_EX_SUBITEMIMAGES;
	m_digit_list.SetExtendedStyle(dwStyle);

	m_digit_list.SetAlwaysHighlight(TRUE);

	m_digit_list.SetImageList(&m_image_list, LVSIL_SMALL);

	m_digit_list.InsertColumn(IMAGE_COL_NUM, L"字模图", LVCFMT_CENTER);
	//	//解决第一列不置中的问题
	// 	LVCOLUMN lvc;
	// 	lvc.mask = LVCF_FMT;
	// 	m_digit_list.GetColumn(IMAGE_COL_NUM, &lvc);
	// 	lvc.fmt &= ~LVCFMT_JUSTIFYMASK;
	// 	lvc.fmt |= LVCFMT_CENTER;
	//	m_digit_list.SetColumn(IMAGE_COL_NUM, &lvc);
	//
	m_digit_list.InsertColumn(VLAUE_COL_NUM, L"标定值", LVCFMT_CENTER);
	m_digit_list.InsertColumn(ID_COL_NUM, L"ID", LVCFMT_CENTER);

	m_digit_list.SetColumnWidth(IMAGE_COL_NUM, 120);
	m_digit_list.SetColumnWidth(VLAUE_COL_NUM, 100);
	m_digit_list.SetColumnWidth(ID_COL_NUM, 100);

	m_digit_list.SetColumnSorting(ID_COL_NUM, CListCtrlEx::Auto, CListCtrlEx::Int);
	m_digit_list.SetColumnSorting(VLAUE_COL_NUM, CListCtrlEx::Auto, CListCtrlEx::Int);

	//将第一列（图标显示列）和第三列（ID列）对调显示位置
	CHeaderCtrl *pmyHeaderCtrl = m_digit_list.GetHeaderCtrl();
	int   nCount = pmyHeaderCtrl->GetItemCount();
	LPINT   pnOrder = (LPINT)malloc(nCount*sizeof(int));
	ASSERT(pnOrder != NULL);

	pmyHeaderCtrl->GetOrderArray(pnOrder, nCount);
	int nTemp;
	nTemp = pnOrder[IMAGE_COL_NUM];
	pnOrder[IMAGE_COL_NUM] = pnOrder[ID_COL_NUM];
	pnOrder[ID_COL_NUM] = nTemp;

	pmyHeaderCtrl->SetOrderArray(nCount, pnOrder);
	free(pnOrder);
}

void CDigitToolDlg::RefreshDigitViewList()
{
	m_digits_for_view.clear();

	int sel = m_select_set_value.GetCurSel();
	CString select_value;
	m_select_set_value.GetLBText(sel, select_value);
	if (select_value == L"全部")
	{
		if (m_check_not_set.GetCheck() == BST_CHECKED && m_check_set.GetCheck() == BST_CHECKED)	//全部字模
		{
			int index = 0;
			for (auto& d : m_digits)
			{
				m_digits_for_view[index] = &d;
				index++;
			}
		}
		else if (m_check_not_set.GetCheck() == BST_CHECKED)	//未标定字模
		{
			int index = 0;
			for (auto& d : m_digits)
			{
				if (d.value == "x")
				{
					m_digits_for_view[index] = &d;
				}
				index++;
			}
		}
		else if (m_check_set.GetCheck() == BST_CHECKED)	//已标定字模
		{
			int index = 0;
			for (auto& d : m_digits)
			{
				if (d.value != "x")
				{
					m_digits_for_view[index] = &d;
				}
				index++;
			}
		}
		else
		{
			//显示列表为空
		}
	}
	else
	{
		string select_value_a = CT2A(select_value);
		select_value_a = ConvertSpecialDigitValue(select_value_a, false);
		int index = 0;
		for (auto& d : m_digits)
		{
			if (select_value_a == d.value)
			{
				m_digits_for_view[index] = &d;
			}
			index++;
		}
	}
}

void CDigitToolDlg::InitAll()
{
	InitDigitList();
	InitImageList();
	InitDigitViewList();
}

void CDigitToolDlg::RefreshListCtrlData()
{
	m_digit_list.DeleteAllItems();

	int index = 0;
	for (auto& item : m_digits_for_view)
	{
		m_digit_list.InsertItem(index, L"", index);
		m_digit_list.SetItemText(index, ID_COL_NUM, to_wstring(item.second->id).c_str());
		m_digit_list.SetItemText(index, VLAUE_COL_NUM, CA2T(ConvertSpecialDigitValue(item.second->value, true).c_str()));
		m_digit_list.SetItem(index, IMAGE_COL_NUM, LVIF_IMAGE, NULL, item.first, 0, 0, 0);
		m_digit_list.SetItemData(index, (DWORD_PTR)item.second);

		index++;
	}

	ListSelectDigit(0);
}



void CDigitToolDlg::RefreshCurrentDigit(BOOL bClean)
{
	if (!bClean)
	{
		int mark = m_digit_list.GetSelectionMark();
		if (mark >= 0)
		{
			RECT img_rect;
			m_static_digitview.GetClientRect(&img_rect);
			int img_w = img_rect.right - img_rect.left;
			int img_h = img_rect.bottom - img_rect.top;

			Digit* pD = (Digit*)m_digit_list.GetItemData(mark);
			CBitmapEx bitmap;
			ModelToBitmap(*pD, bitmap);
			//保持比例变更大小到100*150
			if (pD->weight > pD->height)
			{
				float radio = (float)img_w / (float)pD->weight *100.0;
				long lRadio = (long)radio;
				bitmap.Scale(lRadio, lRadio);
			}
			else
			{
				float radio = (float)img_h / (float)pD->height *100.0;
				long lRadio = (long)radio;
				bitmap.Scale(lRadio, lRadio);
			}
			//非等比部分用白色填充
			CBitmapEx bitLarge;
			bitLarge.Create(img_w, img_h);
			bitLarge.Clear(0xFFFFFF);
			bitLarge.Draw((img_w - bitmap.GetWidth()) / 2, (img_h - bitmap.GetHeight()) / 2, bitmap.GetWidth(), bitmap.GetHeight(), bitmap, 0, 0, bitmap.GetWidth(), bitmap.GetHeight());
			//

			CDC* pDC = m_static_digitview.GetDC();
			bitLarge.Draw(pDC->m_hDC);
			ReleaseDC(pDC);

			if (pD->value != "x")
			{
				SetDlgItemText(IDC_EDIT_DIGITVALUE, CA2T(ConvertSpecialDigitValue(pD->value, true).c_str()));
			}
			else
			{
				SetDlgItemText(IDC_EDIT_DIGITVALUE, L"");
			}
		}
	}
	else
	{
		RECT img_rect;
		m_static_digitview.GetClientRect(&img_rect);
		CDC* pDC = m_static_digitview.GetDC();
		pDC->FillSolidRect(&img_rect, RGB(255,255,255));
		ReleaseDC(pDC);

		SetDlgItemText(IDC_EDIT_DIGITVALUE, L"");
	}
}

void CDigitToolDlg::OnBnClickedCheckNotsetdigit()
{
	// TODO: 在此添加控件通知处理程序代码
	RefreshAll();
}


void CDigitToolDlg::OnBnClickedCheckSetdigit()
{
	// TODO: 在此添加控件通知处理程序代码
	RefreshAll();
}


void CDigitToolDlg::OnCbnSelchangeComboSetdigitres()
{
	// TODO: 在此添加控件通知处理程序代码
	RefreshAll();
}


void CDigitToolDlg::OnBnClickedButtonDigitsavevalue()
{
	// TODO: 在此添加控件通知处理程序代码
	CString new_value;
	GetDlgItemText(IDC_EDIT_DIGITVALUE, new_value);
	if (new_value == L"")
	{
		new_value = "x";
	}
	string value = ConvertSpecialDigitValue(CT2A(new_value).m_psz, false);

	int mark = m_digit_list.GetSelectionMark();
	if (mark >= 0)
	{
		Digit* pD = (Digit*)m_digit_list.GetItemData(mark);
		bool bSave = true;
		if (pD->value != value)		//进行了值的修改的话，则进行文件变更动作
		{
			DigitsRW rw;
			Digit newD = *pD;
			newD.value = value;
			bSave = rw.WriteOneDigitToXML(m_xml_Name, newD);
		}
		if (bSave)
		{
			pD->value = value;		//更新内存中的值
			m_digit_list.SetItemText(mark, VLAUE_COL_NUM, CA2T(ConvertSpecialDigitValue(value, true).c_str()));	//更新列表中当前行的标定值
			RefreshComboBoxDataList(); //刷新combo中的值列表
			ListSelectDigit(mark + 1);	//列表切到下一个字模位置
			RefreshCurrentDigit();		//刷新到下一个字模的信息
		}
		else
		{
			MessageBox(L"字模保存失败", L"错误", MB_OK);
		}
	}

	GetDlgItem(IDC_EDIT_DIGITVALUE)->SetFocus();
/*	((CEdit*)GetDlgItem(IDC_EDIT_DIGITVALUE))->SetCaretPos()*/
}

void CDigitToolDlg::ModelToBitmap(const Digit& d, CBitmapEx& bitmap)
{
	bitmap.Create(d.weight, d.height);
	for (int x = 0; x < d.weight; x++)
	{
		for (int y = 0; y < d.height; y++)
		{
			if (d.model.at(y*d.weight + x) == '0')
			{
				bitmap.SetPixel(x, y, 0x00FFFFFF);
			}
			else
			{
				bitmap.SetPixel(x, y, 0x00000000);
			}
		}
	}
}

void CDigitToolDlg::RefreshComboBoxDataList()
{
	int sel_old = m_select_set_value.GetCurSel();
	CString select_value_old;
	m_select_set_value.GetLBText(sel_old, select_value_old);

	m_select_set_value.ResetContent();
	m_select_set_value.InsertString(0, L"全部");

	set<string> datas;
	for (auto& d : m_digits)
	{
		datas.insert(ConvertSpecialDigitValue(d.value, true));
	}
	int index = 1;	//从1开始，0是"全部"
	int sel_new = 0;
	for (auto& s : datas)
	{
		m_select_set_value.InsertString(index, CA2T(s.c_str()));
		if (select_value_old == CA2T(s.c_str()))
		{
			sel_new = index;
		}

		index++;
	}

	m_select_set_value.SetCurSel(sel_new);
}

void CDigitToolDlg::RefreshAll()
{
	RefreshComboBoxDataList();
	RefreshDigitViewList();
	RefreshListCtrlData();
	RefreshCurrentDigit();
}


std::string CDigitToolDlg::ConvertSpecialDigitValue(const string& value, bool isOrgToUI/*原始字符UI转为UI汉字的flag，true为原始转UI*/)
{
	string res;
	if (isOrgToUI)
	{
		if (value == "x")
		{
			res = "未标定";
		}
		else if (value == " ")
		{
			res = "废弃";
		}
		else
		{
			res = value;
		}
	}
	else
	{
		if (value == "未标定")
		{
			res = "x";
		}
		else if (value == "废弃")
		{
			res = " ";
		}
		else
		{
			res = value;
		}
	}

	return res;
}

void CDigitToolDlg::ListSelectDigit(int index)
{
	int total = m_digit_list.GetItemCount();
	if (total > 0)
	{
		if (index > total - 1)
		{
			index = total - 1;
		}
		if (index < 0)
		{
			index = 0;
		}

		m_digit_list.SetSelectionMark(index);		//刷新后设置第一行为被选择状态
		m_digit_list.SelectItem(index, TRUE);
	}

}

void CDigitToolDlg::OnNMClickListDigits(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	if (pNMItemActivate->iItem >= 0)
	{
		RefreshCurrentDigit();
	}
	else
	{
		RefreshCurrentDigit(TRUE);		//Clean
	}
}


void CDigitToolDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码

	CDialogEx::OnPaint();
	CDialogEx::UpdateWindow();

	RefreshCurrentDigit();
}

BOOL CDigitToolDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam) {
		if (GetFocus() == GetDlgItem(IDC_EDIT_DIGITVALUE) || GetFocus() == GetDlgItem(IDC_BUTTON_DIGITSAVEVALUE))
		{
			OnBnClickedButtonDigitsavevalue();
			return TRUE;
		}
		if (GetFocus() == GetDlgItem(IDC_LIST_DIGITS))
		{
			RefreshCurrentDigit();
			return TRUE;
		}
	}

	if (pMsg->message == WM_MOUSEMOVE || pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONUP)
	{
		m_EditValueToolTip.RelayEvent(pMsg);
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDigitToolDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}
