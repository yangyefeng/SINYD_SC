/////////////////////////////////////////////////////////////////////////////
/// @File name: Operation_conf.h
/// @Author: h
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	Define Machine Type
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef OPERATIONCONF_H
#define OPERATIONCONF_H
#include "../../Include/EmsasCommon.h"
#include "../../Include/DebugLogger.h"
#include "MeasureDef.h"
#include "../../Tool/syd_time.h"
#include "../../main/data_provider.h"
#include <string.h>


////拌和机类型：1.沥青 2.水泥 3.改性沥青 4.储罐
//enum EN_MIX_MACHINE_TYPE {
//	MACHINE_INIT = 0,
//	MACHINE_ASPHALT,
//	MACHINE_CEMENT,
//	MACHINE_MODIFIED_ASPHALT,
//	MACHINE_STORAGE_TANK
//};

//计量方式
enum  EN_ANA_MEASURE_TYPE
{
	ANA_MEASURE_INIT = 0,
	ANA_MEASURE_SINGLE, //单值
	ANA_MEASURE_DIFF    //差值
};

//区域类型
enum  EN_AREA_TYPE
{
	AREA_TYPE_INIT = 0,
	AREA_INPUT_START,   //上料开始
	AREA_INPUT_END,     //上料结束
	AREA_OUTPUT_START,  //放料开始
	AREA_OUTPUT_END     //放料结束
};

//计算方式类型：1、第一个值；2、最后一个值；3、最大值；4、最小值；5、平均值
enum EN_CALC_METHOD_TYPE {
	CALC_METHOD_INIT = 0,
	CALC_METHOD_FIRST,
	CALC_METHOD_END,
	CALC_METHOD_MAX,
	CALC_METHOD_MIN,
	CALC_METHOD_AVERAGE
};

/**
@brief 用于记录一个时间点的料仓数据的结构体
*/
struct ST_HOPPER_VALUE {
	SydTime time;
	double analog;
	double value;
	float slope; //斜率，评估数据走向
	string rawdata; //原始数据
	bool isVaildValue;  //判断value是否为有效的值（false：原始数据String包含为空的时候，value为无效值）

	ST_HOPPER_VALUE(INT32 iId, DataProvider* pProvider) {
		time = pProvider->GetTimestamp();
		value = pProvider->GetAnalogData(iId);
		rawdata = pProvider->GetAnalogRawData(iId);
		analog = 0.0;
		slope = 0.0;
		if ( NULL != strstr(rawdata.c_str(), "[") || rawdata.empty() ) {
			isVaildValue = false;
		}
		else {
			isVaildValue = true;
		}
	}
	ST_HOPPER_VALUE(const ST_HOPPER_VALUE& a) {
		time = a.time;
		value = a.value;
		analog = a.analog;
		slope = a.slope;
		rawdata = a.rawdata;
		isVaildValue = a.isVaildValue;
	}
};

struct ST_MEASURE_VALUE
{
	double value;
	string rewdata;
};
#endif //OPERATIONCONF_H
