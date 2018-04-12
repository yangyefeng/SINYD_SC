/////////////////////////////////////////////////////////////////////////////
/// @File name: ScreenWindowController.cpp
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 		
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#include "ScreenWindowController.h"
#include "ConfigInitializer.h"
#include "log.h"
#include <tchar.h>
#include <atlbase.h>
#include "..\SINYD_SC_CommonUtility\BitmapEx.h"



CScreenWindowController::CScreenWindowController()
	: m_ulProcessID(0)
	, m_hScreenWindowHandle(NULL)
	, m_hdcWindow(NULL)
	, m_hdcMemDC(NULL)
	, m_hbmScreen(NULL)
	, m_LastResolution()
{
	m_LastResolution.width = 0;
	m_LastResolution.height = 0;

	m_bmpScaleBase.Load(_T(ConfigFile_Folder "/" "ScaleBase.bmp"));
	m_ScaleThreshold = 10;	//default
	FILE* file = fopen(ConfigFile_Folder "/" "Scale.txt", "r");
	if (file)
	{
		char threshold[11] = { 0 };
		int len = fread(threshold, sizeof(char), 10, file);
		if (len > 0)
		{
			m_ScaleThreshold = atoi(threshold);
		}
		fclose(file);
	}
}


CScreenWindowController::~CScreenWindowController()
{
	ReleaseAllHDC();
}

void CScreenWindowController::SetProcessID(unsigned long ulProcessID)
{
	//CriticalSectionLock lock(m_handleCS);

	if (ulProcessID != m_ulProcessID)
	{
		m_ulProcessID = ulProcessID;

		RefreshWindowHandle();
	}

}

WINDOWSTATUS CScreenWindowController::GetWindowStatus()
{
	//CriticalSectionLock lock(m_handleCS);

	WINDOWSTATUS status = WINDOWSTATUS::STATUS_UNKNOWN;
	if (m_hScreenWindowHandle != NULL)
	{
		if (IsIconic(m_hScreenWindowHandle))
		{
			status = WINDOWSTATUS::STATUS_MINIMIZE;
		}
		else
		{
			if (m_hScreenWindowHandle == ::GetForegroundWindow())
			{
				status = WINDOWSTATUS::STATUS_FOREGROUND;
			}
			else
			{
				status = WINDOWSTATUS::STATUS_BACKGROUND;
			}
		}
	}
	return status;
}

const HWND CScreenWindowController::GetWindowHandle()
{
	//CriticalSectionLock lock(m_handleCS);

	return m_hScreenWindowHandle;
}

void CScreenWindowController::RefreshWindowHandle()
{
	//CriticalSectionLock lock(m_handleCS);

	ReleaseAllHDC();
	if (m_ulProcessID == 0)
	{
		SCERROR("CScreenWindowController::RefreshWindowHandle() m_ulProcessID = 0");
		m_hScreenWindowHandle = NULL;
	}
	else
	{
		if (ConfiginitializerInstance::GetInstance()->CalibrationConfig()->GetWindowName()
			|| ConfiginitializerInstance::GetInstance()->CalibrationConfig()->GetWindowClass())
		{
			string strWindowName = ConfiginitializerInstance::GetInstance()->CalibrationConfig()->GetWindowName() == NULL ? "" : ConfiginitializerInstance::GetInstance()->CalibrationConfig()->GetWindowName();
			string strWindowClass = ConfiginitializerInstance::GetInstance()->CalibrationConfig()->GetWindowClass() == NULL ? "" : ConfiginitializerInstance::GetInstance()->CalibrationConfig()->GetWindowClass();

			m_hScreenWindowHandle = GetProcessWnd(m_ulProcessID,
				strWindowName,
				strWindowClass);
			CheckMinimizeBox();
			RefreshAllHDC();
			if (m_hScreenWindowHandle != NULL)
			{
				SCINFO("CScreenWindowController::RefreshWindowHandle() HANDLE = %x name = %s classname = %s PID = %d", m_hScreenWindowHandle, strWindowName.c_str(), strWindowClass.c_str(), m_ulProcessID);
			}
			else
			{
				SCERROR("CScreenWindowController::RefreshWindowHandle() HANDLE = NULL name = %s classname = %s PID=%d", strWindowName.c_str(), strWindowClass.c_str(), m_ulProcessID);
			}
		}
		else
		{
			SCERROR("CScreenWindowController::RefreshWindowHandle() Name and ClassName = 0");
		}
	}
}

WINDOWRESOLUTION CScreenWindowController::GetWindowClientResolution()
{
	//CriticalSectionLock lock(m_handleCS);

	WINDOWRESOLUTION resolution;
	resolution.height = 0;
	resolution.width = 0;
	HWND handle = NULL;
	if (ConfiginitializerInstance::GetInstance()->CalibrationConfig()->GetBackgroundCaptureType() == 3)
	{
		handle = GetDesktopWindow();
	}
	else
	{
		handle = m_hScreenWindowHandle;
	}
	//if (m_hScreenWindowHandle != NULL)
	if (handle != NULL)
	{
		RECT rect;
		//if (::GetWindowRect(m_hScreenWindowHandle, &rect))
		if (::GetWindowRect(handle, &rect))
		{
			resolution.width = rect.right - rect.left;
			resolution.height = rect.bottom - rect.top;
		}
	}
	return resolution;
}

