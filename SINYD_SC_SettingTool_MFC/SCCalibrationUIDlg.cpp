// SCCalibrationUIDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SINYD_SC_SettingTool_MFC.h"
#include "SCCalibrationUIDlg.h"
#include "afxdialogex.h"
#include "SCCaliNewDlg.h"
#include "SCCaliAreaDlg.h"
#include "SCCaliLoadOldDlg.h"
#include "SCCaliSaveAsDlg.h"
#include "SCCaliRectConversion.h"
#include <wingdi.h>
#include "afxwin.h"
#include "afxcontrolbarutil.h"
#include <Psapi.h>
#include <DbgHelp.h>
#include "RWData.h"
#include "libpng\png.h"
#include <corecrt_io.h>
#include "log.h"

#pragma comment(lib,"Psapi.lib")
#pragma comment(lib, "../Lib/zlib.lib")
#pragma comment(lib, "../Lib/libpng16.lib")


// SCCalibrationUIDlg �Ի���

IMPLEMENT_DYNAMIC(SCCalibrationUIDlg, CDialogEx)

SCCalibrationUIDlg::SCCalibrationUIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SINYD_SC_CALIUI_MFC_DIALOG, pParent)
{
	bGetWindow = false;
	m_hScreenWindowHandle = NULL;
	m_hdcWindow = NULL;
	m_hbmScreen = NULL;
	m_hdcMemDC = NULL;
	pen.CreatePen(0, 1, RGB(255, 0, 0));
	oldpen = NULL;
	localBmpPath = _T("");

	m_parentPos = 0;
}

SCCalibrationUIDlg::~SCCalibrationUIDlg()
{
	ReleaseAllHDC();
	AreaRectMaps.clear();
	vector<AreaRectsMap>(AreaRectMaps).swap(AreaRectMaps);
}

void SCCalibrationUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT3, edtSoftwareName);
	DDX_Control(pDX, IDC_EDIT4, edtSoftwarePath);
	DDX_Control(pDX, IDC_EDIT5, edtWindowName);
	DDX_Control(pDX, IDC_EDIT10, edtWindowClass);
	DDX_Control(pDX, IDC_EDIT9, edtWindowHeight);
	DDX_Control(pDX, IDC_EDIT11, edtWindowWidth);
	//DDX_Control(pDX, IDC_BUTTON1, btnExePath);
	DDX_Control(pDX, IDC_BUTTON5, btnScreenshot);
	DDX_Control(pDX, IDC_BUTTON6, btnWindowHandle);
	DDX_Control(pDX, IDC_BUTTON3, btnSave);
	DDX_Control(pDX, IDC_EDIT12, edtVSoftwareVer);
	DDX_Control(pDX, IDC_Pic, picScreen);
	DDX_Control(pDX, IDC_CHECK1, chkIsAdmixtureTo);
	DDX_Control(pDX, IDC_CHECK2, chkLocalBmp);
	DDX_Control(pDX, IDC_BUTTON1, btnBmp);
	DDX_Control(pDX, IDC_COMBO1, m_ComboBoxCaptureType);
}


BEGIN_MESSAGE_MAP(SCCalibrationUIDlg, CDialogEx)
	ON_COMMAND(ID_MenuBtn_OpenOld, &SCCalibrationUIDlg::OnMenubtnOpenold)
	ON_COMMAND(ID_MenuBtn_New, &SCCalibrationUIDlg::OnMenubtnNew)
	//ON_BN_CLICKED(IDC_BUTTON1, &SCCalibrationUIDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON6, &SCCalibrationUIDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON5, &SCCalibrationUIDlg::OnBnClickedButton5)
	ON_MESSAGE(WM_MYMSG, OnMyMsg)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON3, &SCCalibrationUIDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDCANCEL, &SCCalibrationUIDlg::OnBnClickedCancel)
	ON_COMMAND(ID_MenuBtn_SaveAs, &SCCalibrationUIDlg::OnMenubtnSaveas)
	ON_BN_CLICKED(IDC_CHECK2, &SCCalibrationUIDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_BUTTON1, &SCCalibrationUIDlg::OnBnClickedButton1)
	//ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_BUTTON_BEFORE, &SCCalibrationUIDlg::OnBnClickedButtonBefore)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &SCCalibrationUIDlg::OnBnClickedButtonNext)
	ON_CBN_SELCHANGE(IDC_COMBO1, &SCCalibrationUIDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON_BASEBMP, &SCCalibrationUIDlg::OnBnClickedButtonBasebmp)
	ON_BN_CLICKED(IDC_BUTTON_ParentHWND, &SCCalibrationUIDlg::OnBnClickedButtonParenthwnd)
	ON_BN_CLICKED(IDC_BUTTON_ChildHWND, &SCCalibrationUIDlg::OnBnClickedButtonChildhwnd)
END_MESSAGE_MAP()


// SCCalibrationUIDlg ��Ϣ�������


BOOL SCCalibrationUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//m_hCursor = AfxGetApp()->LoadCursor(IDC_CURSOR2);

	if (!InitCaliConfig())
	{
		MessageBox(_T("��ʼ���궨����ʧ��"), _T("����"), MB_OK);
	}
	//OnBnClickedButton5();

	GetDlgItem(IDC_BUTTON_ParentHWND)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_ChildHWND)->EnableWindow(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void SCCalibrationUIDlg::OnMenubtnOpenold()
{
	// TODO: �ڴ���������������
	SCCaliLoadOldDlg loadCali;
	loadCali.SetPCaliClass(&CaliConfig);
	int nRet = loadCali.DoModal();
	if (nRet == IDOK)
	{
		ShowDlg(&CaliConfig);
		//�������ά���б�
		AreaRectMaps.clear();
		vector<AreaRectsMap>(AreaRectMaps).swap(AreaRectMaps);
		//�����µ������б�
		InitAreaRects();
		//�����µ�ͼƬ�Ϳ�ѡ
		if (chkLocalBmp.GetCheck() && localBmpPath != _T(""))
		{
			//ȡ����չ��
			CString extend = localBmpPath.Right(localBmpPath.GetLength() - localBmpPath.ReverseFind('.') - 1);
			if (extend.CompareNoCase(_T("png")) == 0)
			{
				ShowLocalBmp(localBmpPath, true);
			}
			else if (extend.CompareNoCase(_T("bmp")) == 0)
			{
				ShowLocalBmp(localBmpPath, false);
			}
		} 
		else
		{
			if (m_ComboBoxCaptureType.GetCurSel() != 3)
			{
				ShowPicture();
			}
			else
			{
				CDC* pDC = picScreen.GetDC();
				RECT rt;
				picScreen.GetClientRect(&rt);
				FillRect(pDC->m_hDC, &rt, CreateSolidBrush(RGB(240, 240, 240)));
				//�����Ľ�ͼչʾ��ͼƬ�ؼ�
				bitmapShow.Draw(pDC->m_hDC);
			}
		}
		
		ShowRectangle();
	}
}

void SCCalibrationUIDlg::OnMenubtnNew()
{
	// TODO: �ڴ���������������
	SCCaliNewDlg newCali;
	newCali.SetPCaliClass(&CaliConfig);
	int nRet = newCali.DoModal();
	if (nRet == IDOK)
	{
		ShowDlg(&CaliConfig);
		//���������б�������п�ѡ
		/*for (int i = 0; i < AreaRectMaps.size(); i ++)
		{
			CancelRectangle(CPoint(AreaRectMaps[i].AreaRect.left, AreaRectMaps[i].AreaRect.top), CPoint(AreaRectMaps[i].AreaRect.right, AreaRectMaps[i].AreaRect.bottom));
		}*/
		//�������ά���б�
		AreaRectMaps.clear();
		vector<AreaRectsMap>(AreaRectMaps).swap(AreaRectMaps);
		GetWindowHandle();
		if (chkLocalBmp.GetCheck() && localBmpPath != _T(""))
		{
			//ȡ����չ��
			CString extend = localBmpPath.Right(localBmpPath.GetLength() - localBmpPath.ReverseFind('.') - 1);
			if (extend.CompareNoCase(_T("png")) == 0)
			{
				ShowLocalBmp(localBmpPath, true);
			}
			else if (extend.CompareNoCase(_T("bmp")) == 0)
			{
				ShowLocalBmp(localBmpPath, false);
			}
		}
		else
		{
			if (m_ComboBoxCaptureType.GetCurSel() != 3)
			{
				ShowPicture();
			}
			else
			{
				CDC* pDC = picScreen.GetDC();
				RECT rt;
				picScreen.GetClientRect(&rt);
				FillRect(pDC->m_hDC, &rt, CreateSolidBrush(RGB(240, 240, 240)));
				//�����Ľ�ͼչʾ��ͼƬ�ؼ�
				bitmapShow.Draw(pDC->m_hDC);
			}
		}
	}
}

void SCCalibrationUIDlg::OnMenubtnSaveas()
{
	// TODO: �ڴ���������������
	SCCaliSaveAsDlg saveAs;
	saveAs.SetPCaliClass(&CaliConfig);
	int nRet = saveAs.DoModal();
	if (nRet == IDOK)
	{
		MessageBox(_T("���Ϊ�����궨�ļ��ɹ�"), _T("��ʾ"), MB_OK);
	}
}

bool SCCalibrationUIDlg::InitCaliConfig()
{
	//��ȡ��ǰ�궨xml�����ļ�
	int loadResult = CaliConfig.ReloadConfig();
	if (!loadResult)
	{
		//����ѡ��־��Ϊfalse
		mouseSelect = false;
		//��������¼���ʼ��Ϊ0
		PDown.SetPoint(0, 0);
		PMove.SetPoint(0, 0);
		//�궨��Ϣչʾ��ҳ��
		ShowDlg(&CaliConfig);
		//��������ά���б�
		InitAreaRects();
		//��ȡ���Ӵ��ھ��
		GetWindowHandle();
		//picture�ؼ��޸�type����Ϊbmp
		picScreen.ModifyStyle(0, SS_BITMAP);
		//���ؽ�ͼcheckbox��Ϊ��
		chkLocalBmp.SetCheck(0);
		btnBmp.EnableWindow(false);

		GetDlgItem(IDC_BUTTON_BEFORE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);

		//��˽�������
		m_ComboBoxCaptureType.SetCurSel(CaliConfig.GetBackgroundCaptureType());
		if (m_ComboBoxCaptureType.GetCurSel() == 3)
		{
			GetDlgItem(IDC_BUTTON_BASEBMP)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_BUTTON_BASEBMP)->EnableWindow(FALSE);
		}

		return true;
	}
	return false;
}

void SCCalibrationUIDlg::GetWindowHandle()
{
	CString windowClass;
	CString windowName;
	//���ݼ��Ӵ��������ʹ��������Ҽ��Ӵ��ھ��
	edtWindowClass.GetWindowTextW(windowClass);
	edtWindowName.GetWindowTextW(windowName);
	CWnd *pwnd = FindWindow(windowClass, windowName);
	m_hScreenWindowHandle = pwnd->GetSafeHwnd();
}

void SCCalibrationUIDlg::ShowDlg(const CCalibrationConfig* caliConfig)
{
	//������Ϣչʾ���ؼ���
	USES_CONVERSION;
	edtSoftwareName.SetWindowTextW(CA2T(caliConfig->GetSoftwareName()));
	edtVSoftwareVer.SetWindowTextW(CA2T(caliConfig->GetSoftwareVer()));
	edtSoftwarePath.SetWindowTextW(CA2T(caliConfig->GetSoftwarePath()));
	edtWindowName.SetWindowTextW(CA2T(caliConfig->GetWindowName()));
	edtWindowClass.SetWindowTextW(CA2T(caliConfig->GetWindowClass()));
	CString strWindowHeight;
	strWindowHeight.Format(_T("%d"), caliConfig->GetWindowHeight());
	edtWindowHeight.SetWindowTextW(strWindowHeight);
	CString strWindowWidth;
	strWindowWidth.Format(_T("%d"), caliConfig->GetWindowWidth());
	edtWindowWidth.SetWindowTextW(strWindowWidth);
	int isAdmixtureTo = 0;
	isAdmixtureTo = caliConfig->GetIsAdmixtureToWater();
	chkIsAdmixtureTo.SetCheck(isAdmixtureTo);
	m_ComboBoxCaptureType.SetCurSel(caliConfig->GetBackgroundCaptureType());
}

bool SCCalibrationUIDlg::ReadDlgToEntry(CCalibrationConfig* caliConfig)
{
	//��ȡ������Ϣ�ͱ궨��Ϣ���궨��������
	return true;
}

