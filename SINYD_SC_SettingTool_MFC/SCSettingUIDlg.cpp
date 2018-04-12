
// SCSettingUIDlg.cpp : 实现文件
//

#include <string>
#include "stdafx.h"
#include "SINYD_SC_SettingTool_MFC.h"
#include "SCSettingUIDlg.h"
#include "afxdialogex.h"
#include "afxpriv.h"
#include "RWData.h"
#include "log.h"
#include "SocketAPI.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// SCSettingUIDlg 对话框



SCSettingUIDlg::SCSettingUIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SINYD_SC_SETTINGUI_MFC_DIALOG, pParent)
	, m_trans_protocal(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void SCSettingUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, SocketIP_Ctrl);
	DDX_Control(pDX, IDC_EDIT2, CacheAdressEdit_Ctrl);
	//DDX_Control(pDX, IDC_EDIT5, TargetSoftwarePathEdit_Ctrl);
	DDX_Control(pDX, IDC_EDIT1, SocketPortEdit_Ctrl);
	DDX_Control(pDX, IDC_EDIT3, UsedSpaceEdit_Ctrl);
	DDX_Control(pDX, IDC_EDIT4, SaveTimeEdit_Ctrl);
	//DDX_Control(pDX, IDC_EDIT6, SoftwareNameEdit_Ctrl);
	//DDX_Control(pDX, IDC_EDIT7, ExeNameEdit_Ctrl);
	DDX_Control(pDX, IDC_EDIT8, StationIDEdit_Ctrl);
	DDX_Control(pDX, IDCANCEL, btnCancel);
	DDX_Control(pDX, IDC_BUTTON3, btnSave);
	DDX_Control(pDX, IDC_COMBO1, comboMixType);
	DDX_Control(pDX, IDC_EDIT5, edtThreadCount);
	DDX_Control(pDX, IDC_EDIT9, edtFrequency);
	DDX_Control(pDX, IDC_EDIT_SECTION_ID, SectionIDEdit_Ctrl);
	DDX_Control(pDX, IDC_EDIT_PROJECT_ID, ProjectIDEdit_Ctrl);
	DDX_Radio(pDX, IDC_RADIO1, m_trans_protocal);
	DDV_MinMaxInt(pDX, m_trans_protocal, 0, 1);
}

BEGIN_MESSAGE_MAP(SCSettingUIDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &SCSettingUIDlg::OnBnClickedButton1)
	//ON_BN_CLICKED(IDC_BUTTON2, &SCSettingUIDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &SCSettingUIDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON_IPTest, &SCSettingUIDlg::OnBnClickedButtonIptest)
END_MESSAGE_MAP()


// SCSettingToolDlg 消息处理程序

BOOL SCSettingUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	InitComboBox();
	m_trans_protocal = GetPrivateProfileIntA("Trans", "TransferFormatType", 1/*默认大数据协议*/, ".\\ConfigFile\\Trans.conf");
	UpdateData(FALSE);
	if (!InitSettingConfig())
	{
		MessageBox(_T("初始化配置数据失败"), _T("错误"), MB_OK);
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void SCSettingUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR SCSettingUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void SCSettingUIDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString path = CSelectFolderDlg::Show();
	//赋值
	CacheAdressEdit_Ctrl.SetWindowTextW(path);
	path.ReleaseBuffer();
}


//void SCSettingUIDlg::OnBnClickedButton2()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	CString path = CSelectFolderDlg::Show();
//	//赋值
//	TargetSoftwarePathEdit_Ctrl.SetWindowTextW(path);
//	path.ReleaseBuffer();
//}

bool SCSettingUIDlg::InitSettingConfig()
{
	CSettingConfig settingConfig;

	int loadResult = settingConfig.ReloadConfig();
	if (!loadResult)
	{
		ShowDlg(settingConfig);
		return true;
	}
	return false;
}

void SCSettingUIDlg::InitComboBox()
{
	//清除所有下拉选项
	comboMixType.ResetContent();
	//初始化下拉框
	comboMixType.InsertString(0, _T("沥青"));
	comboMixType.InsertString(1, _T("水泥"));
}

