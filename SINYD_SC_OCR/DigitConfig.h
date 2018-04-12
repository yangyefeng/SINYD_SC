/////////////////////////////////////////////////////////////////////////////
/// @File name: DigitConfig.h
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
#include "ThreadUtil.h"
#include <string>
#include <map>
#include "SCOCRDef.h"

using namespace std;

typedef struct stDigitTemplateAttr
{
	string c;
	int id;
	int w;
	int h;
}DTA, *PDTA;

class CDigitConfig
{
	DECLARE_SINGLETON_CLASS(CDigitConfig);

private:
	CDigitConfig(const CDigitConfig&);
	CDigitConfig& operator=(const CDigitConfig&);

	CDigitConfig();
	virtual ~CDigitConfig();

public:
	int SetDTM(const DIGIT& d);
	bool FindDTA(string m, DTA& dta);
	bool FindModelAndDTA(int id, DTA& dta, string& m);

private:
	void LoadConfig(string fileName);
	int SetDTMToConfig(const DIGIT& d, string fileName);

	CCriticalSection m_mapDTMCS;
	map<string, DTA> m_mapDTM;
	CCriticalSection m_mapDTMIDCS;
	map<int, string> m_mapDTMID;

	int m_maxID;
};
typedef CSingleton<CDigitConfig> DigitConfigIntance;

