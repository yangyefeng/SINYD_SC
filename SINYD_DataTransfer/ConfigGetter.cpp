#include "ConfigGetter.h"
#include "MeasureDef.h"

#define DIG_ID_TO_NODE_ID(canId,channel)  ((canId)*100+channel)

CConfigGetter::CConfigGetter()
{
	m_collect_type.clear();
	m_is_admixture_into_water = 0;
	m_map_device_type.clear();
}


CConfigGetter::~CConfigGetter()
{
	m_map_device_type.clear();
}

void CConfigGetter::Read(string path, int collect_type, string &outConfigStr)
{
	outConfigStr.clear();
	if (1 == collect_type) { //界面采集
		m_collect_type = string("SC");
	}
	else if (2 == collect_type) {//传感器采集
		m_collect_type = string("SS");
	}
	else {
		//入参的采集类别错误
		return;
	}
	m_root_path = path;

	outConfigStr += ReadContent();
}
string CConfigGetter::ReadContent()
{
	string ret_content;
	//content
	ret_content += LoadHopper();
	ret_content += LoadCommon();
	ret_content += LoadMeasures();
	ret_content += LoadTransmission();
#if _WIN32
#else
	ret_content += LoadSensorInfo();
#endif

	return ret_content;
}

#if 0
string CConfigGetter::ReadHeader()
{
	string headerpath = m_root_path + string(CONFIG_SYS_FILE);
	string ret_heaer;
	CTRWData rw;
	char value[255] = { 0 };
	ret_heaer = string("header:{");

	//项目ID
	rw._GetProfileString("mix_station_conf", "mix_project_name", "", value, headerpath.c_str());
	ret_heaer += ToValueString(string("PROJ_ID"), value);
	ret_heaer += string(",");
	m_proj_id = string(value);

	//标段ID
	value[0] = 0;
	rw._GetProfileString("mix_station_conf", "mix_stationname", "", value, headerpath.c_str());
	ret_heaer += ToValueString(string("SECT_ID"), value);
	ret_heaer += string(",");
	m_sect_id = string(value);

	//设备ID,即拌和机的设备编号
	value[0] = 0;
	rw._GetProfileString("mix_machine_1", "mix_machine_device_id", "", value, headerpath.c_str());
	ret_heaer += ToValueString(string("EQU_ID"), value);
	ret_heaer += string(",");
	m_machine_id = string(value);

	//拌和机类型：水泥(CMP)、沥青(AMP)
	string mix_type;
	int mtype = rw._GetProfileInt("mix_machine_1", "mix_machine_type", -1, headerpath.c_str());
	if (1 == mtype) {//沥青
		m_machine_type = 0;
		mix_type = string("AMP");
	} else if ( 2 == mtype){//水泥
		m_machine_type = 1;
		mix_type = string("CMP");
	}
	ret_heaer += ToValueString(string("MIX_TYPE"), mix_type);
	ret_heaer += string(",");

	//采集类型:SC:界面采集;SS：传感器采集
	ret_heaer += ToValueString(string("COLLECT_TYPE"), m_collect_type);
	ret_heaer += string(",");

	//数据类型：盘数据（PRODUCT）、过程数据（RAW/RAW0）、关系数据（MAPPING）、配置数据（CONFIG）
	ret_heaer += ToValueString(string("DATA_TYPE"), string("CONFIG"));
	ret_heaer += string(",");

	//配置文件ID
	m_config_id = m_root_path.substr(m_root_path.size()-31,30);
	ret_heaer += ToValueString(string("CONFIG_ID"), m_config_id);
	ret_heaer += string(",");

	//表名:sys_configuration
	ret_heaer += ToValueString(string("TABLE_NAME"), string("sys_configuration"));
	ret_heaer += string(",");

	//列名组
	ret_heaer += ToValueString(string("COL_NAME_ARR"), string("id,key,content,struct_ver,collection_type,machine_id,station_id,proj_id,sect_id,machine_type,create_time"));
	ret_heaer += string(",");

	//列名组
	ret_heaer += ToValueString(string("COL_TYPE_ARR"), string("VCHAR(50),VCHAR(50),TEXT,VCHAR(30),VCHAR(30),VCHAR(50),VCHAR(50),VCHAR(100),VCHAR(100),INT(4),TIME(3)"));
	ret_heaer += string(",};");

	return ret_heaer;
}

