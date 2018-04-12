
// SCSettingUIDlg.cpp : ʵ���ļ�
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


// SCSettingUIDlg �Ի���



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


// SCSettingToolDlg ��Ϣ�������

BOOL SCSettingUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	InitComboBox();
	m_trans_protocal = GetPrivateProfileIntA("Trans", "TransferFormatType", 1/*Ĭ�ϴ�����Э��*/, ".\\ConfigFile\\Trans.conf");
	UpdateData(FALSE);
	if (!InitSettingConfig())
	{
		MessageBox(_T("��ʼ����������ʧ��"), _T("����"), MB_OK);
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void SCSettingUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR SCSettingUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void SCSettingUIDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString path = CSelectFolderDlg::Show();
	//��ֵ
	CacheAdressEdit_Ctrl.SetWindowTextW(path);
	path.ReleaseBuffer();
}


//void SCSettingUIDlg::OnBnClickedButton2()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	CString path = CSelectFolderDlg::Show();
//	//��ֵ
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
	//�����������ѡ��
	comboMixType.ResetContent();
	//��ʼ��������
	comboMixType.InsertString(0, _T("����"));
	comboMixType.InsertString(1, _T("ˮ��"));
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
		MessageBox(_T("����д��ЧIP"), _T("��ʾ"), MB_OK);
		return false;
	}
	SocketIP_Ctrl.GetAddress(nf1, nf2, nf3, nf4);
	if (nf1 == 0 || (nf1 == 0 && nf2 == 0 && nf3 == 0 && nf4 == 0))
	{
		MessageBox(_T("����д��ЧIP"), _T("��ʾ"), MB_OK);
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
		MessageBox(_T("����д��Ч�˿ں�"), _T("��ʾ"), MB_OK);
		return false;
	}
	settingConfig->SetSoecketPort(_ttoi(strEdit));

	CacheAdressEdit_Ctrl.GetWindowTextW(strEdit);
	if (strEdit == "")
	{
		MessageBox(_T("��ͼCache�����ַ����Ϊ��"), _T("��ʾ"), MB_OK);
		return false;
	}
	tempLen = strlen(CT2A(strEdit));
	settingConfig->SetCacheAdress(CT2A(strEdit), tempLen);
	UsedSpaceEdit_Ctrl.GetWindowTextW(strEdit);
	if (_ttoi(strEdit) == 0)
	{
		MessageBox(_T("����д��Ч��Cacheռ�ÿռ�"), _T("��ʾ"), MB_OK);
		return false;
	}
	settingConfig->SetCacheUsedSpace(_ttoi(strEdit));
	SaveTimeEdit_Ctrl.GetWindowTextW(strEdit);
	if (_ttoi(strEdit) == 0)
	{
		MessageBox(_T("����д��Ч��Cache����ʱ��"), _T("��ʾ"), MB_OK);
		return false;
	}
	settingConfig->SetCacheSaveTime(_ttoi(strEdit));

	/*TargetSoftwarePathEdit_Ctrl.GetWindowTextW(strEdit);
	if (strEdit == "")
	{
		MessageBox(_T("Ŀ�����װ·������Ϊ��"), _T("��ʾ"), MB_OK);
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
		MessageBox(_T("Ŀ������ִ�г���������Ϊ��"), _T("��ʾ"), MB_OK);
		return false;
	}
	tempLen = strlen(CT2A(strEdit));
	settingConfig->SetTargetExeName(CT2A(strEdit), tempLen);*/

	StationIDEdit_Ctrl.GetWindowTextW(strEdit);
	if (strEdit == "")
	{
		MessageBox(_T("��ͻ�ID����Ϊ��"), _T("��ʾ"), MB_OK);
		return false;
	}
	tempLen = strlen(CT2A(strEdit));
	settingConfig->SetMixingStationID(CT2A(strEdit), tempLen);

	SectionIDEdit_Ctrl.GetWindowTextW(strEdit);
	if (strEdit == "")
	{
		MessageBox(_T("���/���վID����Ϊ��"), _T("��ʾ"), MB_OK);
		return false;
	}
	tempLen = strlen(CT2A(strEdit));
	settingConfig->SetMixingSectionID(CT2A(strEdit), tempLen);

	ProjectIDEdit_Ctrl.GetWindowTextW(strEdit);
	if (strEdit == "")
	{
		MessageBox(_T("��ĿID����Ϊ��"), _T("��ʾ"), MB_OK);
		return false;
	}
	tempLen = strlen(CT2A(strEdit));
	settingConfig->SetMixingProjectID(CT2A(strEdit), tempLen);

	int mixType = comboMixType.GetCurSel();
	if (mixType < 0)
	{
		MessageBox(_T("��ѡ���ͻ�����"), _T("��ʾ"), MB_OK);
		return false;
	}
	settingConfig->SetMixType(mixType + 1);

	edtThreadCount.GetWindowTextW(strEdit);
	if (_ttoi(strEdit) < 1 || _ttoi(strEdit) >8)
	{
		MessageBox(_T("����д��Ч���̸߳���"), _T("��ʾ"), MB_OK);
		return false;
	}
	settingConfig->SetCollThreadCount(_ttoi(strEdit));
	edtFrequency.GetWindowTextW(strEdit);
	if (_ttoi(strEdit) < 1 || _ttoi(strEdit) > 5)
	{
		MessageBox(_T("����д��Ч�Ĳɼ�Ƶ��"), _T("��ʾ"), MB_OK);
		return false;
	}
	settingConfig->SetCollFreq(_ttoi(strEdit));

	return true;
}

void SCSettingUIDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CSettingConfig settingConfig;
	bool flg = ReadDlgToEntry(&settingConfig);
	if (!flg)
	{
		return;
	}
	int loadResult = settingConfig.SaveConfig();
	if (!loadResult)
	{
		//�������ڼ���ʹ���������ļ�
		WriteConfigFile(settingConfig);
		MessageBox(_T("����ɹ�"), _T("��ʾ"), MB_OK);


		//��������·���Ƿ�������
		FILE* pathFile = fopen(".\\cwRsync\\RemoteServerPath.txt", "r");
		if (pathFile)
		{
			char ch = fgetc(pathFile);
			if (ch != EOF && ch && '\n' || ch && '\r')
			{
				//���������ϴ��ű�
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

	//д��Э��ѡ�����ý��
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
	//û���򴴽��ļ��������޴���
	CWritefile._CreateFile((char *)fileName);
	//����Socket����IP�Ͷ˿�
	CWritefile._WriteProfileString("modbus_server", "ip", (char *)(settingConfig.GetSocketIP()), (char *)fileName);
	CWritefile._WriteProfileString("modbus_server", "port", port, (char *)fileName);
	//������վ��Ϣ����ͻ�ID�Ͱ�ͻ����ͣ���ĿID�����/���վID
	char mixType[2];
	_itoa(settingConfig.GetMixType(), mixType, 10);
	CWritefile._WriteProfileString("mix_station_conf", "mix_stationname", (char *)(settingConfig.GetMixingSectionID()), (char *)fileName);
	CWritefile._WriteProfileString("mix_station_conf", "mix_project_name", (char *)(settingConfig.GetMixingProjectID()), (char *)fileName);
	CWritefile._WriteProfileString("mix_machine_1", "mix_machine_device_id", (char *)(settingConfig.GetMixingStationID()), (char *)fileName);
	CWritefile._WriteProfileString("mix_machine_1", "mix_machine_type", mixType, (char *)fileName);
}



void SCSettingUIDlg::OnBnClickedButtonIptest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	BYTE nf1, nf2, nf3, nf4;
	int tempLen;
	if (SocketIP_Ctrl.IsBlank())
	{
		MessageBox(_T("����д��ЧIP"), _T("��ʾ"), MB_OK);
		return;
	}
	SocketIP_Ctrl.GetAddress(nf1, nf2, nf3, nf4);
	if (nf1 == 0 || (nf1 == 0 && nf2 == 0 && nf3 == 0 && nf4 == 0))
	{
		MessageBox(_T("����д��ЧIP"), _T("��ʾ"), MB_OK);
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
		MessageBox(_T("����д��Ч�˿ں�"), _T("��ʾ"), MB_OK);
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
			MessageBox(L"���ӳɹ���");
		}
		else
		{
			res = socket_GetLastError();
			CString err;
			TCHAR cerr[1024] = { 0 };
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, (void*)FORMAT_MESSAGE_FROM_STRING, res, 0, cerr, 1024, 0);
			err.Format(L"����ʧ�ܣ������� = %d %s", res, cerr);
			MessageBox(err);
		}
		socket_Close(hs);
	}
	else
	{
		MessageBox(L"���׽��־��ʧ�ܣ�");
	}

	socket_Uninit();
}
