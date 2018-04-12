
// SINYD_SC_keyGen.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CSINYD_SC_keyGenApp: 
// 有关此类的实现，请参阅 SINYD_SC_keyGen.cpp
//

class CSINYD_SC_keyGenApp : public CWinApp
{
public:
	CSINYD_SC_keyGenApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CSINYD_SC_keyGenApp theApp;