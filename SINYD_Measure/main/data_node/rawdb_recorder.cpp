#include "rawdb_recorder.h"

#include "../../Include/DebugLogger.h"
#include "MeasureDef.h"
#include <sstream>
#include "SYDDataTransfer.h"
#include "../../Data/RWData/RWData.h"
#include "../data_provider/can_data_provider.h"
#define DIGITAL_TYPE 1
#define ANALOG_TYPE 2

RawDBRecorder::RawDBRecorder()
	: m_pdTrans(NULL)
{
}

RawDBRecorder::~RawDBRecorder()
{
	Release();
}

void RawDBRecorder::Initialize(DataProvider *pDataProvider)
{
    DataNode::Initialize(pDataProvider);

    if ( MACHINE_MODIFIED_ASPHALT == pDataProvider->GetMachineType() ||
         MACHINE_STORAGE_TANK == pDataProvider->GetMachineType() ) {
        m_rf.CreateFile("product");
    }
	m_pdbo = NULL;
    m_pdbo_ad = NULL;
    m_bNewFile = true;
	m_bNewDB = true;
    RecordRawHeader();
    StartTransfer();
}
//数据库入口
void RawDBRecorder::Execute(){
    DataProvider *pProvider= GetDataProvider();
    if( pProvider ){
        if( m_bNewFile ){
            //写文件头
            //RecordRawHeader();
            //StartTransfer();
            m_bNewFile = false;
        }

		if (m_bNewDB) {
			//取得数字量节点ID和模拟量节点ID
			//pProvider->GetDigitalNodeVector(m_vecDigitalNodes);
			//pProvider->GetAnalogNodeVector(m_vecAnalogNodes);

            RecordRawHeader();
		}

        if(pProvider->IsRunning()){
            //记录原始数据
            RecordRaw();
        }
    }
}


void RawDBRecorder::Release(){
    m_rf.SafeClose();
	ReleaseDBOperator(m_pdbo);
	m_pdbo = NULL;

    ReleaseDBOperator(m_pdbo_ad);
    m_pdbo_ad = NULL;

    if (m_pdTrans){
        m_pdTrans->Stop();
        delete m_pdTrans;
        m_pdTrans = NULL;
    }
 }


INT32  RawDBRecorder::GetId(){
    return 99; //FIXME
}

vector<DataNode*>*  RawDBRecorder::GetChildren() {
    return NULL;
}

