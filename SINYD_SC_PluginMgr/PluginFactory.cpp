/////////////////////////////////////////////////////////////////////////////
/// @File name: PluginFactory.cpp
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 		
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#include "PluginFactory.h"

CCriticalSection CPluginFactory::m_smapCS;
map<int, HMODULE> CPluginFactory::m_smapPluginDLLHandle;

int CPluginFactory::CreatePlugin(const PluginInfo & pluginInfo, IPlugin ** ppPlugin)
{
	int iRes = -1;

	HMODULE hDLL = ::LoadLibraryA(pluginInfo.strPluginFileName.c_str());
	if (hDLL != NULL)
	{
		typedef SC_PLUGIN_ERROR(SC_PLUGIN_EXPORT_CALLCONV *CREATEPLUGINPROC)(IPlugin**);
		CREATEPLUGINPROC createFunc = (CREATEPLUGINPROC)::GetProcAddress(hDLL, "CreatePlugin");
		if (createFunc)
		{
			SC_PLUGIN_ERROR err = createFunc(ppPlugin);
			if (err == SC_PLUGIN_SUCCESS && (*ppPlugin) != nullptr)
			{
				iRes = 0;
				int iCapa = 0;
				(*ppPlugin)->GetCapability(&iCapa);

				CriticalSectionLock lock(m_smapCS);
				m_smapPluginDLLHandle[iCapa] = hDLL;
			}
			else
			{
				::FreeLibrary(hDLL);
				hDLL = NULL;
			}
		}
		else
		{
			::FreeLibrary(hDLL);
			hDLL = NULL;
		}
	}
	return iRes;
}

int CPluginFactory::ReleasePlugin(IPlugin * pPlugin)
{
	int iRes = -1;

	int iCapa = 0;
	pPlugin->GetCapability(&iCapa);
	HMODULE hDLL = m_smapPluginDLLHandle[iCapa];
	if (hDLL != NULL)
	{
		typedef SC_PLUGIN_ERROR(SC_PLUGIN_EXPORT_CALLCONV *RELEASEPLUGINPROC)(IPlugin*);
		RELEASEPLUGINPROC releaseFunc = (RELEASEPLUGINPROC)::GetProcAddress(hDLL, "ReleasePlugin");
		if (releaseFunc)
		{
			releaseFunc(pPlugin);
			pPlugin = nullptr;
			::FreeLibrary(hDLL);
			hDLL = NULL;

			iRes = 0;
		}
	}
	return iRes;
}
