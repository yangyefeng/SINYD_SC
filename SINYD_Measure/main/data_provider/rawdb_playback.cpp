#include <stdlib.h>
#include <string.h>

#include "rawdb_playback.h"
#include "MeasureDef.h"
#include "../../Include/DebugLogger.h"

#define NODE_TYPE_ANA 1
#define NODE_TYPE_DIG 2
#define NODE_TYPE_UNDEFINED 0

Rawdb_Playback::Rawdb_Playback(const char *szRawFilename)
{
    m_strRawFilename = szRawFilename;
	m_pDBO = NULL;
	m_pQuery = NULL;
	m_pmapAnaDatas = NULL;
	m_pmapDigDatas = NULL;
	m_iPAK = 1;
	m_bRunning = true;
    m_bTapOpen = false;
}

Rawdb_Playback::~Rawdb_Playback()
{
	Release();
}
void Rawdb_Playback::Initialize(){
	Release();
	//分配数据map
	m_pmapAnaDatas = new map<int, ST_DATA_PBNODE>();
	m_pmapDigDatas = new map<int, int>();

    try {
        m_pDBO = OpenDBOperator(m_strRawFilename,false);
    }catch(CDBModuleException &ex){
        DebugErrLog("Raw Create Error[%d][%s]!\n", ex.errorCode(), ex.errorCodeAsString(ex.errorCode()));
        return;
    }

	if (m_pDBO) {
		m_pQuery = m_pDBO->Query(m_iPAK, -1);	//检索从m_iPAK开始到DB的最后一条记录
		if (!m_pQuery && m_pQuery->eof()) {
			return; //数据准备就绪
		}
	}

	//DebugLog("=====[%d]\t",m_pQuery->RowPK());
	//const list<FieldInfo>& fields = m_pDBO->GetDBInfo().fields;
	//int iPkid = FindField(fields, "packet_id");
	//int iTimestamp = FindField(fields, "timestamp");
	//string sPkgId = m_pQuery->GetStringDataField(iPkid);
	////m_iPackageId = atoi(sPkgId.c_str());
	//DebugLog("packet_id[%s][%d] \n", sPkgId.c_str(), m_iPackageId);

	//int iCol = -1;
	//const char *szValue;
	//int nodeid;
	//list<FieldInfo>::const_iterator itEnd = fields.end();
	//for (list<FieldInfo>::const_iterator it = fields.begin(); it != itEnd; ++it) {
	//	iCol++;
	//	if (iCol == iPkid || (iCol == iTimestamp)) {
	//		continue;
	//	}
	//	szValue = m_pQuery->GetStringDataField(iCol);
	//	if (szValue && *szValue) {
	//		nodeid = atoi((it->fieldName).c_str());
	//		if (it->fieldOriginalName.find("dig") == 0) { 	//数字量
	//			(*m_pmapDigDatas)[nodeid] = 0;
	//		}
	//		else {		//模拟量
	//			(*m_pmapAnaDatas)[ANA_ID_TO_NODE_ID(nodeid)] = 0.0;
	//		}
	//		//printf("[%d]value[%d] \n", iCol, nodeid);
	//	}
	//}
}

void Rawdb_Playback::Release(){

	if (m_pmapAnaDatas) {
		m_pmapAnaDatas->clear();
		delete m_pmapAnaDatas;
		m_pmapAnaDatas = NULL;
	}
	if (m_pmapDigDatas) {
		m_pmapDigDatas->clear();
		delete m_pmapDigDatas;
		m_pmapDigDatas = NULL;
	}

	if (m_pQuery) {
		m_pDBO->QueryFinalize(m_pQuery);
		m_pQuery = NULL;
	}

	if (m_pDBO) {
		ReleaseDBOperator(m_pDBO);
		m_pDBO = NULL;
	}

}

bool Rawdb_Playback::LoadData(bool bWait){
	if (m_pQuery == NULL || m_pQuery->eof()) {
		m_bRunning = false;
		return false;
	}

	const list<FieldInfo>& fields = m_pDBO->GetDBInfo().fields;
	int iPkid = FindField(fields, "packet_id");
	string sPkgId = m_pQuery->GetStringDataField(iPkid);
	if (m_iPackageId == atoi(sPkgId.c_str())) {
		return false;
	}
	int iTimestamp = FindField(fields, "timestamp");
	SydTime timestamp(m_pQuery->GetStringDataField(iTimestamp));
	m_timestamp = timestamp;
	m_iPackageId = atoi(sPkgId.c_str());
	//DebugLog("load : packet_id[%s][%d] \n", sPkgId.c_str(), m_iPackageId);

	int iCol = -1;
	const char *szValue;
	int nodeid;
	list<FieldInfo>::const_iterator itEnd = fields.end();
	for (list<FieldInfo>::const_iterator it = fields.begin(); it != itEnd; ++it) {
		iCol++;
		if (iCol == iPkid || (iCol == iTimestamp)) {
			continue;
		}
		szValue = m_pQuery->GetStringDataField(iCol);
		if (szValue && *szValue) {
			nodeid = atoi((it->fieldName).c_str());
			if (it->fieldOriginalName.find("dig") == 0) {
				(*m_pmapDigDatas)[nodeid] = atoi(szValue);
				//printf("[%d]load data:id[%d],value[%d],value char[%s] \n", iCol, nodeid, atoi(szValue), szValue);
			}
			else {
				(*m_pmapAnaDatas)[ANA_ID_TO_NODE_ID(nodeid)].raw_value = string(szValue);
				(*m_pmapAnaDatas)[ANA_ID_TO_NODE_ID(nodeid)].value = (NULL == strstr(szValue, "[") || 0 != strcmp(szValue, "")) ? atof(szValue) : 0.0;
				//printf("[%d]load data:id[%d],value[%.3lf],value char[%s] \n", iCol, ANA_ID_TO_NODE_ID(nodeid), atof(szValue), szValue);
			}
			//printf("[%d]load data:value[%d],value char[%s] \n", iCol,atoi(szValue), szValue);
		}
	}
	m_bRunning = true;
	m_pQuery->nextRow();
    
    return m_bRunning;
}

