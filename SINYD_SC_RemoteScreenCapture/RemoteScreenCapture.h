#pragma once
#include "ThreadUtil.h"
#include <string>

using namespace std;

typedef struct stWindowResolution
{
	unsigned long width;
	unsigned long height;
}WINDOWRESOLUTION, *PWINDOWRESOLUTION;

class CRemoteScreenCapture : public CThreadBase
{
public:
	CRemoteScreenCapture();
	virtual ~CRemoteScreenCapture();


protected:
	virtual DWORD Execute();

	HWND GetProcessWnd(unsigned long dwProcessId, string strWindowText, string strWindowClassName);
	void ReleaseAllHDC();
	WINDOWRESOLUTION GetWindowClientResolution();
	void RefreshAllHDC(HWND hWnd);

	static BOOL CALLBACK DrawChildProc(HWND hWnd, LPARAM lParam);

	void GetScreenShot();
	void SaveBitmapToFile(LPTSTR path);
	void SaveBitmapToMapping();

private:
	HWND m_hScreenWindowHandle;
	HBITMAP m_hbmScreen;
	HDC m_hdcMemDC;
	HDC m_hdcWindow;
	WINDOWRESOLUTION m_LastResolution;

	HHOOK m_CallWndprocHHook;
	static LONG m_OldWndProc;
	static bool m_bMyPrintMsg;

	HANDLE m_fileMapping;
	int m_fileMappingW;
	int m_fileMappingH;
};

