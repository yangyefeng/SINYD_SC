// SINYD_SC_PluginMgr.cpp : ���� DLL Ӧ�ó���ĵ���������
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
