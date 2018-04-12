/////////////////////////////////////////////////////////////////////////////
/// @File name: CollectionManager.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 	Collection manager header. Define the collection manager and the plugin manager callback
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ThreadUtil.h"
#include "ISCPluginMgr.h"
#include "..\TinyXML2\tinyxml2.h"
//#include "Measurelogic.h"
#include "Measure.h"
#include <list>
#include <memory>


enum CallbackTaskType
{
	Type_Unknown,
	Type_Result,
	Type_End
};

struct CallbackTask
{
	string result;
	CallbackTaskType type;
};

class CPluginMgrExecuteCallback : public IPluginMgrExecuteCallback
								, public CThreadBase
{
public:
	CPluginMgrExecuteCallback();
	~CPluginMgrExecuteCallback();

	///*******************************************************
	/// @name:   CPluginMgrExecuteCallback::ExecuteResult
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][const string &]result
	/// @brief:  �ص�ִ��һ���ɼ�����
	///*******************************************************
	virtual void ExecuteResult(const string& result) override;
	///*******************************************************
	/// @name:   CPluginMgrExecuteCallback::ExecuteEndEvent
	/// @author: YaoDi
	/// @return: void
	/// @brief:  �ص�ִ��һ֡����
	///*******************************************************
	virtual void ExecuteEndEvent() override;
	///*******************************************************
	/// @name:   CPluginMgrExecuteCallback::IsProducing
	/// @author: YaoDi
	/// @return: bool
	/// @brief:  �Ƿ���������
	///*******************************************************
	bool IsProducing();
protected:
	virtual DWORD Execute() override;
	
private:
	HANDLE m_hTaskEvent;
	CCriticalSection m_listTaskCS;
	std::list<std::shared_ptr<CallbackTask>> m_listTask;
	CMeasureLogic m_MeasureLogic;
};



class CCollectionManager
{
	DECLARE_SINGLETON_CLASS(CCollectionManager);

private:
	CCollectionManager(const CCollectionManager&);
	CCollectionManager& operator=(const CCollectionManager&);

	CCollectionManager();
	virtual ~CCollectionManager();

public:
	///*******************************************************
	/// @name:   CCollectionManager::Run
	/// @author: YaoDi
	/// @return: void
	/// @brief:  �����ɼ�
	///*******************************************************
	void Run();
	///*******************************************************
	/// @name:   CCollectionManager::Stop
	/// @author: YaoDi
	/// @return: void
	/// @brief:  ֹͣ�ɼ�
	///*******************************************************
	void Stop();
	///*******************************************************
	/// @name:   CCollectionManager::Suspend
	/// @author: YaoDi
	/// @return: void
	/// @brief:  �ɼ��̹߳���
	///*******************************************************
	void Suspend();
	///*******************************************************
	/// @name:   CCollectionManager::Resume
	/// @author: YaoDi
	/// @return: void
	/// @brief:  �ɼ��ָ̻߳�
	///*******************************************************
	void Resume();
	///*******************************************************
	/// @name:   CCollectionManager::Collect
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][const string &]time
	/// @brief:  ִ��һ�βɼ�����
	///*******************************************************
	void Collect(const string& time);
	///*******************************************************
	/// @name:   CCollectionManager::IsCollecting
	/// @author: YaoDi
	/// @return: bool
	/// @brief:  �Ƿ�����ִ�вɼ�����
	///*******************************************************
	bool IsCollecting();
	///*******************************************************
	/// @name:   CCollectionManager::IsProducing
	/// @author: YaoDi
	/// @return: bool
	/// @brief:  �Ƿ���������
	///*******************************************************
	bool IsProducing();

private:
	void AddTasksToPlugin();
	int InitPackageID();
	void SavePackageID(int packageID);

private:
	IPluginManager* m_pPluginMgr;
	CPluginMgrExecuteCallback m_PluginMgrExecuteCB;
	tinyxml2::XMLDocument m_XMLDoc;
	int m_packageID;
};
typedef CSingleton<CCollectionManager> CollectionMgrInstance;

