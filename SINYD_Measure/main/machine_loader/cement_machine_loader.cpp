#include <string>
#include <sstream>
#include <stdlib.h>

#include "../../Include/DebugLogger.h"
#include "../../Include/EmsasCommon.h"
#include "../../Include/SwDscuMmdef.h"
#include "../../Data/RWData/RWData.h"
#include "../../Data/Include/Operation_conf.h"

#include "../data_node/mixer_node.h"
#include "../data_node/hopper_node.h"
#include "../data_node/multivalued_hopper_node.h"
#include "../data_node/valuedbytap_hopper_node.h"
#include "../abstract_algorithm.h"
#include "../data_algorithm/data_algorithm.h"

#include "cement_machine_loader.h"

#include "log.h"
//extern Logger logger;
int hop_id[] = {201,202,203,204,205,206,211,212,213,214,215,221,222,231,232,233,401,402,403};

CementMachineLoader::CementMachineLoader()
{
	m_vHopidList.clear();
}

DataNode* CementMachineLoader::Load(const char *szConfigPath) {
	string strConfigPath = szConfigPath;
	string strHopperConfig = strConfigPath + CONFIG_HOPPER_FILE;
	const char *szHopperConfig = strHopperConfig.c_str();
	char szBuf[LINESIZE];

	//创建拌和机对象并从配置文件中读取拌和机信息
	MixerNode* pMixer = new MixerNode();
	LoadMixer(szHopperConfig, "mixer", pMixer);

	//计算料仓和中途斗数量并分配内存
	INT32 nHopperCount = GetHopperCount(szHopperConfig);
	INT32 nBucketCount = GetBucketCount(szHopperConfig);
	ST_HOPPER *stHopper = new ST_HOPPER[nHopperCount];
	ST_HOPPER *stBucket = new ST_HOPPER[nBucketCount];

	//加载料仓信息，并通过排序将模拟量相同的料仓拍在一起
#if _WIN32
	for (INT32 i = 0; i < nHopperCount; i++) {
		sprintf(szBuf, "hopper_%d", i + 1);
		LoadHopAttrStruct(szHopperConfig, szBuf, stHopper + i, HOPPER_ATTR_TYPE_REAL);
		if ( ( 221 == stHopper[i].id && 0 < stHopper[i].iBucketId) || (222 == stHopper[i].id && 0 < stHopper[i].iBucketId)) {
			pMixer->SetSpecialHandleFlg(true);
		}
	}
#else
	for (INT32 i = 0; i < nHopperCount; i++) {
		sprintf(szBuf, "hopper_%d", m_vHopidList[i]);
		LoadHopper(szHopperConfig, szBuf, stHopper + i);
		if ((221 == stHopper[i].id && 0 < stHopper[i].iBucketId) || (222 == stHopper[i].id && 0 < stHopper[i].iBucketId)) {
			pMixer->SetSpecialHandleFlg(true);
		}
    }
#endif

    qsort(stHopper,nHopperCount,sizeof(ST_HOPPER),CompareHopper4qosrt);

    //加载中途斗信息

    for(INT32 i=0; i<nBucketCount; i++){
#if _WIN32
        sprintf(szBuf,"bucket_%d",i+1);
#else
        sprintf(szBuf,"bucket_%d",i+1+500);
#endif
        LoadBucket(szHopperConfig,szBuf,stBucket+i);
    }

    //根据中途斗信息创建相应对象
    HopperNode* pBucket=NULL;
    for(INT32 i=0; i<nBucketCount; i++){
        pBucket = new HopperNode();
        pBucket->SetId( BKT_ID_TO_NODE_ID(stBucket[i].id) );
        pBucket->SetName(stBucket[i].szName);
        pBucket->AddTap(stBucket[i].iOutputTap1);
        pBucket->AddTap(stBucket[i].iOutputTap2);

        pMixer->AddChild(pBucket);
    }

    //根据料仓配置信息创建相应的DataNode
    HopperNode* pHopper=NULL;
    ValuedHopperNode *pValuedHopper=NULL;
    INT32 iValueNodeId;

    for(INT32 i=0; i<nHopperCount;i++){
		if (0 == stHopper[i].iAnalogCanId) {
			if (0 < stHopper[i].iMeaMode) {
				ValuedByTapHopperNode *tapvaluedHopper = new ValuedByTapHopperNode();
				tapvaluedHopper->SetName(stHopper[i].szName);
				tapvaluedHopper->SetId(NOANA_ID_TO_NODE_ID(stHopper[i].id));
				tapvaluedHopper->SetType(stHopper[i].itype);
				tapvaluedHopper->AddTap(stHopper[i].iOutputTap1);
				tapvaluedHopper->AddTap(stHopper[i].iOutputTap2);
				tapvaluedHopper->SetCommonRatio(stHopper[i].dMeaRatio);
				pMixer->AddChild(tapvaluedHopper);

				if (0 < stHopper[i].iInputTap1 || 0 < stHopper[i].iInputTap2) {
					pHopper = new HopperNode();
					pHopper->SetId(HPR_ID_TO_NODE_ID(stHopper[i].id));
					pHopper->SetType(stHopper[i].itype);
					pHopper->SetName(stHopper[i].szName);
					pHopper->SetData(stHopper[i].iData);
					pHopper->AddTap(stHopper[i].iInputTap1);
					pHopper->AddTap(stHopper[i].iInputTap2);
					tapvaluedHopper->AddChild(pHopper);
				}
			}
			continue;
		}
        iValueNodeId = ANA_ID_TO_NODE_ID(stHopper[i].iAnalogCanId);
        if( (pValuedHopper==NULL) || (iValueNodeId!=pValuedHopper->GetId()) ){
            //如果当前节点与下一个节点的模拟量相同，则是多对一结构；否则，是一对一结构
            if( ((i+1)<nHopperCount) && (iValueNodeId==ANA_ID_TO_NODE_ID(stHopper[i+1].iAnalogCanId)) ){
                pValuedHopper = new MultivaluedHopperNode();
                sprintf(szBuf,"[秤]%s",stHopper[i].szName);
                pValuedHopper->SetName(szBuf);
				LoadMeasureMethod(strConfigPath.c_str(), stHopper[i].iAnalogCanId, pValuedHopper);
			}
            else {
                pValuedHopper = new ValuedHopperNode();
				pValuedHopper->SetName(stHopper[i].szName);
				LoadMeasureMethod(strConfigPath.c_str(), stHopper[i].iAnalogCanId, pValuedHopper);
            }
            pValuedHopper->SetId( iValueNodeId );
            pValuedHopper->SetType(stHopper[i].itype);
            if(stHopper[i].iBucketId>0){
                pBucket=pMixer->FindChild( BKT_ID_TO_NODE_ID(stHopper[i].iBucketId) );
				if (pBucket) {
					pBucket->AddChild(pValuedHopper);
#if _WIN32
				} else if (221==stHopper[i].id || 222 == stHopper[i].id){
					//处理中储斗进水的结构
					pBucket = new HopperNode();
					pBucket->SetId(BKT_ID_TO_NODE_ID(stHopper[i].iBucketId));//外参剂
					string name = "外参剂中储斗";
					pBucket->SetName(name);
					for (int j = 0; j < nHopperCount; j++) {
						if (233 == stHopper[j].id ) {
							pBucket->AddTap(stHopper[j].iOutputTap1);
							pBucket->AddTap(stHopper[j].iOutputTap2);
						}
					}
					pMixer->AddChild(pBucket);
					pBucket->AddChild(pValuedHopper);
#endif
                }else{
                    pMixer->AddChild(pValuedHopper);
                    DebugErrLog("Invalid bucket %d for hopper %d\n",stHopper[i].iBucketId,stHopper[i].id);
                }
            }else{
                pMixer->AddChild(pValuedHopper);
            }
        }
        pValuedHopper->AddTap(stHopper[i].iOutputTap1);
        pValuedHopper->AddTap(stHopper[i].iOutputTap2);


        pHopper = new HopperNode();
        pHopper->SetId( HPR_ID_TO_NODE_ID(stHopper[i].id) );
		pHopper->SetType(stHopper[i].itype);
        pHopper->SetName(stHopper[i].szName);
        pHopper->SetData(stHopper[i].iData);
        pHopper->AddTap(stHopper[i].iInputTap1);
        pHopper->AddTap(stHopper[i].iInputTap2);
        pHopper->SetOpenDelay(stHopper[i].iOpenDelay);
        pHopper->SetCloseDelay(stHopper[i].iCloseDelay);
        if (TEMPERATURE_TPYE == stHopper[i].itype) {
            vector<int>::iterator it;
            for (it = stHopper[i].vIputTaps.begin(); it != stHopper[i].vIputTaps.end(); it++) {
                pHopper->AddTap(*it);
            }
        }
        pValuedHopper->AddChild(pHopper);
    }

#if _WIN32
	LoadAttrHop(szConfigPath, pMixer,HOPPER_ATTR_TYPE_CALC);
	LoadAttrHop(szConfigPath, pMixer,HOPPER_ATTR_TYPE_DEAL);
	LoadSCTemp(szConfigPath, pMixer);
#endif

    //print hopper tree to console
    DebugLog("CementMachineLoader:: ------------DUMP HOPPER TREE----------------.\n");
    pMixer->DumpTree();
    DebugLog("CementMachineLoader:: --------------------------------------------.\n");

    delete []stHopper;
    delete []stBucket;
    //返回
    return pMixer;
}

