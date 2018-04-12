#include "multivalued_hopper_node.h"
#include "../../Include/DebugLogger.h"
#include <sstream>

#define TRACE_ACTION_START  1
#define TRACE_ACTION_FINISH 2

MultivaluedHopperNode::MultivaluedHopperNode()
{
}

void MultivaluedHopperNode::Initialize(DataProvider *pDataProvider){
	ValuedHopperNode::Initialize(pDataProvider);

    m_vecTraces.clear();
	m_vecHopid.clear();
}

void MultivaluedHopperNode::Execute(){
	ValuedHopperNode::Execute();
    if(m_pDataProvider==NULL || !m_pDataProvider->IsRunning()){
        if(m_pDataValueMgr->GetValueCount()>0){
            DebugLog("%s:: DataProvider isn't running, clear trace items.\n",m_strName.c_str());
			m_pDataValueMgr->ClearValue();
        }
    }
}

void MultivaluedHopperNode::FireOutputStart(){
	ST_HOPPER_VALUE stHopVal(GetId(), m_pDataProvider);
	m_pDataValueMgr->PushValue(stHopVal);
    map<int,double> mapValues;   //汇总每个料仓的上料量， key->HoperId , value-->value
    ST_VALUE_TRACE  *pTrace, *pPrevTrace=NULL;
    ST_HOPPER_VALUE *pValue;
	ST_HOPPER_VALUE *pValueData = m_pDataValueMgr->GetValue()->data();//m_vecValues.data();
    INT32 iValueIndex,iSavedValueIndex=-1;
    double iSavedValue=0;
    INT32 iTraceCount =(INT32)m_vecTraces.size();

    pTrace=m_vecTraces.data();
    pPrevTrace=NULL;
    for(INT32 i=0; i<iTraceCount; i++, pTrace++){
        iValueIndex = m_pDataValueMgr->FindValue(pTrace->time);
        if(iValueIndex<0){
            string s=pTrace->time.ToString();
            DebugErrLog("%s couldn't find value for timestamp: %s, i=%d, count=%d\n",m_strName.c_str(), s.c_str(),i,iTraceCount);
            continue;
        }

        if( pPrevTrace==NULL){
            //将这一部分重量计入第一个料仓
            if(iValueIndex>0){
                iValueIndex = m_pDataValueMgr->MinValue(0,iValueIndex);
            }
            pValue=pValueData + iValueIndex;
            //mapValues[pTrace->iHopperId] = pValue->value;
            iSavedValueIndex = iValueIndex;
            iSavedValue = pValue->value;//mapValues[pTrace->iHopperId];
        }else{
            if( pTrace->iAction==TRACE_ACTION_START ){
                if( pPrevTrace->iAction==TRACE_ACTION_FINISH ){
                    iValueIndex = m_pDataValueMgr->MinSlope(iSavedValueIndex,iValueIndex+1);
                }
                pValue = pValueData+iValueIndex;
                iSavedValue = mapValues[pPrevTrace->iHopperId];
                if(iSavedValueIndex>=0){
                    mapValues[pPrevTrace->iHopperId] = iSavedValue + (pValue->value-(pValueData+iSavedValueIndex)->value);
                }else{
                    mapValues[pPrevTrace->iHopperId] = iSavedValue + pValue->value;
                }
                iSavedValueIndex = iValueIndex;
                iSavedValue = mapValues[pPrevTrace->iHopperId];
            }else if( i<(iTraceCount-1) ){
                pValue = pValueData + iValueIndex;
                iSavedValue = mapValues[pTrace->iHopperId];
                mapValues[pTrace->iHopperId] = iSavedValue + (pValue->value-(pValueData+iSavedValueIndex)->value);
                iSavedValueIndex = iValueIndex;
                iSavedValue = mapValues[pTrace->iHopperId];
            }else{
                iValueIndex = m_pDataValueMgr->MaxValue(iValueIndex,m_pDataValueMgr->GetValueCount());
                pValue = pValueData + iValueIndex;
                iSavedValue = mapValues[pTrace->iHopperId];
                mapValues[pTrace->iHopperId] = iSavedValue + (pValue->value-(pValueData+iSavedValueIndex)->value);
                iSavedValueIndex = iValueIndex;
                iSavedValue = mapValues[pTrace->iHopperId];
            }
        }//end if
        pPrevTrace=pTrace;
    }//end while

    //根据前面统计的结果，将数据以ProductChild的形式增加到当前Hopper的Product中
//    map<INT32,INT32>::iterator itValues=mapValues.begin();
//    HopperNode *pChild;

//    while(itValues!=mapValues.end()){
//        pChild = FindChild(itValues->first);
//        pChild->GetProduct()->SetValue(itValues->second);
//        itValues++;
//    }
    vector<HopperProduct>* pChildrenProduct=GetProduct()->GetChildren();
    HopperProduct* pProduct = pChildrenProduct->data();
    INT32 iCount = pChildrenProduct->size();
    map<int,double>::iterator itValue;
	char strValue[20];

    for(INT32 i=0; i<iCount; i++){
        itValue=mapValues.find((pProduct+i)->GetId());
        if(itValue!=mapValues.end()){
            (pProduct+i)->SetValue(itValue->second);
			sprintf(strValue, "%.3lf", itValue->second);
			(pProduct + i)->SetRawValue(string(strValue));
		}
		else {
			(pProduct + i)->SetValue(0.0);
			(pProduct + i)->SetRawValue("0.0");
		}
    }

    //
    m_vecTraces.clear();
	m_vecHopid.clear();
    //执行父类的方法
    HopperNode::FireOutputStart();
}