INT32 Rawdb_Playback::GetPackageId(){
    return m_iPackageId;
}

SydTime Rawdb_Playback::GetTimestamp(){
    return m_timestamp;
}

INT32 Rawdb_Playback::GetDigitalData(int iNodeId){
	map<int, int>::iterator it = m_pmapDigDatas->find(iNodeId);
	if (it != m_pmapDigDatas->end()) {
		return it->second;
	}
	else {
		return 0;
	}
}

double Rawdb_Playback::GetAnalogData(int iNodeId){
	map<int, ST_DATA_PBNODE>::iterator it = m_pmapAnaDatas->find(iNodeId);
	if (it != m_pmapAnaDatas->end()) {
		return it->second.value;
	}
	else {
		return 0.0;
	}
}

string Rawdb_Playback::GetAnalogRawData(int iNodeId){
	map<int, ST_DATA_PBNODE>::iterator it = m_pmapAnaDatas->find(iNodeId);
	if (it != m_pmapAnaDatas->end()) {
		return it->second.raw_value;
	}
    return string("0.0");
}

INT32 Rawdb_Playback::GetDigitalDataCount(){
	return m_pmapDigDatas->size();
}

INT32 Rawdb_Playback::GetDigitalDataChannelCount(){
    return 8;
}

INT32 Rawdb_Playback::GetAnalogDataCount(){
	return m_pmapDigDatas->size();
}


void Rawdb_Playback::GetDigitalNodeVector(vector<int> &vec){
    vec.clear();

	if (m_pmapDigDatas) {
		for (map<INT32, int>::iterator i = m_pmapDigDatas->begin(); i != m_pmapDigDatas->end(); i++) {
			vec.push_back(i->first);
		}
	}
}

void Rawdb_Playback::GetAnalogNodeVector(vector<int> &vec){
    vec.clear();

	if (m_pmapAnaDatas) {
		for (map<int, ST_DATA_PBNODE>::iterator i = m_pmapAnaDatas->begin(); i != m_pmapAnaDatas->end(); i++) {
			vec.push_back(i->first);
		}
	}
}

bool Rawdb_Playback::IsRunning(){
    return m_bRunning;
}

int Rawdb_Playback::GetAnaDecimal(int canid) {
    return 1;
}

bool Rawdb_Playback::IsSensorCollect()
{
    return false;
}

int Rawdb_Playback::FindField(const list<FieldInfo>& fields, const char* szFieldName) {
	int index = 0;
	list<FieldInfo>::const_iterator itEnd = fields.end();
	for (list<FieldInfo>::const_iterator it = fields.begin(); it != itEnd; ++it) {
		if (it->fieldName == szFieldName) {
			return index;
		}
		index++;
	}

	return -1;
}

bool Rawdb_Playback::IsDigitalAllOpen()
{
    bool bret = true;
    map<int,int>::iterator itDig;
    for(itDig=m_pmapDigDatas->begin(); itDig!=m_pmapDigDatas->end(); itDig++){
        if ( 0 == itDig->second ) {
            bret = false;
        }
    }
    SydTime now; //= GetTimestamp();

    if ( bret ) {
        //printf("TTTT@@@now[%s][%s][%lf]\n",now.ToString().c_str(),m_timeOn.ToString().c_str(),now - m_timeOn);
        m_bTapOpen = true;
        m_timeOn = now;
    } else if ( m_bTapOpen ) {
        if ( 0.015 > now - m_timeOn ) {
            //printf("@@@now[%s][%s]--[%s]\n",now.ToString().c_str(),m_timeOn.ToString().c_str(),GetTimestamp().ToString().c_str());
            bret = true;
        } else {
            //printf("@@@now[%s][%s][%lf]--[%s]\n",now.ToString().c_str(),m_timeOn.ToString().c_str(),now - m_timeOn,GetTimestamp().ToString().c_str());
            m_bTapOpen = false;
        }
    } else {
        m_bTapOpen = false;
    }
    return bret;
}

EN_MIX_MACHINE_TYPE Rawdb_Playback::GetMachineType()
{
    return MACHINE_INIT;
}