void CementMachineLoader::LoadHopper(const char* szConfigFile, const char* szAppName, ST_HOPPER *pHopper){
    CRWData rw;

    INT32 iCanId,iChannel;

    rw.GetProfileString(szAppName,"hopper_name","undefined",pHopper->szName,szConfigFile);
    pHopper->id = rw.GetProfileInt(szAppName,"hopper_id",-1,szConfigFile);
    pHopper->iBucketId = rw.GetProfileInt(szAppName,"bucket_id",-1,szConfigFile);
    pHopper->iAnalogCanId = rw.GetProfileInt(szAppName,"weighing_cell_id",-1,szConfigFile);
    pHopper->iData =  rw.GetProfileInt(szAppName,"hopper_material",-1,szConfigFile);

    iCanId = rw.GetProfileInt(szAppName,"loading_tap1_can_id",-1,szConfigFile);
    iChannel = rw.GetProfileInt(szAppName,"loading_tap1_channel_id",-1,szConfigFile);
    pHopper->iInputTap1 =(iCanId>0 && iChannel>0)? DIG_ID_TO_NODE_ID(iCanId,iChannel):0;

    iCanId = rw.GetProfileInt(szAppName,"loading_tap2_can_id",-1,szConfigFile);
    iChannel = rw.GetProfileInt(szAppName,"loading_tap2_channel_id",-1,szConfigFile);
    pHopper->iInputTap2 =(iCanId>0 && iChannel>0)? DIG_ID_TO_NODE_ID(iCanId,iChannel):0;

    iCanId = rw.GetProfileInt(szAppName,"blowing_tap1_can_id",-1,szConfigFile);
    iChannel = rw.GetProfileInt(szAppName,"blowing_tap1_channel_id",-1,szConfigFile);
    pHopper->iOutputTap1 =(iCanId>0 && iChannel>0)? DIG_ID_TO_NODE_ID(iCanId,iChannel):0;

    iCanId = rw.GetProfileInt(szAppName,"blowing_tap2_can_id",-1,szConfigFile);
    iChannel = rw.GetProfileInt(szAppName,"blowing_tap2_channel_id",-1,szConfigFile);
    pHopper->iOutputTap2 =(iCanId>0 && iChannel>0)? DIG_ID_TO_NODE_ID(iCanId,iChannel):0;

	if (pHopper->id > TEMPERATURE_TPYE) {
		pHopper->itype = TEMPERATURE_TPYE;
		LoadTHopInputTap(szConfigFile, szAppName, pHopper);
	} 
	else
	{
		pHopper->itype = REAL_WEIGHT_TPYE;
	}

	char retValue[10] = {0};
    rw.GetProfileString(szAppName,"loading_tap1_open_delay","0", retValue,szConfigFile);
	pHopper->iOpenDelay = atoi(retValue);
    rw.GetProfileString(szAppName,"loading_tap1_close_delay","0", retValue,szConfigFile);
	pHopper->iCloseDelay = atoi(retValue);
	pHopper->iMeaMode = rw.GetProfileInt(szAppName, "measure_mode", 0, szConfigFile);
	rw.GetProfileString(szAppName, "measure_unit", "0", retValue, szConfigFile);
	pHopper->dMeaRatio = atof(retValue);

    DebugErrLog("OpenDelay[%d], CloseDelay[%d] \n",pHopper->iOpenDelay, pHopper->iCloseDelay);
    DebugLog("Hopper id=%d, name=%s,analog=%d, input=%d, output=%d, bucket_id=%d, type = %d\n",
             pHopper->id,pHopper->szName, pHopper->iAnalogCanId, pHopper->iInputTap1, pHopper->iOutputTap1, pHopper->iBucketId,pHopper->itype);
}