//记录数据库
void RawDBRecorder::RecordRawHeader(){
	DBInfo tinfo;
	tinfo.DBNamePrefix = STR_RAW_BD_NAME;
	
	FieldInfo cinfo;
	cinfo.fieldName = STR_PACKET_ID;
	cinfo.fieldOriginalName = "包号";
	cinfo.dataType = DATATYPE_INTEGER;
	tinfo.fields.push_back(cinfo);

	cinfo.fieldName = STR_RECORD_TIME;
	cinfo.fieldOriginalName = "记录时间";
	cinfo.dataType = DATATYPE_REAL;
	tinfo.fields.push_back(cinfo);

    map<int, int> mapSensor;
    if ( MACHINE_STORAGE_TANK == GetDataProvider()->GetMachineType() ) {
        LoadTankSensor(mapSensor);
    }
    else if (MACHINE_MODIFIED_ASPHALT == GetDataProvider()->GetMachineType() ) {
        LoadSBSSensor(mapSensor);
    }
    else {
        cinfo.fieldName = STR_RECORD_CURRENT;//yyf add
        cinfo.fieldOriginalName = "拌和机电流";
        cinfo.dataType = DATATYPE_REAL;
        tinfo.fields.push_back(cinfo);
        LoadSensor(mapSensor);
    }
    //printf("@@@@@@@@@@@@@2size[%d] \n",mapSensor.size());
    map<int, int>::iterator its;
    string cname;
    for (its = mapSensor.begin();its != mapSensor.end();++its) {
        if (0 >= its->first) {
            continue;
        }
        cname = GetString(its->first);
        cinfo.fieldName = cname;
        if (DIGITAL_TYPE == its->second) {
            cinfo.fieldOriginalName = "dig" + cname;
            cinfo.dataType = DATATYPE_INTEGER;
            m_vecDigitalNodes.push_back(its->first);
        }
        else {
            cinfo.fieldOriginalName = "ana" + cname;
            cinfo.dataType = DATATYPE_REAL;
            m_vecAnalogNodes.push_back(ANA_ID_TO_NODE_ID(its->first));
        }
        tinfo.fields.push_back(cinfo);
    }

    //vector<int>::iterator it;
    //string cname;
    //for (it = m_vecDigitalNodes.begin(); it != m_vecDigitalNodes.end(); it++) {
    //	cname = GetString(*it);
    //	cinfo.fieldName = cname;
    //	cinfo.fieldOriginalName = "dig" + cname;
    //	cinfo.dataType = DATATYPE_INTEGER;
    //	tinfo.fields.push_back(cinfo);
    //}

    //for (it = m_vecAnalogNodes.begin(); it != m_vecAnalogNodes.end(); it++) {
    //	cname = GetString(NODE_ID_TO_ANA_ID(*it));
    //	cinfo.fieldName = cname;
    //	cinfo.fieldOriginalName = "ana" + cname;
    //	cinfo.dataType = DATATYPE_REAL;
    //	tinfo.fields.push_back(cinfo);
    //}

    DebugWarningLog("time:%s,Path:%s\n",createtime.c_str(),dirpath.c_str());
    tinfo.configInfo.ConfigTimeStamp = createtime;
    tinfo.configInfo.DBConfigPath = dirpath;
    try {
        m_pdbo = CreateDBOperator(tinfo);
    }catch(CDBModuleException &ex){
        CRWData rw;
        char ch[1024] = {0};
        string pah = CONFIG_LOG_PATH;
        pah += ERROR_LOG_FILE;
        sprintf(ch,"Raw Create Error[%d][%s]!\n", ex.errorCode(), ex.errorCodeAsString(ex.errorCode()));
        rw.RepeatLog(ch, (char*)pah.c_str());
        DebugErrLog("Raw Create Error[%d][%s]!\n", ex.errorCode(), ex.errorCodeAsString(ex.errorCode()));
    }

    if (GetDataProvider()->IsSensorCollect()){
        try {
            tinfo.DBNamePrefix = "raw_ad";
            m_pdbo_ad = CreateDBOperator(tinfo);
        }
        catch (CDBModuleException &ex) {
            CRWData rw;
            char ch[1024] = { 0 };
            string pah = CONFIG_LOG_PATH;
            pah += ERROR_LOG_FILE;
            sprintf(ch, "Raw_ad Create Error[%d][%s]!\n", ex.errorCode(), ex.errorCodeAsString(ex.errorCode()));
            rw.RepeatLog(ch, (char*)pah.c_str());
            DebugErrLog("Raw_ad Create Error[%d][%s]!\n", ex.errorCode(), ex.errorCodeAsString(ex.errorCode()));
        }
    }

    m_bNewDB = false;
    vector<int>::iterator it;
    if ( MACHINE_MODIFIED_ASPHALT == GetDataProvider()->GetMachineType() ||
         MACHINE_STORAGE_TANK == GetDataProvider()->GetMachineType() ) {
        m_rf<<"timestamp,package_id";

        for(it=m_vecDigitalNodes.begin(); it!=m_vecDigitalNodes.end(); it++){
            m_rf<<",d"<<*it;
        }

        for(it=m_vecAnalogNodes.begin(); it!=m_vecAnalogNodes.end(); it++){
            m_rf<<",a"<<NODE_ID_TO_ANA_ID(*it);
        }

        m_rf<<endl;
        //m_rf.flush();
    }
}

