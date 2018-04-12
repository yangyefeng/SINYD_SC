#pragma once
#include <Windows.h>

class CPaintHook
{
	static HDC  WINAPI MyBeginPaint(HWND hWnd, LPPAINTSTRUCT lpPaint);
	static BOOL WINAPI MyEndPaint(HWND hWnd, LPPAINTSTRUCT lpPaint);

	static LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static bool   m_bSettedMyWndProc;
	static WNDPROC   m_OldWndProc;
	static HDC       m_hDC;

public:
	CPaintHook();
	~CPaintHook();
	static bool Within_WM_PRINT(void)
	{
		//if ((m_thunk[0] == 0xB9) && ((*(unsigned *)(m_thunk + 5)) == 0x20FF018B))
		if (m_bSettedMyWndProc)
			return m_hDC != 0;
		else
			return false;
	}

	void SubClass(HWND hWnd);
};

