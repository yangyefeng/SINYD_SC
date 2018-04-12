#include "RemoteScreenCapture.h"
#include "..\SINYD_SC_CommonUtility\BitmapEx.h"
#include <string>
#include "..\SINYD_SC_CommonUtility\CSettingConfig.h"
#include <imagehlp.h>
#include "..\SINYD_SC_CommonUtility\CCalibrationConfig.h"
#include <atlbase.h>
#include "Painthook.h"

using namespace std;
#pragma comment(lib, "imagehlp.lib")

#define DLL_CAPTURE_EVENT_NAME  _T("Global\\1E485DD3-7A31-418D-981A-927A7535D56F}")
#define DLL_CAPTURE_DONE_EVENT_NAME  _T("Global\\1E485DD3-7A31-418D-981A-927A7535D56G}")
#define DLL_CAPTURE_FILEMAPPING_NAME _T("DLL_CAPTURE_FILEMAPPING")

CRemoteScreenCapture::CRemoteScreenCapture()
	: m_hScreenWindowHandle(NULL)
	, m_CallWndprocHHook(NULL)
	, m_fileMapping(NULL)
	, m_fileMappingW(0)
	, m_fileMappingH(0)
{
}

CRemoteScreenCapture::~CRemoteScreenCapture()
{
	ReleaseAllHDC();
	if (m_fileMapping)
	{
		CloseHandle(m_fileMapping);
	}}

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
		&& (pInfo->strWindowClassName.empty() || pInfo->strWindowText.compare(szTextA) == 0))
	{
		pInfo->hWnd = hWnd;
		return FALSE;
	}
	return TRUE;
}

HWND CRemoteScreenCapture::GetProcessWnd(unsigned long dwProcessId, string strWindowText, string strWindowClassName)
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

	return hWndRes;
}

void CRemoteScreenCapture::ReleaseAllHDC()
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

WINDOWRESOLUTION CRemoteScreenCapture::GetWindowClientResolution()
{
	WINDOWRESOLUTION resolution;
	resolution.height = 0;
	resolution.width = 0;
	if (m_hScreenWindowHandle != NULL)
	{
		RECT rect;
		if (::GetWindowRect(m_hScreenWindowHandle, &rect))
		{
			resolution.width = rect.right - rect.left;
			resolution.height = rect.bottom - rect.top;
		}
	}
	return resolution;
}

void CRemoteScreenCapture::RefreshAllHDC(HWND hWnd)
{
	ReleaseAllHDC();

	m_hdcMemDC = CreateCompatibleDC(NULL);
	m_hdcWindow = GetWindowDC(hWnd);
	m_LastResolution = GetWindowClientResolution();
	m_hbmScreen = CreateCompatibleBitmap(m_hdcWindow, m_LastResolution.width, m_LastResolution.height);
}

static int name = 0;
BOOL CALLBACK CRemoteScreenCapture::DrawChildProc(HWND hWnd, LPARAM lParam)
{
	CRemoteScreenCapture* pThis = (CRemoteScreenCapture*)lParam;

	WINDOWPLACEMENT wpl;
	wpl.length = sizeof(WINDOWPLACEMENT);
	RECT rectChild;
	GetWindowRect(hWnd, &rectChild);
	if ((rectChild.right - rectChild.left) > 0 && (rectChild.bottom - rectChild.top) > 0)
	{
		RECT rectParent;
		GetWindowRect(pThis->m_hScreenWindowHandle, &rectParent);

		//计算子窗体相对根窗体的坐标
		int x, y;
		x= rectChild.left - rectParent.left;
		y = rectChild.top - rectParent.top;

		HDC hDCMem = CreateCompatibleDC(NULL);
		HBITMAP hBmp = NULL;
		{
			HDC hDC = GetDC(hWnd);
			hBmp = CreateCompatibleBitmap(hDC, rectChild.right - rectChild.left, rectChild.bottom - rectChild.top);
			ReleaseDC(hWnd, hDC);
		}

		HGDIOBJ hOld = SelectObject(hDCMem, hBmp);
		SendMessage(hWnd, WM_PRINT, (WPARAM)hDCMem, PRF_CHILDREN | PRF_CLIENT | PRF_ERASEBKGND | PRF_NONCLIENT | PRF_OWNED);
		BitBlt(pThis->m_hdcMemDC, x, y, rectChild.right - rectChild.left, rectChild.bottom - rectChild.top, hDCMem, 0, 0, SRCCOPY);

// 		CBitmapEx bitmap;
// 		bitmap.Load(hBmp);
// 		char cname[100] = {0};
// 		char cnamePath[100] = { 0 };
// 		itoa(name, cname, 10);
// 		sprintf(cnamePath, "D:\\%s.bmp", cname);
// 		bitmap.Save(CA2W(cnamePath));
// 		name++;

		SelectObject(hDCMem, hOld);

		DeleteObject(hBmp);
		DeleteObject(hDCMem);
	}

	return TRUE;
}

void CRemoteScreenCapture::GetScreenShot()
{

	if (m_hdcMemDC)
	{
		WINDOWRESOLUTION resolution = GetWindowClientResolution();
		if (resolution.width != m_LastResolution.width || resolution.height != m_LastResolution.height)
		{
			RefreshAllHDC(m_hScreenWindowHandle);
		}

		if (m_hbmScreen)
		{
			// Select the compatible bitmap into the compatible memory DC.
			HGDIOBJ obj = SelectObject(m_hdcMemDC, m_hbmScreen);

			SendMessage(m_hScreenWindowHandle, WM_PRINT, (WPARAM)m_hdcMemDC, PRF_CHILDREN | PRF_CLIENT | PRF_ERASEBKGND | PRF_NONCLIENT | PRF_OWNED);
			EnumChildWindows(m_hScreenWindowHandle, DrawChildProc, (LPARAM)this);
			name = 0;

			SelectObject(m_hdcMemDC, obj);
		}
	}
}

