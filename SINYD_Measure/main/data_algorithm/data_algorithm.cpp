#include "data_algorithm.h"

DataAlgorithm::DataAlgorithm()
{
}

FirstDataAlgorithm::FirstDataAlgorithm()
{
}

ST_MEASURE_VALUE FirstDataAlgorithm::Execute(vector<ST_HOPPER_VALUE> *Values, int iStartIdx, int iEndIdx)
{
	ST_MEASURE_VALUE stMeasureValue;
	stMeasureValue.value = 0.0;
	stMeasureValue.rewdata = "";
	if (iStartIdx<=0 || iStartIdx > iEndIdx) {
		DebugLog("FirstDataAlgorithm, invalided index: start=%d,end=%d\n", iStartIdx, iEndIdx);
		return stMeasureValue;
	}
	ST_HOPPER_VALUE* pData = Values->data();
	int index = -1;
	for (int i = iStartIdx-1; i < iEndIdx;i++) {
		if (!(pData + i)->rawdata.empty()) {
			index = i;
			break;
		}
	}

	if (-1 < index) {
		stMeasureValue.value = (pData + index)->value;
		stMeasureValue.rewdata = (pData + index)->rawdata;
	}
	
	return stMeasureValue;
}


EndDataAlgorithm::EndDataAlgorithm()
{
}

ST_MEASURE_VALUE EndDataAlgorithm::Execute(vector<ST_HOPPER_VALUE> *Values, int iStartIdx, int iEndIdx)
{
	ST_MEASURE_VALUE stMeasureValue;
	stMeasureValue.value = 0.0;
	stMeasureValue.rewdata = "";
	if (iEndIdx<=0) {
		DebugLog("EndDataAlgorithm, invalided index: start=%d,end=%d\n", iStartIdx, iEndIdx);
		return stMeasureValue;
	}
	ST_HOPPER_VALUE* pData = Values->data();
	int index = -1;
	for (int i = iEndIdx-1; i >= iStartIdx-1;i--) {
		if (!(pData + i)->rawdata.empty()) {
			index = i;
			break;
		}
	}
	if (-1 < index) {
		stMeasureValue.value = (pData + index)->value;
		stMeasureValue.rewdata = (pData + index)->rawdata;
	}

	return stMeasureValue;
}


MaxDataAlgorithm::MaxDataAlgorithm()
{
}

ST_MEASURE_VALUE MaxDataAlgorithm::Execute(vector<ST_HOPPER_VALUE> *Values, int iStartIdx, int iEndIdx)
{
	ST_MEASURE_VALUE stMeasureValue;
	stMeasureValue.value = 0.0;
	stMeasureValue.rewdata = "";
	ST_HOPPER_VALUE* pData = Values->data();
	ST_HOPPER_VALUE *pValue = NULL;
	int index;
	double value;

	if (iEndIdx > (int)Values->size()) {
		iEndIdx = Values->size();
	}

	if (iStartIdx<0 || iStartIdx>iEndIdx) {
		DebugLog("MaxAlgorithm invalided index: start=%d,end=%d\n", iStartIdx, iEndIdx);
		return stMeasureValue;
	}

	index = -1;
	value = 0.0;

	for (int i = iEndIdx - 1; i >= iStartIdx -1; i--) {
		pValue = pData + i;
		if (pValue->isVaildValue) {
			if (-1 == index) {
				index = i;
				value = pValue->value;
			}
			else if (value < pValue->value) {
				index = i;
				value = pValue->value;
			}
		}
		else {
			continue;
		}
	}

	if (-1 != index) {
		stMeasureValue.value = (pData + index)->value;
		stMeasureValue.rewdata = (pData + index)->rawdata;
	}
	return stMeasureValue;
}


MinDataAlgorithm::MinDataAlgorithm()
{
}

ST_MEASURE_VALUE MinDataAlgorithm::Execute(vector<ST_HOPPER_VALUE> *Values, int iStartIdx, int iEndIdx)
{
	ST_MEASURE_VALUE stMeasureValue;
	stMeasureValue.value = 0.0;
	stMeasureValue.rewdata = "";
	ST_HOPPER_VALUE* pData = Values->data();
	ST_HOPPER_VALUE *pValue;
	int index;
	double value;

	if (iEndIdx > (int)Values->size()) {
		iEndIdx = Values->size();
	}

	if (iStartIdx<0 || iStartIdx>iEndIdx) {
		DebugLog("MinAlgorithm, invalided index: start=%d,end=%d\n", iStartIdx, iEndIdx);
		return stMeasureValue;
	}

	index = -1;
	value = 0.0;

	for (int i = iEndIdx - 1; i >= iStartIdx - 1; i--) {
		pValue = pData + i;
		if (pValue->isVaildValue) {
			if (-1 == index) {
				index = i;
				value = pValue->value;
			}
			else if (value > pValue->value) {
				index = i;
				value = pValue->value;
			}
		}
		else {
			continue;
		}
	}

	if (-1 != index) {
		stMeasureValue.value = value;
		stMeasureValue.rewdata = (pData + index)->rawdata;
	}
	return stMeasureValue;
}



AverageDataAlgorithm::AverageDataAlgorithm()
{
}

ST_MEASURE_VALUE AverageDataAlgorithm::Execute(vector<ST_HOPPER_VALUE> *Values, int iStartIdx, int iEndIdx)
{
	ST_MEASURE_VALUE stMeasureValue;
	stMeasureValue.value = 0.0;
	stMeasureValue.rewdata = "";
	ST_HOPPER_VALUE* pData = Values->data();
	ST_HOPPER_VALUE *pValue;
	int index;
	double value;

	if (iEndIdx > (int)Values->size()) {
		iEndIdx = Values->size();
	}

	if (iStartIdx<0 || iStartIdx>iEndIdx) {
		DebugLog("AverageDataAlgorithm, invalided index: start=%d,end=%d\n", iStartIdx, iEndIdx);
		return stMeasureValue;
	}

	int count = 0;
	for ( int  i = iStartIdx-1; i < iEndIdx; i++ ) {
		pValue = pData + i;
		if (!pValue->isVaildValue) {
			continue;
		}
		value += pValue->value;
		count++;
	}
	
	if ( 0 < count ) {
		value = value / count;
		stMeasureValue.value = value;
		char rewdata[20] = { 0 };
		sprintf(rewdata, "%.3lf", value);
		stMeasureValue.rewdata = string(rewdata);
	}
	return stMeasureValue;
}
