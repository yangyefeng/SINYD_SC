/////////////////////////////////////////////////////////////////////////////
/// @File name: DigitConfig.cpp
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 		
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#include "DigitConfig.h"
#include "..\TinyXML2\tinyxml2.h"


#define DTMConfigFile "ConfigFile\\SCDTMList.xml"

CDigitConfig::CDigitConfig()
{
	char fileFullPath[MAX_PATH] = "";
	::GetModuleFileNameA(::GetModuleHandle(NULL), fileFullPath, MAX_PATH);
	std::string strRes(fileFullPath);
	strRes = strRes.erase(strRes.find_last_of('\\'));

	::SetCurrentDirectoryA(strRes.c_str());


	LoadConfig(DTMConfigFile);
}


CDigitConfig::~CDigitConfig()
{
}

int CDigitConfig::SetDTM(const DIGIT & d)
{
	return SetDTMToConfig(d, DTMConfigFile);
}

bool CDigitConfig::FindDTA(string m, DTA& dta)
{
	bool bFind = false;

	CriticalSectionLock lock(m_mapDTMCS);
	map<string, DTA>::iterator itor = m_mapDTM.find(m);
	if (itor != m_mapDTM.end())
	{
		bFind = true;
		dta = itor->second;
	}
	return bFind;
}

bool CDigitConfig::FindModelAndDTA(int id, DTA& dta, string& m)
{
	bool bFind = false;

	CriticalSectionLock lock(m_mapDTMIDCS);
	map<int, string>::iterator itor = m_mapDTMID.find(id);
	if (itor != m_mapDTMID.end())
	{
		m = itor->second;
		bFind = FindDTA(m, dta);
	}
	return bFind;
}

void CDigitConfig::LoadConfig(string fileName)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError err = doc.LoadFile(fileName.c_str());
	bool bNeedCreate = false;

	if (err == tinyxml2::XML_SUCCESS)
	{
		if (!doc.NoChildren())
		{
			tinyxml2::XMLElement* element = doc.FirstChildElement("SC_DigitTemplateModelList");
			if (element != nullptr)
			{
				m_maxID = element->IntAttribute("MAXID");

				if (!element->NoChildren())
				{
					CriticalSectionLock lock1(m_mapDTMCS);
					CriticalSectionLock lock2(m_mapDTMIDCS);

					tinyxml2::XMLElement* childeElement = element->FirstChildElement();
					while (childeElement)
					{
						//const char* V = childeElement->Attribute("V");
						DTA dta;
						//dta.c = V;
						dta.c = childeElement->Attribute("V");
						dta.id = childeElement->IntAttribute("ID");
						dta.w = childeElement->IntAttribute("W");
						dta.h = childeElement->IntAttribute("H");
						string M = childeElement->Attribute("M");
						m_mapDTM[M] = dta;
						m_mapDTMID[dta.id] = M;

						childeElement = childeElement->NextSiblingElement();
					}
				}
			}
		}
		else
		{
			bNeedCreate = true;
		}
	}
	else if (err == tinyxml2::XML_ERROR_FILE_NOT_FOUND)
	{
		bNeedCreate = true;
	}

	if (bNeedCreate)
	{
		doc.Clear();
		doc.SetBOM(true);
		tinyxml2::XMLElement* element = doc.NewElement("SC_DigitTemplateModelList");
		if (element)
		{
			element->SetAttribute("MAXID", 0);
			doc.InsertFirstChild(element);
			doc.SaveFile(fileName.c_str());
		}
	}
}

int CDigitConfig::SetDTMToConfig(const DIGIT& d, string fileName)
{
	int resID = -1;

	bool bFind = false;
	{
		CriticalSectionLock lock(m_mapDTMCS);
		map<string, DTA>::iterator itor = m_mapDTM.find(d.model.m);
		if (itor != m_mapDTM.end())
		{
			bFind = true;
			resID = itor->second.id;
		}
		else
		{
			bFind = false;
		}
	}

	if (!bFind)
	{
		tinyxml2::XMLDocument doc;
		doc.LoadFile(fileName.c_str());

		if (!doc.NoChildren())
		{
			tinyxml2::XMLElement* element = doc.FirstChildElement("SC_DigitTemplateModelList");
			if (element)
			{
				resID = ++m_maxID;
				{
					CriticalSectionLock lock(m_mapDTMCS);
					DTA dta;
					dta.c = d.digit;
					dta.w = d.model.width;
					dta.h = d.model.height;
					dta.id = resID;
					m_mapDTM[d.model.m] = dta;
				}
				{
					CriticalSectionLock lock(m_mapDTMIDCS);
					m_mapDTMID[resID] = d.model.m;
				}

				tinyxml2::XMLElement* newElement = doc.NewElement("DigitTemplateModel");
				newElement->SetAttribute("ID", m_maxID);
				//char tmpDigit[2] = { d.digit, '\0' };
				//newElement->SetAttribute("V", tmpDigit);
				newElement->SetAttribute("V", d.digit.c_str());
				newElement->SetAttribute("W", d.model.width);
				newElement->SetAttribute("H", d.model.height);
				newElement->SetAttribute("M", d.model.m.c_str());

				element->InsertEndChild(newElement);
				element->SetAttribute("MAXID", m_maxID);

				doc.SaveFile(fileName.c_str());
			}
		}
	}
	else
	{
		tinyxml2::XMLDocument doc;
		doc.LoadFile(fileName.c_str());

		if (!doc.NoChildren())
		{
			tinyxml2::XMLElement* element = doc.FirstChildElement("SC_DigitTemplateModelList");
			if (element)
			{
				tinyxml2::XMLElement* childeElement = element->FirstChildElement();
				while (childeElement)
				{
					string M = childeElement->Attribute("M");
					if (M == d.model.m)
					{
						//char tmpDigit[2] = { d.digit, '\0' };
						//childeElement->SetAttribute("V", tmpDigit);
						childeElement->SetAttribute("V", d.digit.c_str());
						childeElement->SetAttribute("W", d.model.width);
						childeElement->SetAttribute("H", d.model.height);

						break;
					}

					childeElement = childeElement->NextSiblingElement();
				}

				doc.SaveFile(fileName.c_str());
			}
		}
	}

	return resID;
}
