/////////////////////////////////////////////////////////////////////////////
/// @File name: ISCPlugin.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
///		Plugin interface include
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////


#pragma once
#include "SCPluginDef.h"
#include "IScreenReader.h"
#include <string>

using namespace std;

typedef struct stParam
{
	std::string paramName;
	std::string paramValue;
}PARAM, *PPARAM;

typedef struct stAreaPos
{
	int x;
	int y;
	int width;
	int height;
}AREAPOS, *PAREAPOS;

typedef struct stCheckPointPos
{
	int checkType;
	int x;
	int y;
}CHECKPOS, *PCHECKPOS;

typedef struct stTaskInfo
{
	int id;
	int type;
	int valueType;		//Unused
	AREAPOS areaPos;
	CHECKPOS checkPos;
	int paramsCount;
	PPARAM pParams;

	stTaskInfo() { pParams = nullptr; };
	stTaskInfo(const stTaskInfo& info)
		: id(info.id)
		, type(info.type)
		, valueType(info.valueType)
		, areaPos(info.areaPos)
		, checkPos(info.checkPos)
		, paramsCount(info.paramsCount)
	{
		pParams = new PARAM[paramsCount];
		for (int i = 0; i < paramsCount; i++)
		{
			pParams[i] = info.pParams[i];
		}
	}
	stTaskInfo& operator=(const stTaskInfo& info)
	{
		id = info.id;
		type = info.type;
		valueType = info.valueType;
		areaPos = info.areaPos;
		checkPos = info.checkPos;
		paramsCount = info.paramsCount;

		if (pParams)
		{
			delete[] pParams;
		}
		pParams = new PARAM[paramsCount];
		for (int i = 0; i < paramsCount; i++)
		{
			pParams[i] = info.pParams[i];
		}
	}
	~stTaskInfo()
	{
		if (pParams)
		{
			delete[] pParams;
			pParams = nullptr;
		}
	}
}TaskInfo, *PTaskInfo;



class IPluginExecuteCallback
{
public:
	virtual ~IPluginExecuteCallback(){}
	virtual void ExecuteResult(const string& result) = 0;
	//virtual void IncreasePluginCount() = 0;
	virtual void DecreasePluginCount() = 0;
};

class IPlugin
{
public:
	virtual ~IPlugin(){}
	///*******************************************************
	/// @name:   IPlugin::GetCapability
	/// @author: YaoDi
	/// @return: SC_PLUGIN_ERROR
	/// @param:  [in/out/inout][int *]piCapability
	/// @brief:  Get the plugin object's capability.
	///*******************************************************
	virtual SC_PLUGIN_ERROR GetCapability(int* piCapability) = 0;

	///*******************************************************
	/// @name:   IPlugin::Execute
	/// @author: YaoDi
	/// @return: SC_PLUGIN_ERROR
	/// @param:  [in][IScreenReader *]pIScreenAreaReader
	/// @param:  [in][IPluginExecuteCallback *]pIPluginExecuteCB
	/// @param:  [in][int]packageID
	/// @param:  [in][const string &]TimeStamp
	/// @brief:  Execute the plugin. After call this function, plugin execute all task in itself.
	///*******************************************************
	virtual SC_PLUGIN_ERROR Execute(IScreenReader* pIScreenAreaReader, IPluginExecuteCallback* pIPluginExecuteCB, int packageID, const string& TimeStamp) = 0;
	
	///*******************************************************
	/// @name:   IPlugin::Start
	/// @author: YaoDi
	/// @return: SC_PLUGIN_ERROR
	/// @brief:  Start the plugin. After call this function, plugin start all thread in itself.
	///*******************************************************
	virtual SC_PLUGIN_ERROR Start() = 0;

	///*******************************************************
	/// @name:   IPlugin::Stop
	/// @author: YaoDi
	/// @return: SC_PLUGIN_ERROR
	/// @brief:  Stop the plugin. After call this function, plugin stop all thread in itself.
	///*******************************************************
	virtual SC_PLUGIN_ERROR Stop() = 0;

	///*******************************************************
	/// @name:   IPlugin::Suspend
	/// @author: YaoDi
	/// @return: SC_PLUGIN_ERROR
	/// @param:  [out][unsigned long *]plResult
	/// @brief:  Suspend the plugin. After call this function, plugin Suspend all thread in itself.
	///*******************************************************
	virtual SC_PLUGIN_ERROR Suspend(unsigned long* plResult) = 0;

	///*******************************************************
	/// @name:   IPlugin::Resume
	/// @author: YaoDi
	/// @return: SC_PLUGIN_ERROR
	/// @param:  [out][unsigned long *]plResult
	/// @brief:  Resume the plugin. After call this function, plugin resume all thread in itself.
	///*******************************************************
	virtual SC_PLUGIN_ERROR Resume(unsigned long* plResult) = 0;

	///*******************************************************
	/// @name:   IPlugin::AddTask
	/// @author: YaoDi
	/// @return: SC_PLUGIN_ERROR
	/// @param:  [in][PTaskInfo]pTaskInfo
	/// @brief:  Add task to plugin.
	///*******************************************************
	virtual SC_PLUGIN_ERROR AddTask(PTaskInfo pTaskInfo) = 0;

	///*******************************************************
	/// @name:   IPlugin::RemoveTask
	/// @author: YaoDi
	/// @return: SC_PLUGIN_ERROR
	/// @param:  [in][PTaskInfo]pTaskInfo
	/// @brief:  Remove task from plugin
	///*******************************************************
	virtual SC_PLUGIN_ERROR RemoveTask(PTaskInfo pTaskInfo) = 0;

	///*******************************************************
	/// @name:   IPlugin::GetTaskCount
	/// @author: YaoDi
	/// @return: SC_PLUGIN_ERROR
	/// @param:  [out][int *]piCount
	/// @brief:  Get the count in the plugin
	///*******************************************************
	virtual SC_PLUGIN_ERROR GetTaskCount(int* piCount) = 0;

	///*******************************************************
	/// @name:   IPlugin::GetRunningTaskCount
	/// @author: YaoDi
	/// @return: SC_PLUGIN_ERROR
	/// @param:  [out][int *]piCount
	/// @brief:  Get the running task in the plugin
	///*******************************************************
	virtual SC_PLUGIN_ERROR GetRunningTaskCount(int* piCount) = 0;
};
