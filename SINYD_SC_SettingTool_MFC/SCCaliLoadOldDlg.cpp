// SCCaliLoadOldDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SINYD_SC_SettingTool_MFC.h"
#include "SCCaliLoadOldDlg.h"
#include "afxdialogex.h"
#include "../Common/CommonDef.h"


// SCCaliLoadOldDlg �Ի���

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


// SCCaliLoadOldDlg ��Ϣ�������


BOOL SCCaliLoadOldDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//��ʼ��������ѡ��
	InitCaliNameCombo();
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void SCCaliLoadOldDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}

void SCCaliLoadOldDlg::InitCaliNameCombo()
{
	//�����������ѡ��
	comboCaliName.ResetContent();

	CFileFind finder;
	string path = ConfigFile_Existing_Folder"/*.*";
	BOOL bWorking = finder.FindFile(s2ws(path).c_str());//m_pathΪָ�����ļ���
	while (bWorking)//�����ļ���
	{
		bWorking = finder.FindNextFile();
		CString name = finder.GetFileName();//ȡ���ļ���
		CString extend = name.Right(
			name.GetLength() - name.ReverseFind('.') - 1);//ȡ����չ��
		if (!finder.IsDots())
		{
			if (extend == _T("xml"))//xmlΪ��Ҫ���ҵ��ļ���չ��
			{
				//�ļ�������������
				comboCaliName.AddString(name);
			}
		}
	}
}

//string SCCaliLoadOldDlg::ws2s(const wstring& ws, const char* clocale)
//{
//	wstringתstring
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
//	stringתwstring
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
	//ȡ�ñ궨������Ϣ���ָ��
	pCaliConfig = pCaliClass;
}


void SCCaliLoadOldDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	try
	{
		//��ȡ��ǰ������ѡ����ļ���
		CString caliName;
		comboCaliName.GetWindowTextW(caliName);
		//ƴ��·�����ļ�������ȡѡ��XML�ļ��ı궨������Ϣ
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