void CementMachineLoader::LoadBucket(const char* szConfigFile, const char* szAppName, ST_HOPPER *pBucket){
#if _WIN32
	CRWData rw;
	int iCanId;
	char szKeyName[50];

	rw.GetProfileString(szAppName, "bucket_name", "undefined", pBucket->szName, szConfigFile);
	pBucket->id = rw.GetProfileInt(szAppName, "bucket_id", -1, szConfigFile);
	iCanId  = rw.GetProfileInt(szAppName, "blowing_tap1_id", -1, szConfigFile);
	pBucket->iOutputTap1 = (iCanId > 0) ? DIG_ID_TO_NODE_ID(iCanId, 0) : 0;

	pBucket->iData = 0;
	pBucket->iBucketId = 0;
	pBucket->iInputTap1 = 0;
	pBucket->iInputTap2 = 0;
	pBucket->iAnalogCanId = 0;
	pBucket->iOutputTap2 = 0;

#else
    LoadHopper(szConfigFile,szAppName,pBucket);
    pBucket->id = pBucket->iBucketId;
    pBucket->iBucketId = 0;
#endif
}

void CementMachineLoader::LoadMixer(const char* szConfigFile, const char* szAppName, MixerNode *pMixer){
    CRWData rw;
    char szBuf[LINESIZE];
    INT32 iMachineId,iCanId,iChannel;

    //从配置文件中读取拌和机基础信息
    szBuf[0]=0;
    iMachineId=rw.GetProfileInt(szAppName,"mix_machine_id",0,szConfigFile);
    rw.GetProfileString(szAppName,"hopper_name","MIX",szBuf,szConfigFile);
#if _WIN32
	iCanId = rw.GetProfileInt(szAppName, "blowing_tap_id", 0, szConfigFile);
	iChannel = 0;
#else
    iCanId=rw.GetProfileInt(szAppName,"blowing_tap_can_id",0,szConfigFile);
    iChannel=rw.GetProfileInt(szAppName,"blowing_tap_id",0,szConfigFile);
#endif

    pMixer->SetId( MIX_ID_TO_NODE_ID(iMachineId) );
    pMixer->SetName(szBuf);
    pMixer->AddTap(DIG_ID_TO_NODE_ID(iCanId,iChannel));
    //拌和机 放料阀门 ID和通道
}

