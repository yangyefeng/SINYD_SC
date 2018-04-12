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
	string bucket; //�д�������
	string tp_id; //�¶�id
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
	//����Common
	string LoadCommon();
	//����Hopper
	string LoadHopper();
	string LoadSensorBucket(int bucket_id, string path);
	string LoadMixer(string path, map<int, ST_CONFIG_HOPPER> &mapHop);
	string LoadSCTemp(map<int,ST_CONFIG_HOPPER> &mapHop);
	//����Sensor_info
	string LoadSensorInfo();
	//����measures
	string LoadMeasures();
	string GetMeasure(string path,string app_name,int iCount);
	int ConvertMethod(int icalc);
	//����transmission
	string LoadTransmission();
    string GetString(const int i);

	string m_collect_type;   //0����ʼֵ��1:����ɼ�(SC)��2���������ɼ�(SS)	
	string m_root_path;
	string m_proj_id;   //��Ŀ���
	string m_sect_id;   //��κ�
	string m_machine_id;//��ͻ�id
	int m_machine_type; //��ͻ�����:0�������ͻ���1��ˮ���ͻ�
	int m_is_admixture_into_water; //��μ��Ƿ���ˮ0����1����
	string m_config_id;  //���������ļ�������
	map<string, int> m_map_device_type; //�����豸�����з��ϵ��
};

#endif  //_CONFIG_GETTER_H_