void RawDBRecorder::RecordRaw(){
    vector<int>::iterator it;
    DataProvider *pProvider = GetDataProvider();
    double a=10.123;                       //YYF  add
	map<string, string> raw_data;
	raw_data[STR_PACKET_ID] = GetString(pProvider->GetPackageId());
	raw_data[STR_RECORD_TIME] = pProvider->GetTimestamp().ToString();
    raw_data[STR_RECORD_CURRENT] =GetString(a);//YYF add
	for (it = m_vecDigitalNodes.begin(); it != m_vecDigitalNodes.end(); it++) {
		raw_data[GetString(*it)] = GetString(pProvider->GetDigitalData(*it));
	}

    int decimal = 1;
	for (it = m_vecAnalogNodes.begin(); it != m_vecAnalogNodes.end(); it++) {
#if _WIN32
		if (pProvider->GetAnalogRawData(*it).empty()){
			//printf("[%s][%d] \n", pProvider->GetAnalogRawData(*it).c_str(),(*it));
			continue;
		}
		raw_data[GetString(NODE_ID_TO_ANA_ID(*it))] = pProvider->GetAnalogRawData(*it);
#else
        decimal = ((CanDataProvider*)pProvider)->GetAnaDecimalFromCanID(NODE_ID_TO_ANA_ID(*it));
        raw_data[GetString(NODE_ID_TO_ANA_ID(*it))] = GetString(pProvider->GetAnalogData(*it)/decimal);
#endif
	}
	try {
        m_pdbo->Insert(raw_data);//插入数据库
	}
	catch (CDBModuleException &ex)
	{
        CRWData rw;
        char ch[1024] = {0};
        string pah = CONFIG_LOG_PATH;
        pah += ERROR_LOG_FILE;
        sprintf(ch,"Raw Insert Error[%d][%s]!\n", ex.errorCode(), ex.errorCodeAsString(ex.errorCode()));
        rw.RepeatLog(ch, (char*)pah.c_str());
        DebugErrLog("Raw Insert Error[%d][%s]!\n", ex.errorCode(), ex.errorCodeAsString(ex.errorCode()));
	}
	
    if ( pProvider->IsSensorCollect() ) {
        RecordADRaw();
    }

    if ( MACHINE_MODIFIED_ASPHALT == pProvider->GetMachineType() ||
         MACHINE_STORAGE_TANK == pProvider->GetMachineType() ) {
        m_rf<<pProvider->GetTimestamp().ToString()<<","<<pProvider->GetPackageId();

        for(it=m_vecDigitalNodes.begin(); it!=m_vecDigitalNodes.end(); it++){
            m_rf<<","<< pProvider->GetDigitalData(*it);
        }

        for(it=m_vecAnalogNodes.begin(); it!=m_vecAnalogNodes.end(); it++){
		    decimal = ((CanDataProvider*)pProvider)->GetAnaDecimalFromCanID(NODE_ID_TO_ANA_ID(*it));
            m_rf<<","<<  pProvider->GetAnalogData(*it)/decimal;
        }

        m_rf<<endl;
    }
}
//储存AD到数据库
void RawDBRecorder::RecordADRaw()
{
    vector<int>::iterator it;
    DataProvider *pProvider = GetDataProvider();

    map<string, string> ad_data;
    ad_data[STR_PACKET_ID] = GetString(pProvider->GetPackageId());
    ad_data[STR_RECORD_TIME] = pProvider->GetTimestamp().ToString();
    for (it = m_vecAnalogNodes.begin(); it != m_vecAnalogNodes.end(); it++) {
        ad_data[GetString(NODE_ID_TO_ANA_ID(*it))] = pProvider->GetAnalogRawData(*it);
   }

	for (it = m_vecDigitalNodes.begin(); it != m_vecDigitalNodes.end(); it++) {
		ad_data[GetString(*it)] = GetString(pProvider->GetDigitalData(*it));
	}
    //map<int,int> *pmapAdValue = ((CanDataProvider*)GetDataProvider())->GetDigitalADData();
    //map<int,int>::iterator dit;
    //for ( dit = pmapAdValue->begin(); dit != pmapAdValue->end(); dit++) {
    //    ad_data[GetString(dit->first)] = GetString(dit->second);
    //}

    try {
        m_pdbo_ad->Insert(ad_data);
    }
    catch (CDBModuleException &ex)
    {
        CRWData rw;
        char ch[1024] = {0};
        string pah = CONFIG_LOG_PATH;
        pah += ERROR_LOG_FILE;
        sprintf(ch,"Raw_ad Insert Error[%d][%s]!\n", ex.errorCode(), ex.errorCodeAsString(ex.errorCode()));
        rw.RepeatLog(ch, (char*)pah.c_str());
        DebugErrLog("Raw_ad Insert Error[%d][%s]!\n", ex.errorCode(), ex.errorCodeAsString(ex.errorCode()));
    }
}