//void SCCalibrationUIDlg::OnBnClickedButton1()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	CString strFile = _T("");
//	//�򿪲���exe����ĶԻ�����
//	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_NOCHANGEDIR, _T("Describe Files (*.exe)|*.exe|All Files (*.*)|*.*||"), NULL);
//
//	if (dlgFile.DoModal())
//	{
//		//��ȡ����·������������
//		strFile = dlgFile.GetPathName();
//	}
//	//��ֵ����Ӧ�ؼ�
//	edtSoftwarePath.SetWindowTextW(strFile);
//}

void SCCalibrationUIDlg::OnBnClickedButton6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HCURSOR   hcur = GetCursor();
	//����ȫ����깳��
	InstallKbHook(this->GetSafeHwnd());
	SetClassLong(this->GetSafeHwnd(), GCLP_HCURSOR, (LONG)LoadCursor(NULL, IDC_CROSS));
	bGetWindow = true;
}

void SCCalibrationUIDlg::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ȡ���Ӵ��ھ��
	GetWindowHandle();
	//����ͼչʾ��picture�ؼ�
	if (m_ComboBoxCaptureType.GetCurSel() == 3)
	{
		ShowWindow(SW_HIDE);
		GetParent()->ShowWindow(SW_HIDE);
		Sleep(500);
		
		CBitmapEx bitmap;
		GetDesktopScreen(bitmap);

		bitmapShow.SetResampleMode(RM_BICUBIC);
		bitmapShow.Create(bitmap);
		bitmapShow.Scale(50, 50);

		bitmapCali.Create(bitmap);

		GetParent()->ShowWindow(SW_SHOW);
		ShowWindow(SW_SHOW);
	}
	else
	{
		ShowPicture();
	}

	ShowRectangle();
}

LRESULT CALLBACK LowLevelMouseProc(INT nCode, WPARAM wParam, LPARAM lParam)
{
	//��깳�ӵĻص�����
	MSLLHOOKSTRUCT *pkbhs = (MSLLHOOKSTRUCT *)lParam;
	char strMsg[100] = { 0 };

	switch (nCode)
	{
	case HC_ACTION:
	{
		//����ƶ�  
		if (wParam == WM_MOUSEMOVE)
		{
			HCURSOR hCur = LoadCursor(NULL, IDC_CROSS);
			::SetCursor(hCur);
			SetSystemCursor(hCur, (DWORD)IDC_CROSS);
		}

		//������  
		if (wParam == WM_LBUTTONDOWN)
		{
			if (g_hWnd != NULL)
			{
				//���ӵ�������������Ϣ���Լ�
				::SendMessage(g_hWnd, WM_MYMSG, wParam, lParam);
			}
		}
	}
	default:
		break;
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

BOOL SCCalibrationUIDlg::InstallKbHook(HWND hWnd)
{
	//ȫ����깳���Ѵ��ڣ������ͷŹ���
	if (hhookMs)
		UninstallKbHook();

	g_hWnd = hWnd;
	//����ȫ����깳��
	hhookMs = SetWindowsHookEx(WH_MOUSE_LL,
		(HOOKPROC)LowLevelMouseProc, AfxGetApp()->m_hInstance, NULL);
	//�����Ƿ����óɹ�
	return (hhookMs != NULL);
}

BOOL SCCalibrationUIDlg::UninstallKbHook()
{
	//�ͷ���깳��
	BOOL fOk = FALSE;
	if (hhookMs) {
		fOk = UnhookWindowsHookEx(hhookMs);
		hhookMs = NULL;
	}
	//�����Ƿ��ͷųɹ�
	return(fOk);
}

LRESULT SCCalibrationUIDlg::OnMyMsg(WPARAM wParam, LPARAM lParam)
{
	//������Ϣ������д
	if (bGetWindow)
	{
		//���λ��
		PMSLLHOOKSTRUCT mouseHookStruct = (PMSLLHOOKSTRUCT)lParam;
		//������ڴ��ھ��
		m_hScreenWindowHandle = ::WindowFromPoint(mouseHookStruct->pt);
		CString path = _T("");
		GetExePathFromHWND(m_hScreenWindowHandle, path);
		edtSoftwarePath.SetWindowTextW(path);


		HWND parentHwnd = m_hScreenWindowHandle;
		vector<HWND> parents;
		parents.push_back(m_hScreenWindowHandle);
		do
		{
			parentHwnd = ::GetParent(parentHwnd);
			if (parentHwnd != NULL)
			{
				parents.push_back(parentHwnd);
			}
		} while (parentHwnd != NULL);
		if (parents.size() > 1)
		{
			m_hScreenWindowHandle = parents[parents.size() - 1];
			m_parentPos = parents.size() - 1;

			GetDlgItem(IDC_BUTTON_ParentHWND)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_ChildHWND)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_BUTTON_ParentHWND)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_ChildHWND)->EnableWindow(FALSE);
		}

		m_parents.swap(parents);


		//��ô�����
		TCHAR temp[MAX_PATH];
		::GetWindowText(m_hScreenWindowHandle, temp, MAX_PATH);
		edtWindowName.SetWindowTextW(temp);
		//��ô�������
		::GetClassName(m_hScreenWindowHandle, temp, MAX_PATH);
		edtWindowClass.SetWindowTextW(temp);
		//��ô��ڴ�С
		RECT rc;
		if (m_ComboBoxCaptureType.GetCurSel() != 3)
		{
			::GetWindowRect(m_hScreenWindowHandle, &rc);
		}
		else
		{
			::GetWindowRect(GetDesktopWindow()->GetSafeHwnd(), &rc);
		}
		
		if (&rc != NULL)
		{
			CString strWindowHeight;
			strWindowHeight.Format(_T("%d"), rc.bottom - rc.top);
			edtWindowHeight.SetWindowTextW(strWindowHeight);
			CString strWindowWidth;
			strWindowWidth.Format(_T("%d"), rc.right - rc.left);
			edtWindowWidth.SetWindowTextW(strWindowWidth);
		}
		//�ͷ���깳��
		SetClassLong(this->GetSafeHwnd(), GCLP_HCURSOR, (LONG)LoadCursor(NULL, IDC_ARROW));
		UninstallKbHook();

		Invalidate();
		UpdateWindow();
	}
	return 0;
}

void SCCalibrationUIDlg::ReleaseAllHDC()
{
	if (m_hbmScreen)
	{
		DeleteObject(m_hbmScreen);
		m_hbmScreen = NULL;
	}
	if (m_hdcMemDC)
	{
		DeleteObject(m_hdcMemDC);
		m_hdcMemDC = NULL;
	}
	if (m_hdcWindow)
	{
		::ReleaseDC(m_hScreenWindowHandle, m_hdcWindow);
		m_hdcWindow = NULL;
	}
}

void SCCalibrationUIDlg::RefreshAllHDC(HWND hWnd)
{
	ReleaseAllHDC();

	m_hdcWindow = ::GetWindowDC(hWnd);
	m_hdcMemDC = CreateCompatibleDC(m_hdcWindow);

	RECT rc;
	::GetWindowRect(m_hScreenWindowHandle, &rc);
	m_hbmScreen = CreateCompatibleBitmap(m_hdcWindow, rc.right - rc.left, rc.bottom - rc.top);
}

WINDOWSTATUS SCCalibrationUIDlg::GetWindowStatus()
{
	WINDOWSTATUS status = WINDOWSTATUS::STATUS_UNKNOWN;
	if (m_hScreenWindowHandle != NULL)
	{
		//������Ӵ��ھ����Ϊ�գ���ȡ���Ӵ��ڵ�ǰ״̬
		if (::IsIconic(m_hScreenWindowHandle))
		{
			//��С��
			status = WINDOWSTATUS::STATUS_MINIMIZE;
		}
		else
		{
			if (m_hScreenWindowHandle == ::GetForegroundWindow())
			{
				//ǰ̨��ʾ
				status = WINDOWSTATUS::STATUS_FOREGROUND;
			}
			else
			{
				//��̨��ʾ
				status = WINDOWSTATUS::STATUS_BACKGROUND;
			}
		}
	}
	return status;
}

bool SCCalibrationUIDlg::CaptureBackgroudWindow()
{
	bool bRes = false;

	RefreshAllHDC(m_hScreenWindowHandle);

	if (m_hbmScreen && m_hdcMemDC)
	{
		// Select the compatible bitmap into the compatible memory DC.
		HGDIOBJ obj = SelectObject(m_hdcMemDC, m_hbmScreen);

		// Bit block transfer into our compatible memory DC.
		if (::PrintWindow(m_hScreenWindowHandle, m_hdcMemDC, 0))
		{
			bRes = true;
		}

		SelectObject(m_hdcMemDC, obj);
	}

	return bRes;
}

int EnableDebugPriv(const TCHAR * name)
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tp;
	LUID luid;
	//�򿪽������ƻ�
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	//��ý��̱���ΨһID
	LookupPrivilegeValue(NULL, name, &luid);

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	tp.Privileges[0].Luid = luid;
	//����Ȩ��
	AdjustTokenPrivileges(hToken, 0, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
	return 0;
}

//*****************************************************************************************************************************

BOOL InjectDll(const char *DllFullPath, const DWORD dwRemoteProcessId)
{
	HANDLE hRemoteProcess;
	EnableDebugPriv(SE_DEBUG_NAME);
	//��Զ���߳�
	hRemoteProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwRemoteProcessId);

	char *pszLibFileRemote;

	//ʹ��VirtualAllocEx������Զ�̽��̵��ڴ��ַ�ռ����DLL�ļ����ռ�
	pszLibFileRemote = (char *)VirtualAllocEx(hRemoteProcess, NULL, lstrlenA(DllFullPath) + 1, MEM_COMMIT, PAGE_READWRITE);


	//ʹ��WriteProcessMemory������DLL��·����д�뵽Զ�̽��̵��ڴ�ռ�
	BOOL bWrite = WriteProcessMemory(hRemoteProcess, pszLibFileRemote, (void *)DllFullPath, lstrlenA(DllFullPath) + 1, NULL);
	if (!bWrite)
	{
		return FALSE;
	}

	//����LoadLibraryA����ڵ�ַ
	PTHREAD_START_ROUTINE pfnStartAddr = (PTHREAD_START_ROUTINE)
		GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryA");
	//(����GetModuleHandle������GetProcAddress����)

	//����Զ���߳�LoadLibraryA��ͨ��Զ���̵߳��ô����µ��߳�
	HANDLE hRemoteThread;
	if ((hRemoteThread = CreateRemoteThread(hRemoteProcess, NULL, 0, pfnStartAddr, pszLibFileRemote, 0, NULL)) == NULL)
	{
		return FALSE;
	}

	// �ͷž��

	CloseHandle(hRemoteProcess);
	CloseHandle(hRemoteThread);

	return TRUE;
}


#define DLL_CAPTURE_EVENT_NAME  TEXT("Global\\1E485DD3-7A31-418D-981A-927A7535D56F}")
#define DLL_CAPTURE_DONE_EVENT_NAME  TEXT("Global\\1E485DD3-7A31-418D-981A-927A7535D56G}")
#define DLL_CAPTURE_FILEMAPPING_NAME _T("DLL_CAPTURE_FILEMAPPING")

bool SCCalibrationUIDlg::CaptureBackgroudWindowByHook(CBitmapEx& bitmap)
{
	bool bRes = false;

	if (m_hScreenWindowHandle)
	{
		HANDLE h_CaptureEvent = ::OpenEvent(EVENT_ALL_ACCESS, FALSE, DLL_CAPTURE_EVENT_NAME);
		HANDLE h_CaptureDoneEvent = ::OpenEvent(EVENT_ALL_ACCESS, FALSE, DLL_CAPTURE_DONE_EVENT_NAME);

		if (h_CaptureEvent == NULL || h_CaptureDoneEvent == NULL)
		{
			char InjectfileFullPath[MAX_PATH] = "";
			::GetModuleFileNameA(::GetModuleHandleA(NULL), InjectfileFullPath, MAX_PATH);

			std::string injectDllPath(InjectfileFullPath);
			injectDllPath = injectDllPath.erase(injectDllPath.find_last_of('\\'));
			char cpathend = *(injectDllPath.crbegin());
			if (cpathend != '\\')
			{
				injectDllPath = injectDllPath + '\\';
			}
			injectDllPath = injectDllPath + "SINYD_SC_RemoteScreenCapture.dll";

			DWORD idProcess;
			GetWindowThreadProcessId(m_hScreenWindowHandle, &idProcess);
			InjectDll(injectDllPath.c_str(), idProcess);

			Sleep(50);
			h_CaptureEvent = ::OpenEvent(EVENT_ALL_ACCESS, FALSE, DLL_CAPTURE_EVENT_NAME);
			h_CaptureDoneEvent = ::OpenEvent(EVENT_ALL_ACCESS, FALSE, DLL_CAPTURE_DONE_EVENT_NAME);
		}

		if (h_CaptureEvent != NULL && h_CaptureEvent != INVALID_HANDLE_VALUE &&
			h_CaptureDoneEvent != NULL && h_CaptureDoneEvent != INVALID_HANDLE_VALUE)
		{
			SetEvent(h_CaptureEvent);

			WaitForSingleObject(h_CaptureDoneEvent, 100);

			// 		bitmap.Load(_T(".\\ConfigFile_RunTime\\currentScreen.bmp"));
			// 		::DeleteFile(_T(".\\ConfigFile_RunTime\\currentScreen.bmp"));

			HANDLE hFileMapping = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, 0, DLL_CAPTURE_FILEMAPPING_NAME);
			if (hFileMapping)
			{
				LPBYTE pBuffer = (LPBYTE)MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
				bitmap.Load(pBuffer);
				if (bitmap.IsValid())
				{
					bRes = true;
				}
				::UnmapViewOfFile(pBuffer);
			}
		}
	}

	return bRes;
}

