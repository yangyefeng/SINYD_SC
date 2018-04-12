/////////////////////////////////////////////////////////////////////////////
/// @File name: PluginInfo.cpp
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 		
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#include "PluginInfo.h"
#include "..\TinyXML2\tinyxml2.h"

using namespace tinyxml2;

CPluginInfoLoader::CPluginInfoLoader()
{

}


CPluginInfoLoader::~CPluginInfoLoader()
{
}

int CPluginInfoLoader::Load(string strConfigPath)
{
	int iRes = -1;

	XMLDocument doc;
	doc.LoadFile(strConfigPath.c_str());
	if (!doc.NoChildren())
	{
		XMLNode* node = doc.FirstChildElement("SC_PluginList");
		if (node && !node->NoChildren())
		{
			XMLElement* childeElement = node->FirstChildElement();
			while (childeElement)
			{
				const char * filename = childeElement->Attribute("filename");
				if (filename != nullptr)
				{
					m_vecPluginInfo.push_back(PluginInfo(filename));
				}
				childeElement = childeElement->NextSiblingElement();
			}
			iRes = 0;
		}
	}

	return iRes;
}

long CPluginInfoLoader::GetPluginCount()
{
	return m_vecPluginInfo.size();
}

bool CPluginInfoLoader::GetPluginInfo(long lNum, PPluginInfo pInfo)
{
	if (lNum >= m_vecPluginInfo.size())
	{
		return false;
	}
	else
	{
		*pInfo = m_vecPluginInfo.at(lNum);
	}
	return true;
}