string RawDBRecorder::GetString(const int n)
{
	stringstream newstr;
	newstr << n;
	return newstr.str();
}

string RawDBRecorder::GetString(const double n)
{
    char newstr[100] = {0};
    sprintf(newstr,"%lf",n);
    return string(newstr);
//	stringstream newstr;
//	newstr << n;
//	return newstr.str();
}

void RawDBRecorder::StartTransfer()
{
    m_pdTrans = new CSYDDataTransfer();
    CRWData rw;

    char ip[20] = {0};
    string mixpath = dirpath+CONFIG_SYS_FILE;
    rw.GetProfileString("modbus_server","ip","",ip,mixpath.c_str());
    int port = rw.GetProfileInt("modbus_server","port",0,mixpath.c_str());

	SourceInfo2 src2;
	rw.GetProfileString("mix_station_conf", "mix_project_name", "", src2.mix_project_id, mixpath.c_str());
	rw.GetProfileString("mix_station_conf", "mix_stationname", "", src2.mix_section_id, mixpath.c_str());
	rw.GetProfileString("mix_machine_1", "mix_machine_device_id", "", src2.mix_machine_id,mixpath.c_str());
	src2.mix_type = rw.GetProfileInt("mix_machine_1", "mix_machine_type", 0, mixpath.c_str());
#if _WIN32
	src2.source_type = 1;
#else
	src2.source_type = 2;
#endif
	m_pdTrans->Init2(src2);
	m_pdTrans->Start(ip, port);

//    SourceInfo src;
//    src.mix_id =rw.GetProfileInt("mix_machine_1","mix_machine_device_id",0,mixpath.c_str());
//    src.mix_type =rw.GetProfileInt("mix_machine_1","mix_machine_type",0,mixpath.c_str());
//#if _WIN32
//    src.source_type=1;
//#else
//    src.source_type=2;
//#endif
//    DebugLog("ip[%s],port[%d],mix_id[%d]mix_type[%d]\n",ip,port,src.mix_id,src.mix_type);
//
//    m_pdTrans->Init(src);
//    m_pdTrans->Start(ip, port);
}