HBITMAP CScreenWindowController::GetScreenShot()
{
	//CriticalSectionLock lock(m_handleCS);

	RefreshWindowHandle();

	HBITMAP hbmRes = NULL;
	WINDOWSTATUS status = GetWindowStatus();
	if (status == WINDOWSTATUS::STATUS_FOREGROUND)
	{
		if (CaptureForgegroudWindow())
		{
			hbmRes = m_hbmScreen;
		}
	}
	else if (status == WINDOWSTATUS::STATUS_BACKGROUND)
	{
		if (ConfiginitializerInstance::GetInstance()->CalibrationConfig()->GetBackgroundCaptureType() == 0)		//HOOK方式进行截图
		{
			CBitmapEx bitmap;
			if (CaptureBackgroudWindowByHook(bitmap))
			{
				DeleteObject(m_hbmScreen);
				m_hbmScreen = NULL;
				bitmap.Save(m_hbmScreen);
			}
		}
		else if (ConfiginitializerInstance::GetInstance()->CalibrationConfig()->GetBackgroundCaptureType() == 1)	//Print方式截图
		{
			if (CaptureBackgroudWindow())
			{
				hbmRes = m_hbmScreen;
			}
		}
		else if (ConfiginitializerInstance::GetInstance()->CalibrationConfig()->GetBackgroundCaptureType() == 3)	//截取桌面方式截图
		{
			if (CaptureForgegroudWindow())
			{
				CBitmapEx tmp;
				tmp.Load(m_hbmScreen);

				//计算当前截图与Base截图的差异比例
				//当变化比例超过阈值（默认10）时，抛弃当前截图，小于等于阈值时，则正常使用
				int scale = DiffScale(tmp);
				if (scale <= m_ScaleThreshold)
				{
					hbmRes = m_hbmScreen;
				}

			}
		}
		else if (ConfiginitializerInstance::GetInstance()->CalibrationConfig()->GetBackgroundCaptureType() == 2)	//取消后端显示时截图
		{
			hbmRes = NULL;
		}
		
	}
	else if (status == WINDOWSTATUS::STATUS_MINIMIZE)
	{
		hbmRes = NULL;
	}
	else
	{
		SCERROR("CScreenWindowController::GetScreenShot() STATUS_UNKNOWN");
		hbmRes = NULL;
	}

	return hbmRes;
}

bool CScreenWindowController::GetDesktopScreen(CBitmapEx& bitmap)
{
	bool bRes = false;

	HWND hWndDesk = GetDesktopWindow();
	HDC hdcDesk = GetWindowDC(hWndDesk);
	HDC hdcMemDC = CreateCompatibleDC(hdcDesk);
	int width, height;
	RECT rect;
	GetWindowRect(hWndDesk, &rect);
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
		ReleaseDC(hWndDesk, hdcDesk);
		hdcDesk = NULL;
	}


	return bRes;
}

typedef struct _tagWNDINFO
{
	DWORD dwProcessId;
	string strWindowText;
	string strWindowClassName;
	HWND hWnd;
} WNDINFO, *LPWNDINFO;

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	LPWNDINFO pInfo = (LPWNDINFO)lParam;
	DWORD dwProcessId = 0;
	GetWindowThreadProcessId(hWnd, &dwProcessId);
	char szTextA[512] = { 0 };
	::GetWindowTextA(hWnd, szTextA, 512);
	char szClassNameA[512] = { 0 };
	::GetClassNameA(hWnd, szClassNameA, 512);
	if (dwProcessId == pInfo->dwProcessId
		&& (pInfo->strWindowText.empty() || pInfo->strWindowText.compare(szTextA) == 0)
		&& (pInfo->strWindowClassName.empty() || pInfo->strWindowText.compare(szTextA) == 0) )
	{
		pInfo->hWnd = hWnd;
		return FALSE;
	}
	return TRUE;
}

HWND CScreenWindowController::GetProcessWnd(unsigned long dwProcessId, string strWindowText, string strWindowClassName)
{
	HWND hWndRes = NULL;

	HWND hWnd = ::FindWindowA(strWindowClassName.c_str(), strWindowText.c_str());
	if (hWnd != NULL)
	{
		DWORD dwWndProcessId = 0;
		GetWindowThreadProcessId(hWnd, &dwWndProcessId);
		if (dwProcessId == dwWndProcessId)
		{
			hWndRes = hWnd;
		}
	}
	if (hWndRes == NULL)
	{
		WNDINFO wi;
		wi.dwProcessId = dwProcessId;
		wi.strWindowText = strWindowText;
		wi.strWindowClassName = strWindowClassName;
		wi.hWnd = NULL;
		EnumWindows(EnumWindowsProc, (LPARAM)&wi);
		if (wi.hWnd != NULL)
		{
			hWndRes = wi.hWnd;
		}
	}

	if (hWndRes == NULL)
	{
		hWnd = ::FindWindowA(strWindowClassName.c_str(), strWindowText.c_str());
		if (hWnd != NULL)
		{
			hWndRes = hWnd;
		}
	}

	return hWndRes;
}

