#include "sc_data_provider.h"
#include "../../Include/DebugLogger.h"

#include <errno.h>
#include <string>
#include <sstream>

using namespace std;


#include "../../Include/EmsasCommon.h"
#include "../../Include/SwDscuMmdef.h"

#include "../../Data/RWData/RWData.h"
#include "../../Data/Include/Operation_conf.h"


ScDataProvider::ScDataProvider()
{
    m_iTranId =0;

    m_pmapAnalogData=NULL;
    m_pmapDigitalData=NULL;

    m_bRunning = false;
}

ScDataProvider::~ScDataProvider(){
    Release();
}

void ScDataProvider::Release(){
    //释放数据内存
    if(m_pmapAnalogData){
        m_pmapAnalogData->clear();
        delete m_pmapAnalogData;
        m_pmapAnalogData=NULL;
    }
    if (m_pmapDigitalData){
        m_pmapDigitalData->clear();
        delete m_pmapDigitalData;
        m_pmapDigitalData=NULL;
    }

    m_bRunning = false;
    m_strLastStatus.clear();
}

void ScDataProvider::Initialize(){
    //清空以前分配的内存空间
    Release();
    m_iTranId =-1;

    //分配数据map
    m_pmapAnalogData=new map<INT32, ST_DATA_NODE>();
    m_pmapDigitalData=new map<INT32, int>();

    m_bRunning = true;
	CRWData rw;
	string strMachineConfig = dirpath + CONFIG_SYS_FILE;
	m_iMachineType = (EN_MIX_MACHINE_TYPE)rw.GetProfileInt("mix_machine_1", "mix_machine_type", 0, strMachineConfig.c_str());
}

INT32 ScDataProvider::GetAnalogDataCount(){
    return m_pmapAnalogData->size();
}

INT32 ScDataProvider::GetDigitalDataCount(){
    return m_pmapDigitalData->size();
}

void ScDataProvider::GetAnalogNodeVector(vector<INT32> &vec){
    vec.clear();

    if(m_pmapAnalogData){
        for(map<INT32, ST_DATA_NODE>::iterator i=m_pmapAnalogData->begin(); i!=m_pmapAnalogData->end(); i++){
            vec.push_back( i->first );
        }
    }
}

void ScDataProvider::GetDigitalNodeVector(vector<INT32> &vec){
    vec.clear();

    if(m_pmapDigitalData){
        for(map<INT32, int>::iterator i=m_pmapDigitalData->begin(); i!=m_pmapDigitalData->end(); i++){
            vec.push_back( i->first );
        }
    }
}

bool ScDataProvider::LoadData(bool bWait){
	m_time.Now();
	//DebugLog("[ScDataProvider]LoadData \n");
	//判断是否处于运行状态
	map<INT32, int>::iterator itDig;
	ostringstream osDig;
	for (itDig = m_pmapDigitalData->begin(); itDig != m_pmapDigitalData->end(); itDig++) {
		osDig << itDig->first << "=" << itDig->second << ",";
	}
	string strDig = osDig.str();

	if (strDig.compare(m_strLastStatus) == 0) {
		//FIXME, 暂定300秒
		if (m_bRunning && (m_time - m_timeLastStatus > 300.0)) {
			m_bRunning = false;
		}
	}
	else {
		m_bRunning = true;
		m_strLastStatus = strDig;
		m_timeLastStatus = m_time;
	}
		

	return m_bRunning;
}


INT32 ScDataProvider::GetPackageId(){
    return m_iTranId;
}

double ScDataProvider::GetAnalogData(int iNodeId){
    map<INT32, ST_DATA_NODE>::iterator it= m_pmapAnalogData->find(iNodeId);
    if( it!=m_pmapAnalogData->end()){
        return it->second.value;
    }else{
        return -1;
    }
}

string ScDataProvider::GetAnalogRawData(int iNodeId){
    map<INT32, ST_DATA_NODE>::iterator it= m_pmapAnalogData->find(iNodeId);
    if( it!=m_pmapAnalogData->end()){
        return it->second.raw_value;
    }else{
        return "";
    }
}

INT32 ScDataProvider::GetDigitalData(int iNodeId){
    map<INT32, int>::iterator it= m_pmapDigitalData->find(iNodeId);
    if( it!=m_pmapDigitalData->end()){
        return it->second;
    }else{
        return -1;
    }
}

