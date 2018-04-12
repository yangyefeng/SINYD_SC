// SCCaliSaveAsDlg.cpp : ʵ���ļ�
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

// SCCaliSaveAsDlg �Ի���

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


// SCCaliSaveAsDlg ��Ϣ�������


void SCCaliSaveAsDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}

BOOL SCCaliSaveAsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//ȷ���ļ�·�����ڣ�û���򴴽�
	string path = ConfigFile_Existing_Folder"/";
	::MakeSureDirectoryPathExists(path.c_str());

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void SCCaliSaveAsDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ȡ��ǰ�༭����ļ���
	CString newName;
	edtNewName.GetWindowTextW(newName);
	newName.TrimLeft();
	newName.TrimRight();
	if (newName.Compare(_T("")) == 0)
	{
		//������Ϊ��
		MessageBox(_T("�����궨��Ϣ�����Ʋ���Ϊ��"), _T("��ʾ"), MB_OK);
		return;
	}

	CFileFind finder;
	newName = newName + _T(".xml");
	string path = ConfigFile_Existing_Folder"/*.xml";
	BOOL bWorking = finder.FindFile(s2ws(path).c_str());//pathΪָ�����ļ���
	while (bWorking)//�����ļ���
	{
		bWorking = finder.FindNextFile();
		CString fname = finder.GetFileName();//ȡ���ļ���
		if (newName.CompareNoCase(fname) == 0)
		{
			//����ͬ���ļ�����ʾ�û��Ƿ񸲸ǣ����򸲸ǣ����򷵻�
			if (MessageBox(_T("�ļ����Ѵ��ڣ��Ƿ񸲸�"), _T("��ʾ"), MB_YESNO) == IDNO)
			{
				return;
			}
			else
			{
				break;
			}
		}		
	}
	//ƴ��·�����ļ���������ƴ�Ӻ������Ƶ�XML�ļ�
	wstring wname(newName.GetString());
	string sname = ws2s(wname);
	//���浽��Ӧ·��
	path = ConfigFile_Existing_Folder"/" + sname;
	pCaliConfig->SaveConfig(path);

	EndDialog(IDOK);
}

//string SCCaliSaveAsDlg::ws2s(const wstring& ws, const char* clocale)
//{
//	//wstringתstring
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
//	//stringתwstring
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
	//ȡ�ñ궨������Ϣ���ָ��
	pCaliConfig = pCaliClass;
}

string SCCaliSaveAsDlg::strToLower(const string &str)
{
	//stringתΪСд
	string strTmp = str;
	transform(strTmp.begin(), strTmp.end(), strTmp.begin(), tolower);
	return strTmp;
}

bool SCCaliSaveAsDlg::compareNoCase(const string &strA, const string &strB)
{
	//����string��תΪСд����бȽ�
	string str1 = strToLower(strA);
	string str2 = strToLower(strB);
	return (str1 == str2);
}
