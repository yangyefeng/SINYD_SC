
// SINYD_SC_keyGen.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSINYD_SC_keyGenApp: 
// �йش����ʵ�֣������ SINYD_SC_keyGen.cpp
//

class CSINYD_SC_keyGenApp : public CWinApp
{
public:
	CSINYD_SC_keyGenApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSINYD_SC_keyGenApp theApp;