void CementMachineLoader::LoadTHopInputTap(const char* szConfigFile, const char* szAppName, ST_HOPPER *pHopper)
{
	CRWData rw;
	INT32 count = 2;
	char szKeyName[LINESIZE];
	INT32 i, iCanId, iChannel;

    while (1) {
        sprintf(szKeyName, "loading_tap%d_can_id", count + 1);
		iCanId = rw.GetProfileInt(szAppName, szKeyName, -1, szConfigFile);
        sprintf(szKeyName, "loading_tap%d_channel_id", count + 1);
		iChannel = rw.GetProfileInt(szAppName, szKeyName, -1, szConfigFile);
		if (iCanId > 0 && iChannel > 0) {
			pHopper->vIputTaps.push_back( DIG_ID_TO_NODE_ID(iCanId, iChannel) );
            DebugLog("%s: %d\n", szKeyName, DIG_ID_TO_NODE_ID(iCanId, iChannel));
		}
		else {
			break;
		}
		count++;
	}
}

INT32 CementMachineLoader::GetHopperCount(const char* szConfigFile){
    CRWData rw;
    INT32 count=0;
    char szAppName[LINESIZE];
    INT32 i;

#if _WIN32
    while(1){
		sprintf(szAppName, "hopper_%d", count + 1);
		i = rw.GetProfileInt(szAppName, "hopper_id", -1, szConfigFile);
		if (i == -1) {
			break;
		}
		count++;
	}
#else
	int num = 0;
	printf("sizeof:%d\n", sizeof(hop_id) / sizeof(int));
	while(num < 19){
		sprintf(szAppName, "hopper_%d", hop_id[num]);
		num++;
       
        i=rw.GetProfileInt(szAppName,"hopper_id",-1,szConfigFile);
        if(i==-1){
			continue;
        }
		m_vHopidList.push_back(i);
        count++;
    }
#endif
    return count;
}


INT32 CementMachineLoader::GetBucketCount(const char* szConfigFile){
    CRWData rw;
    INT32 count=0;
    char szAppName[LINESIZE];
    INT32 i;
#if _WIN32
    while(1){
        sprintf(szAppName,"bucket_%d",count+1);
        i=rw.GetProfileInt(szAppName,"bucket_id",-1,szConfigFile);
        if(i<=0){
            break;
        }
        count++;
    }
#else
    int num = 500;
    while(num < 504 ){
        sprintf(szAppName,"bucket_%d",++num);
        i=rw.GetProfileInt(szAppName,"bucket_id",-1,szConfigFile);
        if(i<=0){
            continue;
        }
        count++;
    }
#endif

    return count;
}

int CementMachineLoader::CompareHopper4qosrt ( const void *a , const void *b ){
    INT32 ia=((const ST_HOPPER*)a)->iAnalogCanId;
    INT32 ib=((const ST_HOPPER*)b)->iAnalogCanId;

    if(ia>ib){
        return 1;
    }else if(ia==ib){
        return 0;
    }else{
        return -1;
    }
}

