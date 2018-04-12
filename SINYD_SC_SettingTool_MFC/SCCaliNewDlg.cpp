// SCCaliNewDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SINYD_SC_SettingTool_MFC.h"
#include "SCCaliNewDlg.h"
#include "afxdialogex.h"


// SCCaliNewDlg �Ի���

IMPLEMENT_DYNAMIC(SCCaliNewDlg, CDialogEx)

SCCaliNewDlg::SCCaliNewDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CALIUI_NEWDOC_MFC_DIALOG, pParent)
{

}

SCCaliNewDlg::~SCCaliNewDlg()
{
}

void SCCaliNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT3, edtSoftwareName);
	//DDX_Control(pDX, IDC_EDIT4, edtSoftwarePath);
	DDX_Control(pDX, IDC_EDIT13, edtVSoftwareVer);
}


BEGIN_MESSAGE_MAP(SCCaliNewDlg, CDialogEx)
	//ON_BN_CLICKED(IDC_BUTTON4, &SCCaliNewDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON1, &SCCaliNewDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// SCCaliNewDlg ��Ϣ�������


BOOL SCCaliNewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void SCCaliNewDlg::SetPCaliClass(CCalibrationConfig* pCaliClass)
{
	//ȡ�ñ궨������Ϣ���ָ��
	pCaliConfig = pCaliClass;
}


//void SCCaliNewDlg::OnBnClickedButton4()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	//��ȡ����ִ��exe��ȫ·���ͳ�����
//	CString strFile = _T("");
//
//	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_NOCHANGEDIR, _T("Describe Files (*.exe)|*.exe|All Files (*.*)|*.*||"), NULL);
//
//	if (dlgFile.DoModal())
//	{
//		strFile = dlgFile.GetPathName();
//	}
//	//��������·���ؼ�
//	edtSoftwarePath.SetWindowTextW(strFile);
//}

void SCCaliNewDlg::OnBnClickedButton1()
{
	try
	{
		// TODO: �ڴ���ӿؼ�֪ͨ����������
		USES_CONVERSION;
		int tempLen;
		CString strEdit;
		//��ȡ��������
		edtSoftwareName.GetWindowTextW(strEdit);
		if (strEdit == "")
		{
			MessageBox(_T("���վ�������Ʋ���Ϊ��"), _T("��ʾ"), MB_OK);
			return;
		}
		tempLen = strEdit == _T("") ? 0 : strlen(CT2A(strEdit));
		pCaliConfig->SetSoftwareName(CT2A(strEdit), tempLen);
		//��ȡ����汾
		edtVSoftwareVer.GetWindowTextW(strEdit);
		if (strEdit == "")
		{
			MessageBox(_T("���վ����汾����Ϊ��"), _T("��ʾ"), MB_OK);
			return;
		}
		tempLen = strEdit == _T("") ? 0 : strlen(CT2A(strEdit));
		pCaliConfig->SetSoftwareVer(CT2A(strEdit), tempLen);
		////��ȡִ�г���·��������
		//edtSoftwarePath.GetWindowTextW(strEdit);
		//if (strEdit == "")
		//{
		//	MessageBox(_T("���վִ�г���·�������Ʋ���Ϊ��"), _T("��ʾ"), MB_OK);
		//	return;
		//}
		//tempLen = strEdit == _T("") ? 0 : strlen(CT2A(strEdit));
		//pCaliConfig->SetSoftwarePath(CT2A(strEdit), tempLen);

		//ɾ���궨�������е�������Ϣ
		pCaliConfig->SetSoftwarePath(NULL, 0);
		pCaliConfig->SetWindowName(NULL, 0);
		pCaliConfig->SetWindowClass(NULL, 0);
		pCaliConfig->SetWindowHeight(0);
		pCaliConfig->SetWindowWidth(0);
		pCaliConfig->SetIsAdmixtureToWater(0);

		//ɾ���궨��������Ϣ
		const vector<MonitorArea *>* pMonitorAreas = pCaliConfig->GetMonitorAreas();
		if (pMonitorAreas->size() > 0)
		{
			//����MonitorAreas��ȡ�����е�MonitorAreaID
			vector<int> monitorIDs;
			for (int i = 0; i < pMonitorAreas->size(); i++)
			{
				int monitorID = (*pMonitorAreas)[i]->GetMonitorAreaID();
				monitorIDs.push_back(monitorID);
			}
			//�������е�MonitorAreaID�����ɾ���궨������Ϣ�е�MonitorArea
			for (int j = 0; j < monitorIDs.size(); j++)
			{
				pCaliConfig->DelMonitorArea(monitorIDs[j]);
			}
			//�ͷ�monitorIDs
			monitorIDs.clear();
			vector<int>(monitorIDs).swap(monitorIDs);
		}

		EndDialog(IDOK);
	}
	catch (CException* e)
	{
		return;
	}
}
