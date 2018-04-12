/////////////////////////////////////////////////////////////////////////////
/// @File name: PluginManagerDLL.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 	Plugin manager export function include
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "ISCPluginMgr.h"

#ifdef SC_PLUGIN_MGR_DLL_EXPORT
#define SC_PLUGIN_MGR_API __declspec(dllexport)
#else 
#define SC_PLUGIN_MGR_API __declspec(dllimport)
#endif

extern "C"
{
	SC_PLUGIN_ERROR SC_PLUGIN_MGR_API SC_PLUGIN_EXPORT_CALLCONV CreatePluginMgr(IPluginManager** ppPluginMgr);
	void SC_PLUGIN_MGR_API SC_PLUGIN_EXPORT_CALLCONV ReleasePluginMgr(IPluginManager* pPluginMgr);
}