#if _WIN32
void CementMachineLoader::LoadHopAttrStruct(const char* szConfigFile, const char* szAppName, ST_HOPPER *pHopper, EN_HOPPER_ATTR_TYPE en_hat)
{
	CRWData rw;
	int iCanId;
	char szKeyName[50];

	if (HOPPER_ATTR_TYPE_CALC == en_hat) {
		strcpy(szKeyName, "calc_id");
		pHopper->itype = CALC_VALUE_TPYE;
	}
	else if (HOPPER_ATTR_TYPE_DEAL == en_hat) {
		strcpy(szKeyName, "set_id");
		pHopper->itype = DEAL_VALUE_TPYE;
	} else if (HOPPER_ATTR_TYPE_TEMPERATURE == en_hat) {
		strcpy(szKeyName, "temperature_id");
		pHopper->itype = DEAL_VALUE_TPYE;
	}
	else {
		strcpy(szKeyName, "weighing_cell_id");
		pHopper->itype = REAL_WEIGHT_TPYE;
	}

	rw.GetProfileString(szAppName, "hopper_name", "undefined", pHopper->szName, szConfigFile);
	pHopper->id = rw.GetProfileInt(szAppName, "hopper_id", -1, szConfigFile);
	pHopper->iBucketId = rw.GetProfileInt(szAppName, "bucket_id", -1, szConfigFile);
	pHopper->iAnalogCanId = rw.GetProfileInt(szAppName, szKeyName, -1, szConfigFile);
	pHopper->iData = rw.GetProfileInt(szAppName, "hopper_material", -1, szConfigFile);

	iCanId = rw.GetProfileInt(szAppName, "loading_tap1_id", -1, szConfigFile);
	pHopper->iInputTap1 = (iCanId > 0) ? DIG_ID_TO_NODE_ID(iCanId, 0) : 0;

	iCanId = rw.GetProfileInt(szAppName, "loading_tap2_id", -1, szConfigFile);
	pHopper->iInputTap2 = (iCanId > 0) ? DIG_ID_TO_NODE_ID(iCanId, 0) : 0;

	iCanId = rw.GetProfileInt(szAppName, "blowing_tap1_id", -1, szConfigFile);
	pHopper->iOutputTap1 = (iCanId > 0) ? DIG_ID_TO_NODE_ID(iCanId, 0) : 0;

	iCanId = rw.GetProfileInt(szAppName, "blowing_tap2_id", -1, szConfigFile);
	pHopper->iOutputTap2 = (iCanId > 0) ? DIG_ID_TO_NODE_ID(iCanId, 0) : 0;

	char retValue[10] = { 0 };
	rw.GetProfileString(szAppName, "loading_tap1_open_delay", "0", retValue, szConfigFile);
	pHopper->iOpenDelay = atoi(retValue);
	rw.GetProfileString(szAppName, "loading_tap1_close_delay", "0", retValue, szConfigFile);
	pHopper->iCloseDelay = atoi(retValue);

	pHopper->iMeaMode = rw.GetProfileInt(szAppName, "measure_mode", 0, szConfigFile);
	rw.GetProfileString(szAppName, "measure_unit", "0", retValue, szConfigFile);	
	pHopper->dMeaRatio = atof(retValue);
	//DebugLog("[LoadHopAttrStruct]Hopper lignin Method[%d][%lf]\n", pHopper->iMeaMode, pHopper->dMeaRatio);
	DebugLog("[LoadHopAttrStruct]Hopper id=%d, name=%s,analog=%d, input=%d, output=%d, type = %d\n",
		pHopper->id, pHopper->szName, pHopper->iAnalogCanId, pHopper->iInputTap1, pHopper->iOutputTap1, pHopper->itype);
}
int CementMachineLoader::GetHopAttrCount(const char* szConfigFile, EN_HOPPER_ATTR_TYPE en_hat)
{
	CRWData rw;
	INT32 count = 0;
	char szAppName[LINESIZE];
	char szKeyName[50];
	INT32 i;
	if (HOPPER_ATTR_TYPE_CALC == en_hat) {
		strcpy(szKeyName, "calc_id");
	}
	else if (HOPPER_ATTR_TYPE_DEAL == en_hat) {
		strcpy(szKeyName, "set_id");
	}
	else if (HOPPER_ATTR_TYPE_REAL == en_hat) {
		strcpy(szKeyName, "weighing_cell_id");
	}
	else if (HOPPER_ATTR_TYPE_TEMPERATURE == en_hat) {
		strcpy(szKeyName, "temperature_id");
	}
	else {
		DebugLog("[CementMachineLoader]GetHopAttrCount Hop Attribute Type Invaild!!");
		return count;
	}

	while (1) {
		sprintf(szAppName, "hopper_%d", count + 1);
		i = rw.GetProfileInt(szAppName, szKeyName, -1, szConfigFile);
		if (i == -1) {
			break;
		}
		count++;
	}
	return count;
}