string CConfigGetter::ReadContent()
{
	string ret_content = string("body:");
	ret_content += (m_config_id + string(","));//id
	ret_content += (string("") + string(","));//key
	//content
	ret_content += LoadHopper();
	ret_content += LoadCommon();
	ret_content += LoadMeasures();
	ret_content += LoadTransmission();
#if _WIN32
#else
	ret_content += LoadSensorInfo();
#endif
	ret_content += string(",");
	ret_content += (string("") + string(","));//struct_ver
	ret_content += (m_collect_type + string(","));//collection_type
	ret_content += (m_machine_id + string(","));//machine_id
	ret_content += (string("") + string(","));//station_id
	ret_content += (m_proj_id + string(","));//proj_id
	ret_content += (m_sect_id + string(","));//sect_id
	ret_content += (to_string(m_machine_type) + string(","));//machine_type
	string time = m_config_id.substr(m_config_id.size() - 23, 23);
	//tm tm_ = { 0 };
	//int msec;
	//sscanf(time.c_str(), "%d-%d-%d_%d_%d_%d_%d", &tm_.tm_year, &tm_.tm_mon, &tm_.tm_mday, &tm_.tm_hour, &tm_.tm_min, &tm_.tm_sec, &msec);
	//string create_time = to_string(tm_.tm_year) + string("-") + to_string(tm_.tm_mon);
	time = time.replace(time.find("_"), 1, " ");
	time = time.replace(time.find("_"), 1, ":");
	time = time.replace(time.find("_"), 1, ":");
	time = time.replace(time.find("_"), 1, ".");
	ret_content += (time);//create_time

	return ret_content;
}
#endif

string CConfigGetter::LoadCommon()
{
	string ret = string("common:{");
	ret += (ToValueString(string("project"), string("")) + string(";"));
	ret += (ToValueString(string("tenders"), string("")) + string(";"));
	CTRWData rw;
	char value[255] = { 0 };
	string compath = m_root_path + string(CONFIG_SYS_FILE);
	rw._GetProfileString("mix_machine_1", "mix_station_id", "", value, compath.c_str());
	ret += (ToValueString(string("station_id"), value) + string(";"));
	ret += (ToValueString(string("proj_id"), m_proj_id) + string(";"));
	ret += (ToValueString(string("sect_id"), m_sect_id) + string(";"));
	ret += (ToValueString(string("mixing_machine_id"), m_machine_id) + string(";"));
	ret += (ToValueString(string("mixing_machine_type"), m_machine_type) + string(";"));
	ret += (ToValueString(string("mixing_machine_name"), string("")) + string(";"));
	ret += (ToValueString(string("is_admixture_into_water"), m_is_admixture_into_water) + string(";"));

	ret += string("};");
	return ret;
}

