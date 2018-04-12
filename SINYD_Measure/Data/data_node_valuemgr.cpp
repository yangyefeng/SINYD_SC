#include "data_node_valuemgr.h"
#include "../Include/DebugLogger.h"
#include "RWData/RWData.h"
#include "MeasureDef.h"
#include <fstream>

DataNodeValueMgr::DataNodeValueMgr()
{
}

DataNodeValueMgr::~DataNodeValueMgr()
{
}

void DataNodeValueMgr::Initialize(int iAnaId)
{
#if _WIN32
	m_iAlpha = 6;
#else
	m_iAlpha = 12;
#endif
	InitSlopeAlpha();

	if (0 >= iAnaId) {
		return;
	}
	string strConfigPath = string(dirpath) + string(CONFIG_ANALOG_FILE);
	CRWData rw;
	int iCount = 0;
	char szAppName[LINESIZE] = { 0 };
	int iCanid;
	bool bIsFindAndId = false;
	while (1) {
		sprintf(szAppName, "analogsensor_%d", iCount + 1);
#if _WIN32
		iCanid = rw.GetProfileInt(szAppName, "analogsensor_id", -1, strConfigPath.c_str());
#else
        iCanid = rw.GetProfileInt(szAppName, "can_id", -1, strConfigPath.c_str());
#endif
		if (-1 == iCanid) {
			DebugLog("Not Find Canid [%d]\n", iCanid);
			break;
		}
		if (iAnaId == iCanid) {
			bIsFindAndId = true;
			break;
		}
		iCount++;
	}
	int iAlpha = 0;
	if (bIsFindAndId) {
		iAlpha = rw.GetProfileInt(szAppName, "alpha", 0, strConfigPath.c_str());
	}
	m_iAlpha = (0 < iAlpha) ? iAlpha : m_iAlpha;

	DebugLog("[DataNodeValueMgr]Initialize andid[%d],m_iAlpha=[%d]\n", iAnaId, m_iAlpha);
}

void DataNodeValueMgr::InitSlopeAlpha()
{
	CRWData rw;
	string strProudectIDPath = CONFIG_BASE_PATH;
	strProudectIDPath += "slope.conf";

	fstream fs;
	fs.open(strProudectIDPath.c_str(), ios::in);
	if (fs) {
		m_iAlpha = rw.GetProfileInt("alpha", "default", 0, (char *)strProudectIDPath.c_str());
	}
	else {
		char strAlpha[10] = {0};
		sprintf(strAlpha,"%d", m_iAlpha);
		string context = "[alpha]\ndefault=" + string(strAlpha);
		rw.CreateFile(strProudectIDPath.c_str(), context.c_str());
	}
	fs.close();
}

void DataNodeValueMgr::PushValue(const ST_HOPPER_VALUE& stHopValue)
{
	m_vecValues.push_back(stHopValue);
	const int alpha = m_iAlpha; //FIXME
	INT32 n = m_vecValues.size();

	if (n > alpha) {
		ST_HOPPER_VALUE *pValue = &(m_vecValues.back());
		pValue->slope = CalculateSlope(n - alpha, n);
	}
}

void DataNodeValueMgr::ClearValue()
{
	m_vecValues.clear();
}

int DataNodeValueMgr::GetValueCount()
{
	return m_vecValues.size();
}

vector<ST_HOPPER_VALUE> *DataNodeValueMgr::GetValue()
{
	return &m_vecValues;
}

int DataNodeValueMgr::FindValue(const SydTime& time) {
	int iStart, iThis, iEnd;
	ST_HOPPER_VALUE* pValue;

	iStart = 0;
	iEnd = m_vecValues.size() - 1;
	while (iStart <= iEnd)
	{
		iThis = (iStart + iEnd) / 2;
		pValue = m_vecValues.data() + iThis;
		if (pValue->time < time)
			iStart = iThis + 1;
		else if (pValue->time > time)
			iEnd = iThis - 1;
		else
			return iThis; /*Found*/
	}
	return -1;/*Not Found: Return -1*/
}

int DataNodeValueMgr::MinValue(int iStartIndex, int iEndIndex) {
	ST_HOPPER_VALUE* pData = m_vecValues.data();
	ST_HOPPER_VALUE *pValue;
	int index;
	double value;

	if (iEndIndex > (int)m_vecValues.size()) {
		iEndIndex = m_vecValues.size();
	}

	if (iStartIndex<0 || iStartIndex>(iEndIndex - 1)) {
		DebugLog("MinValue, invalided index: start=%d,end=%d\n", iStartIndex, iEndIndex);
		return -1;
	}

	index = iEndIndex - 1;
	value = (pData + index)->value;

	for (int i = iEndIndex - 2; i >= iStartIndex; i--) {
		pValue = pData + i;
		if (value > pValue->value) {
			index = i;
			value = pValue->value;
		}
	}

	return index;
}

