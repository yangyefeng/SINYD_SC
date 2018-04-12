/////////////////////////////////////////////////////////////////////////////
/// @File name: PluginInfo.h
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
#include <vector>

using namespace std;

#define ConfigFileName "SCPluginList.xml"

typedef struct stPluginInfo
{
	stPluginInfo() : strPluginFileName() {}
	stPluginInfo(const string& PluginFileName) : strPluginFileName(PluginFileName) {}
	stPluginInfo(const char* pPluginFileName) : strPluginFileName(pPluginFileName) {}
	
	string strPluginFileName;
}PluginInfo, *PPluginInfo;

class CPluginInfoLoader
{
public:
	CPluginInfoLoader();
	virtual ~CPluginInfoLoader();

	int Load(string strConfigPath);
	long GetPluginCount();
	bool GetPluginInfo(long lNum, PPluginInfo pInfo);

private:
	vector<PluginInfo> m_vecPluginInfo;
};

