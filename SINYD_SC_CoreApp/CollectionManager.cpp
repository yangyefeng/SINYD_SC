/////////////////////////////////////////////////////////////////////////////
/// @File name: CollectionManager.cpp
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 		
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#include "CollectionManager.h"
#include "PluginManagerDLL.h"
#include "ScreenShotHolder.h"
#include "..\SINYD_SC_CommonUtility\CCalibrationConfig.h"
#include "ConfigInitializer.h"
#include "log.h"
#include "MeasureDef.h"



#define COLLECTIONCONFIGFILEFULLPATH CONFIG_BACKUP_PATH COLLECTIONCONFIGFILE
#define COLLECTIONCONFIGFILE "SCCollectInfo.xml"

CPluginMgrExecuteCallback::CPluginMgrExecuteCallback()
{
	m_hTaskEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_MeasureLogic.Initialize();
}

CPluginMgrExecuteCallback::~CPluginMgrExecuteCallback()
{
	this->Stop();

	if (m_hTaskEvent)
	{
		::CloseHandle(m_hTaskEvent);
		m_hTaskEvent = nullptr;
	}
}

void CPluginMgrExecuteCallback::ExecuteResult(const string& result)
{
	SCINFO("CPluginMgrExecuteCallback::ExecuteResult");
	shared_ptr<CallbackTask> spTask(new CallbackTask());
	spTask->result = result;
	spTask->type = Type_Result;
	CriticalSectionLock lock(m_listTaskCS);
	m_listTask.push_back(spTask);
	::SetEvent(m_hTaskEvent);
	//m_MeasureLogic.SetSrcData(result);
}

void CPluginMgrExecuteCallback::ExecuteEndEvent()
{
	SCINFO("CPluginMgrExecuteCallback::ExecuteEndEvent");
	shared_ptr<CallbackTask> spTask(new CallbackTask());
	spTask->type = Type_End;
	CriticalSectionLock lock(m_listTaskCS);
	m_listTask.push_back(spTask);
	::SetEvent(m_hTaskEvent);
	//m_MeasureLogic.EndFrame();
}

DWORD CPluginMgrExecuteCallback::Execute()
{
	printf("[CPluginMgrExecuteCallback]Execute \n");
	constexpr DWORD dwHandleCount = 2;
	HANDLE handles[dwHandleCount] = { m_hStopEvent, m_hTaskEvent };
	while (true)
	{
		DWORD dw = ::WaitForMultipleObjects(dwHandleCount, handles, FALSE, INFINITE);
		if (WAIT_OBJECT_0 == dw)
		{
			break;
		}
		else if (WAIT_OBJECT_0 + 1 == dw)
		{
			list<shared_ptr<CallbackTask>> tmpList;
			{
				CriticalSectionLock lock(m_listTaskCS);
				tmpList.swap(m_listTask);
			}

			list<shared_ptr<CallbackTask>>::iterator itB = tmpList.begin();
			list<shared_ptr<CallbackTask>>::iterator itE = tmpList.end();
			for (list<shared_ptr<CallbackTask>>::iterator it = itB; it != itE; ++it)
			{
				if ((*it)->type == Type_Result)
				{
					SCINFO("CPluginMgrExecuteCallback SetSrcData Start");
					m_MeasureLogic.SetSrcData((*it)->result);
					SCINFO("CPluginMgrExecuteCallback SetSrcData End");
				}
				else if ((*it)->type == Type_End)
				{
					SCINFO("CPluginMgrExecuteCallback EndFrame Start");
					m_MeasureLogic.EndFrame();
					SCINFO("CPluginMgrExecuteCallback EndFrame End");
				}
			}

			
		}
		else
		{
			continue;
		}
	}

	//m_MeasureLogic.ClearProcessData();

	return 0;
}

bool CPluginMgrExecuteCallback::IsProducing()
{
	return m_MeasureLogic.IsRunning();
}

CCollectionManager::CCollectionManager()
	: m_pPluginMgr(nullptr)
	, m_PluginMgrExecuteCB()
	, m_XMLDoc()
	, m_packageID(0)
{
	SC_PLUGIN_ERROR err = CreatePluginMgr(&m_pPluginMgr);
	if (err == SC_PLUGIN_SUCCESS)
	{
		m_pPluginMgr->InitPlugins();
		AddTasksToPlugin();
		m_packageID = InitPackageID();
	}
	else
	{
		SCERROR("CCollectionManager CreatePluginMgr err %d", err);
	}
}


CCollectionManager::~CCollectionManager()
{
	if (m_pPluginMgr != nullptr)
	{
		this->Stop();

		m_pPluginMgr->UnInitPlugins();
		ReleasePluginMgr(m_pPluginMgr);
	}
}

void CCollectionManager::Run()
{
	if (m_pPluginMgr != nullptr)
	{
		m_pPluginMgr->StartPlugins();
		m_PluginMgrExecuteCB.Start();
	}
}