void MultivaluedHopperNode::FireOutputDone() {
	//m_vecHopid.clear();
	//m_vecValues.clear();
	m_pDataValueMgr->ClearValue();
	ST_HOPPER_VALUE stHopValue(GetId(), m_pDataProvider);
	m_pDataValueMgr->PushValue(stHopValue);
	HopperNode::FireOutputDone();
    //printf("OutPutDone NotifyChild:false-->true[%d]\n", GetId());
	vector<DataNode*>::iterator it;
	HopperNode* pChild;
	for (it = m_vecChildren.begin(); it != m_vecChildren.end(); it++) {
		pChild = (HopperNode*)*it;
		if (!pChild->GetHopStatus()->bOutputing)
			pChild->SetIsAddProduct(true);
	}
}

/**
  @brief 子节点开始放料
  */
void MultivaluedHopperNode::OnOutputStart(HopperNode *pHopper){
	bool b_isSamehop = false;
	for (int i = 0; i < m_vecHopid.size(); i++ ) {
		if (m_vecHopid[i] == pHopper->GetId()) {
			//printf("@@@[MultivaluedHopperNode::OnOutputStart]===id[%d]==[%d]=id[%d]=value[%s]@@@@@@@\n", pHopper->GetId(), m_isaddproduct, m_vecHopid[i], pHopper->m_isaddproduct?"true":"false");
			//return;
			b_isSamehop = true;
		}
	}
	if (!b_isSamehop) {		
		m_vecHopid.push_back(pHopper->GetId());
		HopperNode::OnOutputStart(pHopper);
	}
	//ValuedHopperNode::OnOutputStart(pHopper);
	//printf("===[MultivaluedHopperNode::OnOutputStart]====id[%d]=[%d]==value[%d]==========\n", pHopper->GetId(),m_isaddproduct, pHopper->GetProduct()->GetValue());
    ST_VALUE_TRACE vt;
    vt.time = pHopper->GetDataProvider()->GetTimestamp();
    vt.iHopperId = pHopper->GetId();
    vt.iAction = TRACE_ACTION_START;

    m_vecTraces.push_back(vt);
}

/**
  @brief 子节点放料结束
  */
void MultivaluedHopperNode::OnOutputDone(HopperNode *pHopper){
    ST_VALUE_TRACE vt;

    vt.time = pHopper->GetDataProvider()->GetTimestamp();
    vt.iHopperId = pHopper->GetId();
    vt.iAction = TRACE_ACTION_FINISH;

    m_vecTraces.push_back(vt);

	HopperNode::OnOutputDone(pHopper);
}

string MultivaluedHopperNode::ToString(){
    string str="[M2O] ";
    str += HopperNode::ToString();

    return str;
}

//double MultivaluedHopperNode::Measure(){
//    return ValuedHopperNode::Measure();
//}