void SCSettingUIDlg::ShowDlg(const CSettingConfig& settingConfig)
{
	USES_CONVERSION;
	string strIP = settingConfig.GetSocketIP() == NULL ? "" : settingConfig.GetSocketIP();
	if (strIP != "")
	{
		//string strIP = settingConfig.GetSocketIP();
		//const char * p = strIP.c_str();
		DWORD dwIP = ntohl(inet_addr(settingConfig.GetSocketIP()));
		SocketIP_Ctrl.SetAddress(dwIP);
	}
	if (settingConfig.GetSoecketPort() != 0)
	{
		CString strPort;
		strPort.Format(_T("%d"), settingConfig.GetSoecketPort());
		SocketPortEdit_Ctrl.SetWindowTextW(strPort);
	}
	CacheAdressEdit_Ctrl.SetWindowTextW(CA2T(settingConfig.GetCacheAdress()));
	if (settingConfig.GetCacheUsedSpace() != 0)
	{
		CString strUsedSpace;
		strUsedSpace.Format(_T("%d"), settingConfig.GetCacheUsedSpace());
		UsedSpaceEdit_Ctrl.SetWindowTextW(strUsedSpace);
	}
	if (settingConfig.GetCacheSaveTime() != 0)
	{
		CString strSaveTime;
		strSaveTime.Format(_T("%d"), settingConfig.GetCacheSaveTime());
		SaveTimeEdit_Ctrl.SetWindowTextW(strSaveTime);
	}
	//TargetSoftwarePathEdit_Ctrl.SetWindowTextW(CA2T(settingConfig.GetTargetSoftwarePath()));
	//SoftwareNameEdit_Ctrl.SetWindowTextW(CA2T(settingConfig.GetTargetSoftwareName()));
	//ExeNameEdit_Ctrl.SetWindowTextW(CA2T(settingConfig.GetTargetExeName()));
	StationIDEdit_Ctrl.SetWindowTextW(CA2T(settingConfig.GetMixingStationID()));
	SectionIDEdit_Ctrl.SetWindowTextW(CA2T(settingConfig.GetMixingSectionID()));
	ProjectIDEdit_Ctrl.SetWindowTextW(CA2T(settingConfig.GetMixingProjectID()));
	int mixType = settingConfig.GetMixType();
	if (mixType > 0)
	{
		comboMixType.SetCurSel(mixType - 1);
	}
	CString strThreadCount(_T("2"));
	if (settingConfig.GetCollThreadCount() != 0)
	{
		strThreadCount.Format(_T("%d"), settingConfig.GetCollThreadCount());
	}
	edtThreadCount.SetWindowTextW(strThreadCount);
	CString strFrequency(_T("2"));
	if (settingConfig.GetCollFreq() != 0)
	{
		strFrequency.Format(_T("%d"), settingConfig.GetCollFreq());
	}
	edtFrequency.SetWindowTextW(strFrequency);
}

