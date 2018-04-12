#include "raw_recorder.h"

#include "../../Include/DebugLogger.h"

RawRecorder::RawRecorder()
{
}

void RawRecorder::Initialize(DataProvider *pDataProvider)
{
    DataNode::Initialize(pDataProvider);

    m_rf.CreateFile("raw");
    m_bNewFile = true;

}

void RawRecorder::Execute(){
    DataProvider *pProvider= GetDataProvider();

    if( pProvider ){
        if( m_bNewFile ){
            //取得数字量节点ID和模拟量节点ID
            pProvider->GetDigitalNodeVector(m_vecDigitalNodes);

            pProvider->GetAnalogNodeVector(m_vecAnalogNodes);

            //写文件头
            RecordRawHeader();
            m_bNewFile = false;
        }

        if(pProvider->IsRunning()){
            //记录原始数据
            RecordRaw();
        }
    }
}


void RawRecorder::Release(){
    m_rf.SafeClose();
 }


INT32  RawRecorder::GetId(){
    return 99; //FIXME
}

vector<DataNode*>*  RawRecorder::GetChildren() {
    return NULL;
}


void RawRecorder::RecordRawHeader(){
    vector<INT32>::iterator it;

    m_rf<<"timestamp,package_id";

    for(it=m_vecDigitalNodes.begin(); it!=m_vecDigitalNodes.end(); it++){
        m_rf<<",d"<<*it;
    }

    for(it=m_vecAnalogNodes.begin(); it!=m_vecAnalogNodes.end(); it++){
        m_rf<<",a"<<NODE_ID_TO_ANA_ID(*it);
    }

    m_rf<<endl;
}

void RawRecorder::RecordRaw(){
    vector<INT32>::iterator it;
    DataProvider *pProvider = GetDataProvider();

    m_rf<<pProvider->GetTimestamp().ToString()<<","<<pProvider->GetPackageId();

    for(it=m_vecDigitalNodes.begin(); it!=m_vecDigitalNodes.end(); it++){
        m_rf<<","<< pProvider->GetDigitalData(*it);
    }

    for(it=m_vecAnalogNodes.begin(); it!=m_vecAnalogNodes.end(); it++){
        m_rf<<","<<  pProvider->GetAnalogData(*it);
    }

    m_rf<<endl;
}
