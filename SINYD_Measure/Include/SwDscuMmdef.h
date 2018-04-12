/////////////////////////////////////////////////////////////////////////////
/// @File name: SwDscuMmdef.h
/// @Author: h
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	Common define/struct
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef SWDSCUMMDEF_H
#define SWDSCUMMDEF_H
#include "EmsasCommon.h"

#define ANALOG_CAN_SENSOR_SIZE 30    //模拟信号最大数
#define DIGITAL_CAN_SENSOR_SIZE 70   //数字信号最大数

#define MIX_ID_TO_NODE_ID(mixerId)      ((mixerId) +99000)
#define BKT_ID_TO_NODE_ID(bucketId)     ((bucketId)+98000)
#define HPR_ID_TO_NODE_ID(hopperId)     ((hopperId)+97000)
#define ANA_ID_TO_NODE_ID(canId)        ((canId)   +80000)
#define DIG_ID_TO_NODE_ID(canId,channel)  ((canId)*100+channel)
#define NOANA_ID_TO_NODE_ID(hopId)      ((hopId) +110000)

#define NODE_ID_TO_MIX_ID(nodeId)       ( (nodeId)-99000 )
#define NODE_ID_TO_BKT_ID(nodeId)       ( (nodeId)-98000)
#define NODE_ID_TO_HPR_ID(nodeId)       ( (nodeId)-97000 )
#define NODE_ID_TO_ANA_ID(nodeId)       ( (nodeId)-80000 )
#define NODE_ID_TO_DIG_ID(nodeId)       ( (nodeId)/100 )
#define NODE_ID_TO_DIG_CHANNEL(nodeId)  ( (nodeId)%100 )
#define NODE_ID_TO_NOANA_ID(nodeId)       ( (nodeId)-110000 )

#define IS_MIX_NODE(nodeId)   ( (nodeId)>99000 && (nodeId)<100000 )
#define IS_BKT_NODE(nodeId)   ( (nodeId)>98000 && (nodeId)<99000 )
#define IS_HPR_NODE(nodeId)   ( (nodeId)>97000 && (nodeId)<98000 )
#define IS_ANA_NODE(nodeId)   ( (nodeId)>80000 && (nodeId)<81000 )
#define IS_DIG_NODE(nodeId)   ( (nodeId)>100 && (nodeId)<80000 )
#define IS_NOANA_NODE(nodeId)   ( (nodeId)>110000 && (nodeId)<111000 )

#define REAL_WEIGHT_TPYE 100      //真实计算重量值类型
#define CALC_VALUE_TPYE 200       //画面采集计算值类型
#define DEAL_VALUE_TPYE 300       //标定值类型
#define TEMPERATURE_TPYE 400      //温度值类型

typedef struct _ST_CAN_SENSOR_DATA {
    INT32 id;           //传感器id
    INT32 value;        //Can总线对应的传感器的Value
} ST_CAN_SENSOR_DATA;

typedef struct _ST_RECEIVE_CAN_DATA {
    ST_CAN_SENSOR_DATA st_can_digital_sendata[DIGITAL_CAN_SENSOR_SIZE];//数字传感器对应Can总线的Data值
    ST_CAN_SENSOR_DATA st_can_analog_sendata[ANALOG_CAN_SENSOR_SIZE]; //模拟传感器对应Can总线的Data值
    INT32 itran_id;                               //Can总线的传输号，通过传输号判断Can数据是否新的传输值
} ST_RECEIVE_CAN_DATA;


#endif //SWDSCUMMDEF_H