int DataNodeValueMgr::MaxValue(int iStartIndex, int iEndIndex) {
	ST_HOPPER_VALUE* pData = m_vecValues.data();
	ST_HOPPER_VALUE *pValue;
	int index;
	double value;

	if (iEndIndex > (int)m_vecValues.size()) {
		iEndIndex = m_vecValues.size();
	}

	if (iStartIndex<0 || iStartIndex>(iEndIndex - 1)) {
		DebugLog("MaxValue invalided index: start=%d,end=%d\n", iStartIndex, iEndIndex);
		return -1;
	}

	index = iEndIndex - 1;
	value = (pData + index)->value;

	for (int i = iEndIndex - 2; i >= iStartIndex; i--) {
		pValue = pData + i;
		if (value < pValue->value) {
			index = i;
			value = pValue->value;
		}
	}

	return index;
}

/**
@brief 利用最小二乘法计算斜率
*/
float DataNodeValueMgr::CalculateSlope(int iStartValue, int iEndValue) {
	int count = iEndValue - iStartValue;
	if (count <= 1) {
		DebugLog("CalculateSlope: node count=%d\n", count);
		return 0.0;
	}

	ST_HOPPER_VALUE *pValue = m_vecValues.data();
	ST_HOPPER_VALUE *pValueStart = pValue + iStartValue;
	ST_HOPPER_VALUE *pValueEnd = pValue + iEndValue;

	//规范化处理
	double *xBuf = new double[count];
	double maxX, maxY, minX, minY;

	minX = 0.0;
	maxX = (pValueEnd - 1)->time - pValueStart->time;
	minY = pValueStart->value;
	maxY = minY;

	for (pValue = pValueStart + 1; pValue<pValueEnd; pValue++) {
		if (maxY < pValue->value) {
			maxY = pValue->value;
		}
		if (minY > pValue->value) {
			minY = pValue->value;
		}
	}
	if (minY == maxY) {
		delete[]xBuf;
		//DebugLog("CalculateSlope minY=maxY=%f, from=%d, to=%d\n",minY,iFromValue,iToValue);
		return 0.0;
	}

	maxY = 310000;
	double t = maxY / maxX;
	double d;

	pValue = pValueStart;
	for (int i = 0; i<count; i++)
	{
		d = pValue->time - pValueStart->time;
		xBuf[i] = t*d;
		if (i>1 && d>maxX) {
			string strThis = pValue->time.ToString();
			string strPrev = (pValue - 1)->time.ToString();
			DebugErrLog("CalculateSlope:: unexpcectd timestamp: %s was followed by %s,t=%f,maxX=%f,maxY=%f\n",
				strPrev.c_str(), strThis.c_str(), t, maxX, maxY);
			delete[]xBuf;
			return 0.0;
		}
		pValue++;
	}

	//开始计算斜率
	double t1, t2, t3, t4;

	t1 = 0.0;
	t2 = 0.0;
	t3 = 0.0;
	t4 = 0.0;
	pValue = pValueStart;
	for (int i = 0; i<count; i++) {
		t1 += xBuf[i] * xBuf[i];
		t2 += xBuf[i];
		t3 += xBuf[i] * pValue->value;
		t4 += pValue->value;
		pValue++;
	}

	double k = (t3*count - t2*t4) / (t1*count - t2*t2);
	//double b= (t1*t4-t2*t3) / (t1*n-t2*t2);
	delete[]xBuf;
	return (float)k;
}

int DataNodeValueMgr::MinSlope(int iStartIndex, int iEndIndex) {
	ST_HOPPER_VALUE* pData = m_vecValues.data();
	ST_HOPPER_VALUE *pValue;
	int index;
	double slope;

	if (iEndIndex > (int)m_vecValues.size()) {
		iEndIndex = m_vecValues.size();
	}

	if (iStartIndex<0 || iStartIndex>(iEndIndex - 1)) {
		DebugLog("MinSlope invalided index: start=%d,end=%d\n", iStartIndex, iEndIndex);
		return -1;
	}

	index = iEndIndex - 1;
	slope = (pData + index)->slope;

	for (int i = iEndIndex - 2; i >= iStartIndex; i--) {
		pValue = pData + i;
		if (slope > pValue->slope) {
			index = i;
			slope = pValue->slope;
		}
	}

	return index;
}

int DataNodeValueMgr::FindValueAfter(const SydTime& time)
{
	int iStart, iThis, iEnd;
	ST_HOPPER_VALUE* pValue;

	iStart = 0;
	iEnd = m_vecValues.size() - 1;
	while (iStart <= iEnd)
	{
		iThis = (iStart + iEnd) / 2;
		pValue = m_vecValues.data() + iThis;
		if (iThis == 0 || iThis == iEnd) {
			return iThis;
		}
		else if (pValue->time == time) {
			return iThis + 1;
		}
		else if ((pValue + 1)->time == time) {
			return iThis + 2;
		}
		else if (pValue->time < time && (pValue + 1)->time < time)
			iStart = iThis + 1;
		else if (pValue->time > time && (pValue - 1)->time > time)
			iEnd = iThis - 1;
		else if (pValue->time < time && (pValue + 1)->time > time) {
			return iThis + 2;
					}
		else if (pValue->time > time && (pValue - 1)->time < time)
		{
			return iThis + 1;
		}
		else
			return iThis; /*Found*/
	}

	return 0;
}