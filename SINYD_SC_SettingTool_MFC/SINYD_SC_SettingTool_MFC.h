
// SINYD_SC_SettingTool_MFC.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// SCSettingToolApp: 
// �йش����ʵ�֣������ SINYD_SC_SettingTool_MFC.cpp
//

class SCSettingToolApp : public CWinApp
{
public:
	SCSettingToolApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern SCSettingToolApp theApp;