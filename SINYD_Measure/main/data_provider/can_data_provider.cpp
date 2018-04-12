#include "can_data_provider.h"
#include "../../Include/DebugLogger.h"

#if _WIN32
#else
#include <sys/unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#endif
#include <errno.h>

#include <string>
#include <sstream>
#include <fstream>
using namespace std;


#include "../../Include/EmsasCommon.h"
#include "../../Include/SwDscuMmdef.h"

#include "../../Data/RWData/RWData.h"
#include "../../Data/Include/Operation_conf.h"


#define CAN_DATA_SHM_KEY 1000     //共享内存Key
#define SEM_PERM         0600 |IPC_CREAT     //共享内存的标识符

#if _WIN32
#else
static struct sembuf LockOp[2]={0,0,0,0,1,SEM_UNDO};
static struct sembuf UnlckOp[1]={0,-1,(IPC_NOWAIT|SEM_UNDO)};
#endif

CanDataProvider::CanDataProvider()
{
    m_iTranId =0;
    m_iDigitalDataChannelCount=0;

    m_pmapAnalogCalibrators=NULL;
    m_pmapAnalogData=NULL;
    m_pmapDigitalData=NULL;
    m_pmapDigADdata = NULL;
    m_pmapFlowMixCheckValue = NULL;

    m_bRunning = false;
    m_bIsExistFlowOverOffset = false;
}

CanDataProvider::~CanDataProvider(){
    Release();
}

void CanDataProvider::Release(){
    //释放数据内存
    if(m_pmapAnalogData){
        m_pmapAnalogData->clear();//清空MAP数据
        delete m_pmapAnalogData;
        m_pmapAnalogData=NULL;
    }
    if (m_pmapDigitalData){
        m_pmapDigitalData->clear();
        delete m_pmapDigitalData;
        m_pmapDigitalData=NULL;
    }

    if ( m_pmapDigADdata ) {
        m_pmapDigADdata->clear();
        delete m_pmapDigADdata;
        m_pmapDigADdata = NULL;
    }
    //删除标定信息
    map<INT32,AnalogCalibrator*>::iterator it;
    AnalogCalibrator *pCalibrator;

    if(m_pmapAnalogCalibrators){
        DebugLog("release calibrators\n");
        for(it=m_pmapAnalogCalibrators->begin();it!=m_pmapAnalogCalibrators->end(); it++){
            pCalibrator=it->second;
            delete pCalibrator;
            pCalibrator=NULL;
        }
        m_pmapAnalogCalibrators->clear();

        delete m_pmapAnalogCalibrators;
        m_pmapAnalogCalibrators=NULL;
    }

    if ( m_pmapFlowMixCheckValue ) {
        m_pmapFlowMixCheckValue->clear();
        delete m_pmapFlowMixCheckValue;
        m_pmapFlowMixCheckValue = NULL;
    }

    m_bRunning = false;
    m_strLastStatus.clear();
    m_iMachineType = MACHINE_INIT;
    m_bIsTankFdata = false;
}

void CanDataProvider::Initialize(){
    //清空以前分配的内存空间
    Release();

    m_iTranId =-1;
    m_iDigitalDataChannelCount=8; //FIXME

    //分配数据map
    m_pmapAnalogData=new map<INT32,ST_ANALOG_NODE>();
    m_pmapDigitalData=new map<INT32,ST_DIGITAL_NODE>();
    m_pmapDigADdata = new map<int,int>;
    m_pmapFlowMixCheckValue = NULL;

    //加载模拟量的标定信息
    CRWData rw;
    //string strBaseConfig=CONFIG_BASE_PATH;
    //string strAanlogConfig=strBaseConfig +"cement/analog_sensor.conf";
    string strBaseConfig=dirpath;
    string strAanlogConfig=strBaseConfig + CONFIG_ANALOG_FILE;
    char szAppName[LINESIZE];

    string strMachineConfig = strBaseConfig + CONFIG_SYS_FILE;
    m_iMachineType = (EN_MIX_MACHINE_TYPE)rw.GetProfileInt("mix_machine_1","mix_machine_type",0,strMachineConfig.c_str());
    m_dTimeSpan = rw.GetProfileInt("mix_machine_1","upload_heartbeat_time",300,strMachineConfig.c_str());
    //m_dTimeSpan = 300.0;//FIXME
    if ( MACHINE_MODIFIED_ASPHALT == m_iMachineType || MACHINE_STORAGE_TANK == m_iMachineType ){
        //SBS和储罐,不读取标定配置
        LoadFlowMinChackValue();
    }
    
    INT32 iCanId,count;
    AnalogCalibrator *pCalibrator;

    m_pmapAnalogCalibrators=new map<INT32,AnalogCalibrator*>();
    count=0;
    while(1){
        //读取CAN ID
        sprintf(szAppName,"analogsensor_%d",(count+1));
        iCanId = rw.GetProfileInt(szAppName,"can_id",-1,strAanlogConfig.c_str());
        DebugLog("can_id:%d, appname=%s, file=%s\n",iCanId,szAppName,strAanlogConfig.c_str());
        if(iCanId<=0){
            break;
        }
        //读取标定信息
        pCalibrator=new AnalogCalibrator();
        pCalibrator->LoadConfiguration(strAanlogConfig.c_str(),szAppName);

        //保存到map中
        m_pmapAnalogCalibrators->insert(pair<INT32,AnalogCalibrator*>(iCanId, pCalibrator));
        count++;
    }

    m_bRunning = true;
	m_bTapOpen = false;
    WriteRunning(m_bRunning);
}

