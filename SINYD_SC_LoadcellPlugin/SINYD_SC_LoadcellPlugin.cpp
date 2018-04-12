/////////////////////////////////////////////////////////////////////////////
/// @File name: SINYD_SC_LoadcellPlugin.cpp
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 		
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "ISCPlugin.h"
#include "SCPluginDef.h"
#include "LoadcellPlugin.h"

extern "C"
{
	SC_PLUGIN_ERROR SC_PLUGIN_API SC_PLUGIN_EXPORT_CALLCONV CreatePlugin(IPlugin** ppPlugin)
	{
		*ppPlugin = new CLoadcellPlugin();
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
