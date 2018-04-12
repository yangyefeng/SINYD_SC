#include "JsonRW.h"

Json::Value CJsonRW::s_source;

CJsonRW::CJsonRW()
{
}


CJsonRW::~CJsonRW()
{
}

void CJsonRW::Clear()
{
	m_finalJson.clear();
	m_root.clear();
    m_vecContent.clear();
    m_ContentChild.clear();
    m_vecContentData.clear();
    m_ContentDataChild.clear();
    m_vecData.clear();
}

int CJsonRW::CreateContent(int contentType)
{
	Json::Value content;
	content["content_type"] = contentType;
	m_vecContent.push_back(content);
	return m_vecContent.size() - 1;
}

int CJsonRW::CreateContentDataIntoContent(int ContentIndex, int dataType)
{
	Json::Value contentData;
	contentData["data_type"] = dataType;
	m_vecContentData.push_back(contentData);
	int index = m_vecContentData.size() - 1;
	m_ContentChild[ContentIndex].push_back(index);
	return index;
}

int CJsonRW::CreateContentDataIntoContent(int ContentIndex, int dataType, const string & timeStamp)
{
	Json::Value contentData;
	contentData["data_type"] = dataType;
	contentData["data_timestamp"] = timeStamp;
	m_vecContentData.push_back(contentData);
	int index = m_vecContentData.size() - 1;
	m_ContentChild[ContentIndex].push_back(index);
	return index;
}

int CJsonRW::CreateDataIntoContentData(int ContentDataIndex)
{
	Json::Value data;
	m_vecData.push_back(data);
	int index = m_vecData.size() - 1;
	m_ContentDataChild[ContentDataIndex].push_back(index);
	return index;
}

const string * CJsonRW::GetFinal()
{
	{
        map<int, vector<int> >::iterator itEnd = m_ContentDataChild.end();
        for (map<int, vector<int> >::iterator it = m_ContentDataChild.begin(); it != itEnd; ++it)
		{
			vector<int>& vecChlid = it->second;
			for (int i = 0; i < vecChlid.size(); i++)
			{
				m_vecContentData[it->first]["data"].append(m_vecData[vecChlid[i]]);
			}
		}
	}

	{
        map<int, vector<int> >::iterator itEnd = m_ContentChild.end();
        for (map<int, vector<int> >::iterator it = m_ContentChild.begin(); it != itEnd; ++it)
		{
			vector<int>& vecChlid = it->second;
			for (int i = 0; i < vecChlid.size(); i++)
			{
				m_vecContent[it->first]["content_data"].append(m_vecContentData[vecChlid[i]]);
			}
		}
	}

	{
		for (int i = 0; i < m_vecContent.size(); i++)
		{
			m_root["content"].append(m_vecContent[i]);
		}
	}

	m_root["source"] = s_source;
	
	Json::FastWriter fast_writer;
	m_finalJson = fast_writer.write(m_root);
	return &m_finalJson;
}

void CJsonRW::initSourceInfo(const SourceInfo & info)
{
	s_source["type"] = info.source_type;
	Json::Value infoNode;
	infoNode["mix_id"] = info.mix_id;
	infoNode["mix_type"] = info.mix_type;
	s_source["info"] = infoNode;
}