INT32 CanDataProvider::GetAnalogDataCount(){
    return m_pmapAnalogData->size();
}

INT32 CanDataProvider::GetDigitalDataCount(){
    return m_pmapDigitalData->size();
}

INT32 CanDataProvider::GetDigitalDataChannelCount(){
    return m_iDigitalDataChannelCount;
}

void CanDataProvider::GetAnalogNodeVector(vector<INT32> &vec){
    vec.clear();

    if(m_pmapAnalogData){
        for(map<INT32,ST_ANALOG_NODE>::iterator i=m_pmapAnalogData->begin(); i!=m_pmapAnalogData->end(); i++){
            vec.push_back( i->first );
        }
    }
}


void CanDataProvider::GetDigitalNodeVector(vector<INT32> &vec){
    vec.clear();

    if(m_pmapDigitalData){
        for(map<INT32,ST_DIGITAL_NODE>::iterator i=m_pmapDigitalData->begin(); i!=m_pmapDigitalData->end(); i++){
            vec.push_back( i->first );
        }
    }
}

const AnalogCalibrator* CanDataProvider::GetCalibrator(int iCanId){
    if ( NULL == m_pmapAnalogCalibrators ) {
        return NULL;
    }
    map<INT32,AnalogCalibrator*>::iterator it=m_pmapAnalogCalibrators->find(iCanId);

    if(it!=m_pmapAnalogCalibrators->end()){
        return it->second;
    }else{
        DebugErrLog("GetCalibrator:: not calibrator found for can node %d\n",iCanId);
        return NULL;
    }
    return NULL;
}

bool CanDataProvider::LoadData(bool bWait){
#if _WIN32
	return false;
#else
    ST_RECEIVE_CAN_DATA *pData = NULL;
    UINT32 shmid;

    /* 获取共享内存shmid   */
    if((shmid=shmget(CAN_DATA_SHM_KEY,sizeof(ST_RECEIVE_CAN_DATA),SEM_PERM|IPC_CREAT))==-1)
    {
        DebugErrLog("LoadData:: Get share memory error\n");
        return false;
    }

    bool bLoaded = false;
    bool bDataArrived;

    while( true ){
        /* 连接共享内存    */
        if((pData=(ST_RECEIVE_CAN_DATA*)shmat(shmid,(const void*)0,0))==(ST_RECEIVE_CAN_DATA*)-1)
        {
            DebugErrLog("LoadData:: Attach memory error reg=%d, errno=%d\n",pData,errno);
            break;
        }

        //判断数据包是否发生变化
        bDataArrived = (m_iTranId != pData->itran_id);
        if ( bDataArrived ) {
            // 加锁（解锁）信号灯，并读取数据/
            LockSharedMemory();
            bLoaded = LoadDataFromMemory(pData);
            UnlockSharedMemory();

            bool bLastRunning = m_bRunning;
            //判断是否处于运行状态
            CalcRunningStatus();

            if ( bLastRunning != m_bRunning ) {
                WriteRunning(m_bRunning);
            }
//            map<INT32,ST_DIGITAL_NODE>::iterator itDig;
//            ostringstream osDig;
//            for(itDig=m_pmapDigitalData->begin(); itDig!=m_pmapDigitalData->end(); itDig++){
//                osDig<< itDig->second.canId <<"="<<itDig->second.value<<",";
//            }
//            string strDig=osDig.str();

//            if( strDig.compare(m_strLastStatus)==0 ){
//                if(m_bRunning && (m_time-m_timeLastStatus > 300.0)/*FIXME, 暂定300秒*/){
//                    m_bRunning = false;
//                }
//            }else{
//                m_bRunning = true;
//                m_strLastStatus = strDig;
//                m_timeLastStatus = m_time;
//            }
        }

        /* 脱离共享内存    */
        if(shmdt(pData)==-1) {
            DebugErrLog("LoadData:: detach memory error\n");
        }

        if( bDataArrived ||!bWait ){
            break;
        }
        usleep(10000); //FIXME
    }

    return ( bLoaded && m_bRunning );
	
#endif
}