SydTime ScDataProvider::GetTimestamp(){
    return m_time;
}

bool ScDataProvider::IsRunning(){
    return m_bRunning;
}

int ScDataProvider::GetAnaDecimal(int canid) {
	return 1;
}

bool ScDataProvider::IsSensorCollect()
{
	return false;
}

bool ScDataProvider::IsDigitalAllOpen() {
	map<int, int>::iterator itDig;
	for (itDig = m_pmapDigitalData->begin(); itDig != m_pmapDigitalData->end(); itDig++) {
		if (0 == itDig->second) {
			return false;
		}
	}
	return true;
}

EN_MIX_MACHINE_TYPE ScDataProvider::GetMachineType()
{
	return m_iMachineType;
}

void ScDataProvider::SetRawData(const string &rawdata)
{
	//DebugLog("[Json]%s\n", rawdata.c_str());
	map<string, string> mapRew;
	ReadJson(mapRew, rawdata);
	//DebugLog("============end=======\n");
	int sensorid;
	map<string, string >::iterator l_it = mapRew.find("type");
	int type = atoi((l_it->second).c_str());
	l_it = mapRew.find("packet_id");
	m_iTranId = atoi((l_it->second).c_str());
	l_it = mapRew.find("sensor_id");
	sensorid = atoi((l_it->second).c_str());
	l_it = mapRew.find("value");
	string value = l_it->second;
	//if (( NULL != strstr(value.c_str(), "[") || value.empty())) {
	//	printf("Raw[%s]\n",value.c_str());
	//}
	//char log[50] = { 0 };
	//sprintf(log, "pid:%d, Type:%d, sensor_id:%d, value:%s[%s]", m_iTranId, type, sensorid, value.c_str(), (l_it->second).c_str());
	//DebugLog("pid:%d, Type:%d, sensor_id:%d, value:%s\n", m_iTranId, type, sensorid, value.c_str());
	//CRWData rw;
	//rw.WriteDebugForLog(log, ".\\LOG\\test.log");
	ST_DATA_NODE st_ananode;
	if (1 == type) {
		//数字信号（阀门信号）
		//st_data.value = atof(value.c_str());
		//DebugLog("dig:%d \n", atoi(value.c_str()));
		(*m_pmapDigitalData)[DIG_ID_TO_NODE_ID(sensorid,0)] = atoi(value.c_str());
	}
	else if (2 == type) {
		//模拟信号（重量、温度值）
		//st_data.value = (NULL == strstr(value.c_str(), "[")) ? atoi(value.c_str()) : 0;
		//DebugLog("ana=Raw:%d \n", sensorid);
		st_ananode.sensorId = sensorid;
		st_ananode.value = ( NULL == strstr(value.c_str(), "[") || !value.empty()) ? atof(value.c_str()) : 0.0;
		st_ananode.raw_value = value;
		(*m_pmapAnalogData)[ANA_ID_TO_NODE_ID(sensorid)] = st_ananode;
	}
	else {
		DebugErrLog("[ScDataProvider::SetRawData]Set Rew Type INVALID");
	}

}

void ScDataProvider::ReadJson(map<string, string> &outrew, const string &rawdata)
{
	char data_node[][10] = { "packet_id","sensor_id","value","time","type" };
	string data;

	Json::Value retv;
	for (int i = 0; i < 5; i++) {
		data = data_node[i];
		retv = GetJsonValue(rawdata, data);
		//GetSrcValue(pic_data, (EN_DATA_INFO)i, ret);
		//printf("%s******************* \n",retv.asString().c_str());
		outrew[data] = retv.asString();
	}
}

Json::Value ScDataProvider::GetJsonValue(const std::string &strContent, std::string& strKey)
{
	Json::Value v;
	if (!(strContent).empty())
	{
		Json::Reader reader;
		if (!reader.parse(strContent, v))
		{
			return v;
		}
	}

	Json::Value::Members mem = v.getMemberNames();
	Json::Value::Members::iterator it;
	Json::Value ret;
	//遍历所有key广度搜索  
	for (it = mem.begin(); it != mem.end(); it++)
	{
		if (*it == strKey)
		{
			ret = v[*it];
			return ret;
		}
	}

	return v;
}