bool SCCalibrationUIDlg::GetDesktopScreen(CBitmapEx& bitmap)
{
	bool bRes = false;

	HWND hWndDesk = ::GetDesktopWindow();
	HDC hdcDesk = ::GetWindowDC(hWndDesk);
	HDC hdcMemDC = CreateCompatibleDC(hdcDesk);
	int width, height;
	RECT rect;
	::GetWindowRect(hWndDesk, &rect);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;
	HBITMAP hbmScreen = CreateCompatibleBitmap(hdcDesk, width, height);

	// Select the compatible bitmap into the compatible memory DC.
	HGDIOBJ obj = SelectObject(hdcMemDC, hbmScreen);

	// Bit block transfer into our compatible memory DC.
	if (BitBlt(hdcMemDC,
		0, 0,
		width, height,
		hdcDesk,
		0, 0,
		SRCCOPY))
	{
		bitmap.Load(hbmScreen);
		bRes = true;
	}

	SelectObject(hdcMemDC, obj);

	if (hbmScreen)
	{
		DeleteObject(hbmScreen);
		hbmScreen = NULL;
	}
	if (hdcMemDC)
	{
		DeleteObject(hdcMemDC);
		hdcMemDC = NULL;
	}
	if (hdcDesk)
	{
		::ReleaseDC(hWndDesk, hdcDesk);
		hdcDesk = NULL;
	}


	return bRes;
}

HBITMAP SCCalibrationUIDlg::GetScreenShot()
{
	HBITMAP hbmRes = NULL;
	//��ȡ��ǰ������ʾ״̬
	WINDOWSTATUS status = GetWindowStatus();
	//��Ժ�̨��ʾ��״̬���д���
	if (status == WINDOWSTATUS::STATUS_BACKGROUND)
	{
		if (m_ComboBoxCaptureType.GetCurSel() == 0)
		{
			CBitmapEx bitmap;
			if (CaptureBackgroudWindowByHook(bitmap))
			{
				bitmap.Save(hbmRes);
				//hbmRes = m_hbmScreen;
			}
		}
		else
		{
			if (CaptureBackgroudWindow())
			{
				hbmRes = m_hbmScreen;
			}
		}
	}
	else
	{
		hbmRes = NULL;
	}

	return hbmRes;
}

void SCCalibrationUIDlg::ShowPicture()
{
	if (m_hScreenWindowHandle)
	{
		//���ھ����Ϊ�գ����Ӵ��ڽ�ͼ
		HBITMAP hBitmap = GetScreenShot();
		if (hBitmap)
		{
			//ʹ��CBitmapEx�ദ���ͼ������ͼ��С��50%
			CDC* pDC = picScreen.GetDC();
			bitmapShow.SetResampleMode(RM_BICUBIC);
			bitmapShow.Load(hBitmap);
			bitmapShow.Scale(50, 50);
			//��ȡpicture�ؼ���С�������ؼ���ɫ��ȫ���ûң�RGB(240,240,240)
			RECT rt;
			picScreen.GetClientRect(&rt);
			FillRect(pDC->m_hDC, &rt, CreateSolidBrush(RGB(240, 240, 240)));
			//�����Ľ�ͼչʾ��ͼƬ�ؼ�
			bitmapShow.Draw(pDC->m_hDC);
			ReleaseDC(pDC);						// add by yaodi for DC release

												//���ر궨��ͼ
			bitmapCali.Load(hBitmap);

			DeleteObject(hBitmap);
			hBitmap = NULL;
		}
		else
		{
			//���ڽ�ͼΪ�գ���ȡpicture�ؼ���С�������ؼ���ɫȫ���ûң�RGB(240,240,240)
			RECT rt;
			picScreen.GetClientRect(&rt);
			CDC* pDC = picScreen.GetDC();
			FillRect(pDC->m_hDC, &rt, CreateSolidBrush(RGB(240, 240, 240)));
			ReleaseDC(pDC);
		}
	}
	else
	{
		//���ھ��Ϊ�գ���ȡpicture�ؼ���С�������ؼ���ɫȫ���ûң�RGB(240,240,240)
		RECT rt;
		picScreen.GetClientRect(&rt);
		CDC* pDC = picScreen.GetDC();
		FillRect(pDC->m_hDC, &rt, CreateSolidBrush(RGB(240, 240, 240)));
		ReleaseDC(pDC);
	}
}

void SCCalibrationUIDlg::read_png_to_bmp(const TCHAR* fn, CBitmapEx& bitmap)
{
	png_structp png_ptr;
	png_infop info_ptr;
	png_uint_32 width, height;
	int bit_depth, color_type;
	int xbit_depth, xcolor_type, xchannels;
	const char *errmsg;
	FILE *fp;

	fp = _tfopen(fn, _T("rb"));
	if (fp == NULL) goto error_abort;

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		goto error_abort;
	}
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		goto error_abort;
	}
	if (setjmp(png_jmpbuf(png_ptr))) {
		/* If we get here, we had a problem writing the file */
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		goto error_abort;
	}
	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 0);

	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);
	width = png_get_image_width(png_ptr, info_ptr);
	height = png_get_image_height(png_ptr, info_ptr);
	bitmap.Create(width, height);

	const int pixelBytesCount = 3;
	png_bytep* row_pointers = png_get_rows(png_ptr, info_ptr);
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < (pixelBytesCount * width); col += pixelBytesCount)
		{
			bitmap.SetPixel(col / pixelBytesCount, row, _RGB(row_pointers[row][col], row_pointers[row][col + 1], row_pointers[row][col + 2]));
		}
	}

	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

	if (fp) fclose(fp);
	return;

error_abort:				/* error */
	if (fp) fclose(fp);
	return;
}

void SCCalibrationUIDlg::ShowLocalBmp(CString localPath, bool isPNG)
{
	//���ر궨��ͼ
	bitmapCali.Clear();
	if (isPNG)
	{
		read_png_to_bmp(localPath.GetString(), bitmapCali);
	}
	else
	{
		bitmapCali.Load(localPath.GetBuffer());
	}

#if 0
	if (AreaRectMaps.size() > 0)
	{
		CSCDigitOCR ocr;
		for (int i = 0; i < AreaRectMaps.size(); i++)
		{
			long x = AreaRectMaps[i].AreaRect.left;
			long y = AreaRectMaps[i].AreaRect.top;
			long w = AreaRectMaps[i].AreaRect.right - AreaRectMaps[i].AreaRect.left;
			long h = AreaRectMaps[i].AreaRect.bottom - AreaRectMaps[i].AreaRect.top;

			if ( w <= 20)
			{
				continue;
			}

			long BufLength = h*w;
			unsigned long* ulBuf = new unsigned long[BufLength];
			long rWidth = 0, rHeight = 0;
			bitmapCali.GetPixels(x, y, w, h, ulBuf, &BufLength, &rWidth, &rHeight);
			if (BufLength > 0)
			{
				list<DIGIT> tmpDigit;
				
				const vector<Parameter*>* pParameters = CaliConfig.GetMonitorArea(AreaRectMaps[i].areaID)->GetParameters();
				DWORD rgb;
				int equal;
				for (int i = 0; i < pParameters->size(); i++)
				{
					CString strA = CA2T((*pParameters)[i]->GetParameterName());
					if (strA == _T(SC_LOADCELL_FONT_COLOR_PARAMNAME))
					{
						//���������Ϊ��ɫRGBֵ
						rgb = atoll((*pParameters)[i]->GetParameterValue());
					}
					else if (strA == _T(SC_COMMON_COLOR_THRESHOLD_PARAMNAME))
					{
						equal = atoi((*pParameters)[i]->GetParameterValue());
						ocr.SetEqulaTolerance(equal);
					}
				}
				SC_OCR_ERROR err = ocr.RecognizeDigits(ulBuf, rWidth, rHeight, rgb, tmpDigit);
				if (err == SC_OCR_SUCCESS || err == SC_OCR_EXISTS_UNRECOGNIZE)
				{
					if (tmpDigit.size() > 0)
					{
						string digit = "";
						list<DIGIT>::iterator itB = tmpDigit.begin();
						list<DIGIT>::iterator itE = tmpDigit.end();
						for (list<DIGIT>::iterator it = itB; it != itE; ++it)
						{

							if (it->digit == "x")
							{
#if SC_PLUGIN_NO_UNDEFINECHAR_RESULT
								digit = digit + '0';
#else
								char cID[10] = { 0 };
								_itoa_s(it->model.id, cID, 10);
								digit = digit + '[' + cID + ']';
#endif
							}
							else
							{
								digit = digit + it->digit;
							}

						}
						char ID[10] = { 0 };
						_itoa_s(AreaRectMaps[i].areaID, ID, 10);
						digit = "{" + string(ID) + "}---" + digit;
						digit = digit + "\n";
						OutputDebugStringA(digit.c_str());
					}
				}
			}
			if (ulBuf)
			{
				delete[] ulBuf;
				ulBuf = nullptr;
			}
		}
	}
#endif

	//ʹ��CBitmapEx�ദ���ͼ������ͼ��С��50%
	CDC* pDC = picScreen.GetDC();
	bitmapShow.SetResampleMode(RM_BICUBIC);
	bitmapShow.Clear();
	if (isPNG)
	{
		read_png_to_bmp(localPath.GetString(), bitmapShow);
	}
	else
	{
		bitmapShow.Load(localPath.GetBuffer());
	}
	
	//bitmapShow.Load(localPath.GetBuffer(0));
	bitmapShow.Scale(50, 50);
	//��ȡpicture�ؼ���С�������ؼ���ɫ��ȫ���ûң�RGB(240,240,240)
	RECT rt;
	picScreen.GetClientRect(&rt);
	FillRect(pDC->m_hDC, &rt, CreateSolidBrush(RGB(240, 240, 240)));
	//�����Ľ�ͼչʾ��ͼƬ�ؼ�
	bitmapShow.Draw(pDC->m_hDC);
	ReleaseDC(pDC);						// add by yaodi for DC release

	//���ر궨��ͼ
	//bitmapCali.Load(localPath.GetBuffer(0));
}

void SCCalibrationUIDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CDialogEx::OnPaint();
	CDialogEx::UpdateWindow();
	// TODO: �ڴ˴������Ϣ����������
	if (chkLocalBmp.GetCheck() && localBmpPath != _T(""))
	{
		//ȡ����չ��
		CString extend = localBmpPath.Right(localBmpPath.GetLength() - localBmpPath.ReverseFind('.') - 1);
		if (extend.CompareNoCase(_T("png")) == 0)
		{
			ShowLocalBmp(localBmpPath, true);
		}
		else if (extend.CompareNoCase(_T("bmp")) == 0)
		{
			ShowLocalBmp(localBmpPath, false);
		}
	}
	else
	{
		if (m_ComboBoxCaptureType.GetCurSel() != 3)
		{
			ShowPicture();
		}
		else
		{
			CDC* pDC = picScreen.GetDC();
			RECT rt;
			picScreen.GetClientRect(&rt);
			FillRect(pDC->m_hDC, &rt, CreateSolidBrush(RGB(240, 240, 240)));
			//�����Ľ�ͼչʾ��ͼƬ�ؼ�
			bitmapShow.Draw(pDC->m_hDC);
		}
		
	}
	ShowRectangle();
    // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}


// void SCCalibrationUIDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
// {
// 	CDialogEx::OnActivate(nState, pWndOther, bMinimized);
// 
// // 	if ( nState == WA_ACTIVE )
// // 	{
// // 		if (chkLocalBmp.GetCheck() && localBmpPath != _T(""))
// // 		{
// // 			//ȡ����չ��
// // 			CString extend = localBmpPath.Right(localBmpPath.GetLength() - localBmpPath.ReverseFind('.') - 1);
// // 			if (extend.CompareNoCase(_T("png")) == 0)
// // 			{
// // 				ShowLocalBmp(localBmpPath, true);
// // 			}
// // 			else if (extend.CompareNoCase(_T("bmp")) == 0)
// // 			{
// // 				ShowLocalBmp(localBmpPath, false);
// // 			}
// // 		}
// // 		else
// // 		{
// // 			ShowPicture();
// // 		}
// 		//ShowRectangle();
// /*	}*/
// }

void SCCalibrationUIDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	RECT rtPic;
	picScreen.GetWindowRect(&rtPic);
	//::ClientToScreen(picScreen.m_hWnd, &point);
	ScreenToClient(&rtPic);
	if (PtInRect(&rtPic, point))
	{
		mouseSelect = true;
		PDown = PMove = point;
	}
	else
	{
		PDown = PMove = point;
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

void SCCalibrationUIDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	RECT rtPic;
	picScreen.GetWindowRect(&rtPic);
	ScreenToClient(&rtPic);
	RECT curRect;
	SCCaliRectConversion rectConv;
	curRect = rectConv.MakeRect(PDown, point);
	if (curRect.right - rtPic.left > bitmapShow.GetWidth() || curRect.bottom - rtPic.top > bitmapShow.GetHeight())
	{
		mouseSelect = false;
		CancelRectangle(PDown, PMove);
		CDialogEx::OnLButtonUp(nFlags, point);
		return;
	}

	if (AreaRectMaps.size() > 0)
	{
		//�������б궨����
		vector<AreaRectsMap>::iterator it;
		for (it = AreaRectMaps.begin();it != AreaRectMaps.end();)
		//for (int i = 0; i < AreaRects.size(); i++)
		{
			//������������picture�ؼ��ϵ����귶Χ
			RECT rtArea;
			rtArea.left = (*it).AreaRect.left / 2 + rtPic.left;// 2 * (PDown.x - rtPic.left);
			rtArea.top = (*it).AreaRect.top / 2 + rtPic.top;//2 * (PDown.y - rtPic.top);
			rtArea.right = (*it).AreaRect.right / 2 + rtPic.left;//2 * (PMove.x - rtPic.left);
			rtArea.bottom = (*it).AreaRect.bottom / 2 + rtPic.top;//2 * (PMove.y - rtPic.top);
			//���쵱ǰ��껭�����
			RECT curRect, tempRect;
			curRect = rectConv.MakeRect(PDown, point);
			//�ж����������Ƿ��ཻ���ཻ��Ϊ���޸��������򣬻���ֱ�ӵ�����о���������
			if (IntersectRect(&tempRect, &rtArea, &curRect) || PtInRect(&rtArea, point) || PtInRect(&rtArea, PDown))
			//if (PtInRect(&rtArea, point) || PtInRect(&rtArea, PDown) || PtInRect(&curRect, CPoint(rtArea.left, rtArea.top)))
			{
				//��ʱ��¼�Ƿ�������ѡ״̬
				bool tempMouseSelect = mouseSelect;
				//���ѡȡ��־��Ϊfalse
				mouseSelect = false;
				//�����궨�������
				SCCaliAreaDlg area;
				//���궨��Ϣ�ࡢ��ͼ��Ļbmp����Խ�ͼ���������ꡢ�Ŵ�ϵ������궨�������
				area.SetPCaliClass(&CaliConfig);
				//area.SetHopperList(&HopperList);
				area.SetAreaInfo((*it).areaID);
				area.SetRectArea((*it).AreaRect);
				//����궨��ѡͼ�Ŵ�ϵ��
				float coef = rectConv.CalcAreaCoef(rectConv.areaPicSide, rectConv.areaPicSide, (*it).AreaRect.bottom - (*it).AreaRect.top, (*it).AreaRect.right - (*it).AreaRect.left);
				area.SetCalcCoef(coef);
				area.SetCBitmap(bitmapCali);
				int nRet = area.DoModal();
				//֮ǰ��������ѡ״̬��ȡ����ѡ��ͼ
				if (tempMouseSelect)
					CancelRectangle(PDown, PMove);
				if (nRet == IDOK)
				{
					//����ɹ�
					//ɾ����Ӧ����Ŀ�ѡ��ͼ
					CancelRectangle(CPoint(rtArea.left, rtArea.top), CPoint(rtArea.right, rtArea.bottom));
					//�����µ���ʵ����
					area.GetRectArea((*it).AreaRect);
					//���¿ؼ���������
					rtArea.left = (*it).AreaRect.left / 2 + rtPic.left;// 2 * (PDown.x - rtPic.left);
					rtArea.top = (*it).AreaRect.top / 2 + rtPic.top;//2 * (PDown.y - rtPic.top);
					rtArea.right = (*it).AreaRect.right / 2 + rtPic.left;//2 * (PMove.x - rtPic.left);
					rtArea.bottom = (*it).AreaRect.bottom / 2 + rtPic.top;//2 * (PMove.y - rtPic.top);
					//�����򻭿�ѡ
					CClientDC dcMouse(this);//��ǰDC
					int oldmode = dcMouse.SetROP2(R2_NOTXORPEN);//���û�ͼģʽ
					oldpen = dcMouse.SelectObject(&pen);//ѡ�񻭱ʵ���ǰDC
					dcMouse.Rectangle(CRect(rtArea));
					ReleaseDC(&dcMouse);
				}
				else if (nRet == AREA_DELETE)
				{
					//ɾ���ɹ�
					//ɾ����Ӧ��ɾ������Ŀ�ѡ��ͼ
					CancelRectangle(CPoint(rtArea.left, rtArea.top), CPoint(rtArea.right, rtArea.bottom));
					//�ͷ���ɾ������Ŀ�ѡ��¼
					AreaRectMaps.erase(it);
				}
				else
				{
					//ȡ��
				}
				break;
			}
			else
			{
				++it;
			}
		}
	}
	if (mouseSelect)
	{
		//���ѡȡ��־��Ϊfalse
		mouseSelect = false;
		if (fabs(point.x - PDown.x) > 2 && fabs(point.y - PDown.y) > 1/*fabs(point.x - PDown.x) > 10 && fabs(point.y - PDown.y) > 8*/)	//ȡ�����
		{
			//�����궨�������
			SCCaliAreaDlg area;
			//���궨��Ϣ�ࡢ��ͼ��Ļbmp����Խ�ͼ���������괫��궨�������
			area.SetPCaliClass(&CaliConfig);
			//area.SetHopperList(&HopperList);
			//������Խ�ͼ����������
			RECT rtArea = rectConv.MakeRect(CPoint(2 * (PDown.x - rtPic.left), 2 * (PDown.y - rtPic.top)), CPoint(2 * (PMove.x - rtPic.left), 2 * (PMove.y - rtPic.top)));
			area.SetRectArea(rtArea);
			//����궨��ѡͼ�Ŵ�ϵ��
			float coef = rectConv.CalcAreaCoef(rectConv.areaPicSide, rectConv.areaPicSide, rtArea.bottom - rtArea.top, rtArea.right - rtArea.left);
			area.SetCalcCoef(coef);
			area.SetCBitmap(bitmapCali);
			int nRet = area.DoModal();
			if (nRet == IDOK)
			{
				//����ɹ�
				//ɾ����Ӧ����Ŀ�ѡ��ͼ
				CancelRectangle(PDown, PMove);
				//�����µ���ʵ����
				area.GetRectArea(rtArea);
				//���¿ؼ���������
				RECT rtShow;
				rtShow.left = rtArea.left / 2 + rtPic.left;// 2 * (PDown.x - rtPic.left);
				rtShow.top = rtArea.top / 2 + rtPic.top;//2 * (PDown.y - rtPic.top);
				rtShow.right = rtArea.right / 2 + rtPic.left;//2 * (PMove.x - rtPic.left);
				rtShow.bottom = rtArea.bottom / 2 + rtPic.top;//2 * (PMove.y - rtPic.top);
				//�����򻭿�ѡ
				CClientDC dcMouse(this);//��ǰDC
				int oldmode = dcMouse.SetROP2(R2_NOTXORPEN);//���û�ͼģʽ
				oldpen = dcMouse.SelectObject(&pen);//ѡ�񻭱ʵ���ǰDC
				dcMouse.Rectangle(CRect(rtShow));
				ReleaseDC(&dcMouse);
				//ȡ�õ�ǰ����MonitorAreaID
				int maxID = 1;
				const vector<MonitorArea *>* areas = CaliConfig.GetMonitorAreas();
				for (int i = 0; i < areas->size(); i++)
				{
					if ((*areas)[i]->GetMonitorAreaID() >= maxID)
						maxID = (*areas)[i]->GetMonitorAreaID();
				}
				AreaRectsMap newMap;
				newMap.areaID = maxID;
				newMap.AreaRect = rtArea;
				AreaRectMaps.push_back(newMap);
			}
			else if (nRet == AREA_DELETE)
			{
				//ɾ���ɹ���ʵ�ʴ˴�Ӧ�����½��궨����ɾ����֧�޷�����
			}
			else
			{
				//ȡ��
				CancelRectangle(PDown, PMove);
			}
		}
		else
		{
			CancelRectangle(PDown, PMove);
		}
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}


void SCCalibrationUIDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (mouseSelect)
	{
		CClientDC dcMouse(this);//��ǰDC
		int oldmode = dcMouse.SetROP2(R2_NOTXORPEN);//���û�ͼģʽ
		oldpen = dcMouse.SelectObject(&pen);//ѡ�񻭱ʵ���ǰDC

		dcMouse.Rectangle(CRect(PDown, PMove));

		PMove = point;

		dcMouse.Rectangle(CRect(PDown, PMove));

		dcMouse.SelectObject(oldpen);
		dcMouse.SetROP2(oldmode);
		ReleaseDC(&dcMouse);
	}
	CDialogEx::OnMouseMove(nFlags, point);
}

void SCCalibrationUIDlg::CancelRectangle(CPoint POrigin, CPoint PEnd)
{
	//ȡ�����λ���
	CClientDC dcMouse(this);//��ǰDC
	int oldmode = dcMouse.SetROP2(R2_NOTXORPEN);//���û�ͼģʽ
	oldpen = dcMouse.SelectObject(&pen);//ѡ�񻭱ʵ���ǰDC

	dcMouse.Rectangle(CRect(POrigin, PEnd));

	dcMouse.SelectObject(oldpen);
	dcMouse.SetROP2(oldmode);
	ReleaseDC(&dcMouse);
}

//float SCCalibrationUIDlg::CalcAreaCoef(int height, int width)
//{
//	float hCoef = (float)areaPicSide / (float)height;
//	float wCoef = (float)areaPicSide / (float)width;
//
//	//����10֮�����5
//	hCoef = hCoef * 10 / 5;
//	wCoef = wCoef * 10 / 5;
//	//�������뱣������λ��
//	int ihCoef = floor(hCoef * 10 + 0.5) / 10;
//	int iwCoef = floor(wCoef * 10 + 0.5) / 10;
//	//�ٴγ���5������ʼֵ��10��ֵ��Լ��5��֮�����10���õ����ս��Ϊ0.5�ı���
//	hCoef = ihCoef * 5.0 / 10.0;
//	wCoef = iwCoef * 5.0 / 10.0;
//	//���շ��ر���С���Ǹ�ϵ��
//	return hCoef >= wCoef ? wCoef : hCoef;
//}

void SCCalibrationUIDlg::InitAreaRects()
{
	int areaNum = CaliConfig.GetMonitorAreas()->size();
	if (areaNum > 0)
	{
		const vector<MonitorArea*>* pAreas = CaliConfig.GetMonitorAreas();
		for (int i = 0; i < areaNum; i++)
		{
			AreaRectsMap areaMap;
			if ((*pAreas)[i]->GetMonitorAreaType() == 1)
			{
				//����������ʾ���ţ�������������Ϊ�Է��ű궨��Ϊ���ĵ�20*20����
				areaMap.AreaRect.left = (*pAreas)[i]->GetMonitorAreaX() - 10;
				areaMap.AreaRect.top = (*pAreas)[i]->GetMonitorAreaY() - 10;
				areaMap.AreaRect.right = (*pAreas)[i]->GetMonitorAreaX() + 10;
				areaMap.AreaRect.bottom = (*pAreas)[i]->GetMonitorAreaY() + 10;
			}
			else
			{
				//�Ƿ��ţ���������Ϊ��ʵ�궨����
				areaMap.AreaRect.left = (*pAreas)[i]->GetMonitorAreaX();
				areaMap.AreaRect.top = (*pAreas)[i]->GetMonitorAreaY();
				areaMap.AreaRect.right = (*pAreas)[i]->GetMonitorAreaX() + (*pAreas)[i]->GetMonitorAreaWidth();
				areaMap.AreaRect.bottom = (*pAreas)[i]->GetMonitorAreaY() + (*pAreas)[i]->GetMonitorAreaHeight();
			}
			areaMap.areaID = (*pAreas)[i]->GetMonitorAreaID();
			AreaRectMaps.push_back(areaMap);
		}
	}
}

