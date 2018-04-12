/////////////////////////////////////////////////////////////////////////////
/// @File name: ProcessNotification.cpp
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 		
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <comdef.h>
#include <atlcomcli.h>
#include "ProcessNotification.h"

#pragma comment(lib, "wbemuuid.lib")
#define _WIN32_DCOM
using namespace std;


EventSink::EventSink(CProcessNotification::TNotificationFunction callback)
	: m_lRef(0)
	, m_callback(callback)
{
}
EventSink::~EventSink()
{
}

void EventSink::CancelAsyncCall()
{
	if (pSvc && pStubSink)
	{
		pSvc->CancelAsyncCall(pStubSink);
	}
	pStubSink = nullptr;
	pSvc = nullptr;
}

ULONG STDMETHODCALLTYPE EventSink::AddRef()
{
	return InterlockedIncrement(&m_lRef);
}
ULONG STDMETHODCALLTYPE EventSink::Release()
{
	LONG lRef = InterlockedDecrement(&m_lRef);
	if (lRef == 0)
		delete this;
	return lRef;
}
HRESULT STDMETHODCALLTYPE EventSink::QueryInterface(REFIID riid, void** ppv)
{
	if (riid == IID_IUnknown || riid == IID_IWbemObjectSink)
	{
		*ppv = (IWbemObjectSink *) this;
		AddRef();
		return WBEM_S_NO_ERROR;
	}
	else return E_NOINTERFACE;
}

HRESULT STDMETHODCALLTYPE EventSink::Indicate(
	LONG lObjectCount,
	IWbemClassObject __RPC_FAR *__RPC_FAR *apObjArray
	)
{
	for (LONG lCount = 0; lCount < lObjectCount; lCount++)
	{
		if (m_callback(apObjArray[lCount]))
		{
// 			pSvc->CancelAsyncCall(pStubSink);
// 			pStubSink = nullptr;
// 			pSvc = nullptr;
			break;
		}
	}

	return WBEM_S_NO_ERROR;
}

HRESULT STDMETHODCALLTYPE EventSink::SetStatus(
	/* [in] */ LONG lFlags,
	/* [in] */ HRESULT hResult,
	/* [in] */ BSTR strParam,
	/* [in] */ IWbemClassObject __RPC_FAR *pObjParam
	)
{
	return WBEM_S_NO_ERROR;
}

CProcessNotification::CProcessNotification()
	: m_spCreationSink(nullptr)
	, m_spTerminationSink(nullptr)
{
}

CProcessNotification::~CProcessNotification()
{
}

int CProcessNotification::registerCreationCallback(TNotificationFunction callback)
{
	if (!m_spCreationSink)
	{
		m_spCreationSink = (new EventSink(callback));
	}
	else
	{
		m_spCreationSink->pSvc = nullptr;
		m_spCreationSink->pStubSink = nullptr;
	}

	CComPtr<IWbemLocator> pLoc;

	HRESULT hres = CoCreateInstance(
		CLSID_WbemLocator,
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator,
		(LPVOID*)&pLoc);

	if (FAILED(hres))
	{
		throw exception("ProcessTerminationNotificaiton initialization failed");
	}


	// Connect to the local root\cimv2 namespace
	// and obtain pointer pSvc to make IWbemServices calls.
	hres = pLoc->ConnectServer(
		_bstr_t(L"ROOT\\CIMV2"),
		NULL,
		NULL,
		0,
		NULL,
		0,
		0,
		&m_spCreationSink->pSvc
		);

	if (FAILED(hres))
	{
		throw exception("ProcessTerminationNotificaiton initialization failed");
	}

	hres = CoSetProxyBlanket(
		m_spCreationSink->pSvc,                        // Indicates the proxy to set
		RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx 
		RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx 
		NULL,                        // Server principal name 
		RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
		RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
		NULL,                        // client identity
		EOAC_NONE                    // proxy capabilities 
		);

	if (FAILED(hres))
	{
		throw exception("ProcessTerminationNotificaiton initialization failed");
	}

	// Use an unsecured apartment for security
	CComPtr<IUnsecuredApartment> pUnsecApp;

	hres = CoCreateInstance(CLSID_UnsecuredApartment, NULL,
		CLSCTX_LOCAL_SERVER, IID_IUnsecuredApartment,
		(void**)&pUnsecApp);

	CComPtr<IUnknown> pStubUnk;
	pUnsecApp->CreateObjectStub(m_spCreationSink, &pStubUnk);

	pStubUnk->QueryInterface(IID_IWbemObjectSink,
		(void **)&m_spCreationSink->pStubSink);

	// The ExecNotificationQueryAsync method will call
	// The EventQuery::Indicate method when an event occurs

	hres = m_spCreationSink->pSvc->ExecNotificationQueryAsync(
		_bstr_t("WQL"),
		_bstr_t("SELECT * "
			"FROM __InstanceCreationEvent WITHIN 1 "
			"WHERE TargetInstance ISA 'Win32_Process'"),
		WBEM_FLAG_SEND_STATUS,
		NULL,
		m_spCreationSink->pStubSink);

	// Check for errors.
	if (FAILED(hres))
	{
		throw std::exception("ProcessTerminationNotificaiton initialization failed");
	}

	return 0;
}