bool SCSettingUIDlg::ReadDlgToEntry(CSettingConfig* settingConfig)
{
	BYTE nf1, nf2, nf3, nf4;
	int tempLen;
	if (SocketIP_Ctrl.IsBlank())
	{
		MessageBox(_T("请填写有效IP"), _T("提示"), MB_OK);
		return false;
	}
	SocketIP_Ctrl.GetAddress(nf1, nf2, nf3, nf4);
	if (nf1 == 0 || (nf1 == 0 && nf2 == 0 && nf3 == 0 && nf4 == 0))
	{
		MessageBox(_T("请填写有效IP"), _T("提示"), MB_OK);
		return false;
	}
	USES_CONVERSION;
	CString cstrIP;
	cstrIP.Format(_T("%d.%d.%d.%d"), nf1, nf2, nf3, nf4);
	tempLen = strlen(CT2A(cstrIP));
	settingConfig->SetSocketIP(CT2A(cstrIP), tempLen);
	CString strEdit;
	SocketPortEdit_Ctrl.GetWindowTextW(strEdit);
	if (_ttoi(strEdit) == 0)
	{
		MessageBox(_T("请填写有效端口号"), _T("提示"), MB_OK);
		return false;
	}
	settingConfig->SetSoecketPort(_ttoi(strEdit));

	CacheAdressEdit_Ctrl.GetWindowTextW(strEdit);
	if (strEdit == "")
	{
		MessageBox(_T("截图Cache保存地址不能为空"), _T("提示"), MB_OK);
		return false;
	}
	tempLen = strlen(CT2A(strEdit));
	settingConfig->SetCacheAdress(CT2A(strEdit), tempLen);
	UsedSpaceEdit_Ctrl.GetWindowTextW(strEdit);
	if (_ttoi(strEdit) == 0)
	{
		MessageBox(_T("请填写有效的Cache占用空间"), _T("提示"), MB_OK);
		return false;
	}
	settingConfig->SetCacheUsedSpace(_ttoi(strEdit));
	SaveTimeEdit_Ctrl.GetWindowTextW(strEdit);
	if (_ttoi(strEdit) == 0)
	{
		MessageBox(_T("请填写有效的Cache保存时长"), _T("提示"), MB_OK);
		return false;
	}
	settingConfig->SetCacheSaveTime(_ttoi(strEdit));

	/*TargetSoftwarePathEdit_Ctrl.GetWindowTextW(strEdit);
	if (strEdit == "")
	{
		MessageBox(_T("目标程序安装路径不能为空"), _T("提示"), MB_OK);
		return false;
	}
	tempLen = strlen(CT2A(strEdit));
	settingConfig->SetTargetSoftwarePath(CT2A(strEdit), tempLen);
	SoftwareNameEdit_Ctrl.GetWindowTextW(strEdit);
	tempLen = strlen(CT2A(strEdit));
	settingConfig->SetTargetSoftwareName(CT2A(strEdit), tempLen);
	ExeNameEdit_Ctrl.GetWindowTextW(strEdit);
	if (strEdit == "")
	{
		MessageBox(_T("目标程序可执行程序名不能为空"), _T("提示"), MB_OK);
		return false;
	}
	tempLen = strlen(CT2A(strEdit));
	settingConfig->SetTargetExeName(CT2A(strEdit), tempLen);*/

	StationIDEdit_Ctrl.GetWindowTextW(strEdit);
	if (strEdit == "")
	{
		MessageBox(_T("拌和机ID不能为空"), _T("提示"), MB_OK);
		return false;
	}
	tempLen = strlen(CT2A(strEdit));
	settingConfig->SetMixingStationID(CT2A(strEdit), tempLen);

	SectionIDEdit_Ctrl.GetWindowTextW(strEdit);
	if (strEdit == "")
	{
		MessageBox(_T("标段/拌和站ID不能为空"), _T("提示"), MB_OK);
		return false;
	}
	tempLen = strlen(CT2A(strEdit));
	settingConfig->SetMixingSectionID(CT2A(strEdit), tempLen);

	ProjectIDEdit_Ctrl.GetWindowTextW(strEdit);
	if (strEdit == "")
	{
		MessageBox(_T("项目ID不能为空"), _T("提示"), MB_OK);
		return false;
	}
	tempLen = strlen(CT2A(strEdit));
	settingConfig->SetMixingProjectID(CT2A(strEdit), tempLen);

	int mixType = comboMixType.GetCurSel();
	if (mixType < 0)
	{
		MessageBox(_T("请选择拌和机类型"), _T("提示"), MB_OK);
		return false;
	}
	settingConfig->SetMixType(mixType + 1);

	edtThreadCount.GetWindowTextW(strEdit);
	if (_ttoi(strEdit) < 1 || _ttoi(strEdit) >8)
	{
		MessageBox(_T("请填写有效的线程个数"), _T("提示"), MB_OK);
		return false;
	}
	settingConfig->SetCollThreadCount(_ttoi(strEdit));
	edtFrequency.GetWindowTextW(strEdit);
	if (_ttoi(strEdit) < 1 || _ttoi(strEdit) > 5)
	{
		MessageBox(_T("请填写有效的采集频率"), _T("提示"), MB_OK);
		return false;
	}
	settingConfig->SetCollFreq(_ttoi(strEdit));

	return true;
}

void SCSettingUIDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CSettingConfig settingConfig;
	bool flg = ReadDlgToEntry(&settingConfig);
	if (!flg)
	{
		return;
	}
	int loadResult = settingConfig.SaveConfig();
	if (!loadResult)
	{
		//保存用于计算和传输的配置文件
		WriteConfigFile(settingConfig);
		MessageBox(_T("保存成功"), _T("提示"), MB_OK);


		//检查服务器路径是否已设置
		FILE* pathFile = fopen(".\\cwRsync\\RemoteServerPath.txt", "r");
		if (pathFile)
		{
			char ch = fgetc(pathFile);
			if (ch != EOF && ch && '\n' || ch && '\r')
			{
				//触发配置上传脚本
				char fileFullPath[MAX_PATH] = "";
				::GetModuleFileNameA(::GetModuleHandle(NULL), fileFullPath, MAX_PATH);
				std::string strDir(fileFullPath);
				strDir = strDir.erase(strDir.find_last_of('\\'));
				string batPath = strDir + "\\cwRsync\\UploadConfig.bat";

				PROCESS_INFORMATION pi;
				STARTUPINFOA si;
				ZeroMemory(&si, sizeof(si));
				si.cb = sizeof(si);
				si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
				if (!CreateProcessA(NULL,
					(LPSTR)batPath.c_str(),
					NULL,
					NULL,
					FALSE,
					CREATE_NO_WINDOW,
					NULL,
					NULL,
					&si, &pi))
				{
					SCERROR("[AppManager]%s run Fail", batPath.c_str());
				}
			}

			fclose(pathFile);
		}
		
	}

	//写入协议选项设置结果
	UpdateData(TRUE);
	WritePrivateProfileStringA("Trans", "TransferFormatType", to_string(m_trans_protocal).c_str(), ".\\ConfigFile\\Trans.conf");

	return;
}

void SCSettingUIDlg::WriteConfigFile(const CSettingConfig& settingConfig)
{
	const char * fileName = ConfigFile_Folder "/" Mix_Sys_Conf;
	char port[10];
	_itoa(settingConfig.GetSoecketPort(), port, 10);
	CRWData CWritefile;
	//没有则创建文件，有则无处理
	CWritefile._CreateFile((char *)fileName);
	//保存Socket传输IP和端口
	CWritefile._WriteProfileString("modbus_server", "ip", (char *)(settingConfig.GetSocketIP()), (char *)fileName);
	CWritefile._WriteProfileString("modbus_server", "port", port, (char *)fileName);
	//保存拌和站信息，拌和机ID和拌和机类型，项目ID及标段/拌和站ID
	char mixType[2];
	_itoa(settingConfig.GetMixType(), mixType, 10);
	CWritefile._WriteProfileString("mix_station_conf", "mix_stationname", (char *)(settingConfig.GetMixingSectionID()), (char *)fileName);
	CWritefile._WriteProfileString("mix_station_conf", "mix_project_name", (char *)(settingConfig.GetMixingProjectID()), (char *)fileName);
	CWritefile._WriteProfileString("mix_machine_1", "mix_machine_device_id", (char *)(settingConfig.GetMixingStationID()), (char *)fileName);
	CWritefile._WriteProfileString("mix_machine_1", "mix_machine_type", mixType, (char *)fileName);
}



void SCSettingUIDlg::OnBnClickedButtonIptest()
{
	// TODO: 在此添加控件通知处理程序代码

	BYTE nf1, nf2, nf3, nf4;
	int tempLen;
	if (SocketIP_Ctrl.IsBlank())
	{
		MessageBox(_T("请填写有效IP"), _T("提示"), MB_OK);
		return;
	}
	SocketIP_Ctrl.GetAddress(nf1, nf2, nf3, nf4);
	if (nf1 == 0 || (nf1 == 0 && nf2 == 0 && nf3 == 0 && nf4 == 0))
	{
		MessageBox(_T("请填写有效IP"), _T("提示"), MB_OK);
		return;
	}

	USES_CONVERSION;
	CString cstrIP;
	cstrIP.Format(_T("%d.%d.%d.%d"), nf1, nf2, nf3, nf4);

	CString strEdit;
	SocketPortEdit_Ctrl.GetWindowTextW(strEdit);
	int portNum = _ttoi(strEdit);
	if (portNum == 0)
	{
		MessageBox(_T("请填写有效端口号"), _T("提示"), MB_OK);
		return;
	}


	socket_Init();

	HSocket hs = socket_Open(SOCK_STREAM);
	if (socket_Isvalid(hs))
	{
		sockaddr_in addr;
		socket_GetAddr_in(CT2A(cstrIP), portNum, &addr);
		int res = socket_Connect(hs, &addr);
		if (res == 0)
		{
			MessageBox(L"连接成功！");
		}
		else
		{
			res = socket_GetLastError();
			CString err;
			TCHAR cerr[1024] = { 0 };
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, (void*)FORMAT_MESSAGE_FROM_STRING, res, 0, cerr, 1024, 0);
			err.Format(L"连接失败，错误码 = %d %s", res, cerr);
			MessageBox(err);
		}
		socket_Close(hs);
	}
	else
	{
		MessageBox(L"打开套接字句柄失败！");
	}

	socket_Uninit();
}