void CementMachineLoader::LoadAttrHop(const char* szConfigPath, MixerNode* pMixer, EN_HOPPER_ATTR_TYPE en_hat)
{
	string strConfigPath = szConfigPath;
	string strHopperConfig = strConfigPath + CONFIG_HOPPER_FILE;
	const char *szHopperConfig = strHopperConfig.c_str();
	char szBuf[LINESIZE];

	//计算料仓和中途斗数量并分配内存
	int  nHopAttrCount = GetHopAttrCount(szHopperConfig, en_hat);
	INT32 nBucketCount = GetBucketCount(szHopperConfig);
	ST_HOPPER *stHopper = new ST_HOPPER[nHopAttrCount];
	ST_HOPPER *stBucket = new ST_HOPPER[nBucketCount];

	//加载料仓信息，并通过排序将模拟量相同的料仓拍在一起
	for (INT32 i = 0; i < nHopAttrCount; i++) {
		sprintf(szBuf, "hopper_%d", i + 1);
		LoadHopAttrStruct(szHopperConfig, szBuf, stHopper + i, en_hat);
	}
	//qsort(stHopper, nHopAttrCount, sizeof(ST_HOPPER), CompareHopper4qosrt);

	//加载中途斗信息
	//for (INT32 i = 0; i < nBucketCount; i++) {
	//	sprintf(szBuf, "bucket_%d", i + 1);
	//	LoadBucket(szHopperConfig, szBuf, stBucket + i);
	//}

	//根据中途斗信息创建相应对象
	HopperNode* pBucket = NULL;
	//for (INT32 i = 0; i < nBucketCount; i++) {
	//	pBucket = new HopperNode();
	//	pBucket->SetId(BKT_ID_TO_NODE_ID(stBucket[i].id));
	//	pBucket->SetName(stBucket[i].szName);
	//	pBucket->AddTap(stBucket[i].iOutputTap1);
	//	pBucket->AddTap(stBucket[i].iOutputTap2);

	//	pMixer->AddChild(pBucket);
	//}

	//根据料仓配置信息创建相应的DataNode
	HopperNode* pHopper = NULL;
	ValuedHopperNode *pValuedHopper = NULL;
	INT32 iValueNodeId;

	for (INT32 i = 0; i < nHopAttrCount;i++) {
		if (stHopper[i].iAnalogCanId <= 0) {
			continue;
		}
		iValueNodeId = ANA_ID_TO_NODE_ID(stHopper[i].iAnalogCanId);
		if ((pValuedHopper == NULL) || (iValueNodeId != pValuedHopper->GetId())) {
			//如果当前节点与下一个节点的模拟量相同，则是多对一结构；否则，是一对一结构
			if (((i + 1) < nHopAttrCount) && (iValueNodeId == ANA_ID_TO_NODE_ID(stHopper[i + 1].iAnalogCanId))) {
				pValuedHopper = new MultivaluedHopperNode();
				sprintf(szBuf, "[Analog]%s", stHopper[i].szName);
				pValuedHopper->SetName(szBuf);
			}
			else {
				pValuedHopper = new ValuedHopperNode();
				pValuedHopper->SetName(stHopper[i].szName);
				LoadMeasureMethod(strConfigPath.c_str(), stHopper[i].iAnalogCanId, pValuedHopper);
			}
			pValuedHopper->SetId(iValueNodeId);
			pValuedHopper->SetType(stHopper[i].itype);
			if (stHopper[i].iBucketId > 0) {
				pBucket = pMixer->FindChild(BKT_ID_TO_NODE_ID(stHopper[i].iBucketId));
				if (pBucket) {
					pBucket->AddChild(pValuedHopper);
				}
				else {
					pMixer->AddChild(pValuedHopper);
					DebugErrLog("Invalid bucket %d for hopper %d\n", stHopper[i].iBucketId, stHopper[i].id);
				}
			}
			else {
				pMixer->AddChild(pValuedHopper);
			}
		}
		pValuedHopper->AddTap(stHopper[i].iOutputTap1);
		pValuedHopper->AddTap(stHopper[i].iOutputTap2);


		pHopper = new HopperNode();
		pHopper->SetId(HPR_ID_TO_NODE_ID(stHopper[i].id));
		pHopper->SetType(stHopper[i].itype);
		pHopper->SetName(stHopper[i].szName);
		pHopper->SetData(stHopper[i].iData);
		pHopper->AddTap(stHopper[i].iInputTap1);
		pHopper->AddTap(stHopper[i].iInputTap2);
		pHopper->SetOpenDelay(stHopper[i].iOpenDelay);
		pHopper->SetCloseDelay(stHopper[i].iCloseDelay);
		if (TEMPERATURE_TPYE == stHopper[i].itype) {
			vector<int>::iterator it;
			for (it = stHopper[i].vIputTaps.begin(); it != stHopper[i].vIputTaps.end(); it++) {
				pHopper->AddTap(*it);
			}
		}
		pValuedHopper->AddChild(pHopper);
	}
}