void RawDBRecorder::LoadSensor(map<int, int> &mapSensor)
{
	CRWData rw;
	//char szBuf[LINESIZE] = {0};
	char szAppName[LINESIZE] = { 0 };
	int iSensorId, iSensorCh,iHopid;
	int iCount = 0;
	string szConfigFile = dirpath + string(CONFIG_HOPPER_FILE);

//获取搅拌机
	sprintf(szAppName,"mixer");
	iHopid = rw.GetProfileInt(szAppName, "mix_machine_id", 0, szConfigFile.c_str());
	if (0 <= iHopid) {
#if _WIN32
		iSensorId = rw.GetProfileInt(szAppName, "blowing_tap_id", 0, szConfigFile.c_str());
		iSensorCh = 0;
#else
		iSensorId = rw.GetProfileInt(szAppName, "blowing_tap_can_id", 0, szConfigFile.c_str());
		iSensorCh = rw.GetProfileInt(szAppName, "blowing_tap_id", 0, szConfigFile.c_str());
#endif
		mapSensor[DIG_ID_TO_NODE_ID(iSensorId, iSensorCh)] = DIGITAL_TYPE;
#if _WIN32
		iSensorId = rw.GetProfileInt(szAppName, "temperature_id", 0, szConfigFile.c_str());
		mapSensor[iSensorId] = ANALOG_TYPE;
#endif
	}
//获取Hop
	iSensorCh = 0;
#if _WIN32
	while (1) {
		sprintf(szAppName, "hopper_%d", iCount + 1);
		iHopid = rw.GetProfileInt(szAppName, "hopper_id", -1, szConfigFile.c_str());
		if ( 0 >=iHopid ) {
			break;
		}
		iSensorId = rw.GetProfileInt(szAppName, "weighing_cell_id", 0, szConfigFile.c_str());
		mapSensor[iSensorId] = ANALOG_TYPE;
		iSensorId = rw.GetProfileInt(szAppName, "set_id", 0, szConfigFile.c_str());
		mapSensor[iSensorId] = ANALOG_TYPE;
		iSensorId = rw.GetProfileInt(szAppName, "calc_id", 0, szConfigFile.c_str());
		mapSensor[iSensorId] = ANALOG_TYPE;
		iSensorId = rw.GetProfileInt(szAppName, "temperature_id", 0, szConfigFile.c_str());
		mapSensor[iSensorId] = ANALOG_TYPE;

		if (221 != iHopid && 222 != iHopid) {
			iSensorId = rw.GetProfileInt(szAppName, "bucket_id", 0, szConfigFile.c_str());
			mapSensor[DIG_ID_TO_NODE_ID(iSensorId, iSensorCh)] = DIGITAL_TYPE;
		}
		iSensorId = rw.GetProfileInt(szAppName, "loading_tap1_id", 0, szConfigFile.c_str());
		mapSensor[DIG_ID_TO_NODE_ID(iSensorId, iSensorCh)] = DIGITAL_TYPE;
		iSensorId = rw.GetProfileInt(szAppName, "loading_tap2_id", 0, szConfigFile.c_str());
		mapSensor[DIG_ID_TO_NODE_ID(iSensorId, iSensorCh)] = DIGITAL_TYPE;
		iSensorId = rw.GetProfileInt(szAppName, "blowing_tap1_id", 0, szConfigFile.c_str());
		mapSensor[DIG_ID_TO_NODE_ID(iSensorId, iSensorCh)] = DIGITAL_TYPE;
		iSensorId = rw.GetProfileInt(szAppName, "blowing_tap2_id", 0, szConfigFile.c_str());
		mapSensor[DIG_ID_TO_NODE_ID(iSensorId, iSensorCh)] = DIGITAL_TYPE;

		iCount++;
	}
#else
	int hop_id[] = { 201,202,203,204,205,206,211,212,213,214,215,221,222,231,232,233,401,402,403 };
    iCount = 0;
	int iSize = sizeof(hop_id) / sizeof(int);
    while (iCount < iSize) {
		sprintf(szAppName, "hopper_%d", hop_id[iCount]);
		iCount++;

		iHopid = rw.GetProfileInt(szAppName, "hopper_id", -1, szConfigFile.c_str());
		if (0 >= iHopid) {
			continue;
		}
		iSensorId = rw.GetProfileInt(szAppName, "weighing_cell_id", 0, szConfigFile.c_str());
		mapSensor[iSensorId] = ANALOG_TYPE;

		iSensorId = rw.GetProfileInt(szAppName, "loading_tap1_can_id", 0, szConfigFile.c_str());
		iSensorCh = rw.GetProfileInt(szAppName, "loading_tap1_channel_id", 0, szConfigFile.c_str());
		mapSensor[DIG_ID_TO_NODE_ID(iSensorId, iSensorCh)] = DIGITAL_TYPE;
		iSensorId = rw.GetProfileInt(szAppName, "loading_tap2_can_id", 0, szConfigFile.c_str());
		iSensorCh = rw.GetProfileInt(szAppName, "loading_tap2_channel_id", 0, szConfigFile.c_str());
		mapSensor[DIG_ID_TO_NODE_ID(iSensorId, iSensorCh)] = DIGITAL_TYPE;
		iSensorId = rw.GetProfileInt(szAppName, "blowing_tap1_can_id", 0, szConfigFile.c_str());
		iSensorCh = rw.GetProfileInt(szAppName, "blowing_tap1_channel_id", 0, szConfigFile.c_str());
		mapSensor[DIG_ID_TO_NODE_ID(iSensorId, iSensorCh)] = DIGITAL_TYPE;
		iSensorId = rw.GetProfileInt(szAppName, "blowing_tap2_can_id", 0, szConfigFile.c_str());
		iSensorCh = rw.GetProfileInt(szAppName, "blowing_tap2_channel_id", 0, szConfigFile.c_str());
		mapSensor[DIG_ID_TO_NODE_ID(iSensorId, iSensorCh)] = DIGITAL_TYPE;
	}

    iCount = 500;
	iSensorCh = 0;
	while (iCount < 504) {
		sprintf(szAppName, "bucket_%d", ++iCount);
		iHopid = rw.GetProfileInt(szAppName, "bucket_id", -1, szConfigFile.c_str());
		if (iHopid <= 0) {
			continue;
		}
		iSensorId = rw.GetProfileInt(szAppName, "weighing_cell_id", 0, szConfigFile.c_str());
		mapSensor[iSensorId] = ANALOG_TYPE;

		iSensorId = rw.GetProfileInt(szAppName, "loading_tap1_can_id", 0, szConfigFile.c_str());
		iSensorCh = rw.GetProfileInt(szAppName, "loading_tap1_channel_id", 0, szConfigFile.c_str());
		mapSensor[DIG_ID_TO_NODE_ID(iSensorId, iSensorCh)] = DIGITAL_TYPE;
		iSensorId = rw.GetProfileInt(szAppName, "loading_tap2_can_id", 0, szConfigFile.c_str());
		iSensorCh = rw.GetProfileInt(szAppName, "loading_tap2_channel_id", 0, szConfigFile.c_str());
		mapSensor[DIG_ID_TO_NODE_ID(iSensorId, iSensorCh)] = DIGITAL_TYPE;
		iSensorId = rw.GetProfileInt(szAppName, "blowing_tap1_can_id", 0, szConfigFile.c_str());
		iSensorCh = rw.GetProfileInt(szAppName, "blowing_tap1_channel_id", 0, szConfigFile.c_str());
		mapSensor[DIG_ID_TO_NODE_ID(iSensorId, iSensorCh)] = DIGITAL_TYPE;
		iSensorId = rw.GetProfileInt(szAppName, "blowing_tap2_can_id", 0, szConfigFile.c_str());
		iSensorCh = rw.GetProfileInt(szAppName, "blowing_tap2_channel_id", 0, szConfigFile.c_str());
		mapSensor[DIG_ID_TO_NODE_ID(iSensorId, iSensorCh)] = DIGITAL_TYPE;
	}
#endif
}