string CConfigGetter::LoadHopper() 
{
	CTRWData rw;
	int count = 0;
	char szAppName[LINESIZE] = { 0 };
	int hopid;
	string ret;
	string hopper;
	char value[255] = { 0 };
	string hop_path = m_root_path + string(CONFIG_HOPPER_FILE);
	int node_id, iCanId, iChannel;
	string tap, ana;
	ST_CONFIG_HOPPER st_chopper;
	map<int, ST_CONFIG_HOPPER> mapHop;
#if _WIN32
	while (1) {
		sprintf(szAppName, "hopper_%d", count + 1);
		hopid = rw._GetProfileInt(szAppName, "hopper_id", -1, hop_path.c_str());
		if (hopid == -1) {
			break;
		}
#else
	int num = 0;
	int hop_id[] = { 201,202,203,204,205,206,211,212,213,214,215,221,222,231,232,233,401,402,403 };
	printf("sizeof:%d\n", sizeof(hop_id) / sizeof(int));
	while (num < 19) {
		sprintf(szAppName, "hopper_%d", hop_id[num]);
		num++;

		hopid = rw._GetProfileInt(szAppName, "hopper_id", -1, hop_path.c_str());
		if (hopid == -1) {
			continue;
		}
#endif
		st_chopper.clear();
		count++;
		hopper = string("{");
		//hopper id
		hopper += ToValueString(string("hopper_id"), hopid);
		hopper += string(";");
        st_chopper.id = GetString(hopid);

		//in
#if _WIN32
		iCanId = rw._GetProfileInt(szAppName, "loading_tap1_id", -1, hop_path.c_str());
		iChannel = 0;
#else
		iCanId = rw._GetProfileInt(szAppName, "loading_tap1_can_id", -1, hop_path.c_str());
		iChannel = rw._GetProfileInt(szAppName, "loading_tap1_channel_id", -1, hop_path.c_str());
#endif
		node_id = (iCanId > 0) ? DIG_ID_TO_NODE_ID(iCanId, iChannel) : 0;
        tap = (0 == node_id) ? "" : GetString(node_id);
#if _WIN32
		iCanId = rw._GetProfileInt(szAppName, "loading_tap2_id", -1, hop_path.c_str());
		iChannel = 0;
#else
		iCanId = rw._GetProfileInt(szAppName, "loading_tap2_can_id", -1, hop_path.c_str());
		iChannel = rw._GetProfileInt(szAppName, "loading_tap2_channel_id", -1, hop_path.c_str());
#endif
		node_id = (iCanId > 0) ? DIG_ID_TO_NODE_ID(iCanId, iChannel) : 0;
        tap = tap + ((0 == node_id) ? "" : ( (tap.empty())?(GetString(node_id)):(string("-") + GetString(node_id))) );
		hopper += ToValueString(string("in"), tap);
		hopper += string(";");
		st_chopper.in = tap;

		//out
#if _WIN32
		iCanId = rw._GetProfileInt(szAppName, "blowing_tap1_id", -1, hop_path.c_str());
		iChannel = 0;
#else
		iCanId = rw._GetProfileInt(szAppName, "blowing_tap1_can_id", -1, hop_path.c_str());
		iChannel = rw._GetProfileInt(szAppName, "blowing_tap1_channel_id", -1, hop_path.c_str());
#endif
		node_id = (iCanId > 0) ? DIG_ID_TO_NODE_ID(iCanId, iChannel) : 0;
        tap = (0 == node_id) ? "" : GetString(node_id);
#if _WIN32
		iCanId = rw._GetProfileInt(szAppName, "blowing_tap2_id", -1, hop_path.c_str());
		iChannel = 0;
#else
		iCanId = rw._GetProfileInt(szAppName, "blowing_tap2_can_id", -1, hop_path.c_str());
		iChannel = rw._GetProfileInt(szAppName, "blowing_tap2_channel_id", -1, hop_path.c_str());
#endif
		node_id = (iCanId > 0 && iChannel > 0) ? DIG_ID_TO_NODE_ID(iCanId, iChannel) : 0;
        tap = tap + ((0 == node_id) ? "" : ((tap.empty()) ? (GetString(node_id)) : (string("-") + GetString(node_id))));
		hopper += ToValueString(string("out"), tap);
		hopper += string(";");
		st_chopper.out = tap;

		//bucket tap
		iCanId = rw._GetProfileInt(szAppName, "bucket_id", -1, hop_path.c_str());
#if _WIN32
		node_id = (iCanId > 0) ? DIG_ID_TO_NODE_ID(iCanId, 0) : 0;
		tap = (0 == node_id) ? "" : to_string(node_id);
#else
		tap = LoadSensorBucket(iCanId, hop_path);
#endif
		hopper += ToValueString(string("bucket"), tap);
		hopper += string(";");
		st_chopper.bucket = tap;
		if (233 == hopid && !tap.empty()) {
			m_is_admixture_into_water = 1;
		}

		//real
		node_id = rw._GetProfileInt(szAppName, "weighing_cell_id", -1, hop_path.c_str());
        ana = (0 == node_id) ? "" : GetString(node_id);
		hopper += ToValueString(string("real"), ana);
		hopper += string(";");
		node_id = rw._GetProfileInt(szAppName, "measure_mode", 0, hop_path.c_str());//获取计量设备类型：0：秤；1：风机
		if (0 < node_id) {
			node_id = rw._GetProfileInt(szAppName, "measure_unit", 0, hop_path.c_str());//获取吹风系数
			m_map_device_type.insert(pair<string, int>(ana,node_id));
		}

		//calc
		node_id = rw._GetProfileInt(szAppName, "calc_id", 0, hop_path.c_str());
        ana = (0 == node_id) ? "" : GetString(node_id);
		hopper += ToValueString(string("calc"), ana);
		hopper += string(";");

		//deal
		node_id = rw._GetProfileInt(szAppName, "set_id", 0, hop_path.c_str());
        ana = (0 == node_id) ? "" : GetString(node_id);
		hopper += ToValueString(string("deal"), ana);
		hopper += string(";");

		hopper += string("};");
		ret += hopper;
#if _WIN32
		node_id = rw._GetProfileInt(szAppName, "temperature_id", -1, hop_path.c_str());
		st_chopper.tp_id = (0 == node_id) ? "" : to_string(node_id);
#endif

		mapHop.insert(pair<int, ST_CONFIG_HOPPER>(hopid, st_chopper));
	}
#if _WIN32
	ret += LoadSCTemp(mapHop);
#endif
	ret += LoadMixer(hop_path,mapHop);
	ret = string("hoppers:[") + ret + string("];");

	return ret;
}
string CConfigGetter::LoadSensorBucket(int bucket_id,string path)
{
	CTRWData rw;
	int node_id, iCanId, iChannel;
	string tap, ret;
	char bucketAppName[LINESIZE] = { 0 };
	sprintf(bucketAppName, "bucket_%d", bucket_id);

	iCanId = rw._GetProfileInt(bucketAppName, "blowing_tap1_can_id", 0, path.c_str());
	iChannel = rw._GetProfileInt(bucketAppName, "blowing_tap1_channel_id", 0, path.c_str());
	node_id = (iCanId > 0) ? DIG_ID_TO_NODE_ID(iCanId, iChannel) : 0;
    tap = (0 == node_id) ? "" : GetString(node_id);
	iCanId = rw._GetProfileInt(bucketAppName, "blowing_tap2_can_id", 0, path.c_str());
	iChannel = rw._GetProfileInt(bucketAppName, "blowing_tap2_channel_id", 0, path.c_str());
	node_id = (iCanId > 0) ? DIG_ID_TO_NODE_ID(iCanId, iChannel) : 0;
    tap = tap + ((0 == node_id) ? "" : ((tap.empty()) ? (GetString(node_id)) : (string("-") + GetString(node_id))));

	return tap;
}
string CConfigGetter::LoadMixer(string path,map<int, ST_CONFIG_HOPPER> &mapHop)
{
	string ret = string("{");
	ret += (ToValueString(string("hopper_id"), string("101")) + string(";"));
	ret += (ToValueString(string("in"), string("")) + string(";"));

	int node_id, iCanId, iChannel;
	CTRWData rw;
#if _WIN32
	iCanId = rw._GetProfileInt("mixer", "blowing_tap_id", 0, path.c_str());
	iChannel = 0;
#else
	iCanId = rw._GetProfileInt("mixer", "blowing_tap_can_id", 0, path.c_str());
	iChannel = rw._GetProfileInt("mixer", "blowing_tap_id", 0, path.c_str());
#endif
	node_id = (iCanId > 0) ? DIG_ID_TO_NODE_ID(iCanId, iChannel) : 0;
	ret += (ToValueString(string("out"), node_id) + string(";"));
	ret += (ToValueString(string("bucket"), string("")) + string(";"));
	ret += (ToValueString(string("real"), string("")) + string(";"));
	ret += (ToValueString(string("calc"), string("")) + string(";"));
	ret += (ToValueString(string("deal"), string("")) + string(";"));

	ret += string("};");
#if _WIN32
	int tp_id = rw._GetProfileInt("mixer", "temperature_id", 0, path.c_str());
	map<int, ST_CONFIG_HOPPER>::iterator it;
	it = mapHop.find(232);
	if (0 < tp_id && it != mapHop.end()) {
		string hop = string("{");
		hop += (ToValueString(string("hopper_id"), string("401")) + string(";"));
		hop += (ToValueString(string("in"), it->second.out) + string(";")); //混合料温度上料阀门是沥青的放料阀门
		hop += (ToValueString(string("out"), node_id) + string(";"));
		hop += (ToValueString(string("bucket"), string("")) + string(";"));
		hop += (ToValueString(string("real"), tp_id) + string(";"));
		hop += (ToValueString(string("calc"), string("")) + string(";"));
		hop += (ToValueString(string("deal"), string("")) + string(";"));
		hop += string("};");
		ret += hop;
	}
#endif
	return ret;
}

string CConfigGetter::LoadSCTemp(map<int, ST_CONFIG_HOPPER> &mapHop)
{
	string hop,asp_out;
	string ret;
	map<int, ST_CONFIG_HOPPER>::iterator it;
	it = mapHop.find(232);
	if (it != mapHop.end() && !it->second.tp_id.empty()) {
		hop = string("{");
		hop += (ToValueString(string("hopper_id"), string("402")) + string(";"));
		hop += (ToValueString(string("in"), it->second.in) + string(";"));
		hop += (ToValueString(string("out"), it->second.out) + string(";"));
		asp_out = it->second.out;
		hop += (ToValueString(string("bucket"), string("")) + string(";"));
		hop += (ToValueString(string("real"), it->second.tp_id) + string(";"));
		hop += (ToValueString(string("calc"), string("")) + string(";"));
		hop += (ToValueString(string("deal"), string("")) + string(";"));
		hop += string("};");
	}
	ret += hop;
	hop.clear();

	bool b_find = false;
	string intap,ana;
	for (it = mapHop.begin();it != mapHop.end();it++) {
		if ( 201 <= it->first && 206 >= it->first && !it->second.tp_id.empty()) {
			if (!b_find) { //找到骨料温度
				b_find = true;
				ana = it->second.tp_id;
			}
			if (it->second.bucket.empty()) {
				intap = (intap.empty())?it->second.out:(intap+string("-")+ it->second.out);
			}
			else {
				intap = it->second.bucket;
			}
		}
	}
	if (b_find) {
		hop = string("{");
		hop += (ToValueString(string("hopper_id"), string("403")) + string(";"));
		hop += (ToValueString(string("in"), intap) + string(";"));
		hop += (ToValueString(string("out"), asp_out) + string(";")); //骨料温度的out阀门是沥青的放料阀门
		hop += (ToValueString(string("bucket"), string("")) + string(";"));
		hop += (ToValueString(string("real"), ana) + string(";"));
		hop += (ToValueString(string("calc"), string("")) + string(";"));
		hop += (ToValueString(string("deal"), string("")) + string(";"));
		hop += string("};");
	}
	ret += hop;
	return ret;
}

string CConfigGetter::LoadSensorInfo()
{
	string ret = string("sensors_info:{ ");
	CTRWData rw;
	char szBuf[LINESIZE] = { 0 };
	//m_root_path = string("D:\\src\\zh3\\cgq\\config_2016-09-03_11_34_56_133\\");//TBD
	string strAnaConfig = m_root_path + string(CONFIG_ANALOG_FILE);
	int iCount = 0;
	char szAppName[LINESIZE] = { 0 };
	char szKeyName[LINESIZE] = { 0 };
	int iCanid, ivalue, imode;
	string info, tmp;
	ret = string("sensors_info:{");
	rw._GetProfileString("network", "ip", "", szBuf, (m_root_path + string(CONFIG_SYS_FILE)).c_str());
	ret += (ToValueString(string("master_ip"), szBuf) + string(";")); //
	ret += (ToValueString(string("master_port"), string("")) + string(";")); //
	rw._GetProfileString("mix_machine_1", "can_sync_time", "", szBuf, (m_root_path + string(CONFIG_SYS_FILE)).c_str());
	ret += (ToValueString(string("collect_freq"), szBuf) + string(";")); //
	ret += string("sensors:["); //

	while (1) {
		sprintf(szAppName, "analogsensor_%d", iCount + 1);
		iCanid = rw._GetProfileInt(szAppName, "can_id", -1, strAnaConfig.c_str());

		if (-1 == iCanid) {
			printf("Not Find Canid [%d]\n", iCount + 1);
			break;
		}
		iCount++;
		info += string("{");

		info += (ToValueString(string("sensor_id"), iCanid) + string(";"));
		info += (ToValueString(string("sensor_type"), 3) + string(";")); // 模拟量
		info += (ToValueString(string("can_id"), iCanid) + string(";"));
		info += (ToValueString(string("channel_id"), string("")) + string(";"));
		rw._GetProfileString(szAppName, "senser_pga", "", szBuf, strAnaConfig.c_str());
		info += (ToValueString(string("pga"), string(szBuf)) + string(";"));//增益
		szBuf[0] = 0;
		rw._GetProfileString(szAppName, "samplerate", "", szBuf, strAnaConfig.c_str());
		info += (ToValueString(string("samplerate"), string(szBuf)) + string(";"));//采样率
		szBuf[0] = 0;
		rw._GetProfileString(szAppName, "filter_coefficient", "", szBuf, strAnaConfig.c_str());
		info += (ToValueString(string("filter"), string(szBuf)) + string(";"));//滤波参数
		info += string("calibrations:[");//标定
		for (int i = 0; i < 6;i++) {
			info += string("{");
			sprintf(szKeyName, "calibration%d", (i + 1));
			rw._GetProfileString(szAppName, szKeyName, "0", szBuf, strAnaConfig.c_str());
			info += (ToValueString(string("weight_value"), string(szBuf)) + string(";"));
			sprintf(szKeyName, "calib_value%d", (i + 1));
			rw._GetProfileString(szAppName, szKeyName, "0", szBuf, strAnaConfig.c_str());
			info += (ToValueString(string("ad_value"), string(szBuf)) + string(";"));
			info += string("};");
		}

		info += string("];");


		info += string("};");
	}
	ret += info;
	ret += string("];};");
	return ret;
}

string CConfigGetter::LoadMeasures()
{
	CTRWData rw;
	char szBuf[LINESIZE] = {0};

	string strAnaConfig = m_root_path + string(CONFIG_ANALOG_FILE);
	int iCount = 0;
	char szAppName[LINESIZE] = { 0 };
	int iCanid,ivalue,imode;
	string measure,tmp;

	while (1) {
		sprintf(szAppName, "analogsensor_%d", iCount + 1);
#if _WIN32
		iCanid = rw._GetProfileInt(szAppName, "analogsensor_id", -1, strAnaConfig.c_str());
#else
		iCanid = rw._GetProfileInt(szAppName, "can_id", -1, strAnaConfig.c_str());
#endif
		if (-1 == iCanid) {
			printf("Not Find Canid [%d]\n", iCount + 1);
			break;
		}
		iCount++;
		measure += string("{");
		measure += (ToValueString(string("sensor_id"), iCanid) + string(";"));
		measure += (ToValueString(string("method"), 3) + string(";"));//计量设置模式:0：减量计量；1：增量计量；2：温度计量；3：自定义
		ivalue = rw._GetProfileInt(szAppName, "measure_method", -1, strAnaConfig.c_str());
		if (1 == ivalue) {
			imode = 1;
		} else if(2==ivalue){
			imode = 0;
		} else {
			imode = -1;
		}
		tmp = GetMeasure(strAnaConfig, string(szAppName), ivalue);
		measure += (ToValueString(string("mode"), imode) + string(";")); //计量方式:0：差值计量；1：单值计量

		rw._GetProfileString("alpha", "default", "", szBuf,(string(m_root_path)+string("slope.conf")).c_str());
		measure += (ToValueString(string("alpha"), szBuf) + string(";")); //斜率alpha值,default：12（传感器）/6（界面）

		//计量设备类型（0：秤；1：风机）和吹风系数
        map<string, int>::iterator it = m_map_device_type.find(GetString(iCanid));
		if (it != m_map_device_type.end()) {
			measure += (ToValueString(string("device_type"), 1) + string(";"));
			measure += (ToValueString(string("blowing_coef"), it->second) + string(";"));
		}
		else {
			measure += (ToValueString(string("device_type"), 0) + string(";"));
			measure += (ToValueString(string("blowing_coef"), string("")) + string(";"));
		}
		
		//value_modes
		if (!tmp.empty()) {
			measure += string("value_modes:[");
			measure += tmp;
			measure += string("];");
		}
		measure += string("};");
	}

	string ret = string("measures:[") +measure+ string("];");
	return ret;
}

string CConfigGetter::GetMeasure(string path, string app_name, int iCount)
{
	string ret;
	CTRWData rw;
	int iCalcType,iTiming,iDelay;
	char szKeyName[30] = { 0 };
	for (int i = 1; i <= iCount; i++) {
		ret += string("{");
		sprintf(szKeyName, "value%d_start_at", i);
		iTiming = rw._GetProfileInt(app_name.c_str(), szKeyName, 0, path.c_str());
		ret += (ToValueString(string("start_timing"), iTiming) + string(";"));
		sprintf(szKeyName, "value%d_start_delay", i);
		iDelay = rw._GetProfileInt(app_name.c_str(), szKeyName, 0, path.c_str());
		ret += (ToValueString(string("start_delay"), iDelay) + string(";"));
		

		sprintf(szKeyName, "value%d_end_at", i);
		iTiming = rw._GetProfileInt(app_name.c_str(), szKeyName, 0, path.c_str());
		ret += (ToValueString(string("end_timing"), iTiming) + string(";"));
		sprintf(szKeyName, "value%d_end_delay", i);
		iDelay = rw._GetProfileInt(app_name.c_str(), szKeyName, 0, path.c_str());
		ret += (ToValueString(string("end_delay"), iDelay) + string(";"));

		sprintf(szKeyName, "value%d_calc_method", i);
		iCalcType = rw._GetProfileInt(app_name.c_str(), szKeyName, 0, path.c_str());
		ret += (ToValueString(string("calc_method"), ConvertMethod(iCalcType)) + string(";"));

		ret += string("};");
	}
	return ret;
}

int CConfigGetter::ConvertMethod(int icalc)
{
	if (1 == icalc) { //第一个值
		return 3;
	}
	else if (2 == icalc) {//最后一个值
		return 4;
	}
	else if (3 == icalc) {//最大值
		return 0;
	}
	else if (4 == icalc) {//最小值
		return 1;
	}
	else if (5 == icalc) {//平均值
		return 2;
	}
	else { //无效值
		return -1;
	}
}

string CConfigGetter::LoadTransmission()
{
	string ret = string("transmission:{");
	CTRWData rw;
	char value[255] = { 0 };
	string compath = m_root_path + string(CONFIG_SYS_FILE);
	rw._GetProfileString("mix_machine_1", "mix_station_id", "", value, compath.c_str());
	ret += (ToValueString(string("station_id"), value) + string(";"));
	ret += (ToValueString(string("mixing_machine_id"), m_machine_id) + string(";"));
	value[0] = 0;
	rw._GetProfileString("modbus_server", "ip", "", value, compath.c_str());
	ret += (ToValueString(string("ip"), value) + string(";"));
	value[0] = 0;
	rw._GetProfileString("modbus_server", "port", "", value, compath.c_str());
	ret += (ToValueString(string("port"), value) + string(";"));
	ret += (ToValueString(string("conn_timeout"), string("")) + string(";"));
	ret += (ToValueString(string("snd_timeout"), string("")) + string(";"));
	ret += (ToValueString(string("rcv_timeout"), string("")) + string(";"));
	ret += (ToValueString(string("config_version"), string("")) + string(";"));
	ret += (ToValueString(string("is_transmit_result_data"), string("1")) + string(";"));
	ret += (ToValueString(string("is_transmit_process_data"), string("1")) + string(";"));
	ret += (ToValueString(string("is_transmit_relation_data"), string("1")) + string(";"));
	ret += (ToValueString(string("is_transmit_img_data"), string("")) + string(";"));

	ret += string("};");
	return ret;
}

string CConfigGetter::GetString(const int i)
{
    stringstream newstr;
    newstr << i;
    return newstr.str();
}
