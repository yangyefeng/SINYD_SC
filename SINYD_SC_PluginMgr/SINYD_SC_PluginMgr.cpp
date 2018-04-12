// SINYD_SC_PluginMgr.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "PluginManagerDLL.h"
#include "SCPluginManager.h"

SC_PLUGIN_ERROR SC_PLUGIN_MGR_API SC_PLUGIN_EXPORT_CALLCONV CreatePluginMgr(IPluginManager** ppPluginMgr)
{
	*ppPluginMgr = new CSCPluginManager();
	if (*ppPluginMgr == nullptr)
	{
		return SC_PLUGIN_FAIL;
	}
	return SC_PLUGIN_SUCCESS;
}

void SC_PLUGIN_MGR_API SC_PLUGIN_EXPORT_CALLCONV ReleasePluginMgr(IPluginManager* pPluginMgr)
{
	if (pPluginMgr != nullptr)
	{
		delete pPluginMgr;
		pPluginMgr = nullptr;
	}
}