bool CanDataProvider::LoadDataFromMemory(const ST_RECEIVE_CAN_DATA *pData){
    //读取一个新的数据包
    m_iTranId = pData->itran_id;
    m_time.Now();
    m_bIsExistFlowOverOffset = false;

    const AnalogCalibrator *pCalibrator;
    INT32 iDigitalChannelCount = GetDigitalDataChannelCount();
    INT32 value,id;
    INT32 i;
    ST_ANALOG_NODE stAna;
    ST_DIGITAL_NODE stDig;
    ostringstream osLog;
    string strLog;

    //计算模拟量，并输出LOG
    osLog<<"analog: pkg="<<m_iTranId;
    for( i = 0; i < ANALOG_CAN_SENSOR_SIZE; i++ ) {
        id = pData->st_can_analog_sendata[i].id;
        value= pData->st_can_analog_sendata[i].value;
        if(id<=0){
            break;
        }
        stAna.canId = id;
        stAna.analog = value;
        pCalibrator = GetCalibrator(id);

        if(pCalibrator){
            stAna.value = pCalibrator->Calibrate(value);
        }else{
            stAna.value = value;
        }
        double dLastValue = 0.0;
        if ( MACHINE_MODIFIED_ASPHALT == m_iMachineType || MACHINE_STORAGE_TANK == m_iMachineType ) {
            map<INT32, ST_ANALOG_NODE>::iterator it= m_pmapAnalogData->find(ANA_ID_TO_NODE_ID(id));
            if ( it != m_pmapAnalogData->end()) {
                dLastValue = (it->second).value;
            }
        }
        (*m_pmapAnalogData)[ANA_ID_TO_NODE_ID(id)]=stAna;
        osLog<<", ("<<id<<','<<value<<","<< stAna.value<<')';

        if ( ( MACHINE_MODIFIED_ASPHALT == m_iMachineType || MACHINE_STORAGE_TANK == m_iMachineType )
             && !m_bIsExistFlowOverOffset ) {
            m_bIsExistFlowOverOffset = IsExistFlowOffset(id, stAna.value,dLastValue);
        }
    }
    osLog<<endl<<ends;
    strLog=osLog.str();
//    DebugLogWithTm(m_time,strLog.c_str());

    //计算数字量
    osLog.seekp(0);
    osLog<<"valve:  pkg="<<m_iTranId;

    //DebugLogWithTm(tvNow,"load digital, count=%d, channels=%d\n",iDigitalCount,iDigitalChannelCount);
    for( i = 0; i < DIGITAL_CAN_SENSOR_SIZE; i++ ) {
        id = pData->st_can_digital_sendata[i].id;
        value = pData->st_can_digital_sendata[i].value ;
        if(id<=0){
            break;
        }
        (*m_pmapDigADdata)[id] = value;
        //DebugLogWithTm(tvNow,"digital:[%d,%d]=%d\n",i,id,value);
        osLog<<",("<<id;
        for(INT16 c=0; c<iDigitalChannelCount; c++){
            //DebugLogWithTm(tvNow,"digital:p=%d\n",p);
            stDig.canId = DIG_ID_TO_NODE_ID(id,c+1);
            stDig.value = (value>>c) & 0x1;
            (*m_pmapDigitalData)[DIG_ID_TO_NODE_ID(id,c+1)]=stDig;
            osLog<<","<<(INT32)(stDig.value);
        }
        osLog<<")";
    }

    osLog<<endl<<ends;
    strLog=osLog.str();
//    DebugLogWithTm(m_time,strLog.c_str());

//    osLog.seekp(0);
//    osLog<<"can25, pkg="<<m_iTranId <<", "<<GetDigitalData(3203)<<", "<<GetDigitalData(3202)<<","<<GetAnalogData(CFG_TO_ANA_ID(25));
//    osLog<<endl<<ends;
//    strLog=osLog.str();
//    DebugLogWithTm(tvNow,strLog.c_str());

//    osLog.seekp(0);
//    osLog<<"can23, pkg="<<m_iTranId <<", "<<GetDigitalData(3304)<<", "<<GetDigitalData(3305)<<", "<<GetDigitalData(3306);
//    osLog<<",  "<<GetAnalogData(CFG_TO_ANA_ID(23))<<",  "<<GetDigitalData(3307);
//    osLog<<endl<<ends;
//    strLog=osLog.str();
//    DebugLogWithTm(m_time,strLog.c_str());

    return true;
}

