// SCCaliSaveAsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SINYD_SC_SettingTool_MFC.h"
#include "SCCaliSaveAsDlg.h"
#include "afxdialogex.h"
#include <string>
#include <DbgHelp.h>
#include <algorithm>
#include "../Common/CommonDef.h"

using namespace std;

// SCCaliSaveAsDlg 对话框

IMPLEMENT_DYNAMIC(SCCaliSaveAsDlg, CDialogEx)

SCCaliSaveAsDlg::SCCaliSaveAsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CALIUI_SAVEAS_MFC_DIALOG, pParent)
{

}

SCCaliSaveAsDlg::~SCCaliSaveAsDlg()
{
}

void SCCaliSaveAsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, edtNewName);
}


BEGIN_MESSAGE_MAP(SCCaliSaveAsDlg, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &SCCaliSaveAsDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &SCCaliSaveAsDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// SCCaliSaveAsDlg 消息处理程序


void SCCaliSaveAsDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

BOOL SCCaliSaveAsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//确定文件路径存在，没有则创建
	string path = ConfigFile_Existing_Folder"/";
	::MakeSureDirectoryPathExists(path.c_str());

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void SCCaliSaveAsDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//获取当前编辑框的文件名
	CString newName;
	edtNewName.GetWindowTextW(newName);
	newName.TrimLeft();
	newName.TrimRight();
	if (newName.Compare(_T("")) == 0)
	{
		//新名称为空
		MessageBox(_T("样本标定信息的名称不能为空"), _T("提示"), MB_OK);
		return;
	}

	CFileFind finder;
	newName = newName + _T(".xml");
	string path = ConfigFile_Existing_Folder"/*.xml";
	BOOL bWorking = finder.FindFile(s2ws(path).c_str());//path为指定的文件夹
	while (bWorking)//遍历文件夹
	{
		bWorking = finder.FindNextFile();
		CString fname = finder.GetFileName();//取得文件名
		if (newName.CompareNoCase(fname) == 0)
		{
			//存在同名文件，提示用户是否覆盖，是则覆盖，否则返回
			if (MessageBox(_T("文件名已存在，是否覆盖"), _T("提示"), MB_YESNO) == IDNO)
			{
				return;
			}
			else
			{
				break;
			}
		}		
	}
	//拼接路径和文件名，保存拼接好新名称的XML文件
	wstring wname(newName.GetString());
	string sname = ws2s(wname);
	//保存到对应路径
	path = ConfigFile_Existing_Folder"/" + sname;
	pCaliConfig->SaveConfig(path);

	EndDialog(IDOK);
}

//string SCCaliSaveAsDlg::ws2s(const wstring& ws, const char* clocale)
//{
//	//wstring转string
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
//wstring SCCaliSaveAsDlg::s2ws(const string& s, const char* clocale)
//{
//	//string转wstring
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

void SCCaliSaveAsDlg::SetPCaliClass(CCalibrationConfig* pCaliClass)
{
	//取得标定配置信息类的指针
	pCaliConfig = pCaliClass;
}

string SCCaliSaveAsDlg::strToLower(const string &str)
{
	//string转为小写
	string strTmp = str;
	transform(strTmp.begin(), strTmp.end(), strTmp.begin(), tolower);
	return strTmp;
}

bool SCCaliSaveAsDlg::compareNoCase(const string &strA, const string &strB)
{
	//两个string都转为小写后进行比较
	string str1 = strToLower(strA);
	string str2 = strToLower(strB);
	return (str1 == str2);
}
