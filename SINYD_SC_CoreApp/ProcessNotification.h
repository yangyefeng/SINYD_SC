/////////////////////////////////////////////////////////////////////////////
/// @File name: ProcessNotification.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 	Watch Process start and stop using WMI
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#pragma once

#ifndef __ProcessNotification_h__
#define __ProcessNotification_h__
#include <functional>
#include <Wbemidl.h>
#include <atlcomcli.h>



class EventSink;
class CProcessNotification
{
public:
	CProcessNotification();
	~CProcessNotification();

	typedef std::tr1::function<bool(IWbemClassObject*)> TNotificationFunction;

	///*******************************************************
	/// @name:   CProcessNotification::registerCreationCallback
	/// @author: YaoDi
	/// @return: int
	/// @param:  [in][TNotificationFunction]callback
	/// @brief:  注册进程启动事件回调函数
	///*******************************************************
	int registerCreationCallback(TNotificationFunction callback);
	///*******************************************************
	/// @name:   CProcessNotification::unregisterCreationCallback
	/// @author: YaoDi
	/// @return: int
	/// @brief:  注销系统下进程启动事件回调
	///*******************************************************
	int unregisterCreationCallback();
	///*******************************************************
	/// @name:   CProcessNotification::registerTerminationCallback
	/// @author: YaoDi
	/// @return: int
	/// @param:  [in][TNotificationFunction]callback
	/// @param:  [in][unsigned long]processId
	/// @brief:  注册目标进程退出事件回调
	///*******************************************************
	int registerTerminationCallback(TNotificationFunction callback/*, unsigned long processId*/);
	///*******************************************************
	/// @name:   CProcessNotification::unregisterTerminationCallback
	/// @author: YaoDi
	/// @return: int
	/// @brief:  注销目标进程退出事件回调
	///*******************************************************
	int unregisterTerminationCallback();
private:
	CComPtr<EventSink> m_spCreationSink;
	CComPtr<EventSink> m_spTerminationSink;
};

class EventSink : public IWbemObjectSink
{
	friend class CProcessNotification;

	CComPtr<IWbemServices> pSvc;
	CComPtr<IWbemObjectSink> pStubSink;

	LONG m_lRef;
	CProcessNotification::TNotificationFunction m_callback;

public:
	EventSink(CProcessNotification::TNotificationFunction callback);
	~EventSink();

	void CancelAsyncCall();

	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv);

	virtual HRESULT STDMETHODCALLTYPE Indicate(
		LONG lObjectCount,
		IWbemClassObject __RPC_FAR *__RPC_FAR *apObjArray
		);

	virtual HRESULT STDMETHODCALLTYPE SetStatus(
		/* [in] */ LONG lFlags,
		/* [in] */ HRESULT hResult,
		/* [in] */ BSTR strParam,
		/* [in] */ IWbemClassObject __RPC_FAR *pObjParam
		);

};

#endif // __ProcessNotification_h__
