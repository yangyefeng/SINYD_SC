#include <string>
#include <iostream>
using namespace std;

#include "data_collector.h"
#include "data_provider/can_data_provider.h"
#include "data_provider/rawfile_playback.h"
#include "data_provider/rawdb_playback.h"

#include "machine_loader.h"
#include "machine_loader/cement_machine_loader.h"
//#include "data_node/raw_recorder.h"
#include "data_node/rawdb_recorder.h"

#include "../Include/DebugLogger.h"
#include "../Include/EmsasCommon.h"
#include "../Include/SwDscuMmdef.h"
#include "../Data/RWData/RWData.h"
#include "../Data/Include/Operation_conf.h"
#include "MeasureDef.h"
#if _WIN32
#include "data_provider/sc_data_provider.h"
#endif
DataCollector::DataCollector()
{
    m_pDataProvider=NULL;
}

DataCollector::~DataCollector(){
    Release();
}

void DataCollector::Initialize(){
    Release();

    //初始化数据加载器
#ifdef _WIN32
		m_pDataProvider = new ScDataProvider();
#else 
		m_pDataProvider = new CanDataProvider();
		//m_pDataProvider=new Rawfile_Playback("raw.txt");
#endif // _WIN32
    m_pDataProvider->Initialize();

    //初始化2个数据节点
    m_vecDataNodes.push_back(LoadSysDataNode(m_pDataProvider));
    m_vecDataNodes.push_back(LoadRawRecorder(m_pDataProvider));


    DebugLog("DataCollector is initialized.\n");
}

void DataCollector::Release(){
    //删除数据节点
    DataNode *pNode;
    for(vector<DataNode*>::iterator i=m_vecDataNodes.begin(); i!=m_vecDataNodes.end(); i++){
        pNode=*i;
        delete pNode;
        while(1);
    }
    m_vecDataNodes.clear();

    //删除DataProvider
    if( m_pDataProvider){
        delete m_pDataProvider;
        m_pDataProvider = NULL;
    }
}

void DataCollector::Start(){
    vector<DataNode*>::iterator it;
    DataNode *pNode;
#if _WIN32
	if (m_pDataProvider != NULL && m_vecDataNodes.size()>0) {
#else
    while (m_pDataProvider!=NULL && m_vecDataNodes.size()>0) {
#endif
        if ( m_pDataProvider->LoadData(true) ) {
            for(it=m_vecDataNodes.begin(); it!=m_vecDataNodes.end(); it++){
                pNode = *it;
                if ( pNode ) {
                    pNode->Execute();//HopperNode::Execute and raw::Execute
                }
            }
        }else{
            DebugLog("DataCollector:: failed to load data.\n");
        }
    }
}

void DataCollector::Stop(){
    //FIXME
}

DataNode* DataCollector::LoadSysDataNode(DataProvider *pProvider){
    //初始化数据节点
    DataNode* pNode;
    MachineLoader *pml=NULL;
    CRWData rw;
    //string strBaseConfig = CONFIG_BASE_PATH;
    string strBaseConfig = dirpath;
    string strSysConfig = strBaseConfig + CONFIG_SYS_FILE;
    string strMachineConfig;
    INT32 iMachineType = rw.GetProfileInt("mix_machine_1","mix_machine_type",0,strSysConfig.c_str());

    switch(iMachineType){
    case MACHINE_CEMENT:
    case MACHINE_ASPHALT:
        //strMachineConfig=strBaseConfig + CONFIG_CEMENT_PATH;
        strMachineConfig=strBaseConfig;
        pml = new CementMachineLoader();
        break;
    //case MACHINE_ASPHALT:
    case MACHINE_MODIFIED_ASPHALT:
    case MACHINE_STORAGE_TANK:
        DebugErrLog("Unsuported  machine type: %d \n", iMachineType);
        break;
    default:
        DebugErrLog("Invalid  machine type: %d \n", iMachineType);
        break;
    }

    if( pml ){
        pNode = pml->Load(strMachineConfig.c_str());
        pNode->Initialize(pProvider);
        delete pml;
        pml=NULL;
    }else{
        pNode=NULL;
    }

    return pNode;
}

DataNode* DataCollector::LoadRawRecorder(DataProvider *pProvider){
    //RawRecorder *pNode = new RawRecorder();
	RawDBRecorder *pNode = new RawDBRecorder();
    pNode->Initialize(pProvider);

    return pNode;
}

bool DataCollector::IsRunning() {
	return m_pDataProvider->IsRunning();
}

#if _WIN32
void DataCollector::SetRawData(const string &srcdata) {
	((ScDataProvider*)m_pDataProvider)->SetRawData(srcdata);
}
#endif