int CProcessNotification::unregisterCreationCallback()
{
	if (!m_spCreationSink)
	{
		return -1;
	}
	m_spCreationSink->CancelAsyncCall();
	m_spCreationSink = nullptr;
	return 0;
}

int CProcessNotification::registerTerminationCallback(TNotificationFunction callback/*, unsigned long processId*/)
{
	if (!m_spTerminationSink)
	{
		m_spTerminationSink = (new EventSink(callback));
	}
	else
	{
		m_spTerminationSink->pSvc = nullptr;
		m_spTerminationSink->pSvc = nullptr;
	}

	CComPtr<IWbemLocator> pLoc;

	HRESULT hres = CoCreateInstance(
		CLSID_WbemLocator,
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator,
		(LPVOID*)&pLoc);

	if (FAILED(hres))
	{
// 		cout << "Failed to create IWbemLocator object. "
// 			<< "Err code = 0x"
// 			<< hex << hres << endl;
		throw exception("ProcessTerminationNotificaiton initialization failed");
	}


	// Connect to the local root\cimv2 namespace
	// and obtain pointer pSvc to make IWbemServices calls.
	hres = pLoc->ConnectServer(
		_bstr_t(L"ROOT\\CIMV2"),
		NULL,
		NULL,
		0,
		NULL,
		0,
		0,
		&m_spTerminationSink->pSvc
		);

	if (FAILED(hres))
	{
// 		cout << "Could not connect. Error code = 0x"
// 			<< hex << hres << endl;
		throw exception("ProcessTerminationNotificaiton initialization failed");
	}

	hres = CoSetProxyBlanket(
		m_spTerminationSink->pSvc,                        // Indicates the proxy to set
		RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx 
		RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx 
		NULL,                        // Server principal name 
		RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
		RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
		NULL,                        // client identity
		EOAC_NONE                    // proxy capabilities 
		);

	if (FAILED(hres))
	{
// 		cout << "Could not set proxy blanket. Error code = 0x"
// 			<< hex << hres << endl;
		throw exception("ProcessTerminationNotificaiton initialization failed");
	}

	// Use an unsecured apartment for security
	CComPtr<IUnsecuredApartment> pUnsecApp;

	hres = CoCreateInstance(CLSID_UnsecuredApartment, NULL,
		CLSCTX_LOCAL_SERVER, IID_IUnsecuredApartment,
		(void**)&pUnsecApp);

	CComPtr<IUnknown> pStubUnk;
	pUnsecApp->CreateObjectStub(m_spTerminationSink, &pStubUnk);

	pStubUnk->QueryInterface(IID_IWbemObjectSink,
		(void **)&m_spTerminationSink->pStubSink);

	// The ExecNotificationQueryAsync method will call
	// The EventQuery::Indicate method when an event occurs
	char buffer[512];
// 	sprintf_s(buffer, "SELECT * "
// 		"FROM __InstanceDeletionEvent WITHIN 1 "
// 		"WHERE TargetInstance ISA 'Win32_Process' AND TargetInstance.ProcessId=%u", processId);
	sprintf_s(buffer, "SELECT * "
		"FROM __InstanceDeletionEvent WITHIN 1 "
		"WHERE TargetInstance ISA 'Win32_Process'");

	hres = m_spTerminationSink->pSvc->ExecNotificationQueryAsync(
		_bstr_t("WQL"),
		_bstr_t(buffer),
		WBEM_FLAG_SEND_STATUS,
		NULL,
		m_spTerminationSink->pStubSink);

	// Check for errors.
	if (FAILED(hres))
	{
// 		cout << "ExecNotificationQueryAsync failed "
// 			"with = 0x" << hex << hres << endl;
		throw std::exception("ProcessTerminationNotificaiton initialization failed");
	}

	return 0;
}

int CProcessNotification::unregisterTerminationCallback()
{
	if (!m_spTerminationSink)
	{
		return -1;
	}
	m_spTerminationSink->CancelAsyncCall();

	return 0;
}