void CementMachineLoader::LoadSCTemp(const char* szConfigPath, MixerNode* pMixer)
{
	string strHopperConfig = szConfigPath + string(CONFIG_HOPPER_FILE);

	//计算料仓数量并分配内存
	int  nHopAttrCount = GetHopAttrCount(strHopperConfig.c_str(), HOPPER_ATTR_TYPE_REAL);
	ST_HOPPER *stHopper = new ST_HOPPER[nHopAttrCount];
	char szBuf[20];

	//加载料仓信息，并通过排序将模拟量相同的料仓排在一起
	for (INT32 i = 0; i < nHopAttrCount; i++) {
		sprintf(szBuf, "hopper_%d", i + 1);
		LoadHopAttrStruct(strHopperConfig.c_str(), szBuf, stHopper + i, HOPPER_ATTR_TYPE_REAL);
	}

	CRWData rw;
	ValuedHopperNode *pTempHopper = NULL;
	HopperNode* pTChildHop = NULL;
	//搅拌机温度(401),type:400	
	int outputtap, temp_id;
	temp_id = rw.GetProfileInt("mixer", "temperature_id", 0, strHopperConfig.c_str());
	outputtap = rw.GetProfileInt("mixer", "blowing_tap_id", 0, strHopperConfig.c_str());
	if (temp_id > 0) {
		pTempHopper = new ValuedHopperNode();
		pTempHopper->SetId(ANA_ID_TO_NODE_ID(temp_id));
		pTempHopper->SetName(string("混合料温度"));
		pTempHopper->SetType(TEMPERATURE_TPYE);
		pTempHopper->SetData(0);
		pTempHopper->AddTap(DIG_ID_TO_NODE_ID(outputtap, 0));
		LoadMeasureMethod(szConfigPath, temp_id, pTempHopper);

		pTChildHop = new HopperNode();
		pTChildHop->SetId(HPR_ID_TO_NODE_ID(401));
		pTChildHop->SetType(TEMPERATURE_TPYE);
		pTChildHop->SetName(string("混合料温度"));
		pTChildHop->SetData(0);
		for (int i = 0; i < nHopAttrCount; i++) {
			if (232 == stHopper[i].id) {
				pTChildHop->AddTap(stHopper[i].iOutputTap1);
				pTChildHop->AddTap(stHopper[i].iOutputTap2);
			}
		}
	
		//vector<DataNode*> *pChildren = pMixer->GetChildren();
		//INT32 nChildrenCount = pChildren->size();
		//int *data;
		//int count = 0;

		//for (int i = 0; i < nChildrenCount; i++) {
		//	if (TEMPERATURE_TPYE == ((HopperNode*)(pChildren->at(i)))->GetType()) {
		//		//不累计温度子节点的阀门
		//		continue;
		//	}
		//	data = ((HopperNode*)(pChildren->at(i)))->GetTaps()->data();
		//	count = ((HopperNode*)(pChildren->at(i)))->GetTaps()->size();
		//	for (INT32 i = 0; i < count; i++) {
		//		pTChildHop->AddTap(*(data + i));
		//	}
		//}
		pMixer->AddChild(pTempHopper);
		pTempHopper->AddChild(pTChildHop);
	}

	//计算料仓数量并分配内存
	nHopAttrCount = GetHopAttrCount(strHopperConfig.c_str(), HOPPER_ATTR_TYPE_TEMPERATURE);
	delete[] stHopper;
	stHopper = new ST_HOPPER[nHopAttrCount];
	//char szBuf[20];

	//加载料仓信息，并通过排序将模拟量相同的料仓排在一起
	for (INT32 i = 0; i < nHopAttrCount; i++) {
		sprintf(szBuf, "hopper_%d", i + 1);
		LoadHopAttrStruct(strHopperConfig.c_str(), szBuf, stHopper + i, HOPPER_ATTR_TYPE_TEMPERATURE);
	}

	//沥青温度(402),type:400	
	pTempHopper = new ValuedHopperNode();
	for (int i = 0; i < nHopAttrCount; i++ ) {
		if (232 == stHopper[i].id && 0 < stHopper[i].iAnalogCanId ) {
			//找到沥青料仓
			pTempHopper->SetId(ANA_ID_TO_NODE_ID(stHopper[i].iAnalogCanId));
			pTempHopper->SetName(string("沥青温度"));
			pTempHopper->SetType(TEMPERATURE_TPYE);
			pTempHopper->SetData(0);
			pTempHopper->AddTap(stHopper[i].iOutputTap1);
			pTempHopper->AddTap(stHopper[i].iOutputTap2);
			LoadMeasureMethod(szConfigPath, stHopper[i].iAnalogCanId, pTempHopper);
			pMixer->AddChild(pTempHopper);
			pTChildHop = new HopperNode();
			pTChildHop->SetId(HPR_ID_TO_NODE_ID(402));
			pTChildHop->SetType(TEMPERATURE_TPYE);
			pTChildHop->SetName(string("沥青温度"));
			pTChildHop->SetData(0);
			pTChildHop->AddTap(stHopper[i].iInputTap1);
			pTChildHop->AddTap(stHopper[i].iInputTap2);
			pTempHopper->AddChild(pTChildHop);
		}
	}

	//骨料温度(403),type:400
	pTempHopper = new ValuedHopperNode();
	pTChildHop = new HopperNode();
	bool is_find = false;
	for (int i = 0; i < nHopAttrCount; i++) {
		if (201 <= stHopper[i].id && 206 >= stHopper[i].id && 0 < stHopper[i].iAnalogCanId) {
			//找到骨料料仓
			if (!is_find) {
				is_find = true;
				pTempHopper->SetId(ANA_ID_TO_NODE_ID(stHopper[i].iAnalogCanId));
				pTempHopper->SetName(string("骨料温度"));
				pTempHopper->SetType(TEMPERATURE_TPYE);
				pTempHopper->SetData(0);
				LoadMeasureMethod(szConfigPath, stHopper[i].iAnalogCanId, pTempHopper);
				//pTempHopper->AddTap(DIG_ID_TO_NODE_ID(outputtap, 0));

				pTChildHop->SetId(HPR_ID_TO_NODE_ID(403));
				pTChildHop->SetType(TEMPERATURE_TPYE);
				pTChildHop->SetName(string("骨料温度"));
				pTChildHop->SetData(0);
				
			}
			if (0 < stHopper[i].iBucketId) {
				pTChildHop->AddTap(stHopper[i].iBucketId);
			}
			else {
				pTChildHop->AddTap(stHopper[i].iOutputTap1);
				pTChildHop->AddTap(stHopper[i].iOutputTap2);
			}
			
		}
		if (232 == stHopper[i].id) {
			pTempHopper->AddTap(stHopper[i].iOutputTap1);
			pTempHopper->AddTap(stHopper[i].iOutputTap2);
		}
	}
	if (is_find) {
		pMixer->AddChild(pTempHopper);
		pTempHopper->AddChild(pTChildHop);
	}

	delete[] stHopper;
}
#endif

