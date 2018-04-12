// SCCalibrationUIDlg.cpp : 实现文件
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


// SCCalibrationUIDlg 对话框

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


// SCCalibrationUIDlg 消息处理程序


BOOL SCCalibrationUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//m_hCursor = AfxGetApp()->LoadCursor(IDC_CURSOR2);

	if (!InitCaliConfig())
	{
		MessageBox(_T("初始化标定数据失败"), _T("错误"), MB_OK);
	}
	//OnBnClickedButton5();

	GetDlgItem(IDC_BUTTON_ParentHWND)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_ChildHWND)->EnableWindow(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void SCCalibrationUIDlg::OnMenubtnOpenold()
{
	// TODO: 在此添加命令处理程序代码
	SCCaliLoadOldDlg loadCali;
	loadCali.SetPCaliClass(&CaliConfig);
	int nRet = loadCali.DoModal();
	if (nRet == IDOK)
	{
		ShowDlg(&CaliConfig);
		//清空区域维护列表
		AreaRectMaps.clear();
		vector<AreaRectsMap>(AreaRectMaps).swap(AreaRectMaps);
		//加载新的区域列表
		InitAreaRects();
		//加载新的图片和框选
		if (chkLocalBmp.GetCheck() && localBmpPath != _T(""))
		{
			//取得扩展名
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
				//处理后的截图展示到图片控件
				bitmapShow.Draw(pDC->m_hDC);
			}
		}
		
		ShowRectangle();
	}
}

void SCCalibrationUIDlg::OnMenubtnNew()
{
	// TODO: 在此添加命令处理程序代码
	SCCaliNewDlg newCali;
	newCali.SetPCaliClass(&CaliConfig);
	int nRet = newCali.DoModal();
	if (nRet == IDOK)
	{
		ShowDlg(&CaliConfig);
		//根据区域列表清空已有框选
		/*for (int i = 0; i < AreaRectMaps.size(); i ++)
		{
			CancelRectangle(CPoint(AreaRectMaps[i].AreaRect.left, AreaRectMaps[i].AreaRect.top), CPoint(AreaRectMaps[i].AreaRect.right, AreaRectMaps[i].AreaRect.bottom));
		}*/
		//清空区域维护列表
		AreaRectMaps.clear();
		vector<AreaRectsMap>(AreaRectMaps).swap(AreaRectMaps);
		GetWindowHandle();
		if (chkLocalBmp.GetCheck() && localBmpPath != _T(""))
		{
			//取得扩展名
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
				//处理后的截图展示到图片控件
				bitmapShow.Draw(pDC->m_hDC);
			}
		}
	}
}

void SCCalibrationUIDlg::OnMenubtnSaveas()
{
	// TODO: 在此添加命令处理程序代码
	SCCaliSaveAsDlg saveAs;
	saveAs.SetPCaliClass(&CaliConfig);
	int nRet = saveAs.DoModal();
	if (nRet == IDOK)
	{
		MessageBox(_T("另存为样本标定文件成功"), _T("提示"), MB_OK);
	}
}

bool SCCalibrationUIDlg::InitCaliConfig()
{
	//读取当前标定xml配置文件
	int loadResult = CaliConfig.ReloadConfig();
	if (!loadResult)
	{
		//鼠标框选标志置为false
		mouseSelect = false;
		//鼠标坐标记录点初始化为0
		PDown.SetPoint(0, 0);
		PMove.SetPoint(0, 0);
		//标定信息展示到页面
		ShowDlg(&CaliConfig);
		//建立区域维护列表
		InitAreaRects();
		//获取监视窗口句柄
		GetWindowHandle();
		//picture控件修改type类型为bmp
		picScreen.ModifyStyle(0, SS_BITMAP);
		//加载截图checkbox置为否
		chkLocalBmp.SetCheck(0);
		btnBmp.EnableWindow(false);

		GetDlgItem(IDC_BUTTON_BEFORE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);

		//后端截屏类型
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
	//根据监视窗口类名和窗口名查找监视串口句柄
	edtWindowClass.GetWindowTextW(windowClass);
	edtWindowName.GetWindowTextW(windowName);
	CWnd *pwnd = FindWindow(windowClass, windowName);
	m_hScreenWindowHandle = pwnd->GetSafeHwnd();
}

void SCCalibrationUIDlg::ShowDlg(const CCalibrationConfig* caliConfig)
{
	//基本信息展示到控件中
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
	//读取界面信息和标定信息到标定配置类中
	return true;
}

//void SCCalibrationUIDlg::OnBnClickedButton1()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	CString strFile = _T("");
//	//打开查找exe程序的对话窗口
//	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_NOCHANGEDIR, _T("Describe Files (*.exe)|*.exe|All Files (*.*)|*.*||"), NULL);
//
//	if (dlgFile.DoModal())
//	{
//		//获取完整路径包含程序名
//		strFile = dlgFile.GetPathName();
//	}
//	//赋值给对应控件
//	edtSoftwarePath.SetWindowTextW(strFile);
//}

void SCCalibrationUIDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	HCURSOR   hcur = GetCursor();
	//开启全局鼠标钩子
	InstallKbHook(this->GetSafeHwnd());
	SetClassLong(this->GetSafeHwnd(), GCLP_HCURSOR, (LONG)LoadCursor(NULL, IDC_CROSS));
	bGetWindow = true;
}

void SCCalibrationUIDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	//获取监视窗口句柄
	GetWindowHandle();
	//将截图展示到picture控件
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
	//鼠标钩子的回调函数
	MSLLHOOKSTRUCT *pkbhs = (MSLLHOOKSTRUCT *)lParam;
	char strMsg[100] = { 0 };

	switch (nCode)
	{
	case HC_ACTION:
	{
		//鼠标移动  
		if (wParam == WM_MOUSEMOVE)
		{
			HCURSOR hCur = LoadCursor(NULL, IDC_CROSS);
			::SetCursor(hCur);
			SetSystemCursor(hCur, (DWORD)IDC_CROSS);
		}

		//鼠标左击  
		if (wParam == WM_LBUTTONDOWN)
		{
			if (g_hWnd != NULL)
			{
				//监视到鼠标左击后发送消息给自己
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
	//全局鼠标钩子已存在，则先释放钩子
	if (hhookMs)
		UninstallKbHook();

	g_hWnd = hWnd;
	//设置全局鼠标钩子
	hhookMs = SetWindowsHookEx(WH_MOUSE_LL,
		(HOOKPROC)LowLevelMouseProc, AfxGetApp()->m_hInstance, NULL);
	//返回是否设置成功
	return (hhookMs != NULL);
}

BOOL SCCalibrationUIDlg::UninstallKbHook()
{
	//释放鼠标钩子
	BOOL fOk = FALSE;
	if (hhookMs) {
		fOk = UnhookWindowsHookEx(hhookMs);
		hhookMs = NULL;
	}
	//返回是否释放成功
	return(fOk);
}

LRESULT SCCalibrationUIDlg::OnMyMsg(WPARAM wParam, LPARAM lParam)
{
	//接收消息函数重写
	if (bGetWindow)
	{
		//鼠标位置
		PMSLLHOOKSTRUCT mouseHookStruct = (PMSLLHOOKSTRUCT)lParam;
		//获得所在窗口句柄
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


		//获得窗口名
		TCHAR temp[MAX_PATH];
		::GetWindowText(m_hScreenWindowHandle, temp, MAX_PATH);
		edtWindowName.SetWindowTextW(temp);
		//获得窗口类名
		::GetClassName(m_hScreenWindowHandle, temp, MAX_PATH);
		edtWindowClass.SetWindowTextW(temp);
		//获得窗口大小
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
		//释放鼠标钩子
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
		//如果监视窗口句柄不为空，获取监视窗口当前状态
		if (::IsIconic(m_hScreenWindowHandle))
		{
			//最小化
			status = WINDOWSTATUS::STATUS_MINIMIZE;
		}
		else
		{
			if (m_hScreenWindowHandle == ::GetForegroundWindow())
			{
				//前台显示
				status = WINDOWSTATUS::STATUS_FOREGROUND;
			}
			else
			{
				//后台显示
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
	//打开进程令牌环
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	//获得进程本地唯一ID
	LookupPrivilegeValue(NULL, name, &luid);

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	tp.Privileges[0].Luid = luid;
	//调整权限
	AdjustTokenPrivileges(hToken, 0, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
	return 0;
}

//*****************************************************************************************************************************

BOOL InjectDll(const char *DllFullPath, const DWORD dwRemoteProcessId)
{
	HANDLE hRemoteProcess;
	EnableDebugPriv(SE_DEBUG_NAME);
	//打开远程线程
	hRemoteProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwRemoteProcessId);

	char *pszLibFileRemote;

	//使用VirtualAllocEx函数在远程进程的内存地址空间分配DLL文件名空间
	pszLibFileRemote = (char *)VirtualAllocEx(hRemoteProcess, NULL, lstrlenA(DllFullPath) + 1, MEM_COMMIT, PAGE_READWRITE);


	//使用WriteProcessMemory函数将DLL的路径名写入到远程进程的内存空间
	BOOL bWrite = WriteProcessMemory(hRemoteProcess, pszLibFileRemote, (void *)DllFullPath, lstrlenA(DllFullPath) + 1, NULL);
	if (!bWrite)
	{
		return FALSE;
	}

	//计算LoadLibraryA的入口地址
	PTHREAD_START_ROUTINE pfnStartAddr = (PTHREAD_START_ROUTINE)
		GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryA");
	//(关于GetModuleHandle函数和GetProcAddress函数)

	//启动远程线程LoadLibraryA，通过远程线程调用创建新的线程
	HANDLE hRemoteThread;
	if ((hRemoteThread = CreateRemoteThread(hRemoteProcess, NULL, 0, pfnStartAddr, pszLibFileRemote, 0, NULL)) == NULL)
	{
		return FALSE;
	}

	// 释放句柄

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
	//获取当前窗口显示状态
	WINDOWSTATUS status = GetWindowStatus();
	//针对后台显示的状态进行处理
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
		//窗口句柄不为空，监视窗口截图
		HBITMAP hBitmap = GetScreenShot();
		if (hBitmap)
		{
			//使用CBitmapEx类处理截图，将截图缩小到50%
			CDC* pDC = picScreen.GetDC();
			bitmapShow.SetResampleMode(RM_BICUBIC);
			bitmapShow.Load(hBitmap);
			bitmapShow.Scale(50, 50);
			//获取picture控件大小，并将控件颜色先全部置灰，RGB(240,240,240)
			RECT rt;
			picScreen.GetClientRect(&rt);
			FillRect(pDC->m_hDC, &rt, CreateSolidBrush(RGB(240, 240, 240)));
			//处理后的截图展示到图片控件
			bitmapShow.Draw(pDC->m_hDC);
			ReleaseDC(pDC);						// add by yaodi for DC release

												//加载标定大图
			bitmapCali.Load(hBitmap);

			DeleteObject(hBitmap);
			hBitmap = NULL;
		}
		else
		{
			//窗口截图为空，获取picture控件大小，并将控件颜色全部置灰，RGB(240,240,240)
			RECT rt;
			picScreen.GetClientRect(&rt);
			CDC* pDC = picScreen.GetDC();
			FillRect(pDC->m_hDC, &rt, CreateSolidBrush(RGB(240, 240, 240)));
			ReleaseDC(pDC);
		}
	}
	else
	{
		//窗口句柄为空，获取picture控件大小，并将控件颜色全部置灰，RGB(240,240,240)
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
	//加载标定大图
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
						//其他情况均为颜色RGB值
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

	//使用CBitmapEx类处理截图，将截图缩小到50%
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
	//获取picture控件大小，并将控件颜色先全部置灰，RGB(240,240,240)
	RECT rt;
	picScreen.GetClientRect(&rt);
	FillRect(pDC->m_hDC, &rt, CreateSolidBrush(RGB(240, 240, 240)));
	//处理后的截图展示到图片控件
	bitmapShow.Draw(pDC->m_hDC);
	ReleaseDC(pDC);						// add by yaodi for DC release

	//加载标定大图
	//bitmapCali.Load(localPath.GetBuffer(0));
}

void SCCalibrationUIDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CDialogEx::OnPaint();
	CDialogEx::UpdateWindow();
	// TODO: 在此处添加消息处理程序代码
	if (chkLocalBmp.GetCheck() && localBmpPath != _T(""))
	{
		//取得扩展名
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
			//处理后的截图展示到图片控件
			bitmapShow.Draw(pDC->m_hDC);
		}
		
	}
	ShowRectangle();
    // 不为绘图消息调用 CDialogEx::OnPaint()
}


// void SCCalibrationUIDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
// {
// 	CDialogEx::OnActivate(nState, pWndOther, bMinimized);
// 
// // 	if ( nState == WA_ACTIVE )
// // 	{
// // 		if (chkLocalBmp.GetCheck() && localBmpPath != _T(""))
// // 		{
// // 			//取得扩展名
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
		//遍历已有标定区域
		vector<AreaRectsMap>::iterator it;
		for (it = AreaRectMaps.begin();it != AreaRectMaps.end();)
		//for (int i = 0; i < AreaRects.size(); i++)
		{
			//用已有区域反算picture控件上的坐标范围
			RECT rtArea;
			rtArea.left = (*it).AreaRect.left / 2 + rtPic.left;// 2 * (PDown.x - rtPic.left);
			rtArea.top = (*it).AreaRect.top / 2 + rtPic.top;//2 * (PDown.y - rtPic.top);
			rtArea.right = (*it).AreaRect.right / 2 + rtPic.left;//2 * (PMove.x - rtPic.left);
			rtArea.bottom = (*it).AreaRect.bottom / 2 + rtPic.top;//2 * (PMove.y - rtPic.top);
			//构造当前鼠标画框矩形
			RECT curRect, tempRect;
			curRect = rectConv.MakeRect(PDown, point);
			//判断两个矩形是否相交，相交认为是修改已有区域，或者直接点击已有矩形区域内
			if (IntersectRect(&tempRect, &rtArea, &curRect) || PtInRect(&rtArea, point) || PtInRect(&rtArea, PDown))
			//if (PtInRect(&rtArea, point) || PtInRect(&rtArea, PDown) || PtInRect(&curRect, CPoint(rtArea.left, rtArea.top)))
			{
				//临时记录是否处于鼠标框选状态
				bool tempMouseSelect = mouseSelect;
				//鼠标选取标志置为false
				mouseSelect = false;
				//建立标定区域界面
				SCCaliAreaDlg area;
				//将标定信息类、截图屏幕bmp、相对截图的区域坐标、放大系数传入标定区域界面
				area.SetPCaliClass(&CaliConfig);
				//area.SetHopperList(&HopperList);
				area.SetAreaInfo((*it).areaID);
				area.SetRectArea((*it).AreaRect);
				//计算标定框选图放大系数
				float coef = rectConv.CalcAreaCoef(rectConv.areaPicSide, rectConv.areaPicSide, (*it).AreaRect.bottom - (*it).AreaRect.top, (*it).AreaRect.right - (*it).AreaRect.left);
				area.SetCalcCoef(coef);
				area.SetCBitmap(bitmapCali);
				int nRet = area.DoModal();
				//之前处于鼠标框选状态，取消框选画图
				if (tempMouseSelect)
					CancelRectangle(PDown, PMove);
				if (nRet == IDOK)
				{
					//保存成功
					//删除对应区域的框选画图
					CancelRectangle(CPoint(rtArea.left, rtArea.top), CPoint(rtArea.right, rtArea.bottom));
					//更新新的真实区域
					area.GetRectArea((*it).AreaRect);
					//更新控件坐标区域
					rtArea.left = (*it).AreaRect.left / 2 + rtPic.left;// 2 * (PDown.x - rtPic.left);
					rtArea.top = (*it).AreaRect.top / 2 + rtPic.top;//2 * (PDown.y - rtPic.top);
					rtArea.right = (*it).AreaRect.right / 2 + rtPic.left;//2 * (PMove.x - rtPic.left);
					rtArea.bottom = (*it).AreaRect.bottom / 2 + rtPic.top;//2 * (PMove.y - rtPic.top);
					//新区域画框选
					CClientDC dcMouse(this);//当前DC
					int oldmode = dcMouse.SetROP2(R2_NOTXORPEN);//设置绘图模式
					oldpen = dcMouse.SelectObject(&pen);//选择画笔到当前DC
					dcMouse.Rectangle(CRect(rtArea));
					ReleaseDC(&dcMouse);
				}
				else if (nRet == AREA_DELETE)
				{
					//删除成功
					//删除对应已删除区域的框选画图
					CancelRectangle(CPoint(rtArea.left, rtArea.top), CPoint(rtArea.right, rtArea.bottom));
					//释放已删除区域的框选记录
					AreaRectMaps.erase(it);
				}
				else
				{
					//取消
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
		//鼠标选取标志置为false
		mouseSelect = false;
		if (fabs(point.x - PDown.x) > 2 && fabs(point.y - PDown.y) > 1/*fabs(point.x - PDown.x) > 10 && fabs(point.y - PDown.y) > 8*/)	//取消检查
		{
			//建立标定区域界面
			SCCaliAreaDlg area;
			//将标定信息类、截图屏幕bmp、相对截图的区域坐标传入标定区域界面
			area.SetPCaliClass(&CaliConfig);
			//area.SetHopperList(&HopperList);
			//计算相对截图的区域坐标
			RECT rtArea = rectConv.MakeRect(CPoint(2 * (PDown.x - rtPic.left), 2 * (PDown.y - rtPic.top)), CPoint(2 * (PMove.x - rtPic.left), 2 * (PMove.y - rtPic.top)));
			area.SetRectArea(rtArea);
			//计算标定框选图放大系数
			float coef = rectConv.CalcAreaCoef(rectConv.areaPicSide, rectConv.areaPicSide, rtArea.bottom - rtArea.top, rtArea.right - rtArea.left);
			area.SetCalcCoef(coef);
			area.SetCBitmap(bitmapCali);
			int nRet = area.DoModal();
			if (nRet == IDOK)
			{
				//保存成功
				//删除对应区域的框选画图
				CancelRectangle(PDown, PMove);
				//更新新的真实区域
				area.GetRectArea(rtArea);
				//更新控件坐标区域
				RECT rtShow;
				rtShow.left = rtArea.left / 2 + rtPic.left;// 2 * (PDown.x - rtPic.left);
				rtShow.top = rtArea.top / 2 + rtPic.top;//2 * (PDown.y - rtPic.top);
				rtShow.right = rtArea.right / 2 + rtPic.left;//2 * (PMove.x - rtPic.left);
				rtShow.bottom = rtArea.bottom / 2 + rtPic.top;//2 * (PMove.y - rtPic.top);
				//新区域画框选
				CClientDC dcMouse(this);//当前DC
				int oldmode = dcMouse.SetROP2(R2_NOTXORPEN);//设置绘图模式
				oldpen = dcMouse.SelectObject(&pen);//选择画笔到当前DC
				dcMouse.Rectangle(CRect(rtShow));
				ReleaseDC(&dcMouse);
				//取得当前最大的MonitorAreaID
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
				//删除成功，实际此处应该是新建标定区域，删除分支无法进入
			}
			else
			{
				//取消
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (mouseSelect)
	{
		CClientDC dcMouse(this);//当前DC
		int oldmode = dcMouse.SetROP2(R2_NOTXORPEN);//设置绘图模式
		oldpen = dcMouse.SelectObject(&pen);//选择画笔到当前DC

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
	//取消矩形绘制
	CClientDC dcMouse(this);//当前DC
	int oldmode = dcMouse.SetROP2(R2_NOTXORPEN);//设置绘图模式
	oldpen = dcMouse.SelectObject(&pen);//选择画笔到当前DC

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
//	//乘以10之后除以5
//	hCoef = hCoef * 10 / 5;
//	wCoef = wCoef * 10 / 5;
//	//四舍五入保留到个位数
//	int ihCoef = floor(hCoef * 10 + 0.5) / 10;
//	int iwCoef = floor(wCoef * 10 + 0.5) / 10;
//	//再次乘以5，将初始值的10倍值修约到5，之后除以10，得到最终结果为0.5的倍数
//	hCoef = ihCoef * 5.0 / 10.0;
//	wCoef = iwCoef * 5.0 / 10.0;
//	//最终返回倍数小的那个系数
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
				//开关量，表示阀门，将画框区域设为以阀门标定点为中心的20*20方框
				areaMap.AreaRect.left = (*pAreas)[i]->GetMonitorAreaX() - 10;
				areaMap.AreaRect.top = (*pAreas)[i]->GetMonitorAreaY() - 10;
				areaMap.AreaRect.right = (*pAreas)[i]->GetMonitorAreaX() + 10;
				areaMap.AreaRect.bottom = (*pAreas)[i]->GetMonitorAreaY() + 10;
			}
			else
			{
				//非阀门，画框区域为真实标定区域
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
			MessageBox(L"选择抓取桌面的截图方式时，请保存一张完整的基准截图");
			return;
		}
	}
	

	// TODO: 在此添加控件通知处理程序代码
	USES_CONVERSION;
	int tempLen;
	CString strTemp;
	//获取程序名称
	edtSoftwareName.GetWindowTextW(strTemp);
	if (strTemp == "")
	{
		MessageBox(_T("程序名称不能为空"), _T("提示"), MB_OK);
		return;
	}
	tempLen = strlen(CT2A(strTemp));
	CaliConfig.SetSoftwareName(CT2A(strTemp), tempLen);
	//获取程序版本
	edtVSoftwareVer.GetWindowTextW(strTemp);
	if (strTemp == "")
	{
		MessageBox(_T("程序版本不能为空"), _T("提示"), MB_OK);
		return;
	}
	tempLen = strlen(CT2A(strTemp));
	CaliConfig.SetSoftwareVer(CT2A(strTemp), tempLen);
	//获取程序路径
	edtSoftwarePath.GetWindowTextW(strTemp);
	if (strTemp != "")
	{
// 		MessageBox(_T("程序路径不能为空"), _T("提示"), MB_OK);
// 		return;
		tempLen = strlen(CT2A(strTemp));
		CaliConfig.SetSoftwarePath(CT2A(strTemp), tempLen);
	}

	//获取窗口名称
	edtWindowName.GetWindowTextW(strTemp);
	if (strTemp != "")
	{
// 		MessageBox(_T("窗口名称不能为空"), _T("提示"), MB_OK);
// 		return;
		tempLen = strlen(CT2A(strTemp));
		CaliConfig.SetWindowName(CT2A(strTemp), tempLen);
	}

	//获取窗口类名
	edtWindowClass.GetWindowTextW(strTemp);
	if (strTemp != "")
	{
// 		MessageBox(_T("窗口类名不能为空"), _T("提示"), MB_OK);
// 		return;
		tempLen = strlen(CT2A(strTemp));
		CaliConfig.SetWindowClass(CT2A(strTemp), tempLen);
	}

	//获取窗口高度
	edtWindowHeight.GetWindowTextW(strTemp);
	if (_ttoi(strTemp) != 0)
	{
// 		MessageBox(_T("请获取有效窗口高度"), _T("提示"), MB_OK);
// 		return;
		CaliConfig.SetWindowHeight(_ttoi(strTemp));
	}

	//获取窗口宽度
	edtWindowWidth.GetWindowTextW(strTemp);
	if (_ttoi(strTemp) != 0)
	{
// 		MessageBox(_T("请获取有效窗口宽度"), _T("提示"), MB_OK);
// 		return;
		CaliConfig.SetWindowWidth(_ttoi(strTemp));
	}

	//获取外掺剂是否入水
	int isAdmixtureTo = chkIsAdmixtureTo.GetCheck();
	CaliConfig.SetIsAdmixtureToWater(isAdmixtureTo);

	//保存截图方式
	CaliConfig.SetBackgroundCaptureType(m_ComboBoxCaptureType.GetCurSel());


	//检查计量算法是否为空，如为空则设置为默认算法
	int area_count = CaliConfig.GetMonitorAreaCount();
	if (area_count > 0)
	{
		const vector<MonitorArea*>* pAreas = CaliConfig.GetMonitorAreas();
		for (int i = 0; i < pAreas->size(); i++)
		{
			AreaMeasureFunction func = (*pAreas)[i]->GetMeasureFunction();
			if (func.method_type == 0 && func.v1_start_at == 0 && func.v1_end_at == 0)		//检查是否是空的计量算法
			{
				SCCaliAreaDlg::DefaultMeasureFunc(func, CA2T((*pAreas)[i]->GetMonitorAreaName()).m_psz);
				(*pAreas)[i]->SetMeasureFunction(func);
			}
		}
	}


	CaliConfig.SaveConfig();



	//保存计算和传输需要的配置文件
	WriteConfigFile();

	MessageBox(_T("保存成功！"), _T("提示"), MB_OK);


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

void SCCalibrationUIDlg::ShowRectangle()
{
	if (AreaRectMaps.size() > 0)
	{
		CClientDC dcMouse(this);//当前DC
		int oldmode = dcMouse.SetROP2(R2_NOTXORPEN);//设置绘图模式
		oldpen = dcMouse.SelectObject(&pen);//选择画笔到当前DC

		for (int i = 0; i < AreaRectMaps.size(); i++)
		{
			//计算相对截图的区域坐标
			RECT rtPic;
			picScreen.GetWindowRect(&rtPic);
			ScreenToClient(&rtPic);
			//用已有区域反算picture控件上的坐标范围
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
//		//鼠标向右下框选
//		rt.left = POrigin.x;
//		rt.top = POrigin.y;
//		rt.right = PEnd.x;
//		rt.bottom = PEnd.y;
//	} 
//	else if (POrigin.x > PEnd.x && POrigin.y > PEnd.y)
//	{
//		//鼠标向左上框选
//		rt.left = PEnd.x;
//		rt.top = PEnd.y;
//		rt.right = POrigin.x;
//		rt.bottom = POrigin.y;
//	}
//	else if (POrigin.x <= PEnd.x && POrigin.y > PEnd.y)
//	{
//		//鼠标向右上框选
//		rt.left = POrigin.x;
//		rt.top = PEnd.y;
//		rt.right = PEnd.x;
//		rt.bottom = POrigin.y;
//	}
//	else
//	{
//		//鼠标向左下框选
//		rt.left = PEnd.x;
//		rt.top = POrigin.y;
//		rt.right = POrigin.x;
//		rt.bottom = PEnd.y;
//	}
//	return rt;
//}

void SCCalibrationUIDlg::WriteConfigFile()
{
	//首先清空各种配置文件需要的参数
	ClearWriteConfPara();
	//获取拌和机信息
	GetMixInfo();
	//初始化MapHopper
	InitMapHopper();
	//初始化中储斗和拌和机节点
	InitOtherHopper();
	//遍历CaliConfig，构建配置相关AnalogList和DigitalList
	BulidSensorConfList();
	//构建Hopper配置文件Map
	BulidHopperConf();
	//AnalogList，DigitalList写入Sensor配置文件
	SaveSensorConf();
	//MapHopper、中储斗节点和搅拌机节点写入Hopper配置文件
	SaveHopperConf();
	//一些统计信息写入mix_sys配置文件
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
	CreatHopper(&hp1, _T("骨料1"));
	hp1.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(hp1.machine_id, mix_machine_id);
	hp1.hopper_id = EN_HOPPER_ID_AGGR1;
	mapHopper.insert(pair<CString, HopperStru>(_T("骨料1"), hp1));

	HopperStru hp2;
	CreatHopper(&hp2, _T("骨料2"));
	hp2.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(hp2.machine_id, mix_machine_id);
	hp2.hopper_id = EN_HOPPER_ID_AGGR2;
	mapHopper.insert(pair<CString, HopperStru>(_T("骨料2"), hp2));

	HopperStru hp3;
	CreatHopper(&hp3, _T("骨料3"));
	hp3.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(hp3.machine_id, mix_machine_id);
	hp3.hopper_id = EN_HOPPER_ID_AGGR3;
	mapHopper.insert(pair<CString, HopperStru>(_T("骨料3"), hp3));

	HopperStru hp4;
	CreatHopper(&hp4, _T("骨料4"));
	hp4.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(hp4.machine_id, mix_machine_id);
	hp4.hopper_id = EN_HOPPER_ID_AGGR4;
	mapHopper.insert(pair<CString, HopperStru>(_T("骨料4"), hp4));

	HopperStru hp5;
	CreatHopper(&hp5, _T("骨料5"));
	hp5.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(hp5.machine_id, mix_machine_id);
	hp5.hopper_id = EN_HOPPER_ID_AGGR5;
	mapHopper.insert(pair<CString, HopperStru>(_T("骨料5"), hp5));

	HopperStru hp6;
	CreatHopper(&hp6, _T("骨料6"));
	hp6.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(hp6.machine_id, mix_machine_id);
	hp6.hopper_id = EN_HOPPER_ID_AGGR6;
	mapHopper.insert(pair<CString, HopperStru>(_T("骨料6"), hp6));

	HopperStru hp7;
	CreatHopper(&hp7, _T("粉料1"));
	hp7.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(hp7.machine_id, mix_machine_id);
	hp7.hopper_id = EN_HOPPER_ID_POWDER1;
	mapHopper.insert(pair<CString, HopperStru>(_T("粉料1"), hp7));

	HopperStru hp8;
	CreatHopper(&hp8, _T("粉料2"));
	hp8.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(hp8.machine_id, mix_machine_id);
	hp8.hopper_id = EN_HOPPER_ID_POWDER2;
	mapHopper.insert(pair<CString, HopperStru>(_T("粉料2"), hp8));

	HopperStru hp9;
	CreatHopper(&hp9, _T("粉料3"));
	hp9.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(hp9.machine_id, mix_machine_id);
	hp9.hopper_id = EN_HOPPER_ID_POWDER3;
	mapHopper.insert(pair<CString, HopperStru>(_T("粉料3"), hp9));

	HopperStru hp10;
	CreatHopper(&hp10, _T("粉料4"));
	hp10.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(hp10.machine_id, mix_machine_id);
	hp10.hopper_id = EN_HOPPER_ID_POWDER4;
	mapHopper.insert(pair<CString, HopperStru>(_T("粉料4"), hp10));

	HopperStru hp11;
	CreatHopper(&hp11, _T("粉料5"));
	hp11.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(hp11.machine_id, mix_machine_id);
	hp11.hopper_id = EN_HOPPER_ID_POWDER5;
	mapHopper.insert(pair<CString, HopperStru>(_T("粉料5"), hp11));

	HopperStru hp12;
	CreatHopper(&hp12, _T("粉料6"));
	hp12.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(hp12.machine_id, mix_machine_id);
	hp12.hopper_id = EN_HOPPER_ID_POWDER6;
	mapHopper.insert(pair<CString, HopperStru>(_T("粉料6"), hp12));

	HopperStru hp13;
	CreatHopper(&hp13, _T("外参剂1"));
	hp13.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(hp13.machine_id, mix_machine_id);
	hp13.hopper_id = EN_HOPPER_ID_ADMIX1;
	mapHopper.insert(pair<CString, HopperStru>(_T("外参剂1"), hp13));

	HopperStru hp14;
	CreatHopper(&hp14, _T("外参剂2"));
	hp14.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(hp14.machine_id, mix_machine_id);
	hp14.hopper_id = EN_HOPPER_ID_ADMIX2;
	mapHopper.insert(pair<CString, HopperStru>(_T("外参剂2"), hp14));

	HopperStru hp15;
	CreatHopper(&hp15, _T("木质素"));
	hp15.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(hp15.machine_id, mix_machine_id);
	hp15.hopper_id = EN_HOPPER_ID_LIGNIN;
	mapHopper.insert(pair<CString, HopperStru>(_T("木质素"), hp15));

	HopperStru hp16;
	CreatHopper(&hp16, _T("沥青"));
	hp16.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(hp16.machine_id, mix_machine_id);
	hp16.hopper_id = EN_HOPPER_ID_ASPHALT;
	mapHopper.insert(pair<CString, HopperStru>(_T("沥青"), hp16));

	HopperStru hp17;
	CreatHopper(&hp17, _T("水"));
	hp17.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(hp17.machine_id, mix_machine_id);
	hp17.hopper_id = EN_HOPPER_ID_WATER;
	mapHopper.insert(pair<CString, HopperStru>(_T("水"), hp17));
}

void SCCalibrationUIDlg::InitOtherHopper()
{
	//中储斗节点
	bucketHopper.bucket_serial = "bucket_1";
	bucketHopper.bucket_name = "中储斗";
	bucketHopper.machine_id = new char[strlen(mix_machine_id) + 1];
	strcpy(bucketHopper.machine_id, mix_machine_id);
	//拌和机节点
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
	//清空Hopper节点和AnalogList，DigitalList
	ClearHopper();
	ClearSensorConfList();
}

void SCCalibrationUIDlg::ClearHopper()
{
	//清空MapHopper
	map<CString, HopperStru>::iterator itM;
	for (itM = mapHopper.begin(); itM != mapHopper.end(); itM++)
	{
		//遍历map，并释放每个元素中的字符串指针
		if (itM->second.machine_id != NULL)
		{
			delete itM->second.machine_id;
			itM->second.machine_id = NULL;
		}
	}
	mapHopper.clear();
	//清空中储斗节点
	bucketHopper.bucket_id = 0;
	bucketHopper.blow_tap1_id = 0;
	if (bucketHopper.machine_id != NULL)
	{
		delete bucketHopper.machine_id;
		bucketHopper.machine_id = NULL;
	}
	//清空拌和机节点
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
	//释放DigitalList
	list<DigitalSensor>::iterator itD;
	for (itD = DigitalList.begin(); itD != DigitalList.end(); itD++)
	{
		//遍历DigitalList，并释放每个元素中的字符串指针
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
	//释放AnalogList
	list<AnalogSensor>::iterator itA;
	for (itA = AnalogList.begin(); itA != AnalogList.end(); itA++)
	{
		//遍历DigitalList，并释放每个元素中的字符串指针
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
			//构建AnalogList和DigitalList
			CString areaName;
			areaName = CA2T((*pAreas)[i]->GetMonitorAreaName());
			int index = areaName.Find(_T("阀门"));
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
				//因为传输和计算要求analog_sensor中秤的序号放在最前面，因此不在此处生成序号
				//生成序号的功能挪移到保存文件遍历list时
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
				if (areaName.Find(_T("秤")) > 0)
				{
					//所有的秤放在队列最前
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
			//逐个判断是否是某个材料的Hopper，并填充对应Hopper的各种类型ID
			FillMapHopper((*pAreas)[i], _T("骨料1"));
			FillMapHopper((*pAreas)[i], _T("骨料2"));
			FillMapHopper((*pAreas)[i], _T("骨料3"));
			FillMapHopper((*pAreas)[i], _T("骨料4"));
			FillMapHopper((*pAreas)[i], _T("骨料5"));
			FillMapHopper((*pAreas)[i], _T("骨料6"));
			FillMapHopper((*pAreas)[i], _T("粉料1"));
			FillMapHopper((*pAreas)[i], _T("粉料2"));
			FillMapHopper((*pAreas)[i], _T("粉料3"));
			FillMapHopper((*pAreas)[i], _T("粉料4"));
			FillMapHopper((*pAreas)[i], _T("粉料5"));
			FillMapHopper((*pAreas)[i], _T("粉料6"));
			FillMapHopper((*pAreas)[i], _T("外参剂1"));
			FillMapHopper((*pAreas)[i], _T("外参剂2"));
			FillMapHopper((*pAreas)[i], _T("木质素"));
			FillMapHopper((*pAreas)[i], _T("沥青"));
			FillMapHopper((*pAreas)[i], _T("水"));
			FillMapHopper((*pAreas)[i], _T("搅拌机"));
		}
		//选择了外掺剂入水选项，则此处更新外掺剂的中储斗阀门ID为水的秤ID
		mapHopper[_T("外参剂1")].bucket_id = admixtureTo;
		mapHopper[_T("外参剂2")].bucket_id = admixtureTo;
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
		//搅拌机只有一个放料阀门和温度，判断这两个之后可以直接返回
		if (hopperName == _T("搅拌机"))
		{
			if (areaTypeTrue == _T("放料阀门1") || areaTypeTrue == _T("放料阀门2"))
			{
				mixerHopper.blow_tap_id = area->GetMonitorAreaID();
			}
			else if (areaTypeTrue == _T("温度"))
			{
				mixerHopper.temperature_id = area->GetMonitorAreaID();
			}

			return;
		}
		//判断是否是其他材料的Hopper，该Hopper一旦存在相关类型的数据，有效标志isValid置为true
		if (areaTypeTrue == _T("上料阀门1"))
		{
			mapHopper[hopperName].load_tap1_id = area->GetMonitorAreaID();
			mapHopper[hopperName].isValid = true;
		}
		else if (areaTypeTrue == _T("上料阀门2"))
		{
			mapHopper[hopperName].load_tap2_id = area->GetMonitorAreaID();
			mapHopper[hopperName].isValid = true;
		}
		else if (areaTypeTrue == _T("放料阀门1"))
		{
			mapHopper[hopperName].blow_tap1_id = area->GetMonitorAreaID();
			mapHopper[hopperName].isValid = true;
		}
		else if (areaTypeTrue == _T("放料阀门2"))
		{
			mapHopper[hopperName].blow_tap2_id = area->GetMonitorAreaID();
			mapHopper[hopperName].isValid = true;
		}
		else if (areaTypeTrue == _T("中储斗阀门"))
		{
			mapHopper[hopperName].bucket_id = area->GetMonitorAreaID();
			mapHopper[hopperName].isValid = true;
			//同时赋值中储斗节点，中储斗放料阀门的ID即是中储斗ID
			bucketHopper.bucket_id = area->GetMonitorAreaID();
			bucketHopper.blow_tap1_id = area->GetMonitorAreaID();
		}
		else if (areaTypeTrue == _T("秤"))
		{
			mapHopper[hopperName].weight_cell_id = area->GetMonitorAreaID();
			mapHopper[hopperName].isValid = true;
			//水的秤，且选择了外掺剂入水选项时，记录水的秤的ID
			if (hopperName == _T("水") && chkIsAdmixtureTo.GetCheck())
			{
				admixtureTo = area->GetMonitorAreaID();
			}
		}
		else if (areaTypeTrue == _T("计算值"))
		{
			mapHopper[hopperName].calc_id = area->GetMonitorAreaID();
			mapHopper[hopperName].isValid = true;
		}
		else if (areaTypeTrue == _T("设定值"))
		{
			mapHopper[hopperName].set_id = area->GetMonitorAreaID();
			mapHopper[hopperName].isValid = true;
		}
		else if (areaTypeTrue == _T("温度"))
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
	//先清空analog_sensor.conf和digital_sensor.conf，先删除再创建文件
	const char * anFileName = ConfigFile_Folder "/" Analog_Sensor_Conf;
	const char * diFileName = ConfigFile_Folder "/" Digital_Sensor_Conf;
	CRWData CWritefile;
	CWritefile._DeleteFile((char *)anFileName);
	CWritefile._CreateFile((char *)anFileName);
	CWritefile._DeleteFile((char *)diFileName);
	CWritefile._CreateFile((char *)diFileName);
	//遍历AnalogList，写入配置文件
	list<AnalogSensor>::iterator itA;
	for (itA = AnalogList.begin(); itA != AnalogList.end(); itA++)
	{
		//生成hopper标签的序号
		char* serial = new char[10];
		_itoa(analogsensor_total + 1, serial, 10);
		char* serialTrue = new char[strlen(ANALOGSENSOR_) + strlen(serial) + 1];
		strcpy(serialTrue, ANALOGSENSOR_);
		strcat(serialTrue, serial);
		//保存id和name
		char anID[10];
		_itoa(itA->analogsensor_id, anID, 10);
		CWritefile._WriteProfileString(serialTrue, ANALOGSENSOR_ID, anID, (char *)anFileName);
		CWritefile._WriteProfileString(serialTrue, ANALOGSENSOR_NAME, itA->analogsensor_name, (char *)anFileName);
		if (itA->function.method_type == 0 && itA->function.v1_start_at == 0 && itA->function.v1_end_at == 0)		//检查是否是空的计量算法
		{
			SCCaliAreaDlg::DefaultMeasureFunc(itA->function, CA2T(itA->analogsensor_name).m_psz);
		}
		WriteMeasureConfig(CWritefile, serialTrue, (char*)anFileName, itA->function);

// 		CString areaName(CA2T(itA->analogsensor_name));
// 		int index = areaName.Find(_T("@"));
// 		CString areaTypeTrue = areaName.Mid(index + 1);
// 		if (areaTypeTrue == _T("秤"))
// 		{
// 			MeasurePointConfig point1(1/*上料开*/, 0/*延迟0*/, 3/*放料开*/, 0/*延迟0*/, 3/*最大值*/);
// 			MeasurePointConfig point2(2/*上料关*/, 0/*延迟0*/, 4/*放料关*/, 0/*延迟0*/, 4/*最小值*/);
// 			vector<MeasurePointConfig> points;
// 			points.push_back(point1);
// 			points.push_back(point2);
// 			WriteMeasureConfig(CWritefile, serialTrue, (char*)anFileName, 2/*差值*/, points);
// 		}
// 		else if (areaTypeTrue == _T("计算值"))
// 		{
// 			MeasurePointConfig point1(1/*上料开*/, 0/*延迟0*/, 4/*放料关*/, 0/*延迟0*/, 3/*最大值*/);
// 			vector<MeasurePointConfig> points;
// 			points.push_back(point1);
// 			WriteMeasureConfig(CWritefile, serialTrue, (char*)anFileName, 1/*单值*/, points);
// 		}
// 		else if (areaTypeTrue == _T("设定值"))
// 		{
// 			MeasurePointConfig point1(1/*上料开*/, 0/*延迟0*/, 2/*上料关*/, 0/*延迟0*/, 2/*最后一个值*/);
// 			vector<MeasurePointConfig> points;
// 			points.push_back(point1);
// 			WriteMeasureConfig(CWritefile, serialTrue, (char*)anFileName, 1/*单值*/, points);
// 		}
// 		else if (areaTypeTrue == _T("温度"))
// 		{
// 			MeasurePointConfig point1(1/*上料开*/, 0/*延迟0*/, 3/*放料开*/, 0/*延迟0*/, 3/*最大值*/);
// 			vector<MeasurePointConfig> points;
// 			points.push_back(point1);
// 			WriteMeasureConfig(CWritefile, serialTrue, (char*)anFileName, 1/*单值*/, points);
// 		}

		//有效an计数加1
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

	//遍历DigitalList，写入配置文件
	list<DigitalSensor>::iterator itD;
	for (itD = DigitalList.begin(); itD != DigitalList.end(); itD++)
	{
		//保存id和name，delay目前都是0
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
	//先清空hopper.conf，先删除再创建文件
	const char * fileName = ConfigFile_Folder "/" Hopper_Conf;
	CRWData CWritefile;
	CWritefile._DeleteFile((char *)fileName);
	CWritefile._CreateFile((char *)fileName);
	//遍历mapHopper，将有效的HopperStru写入配置文件
	map<CString, HopperStru>::iterator itM;
	for (itM = mapHopper.begin(); itM != mapHopper.end(); itM++)
	{
		if (itM->second.isValid)
		{
			USES_CONVERSION;
			//生成hopper标签的序号
			char* serial = new char[10];
			_itoa(hopper_total + 1, serial, 10);
			char* serialTrue = new char[strlen(HOPPER_) + strlen(serial) + 1];
			strcpy(serialTrue, HOPPER_);
			strcat(serialTrue, serial);
			//拌和机ID
			CWritefile._WriteProfileString(serialTrue, MIX_MACHINE_ID, itM->second.machine_id, (char *)fileName);
			//hopper id
			char* hID = new char[10];
			_itoa(itM->second.hopper_id, hID, 10);
			CWritefile._WriteProfileString(serialTrue, HOPPER_ID, hID, (char *)fileName);
			//hopper name
			CWritefile._WriteProfileString(serialTrue, HOPPER_NAME, CT2A(itM->second.hopper_name), (char *)fileName);
			//上料阀门1，2
			char* load1 = new char[20];
			_itoa(itM->second.load_tap1_id, load1, 10);
			CWritefile._WriteProfileString(serialTrue, LOADING_TAP_ID_1, load1, (char *)fileName);
			char* load2 = new char[20];
			_itoa(itM->second.load_tap2_id, load2, 10);
			CWritefile._WriteProfileString(serialTrue, LOADING_TAP_ID_2, load2, (char *)fileName);
			//放料阀门1，2
			char* blow1 = new char[20];
			_itoa(itM->second.blow_tap1_id, blow1, 10);
			CWritefile._WriteProfileString(serialTrue, BLOWING_TAP_ID_1, blow1, (char *)fileName);
			char* blow2 = new char[20];
			_itoa(itM->second.blow_tap2_id, blow2, 10);
			CWritefile._WriteProfileString(serialTrue, BLOWING_TAP_ID_2, blow2, (char *)fileName);
			//秤
			char* weight = new char[20];
			_itoa(itM->second.weight_cell_id, weight, 10);
			CWritefile._WriteProfileString(serialTrue, WEIGHING_CELL_ID, weight, (char *)fileName);
			//中储斗ID
			char* bucket = new char[20];
			_itoa(itM->second.bucket_id, bucket, 10);
			CWritefile._WriteProfileString(serialTrue, BUKET_ID, bucket, (char *)fileName);
			//设定值
			char* setv = new char[20];
			_itoa(itM->second.set_id, setv, 10);
			CWritefile._WriteProfileString(serialTrue, SET_ID, setv, (char *)fileName);
			//计算值
			char* calcv = new char[20];
			_itoa(itM->second.calc_id, calcv, 10);
			CWritefile._WriteProfileString(serialTrue, CALC_ID, calcv, (char *)fileName);
			//温度
			char* tempv = new char[20];
			_itoa(itM->second.temperature_id, tempv, 10);
			CWritefile._WriteProfileString(serialTrue, TEMPERATURE_ID, tempv, (char *)fileName);
			//有效hopper计数加1
			hopper_total += 1;
		}
	}
	//写中储斗节点
	//mix_machine_id
	CWritefile._WriteProfileString((char*)bucketHopper.bucket_serial, MIX_MACHINE_ID, bucketHopper.machine_id, (char *)fileName);
	//name
	CWritefile._WriteProfileString((char*)bucketHopper.bucket_serial, BUKET_NAME, (char*)bucketHopper.bucket_name, (char *)fileName);
	//bucket_id和blowing_tap1_id
	char* bid1 = new char[20];
	_itoa(bucketHopper.bucket_id, bid1, 10);
	CWritefile._WriteProfileString((char*)bucketHopper.bucket_serial, BUKET_ID, bid1, (char *)fileName);
	CWritefile._WriteProfileString((char*)bucketHopper.bucket_serial, BLOWING_TAP_ID_1, bid1, (char *)fileName);
	//写搅拌机节点
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
	//没有则创建文件，有则无处理
	CWritefile._CreateFile((char *)fileName);
	//除了阀门以外的区域数量
	char* anNum = new char[20];
	_itoa(analogsensor_total, anNum, 10);
	CWritefile._WriteProfileString(HOPPER_SENSOR, ANALOGSENSOR_TOTAL, anNum, (char *)fileName);
	//秤数量
	char* wNum = new char[20];
	_itoa(weight_total, wNum, 10);
	CWritefile._WriteProfileString(HOPPER_SENSOR, WEIGHT_TOTAL, wNum, (char *)fileName);
	//阀门数量
	char* dNum = new char[20];
	_itoa(digitalsensor_total, dNum, 10);
	CWritefile._WriteProfileString(HOPPER_SENSOR, DIGITALSENSOR_TOTAL, dNum, (char *)fileName);
	//hopper数量
	char* hNum = new char[20];
	_itoa(hopper_total, hNum, 10);
	CWritefile._WriteProfileString(HOPPER_SENSOR, HOPPER_TOTAL, hNum, (char *)fileName);
	//中储斗数量
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
	// TODO: 在此添加控件通知处理程序代码
	//鼠标指针回复原状
	SetClassLong(this->GetSafeHwnd(), GCLP_HCURSOR, (LONG)LoadCursor(NULL, IDC_ARROW));
	CDialogEx::OnCancel();
}

int SCCalibrationUIDlg::GetExePathFromHWND(HWND hwnd, CString& ExePath)
{
	TCHAR szPath[MAX_PATH];
	int ErrorCode = 0;
	DWORD idProcess;
	//获取监控软件的进程句柄
	GetWindowThreadProcessId(hwnd, &idProcess);
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, idProcess);
	if (NULL != hProcess)
	{
		HMODULE hMod;
		DWORD cbNeeded;
		//通过进程句柄获取监控程序所在路径和文件名
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
	// TODO: 在此添加控件通知处理程序代码
	if (chkLocalBmp.GetCheck())
	{
		//点选加载本地截图时，本地截图浏览按钮可用
		btnBmp.EnableWindow(true);
	} 
	else
	{
		//否则不可用
		btnBmp.EnableWindow(false);
		GetDlgItem(IDC_BUTTON_BEFORE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);
	}
}

void SCCalibrationUIDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	USES_CONVERSION;
	CString strFile = _T("");
	TCHAR current[MAX_PATH];
	//获取当前路径
	GetCurrentDirectory(MAX_PATH, current);
	//指定路径为当前程序路径下的ConfigFile文件夹
	CString currentPath = current;
	currentPath += _T("\\CalibrationInfo\\");
	//子文件夹不存在则创建
	::MakeSureDirectoryPathExists(CT2A(currentPath));
	//打开查找bmp截图的对话窗口
	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_NOCHANGEDIR, _T("Describe Files (*.png)|*.png|All Files (*.*)|*.*||"), NULL);
	dlgFile.m_ofn.lpstrInitialDir = currentPath;

	if (dlgFile.DoModal())
	{
		//获取完整路径包含程序名
		localBmpPath = dlgFile.GetPathName();
		//取得扩展名
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
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
	::DeleteFileA(ConfigFile_Folder "/" "ScaleBase.bmp");

	bitmapCali.Save(_T(ConfigFile_Folder "/" "ScaleBase.bmp"));
	if (::PathFileExists(_T(ConfigFile_Folder "/" "ScaleBase.bmp")))
	{
		MessageBox(L"保存成功");
	}
	else
	{
		MessageBox(L"保存失败");
	}
}


void SCCalibrationUIDlg::OnBnClickedButtonParenthwnd()
{
	// TODO: 在此添加控件通知处理程序代码
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


	//获得窗口名
	TCHAR temp[MAX_PATH];
	::GetWindowText(m_hScreenWindowHandle, temp, MAX_PATH);
	edtWindowName.SetWindowTextW(temp);
	//获得窗口类名
	::GetClassName(m_hScreenWindowHandle, temp, MAX_PATH);
	edtWindowClass.SetWindowTextW(temp);
	//获得窗口大小
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
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
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


	//获得窗口名
	TCHAR temp[MAX_PATH];
	::GetWindowText(m_hScreenWindowHandle, temp, MAX_PATH);
	edtWindowName.SetWindowTextW(temp);
	//获得窗口类名
	::GetClassName(m_hScreenWindowHandle, temp, MAX_PATH);
	edtWindowClass.SetWindowTextW(temp);
	//获得窗口大小
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