void RawDBRecorder::LoadSBSSensor(map<int, int> &mapSensor)
{
	CRWData rw;
	char szAppName[LINESIZE] = { 0 };
	int iSensorId, iSensorCh, iHopid;
	int iCount = 0;
	string szConfigFile = dirpath + string(CONFIG_HOPPER_FILE);

//反应罐
	while (1)
	{
        sprintf(szAppName, "retorttank_%d", ++iCount);
		iHopid = rw.GetProfileInt(szAppName, "tank_id", -1, szConfigFile.c_str());

		if (0 >= iHopid) {
			break;
		}
		iSensorId = rw.GetProfileInt(szAppName, "flowing_tap_id", 0, szConfigFile.c_str());
		mapSensor[iSensorId] = ANALOG_TYPE;
		iSensorId = rw.GetProfileInt(szAppName, "temperature_id", 0, szConfigFile.c_str());
		mapSensor[iSensorId] = ANALOG_TYPE;

		iSensorId = rw.GetProfileInt(szAppName, "loading_tap1_can_id", 0, szConfigFile.c_str());
		iSensorCh = rw.GetProfileInt(szAppName, "loading_tap1_channel_id", 0, szConfigFile.c_str());
		mapSensor[DIG_ID_TO_NODE_ID(iSensorId, iSensorCh)] = DIGITAL_TYPE;
		iSensorId = rw.GetProfileInt(szAppName, "loading_tap2_can_id", 0, szConfigFile.c_str());
		iSensorCh = rw.GetProfileInt(szAppName, "loading_tap2_channel_id", 0, szConfigFile.c_str());
		mapSensor[DIG_ID_TO_NODE_ID(iSensorId, iSensorCh)] = DIGITAL_TYPE;
		iSensorId = rw.GetProfileInt(szAppName, "blowing_tap_can_id", 0, szConfigFile.c_str());
		iSensorCh = rw.GetProfileInt(szAppName, "blowing_tap_channel_id", 0, szConfigFile.c_str());
		mapSensor[DIG_ID_TO_NODE_ID(iSensorId, iSensorCh)] = DIGITAL_TYPE;
    }
//研磨罐
	iCount = 0;
	iSensorCh = 0;
	while (1)
	{
        sprintf(szAppName, "grindtank_%d", ++iCount);
		iHopid = rw.GetProfileInt(szAppName, "tank_id", -1, szConfigFile.c_str());
		if (0 >= iHopid) {
			break;
		}
		iSensorId = rw.GetProfileInt(szAppName, "loading_tap1_can_id", 0, szConfigFile.c_str());
		iSensorCh = rw.GetProfileInt(szAppName, "loading_tap1_channel_id", 0, szConfigFile.c_str());
		mapSensor[DIG_ID_TO_NODE_ID(iSensorId, iSensorCh)] = DIGITAL_TYPE;
		iSensorId = rw.GetProfileInt(szAppName, "blowing_tap_can_id", 0, szConfigFile.c_str());
		iSensorCh = rw.GetProfileInt(szAppName, "blowing_tap_channel_id", 0, szConfigFile.c_str());
		mapSensor[DIG_ID_TO_NODE_ID(iSensorId, iSensorCh)] = DIGITAL_TYPE;
    }
//发育罐
	iCount = 0;
	iSensorCh = 0;
	while (1)
	{
        sprintf(szAppName, "developtank_%d", ++iCount);
		iHopid = rw.GetProfileInt(szAppName, "tank_id", -1, szConfigFile.c_str());
		if (0 >= iHopid) {
			break;
		}
		iSensorId = rw.GetProfileInt(szAppName, "temperature_id", 0, szConfigFile.c_str());
		mapSensor[iSensorId] = ANALOG_TYPE;

		iSensorId = rw.GetProfileInt(szAppName, "loading_tap1_can_id", 0, szConfigFile.c_str());
		iSensorCh = rw.GetProfileInt(szAppName, "loading_tap1_channel_id", 0, szConfigFile.c_str());
		mapSensor[DIG_ID_TO_NODE_ID(iSensorId, iSensorCh)] = DIGITAL_TYPE;
		iSensorId = rw.GetProfileInt(szAppName, "loading_tap2_can_id", 0, szConfigFile.c_str());
		iSensorCh = rw.GetProfileInt(szAppName, "loading_tap2_channel_id", 0, szConfigFile.c_str());
		mapSensor[DIG_ID_TO_NODE_ID(iSensorId, iSensorCh)] = DIGITAL_TYPE;
		iSensorId = rw.GetProfileInt(szAppName, "blowing_tap_can_id", 0, szConfigFile.c_str());
		iSensorCh = rw.GetProfileInt(szAppName, "blowing_tap_channel_id", 0, szConfigFile.c_str());
		mapSensor[DIG_ID_TO_NODE_ID(iSensorId, iSensorCh)] = DIGITAL_TYPE;
    }
//Hopper
	iCount = 0;
	iSensorCh = 0;
	while (1) {
		sprintf(szAppName, "hopper_%d", ++iCount);
		iHopid = rw.GetProfileInt(szAppName, "hopper_id", -1, szConfigFile.c_str());
		if (iHopid <= 0) {
            break;
		}
		iSensorId = rw.GetProfileInt(szAppName, "weighing_cell_id", 0, szConfigFile.c_str());
		mapSensor[iSensorId] = ANALOG_TYPE;

		iSensorId = rw.GetProfileInt(szAppName, "loading_tap1_can_id", 0, szConfigFile.c_str());
		iSensorCh = rw.GetProfileInt(szAppName, "loading_tap1_channel_id", 0, szConfigFile.c_str());
		mapSensor[DIG_ID_TO_NODE_ID(iSensorId, iSensorCh)] = DIGITAL_TYPE;
		iSensorId = rw.GetProfileInt(szAppName, "loading_tap2_can_id", 0, szConfigFile.c_str());
		iSensorCh = rw.GetProfileInt(szAppName, "loading_tap2_channel_id", 0, szConfigFile.c_str());
		mapSensor[DIG_ID_TO_NODE_ID(iSensorId, iSensorCh)] = DIGITAL_TYPE;
		iSensorId = rw.GetProfileInt(szAppName, "blowing_tap_can_id", 0, szConfigFile.c_str());
		iSensorCh = rw.GetProfileInt(szAppName, "blowing_tap_channel_id", 0, szConfigFile.c_str());
		mapSensor[DIG_ID_TO_NODE_ID(iSensorId, iSensorCh)] = DIGITAL_TYPE;
    }
}