void CementMachineLoader::LoadMeasureMethod(const char* szConfigPath, int iAnaId, ValuedHopperNode *pVHopNode)
{
	string strHopperConfig = szConfigPath + string(CONFIG_ANALOG_FILE);
	CRWData rw;
	int iMeasureType;
	int iCount = 0;
	char szAppName[LINESIZE] = {0};
	int iCanid;
	bool bIsFindAndId = false;

	while (1) {
		sprintf(szAppName, "analogsensor_%d", iCount + 1);
#if _WIN32
		iCanid = rw.GetProfileInt(szAppName, "analogsensor_id", -1, strHopperConfig.c_str());
#else
        iCanid = rw.GetProfileInt(szAppName, "can_id", -1, strHopperConfig.c_str());
#endif
		if (-1 == iCanid) {
			DebugLog("Not Find Canid [%d]\n", iCount + 1);
			break;
		}
		if (iAnaId == iCanid) {
			bIsFindAndId = true;
			break;
		}
		iCount++;
	}

	if (bIsFindAndId) {
		iMeasureType = rw.GetProfileInt(szAppName, "measure_method", 0, strHopperConfig.c_str());
		pVHopNode->AddMeasureMethod((EN_ANA_MEASURE_TYPE)iMeasureType);
		
		if ( ANA_MEASURE_SINGLE == iMeasureType) {
			printf("ID[%d]",iAnaId);
			LoadValueAlgorithm(strHopperConfig.c_str(), szAppName, 1, pVHopNode);
		}
		else if ( ANA_MEASURE_DIFF == iMeasureType) {
			printf("ID[%d]", iAnaId);
			LoadValueAlgorithm(strHopperConfig.c_str(), szAppName, 2, pVHopNode);
		}
		else {
			DebugErrLog("ID[%d] Get MeasureMethod[%d] Invalid! \n", iAnaId,iMeasureType);
			char szErr[128] = { 0 };
            sprintf(szErr, "ID[%d] Get MeasureMethod[%d] Invalid!", iAnaId,iMeasureType);
            string szLogPath = string(CONFIG_LOG_PATH) + string(ERROR_LOG_FILE);
            rw.WriteDebugForLog(szErr, (char *)szLogPath.c_str());
		}
	}

}

void CementMachineLoader::LoadValueAlgorithm(const char* strAnaConfigPath, const char *szAppName, int iCount, ValuedHopperNode *pVHopNode)
{
	CRWData rw;
	int iCalcType;
	char szKeyName[30] = { 0 };
	
	AlgorithmFactory cAlgorithm;
	for (int i = 1; i <= iCount; i++) {
		ST_AREA_CONF* pstArea = new ST_AREA_CONF;
		sprintf(szKeyName, "value%d_calc_method", i);
		iCalcType = rw.GetProfileInt(szAppName, szKeyName, 0, strAnaConfigPath);

		sprintf(szKeyName, "value%d_start_at", i);
		pstArea->startArea = (EN_AREA_TYPE)rw.GetProfileInt(szAppName, szKeyName, 0, strAnaConfigPath);
		sprintf(szKeyName, "value%d_start_delay", i);
		pstArea->startDelay = rw.GetProfileInt(szAppName, szKeyName, 0, strAnaConfigPath);

		sprintf(szKeyName, "value%d_end_at", i);
		pstArea->endArea = (EN_AREA_TYPE)rw.GetProfileInt(szAppName, szKeyName, 0, strAnaConfigPath);
		sprintf(szKeyName, "value%d_end_delay", i);
		pstArea->endDelay = rw.GetProfileInt(szAppName, szKeyName, 0, strAnaConfigPath);

		DataAlgorithm * pDataAlgorithm = cAlgorithm.createAlgorithm((EN_CALC_METHOD_TYPE)iCalcType);
		if (NULL == pDataAlgorithm) {
			char szErr[128] = {0};
            SydTime now;
            sprintf(szErr, "[%s]Create Algorithm[%d] Error", now.ToString().c_str(),iCalcType);
            string szLogPath = string(CONFIG_LOG_PATH) + string(ERROR_LOG_FILE);
            rw.WriteDebugForLog(szErr, (char *)szLogPath.c_str());
		}
		else {
			pstArea->pDataAlgorithm = pDataAlgorithm;
			pVHopNode->AddArea(pstArea);
			DebugLog("@[LoadMeasureMethod_%d]measureMethod[%d],calcType[%d],area_start[%d]delay[%d],area_end[%d]delay[%d] \n",
				i, iCount, iCalcType, pstArea->startArea, pstArea->startDelay, pstArea->endArea, pstArea->endDelay);
		}
	}
}
