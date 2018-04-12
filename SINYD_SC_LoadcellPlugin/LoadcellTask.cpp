/////////////////////////////////////////////////////////////////////////////
/// @File name: LoadcellTask.cpp
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 		
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#include "LoadcellTask.h"
#include "json\json.h"
#include "log.h"
#include "CommonDef.h"

CLoadcellTask::CLoadcellTask(PTaskInfo pTaskInfo)
	: m_bIsRunning(false)
	, m_taskInfo(*pTaskInfo)
	, m_DigitOCR()
	, m_ColorOCR()
	, m_lastCheckPointState(0)
	, m_bFirstExecute(true)
	, m_lastResult()
	, m_DigitRGB(0)
	, m_bLoadDigitRGB(false)
{
	for (int i = 0; i < m_taskInfo.paramsCount; i++)
	{
		if (m_taskInfo.pParams[i].paramName == SC_COMMON_COLOR_THRESHOLD_PARAMNAME)
		{
			int tolerance = strtoul(m_taskInfo.pParams[i].paramValue.c_str(), NULL, 10);
// 			if (tolerance > 150)
// 			{
// 				tolerance = 150;
// 			}
			m_ColorOCR.SetEqulaTolerance(tolerance);
			m_DigitOCR.SetEqulaTolerance(tolerance);
		}
		else if (m_taskInfo.pParams[i].paramName == SC_LOADCELL_FONT_COLOR_PARAMNAME)
		{
			m_DigitRGB = strtoul(m_taskInfo.pParams[i].paramValue.c_str(), NULL, 10);
			m_bLoadDigitRGB = true;
		}
	}
}

CLoadcellTask::~CLoadcellTask()
{
	if (m_taskInfo.pParams)
	{
		delete[] m_taskInfo.pParams;
		m_taskInfo.pParams = nullptr;
	}
	m_lastResult.clear();
}

int CLoadcellTask::GetID()
{
	return m_taskInfo.id;
}

bool CLoadcellTask::IsRunning()
{
	return m_bIsRunning;
}

void CLoadcellTask::Execute(IScreenReader* pIScreenAreaReader, IPluginExecuteCallback* pIPluginExecuteCB, int packageID, const string& TimeStamp)
{
	m_bIsRunning = true;
	//Work
	//if (NeedExecute(pIScreenAreaReader))
	{
		Execute(pIScreenAreaReader);
		//if (m_lastResult.size() > 0)
		{
			AfterExecute(pIPluginExecuteCB, ResultToJson(packageID, TimeStamp));
		}
	}


	m_bIsRunning = false;
	if (m_bFirstExecute)
	{
		m_bFirstExecute = false;
	}
}

void CLoadcellTask::Execute(IScreenReader * pIScreenAreaReader)
{
	bool bRcg = false;

	float widthRatio = 1.0, heightRatio = 1.0;
	//pIScreenAreaReader->GetScreenRatio(&widthRatio, &heightRatio);

	long bufX = m_taskInfo.areaPos.x * widthRatio;
	long bufY = m_taskInfo.areaPos.y * heightRatio;
	long bufWidth = m_taskInfo.areaPos.width * widthRatio;
	long bufHeight = m_taskInfo.areaPos.height * heightRatio;
	long BufLength = bufHeight*bufWidth;
	unsigned long* ulBuf = new unsigned long[BufLength];
	long rWidth = 0, rHeight = 0;
	pIScreenAreaReader->GetScreenArea(bufX, bufY, bufWidth, bufHeight, ulBuf, &BufLength, &rWidth, &rHeight);
	if (BufLength > 0)
	{
		if (m_bLoadDigitRGB)
		{
			list<DIGIT> tmpDigit;
			SC_OCR_ERROR err = m_DigitOCR.RecognizeDigits(ulBuf, rWidth, rHeight, m_DigitRGB, tmpDigit);
			if (err == SC_OCR_SUCCESS || err == SC_OCR_EXISTS_UNRECOGNIZE)
			{
				m_lastResult = tmpDigit;
				m_DigitOCR.ClearRecogizeResList(tmpDigit);
				bRcg = true;
			}
		}
	}
	if (ulBuf)
	{
		delete[] ulBuf;
		ulBuf = nullptr;
	}

	if (!bRcg)
	{
		m_lastResult.clear();
	}
}

void CLoadcellTask::AfterExecute(IPluginExecuteCallback* pIPluginExecuteCB, string jsonRes)
{
	SCINFO("AfterExecute json=%s", jsonRes.c_str());
	pIPluginExecuteCB->ExecuteResult(jsonRes);
}

bool CLoadcellTask::NeedExecute(IScreenReader* pIScreenAreaReader)
{
	bool bRes = false;
	if (m_taskInfo.checkPos.checkType == SC_PLUGIN_CHECK_TYPE_RGBEQUAL)
	{
		float widthRatio, heightRatio;
		pIScreenAreaReader->GetScreenRatio(&widthRatio, &heightRatio);
		if (m_bFirstExecute)
		{
			m_lastCheckPointState = pIScreenAreaReader->GetPixel(m_taskInfo.checkPos.x * widthRatio, m_taskInfo.checkPos.y * heightRatio);
			bRes = true;
		}
		else
		{
			unsigned long dwRGB = pIScreenAreaReader->GetPixel(m_taskInfo.checkPos.x * widthRatio, m_taskInfo.checkPos.y * heightRatio);
			bool bEqual = m_ColorOCR.RGB32Equal(m_lastCheckPointState, dwRGB);
			bRes = !bEqual;
			m_lastCheckPointState = dwRGB;
		}
	}
	else
	{
		bRes = true;
	}

	return bRes;
}

string CLoadcellTask::ResultToJson(int packetID, const string& timeStamp)
{
	Json::Value data;
	data["packet_id"] = packetID;
	data["sensor_id"] = m_taskInfo.id;
	data["time"] = timeStamp.c_str();
	data["type"] = m_taskInfo.type;
	data["value"];
	
	if (m_lastResult.size() > 0)
	{
		string digit = "";
		list<DIGIT>::iterator itB = m_lastResult.begin();
		list<DIGIT>::iterator itE = m_lastResult.end();
		for (list<DIGIT>::iterator it = itB; it != itE; ++it)
		{
			
			if (it->digit == "x")
			{
#if SC_PLUGIN_NO_UNDEFINECHAR_RESULT
				digit = digit + '0';
#else
				char cID[10] = { 0 };
				_itoa_s(it->model.id, cID, 10);
				digit = digit + '[' + cID + ']';
#endif
			}
			else if (it->digit == " ")
			{
				continue;
			}
			else
			{
				digit = digit + it->digit;
			}

		}
		if (!digit.empty())
		{
			data["value"] = digit.c_str();
		}
	}
// 
// 	Json::Value root;
// 	root["data"] = data;
	
	return data.toStyledString();
}
