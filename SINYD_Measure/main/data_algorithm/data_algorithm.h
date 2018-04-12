/////////////////////////////////////////////////////////////////////////////
/// @File name: data_algorithm.h
/// @Author: h
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/04/13
/// @Description:
/// 	
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef DATA_ALGORITHM_H
#define DATA_ALGORITHM_H

#include <vector>
using namespace std;
#include "../../Data/Include/Operation_conf.h"
#include "../../Include/DebugLogger.h"

class DataAlgorithm
{
public:
	DataAlgorithm();

    virtual ST_MEASURE_VALUE Execute(vector<ST_HOPPER_VALUE> *Values, int iStartIdx, int iEndIdx)=0;
};

struct ST_AREA_CONF
{
	EN_AREA_TYPE startArea;
	int startDelay;
	EN_AREA_TYPE endArea;
	int endDelay;
	DataAlgorithm * pDataAlgorithm;
};

class FirstDataAlgorithm:public DataAlgorithm
{
public:
	FirstDataAlgorithm();

    virtual ST_MEASURE_VALUE Execute(vector<ST_HOPPER_VALUE> *Values, int iStartIdx, int iEndIdx);
};


class EndDataAlgorithm:public DataAlgorithm
{
public:
	EndDataAlgorithm();

    virtual ST_MEASURE_VALUE Execute(vector<ST_HOPPER_VALUE> *Values, int iStartIdx, int iEndIdx);
};


class MaxDataAlgorithm:public DataAlgorithm
{
public:
	MaxDataAlgorithm();

    virtual ST_MEASURE_VALUE Execute(vector<ST_HOPPER_VALUE> *Values, int iStartIdx, int iEndIdx);
};


class MinDataAlgorithm:public DataAlgorithm
{
public:
	MinDataAlgorithm();

    virtual ST_MEASURE_VALUE Execute(vector<ST_HOPPER_VALUE> *Values, int iStartIdx, int iEndIdx);
};


class AverageDataAlgorithm:public DataAlgorithm
{
public:
	AverageDataAlgorithm();

    virtual ST_MEASURE_VALUE Execute(vector<ST_HOPPER_VALUE> *Values, int iStartIdx, int iEndIdx);
};
#endif // DATA_ALGORITHM_H
