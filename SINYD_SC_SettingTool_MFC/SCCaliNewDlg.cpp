// SCCaliNewDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SINYD_SC_SettingTool_MFC.h"
#include "SCCaliNewDlg.h"
#include "afxdialogex.h"


// SCCaliNewDlg 对话框

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


// SCCaliNewDlg 消息处理程序


BOOL SCCaliNewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void SCCaliNewDlg::SetPCaliClass(CCalibrationConfig* pCaliClass)
{
	//取得标定配置信息类的指针
	pCaliConfig = pCaliClass;
}


//void SCCaliNewDlg::OnBnClickedButton4()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	//获取程序执行exe的全路径和程序名
//	CString strFile = _T("");
//
//	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_NOCHANGEDIR, _T("Describe Files (*.exe)|*.exe|All Files (*.*)|*.*||"), NULL);
//
//	if (dlgFile.DoModal())
//	{
//		strFile = dlgFile.GetPathName();
//	}
//	//赋给程序路径控件
//	edtSoftwarePath.SetWindowTextW(strFile);
//}

void SCCaliNewDlg::OnBnClickedButton1()
{
	try
	{
		// TODO: 在此添加控件通知处理程序代码
		USES_CONVERSION;
		int tempLen;
		CString strEdit;
		//获取程序名称
		edtSoftwareName.GetWindowTextW(strEdit);
		if (strEdit == "")
		{
			MessageBox(_T("拌和站程序名称不能为空"), _T("提示"), MB_OK);
			return;
		}
		tempLen = strEdit == _T("") ? 0 : strlen(CT2A(strEdit));
		pCaliConfig->SetSoftwareName(CT2A(strEdit), tempLen);
		//获取程序版本
		edtVSoftwareVer.GetWindowTextW(strEdit);
		if (strEdit == "")
		{
			MessageBox(_T("拌和站程序版本不能为空"), _T("提示"), MB_OK);
			return;
		}
		tempLen = strEdit == _T("") ? 0 : strlen(CT2A(strEdit));
		pCaliConfig->SetSoftwareVer(CT2A(strEdit), tempLen);
		////获取执行程序路径和名称
		//edtSoftwarePath.GetWindowTextW(strEdit);
		//if (strEdit == "")
		//{
		//	MessageBox(_T("拌和站执行程序路径和名称不能为空"), _T("提示"), MB_OK);
		//	return;
		//}
		//tempLen = strEdit == _T("") ? 0 : strlen(CT2A(strEdit));
		//pCaliConfig->SetSoftwarePath(CT2A(strEdit), tempLen);

		//删除标定配置类中的其他信息
		pCaliConfig->SetSoftwarePath(NULL, 0);
		pCaliConfig->SetWindowName(NULL, 0);
		pCaliConfig->SetWindowClass(NULL, 0);
		pCaliConfig->SetWindowHeight(0);
		pCaliConfig->SetWindowWidth(0);
		pCaliConfig->SetIsAdmixtureToWater(0);

		//删除标定的区域信息
		const vector<MonitorArea *>* pMonitorAreas = pCaliConfig->GetMonitorAreas();
		if (pMonitorAreas->size() > 0)
		{
			//遍历MonitorAreas，取出所有的MonitorAreaID
			vector<int> monitorIDs;
			for (int i = 0; i < pMonitorAreas->size(); i++)
			{
				int monitorID = (*pMonitorAreas)[i]->GetMonitorAreaID();
				monitorIDs.push_back(monitorID);
			}
			//遍历所有的MonitorAreaID，逐个删除标定配置信息中的MonitorArea
			for (int j = 0; j < monitorIDs.size(); j++)
			{
				pCaliConfig->DelMonitorArea(monitorIDs[j]);
			}
			//释放monitorIDs
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