void SCCalibrationUIDlg::OnBnClickedButton3()
{
	if (m_ComboBoxCaptureType.GetCurSel() == 3)
	{
		CBitmapEx base;
		base.Load(_T(ConfigFile_Folder "/" "ScaleBase.bmp"));
		if (!base.IsValid())
		{
			MessageBox(L"ѡ��ץȡ����Ľ�ͼ��ʽʱ���뱣��һ�������Ļ�׼��ͼ");
			return;
		}
	}
	

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	USES_CONVERSION;
	int tempLen;
	CString strTemp;
	//��ȡ��������
	edtSoftwareName.GetWindowTextW(strTemp);
	if (strTemp == "")
	{
		MessageBox(_T("�������Ʋ���Ϊ��"), _T("��ʾ"), MB_OK);
		return;
	}
	tempLen = strlen(CT2A(strTemp));
	CaliConfig.SetSoftwareName(CT2A(strTemp), tempLen);
	//��ȡ����汾
	edtVSoftwareVer.GetWindowTextW(strTemp);
	if (strTemp == "")
	{
		MessageBox(_T("����汾����Ϊ��"), _T("��ʾ"), MB_OK);
		return;
	}
	tempLen = strlen(CT2A(strTemp));
	CaliConfig.SetSoftwareVer(CT2A(strTemp), tempLen);
	//��ȡ����·��
	edtSoftwarePath.GetWindowTextW(strTemp);
	if (strTemp != "")
	{
// 		MessageBox(_T("����·������Ϊ��"), _T("��ʾ"), MB_OK);
// 		return;
		tempLen = strlen(CT2A(strTemp));
		CaliConfig.SetSoftwarePath(CT2A(strTemp), tempLen);
	}

	//��ȡ��������
	edtWindowName.GetWindowTextW(strTemp);
	if (strTemp != "")
	{
// 		MessageBox(_T("�������Ʋ���Ϊ��"), _T("��ʾ"), MB_OK);
// 		return;
		tempLen = strlen(CT2A(strTemp));
		CaliConfig.SetWindowName(CT2A(strTemp), tempLen);
	}

	//��ȡ��������
	edtWindowClass.GetWindowTextW(strTemp);
	if (strTemp != "")
	{
// 		MessageBox(_T("������������Ϊ��"), _T("��ʾ"), MB_OK);
// 		return;
		tempLen = strlen(CT2A(strTemp));
		CaliConfig.SetWindowClass(CT2A(strTemp), tempLen);
	}

	//��ȡ���ڸ߶�
	edtWindowHeight.GetWindowTextW(strTemp);
	if (_ttoi(strTemp) != 0)
	{
// 		MessageBox(_T("���ȡ��Ч���ڸ߶�"), _T("��ʾ"), MB_OK);
// 		return;
		CaliConfig.SetWindowHeight(_ttoi(strTemp));
	}

	//��ȡ���ڿ��
	edtWindowWidth.GetWindowTextW(strTemp);
	if (_ttoi(strTemp) != 0)
	{
// 		MessageBox(_T("���ȡ��Ч���ڿ��"), _T("��ʾ"), MB_OK);
// 		return;
		CaliConfig.SetWindowWidth(_ttoi(strTemp));
	}

	//��ȡ������Ƿ���ˮ
	int isAdmixtureTo = chkIsAdmixtureTo.GetCheck();
	CaliConfig.SetIsAdmixtureToWater(isAdmixtureTo);

	//�����ͼ��ʽ
	CaliConfig.SetBackgroundCaptureType(m_ComboBoxCaptureType.GetCurSel());


	//�������㷨�Ƿ�Ϊ�գ���Ϊ��������ΪĬ���㷨
	int area_count = CaliConfig.GetMonitorAreaCount();
	if (area_count > 0)
	{
		const vector<MonitorArea*>* pAreas = CaliConfig.GetMonitorAreas();
		for (int i = 0; i < pAreas->size(); i++)
		{
			AreaMeasureFunction func = (*pAreas)[i]->GetMeasureFunction();
			if (func.method_type == 0 && func.v1_start_at == 0 && func.v1_end_at == 0)		//����Ƿ��ǿյļ����㷨
			{
				SCCaliAreaDlg::DefaultMeasureFunc(func, CA2T((*pAreas)[i]->GetMonitorAreaName()).m_psz);
				(*pAreas)[i]->SetMeasureFunction(func);
			}
		}
	}


	CaliConfig.SaveConfig();



	//�������ʹ�����Ҫ�������ļ�
	WriteConfigFile();

	MessageBox(_T("����ɹ���"), _T("��ʾ"), MB_OK);


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

void SCCalibrationUIDlg::ShowRectangle()
{
	if (AreaRectMaps.size() > 0)
	{
		CClientDC dcMouse(this);//��ǰDC
		int oldmode = dcMouse.SetROP2(R2_NOTXORPEN);//���û�ͼģʽ
		oldpen = dcMouse.SelectObject(&pen);//ѡ�񻭱ʵ���ǰDC

		for (int i = 0; i < AreaRectMaps.size(); i++)
		{
			//������Խ�ͼ����������
			RECT rtPic;
			picScreen.GetWindowRect(&rtPic);
			ScreenToClient(&rtPic);
			//������������picture�ؼ��ϵ����귶Χ
			RECT rtArea;
			rtArea.left = AreaRectMaps[i].AreaRect.left / 2 + rtPic.left;// 2 * (PDown.x - rtPic.left);
			rtArea.top = AreaRectMaps[i].AreaRect.top / 2 + rtPic.top;//2 * (PDown.y - rtPic.top);
			rtArea.right = AreaRectMaps[i].AreaRect.right / 2 + rtPic.left;//2 * (PMove.x - rtPic.left);
			rtArea.bottom = AreaRectMaps[i].AreaRect.bottom / 2 + rtPic.top;//2 * (PMove.y - rtPic.top);
			dcMouse.Rectangle(CRect(rtArea));
		}

		ReleaseDC(&dcMouse);
	}
}

//RECT SCCalibrationUIDlg::MakeRect(CPoint POrigin, CPoint PEnd)
//{
//	RECT rt;
//	if (POrigin.x <= PEnd.x && POrigin.y <= PEnd.y)
//	{
//		//��������¿�ѡ
//		rt.left = POrigin.x;
//		rt.top = POrigin.y;
//		rt.right = PEnd.x;
//		rt.bottom = PEnd.y;
//	} 
//	else if (POrigin.x > PEnd.x && POrigin.y > PEnd.y)
//	{
//		//��������Ͽ�ѡ
//		rt.left = PEnd.x;
//		rt.top = PEnd.y;
//		rt.right = POrigin.x;
//		rt.bottom = POrigin.y;
//	}
//	else if (POrigin.x <= PEnd.x && POrigin.y > PEnd.y)
//	{
//		//��������Ͽ�ѡ
//		rt.left = POrigin.x;
//		rt.top = PEnd.y;
//		rt.right = PEnd.x;
//		rt.bottom = POrigin.y;
//	}
//	else
//	{
//		//��������¿�ѡ
//		rt.left = PEnd.x;
//		rt.top = POrigin.y;
//		rt.right = POrigin.x;
//		rt.bottom = PEnd.y;
//	}
//	return rt;
//}

void SCCalibrationUIDlg::WriteConfigFile()
{
	//������ո��������ļ���Ҫ�Ĳ���
	ClearWriteConfPara();
	//��ȡ��ͻ���Ϣ
	GetMixInfo();
	//��ʼ��MapHopper
	InitMapHopper();
	//��ʼ���д����Ͱ�ͻ��ڵ�
	InitOtherHopper();
	//����CaliConfig�������������AnalogList��DigitalList
	BulidSensorConfList();
	//����Hopper�����ļ�Map
	BulidHopperConf();
	//AnalogList��DigitalListд��Sensor�����ļ�
	SaveSensorConf();
	//MapHopper���д����ڵ�ͽ�����ڵ�д��Hopper�����ļ�
	SaveHopperConf();
	//һЩͳ����Ϣд��mix_sys�����ļ�
	SaveMixSysConf();
}

void SCCalibrationUIDlg::GetMixInfo()
{
	CSettingConfig setting;
	int loadResult = setting.ReloadConfig();
	if (!loadResult)
	{
		const char* mixid = setting.GetMixingStationID();
		if (mixid)
		{
			int len = strlen(setting.GetMixingStationID());
			mix_machine_id = new char[len + 1];
			strcpy(mix_machine_id, setting.GetMixingStationID());
		}
		else
		{
			mix_machine_id = new char[2];
			strcpy(mix_machine_id, "0");
		}
	}
}

void SCCalibrationUIDlg::CreatHopper(HopperStru* hopper, CString hopperName)
{
	hopper->isValid = false;
	hopper->hopper_name = hopperName;
	hopper->machine_id = NULL;
	hopper->load_tap1_id = 0;
	hopper->load_tap2_id = 0;
	hopper->blow_tap1_id = 0;
	hopper->blow_tap2_id = 0;
	hopper->weight_cell_id = 0;
	hopper->bucket_id = 0;
	hopper->calc_id = 0;
	hopper->set_id = 0;
	hopper->temperature_id = 0;
}

void SCCalibrationUIDlg::InitMapHopper()
{
	HopperStru hp1;
	CreatHopper(&hp1, _T("����1"));
	hp1.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(hp1.machine_id, mix_machine_id);
	hp1.hopper_id = EN_HOPPER_ID_AGGR1;
	mapHopper.insert(pair<CString, HopperStru>(_T("����1"), hp1));

	HopperStru hp2;
	CreatHopper(&hp2, _T("����2"));
	hp2.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(hp2.machine_id, mix_machine_id);
	hp2.hopper_id = EN_HOPPER_ID_AGGR2;
	mapHopper.insert(pair<CString, HopperStru>(_T("����2"), hp2));

	HopperStru hp3;
	CreatHopper(&hp3, _T("����3"));
	hp3.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(hp3.machine_id, mix_machine_id);
	hp3.hopper_id = EN_HOPPER_ID_AGGR3;
	mapHopper.insert(pair<CString, HopperStru>(_T("����3"), hp3));

	HopperStru hp4;
	CreatHopper(&hp4, _T("����4"));
	hp4.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(hp4.machine_id, mix_machine_id);
	hp4.hopper_id = EN_HOPPER_ID_AGGR4;
	mapHopper.insert(pair<CString, HopperStru>(_T("����4"), hp4));

	HopperStru hp5;
	CreatHopper(&hp5, _T("����5"));
	hp5.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(hp5.machine_id, mix_machine_id);
	hp5.hopper_id = EN_HOPPER_ID_AGGR5;
	mapHopper.insert(pair<CString, HopperStru>(_T("����5"), hp5));

	HopperStru hp6;
	CreatHopper(&hp6, _T("����6"));
	hp6.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(hp6.machine_id, mix_machine_id);
	hp6.hopper_id = EN_HOPPER_ID_AGGR6;
	mapHopper.insert(pair<CString, HopperStru>(_T("����6"), hp6));

	HopperStru hp7;
	CreatHopper(&hp7, _T("����1"));
	hp7.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(hp7.machine_id, mix_machine_id);
	hp7.hopper_id = EN_HOPPER_ID_POWDER1;
	mapHopper.insert(pair<CString, HopperStru>(_T("����1"), hp7));

	HopperStru hp8;
	CreatHopper(&hp8, _T("����2"));
	hp8.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(hp8.machine_id, mix_machine_id);
	hp8.hopper_id = EN_HOPPER_ID_POWDER2;
	mapHopper.insert(pair<CString, HopperStru>(_T("����2"), hp8));

	HopperStru hp9;
	CreatHopper(&hp9, _T("����3"));
	hp9.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(hp9.machine_id, mix_machine_id);
	hp9.hopper_id = EN_HOPPER_ID_POWDER3;
	mapHopper.insert(pair<CString, HopperStru>(_T("����3"), hp9));

	HopperStru hp10;
	CreatHopper(&hp10, _T("����4"));
	hp10.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(hp10.machine_id, mix_machine_id);
	hp10.hopper_id = EN_HOPPER_ID_POWDER4;
	mapHopper.insert(pair<CString, HopperStru>(_T("����4"), hp10));

	HopperStru hp11;
	CreatHopper(&hp11, _T("����5"));
	hp11.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(hp11.machine_id, mix_machine_id);
	hp11.hopper_id = EN_HOPPER_ID_POWDER5;
	mapHopper.insert(pair<CString, HopperStru>(_T("����5"), hp11));

	HopperStru hp12;
	CreatHopper(&hp12, _T("����6"));
	hp12.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(hp12.machine_id, mix_machine_id);
	hp12.hopper_id = EN_HOPPER_ID_POWDER6;
	mapHopper.insert(pair<CString, HopperStru>(_T("����6"), hp12));

	HopperStru hp13;
	CreatHopper(&hp13, _T("��μ�1"));
	hp13.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(hp13.machine_id, mix_machine_id);
	hp13.hopper_id = EN_HOPPER_ID_ADMIX1;
	mapHopper.insert(pair<CString, HopperStru>(_T("��μ�1"), hp13));

	HopperStru hp14;
	CreatHopper(&hp14, _T("��μ�2"));
	hp14.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(hp14.machine_id, mix_machine_id);
	hp14.hopper_id = EN_HOPPER_ID_ADMIX2;
	mapHopper.insert(pair<CString, HopperStru>(_T("��μ�2"), hp14));

	HopperStru hp15;
	CreatHopper(&hp15, _T("ľ����"));
	hp15.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(hp15.machine_id, mix_machine_id);
	hp15.hopper_id = EN_HOPPER_ID_LIGNIN;
	mapHopper.insert(pair<CString, HopperStru>(_T("ľ����"), hp15));

	HopperStru hp16;
	CreatHopper(&hp16, _T("����"));
	hp16.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(hp16.machine_id, mix_machine_id);
	hp16.hopper_id = EN_HOPPER_ID_ASPHALT;
	mapHopper.insert(pair<CString, HopperStru>(_T("����"), hp16));

	HopperStru hp17;
	CreatHopper(&hp17, _T("ˮ"));
	hp17.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(hp17.machine_id, mix_machine_id);
	hp17.hopper_id = EN_HOPPER_ID_WATER;
	mapHopper.insert(pair<CString, HopperStru>(_T("ˮ"), hp17));
}

void SCCalibrationUIDlg::InitOtherHopper()
{
	//�д����ڵ�
	bucketHopper.bucket_serial = "bucket_1";
	bucketHopper.bucket_name = "�д���";
	bucketHopper.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(bucketHopper.machine_id, mix_machine_id);
	//��ͻ��ڵ�
	mixerHopper.mixer_caption = "mixer";
	mixerHopper.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(mixerHopper.machine_id, mix_machine_id);
}

void SCCalibrationUIDlg::ClearWriteConfPara()
{
	admixtureTo = 0;

	digitalsensor_total = 0;
	analogsensor_total = 0;
	weight_total = 0;
	bucket_total = 0;
	hopper_total = 0;
	if (mix_machine_id != NULL)
	{
		delete mix_machine_id;
		mix_machine_id = NULL;
	}
	//���Hopper�ڵ��AnalogList��DigitalList
	ClearHopper();
	ClearSensorConfList();
}

void SCCalibrationUIDlg::ClearHopper()
{
	//���MapHopper
	map<CString, HopperStru>::iterator itM;
	for (itM = mapHopper.begin(); itM != mapHopper.end(); itM++)
	{
		//����map�����ͷ�ÿ��Ԫ���е��ַ���ָ��
		if (itM->second.machine_id != NULL)
		{
			delete itM->second.machine_id;
			itM->second.machine_id = NULL;
		}
	}
	mapHopper.clear();
	//����д����ڵ�
	bucketHopper.bucket_id = 0;
	bucketHopper.blow_tap1_id = 0;
	if (bucketHopper.machine_id != NULL)
	{
		delete bucketHopper.machine_id;
		bucketHopper.machine_id = NULL;
	}
	//��հ�ͻ��ڵ�
	mixerHopper.temperature_id = 0;
	mixerHopper.blow_tap_id = 0;
	if (mixerHopper.machine_id != NULL)
	{
		delete mixerHopper.machine_id;
		mixerHopper.machine_id = NULL;
	}
}

void SCCalibrationUIDlg::ClearSensorConfList()
{
	//�ͷ�DigitalList
	list<DigitalSensor>::iterator itD;
	for (itD = DigitalList.begin(); itD != DigitalList.end(); itD++)
	{
		//����DigitalList�����ͷ�ÿ��Ԫ���е��ַ���ָ��
		if (itD->digital_serial != NULL)
		{
			delete itD->digital_serial;
			itD->digital_serial = NULL;
		}
		if (itD->digitalsensor_name != NULL)
		{
			delete itD->digitalsensor_name;
			itD->digitalsensor_name = NULL;
		}
	}
	DigitalList.clear();
	//�ͷ�AnalogList
	list<AnalogSensor>::iterator itA;
	for (itA = AnalogList.begin(); itA != AnalogList.end(); itA++)
	{
		//����DigitalList�����ͷ�ÿ��Ԫ���е��ַ���ָ��
		/*if (itA->analog_serial != NULL)
		{
			delete itA->analog_serial;
			itA->analog_serial = NULL;
		}*/
		if (itA->analogsensor_name != NULL)
		{
			delete itA->analogsensor_name;
			itA->analogsensor_name = NULL;
		}
	}
	AnalogList.clear();
}

void SCCalibrationUIDlg::BulidSensorConfList()
{
	if (CaliConfig.GetMonitorAreaCount() > 0)
	{
		USES_CONVERSION;
		const vector<MonitorArea*>* pAreas = CaliConfig.GetMonitorAreas();
		for (int i = 0; i < CaliConfig.GetMonitorAreas()->size(); i++)
		{
			//����AnalogList��DigitalList
			CString areaName;
			areaName = CA2T((*pAreas)[i]->GetMonitorAreaName());
			int index = areaName.Find(_T("����"));
			if (index > 0)
			{
				char* serial = new char[10];
				_itoa(digitalsensor_total + 1, serial,10);
				DigitalSensor ds;
				ds.digital_serial = new char[strlen(DIGITALSENSOR_) + strlen(serial) + 1];;
				strcpy(ds.digital_serial, DIGITALSENSOR_);
				strcat(ds.digital_serial, serial);
				ds.digitalsensor_id = (*pAreas)[i]->GetMonitorAreaID();
				ds.digitalsensor_name = new char[strlen((*pAreas)[i]->GetMonitorAreaName()) + 1];
				strcpy(ds.digitalsensor_name, (*pAreas)[i]->GetMonitorAreaName());
				ds.open_delay = 0;
				ds.close_delay = 0;

				DigitalList.push_back(ds);
				digitalsensor_total += 1;
			} 
			else
			{
				//��Ϊ����ͼ���Ҫ��analog_sensor�гӵ���ŷ�����ǰ�棬��˲��ڴ˴��������
				//������ŵĹ���Ų�Ƶ������ļ�����listʱ
				/*char* serial = new char[10];
				_itoa(analogsensor_total + 1, serial, 10);*/
				AnalogSensor as;
				/*as.analog_serial = new char[strlen(ANALOGSENSOR_) + strlen(serial) + 1];
				strcpy(as.analog_serial, ANALOGSENSOR_);
				strcat(as.analog_serial, serial);*/
				as.analogsensor_id = (*pAreas)[i]->GetMonitorAreaID();
				as.analogsensor_name = new char[strlen((*pAreas)[i]->GetMonitorAreaName()) + 1];
				strcpy(as.analogsensor_name, (*pAreas)[i]->GetMonitorAreaName());
				as.function = (*pAreas)[i]->GetMeasureFunction();
				if (areaName.Find(_T("��")) > 0)
				{
					//���еĳӷ��ڶ�����ǰ
					AnalogList.push_front(as);
					weight_total += 1;
				} 
				else
				{
					AnalogList.push_back(as);
				}
				//analogsensor_total += 1;
			}
		}
	}
}

void SCCalibrationUIDlg::BulidHopperConf()
{
	if (CaliConfig.GetMonitorAreaCount() > 0)
	{
		const vector<MonitorArea*>* pAreas = CaliConfig.GetMonitorAreas();
		for (int i = 0; i < CaliConfig.GetMonitorAreas()->size(); i++)
		{
			//����ж��Ƿ���ĳ�����ϵ�Hopper��������ӦHopper�ĸ�������ID
			FillMapHopper((*pAreas)[i], _T("����1"));
			FillMapHopper((*pAreas)[i], _T("����2"));
			FillMapHopper((*pAreas)[i], _T("����3"));
			FillMapHopper((*pAreas)[i], _T("����4"));
			FillMapHopper((*pAreas)[i], _T("����5"));
			FillMapHopper((*pAreas)[i], _T("����6"));
			FillMapHopper((*pAreas)[i], _T("����1"));
			FillMapHopper((*pAreas)[i], _T("����2"));
			FillMapHopper((*pAreas)[i], _T("����3"));
			FillMapHopper((*pAreas)[i], _T("����4"));
			FillMapHopper((*pAreas)[i], _T("����5"));
			FillMapHopper((*pAreas)[i], _T("����6"));
			FillMapHopper((*pAreas)[i], _T("��μ�1"));
			FillMapHopper((*pAreas)[i], _T("��μ�2"));
			FillMapHopper((*pAreas)[i], _T("ľ����"));
			FillMapHopper((*pAreas)[i], _T("����"));
			FillMapHopper((*pAreas)[i], _T("ˮ"));
			FillMapHopper((*pAreas)[i], _T("�����"));
		}
		//ѡ�����������ˮѡ���˴�������������д�������IDΪˮ�ĳ�ID
		mapHopper[_T("��μ�1")].bucket_id = admixtureTo;
		mapHopper[_T("��μ�2")].bucket_id = admixtureTo;
	}
}

void SCCalibrationUIDlg::FillMapHopper(const MonitorArea* area, CString hopperName)
{
	USES_CONVERSION;
	CString areaName, areaTypeTrue;
	areaName = CA2T(area->GetMonitorAreaName());
	int index = areaName.Find(_T("@"));
	if (areaName.Find(hopperName) >= 0)
	{
		areaTypeTrue = areaName.Mid(index + 1);
		//�����ֻ��һ�����Ϸ��ź��¶ȣ��ж�������֮�����ֱ�ӷ���
		if (hopperName == _T("�����"))
		{
			if (areaTypeTrue == _T("���Ϸ���1") || areaTypeTrue == _T("���Ϸ���2"))
			{
				mixerHopper.blow_tap_id = area->GetMonitorAreaID();
			}
			else if (areaTypeTrue == _T("�¶�"))
			{
				mixerHopper.temperature_id = area->GetMonitorAreaID();
			}

			return;
		}
		//�ж��Ƿ����������ϵ�Hopper����Hopperһ������������͵����ݣ���Ч��־isValid��Ϊtrue
		if (areaTypeTrue == _T("���Ϸ���1"))
		{
			mapHopper[hopperName].load_tap1_id = area->GetMonitorAreaID();
			mapHopper[hopperName].isValid = true;
		}
		else if (areaTypeTrue == _T("���Ϸ���2"))
		{
			mapHopper[hopperName].load_tap2_id = area->GetMonitorAreaID();
			mapHopper[hopperName].isValid = true;
		}
		else if (areaTypeTrue == _T("���Ϸ���1"))
		{
			mapHopper[hopperName].blow_tap1_id = area->GetMonitorAreaID();
			mapHopper[hopperName].isValid = true;
		}
		else if (areaTypeTrue == _T("���Ϸ���2"))
		{
			mapHopper[hopperName].blow_tap2_id = area->GetMonitorAreaID();
			mapHopper[hopperName].isValid = true;
		}
		else if (areaTypeTrue == _T("�д�������"))
		{
			mapHopper[hopperName].bucket_id = area->GetMonitorAreaID();
			mapHopper[hopperName].isValid = true;
			//ͬʱ��ֵ�д����ڵ㣬�д������Ϸ��ŵ�ID�����д���ID
			bucketHopper.bucket_id = area->GetMonitorAreaID();
			bucketHopper.blow_tap1_id = area->GetMonitorAreaID();
		}
		else if (areaTypeTrue == _T("��"))
		{
			mapHopper[hopperName].weight_cell_id = area->GetMonitorAreaID();
			mapHopper[hopperName].isValid = true;
			//ˮ�ĳӣ���ѡ�����������ˮѡ��ʱ����¼ˮ�ĳӵ�ID
			if (hopperName == _T("ˮ") && chkIsAdmixtureTo.GetCheck())
			{
				admixtureTo = area->GetMonitorAreaID();
			}
		}
		else if (areaTypeTrue == _T("����ֵ"))
		{
			mapHopper[hopperName].calc_id = area->GetMonitorAreaID();
			mapHopper[hopperName].isValid = true;
		}
		else if (areaTypeTrue == _T("�趨ֵ"))
		{
			mapHopper[hopperName].set_id = area->GetMonitorAreaID();
			mapHopper[hopperName].isValid = true;
		}
		else if (areaTypeTrue == _T("�¶�"))
		{
			mapHopper[hopperName].temperature_id = area->GetMonitorAreaID();
			mapHopper[hopperName].isValid = true;
		}
	}
}

void SCCalibrationUIDlg::WriteMeasureConfig(CRWData& rw, char* serialTrue, char* anFileName, int measureType, const vector<MeasurePointConfig>& points)
{
	rw._WriteProfileInt(serialTrue, MEASURE_METHOD, measureType, (char *)anFileName);
	char appName[256] = { 0 };
	for (int i = 0; i < points.size(); i++)
	{
		sprintf(appName, "value%d_start_at", i+1);
		rw._WriteProfileInt(serialTrue, appName, points[i]._start, (char *)anFileName);
		sprintf(appName, "value%d_start_delay", i + 1);
		rw._WriteProfileInt(serialTrue, appName, points[i]._start_delay, (char *)anFileName);
		sprintf(appName, "value%d_end_at", i + 1);
		rw._WriteProfileInt(serialTrue, appName, points[i]._end, (char *)anFileName);
		sprintf(appName, "value%d_end_delay", i + 1);
		rw._WriteProfileInt(serialTrue, appName, points[i]._end_delay, (char *)anFileName);
		sprintf(appName, "value%d_calc_method", i + 1);
		rw._WriteProfileInt(serialTrue, appName, points[i]._calc_method, (char *)anFileName);
	}
}

void SCCalibrationUIDlg::WriteMeasureConfig(CRWData& rw, char* serialTrue, char* anFileName, const AreaMeasureFunction& func)
{
	rw._WriteProfileInt(serialTrue, MEASURE_METHOD, func.method_type, (char *)anFileName);

	rw._WriteProfileInt(serialTrue, "value1_start_at", func.v1_start_at, (char *)anFileName);
	rw._WriteProfileInt(serialTrue, "value1_start_delay", func.v1_start_delay, (char *)anFileName);
	rw._WriteProfileInt(serialTrue, "value1_end_at", func.v1_end_at, (char *)anFileName);
	rw._WriteProfileInt(serialTrue, "value1_end_delay", func.v1_end_delay, (char *)anFileName);
	rw._WriteProfileInt(serialTrue, "value1_calc_method", func.v1_calc_method, (char *)anFileName);

	if (func.method_type > 1)
	{
		rw._WriteProfileInt(serialTrue, "value2_start_at", func.v2_start_at, (char *)anFileName);
		rw._WriteProfileInt(serialTrue, "value2_start_delay", func.v2_start_delay, (char *)anFileName);
		rw._WriteProfileInt(serialTrue, "value2_end_at", func.v2_end_at, (char *)anFileName);
		rw._WriteProfileInt(serialTrue, "value2_end_delay", func.v2_end_delay, (char *)anFileName);
		rw._WriteProfileInt(serialTrue, "value2_calc_method", func.v2_calc_method, (char *)anFileName);
	}
}

void SCCalibrationUIDlg::SaveSensorConf()
{
	//�����analog_sensor.conf��digital_sensor.conf����ɾ���ٴ����ļ�
	const char * anFileName = ConfigFile_Folder "/" Analog_Sensor_Conf;
	const char * diFileName = ConfigFile_Folder "/" Digital_Sensor_Conf;
	CRWData CWritefile;
	CWritefile._DeleteFile((char *)anFileName);
	CWritefile._CreateFile((char *)anFileName);
	CWritefile._DeleteFile((char *)diFileName);
	CWritefile._CreateFile((char *)diFileName);
	//����AnalogList��д�������ļ�
	list<AnalogSensor>::iterator itA;
	for (itA = AnalogList.begin(); itA != AnalogList.end(); itA++)
	{
		//����hopper��ǩ�����
		char* serial = new char[10];
		_itoa(analogsensor_total + 1, serial, 10);
		char* serialTrue = new char[strlen(ANALOGSENSOR_) + strlen(serial) + 1];
		strcpy(serialTrue, ANALOGSENSOR_);
		strcat(serialTrue, serial);
		//����id��name
		char anID[10];
		_itoa(itA->analogsensor_id, anID, 10);
		CWritefile._WriteProfileString(serialTrue, ANALOGSENSOR_ID, anID, (char *)anFileName);
		CWritefile._WriteProfileString(serialTrue, ANALOGSENSOR_NAME, itA->analogsensor_name, (char *)anFileName);
		if (itA->function.method_type == 0 && itA->function.v1_start_at == 0 && itA->function.v1_end_at == 0)		//����Ƿ��ǿյļ����㷨
		{
			SCCaliAreaDlg::DefaultMeasureFunc(itA->function, CA2T(itA->analogsensor_name).m_psz);
		}
		WriteMeasureConfig(CWritefile, serialTrue, (char*)anFileName, itA->function);

// 		CString areaName(CA2T(itA->analogsensor_name));
// 		int index = areaName.Find(_T("@"));
// 		CString areaTypeTrue = areaName.Mid(index + 1);
// 		if (areaTypeTrue == _T("��"))
// 		{
// 			MeasurePointConfig point1(1/*���Ͽ�*/, 0/*�ӳ�0*/, 3/*���Ͽ�*/, 0/*�ӳ�0*/, 3/*���ֵ*/);
// 			MeasurePointConfig point2(2/*���Ϲ�*/, 0/*�ӳ�0*/, 4/*���Ϲ�*/, 0/*�ӳ�0*/, 4/*��Сֵ*/);
// 			vector<MeasurePointConfig> points;
// 			points.push_back(point1);
// 			points.push_back(point2);
// 			WriteMeasureConfig(CWritefile, serialTrue, (char*)anFileName, 2/*��ֵ*/, points);
// 		}
// 		else if (areaTypeTrue == _T("����ֵ"))
// 		{
// 			MeasurePointConfig point1(1/*���Ͽ�*/, 0/*�ӳ�0*/, 4/*���Ϲ�*/, 0/*�ӳ�0*/, 3/*���ֵ*/);
// 			vector<MeasurePointConfig> points;
// 			points.push_back(point1);
// 			WriteMeasureConfig(CWritefile, serialTrue, (char*)anFileName, 1/*��ֵ*/, points);
// 		}
// 		else if (areaTypeTrue == _T("�趨ֵ"))
// 		{
// 			MeasurePointConfig point1(1/*���Ͽ�*/, 0/*�ӳ�0*/, 2/*���Ϲ�*/, 0/*�ӳ�0*/, 2/*���һ��ֵ*/);
// 			vector<MeasurePointConfig> points;
// 			points.push_back(point1);
// 			WriteMeasureConfig(CWritefile, serialTrue, (char*)anFileName, 1/*��ֵ*/, points);
// 		}
// 		else if (areaTypeTrue == _T("�¶�"))
// 		{
// 			MeasurePointConfig point1(1/*���Ͽ�*/, 0/*�ӳ�0*/, 3/*���Ͽ�*/, 0/*�ӳ�0*/, 3/*���ֵ*/);
// 			vector<MeasurePointConfig> points;
// 			points.push_back(point1);
// 			WriteMeasureConfig(CWritefile, serialTrue, (char*)anFileName, 1/*��ֵ*/, points);
// 		}

		//��Чan������1
		analogsensor_total += 1;

		if (serial)
		{
			delete[] serial;
		}
		if (serialTrue)
		{
			delete[] serialTrue;
		}
	}

	//����DigitalList��д�������ļ�
	list<DigitalSensor>::iterator itD;
	for (itD = DigitalList.begin(); itD != DigitalList.end(); itD++)
	{
		//����id��name��delayĿǰ����0
		char diID[10];
		_itoa(itD->digitalsensor_id, diID, 10);
		CWritefile._WriteProfileString(itD->digital_serial, DIGITALSENSOR_ID, diID, (char *)diFileName);
		CWritefile._WriteProfileString(itD->digital_serial, DIGITALSENSOR_NAME, itD->digitalsensor_name, (char *)diFileName);
		char delay[10];
		_itoa(itD->open_delay, delay, 10);
		CWritefile._WriteProfileString(itD->digital_serial, OPEN_DELAY, delay, (char *)diFileName);
		CWritefile._WriteProfileString(itD->digital_serial, CLOSE_DELAY, delay, (char *)diFileName);
	}
}

void SCCalibrationUIDlg::SaveHopperConf()
{
	//�����hopper.conf����ɾ���ٴ����ļ�
	const char * fileName = ConfigFile_Folder "/" Hopper_Conf;
	CRWData CWritefile;
	CWritefile._DeleteFile((char *)fileName);
	CWritefile._CreateFile((char *)fileName);
	//����mapHopper������Ч��HopperStruд�������ļ�
	map<CString, HopperStru>::iterator itM;
	for (itM = mapHopper.begin(); itM != mapHopper.end(); itM++)
	{
		if (itM->second.isValid)
		{
			USES_CONVERSION;
			//����hopper��ǩ�����
			char* serial = new char[10];
			_itoa(hopper_total + 1, serial, 10);
			char* serialTrue = new char[strlen(HOPPER_) + strlen(serial) + 1];
			strcpy(serialTrue, HOPPER_);
			strcat(serialTrue, serial);
			//��ͻ�ID
			CWritefile._WriteProfileString(serialTrue, MIX_MACHINE_ID, itM->second.machine_id, (char *)fileName);
			//hopper id
			char* hID = new char[10];
			_itoa(itM->second.hopper_id, hID, 10);
			CWritefile._WriteProfileString(serialTrue, HOPPER_ID, hID, (char *)fileName);
			//hopper name
			CWritefile._WriteProfileString(serialTrue, HOPPER_NAME, CT2A(itM->second.hopper_name), (char *)fileName);
			//���Ϸ���1��2
			char* load1 = new char[20];
			_itoa(itM->second.load_tap1_id, load1, 10);
			CWritefile._WriteProfileString(serialTrue, LOADING_TAP_ID_1, load1, (char *)fileName);
			char* load2 = new char[20];
			_itoa(itM->second.load_tap2_id, load2, 10);
			CWritefile._WriteProfileString(serialTrue, LOADING_TAP_ID_2, load2, (char *)fileName);
			//���Ϸ���1��2
			char* blow1 = new char[20];
			_itoa(itM->second.blow_tap1_id, blow1, 10);
			CWritefile._WriteProfileString(serialTrue, BLOWING_TAP_ID_1, blow1, (char *)fileName);
			char* blow2 = new char[20];
			_itoa(itM->second.blow_tap2_id, blow2, 10);
			CWritefile._WriteProfileString(serialTrue, BLOWING_TAP_ID_2, blow2, (char *)fileName);
			//��
			char* weight = new char[20];
			_itoa(itM->second.weight_cell_id, weight, 10);
			CWritefile._WriteProfileString(serialTrue, WEIGHING_CELL_ID, weight, (char *)fileName);
			//�д���ID
			char* bucket = new char[20];
			_itoa(itM->second.bucket_id, bucket, 10);
			CWritefile._WriteProfileString(serialTrue, BUKET_ID, bucket, (char *)fileName);
			//�趨ֵ
			char* setv = new char[20];
			_itoa(itM->second.set_id, setv, 10);
			CWritefile._WriteProfileString(serialTrue, SET_ID, setv, (char *)fileName);
			//����ֵ
			char* calcv = new char[20];
			_itoa(itM->second.calc_id, calcv, 10);
			CWritefile._WriteProfileString(serialTrue, CALC_ID, calcv, (char *)fileName);
			//�¶�
			char* tempv = new char[20];
			_itoa(itM->second.temperature_id, tempv, 10);
			CWritefile._WriteProfileString(serialTrue, TEMPERATURE_ID, tempv, (char *)fileName);
			//��Чhopper������1
			hopper_total += 1;
		}
	}
	//д�д����ڵ�
	//mix_machine_id
	CWritefile._WriteProfileString((char*)bucketHopper.bucket_serial, MIX_MACHINE_ID, bucketHopper.machine_id, (char *)fileName);
	//name
	CWritefile._WriteProfileString((char*)bucketHopper.bucket_serial, BUKET_NAME, (char*)bucketHopper.bucket_name, (char *)fileName);
	//bucket_id��blowing_tap1_id
	char* bid1 = new char[20];
	_itoa(bucketHopper.bucket_id, bid1, 10);
	CWritefile._WriteProfileString((char*)bucketHopper.bucket_serial, BUKET_ID, bid1, (char *)fileName);
	CWritefile._WriteProfileString((char*)bucketHopper.bucket_serial, BLOWING_TAP_ID_1, bid1, (char *)fileName);
	//д������ڵ�
	//mix_machine_id
	CWritefile._WriteProfileString((char*)mixerHopper.mixer_caption, MIX_MACHINE_ID, mixerHopper.machine_id, (char *)fileName);
	//blowing_tap_id
	char* blowid = new char[20];
	_itoa(mixerHopper.blow_tap_id, blowid, 10);
	CWritefile._WriteProfileString((char*)mixerHopper.mixer_caption, BLOWING_TAP_ID, blowid, (char *)fileName);
	//temperature
	char* temp = new char[20];
	_itoa(mixerHopper.temperature_id, temp, 10);
	CWritefile._WriteProfileString((char*)mixerHopper.mixer_caption, TEMPERATURE_ID, temp, (char *)fileName);
}

void SCCalibrationUIDlg::SaveMixSysConf()
{
	const char * fileName = ConfigFile_Folder "/" Mix_Sys_Conf;
	CRWData CWritefile;
	//û���򴴽��ļ��������޴���
	CWritefile._CreateFile((char *)fileName);
	//���˷����������������
	char* anNum = new char[20];
	_itoa(analogsensor_total, anNum, 10);
	CWritefile._WriteProfileString(HOPPER_SENSOR, ANALOGSENSOR_TOTAL, anNum, (char *)fileName);
	//������
	char* wNum = new char[20];
	_itoa(weight_total, wNum, 10);
	CWritefile._WriteProfileString(HOPPER_SENSOR, WEIGHT_TOTAL, wNum, (char *)fileName);
	//��������
	char* dNum = new char[20];
	_itoa(digitalsensor_total, dNum, 10);
	CWritefile._WriteProfileString(HOPPER_SENSOR, DIGITALSENSOR_TOTAL, dNum, (char *)fileName);
	//hopper����
	char* hNum = new char[20];
	_itoa(hopper_total, hNum, 10);
	CWritefile._WriteProfileString(HOPPER_SENSOR, HOPPER_TOTAL, hNum, (char *)fileName);
	//�д�������
	if (bucketHopper.blow_tap1_id == 0)
	{
		CWritefile._WriteProfileString(HOPPER_SENSOR, BUCKET_TOTAL, "0", (char *)fileName);
	}
	else
	{
		CWritefile._WriteProfileString(HOPPER_SENSOR, BUCKET_TOTAL, "1", (char *)fileName);
	}
}

void SCCalibrationUIDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���ָ��ظ�ԭ״
	SetClassLong(this->GetSafeHwnd(), GCLP_HCURSOR, (LONG)LoadCursor(NULL, IDC_ARROW));
	CDialogEx::OnCancel();
}

