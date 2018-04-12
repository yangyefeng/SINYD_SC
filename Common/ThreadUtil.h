#pragma once


#include <windows.h>

template<class T>
class CLock
{
	T & section;

public:

	CLock(T & t) : section(t) { section.Enter(); }

	~CLock() { section.Leave(); }
};

class CCriticalSection
{
	CRITICAL_SECTION m_cs;

public:
	CCriticalSection()
	{
		InitializeCriticalSection(&m_cs);
	}

	~CCriticalSection()
	{
		DeleteCriticalSection(&m_cs);
	}

private:
	friend CLock < CCriticalSection >;

	void Enter() { EnterCriticalSection(&m_cs); }
	void Leave() { LeaveCriticalSection(&m_cs); }
};

typedef CLock<CCriticalSection> CriticalSectionLock;


template<class T>
class CSingleton
{
public:
	static T* GetInstance();
	static void ReleaseInstance();

private:
	CSingleton(CSingleton const&);
	CSingleton& operator=(CSingleton const&);

private:
	static T* m_pInstance;
	static CCriticalSection m_cs;

	CSingleton(){}
	~CSingleton(){}
};

template<class T>
T* CSingleton<T>::m_pInstance = nullptr;
template<class T>
CCriticalSection CSingleton<T>::m_cs;

template<class T>
T* CSingleton<T>::GetInstance()
{
	if (!m_pInstance)
	{
		CriticalSectionLock lock(m_cs);
		if (!m_pInstance)
		{
			CSingleton<T>::m_pInstance = new T();
		}
	}

	return m_pInstance;
}

template<class T>
void CSingleton<T>::ReleaseInstance()
{
	CriticalSectionLock lock(m_cs);
	if (!!m_pInstance)
	{
		delete CSingleton<T>::m_pInstance;
		CSingleton<T>::m_pInstance = 0;
	}
}

#define DECLARE_SINGLETON_CLASS(type) friend class CSingleton<type>;

const int THREAD_STOP_WAIT_TIME_DEFAULT = 2 * 1000;

class CThreadBase
{
	CThreadBase(CThreadBase const&);
	CThreadBase& operator=(CThreadBase const&);

public:
	CThreadBase()
	: m_hThreadHanlder(nullptr)
	, m_dwThreadID(0)
	, m_hStopEvent(nullptr)
	{
		m_hStopEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
	}
	virtual ~CThreadBase()
	{
		::CloseHandle(m_hStopEvent);
		m_hStopEvent = nullptr;
	}

	DWORD Start(DWORD dwCreationFlags = 0)
	{
		return StartImpl(dwCreationFlags);
	}
	void Stop(int nWaitTime = THREAD_STOP_WAIT_TIME_DEFAULT)
	{
		return StopImpl(nWaitTime);
	}
	DWORD Suspend()
	{
		return SuspendImpl();
	}
	DWORD Resume()
	{
		return ResumeImpl();
	}
	bool IsRunning(int nWaitTime = 0)
	{
		return IsRunningImpl(nWaitTime);
	}

protected:
	virtual DWORD StartImpl(DWORD dwCreationFlags)
	{
		DWORD dwRes = 0;
		if (nullptr == m_hThreadHanlder)
		{
			::ResetEvent(m_hStopEvent);
			m_hThreadHanlder = ::CreateThread(nullptr, 0, ThreadFunc, (LPVOID)this, dwCreationFlags, &m_dwThreadID);
			if (!m_hThreadHanlder)
			{
				dwRes = -1;
			}
		}
		return dwRes;
	}
	virtual void StopImpl(int nWaitTime)
	{
		::SetEvent(m_hStopEvent);
		if (IsRunning(nWaitTime))
		{
			::TerminateThread(m_hThreadHanlder, 0);
		}
		::CloseHandle(m_hThreadHanlder);
		m_hThreadHanlder = nullptr;
	}
	virtual DWORD SuspendImpl()
	{
		DWORD dw = -1;
		if (m_hThreadHanlder)
		{
			dw = ::SuspendThread(m_hThreadHanlder);
		}
		return dw;
	}
	virtual DWORD ResumeImpl()
	{
		DWORD dw = -1;
		if (m_hThreadHanlder)
		{
			dw = ::ResumeThread(m_hThreadHanlder);
		}
		return dw;
	}
	virtual bool IsRunningImpl(int nWaitTime)
	{
		return ::WaitForSingleObject(m_hThreadHanlder, nWaitTime) == WAIT_TIMEOUT ? true : false;
	}

	virtual DWORD Execute() = 0;
	static DWORD WINAPI ThreadFunc(LPVOID lpParam)
	{
		if (nullptr == lpParam)
		{
			return -1;
		}

		CThreadBase* pThread = (CThreadBase*)lpParam;
		DWORD dw = pThread->Execute();

		if (pThread->m_hThreadHanlder)
		{
			CloseHandle(pThread->m_hThreadHanlder);
			pThread->m_hThreadHanlder = nullptr;
		}

		return dw;
	}

protected:
	HANDLE m_hThreadHanlder;
	DWORD m_dwThreadID;
	HANDLE m_hStopEvent;
};
