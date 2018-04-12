// SINYD_SC_ValvePlugin.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "ISCPlugin.h"
#include "SCPluginDef.h"
#include "ValvePlugin.h"

extern "C"
{
	SC_PLUGIN_ERROR SC_PLUGIN_API SC_PLUGIN_EXPORT_CALLCONV CreatePlugin(IPlugin** ppPlugin)
	{
		*ppPlugin = new CValvePlugin();
		if (*ppPlugin == nullptr)
		{
			return SC_PLUGIN_FAIL;
		}
		return SC_PLUGIN_SUCCESS;
	}

	void SC_PLUGIN_API SC_PLUGIN_EXPORT_CALLCONV ReleasePlugin(IPlugin* pPlugin)
	{
		if (pPlugin != nullptr)
		{
			delete pPlugin;
			pPlugin = nullptr;
		}
	}
}