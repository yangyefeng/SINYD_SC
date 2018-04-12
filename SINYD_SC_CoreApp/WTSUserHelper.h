/////////////////////////////////////////////////////////////////////////////
/// @File name: WTSUserHelper.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 		
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include <WtsApi32.h>
#include <Windows.h>
#include <atlbase.h>

#pragma comment(lib, "Wtsapi32.lib")

class WTSSessionHolder
{
	DWORD nCount;
	WTS_SESSION_INFO* pSessions;

	WTSSessionHolder(const WTSSessionHolder&);
	WTSSessionHolder& operator=(const WTSSessionHolder&);

public:
	WTSSessionHolder()
		: nCount(0), pSessions(NULL)
	{
		if (!::WTSEnumerateSessions(WTS_CURRENT_SERVER_HANDLE, 0, 1, &pSessions, &nCount))
		{
		}
	}

	~WTSSessionHolder()
	{
		::WTSFreeMemory(pSessions);
	}

	typedef const WTS_SESSION_INFO* const_iterator;

	const WTS_SESSION_INFO* begin() const
	{
		return pSessions;
	}

	const WTS_SESSION_INFO* end() const
	{
		return pSessions + nCount;
	}
};

class WTSUserHolder
{
	CHandle m_hUserToken;
	BOOL bImpersonated;

	WTSUserHolder(const WTSUserHolder&);
	WTSUserHolder& operator=(const WTSUserHolder&);

	static inline HANDLE GetSessionUserToken()
	{
		DWORD dwSessionID = ::WTSGetActiveConsoleSessionId();
		if (dwSessionID != 0xFFFFFFFF)
		{
			CHandle hUserToken;
			if (::WTSQueryUserToken(dwSessionID, &hUserToken.m_h))
			{
				return hUserToken.Detach();
			}
		}
		WTSSessionHolder sessions;
		WTSSessionHolder::const_iterator end = sessions.end();
		for (WTSSessionHolder::const_iterator i = sessions.begin(); i != end; ++i)
		{
			CHandle hUserToken;
			if (::WTSQueryUserToken(i->SessionId, &hUserToken.m_h))
			{
				return hUserToken.Detach();
			}
		}
		return NULL;
	}

public:
	WTSUserHolder()
		: m_hUserToken(), bImpersonated(FALSE)
	{
	}

	~WTSUserHolder()
	{
		UnImpersonate();
	}

	BOOL AttachCurrentUser()
	{
		CHandle hUserToken(GetSessionUserToken());
		m_hUserToken = hUserToken;
		return m_hUserToken ? TRUE : FALSE;
	}

	HANDLE GetUserToken() const
	{
		return m_hUserToken;
	}

	BOOL Impersonate()
	{
		if (!bImpersonated)
		{
			bImpersonated = ::ImpersonateLoggedOnUser(m_hUserToken);
		}
		return bImpersonated;
	}

	void UnImpersonate()
	{
		if (bImpersonated && ::RevertToSelf())
		{
			bImpersonated = FALSE;
		}
	}

	BOOL IsImpersonated() const
	{
		return bImpersonated;
	}
};