void CScreenWindowController::ReleaseAllHDC()
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
		ReleaseDC(m_hScreenWindowHandle, m_hdcWindow);
		m_hdcMemDC = NULL;
	}
}

void CScreenWindowController::RefreshAllHDC()
{
	ReleaseAllHDC();

	HWND hWnd = NULL;
	if (ConfiginitializerInstance::GetInstance()->CalibrationConfig()->GetBackgroundCaptureType() == 3)
	{
		hWnd = GetDesktopWindow();
	}
	else
	{
		hWnd = m_hScreenWindowHandle;
	}

	m_hdcWindow = GetWindowDC(hWnd);
	m_hdcMemDC = CreateCompatibleDC(m_hdcWindow);

	m_LastResolution = GetWindowClientResolution();

	m_hbmScreen = CreateCompatibleBitmap(m_hdcWindow, m_LastResolution.width, m_LastResolution.height);
}

bool CScreenWindowController::CaptureBackgroudWindow()
{
	bool bRes = false;

	if (m_hdcMemDC)
	{
		WINDOWRESOLUTION resolution = GetWindowClientResolution();
		if (resolution.width != m_LastResolution.width || resolution.height != m_LastResolution.height)
		{
			RefreshAllHDC();
		}

		if (m_hbmScreen)
		{
			// Select the compatible bitmap into the compatible memory DC.
			HGDIOBJ obj = SelectObject(m_hdcMemDC, m_hbmScreen);

			// Bit block transfer into our compatible memory DC.
			if (PrintWindow(m_hScreenWindowHandle, m_hdcMemDC, 0))
			{
				bRes = true;
				BitBlt(m_hdcWindow, 0, 0, resolution.width, resolution.height, m_hdcMemDC, 0, 0, SRCCOPY);
				RECT rect;
				GetWindowRect(m_hScreenWindowHandle, &rect);

				RedrawWindow(m_hScreenWindowHandle, &rect, NULL, RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN | RDW_UPDATENOW);
			}

			SelectObject(m_hdcMemDC, obj);
		}
	}

	return bRes;
}



#define DLL_CAPTURE_EVENT_NAME  TEXT("Global\\1E485DD3-7A31-418D-981A-927A7535D56F}")
#define DLL_CAPTURE_DONE_EVENT_NAME  TEXT("Global\\1E485DD3-7A31-418D-981A-927A7535D56G}")
#define DLL_CAPTURE_FILEMAPPING_NAME _T("DLL_CAPTURE_FILEMAPPING")

bool CScreenWindowController::CaptureBackgroudWindowByHook(CBitmapEx& bitmap)
{
	bool bRes = false;
	bitmap.Clear();

	HANDLE h_CaptureEvent = ::OpenEvent(EVENT_ALL_ACCESS, FALSE, DLL_CAPTURE_EVENT_NAME);
	HANDLE h_CaptureDoneEvent = ::OpenEvent(EVENT_ALL_ACCESS, FALSE, DLL_CAPTURE_DONE_EVENT_NAME);

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

	return bRes;
}

void CScreenWindowController::CheckMinimizeBox()
{
	if (false)
	{
		::SetWindowLong(m_hScreenWindowHandle, GWL_STYLE, GetWindowLong(m_hScreenWindowHandle, GWL_STYLE) &~WS_MINIMIZEBOX);
	}
}

int CScreenWindowController::DiffScale(CBitmapEx& tmp)
{
	int res = 100;

	if (m_bmpScaleBase.IsValid())
	{
		DWORD* buf1 = (DWORD*)tmp.GetData();
		DWORD* buf2 = (DWORD*)m_bmpScaleBase.GetData();


		DWORD len = m_bmpScaleBase.GetWidth()*m_bmpScaleBase.GetHeight();
		DWORD xor = 0;
		for (DWORD dw = 0; dw < len; ++dw)
		{
			((buf1[dw] ^ buf2[dw]) & 0x00FFFFFF) != 0 ? ++xor : xor;
		}

		res = xor * 100 / len;
	}

	return res;
}

bool CScreenWindowController::CaptureForgegroudWindow()
{
	bool bRes = false;

	if (m_hdcMemDC)
	{
		WINDOWRESOLUTION resolution = GetWindowClientResolution();
		if (resolution.width != m_LastResolution.width || resolution.height != m_LastResolution.height)
		{
			RefreshAllHDC();
		}

		if (m_hbmScreen)
		{
			// Select the compatible bitmap into the compatible memory DC.
			HGDIOBJ obj = SelectObject(m_hdcMemDC, m_hbmScreen);

			// Bit block transfer into our compatible memory DC.
			if (BitBlt(m_hdcMemDC,
				0, 0,
				m_LastResolution.width, m_LastResolution.height,
				m_hdcWindow,
				0, 0,
				SRCCOPY))
			{
				bRes = true;
			}

			SelectObject(m_hdcMemDC, obj);
		}
	}

	return bRes;
}
