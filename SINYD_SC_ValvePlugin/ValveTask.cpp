/////////////////////////////////////////////////////////////////////////////
/// @File name: ValveTask.cpp
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 		
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#include "ValveTask.h"
#include "json/json.h"
#include "log.h"

CValveTask::CValveTask(PTaskInfo pTaskInfo)
	: m_bIsRunning(false)
	, m_taskInfo(*pTaskInfo)
	, m_ColorOCR()
	, m_lastCheckPointState(0)
	, m_bFirstExecute(true)
	, m_lastResult(false)
	, m_bColorInvert(false)
{
	for (int i = 0; i < m_taskInfo.paramsCount; i++)
	{
		if (m_taskInfo.pParams[i].paramName == SC_COMMON_COLOR_THRESHOLD_PARAMNAME)
		{
			int tolerance = strtoul(m_taskInfo.pParams[i].paramValue.c_str(), NULL, 10);
			m_ColorOCR.SetEqulaTolerance(tolerance);
		}
		else if (m_taskInfo.pParams[i].paramName == SC_VALVE_IS_COLOR_INVERT)
		{
			m_bColorInvert = strtoul(m_taskInfo.pParams[i].paramValue.c_str(), NULL, 10) == 1;
		}
	}
}

CValveTask::~CValveTask()
{
	if (m_taskInfo.pParams)
	{
		delete[] m_taskInfo.pParams;
		m_taskInfo.pParams = nullptr;
	}
}

int CValveTask::GetID()
{
	return m_taskInfo.id;
}

bool CValveTask::IsRunning()
{
	return m_bIsRunning;
}

void CValveTask::Execute(IScreenReader* pIScreenAreaReader, IPluginExecuteCallback* pIPluginExecuteCB, int packageID, const string& TimeStamp)
{
	m_bIsRunning = true;

	//Work
	//if (NeedExecute(pIScreenAreaReader))
	{
		unsigned long ValveRGB = 0;
		float widthRatio = 1.0, heightRatio  = 1.0;
		//pIScreenAreaReader->GetScreenRatio(&widthRatio, &heightRatio);
		bool bOpenEqual = false;
		for (int i = 0; i < m_taskInfo.paramsCount; i++)
		{
			if (m_taskInfo.pParams[i].paramName == SC_VALVE_OPEN_COLOR_PARAMNAME)
			{
				ValveRGB = strtoul(m_taskInfo.pParams[i].paramValue.c_str(), NULL, 10);
				if (m_bColorInvert)
				{
					bOpenEqual = bOpenEqual | (!m_ColorOCR.RGB32Equal(ValveRGB, pIScreenAreaReader->GetPixel(m_taskInfo.areaPos.x * widthRatio, m_taskInfo.areaPos.y * heightRatio)));
				}
				else
				{
					bOpenEqual = bOpenEqual | m_ColorOCR.RGB32Equal(ValveRGB, pIScreenAreaReader->GetPixel(m_taskInfo.areaPos.x * widthRatio, m_taskInfo.areaPos.y * heightRatio));
				}
			}
		}
		m_lastResult = bOpenEqual;
	}

	AfterExecute(pIPluginExecuteCB, ResultToJson(packageID, TimeStamp));


	m_bIsRunning = false;
	if (m_bFirstExecute)
	{
		m_bFirstExecute = false;
	}
}

void CValveTask::AfterExecute(IPluginExecuteCallback* pIPluginExecuteCB, string jsonRes)
{
	SCINFO("AfterExecute json=%s", jsonRes.c_str());
	pIPluginExecuteCB->ExecuteResult(jsonRes);
}

bool CValveTask::NeedExecute(IScreenReader* pIScreenAreaReader)
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
			unsigned long dwRGB = pIScreenAreaReader->GetPixel(m_taskInfo.checkPos.x, m_taskInfo.checkPos.y);
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

string CValveTask::ResultToJson(int packetID, const string& timeStamp)
{
	Json::Value data;
	data["packet_id"] = packetID;
	data["sensor_id"] = m_taskInfo.id;
	data["time"] = timeStamp.c_str();
	data["type"] = m_taskInfo.type;
	data["value"] = (int)m_lastResult;
// 
// 	Json::Value root;
// 	root["data"] = data;
	
	return data.toStyledString();
}