void CanDataProvider::CalcRunningStatus()
{
    map<INT32,ST_DIGITAL_NODE>::iterator itDig;
    ostringstream osDig;
    for(itDig=m_pmapDigitalData->begin(); itDig!=m_pmapDigitalData->end(); itDig++){
        osDig<< itDig->second.canId <<"="<<itDig->second.value<<",";
    }
    string strDig=osDig.str();


    if ( MACHINE_MODIFIED_ASPHALT == m_iMachineType ) {
        if( strDig.compare(m_strLastStatus)==0  && m_dTimeSpan > m_time-m_timeLastStatus && !m_bIsExistFlowOverOffset) {
            //FIXME,Add flowing offset condition
            m_bRunning = false;
            m_strLastStatus = strDig;
        }  else {
            m_bRunning = true;
            m_strLastStatus = strDig;
            m_timeLastStatus = m_time;
        }
    } else if ( MACHINE_STORAGE_TANK == m_iMachineType ) {
        //DebugWarningLog("last[%s],now[%s]\n",m_timeLastStatus.ToString().c_str(), m_time.ToString().c_str());
        if ( m_time-m_timeLastStatus >= m_dTimeSpan || !m_bIsTankFdata || m_bIsExistFlowOverOffset) {
            m_bRunning = true;
            m_timeLastStatus = m_time;
            m_bIsTankFdata = true;
        } else {
            m_bRunning = false;
        }
    } else {
        if( strDig.compare(m_strLastStatus)==0 ){
            if(m_bRunning && (m_time-m_timeLastStatus > m_dTimeSpan)){
				//FIXME, 暂定300秒
                m_bRunning = false;
            }
        }else{
            m_bRunning = true;
            m_strLastStatus = strDig;
            m_timeLastStatus = m_time;
        }
    }
}

INT32 CanDataProvider::GetPackageId(){
    return m_iTranId;
}

double CanDataProvider::GetAnalogData(int iNodeId){
    map<INT32,ST_ANALOG_NODE>::iterator it= m_pmapAnalogData->find(iNodeId);
    if( it!=m_pmapAnalogData->end()){
        return it->second.value;
    }else{
        return -1;
    }
}
//在MAP数据中查找指定ID的数据
string CanDataProvider::GetAnalogRawData(int iNodeId){
    map<INT32,ST_ANALOG_NODE>::iterator it= m_pmapAnalogData->find(iNodeId);
    if( it!=m_pmapAnalogData->end()){
		char value[20] = {0};
        sprintf(value, "%d", it->second.analog);
        return string(value);
    }else{
        return "";
    }
}

INT32 CanDataProvider::GetDigitalData(int iNodeId){
    map<INT32,ST_DIGITAL_NODE>::iterator it= m_pmapDigitalData->find(iNodeId);
    if( it!=m_pmapDigitalData->end()){
        return it->second.value;
    }else{
        return -1;
    }
}

SydTime CanDataProvider::GetTimestamp(){
    return m_time;
}

bool CanDataProvider::IsRunning(){
    return m_bRunning;
}

int CanDataProvider::GetAnaDecimal(int hopid) {
	CRWData rw;
	string szConfigFile = dirpath + CONFIG_HOPPER_FILE;
	char szAppName[15];
	sprintf(szAppName,"hopper_%d",hopid);
	int CanId = rw.GetProfileInt(szAppName, "weighing_cell_id", -1, szConfigFile.c_str());
	if (-1 == CanId) {
		return 1;
	}

    int decimal = GetAnaDecimalFromCanID(CanId);
	return decimal;
}

int CanDataProvider::GetAnaDecimalFromCanID(int canid) {
    const AnalogCalibrator* pAnaCalib = GetCalibrator(canid);
    int decimal = 1;
    if (pAnaCalib) {
        decimal = pAnaCalib->GetDecimal();
    }
    return decimal;
}

bool CanDataProvider::IsSensorCollect()
{
    return true;
}

int CanDataProvider::GetDigADDataCount()
{
    return m_pmapDigADdata->size();
}

