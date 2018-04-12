/////////////////////////////////////////////////////////////////////////////
/// @File name: MeasureDef.h
/// @Author: h
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	Measure Define
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef MEASUREDEF_H
#define MEASUREDEF_H
#include <string>

extern  std::string dirpath;
extern  std::string createtime;

#define VERSION_NUMBER "5.1.0.0"
#define DESIGN_SUFFIX "_D"         //标定值后缀
#define REAL_SUFFIX "_R"           //真实计算重量值后缀
#define CALC_SUFFIX "_C"           //画面采集计算值后缀
#define TEMPERATURE_SUFFIX "_T"    //温度值后缀

#define STR_RAW_BD_NAME "raw_record"   //原始数据数据库名
#define STR_PRODUCT_BD_NAME "product"  //生产数据数据库名
#define STR_DETAIL_BD_NAME "detail"    //过程数据数据库名

#define STR_PACKET_ID  "packet_id"     //包号的列名称
#define STR_MIX_ID  "mix_id"           //盘号的列名称
#define STR_REAL_SENSOR_ID "real_sonsorid"     //秤对应sensorid的列名称
#define STR_START_TIME "start_time"    //过程数据开始时间的列名称
#define STR_END_TIME "end_time"        //过程数据结束时间的列名称

#define STR_ILLEGAL "illegall"         //违规标识的列名称
#define STR_DRY_MIX_TIME "drytime"     //干拌时长
#define STR_WET_MIX_TIME "wettime"     //湿拌时长
#define STR_MIX_TIME "mixtime"         //拌合时长
#define STR_RECORD_TIME "timestamp"    //记录时间
#define STR_RECORD_CURRENT "current"    //拌和机电流

#define AGGR1_HOPPER_ID "201"
#define AGGR2_HOPPER_ID "202"
#define AGGR3_HOPPER_ID "203"
#define AGGR4_HOPPER_ID "204"
#define AGGR5_HOPPER_ID "205"
#define AGGR6_HOPPER_ID "206"
#define POWDER1_HOPPER_ID "211"
#define POWDER2_HOPPER_ID "212"
#define POWDER3_HOPPER_ID "213"
#define POWDER4_HOPPER_ID "214"
#define POWDER5_HOPPER_ID "215"
#define POWDER6_HOPPER_ID "216"
#define ADMIXTURE1_HOPPER_ID "221"  //外添加剂
#define ADMIXTURE2_HOPPER_ID "222"
#define LIGNIN_HOPPER_ID "231"
#define ASPHALT_HOPPER_ID "232"
#define WATER_HOPPER_ID "233"
#define MIX_HOPPER_ID "101"
#define MIX_TEMPERATURE_ID "401"
#define ASPHALT_TEMPERATURE_ID "402"
#define AGGR_TEMPERATURE_ID "403"

#define CONFIG_SYS_FILE "mix_sys.conf"
#define CONFIG_ANALOG_FILE "analog_sensor.conf"
#define CONFIG_DIGITAL_FILE "digital_sensor.conf"
#define CONFIG_HOPPER_FILE "hopper.conf"
#define CONFIG_485_FILE "485_sensor.conf"
#define CONFIG_BACKUP_DIR  "config_"
#define ERROR_LOG_FILE  "error.log"

#if _WIN32
#define CONFIG_BASE_PATH  ".\\ConfigFile\\"
#define CONFIG_BACKUP_PATH  ".\\ConfigFile_RunTime\\"
#define CONFIG_RUNTIME_PATH  ".\\ConfigFile_RunTime\\"
#define CONFIG_CEMENT_PATH   ""
#define CONFIG_PATH_DELIMITER "\\"
#define CONFIG_DB_PATH ".\\DB\\"
#define CONFIG_BACKUP_DB_PATH ".\\DB\\"
#define CONFIG_LOG_PATH ".\\LOG\\"
#define CONFIG_NET_LOG_PATH ".\\ConfigFile_RunTime\\"
#else
#define CONFIG_BASE_PATH  "/opt/web/webpages/config/"
#define CONFIG_BACKUP_PATH  "/media/mmcblk0p1/config_runtime/"
#define CONFIG_CEMENT_PATH "cement/"
#define CONFIG_PATH_DELIMITER "/"
#define CONFIG_DB_PATH "/opt/database/"
#define CONFIG_BACKUP_DB_PATH "/media/mmcblk0p1/database/"
#define CONFIG_RUNTIME_PATH "/opt/config_runtime/"
#define CONFIG_LOG_PATH "/media/mmcblk0p1/log/"
//#define CONFIG_LOG_PATH "/tmp/log/"
#define CONFIG_NET_LOG_PATH CONFIG_LOG_PATH
#endif

//拌和机类型：1.沥青 2.水泥 3.改性沥青 4.储罐
enum EN_MIX_MACHINE_TYPE {
    MACHINE_INIT = 0,
    MACHINE_ASPHALT,
    MACHINE_CEMENT,
    MACHINE_MODIFIED_ASPHALT,
    MACHINE_STORAGE_TANK
};

#endif //MEASUREDEF_H
