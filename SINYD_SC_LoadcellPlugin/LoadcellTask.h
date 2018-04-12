/////////////////////////////////////////////////////////////////////////////
/// @File name: LoadcellTask.h
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
#include <list>
#include <memory>
#include "SCOCR.h"

using namespace std;


class CLoadcellTask
{
public:
	CLoadcellTask(PTaskInfo pTaskInfo);
	virtual ~CLoadcellTask();

	int GetID();
	bool IsRunning();
	void Execute(IScreenReader* pIScreenAreaReader, IPluginExecuteCallback* pIPluginExecuteCB, int packageID, const string& TimeStamp);
private:
	void Execute(IScreenReader* pIScreenAreaReader);
	void AfterExecute(IPluginExecuteCallback* pIPluginExecuteCB, string jsonRes);
	bool NeedExecute(IScreenReader* pIScreenAreaReader);
	string ResultToJson(int packetID, const string& timeStamp);

private:
	bool m_bIsRunning;
	TaskInfo m_taskInfo;
	CSCDigitOCR m_DigitOCR;
	CSCColorOCR m_ColorOCR;
	unsigned long m_lastCheckPointState;
	bool m_bFirstExecute;
	list<DIGIT> m_lastResult;
	unsigned long m_DigitRGB;
	bool m_bLoadDigitRGB;
};

typedef list<shared_ptr<CLoadcellTask>> TaskList;
