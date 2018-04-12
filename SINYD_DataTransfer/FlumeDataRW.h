#pragma once
#include "SYDDataTransferDef.h"
#include <string>
#include <map>
#include <list>

using namespace std;
class CFlumeDataRW
{
public:
	CFlumeDataRW();
	virtual ~CFlumeDataRW();

	static void initSourceInfo(const SourceInfo2& info);
	static const SourceInfo2* getSourceInfo();

	void AddHeaderInfo(const string& key, const string& value);
	void AddValueToLastBodyRow(const string& value);
	void NewlineInBody();
	void AddRowToBody(const string& row_value);

	const string* GetFinal();
	void Clear();

protected:
	static SourceInfo2 s_source_info;
	string m_final_data;		//json format.
	map<string, string> m_header;
	list<string> m_body;		//csv format; every string is one row.
};

