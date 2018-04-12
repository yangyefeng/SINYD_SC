#pragma once
#include "..\SINYD_SC_CommonUtility\CCalibrationConfig.h"

//file name
#define Mix_Sys_Conf "mix_sys.conf"
#define Hopper_Conf "hopper.conf"
#define Analog_Sensor_Conf "analog_sensor.conf"
#define Digital_Sensor_Conf "digital_sensor.conf"

//hopper
#define HOPPER_ "hopper_"
#define MIX_MACHINE_ID "mix_machine_id"
#define HOPPER_ID "hopper_id"
#define HOPPER_NAME "hopper_name"
#define LOADING_TAP_ID_1 "loading_tap1_id"
#define LOADING_TAP_ID_2 "loading_tap2_id"
#define BLOWING_TAP_ID_1 "blowing_tap1_id"
#define BLOWING_TAP_ID_2 "blowing_tap2_id"
#define BLOWING_TAP_ID "blowing_tap_id"
#define WEIGHING_CELL_ID "weighing_cell_id"
#define BUKET_ID "bucket_id"
#define CALC_ID "calc_id"
#define SET_ID "set_id"
#define BUKET_NAME "bucket_name"
#define TEMPERATURE_ID "temperature_id"
#define MIXER_CAPTION "mixer"

//mix_sys
#define HOPPER_SENSOR "hopper_sensor"
#define ANALOGSENSOR_TOTAL "analogsensor_total"
#define WEIGHT_TOTAL "weight_total"
#define DIGITALSENSOR_TOTAL "digitalsensor_total"
#define HOPPER_TOTAL "hopper_total"
#define BUCKET_TOTAL "bucket_total"

//analog_sensor
#define ANALOGSENSOR_ "analogsensor_"
#define ANALOGSENSOR_ID "analogsensor_id"
#define ANALOGSENSOR_NAME "analogsensor_name"
#define MEASURE_METHOD "measure_method"
#define AREA_START "area_start"
#define AREA_START_DELAY "area_start_delay"
#define AREA_END "area_end"
#define AREA_END_DELAY "area_end_delay"
#define CALC_METHED "calc_method"

//digital_sensor
#define DIGITALSENSOR_ "digitalsensor_"
#define DIGITALSENSOR_ID "digitalsensor_id"
#define DIGITALSENSOR_NAME "digitalsensor_name"
#define OPEN_DELAY "open_delay"
#define CLOSE_DELAY "close_delay"

//Hopper ID
enum ENUM_HOPPER_ID {
	EN_HOPPER_ID_INIT = 0,          //0:
	EN_HOPPER_ID_AGGR1 = 201,         //201:
	EN_HOPPER_ID_AGGR2,             //202:1?ив?2
	EN_HOPPER_ID_AGGR3,             //203:1?ив?3
	EN_HOPPER_ID_AGGR4,             //204:1?ив?4
	EN_HOPPER_ID_AGGR5,             //205:1?ив?5
	EN_HOPPER_ID_AGGR6,             //206:1?ив?6
	EN_HOPPER_ID_POWDER1 = 211,       //211:бд?ив?1
	EN_HOPPER_ID_POWDER2,           //212:бд?ив?2
	EN_HOPPER_ID_POWDER3,           //213:бд?ив?3
	EN_HOPPER_ID_POWDER4,           //214:бд?ив?4
	EN_HOPPER_ID_POWDER5,           //215:бд?ив?5
	EN_HOPPER_ID_POWDER6,           //216
	EN_HOPPER_ID_ADMIX1 = 221,        //221:икa2??ив1
	EN_HOPPER_ID_ADMIX2,            //222:икa2??ив2
	EN_HOPPER_ID_LIGNIN = 231,        //231:???и║??
	EN_HOPPER_ID_ASPHALT,           //232:ивбш?ид
	EN_HOPPER_ID_WATER              //233:??
};

//hopper
typedef struct _Hopper
{
	bool isValid;
	char* machine_id;
	int hopper_id;
	CString hopper_name;
	int load_tap1_id;
	int load_tap2_id;
	int blow_tap1_id;
	int blow_tap2_id;
	int weight_cell_id;
	int bucket_id;
	int calc_id;
	int set_id;
	int temperature_id;
} HopperStru;

typedef struct _Bucket
{
	const char* bucket_serial;
	char* machine_id;
	int bucket_id;
	const char* bucket_name;
	int blow_tap1_id;
}BucketStru;

typedef struct _Mixer
{
	const char* mixer_caption;
	char* machine_id;
	int blow_tap_id;
	int temperature_id;
}MixerStru;

//analog_sensor
typedef struct _AnalogSensor
{
	//char* analog_serial;
	int analogsensor_id;
	char* analogsensor_name;
	AreaMeasureFunction function;
}AnalogSensor;

//digital_sensor
typedef struct _DigitalSensor
{
	char* digital_serial;
	int digitalsensor_id;
	char* digitalsensor_name;
	int open_delay;
	int close_delay;
}DigitalSensor;