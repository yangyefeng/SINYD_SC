// SCCaliLoadOldDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SINYD_SC_SettingTool_MFC.h"
#include "SCCaliLoadOldDlg.h"
#include "afxdialogex.h"
#include "../Common/CommonDef.h"


// SCCaliLoadOldDlg 对话框

IMPLEMENT_DYNAMIC(SCCaliLoadOldDlg, CDialogEx)

SCCaliLoadOldDlg::SCCaliLoadOldDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CALIUI_LOADDOC_MFC_DIALOG, pParent)
{

}

SCCaliLoadOldDlg::~SCCaliLoadOldDlg()
{
}

void SCCaliLoadOldDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, comboCaliName);
}


BEGIN_MESSAGE_MAP(SCCaliLoadOldDlg, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &SCCaliLoadOldDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &SCCaliLoadOldDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// SCCaliLoadOldDlg 消息处理程序


BOOL SCCaliLoadOldDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//初始化下拉框选项
	InitCaliNameCombo();
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void SCCaliLoadOldDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

void SCCaliLoadOldDlg::InitCaliNameCombo()
{
	//清除所有下拉选项
	comboCaliName.ResetContent();

	CFileFind finder;
	string path = ConfigFile_Existing_Folder"/*.*";
	BOOL bWorking = finder.FindFile(s2ws(path).c_str());//m_path为指定的文件夹
	while (bWorking)//遍历文件夹
	{
		bWorking = finder.FindNextFile();
		CString name = finder.GetFileName();//取得文件名
		CString extend = name.Right(
			name.GetLength() - name.ReverseFind('.') - 1);//取得扩展名
		if (!finder.IsDots())
		{
			if (extend == _T("xml"))//xml为你要查找的文件扩展名
			{
				//文件名加入下拉框
				comboCaliName.AddString(name);
			}
		}
	}
}

//string SCCaliLoadOldDlg::ws2s(const wstring& ws, const char* clocale)
//{
//	wstring转string
//	string curLocale = setlocale(LC_ALL, NULL);
//
//	setlocale(LC_ALL, clocale);
//
//	const wchar_t* _Source = ws.c_str();
//	size_t _Dsize = 2 * ws.size() + 1;
//	char *_Dest = new char[_Dsize];
//	memset(_Dest, 0, _Dsize);
//	size_t ReturnValue = 0;
//	wcstombs_s(&ReturnValue, _Dest, _Dsize, _Source, _Dsize);
//	string result = _Dest;
//	delete[]_Dest;
//
//	setlocale(LC_ALL, curLocale.c_str());
//
//	return result;
//}
//
//wstring SCCaliLoadOldDlg::s2ws(const string& s, const char* clocale)
//{
//	string转wstring
//	string curLocale = setlocale(LC_ALL, NULL);
//
//	setlocale(LC_ALL, clocale);
//
//	const char* _Source = s.c_str();
//	size_t _Dsize = s.size() + 1;
//	wchar_t *_Dest = new wchar_t[_Dsize];
//	wmemset(_Dest, 0, _Dsize);
//	size_t ReturnValue = 0;
//	mbstowcs_s(&ReturnValue, _Dest, _Dsize, _Source, _Dsize);
//	wstring result = _Dest;
//	delete[]_Dest;
//
//	setlocale(LC_ALL, curLocale.c_str());
//
//	return result;
//}

void SCCaliLoadOldDlg::SetPCaliClass(CCalibrationConfig* pCaliClass)
{
	//取得标定配置信息类的指针
	pCaliConfig = pCaliClass;
}


void SCCaliLoadOldDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	try
	{
		//获取当前下拉框选择的文件名
		CString caliName;
		comboCaliName.GetWindowTextW(caliName);
		//拼接路径和文件名，读取选中XML文件的标定配置信息
		wstring wpath(caliName.GetString());
		string path = ConfigFile_Existing_Folder"/" + ws2s(wpath);
		pCaliConfig->ReloadConfig(path);

		EndDialog(IDOK);
	}
	catch (CException* e)
	{
		return;
	}
}