int SCCalibrationUIDlg::GetExePathFromHWND(HWND hwnd, CString& ExePath)
{
	TCHAR szPath[MAX_PATH];
	int ErrorCode = 0;
	DWORD idProcess;
	//��ȡ�������Ľ��̾��
	GetWindowThreadProcessId(hwnd, &idProcess);
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, idProcess);
	if (NULL != hProcess)
	{
		HMODULE hMod;
		DWORD cbNeeded;
		//ͨ�����̾����ȡ��س�������·�����ļ���
		if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
		{
			DWORD dw = GetModuleFileNameEx(hProcess, hMod, szPath, MAX_PATH);
			if (dw > 0)
			{
				//printf("szPath:   %S   \n",   szPath); 
				//MsgBox(szPath); 
				ExePath = szPath;
			}
			else
			{
				//printf("GetLastError:%d   \n",   GetLastError()); 
				ErrorCode = GetLastError();
			}
		}
		CloseHandle(hProcess);
	}

	return ErrorCode;
}

void SCCalibrationUIDlg::OnBnClickedCheck2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (chkLocalBmp.GetCheck())
	{
		//��ѡ���ر��ؽ�ͼʱ�����ؽ�ͼ�����ť����
		btnBmp.EnableWindow(true);
	} 
	else
	{
		//���򲻿���
		btnBmp.EnableWindow(false);
		GetDlgItem(IDC_BUTTON_BEFORE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);
	}
}

void SCCalibrationUIDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	USES_CONVERSION;
	CString strFile = _T("");
	TCHAR current[MAX_PATH];
	//��ȡ��ǰ·��
	GetCurrentDirectory(MAX_PATH, current);
	//ָ��·��Ϊ��ǰ����·���µ�ConfigFile�ļ���
	CString currentPath = current;
	currentPath += _T("\\CalibrationInfo\\");
	//���ļ��в������򴴽�
	::MakeSureDirectoryPathExists(CT2A(currentPath));
	//�򿪲���bmp��ͼ�ĶԻ�����
	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_NOCHANGEDIR, _T("Describe Files (*.png)|*.png|All Files (*.*)|*.*||"), NULL);
	dlgFile.m_ofn.lpstrInitialDir = currentPath;

	if (dlgFile.DoModal())
	{
		//��ȡ����·������������
		localBmpPath = dlgFile.GetPathName();
		//ȡ����չ��
		CString extend = localBmpPath.Right(localBmpPath.GetLength() - localBmpPath.ReverseFind('.') - 1);
		if (localBmpPath != _T("") && extend.CompareNoCase(_T("png")) == 0)
		{
			ShowLocalBmp(localBmpPath, true);
			ShowRectangle();
		}
		else if (localBmpPath != _T("") && extend.CompareNoCase(_T("bmp")) == 0)
		{
			ShowLocalBmp(localBmpPath, false);
			ShowRectangle();
		}

		RefreshPNGList(localBmpPath);
	}
}

