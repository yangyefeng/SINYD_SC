#ifndef _Protocol_Def_H_
#define _Protocol_Def_H_

#define ROOT_DATASRC "source"
#define ROOT_CONTENT "content"
#define SECOND_INFO "info"
#define SECOND_CONTENT_DATA "content_data"
#define THIRD_DATA "data"
#define SECOND_TYPE "type"
#define CODE_ID "code_id"
#define NUM_ID "num_id"

//Json数据头信息
enum EN_DATASRC_INFO
{
    DATASRC_INFO_MIXID = 0,    //拌和机ID
	DATASRC_INFO_MIXTYPE       //拌和站类型
};

enum EN_DATA_INFO
{
	DATA_INFO_PACKETID = 0,  //包id
	DATA_INFO_SENSORID,      //信号量id
	DATA_INFO_VALUE,         //值
	DATA_INFO_TIME,          //时间
	DATA_INFO_TYPE           //信号量类型
};

enum EN_CONTENT_INFO
{
	CONTENT_INFO_CONTENT_TYPE = 0,   //Json的Content类型
	CONTENT_INFO_DATA_TYPE           //Json的Data类型
};

//生产数据（盘数据）对应字符表id
enum EN_PRODECT_INFO
{
	PRODECT_INFO_MIXTURE_ID = 16,  //盘号
	PRODECT_INFO_TIME,             //记录时间
	PRODECT_INFO_DRYMIXING_TIME,   //干拌时间
	PRODECT_INFO_WETMIXING_TIME,   //湿拌时间
	PRODECT_INFO_MIXING_TEMP,      //混合料温度
	PRODECT_INFO_ASPHALT_TEMP,     //沥青温度
	PRODECT_INFO_AGGR_TEMP,        //骨料温度
	PRODECT_INFO_ILLEGAL           //违规标识
};

enum EN_CONTENT_TYPE
{
	CONTENT_TYPE_INIT = 0,
	CONTENT_TYPE_DB,          //传输DB数据
	CONTENT_TYPE_CONFIG,      //传输配置文件数据
	CONTENT_TYPE_TEXT         //传输字模数据
};

enum EN_DATA_TYPE
{
	DATA_TYPE_INIT = 0,
	DATA_TYPE_MIXDATA,          //盘数据信息
	DATA_TYPE_SRCDATA,          //源数据信息
	DATA_TYPE_CONFIG_FONT,      //字模配置信息
	DATA_TYPE_HOPPER            //料仓配置信息
};


#endif	//_Protocol_Def_H_