void RawDBRecorder::LoadTankSensor(map<int, int> &mapSensor)
{
	CRWData rw;
	char szAppName[LINESIZE] = { 0 };
	int iSensorId, iHopid;
	int iCount = 0;
	string szConfigFile = dirpath + string(CONFIG_HOPPER_FILE);

	while (1) {
		sprintf(szAppName, "hopper_%d", ++iCount);
		iHopid = rw.GetProfileInt(szAppName, "hopper_id", -1, szConfigFile.c_str());
		if (iHopid <= 0) {
            break;
		}
		iSensorId = rw.GetProfileInt(szAppName, "in_flow", 0, szConfigFile.c_str());
		mapSensor[iSensorId] = ANALOG_TYPE;
		iSensorId = rw.GetProfileInt(szAppName, "out_flow", 0, szConfigFile.c_str());
		mapSensor[iSensorId] = ANALOG_TYPE;
		iSensorId = rw.GetProfileInt(szAppName, "in_cumuflow", 0, szConfigFile.c_str());
		mapSensor[iSensorId] = ANALOG_TYPE;
		iSensorId = rw.GetProfileInt(szAppName, "out_cumuflow", 0, szConfigFile.c_str());
		mapSensor[iSensorId] = ANALOG_TYPE;
		iSensorId = rw.GetProfileInt(szAppName, "liquid_level", 0, szConfigFile.c_str());
		mapSensor[iSensorId] = ANALOG_TYPE;
		iSensorId = rw.GetProfileInt(szAppName, "temperature", 0, szConfigFile.c_str());
		mapSensor[iSensorId] = ANALOG_TYPE;
	}
}
