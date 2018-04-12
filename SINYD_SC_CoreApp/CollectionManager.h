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
	/// @brief:  回调执行一条采集数据
	///*******************************************************
	virtual void ExecuteResult(const string& result) override;
	///*******************************************************
	/// @name:   CPluginMgrExecuteCallback::ExecuteEndEvent
	/// @author: YaoDi
	/// @return: void
	/// @brief:  回调执行一帧结束
	///*******************************************************
	virtual void ExecuteEndEvent() override;
	///*******************************************************
	/// @name:   CPluginMgrExecuteCallback::IsProducing
	/// @author: YaoDi
	/// @return: bool
	/// @brief:  是否正在生产
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
	/// @brief:  启动采集
	///*******************************************************
	void Run();
	///*******************************************************
	/// @name:   CCollectionManager::Stop
	/// @author: YaoDi
	/// @return: void
	/// @brief:  停止采集
	///*******************************************************
	void Stop();
	///*******************************************************
	/// @name:   CCollectionManager::Suspend
	/// @author: YaoDi
	/// @return: void
	/// @brief:  采集线程挂起
	///*******************************************************
	void Suspend();
	///*******************************************************
	/// @name:   CCollectionManager::Resume
	/// @author: YaoDi
	/// @return: void
	/// @brief:  采集线程恢复
	///*******************************************************
	void Resume();
	///*******************************************************
	/// @name:   CCollectionManager::Collect
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][const string &]time
	/// @brief:  执行一次采集任务
	///*******************************************************
	void Collect(const string& time);
	///*******************************************************
	/// @name:   CCollectionManager::IsCollecting
	/// @author: YaoDi
	/// @return: bool
	/// @brief:  是否正在执行采集任务
	///*******************************************************
	bool IsCollecting();
	///*******************************************************
	/// @name:   CCollectionManager::IsProducing
	/// @author: YaoDi
	/// @return: bool
	/// @brief:  是否正在生产
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

