#ifndef _CONFIG_GETTER_H_
#define _CONFIG_GETTER_H_
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include "TRWData.h"

using namespace std;

template <typename T>
string ToValueString(string key,const T& value) {
	ostringstream os;
	os << key << ":\"" << value << "\"";
	return os.str();
}
struct ST_CONFIG_HOPPER
{
	string id;
	string in;
	string out;
	string bucket; //中储斗阀门
	string tp_id; //温度id
	void clear() {
		id.clear();
		in.clear();
		out.clear();
		bucket.clear();
		tp_id.clear();
	};
};
class CConfigGetter
{
public:
	CConfigGetter();
	virtual ~CConfigGetter();
	void Read(string path,int collect_type,string &outConfigStr);

private:
	string ReadHeader();
	string ReadContent();
	//加载Common
	string LoadCommon();
	//加载Hopper
	string LoadHopper();
	string LoadSensorBucket(int bucket_id, string path);
	string LoadMixer(string path, map<int, ST_CONFIG_HOPPER> &mapHop);
	string LoadSCTemp(map<int,ST_CONFIG_HOPPER> &mapHop);
	//加载Sensor_info
	string LoadSensorInfo();
	//加载measures
	string LoadMeasures();
	string GetMeasure(string path,string app_name,int iCount);
	int ConvertMethod(int icalc);
	//加载transmission
	string LoadTransmission();
    string GetString(const int i);

	string m_collect_type;   //0：初始值；1:界面采集(SC)；2：传感器采集(SS)	
	string m_root_path;
	string m_proj_id;   //项目编号
	string m_sect_id;   //标段号
	string m_machine_id;//拌和机id
	int m_machine_type; //拌和机类型:0，沥青拌和机；1，水泥拌和机
	int m_is_admixture_into_water; //外参剂是否入水0：否；1：是
	string m_config_id;  //备份配置文件夹名称
	map<string, int> m_map_device_type; //计量设备类型中风机系数
};

#endif  //_CONFIG_GETTER_H_