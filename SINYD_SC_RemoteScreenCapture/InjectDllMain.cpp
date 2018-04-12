#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "RemoteScreenCapture.h"

CRemoteScreenCapture* pCapture = NULL;
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		//MessageBox(NULL, L"111", L"", MB_OK);
		if (!pCapture)
		{
			pCapture = new CRemoteScreenCapture();
		}
		if (pCapture)
		{
			pCapture->Start();
		}
		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
	{
		if (pCapture)
		{
			pCapture->Stop();
			delete pCapture;
			pCapture = NULL;
		}
		break;
	}
	}
	return TRUE;
}
