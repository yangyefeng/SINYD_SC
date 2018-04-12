#include <stdlib.h>

#include "rawfile_playback.h"

#include "../../Include/DebugLogger.h"

#define NODE_TYPE_ANA 1
#define NODE_TYPE_DIG 2
#define NODE_TYPE_UNDEFINED 0

Rawfile_Playback::Rawfile_Playback(const char *szRawFilename)
{
    m_strRawFilename = szRawFilename;
}

void Rawfile_Playback::Initialize(){
    m_ifRawStream.open(m_strRawFilename.c_str(),ios::in);

    if( !m_ifRawStream.is_open() || m_ifRawStream.eof()){
        DebugErrLog("Failed to open file %s\n",m_strRawFilename.c_str());
        return;
    }

    ST_NODE_ITEM stNode;
    char szBuf[512];
    char *p ,*pPre;
    int n=0;
    bool bEnd=false;

    szBuf[sizeof(szBuf)-1]=0;
    m_ifRawStream.getline(szBuf,sizeof(szBuf)-1);
    pPre=szBuf;
    p=pPre+1;

    while(!bEnd){
        if( (*p==',')|| (*p==0) ){
            n++;
            if(*p==0){
                bEnd=true;
            }else{
                *p=0;
            }
            if(n>2){
                if(*pPre=='a'){
                    stNode.nodeType = NODE_TYPE_ANA;
                    stNode.nodeId = ANA_ID_TO_NODE_ID( atoi(pPre+1) );
                }else if(*pPre=='d'){
                    stNode.nodeType = NODE_TYPE_DIG;
                    stNode.nodeId = atoi( pPre+1 );
                }else{
                    stNode.nodeType = NODE_TYPE_UNDEFINED;
                    stNode.nodeId = atoi( pPre+1 );
                }
                m_vecNodes.push_back(stNode);
            }
            p++;
            pPre = p;
        }
        p++;
    }
}

void Rawfile_Playback::Release(){
    m_vecNodes.clear();
    m_vecValues.clear();

    if(m_ifRawStream.is_open()){
        m_ifRawStream.close();
    }
}

bool Rawfile_Playback::LoadData(bool bWait){
    if( !m_ifRawStream.is_open() || m_ifRawStream.eof() ){
        exit(1);
        return false;
    }
    m_vecValues.clear();

    char szBuf[512];
    char *p ,*pPre;
    int n=0;
    bool bEnd=false;
    INT32 iValue;

    szBuf[sizeof(szBuf)-1]=0;
    m_ifRawStream.getline(szBuf,sizeof(szBuf)-1);
    pPre=szBuf;
    p=pPre+1;

    while( !bEnd ){
        if(*p==',' || (*p==0) ){
            n++;
            if(*p==0){
                bEnd=true;
            }else{
                *p=0;
            }
            switch( n ){
            case 1:
                m_timestamp = SydTime(pPre);
                break;
            case 2:
                m_iPackageId=atoi(pPre);
                break;
            default:
                iValue = atoi(pPre);
                m_vecValues.push_back(iValue);
                break;
            }
            p++;
            pPre = p;
        }
        p++;
    }

    if(m_vecValues.size()!=m_vecNodes.size()){
        exit(2);
    }
    return true;
}

INT32 Rawfile_Playback::GetPackageId(){
    return m_iPackageId;
}

SydTime Rawfile_Playback::GetTimestamp(){
    return m_timestamp;
}

INT32 Rawfile_Playback::GetDigitalData(int iNodeId){
    ST_NODE_ITEM *pNodes=m_vecNodes.data();
    INT32 iNodeCount = m_vecNodes.size();

    for(INT32 i=0; i<iNodeCount; i++){
        if( (pNodes+i)->nodeType==NODE_TYPE_DIG &&  (pNodes+i)->nodeId ==iNodeId ){
            return m_vecValues[i];
        }
    }

    return -1;
}

double Rawfile_Playback::GetAnalogData(int iNodeId){
    ST_NODE_ITEM *pNodes=m_vecNodes.data();
    INT32 iNodeCount = m_vecNodes.size();

    for(INT32 i=0; i<iNodeCount; i++){
        if( (pNodes+i)->nodeType==NODE_TYPE_ANA &&  (pNodes+i)->nodeId ==iNodeId ){
            return m_vecValues[i];
        }
    }

    return -1;
}

string Rawfile_Playback::GetAnalogRawData(int iNodeId){
	char value[20] = { 0 };
	sprintf(value, "%.3lf", GetAnalogData(iNodeId));
    return string(value);
}

INT32 Rawfile_Playback::GetDigitalDataCount(){
    ST_NODE_ITEM *pNodes=m_vecNodes.data();
    INT32 iNodeCount = m_vecNodes.size();
    INT32 iDigCount=0;

    for(INT32 i=0; i<iNodeCount; i++){
        if( (pNodes+i)->nodeType==NODE_TYPE_DIG ){
            iDigCount++;
        }
    }

    return iDigCount;
}

INT32 Rawfile_Playback::GetDigitalDataChannelCount(){
    return 8;
}

INT32 Rawfile_Playback::GetAnalogDataCount(){
    ST_NODE_ITEM *pNodes=m_vecNodes.data();
    INT32 iNodeCount = m_vecNodes.size();
    INT32 iAnaCount=0;

    for(INT32 i=0; i<iNodeCount; i++){
        if( (pNodes+i)->nodeType==NODE_TYPE_ANA ){
            iAnaCount++;
        }
    }

    return iAnaCount;
}


void Rawfile_Playback::GetDigitalNodeVector(vector<int> &vec){
    vec.clear();

    ST_NODE_ITEM *pNodes=m_vecNodes.data();
    INT32 iNodeCount = m_vecNodes.size();

    for(INT32 i=0; i<iNodeCount; i++){
        if( (pNodes+i)->nodeType==NODE_TYPE_DIG ){
            vec.push_back((pNodes+i)->nodeId);
        }
    }
}

void Rawfile_Playback::GetAnalogNodeVector(vector<int> &vec){
    vec.clear();

    ST_NODE_ITEM *pNodes=m_vecNodes.data();
    INT32 iNodeCount = m_vecNodes.size();

    for(INT32 i=0; i<iNodeCount; i++){
        if( (pNodes+i)->nodeType==NODE_TYPE_ANA ){
            vec.push_back((pNodes+i)->nodeId);
        }
    }
}

bool Rawfile_Playback::IsRunning(){
    return true;
}
