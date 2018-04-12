#pragma once
#include "ThreadUtil.h"

class CMainWorkThread : public CThreadBase
{
public:
	CMainWorkThread();
	virtual ~CMainWorkThread();

	void Restart();
	void SetIsSaveCaliPNG(bool bSave);

protected:
	bool StopCoreAppAndCloseAutoStart(int stat);
	virtual DWORD Execute();

private:
	void SetCurrentDirectoryToInstallDir();

private:
	HANDLE m_hRestartEvent;
};