map<int,int> *CanDataProvider::GetDigitalADData()
{
    return m_pmapDigADdata;
}

bool CanDataProvider::IsDigitalAllOpen()
{
    bool bret = true;
    map<INT32,ST_DIGITAL_NODE>::iterator itDig;
    for(itDig=m_pmapDigitalData->begin(); itDig!=m_pmapDigitalData->end(); itDig++){
        if ( 0 == itDig->second.value ) {
            bret = false;
        }
    }
    SydTime now; 

    if ( bret ) {
        m_bTapOpen = true;
        m_timeOn = now;
    } else if ( m_bTapOpen ) {
        if ( 1 > now - m_timeOn ) {
			//过滤拌和机开机后1秒内的数据
            bret = true;
        } else {
            m_bTapOpen = false;
        }
    } else {
        m_bTapOpen = false;
    }
    return bret;
}

EN_MIX_MACHINE_TYPE CanDataProvider::GetMachineType()
{
    return m_iMachineType;
}

void CanDataProvider::LoadFlowMinChackValue()
{
    CRWData rw;
    string szFilePath = dirpath + string(CONFIG_485_FILE);
    int iCount = 0;
    char szAppName[20] = {0};
    int iAnaId;
    m_pmapFlowMixCheckValue = new map<int,double>;

    while (1) {
        sprintf(szAppName,"flowmeter_%d",iCount+1);
        iAnaId = rw.GetProfileInt(szAppName,"can_id",-1,szFilePath.c_str());
        if ( 0 < iAnaId ) {
            (*m_pmapFlowMixCheckValue)[iAnaId]=rw.GetProfileInt(szAppName,"min_check_value",-1,szFilePath.c_str());
            iCount++;
        } else {
            break;
        }
    }

    iCount = 0;
    szFilePath = dirpath + string(CONFIG_ANALOG_FILE);
    while (1) {
        sprintf(szAppName,"analogsensor_%d",iCount+1);
        iAnaId = rw.GetProfileInt(szAppName,"can_id",-1,szFilePath.c_str());
        if ( 0 < iAnaId ) {
            (*m_pmapFlowMixCheckValue)[iAnaId]=0;
            iCount++;
        } else {
            break;
        }
    }

}

bool CanDataProvider::IsExistFlowOffset(int iAnaId, double dValue, double dLastVal)
{
    bool bRet = false;
    map<int, double>::iterator it= m_pmapFlowMixCheckValue->find(iAnaId);
    if( it!=m_pmapFlowMixCheckValue->end()){
        if ( dLastVal != dValue) {
            if ( 0 == it->second ) {
                bRet = true;
            } else if ( dValue > it->second ){
                bRet = true;
            } else if ( dLastVal > it->second && dValue < it->second ) {
                bRet = true;
            }
        }
    }
    return bRet;
}
//CAN设备开始运行
void CanDataProvider::WriteRunning(bool bRunning)
{
    CRWData rw;
    string szRunStatus = bRunning?"1":"0";
    string path = CONFIG_LOG_PATH;
    path += "Running.ini";
    printf("%s\n", path.c_str());
    fstream _file;
    _file.open(path.c_str(), ios::in);
    if (_file){
        rw.WriteProfileString("status", "running", (char *)szRunStatus.c_str(), (char *)path.c_str());
    }
    else {
        string context = "[status]\nrunning=" + szRunStatus;
        rw.CreateFile(path.c_str(), context.c_str());
    }
    _file.close();
}

bool CanDataProvider::LockSharedMemory()
{
#if _WIN32
#else
    INT32 semid;
    if((semid=semget(CAN_DATA_SHM_KEY,1,SEM_PERM))==-1)
    {
        DebugErrLog("LockSharedMemory() Get semaphore error[%d]\n",semid);
        return false;
    }

    if(semop(semid,&LockOp[0],2)==-1)
    {
        DebugErrLog("LockSharedMemory() Lock semaphore error\n");
        return false;
    }
#endif
    return true;
}

/* 解锁信号灯 */
bool CanDataProvider::UnlockSharedMemory()
{
#if _WIN32
#else
    INT32 semid;
    if((semid=semget(CAN_DATA_SHM_KEY,1,SEM_PERM))==-1)
    {
        DebugErrLog("UnlockSharedMemory() Get semaphore error[%d]\n",semid);
        return false;
    }
    if(semop(semid,&UnlckOp[0],1)==-1)
    {
        DebugErrLog("UnlockSharedMemory() Unlock semaphore error\n");
        return false;
    }
#endif
    return true;
}
