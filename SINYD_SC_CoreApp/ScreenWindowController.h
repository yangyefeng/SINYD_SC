/////////////////////////////////////////////////////////////////////////////
/// @File name: ScreenWindowController.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 		��ͼ���ڿ��ƶ���
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "windows.h"
#include <string>
#include "..\SINYD_SC_CommonUtility\BitmapEx.h"
#include "ThreadUtil.h"
using namespace std;

typedef enum enWindowStatus
{
	STATUS_UNKNOWN,
	STATUS_FOREGROUND,
	STATUS_BACKGROUND,
	STATUS_MINIMIZE
}WINDOWSTATUS;

typedef struct stWindowResolution
{
	unsigned long width;
	unsigned long height;
}WINDOWRESOLUTION, *PWINDOWRESOLUTION;

class CScreenWindowController
{
public:
	CScreenWindowController();
	virtual ~CScreenWindowController();

	///*******************************************************
	/// @name:   CScreenWindowController::SetProcessID
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][unsigned long]ulProcessID
	/// @brief:  ����Ŀ�����ID
	///*******************************************************
	void SetProcessID(unsigned long ulProcessID);

	///*******************************************************
	/// @name:   CScreenWindowController::GetWindowStatus
	/// @author: YaoDi
	/// @return: WINDOWSTATUS
	/// @brief:  ��ȡ����״̬
	///*******************************************************
	WINDOWSTATUS GetWindowStatus();
	///*******************************************************
	/// @name:   CScreenWindowController::GetWindowHandle
	/// @author: YaoDi
	/// @return: const HWND
	/// @brief:  ��ȡ���ھ��
	///*******************************************************
	const HWND GetWindowHandle();

	///*******************************************************
	/// @name:   CScreenWindowController::RefreshWindowHandle
	/// @author: YaoDi
	/// @return: 
	/// @brief:  ˢ�´��ھ��
	///*******************************************************
	void RefreshWindowHandle();

	///*******************************************************
	/// @name:   CScreenWindowController::GetWindowClientResolution
	/// @author: YaoDi
	/// @return: WINDOWRESOLUTION
	/// @brief:  ��ȡ���ڿ�߷ֱ��ʣ������߿�
	///*******************************************************
	WINDOWRESOLUTION GetWindowClientResolution();
	///*******************************************************
	/// @name:   CScreenWindowController::GetScreenShot
	/// @author: YaoDi
	/// @return: HBITMAP
	/// @brief:  ��ȡ��ͼ���
	///*******************************************************
	HBITMAP GetScreenShot();
	bool GetDesktopScreen(CBitmapEx & bitmap);
private:
	HWND GetProcessWnd(unsigned long ulProcessID, string strWindowText, string strWindowClassName);
	void ReleaseAllHDC();
	void RefreshAllHDC();
	bool CaptureForgegroudWindow();
	bool CaptureBackgroudWindow();
	bool CaptureBackgroudWindowByHook(CBitmapEx & bitmap);
	void CheckMinimizeBox();
	int DiffScale(CBitmapEx& tmp);
private:
	unsigned long m_ulProcessID;
	HWND m_hScreenWindowHandle;
	HDC m_hdcWindow;
	HDC m_hdcMemDC;
	HBITMAP m_hbmScreen;
	WINDOWRESOLUTION m_LastResolution;

	CCriticalSection m_handleCS;
	CBitmapEx m_bmpScaleBase;
	int m_ScaleThreshold;
};

