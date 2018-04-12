#include "FlumeDataRW.h"
#include <string.h>
#include "json/json.h"
#if _WIN32
#include <windows.h>
#endif

SourceInfo2 CFlumeDataRW::s_source_info;

CFlumeDataRW::CFlumeDataRW()
{
}


CFlumeDataRW::~CFlumeDataRW()
{
}

void CFlumeDataRW::initSourceInfo(const SourceInfo2 & info)
{
	memcpy(&s_source_info, &info, sizeof(SourceInfo2));
}

const SourceInfo2 * CFlumeDataRW::getSourceInfo()
{
	return &s_source_info;
}

void CFlumeDataRW::AddHeaderInfo(const string& key, const string& value)
{
	m_header[key] = value;
}

void CFlumeDataRW::AddValueToLastBodyRow(const string& value)
{
	if (m_body.size() == 0)
	{
		m_body.push_back("");
	}

	if (m_body.rbegin()->empty())
	{
		m_body.rbegin()->append(value);
	}
	else
	{
		string csv_value = "," + value;
		m_body.rbegin()->append(csv_value);
	}
}

void CFlumeDataRW::NewlineInBody()
{
	m_body.push_back("");
}

void CFlumeDataRW::AddRowToBody(const string& row_value)
{
	m_body.push_back(row_value);
}

#if _WIN32
std::string string_To_UTF8(const std::string & str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char * pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr(pBuf);

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;

	return retStr;
}
#endif

const string * CFlumeDataRW::GetFinal()
{
	m_header["PROJ_ID"] = s_source_info.mix_project_id;
	m_header["SECT_ID"] = s_source_info.mix_section_id;
	m_header["EQU_ID"] = s_source_info.mix_machine_id;
	switch (s_source_info.mix_type)
	{
	case 1:	//沥青
		m_header["MIX_TYPE"] = "AMP";
		break;
	case 2:	//水泥
		m_header["MIX_TYPE"] = "CMP";
		break;
	case 3:	//改性沥青
		m_header["MIX_TYPE"] = "MMP";
		break;
	case 4:	//储罐
	default:
		break;
	}
	switch (s_source_info.source_type)
	{
	case 1:	//界面采集
		m_header["COLLECT_TYPE"] = "SC";
		break;
	case 2:	//传感器采集
		m_header["COLLECT_TYPE"] = "SS";
		break;
	default:
		break;
	}

	Json::Value root;

	Json::Value header;
	map<string, string>::iterator header_itEnd = m_header.end();
	for (map<string, string>::iterator it = m_header.begin(); it != header_itEnd; ++it)
	{
		header[it->first] = it->second;
	}

	root["headers"] = header;

	list<string>::iterator body_itEnd = m_body.end();
	string csv_body;
	for (list<string>::iterator it = m_body.begin(); it != body_itEnd; ++it)
	{
		csv_body.append(*it);
		csv_body.append("\r\n");
	}

	root["body"] = csv_body;

	Json::FastWriter fast_writer;
	m_final_data = fast_writer.write(root);

#if _WIN32
	m_final_data = string_To_UTF8(m_final_data);
#endif

	return &m_final_data;
}

void CFlumeDataRW::Clear()
{
	m_header.clear();
	m_body.clear();
	m_final_data = "";
}