void CCollectionManager::Suspend()
{
	if (m_pPluginMgr != nullptr)
	{
		m_pPluginMgr->SuspendPlugins();
	}
}

void CCollectionManager::Resume()
{
	if (m_pPluginMgr != nullptr)
	{
		m_pPluginMgr->ResumePlugins();
	}
}

void CCollectionManager::Stop()
{
	if (m_pPluginMgr != nullptr)
	{
		m_pPluginMgr->StopPlugins();
		m_PluginMgrExecuteCB.Stop();
	}
}

void CCollectionManager::Collect(const string& time)
{
	if (m_pPluginMgr != nullptr)
	{
		SCINFO("Collect Start");
		m_pPluginMgr->ExecutePlugins(ScreenShotHolderInstance::GetInstance()->GetScreenReader(), &m_PluginMgrExecuteCB, m_packageID,time);
		SCINFO("Collect Stop");
		SavePackageID(++m_packageID);
	}
}

bool CCollectionManager::IsCollecting()
{
	int count = 0;
	m_pPluginMgr->GetRunningTaskCount(&count);
	return count > 0;
}

bool CCollectionManager::IsProducing()
{
	return m_PluginMgrExecuteCB.IsProducing();
}

void CCollectionManager::AddTasksToPlugin()
{
	const CCalibrationConfig* caliConfig = ConfiginitializerInstance::GetInstance()->CalibrationConfig();
	const vector<MonitorArea*>* pareas = caliConfig->GetMonitorAreas();
	vector<MonitorArea*>::const_iterator itBegin = pareas->begin();
	vector<MonitorArea*>::const_iterator itEnd = pareas->end();
	for (vector<MonitorArea*>::const_iterator it = itBegin; it != itEnd; ++it)
	{
		TaskInfo task;
		task.id = (*it)->GetMonitorAreaID();
		task.type = (*it)->GetMonitorAreaType();
		task.valueType = (*it)->GetMonitorAreaValueType();
		task.areaPos.x = (*it)->GetMonitorAreaX();
		task.areaPos.y = (*it)->GetMonitorAreaY();
		task.areaPos.width = (*it)->GetMonitorAreaWidth();
		task.areaPos.height = (*it)->GetMonitorAreaHeight();
		task.checkPos.checkType = (*it)->GetMonitorCheckPointType();
		task.checkPos.x = (*it)->GetMonitorCheckPointX();
		task.checkPos.y = (*it)->GetMonitorCheckPointY();
		task.paramsCount = (*it)->GetMonitorParaCount();
		task.pParams = new PARAM[task.paramsCount];
		const vector<Parameter *>* params = (*it)->GetParameters();
		for (int i = 0; i < task.paramsCount; i++)
		{
			task.pParams[i].paramName = (*params)[i]->GetParameterName();
			task.pParams[i].paramValue = (*params)[i]->GetParameterValue();
		}

		m_pPluginMgr->AddTask(&task);
	}

}

int CCollectionManager::InitPackageID()
{
	int iRes = 0;
	bool bNeedCreate = false;

	//string filePath = CONFIG_BACKUP_PATH + string(COLLECTIONCONFIGFILE);
	tinyxml2::XMLError err = m_XMLDoc.LoadFile(COLLECTIONCONFIGFILEFULLPATH);
	if (err == tinyxml2::XML_SUCCESS)
	{
		if (!m_XMLDoc.NoChildren())
		{
			tinyxml2::XMLElement* element = m_XMLDoc.FirstChildElement("SC_CollectCount");
			if (element)
			{
				iRes = atoi(element->GetText());
			}
		}
		else
		{
			bNeedCreate = true;
		}
	}
	else if (err == tinyxml2::XML_ERROR_FILE_NOT_FOUND || err == tinyxml2::XML_ERROR_EMPTY_DOCUMENT)
	{
		bNeedCreate = true;
	}

	if (bNeedCreate)
	{
		m_XMLDoc.Clear();
		m_XMLDoc.SetBOM(true);
		tinyxml2::XMLElement* element = m_XMLDoc.NewElement("SC_CollectCount");
		if (element)
		{
			element->SetText(iRes);
			m_XMLDoc.InsertFirstChild(element);
			m_XMLDoc.SaveFile(COLLECTIONCONFIGFILEFULLPATH);
		}
	}
	

	return iRes;
}

void CCollectionManager::SavePackageID(int packageID)
{
	tinyxml2::XMLElement* element = m_XMLDoc.FirstChildElement("SC_CollectCount");
	if (element)
	{
		element->SetText(packageID);
		//string filePath = CONFIG_BACKUP_PATH + string(COLLECTIONCONFIGFILE);
		m_XMLDoc.SaveFile(COLLECTIONCONFIGFILEFULLPATH);
	}
}
