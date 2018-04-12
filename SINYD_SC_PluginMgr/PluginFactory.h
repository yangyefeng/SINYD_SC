/////////////////////////////////////////////////////////////////////////////
/// @File name: PluginFactory.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 		
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "ISCPlugin.h"
#include "CommonDef.h"
#include "PluginInfo.h"
#include "ThreadUtil.h"
#include <map>
#include <winnt.h>

// class IPluginFactory
// {
// public:
// 	virtual ~IPluginFactory() {}
// 	virtual int CreatePlugin(const PluginInfo& pluginInfo, IPlugin** ppPlugin) = 0;
// 	virtual int ReleasePlugin(const PluginInfo& pluginInfo, IPlugin* ppPlugin) = 0;
// };

class CPluginFactory/* : public IPluginFactory*/
{
public:
// 	CPluginFactory();
// 	virtual ~CPluginFactory();

	static int CreatePlugin(const PluginInfo& pluginInfo, IPlugin** ppPlugin);
	static int ReleasePlugin(IPlugin* pPlugin);


private:
	static CCriticalSection m_smapCS;
	static map<int, HMODULE> m_smapPluginDLLHandle;
};