void SCCalibrationUIDlg::RefreshPNGList(const CString& localBmpPath)
{
	pngFileList.clear();
	pngCurPos = 0;

	CString dir = localBmpPath.Left(localBmpPath.ReverseFind('\\') + 1);
	CString dirFind = dir + L"*.*";

	WIN32_FIND_DATA ffd;
	HANDLE hFind = FindFirstFile(dirFind, &ffd);
	BOOL bFind = TRUE;
	while (hFind && hFind != INVALID_HANDLE_VALUE && bFind)
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			if (_tcsstr(ffd.cFileName, L".png") != NULL || _tcsstr(ffd.cFileName, L".bmp") != NULL ||
				_tcsstr(ffd.cFileName, L".PNG") != NULL || _tcsstr(ffd.cFileName, L".BMP") != NULL)
			{
				CString fileFind = dir + ffd.cFileName;
				pngFileList.push_back(fileFind);
				if (localBmpPath == fileFind)
				{
					pngCurPos = pngFileList.size() - 1;
				}
			}
		}

		bFind = FindNextFile(hFind, &ffd);
	}
	FindClose(hFind);
	
	if (pngFileList.size() > 1)
	{
		if (pngCurPos > 0)
		{
			GetDlgItem(IDC_BUTTON_BEFORE)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_BUTTON_BEFORE)->EnableWindow(FALSE);
		}
		if (pngCurPos < pngFileList.size() - 1)
		{
			GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);
		}
	}
}


void SCCalibrationUIDlg::OnBnClickedButtonBefore()
{
	--pngCurPos;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	bool isPNG = false;
	if (pngFileList[pngCurPos].Right(4).CompareNoCase(L".png") == 0)
	{
		isPNG = true;
	}
	ShowLocalBmp(pngFileList[pngCurPos], isPNG);
	ShowRectangle();

	if (pngFileList.size() > 1)
	{
		if (pngCurPos > 0)
		{
			GetDlgItem(IDC_BUTTON_BEFORE)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_BUTTON_BEFORE)->EnableWindow(FALSE);
		}
		if (pngCurPos < pngFileList.size() - 1)
		{
			GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);
		}
	}
}


void SCCalibrationUIDlg::OnBnClickedButtonNext()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	++pngCurPos;
	bool isPNG = false;
	if (pngFileList[pngCurPos].Right(4).CompareNoCase(L".png") == 0)
	{
		isPNG = true;
	}
	ShowLocalBmp(pngFileList[pngCurPos], isPNG);
	ShowRectangle();
	if (pngFileList.size() > 1)
	{
		if (pngCurPos > 0)
		{
			GetDlgItem(IDC_BUTTON_BEFORE)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_BUTTON_BEFORE)->EnableWindow(FALSE);
		}
		if (pngCurPos < pngFileList.size() - 1)
		{
			GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);
		}
	}
}


void SCCalibrationUIDlg::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_ComboBoxCaptureType.GetCurSel() == 3)
	{
		RECT rc;
		::GetWindowRect(GetDesktopWindow()->GetSafeHwnd(), &rc);
		if (&rc != NULL)
		{
			CString strWindowHeight;
			strWindowHeight.Format(_T("%d"), rc.bottom - rc.top);
			edtWindowHeight.SetWindowTextW(strWindowHeight);
			CString strWindowWidth;
			strWindowWidth.Format(_T("%d"), rc.right - rc.left);
			edtWindowWidth.SetWindowTextW(strWindowWidth);
		}

		GetDlgItem(IDC_BUTTON_BASEBMP)->EnableWindow(TRUE);
	}
	else
	{
		RECT rc;
		::GetWindowRect(m_hScreenWindowHandle, &rc);
		if (&rc != NULL)
		{
			CString strWindowHeight;
			strWindowHeight.Format(_T("%d"), rc.bottom - rc.top);
			edtWindowHeight.SetWindowTextW(strWindowHeight);
			CString strWindowWidth;
			strWindowWidth.Format(_T("%d"), rc.right - rc.left);
			edtWindowWidth.SetWindowTextW(strWindowWidth);
		}

		GetDlgItem(IDC_BUTTON_BASEBMP)->EnableWindow(FALSE);
	}
}


void SCCalibrationUIDlg::OnBnClickedButtonBasebmp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	::DeleteFileA(ConfigFile_Folder "/" "ScaleBase.bmp");

	bitmapCali.Save(_T(ConfigFile_Folder "/" "ScaleBase.bmp"));
	if (::PathFileExists(_T(ConfigFile_Folder "/" "ScaleBase.bmp")))
	{
		MessageBox(L"����ɹ�");
	}
	else
	{
		MessageBox(L"����ʧ��");
	}
}


void SCCalibrationUIDlg::OnBnClickedButtonParenthwnd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_parentPos++;
	if (m_parentPos > m_parents.size() - 1)
	{
		m_parentPos--;
		return;
	}
	m_hScreenWindowHandle = m_parents[m_parentPos];
	if (m_parentPos == (m_parents.size() - 1))
	{
		GetDlgItem(IDC_BUTTON_ParentHWND)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ChildHWND)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_ParentHWND)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_ChildHWND)->EnableWindow(TRUE);
	}


	//��ô�����
	TCHAR temp[MAX_PATH];
	::GetWindowText(m_hScreenWindowHandle, temp, MAX_PATH);
	edtWindowName.SetWindowTextW(temp);
	//��ô�������
	::GetClassName(m_hScreenWindowHandle, temp, MAX_PATH);
	edtWindowClass.SetWindowTextW(temp);
	//��ô��ڴ�С
	RECT rc;
	if (m_ComboBoxCaptureType.GetCurSel() != 3)
	{
		::GetWindowRect(m_hScreenWindowHandle, &rc);
	}
	else
	{
		::GetWindowRect(GetDesktopWindow()->GetSafeHwnd(), &rc);
	}

	if (&rc != NULL)
	{
		CString strWindowHeight;
		strWindowHeight.Format(_T("%d"), rc.bottom - rc.top);
		edtWindowHeight.SetWindowTextW(strWindowHeight);
		CString strWindowWidth;
		strWindowWidth.Format(_T("%d"), rc.right - rc.left);
		edtWindowWidth.SetWindowTextW(strWindowWidth);
	}

	OnBnClickedButton5();
}


void SCCalibrationUIDlg::OnBnClickedButtonChildhwnd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_parentPos--;
	if (m_parentPos < 0)
	{
		m_parentPos++;
		return;
	}
	m_hScreenWindowHandle = m_parents[m_parentPos];
	if (m_parentPos == 0)
	{
		GetDlgItem(IDC_BUTTON_ChildHWND)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ParentHWND)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_ParentHWND)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_ChildHWND)->EnableWindow(TRUE);
	}


	//��ô�����
	TCHAR temp[MAX_PATH];
	::GetWindowText(m_hScreenWindowHandle, temp, MAX_PATH);
	edtWindowName.SetWindowTextW(temp);
	//��ô�������
	::GetClassName(m_hScreenWindowHandle, temp, MAX_PATH);
	edtWindowClass.SetWindowTextW(temp);
	//��ô��ڴ�С
	RECT rc;
	if (m_ComboBoxCaptureType.GetCurSel() != 3)
	{
		::GetWindowRect(m_hScreenWindowHandle, &rc);
	}
	else
	{
		::GetWindowRect(GetDesktopWindow()->GetSafeHwnd(), &rc);
	}

	if (&rc != NULL)
	{
		CString strWindowHeight;
		strWindowHeight.Format(_T("%d"), rc.bottom - rc.top);
		edtWindowHeight.SetWindowTextW(strWindowHeight);
		CString strWindowWidth;
		strWindowWidth.Format(_T("%d"), rc.right - rc.left);
		edtWindowWidth.SetWindowTextW(strWindowWidth);
	}

	OnBnClickedButton5();
}