void CRemoteScreenCapture::SaveBitmapToFile(LPTSTR path)
{
	CBitmapEx bitmap;
	bitmap.Load(m_hbmScreen);
	if (bitmap.IsValid())
	{
		bitmap.Save(path);
	}
}

void CRemoteScreenCapture::SaveBitmapToMapping()
{
	if (!m_fileMapping)
	{
		CBitmapEx tmp;
		tmp.Create(m_fileMappingW, m_fileMappingH);
		m_fileMapping = ::CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, tmp.GetFileInfo()->bfSize*sizeof(BYTE), DLL_CAPTURE_FILEMAPPING_NAME);
	}

	if (m_fileMapping)
	{
		LPVOID pBuffer = ::MapViewOfFile(m_fileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		
		CBitmapEx bitmap;
		bitmap.Load(m_hbmScreen);
		if (bitmap.IsValid())
		{
			if (bitmap.GetWidth() == m_fileMappingW && bitmap.GetHeight() == m_fileMappingH)
			{
				bitmap.Save((LPBYTE)pBuffer);
			}
		}
		::UnmapViewOfFile(pBuffer);
	}
	
}

DWORD CRemoteScreenCapture::Execute()
{
	//m_CallWndprocHHook = SetWindowsHookEx(WH_CALLWNDPROC, CRemoteScreenCapture::CallWndProc, GetModuleHandle(_T("SINYD_SC_RemoteScreenCapture.dll")), GetCurrentThreadId());
	//m_OldWndProc = GetWindowLong(m_hScreenWindowHandle, GWL_WNDPROC);
	//SetWindowLong(m_hScreenWindowHandle, GWL_WNDPROC, (LONG)CRemoteScreenCapture::SubclassWndProc);

	CPaintHook hook;

	char fileFullPath[MAX_PATH] = "";
	::GetModuleFileNameA(::GetModuleHandleA("SINYD_SC_RemoteScreenCapture.dll"), fileFullPath, MAX_PATH);

	std::string path(fileFullPath);
	path = path.erase(path.find_last_of('\\'));
	char cpathend = *(path.crbegin());
	if (cpathend != '\\')
	{
		path = path + '\\';
	}

	CCalibrationConfig caliConf;
	caliConf.ReloadConfig(path + "ConfigFile\\SCCalibrationConfig.xml");

	m_fileMappingW = caliConf.GetWindowWidth();
	m_fileMappingH = caliConf.GetWindowHeight();

	const char* pname = caliConf.GetWindowName();
	const char* pclass = caliConf.GetWindowClass();
	if (pname == NULL && pclass == NULL)
	{
		return -1;
	}
	string windowName = pname == NULL ? "" : pname;
	string windowClassName = pclass == NULL ? "" : pclass;

	path = path + "ConfigFile_RunTime\\currentScreen.bmp";

	::MakeSureDirectoryPathExists(path.c_str());


	TCHAR bmpFilePath[MAX_PATH] = {0}; 
	memcpy(bmpFilePath, CA2W(path.c_str()).m_psz, path.size() < MAX_PATH ? (path.size()*sizeof(TCHAR)) : (MAX_PATH*sizeof(TCHAR)));

	HANDLE h_CaptureEvent = ::CreateEvent(NULL, FALSE, FALSE, DLL_CAPTURE_EVENT_NAME);
	HANDLE h_CaptureDoneEvent = ::CreateEvent(NULL, FALSE, FALSE, DLL_CAPTURE_DONE_EVENT_NAME);

	const DWORD dwHandleCount = 2;
	HANDLE handles[dwHandleCount] = { m_hStopEvent, h_CaptureEvent };
	while (true)
	{
		if (m_hScreenWindowHandle == NULL)
		{
			m_hScreenWindowHandle = GetProcessWnd(GetCurrentProcessId(), windowName, windowClassName);
			if (m_hScreenWindowHandle == NULL)
			{
				Sleep(200);
				continue;
			}
			RefreshAllHDC(m_hScreenWindowHandle);

			hook.SubClass(m_hScreenWindowHandle);
		}




		DWORD dw = ::WaitForMultipleObjects(dwHandleCount, handles, FALSE, INFINITE);
		if (WAIT_OBJECT_0 == dw)
		{
			break;
		}
		else if (WAIT_OBJECT_0 + 1 == dw)
		{
			GetScreenShot();
			//SaveBitmapToFile(bmpFilePath);
			if (m_LastResolution.width == m_fileMappingW && m_LastResolution.height == m_fileMappingH)
			{
				SaveBitmapToMapping();
			}

			::SetEvent(h_CaptureDoneEvent);
		}
		else
		{
			continue;
		}
	}

	CloseHandle(h_CaptureEvent);
	h_CaptureEvent = NULL;
	CloseHandle(h_CaptureDoneEvent);
	h_CaptureDoneEvent = NULL;


	//SetWindowLong(m_hScreenWindowHandle, GWL_WNDPROC, m_OldWndProc);
	//UnhookWindowsHookEx(m_CallWndprocHHook);
	//m_CallWndprocHHook = NULL;


	return 0;
}
