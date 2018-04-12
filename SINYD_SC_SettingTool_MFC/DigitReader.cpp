#include "stdafx.h"
#include "DigitReader.h"
#include "..\TinyXML2\tinyxml2.h"


DigitsRW::DigitsRW()
{

}


DigitsRW::~DigitsRW()
{
}

list<Digit> DigitsRW::ReadFromXML(const string & xml_name)
{
	list<Digit> digits;

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError err = doc.LoadFile(xml_name.c_str());
	if (err == tinyxml2::XMLError::XML_SUCCESS)
	{
		if (!doc.NoChildren())
		{
			tinyxml2::XMLElement* element = doc.FirstChildElement("SC_DigitTemplateModelList");
			if (element != nullptr)
			{
				if (!element->NoChildren())
				{
					tinyxml2::XMLElement* childeElement = element->FirstChildElement();
					while (childeElement)
					{
						Digit d;
						d.value = childeElement->Attribute("V");
						d.id = childeElement->IntAttribute("ID");
						d.weight = childeElement->IntAttribute("W");
						d.height = childeElement->IntAttribute("H");
						d.model = childeElement->Attribute("M");

						digits.push_back(d);

						childeElement = childeElement->NextSiblingElement();
					}
				}
			}
		}
	}


	return digits;
}

bool DigitsRW::WriteOneDigitToXML(const string& xml_name, Digit digit)
{
	bool bRes = false;
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError err = doc.LoadFile(xml_name.c_str());
	if (err == tinyxml2::XMLError::XML_SUCCESS)
	{
		if (!doc.NoChildren())
		{
			tinyxml2::XMLElement* element = doc.FirstChildElement("SC_DigitTemplateModelList");
			if (element != nullptr)
			{
				if (!element->NoChildren())
				{
					tinyxml2::XMLElement* childeElement = element->FirstChildElement();
					while (childeElement)
					{
						if (digit.id == childeElement->IntAttribute("ID"))
						{
							childeElement->SetAttribute("V", digit.value.c_str());
							err = doc.SaveFile(xml_name.c_str());
							if (err == tinyxml2::XMLError::XML_SUCCESS)
							{
								bRes = true;
							}
							break;
						}

						childeElement = childeElement->NextSiblingElement();
					}
				}
			}
		}
	}

	return bRes;
}

bool DigitsRW::DeleteOneDigitInXML(const string & xml_name, Digit digit)
{
	bool bRes = false;
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError err = doc.LoadFile(xml_name.c_str());
	if (err == tinyxml2::XMLError::XML_SUCCESS)
	{
		if (!doc.NoChildren())
		{
			tinyxml2::XMLElement* element = doc.FirstChildElement("SC_DigitTemplateModelList");
			if (element != nullptr)
			{
				if (!element->NoChildren())
				{
					tinyxml2::XMLElement* childeElement = element->FirstChildElement();
					while (childeElement)
					{
						if (digit.id == childeElement->IntAttribute("ID"))
						{
							element->DeleteChild(childeElement);
							err = doc.SaveFile(xml_name.c_str());
							if (err == tinyxml2::XMLError::XML_SUCCESS)
							{
								bRes = true;
							}
							break;
						}

						childeElement = childeElement->NextSiblingElement();
					}
				}
			}
		}
	}

	return bRes;
}
