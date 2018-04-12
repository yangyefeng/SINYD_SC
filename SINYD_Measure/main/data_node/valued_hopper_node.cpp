#include <sstream>

#include "valued_hopper_node.h"
#include "../../Include/DebugLogger.h"

#define abs(a) ((a)>=0?(a):(-(a)))

ValuedHopperNode::ValuedHopperNode()
{
	m_pDataValueMgr = NULL;
}

void ValuedHopperNode::Initialize(DataProvider *pDataProvider){
    HopperNode::Initialize(pDataProvider);
	m_stProcessIdx.SetComIndex(-1);
	m_bInAndOutTogetherAction = false;
	m_bIsExistOutDoneDelay = false;
	m_pDataValueMgr = new DataNodeValueMgr();
	m_pDataValueMgr->Initialize(NODE_ID_TO_ANA_ID(GetId()));
}

void ValuedHopperNode::Execute(){

	if (m_pDataProvider && m_pDataProvider->IsRunning()) {
		ST_HOPPER_VALUE stHopValue(GetId(),m_pDataProvider);
		m_pDataValueMgr->PushValue(stHopValue);
	}
//延时时间到
	if (m_bIsExistOutDoneDelay && m_OutDoneDelayTime <= (m_pDataValueMgr->GetValue()->data() + (m_pDataValueMgr->GetValueCount() - 1))->time)	{
		FireOutputDone();
	}

    HopperNode::Execute();

	if (m_pDataProvider == NULL || !m_pDataProvider->IsRunning()) {
		if ( m_pDataValueMgr->GetValueCount() > 0 ) {
			DebugLog("%s:: DataProvider isn't running, clear trace items.\n", m_strName.c_str());
			m_pDataValueMgr->ClearValue();
		}
	}
    //if(m_pDataProvider && m_pDataProvider->IsRunning()){
    //    PushValue();
    //}else if( GetValueCount()>0 ){
    //    DebugLog("%s:: DataProvider isn't running, clear values.\n",m_strName.c_str());
    //    m_vecValues.clear();
    //}
}

void ValuedHopperNode::Release()
{
	ST_AREA_CONF *pArea;
	for (vector<ST_AREA_CONF*>::iterator i = m_pAreaConf.begin(); i != m_pAreaConf.end(); i++) {
		pArea = *i;
		if (NULL != pArea->pDataAlgorithm) {
			delete pArea->pDataAlgorithm;
			pArea->pDataAlgorithm = NULL;
		}
		delete pArea;
	}
	m_pAreaConf.clear();

	if (m_pDataValueMgr) {
		delete m_pDataValueMgr;
		m_pDataValueMgr = NULL;
	}

	HopperNode::Release();
}

bool ValuedHopperNode::RefreshStatus(){
//    ST_HOPPER_STATUS stOldStatus = m_stStatus;
	if (m_pDataProvider && m_pDataProvider->IsRunning()) {
		JudgeIllegal();
	}
    bool bRet=HopperNode::RefreshStatus();

	return bRet;
}

void ValuedHopperNode::FireOutputStart(){
	if (m_bIsExistOutDoneDelay) {
		FireOutputDone();
	}

	if ( m_pDataValueMgr->GetValueCount() > m_stProcessIdx.OutStartIdx) {
		m_stProcessIdx.OutStartIdx = m_pDataValueMgr->GetValueCount();
	}

	if (97401 == (((HopperNode*)m_vecChildren[0])->GetProduct())->GetId() ) {
		//沥青混合料温度在放料开始时计算
		Measure();
		//混合料温度在放料开始时就记录结束时间
		//因为混合料温度的放料阀门使用的是搅拌锅放料阀门
		//避免当一盘结束（搅拌锅放料开）后，混合料温度的结束时间还没有被设置
		m_product.SetEndTime();
	}

	if (!m_product.IsValueReady() && !m_product.IsRunning()) {
		//一盘中，只有放料动作，没有上料过程的处理
		m_stProcessIdx.InEndIdx = m_stProcessIdx.OutStartIdx;
		m_bRepeatOutputStatus = false;
		m_isaddproduct = true;
	}
	
    HopperNode::FireOutputStart();
}

void ValuedHopperNode::FireOutputDone(){
	if ( ( m_bRepeatOutputStatus && (REAL_WEIGHT_TPYE != GetType()) ) ||
		  ( 97401 == (((HopperNode*)m_vecChildren[0])->GetProduct())->GetId() ) ) {
		//多次放料的时候，除秤重量，其他值不再多次计量
		//沥青混合料温度不在放料结束的时机进行计量
		return;
	}

	bool bInputing = IsInputting();
	if ((bInputing && bInputing == GetHopStatus()->bInputting) ) {
		//当放料完成的时候，上料阀门一直处在开启状态的时候，给上料结束点Index赋值为放料开始点
		if (0 > m_stProcessIdx.InEndIdx) {
			m_stProcessIdx.InEndIdx = m_stProcessIdx.OutStartIdx;//m_pDataValueMgr->GetValueCount();
		}
	}
	else if (!m_bIsExistOutDoneDelay && IsExistOutputDoneAfterDelay()) {
		//存在放料阀门关闭的延时，需要等待延时到时间的时候在进行计量
		m_bIsExistOutDoneDelay = true;
		m_stProcessIdx.OutEndIdx = m_pDataValueMgr->GetValueCount();
		return;
	}

	if (m_pDataValueMgr->GetValueCount() > m_stProcessIdx.OutEndIdx && !m_bIsExistOutDoneDelay) {
		m_stProcessIdx.OutEndIdx = m_pDataValueMgr->GetValueCount();
	}

	DebugLog("[FireOutputDone]Id[%d]size[%d]inS[%d],inE[%d],outS[%d],OutE[%d]\n", GetId(), m_pDataValueMgr->GetValueCount(),m_stProcessIdx.InStartIdx,
		m_stProcessIdx.InEndIdx, m_stProcessIdx.OutStartIdx, m_stProcessIdx.OutEndIdx);
	
	//放料时机的计量计算
	Measure();

	//m_product.SetValue(Measure());
    //放料结束，清空数据队列
	m_pDataValueMgr->ClearValue();
	ST_HOPPER_VALUE stHopValue(GetId(), m_pDataProvider);
	m_pDataValueMgr->PushValue(stHopValue);
	m_stProcessIdx.SetComIndex(1);
	m_bRepeatOutputStatus = true;
	m_bIsExistOutDoneDelay = false;

	HopperNode::FireOutputDone();

	//进料和放料同时开启的时候，如下的处理
	if (m_bInAndOutTogetherAction ) {
		//在放料中，开启上料阀门，同时放料结束了，上料阀门还没有关闭的时候，强制调用上料开始
		m_bInAndOutTogetherAction = false;
		FireInputStart();
	}
	else {
		if (bInputing && bInputing == GetHopStatus()->bInputting) {
			//一对一违规补料，上料过程中，进行放料，放料结束的是时候，上料还没有结束
			if ( ((HopperNode*)m_vecChildren[0])->GetProduct()->IsRunning() ) {
				((HopperNode*)m_vecChildren[0])->GetProduct()->Finish();
			}
			HopperNode::OnOutputStart((HopperNode*)m_vecChildren[0]);
			FireInputStart();
		}
	}
}

void ValuedHopperNode::FireInputStart()
{
	if (m_bIsExistOutDoneDelay) {
		FireOutputDone();
	}

	bool outputing = IsOutputting();
	if (outputing && outputing == GetHopStatus()->bOutputing ) {
		//进料开始的时候，放料没有结束
		m_bInAndOutTogetherAction = true;
		return;
	}

	m_stProcessIdx.SetComIndex(-1);
	m_bRepeatOutputStatus = false;
	if (m_pDataValueMgr->GetValueCount() > m_stProcessIdx.InStartIdx) {
		m_stProcessIdx.InStartIdx = m_pDataValueMgr->GetValueCount();
	}

	HopperNode::FireInputStart();
}

void ValuedHopperNode::FireInputDone()
{
	if (m_pDataValueMgr->GetValueCount() > m_stProcessIdx.InEndIdx) {
		m_stProcessIdx.InEndIdx = m_pDataValueMgr->GetValueCount();
	}

	HopperNode::FireInputDone();
}

void ValuedHopperNode::Measure(){
	double dValue = 0.0;
	string sRawValue = "";
	ST_MEASURE_VALUE stMeaValue;
	stMeaValue.value = 0.0;
	stMeaValue.rewdata = "";
	if ( ANA_MEASURE_SINGLE == m_enMeasureType ) {
		stMeaValue = GetMeasureValue(m_pAreaConf[0]);
		dValue = stMeaValue.value;
		sRawValue = stMeaValue.rewdata;
	}
	else if ( ANA_MEASURE_DIFF == m_enMeasureType ) {
		stMeaValue = GetMeasureValue(m_pAreaConf[0]);
		dValue = stMeaValue.value;
		sRawValue += stMeaValue.rewdata;
		stMeaValue = GetMeasureValue(m_pAreaConf[1]);
		dValue -= stMeaValue.value;
		sRawValue += (sRawValue.empty() ? "" : "-");
		sRawValue += stMeaValue.rewdata;
	}
	else {
		DebugErrLog("[Measure]ID[%d] Not Find Value&Index \n", GetId());
	}

	if (m_bRepeatOutputStatus) {
		//反复只开启放料阀门的时候，对value进行累计
		dValue += m_product.GetValue();
		sRawValue += (m_product.GetRawValue().empty() ? "" : "+");
		sRawValue += m_product.GetRawValue();
	}
    m_product.SetValue(dValue);   //保存测量值
	m_product.SetRawValue(sRawValue);

	DebugLog("[Measure]ID[%d][%d]--[%.3lf][%s] \n",GetId(), m_product.GetId(), dValue, sRawValue.c_str());
}

string ValuedHopperNode::ToString(){
    string str="[O2O] ";
    str += HopperNode::ToString();

    return str;
}
//判断是否违规
bool ValuedHopperNode::JudgeIllegal()
{
	vector<DataNode*>::iterator it;
	HopperNode* pChild;
	int	count = 0;

	//如果有一个子节点处于放料状态，则认为本节点处于进料过程
	for (it = m_vecChildren.begin(); it != m_vecChildren.end(); it++) {
		try {
			pChild = (HopperNode*)*it;
			if (400 < NODE_ID_TO_HPR_ID(pChild->GetId()) && 500 > NODE_ID_TO_HPR_ID(pChild->GetId())) {
				//温度料仓不记录到违规补料的流程
				return false;
			}
			if (pChild->GetHopStatus()->bOutputing) {
				count++;
			}
		}
		catch (...) {}
	}

	if ( (1==count && IsOutputting()) || 2 <= count) {
		m_product.SetIllMark(true);
		return true;
	}

	return false;
}

void ValuedHopperNode::AddArea(ST_AREA_CONF * st_pArea)
{
	if (st_pArea) {
		m_pAreaConf.push_back(st_pArea);
	}
}

void ValuedHopperNode::AddMeasureMethod(EN_ANA_MEASURE_TYPE meaType)
{
	m_enMeasureType = meaType;
}

int ValuedHopperNode::CalcIndex(EN_AREA_TYPE enArea, int iDelay)
{
	int iIndex;
	int iDelayTime = iDelay;
	if (AREA_INPUT_START == enArea) {
		iIndex = m_stProcessIdx.InStartIdx;
	}
	else if (AREA_INPUT_END == enArea) {
		iIndex = m_stProcessIdx.InEndIdx;
	}
	else if (AREA_OUTPUT_START == enArea) {
		iIndex = m_stProcessIdx.OutStartIdx;
	}
	else if (AREA_OUTPUT_END == enArea) {
		iIndex = m_stProcessIdx.OutEndIdx;
		//iDelayTime = (0 < iDelay) ? 0 : iDelay;
	}
	else {
		return -1;
	}

	if (0 == iDelayTime ) {
		return iIndex;
	} else 	if (iIndex > m_pDataValueMgr->GetValueCount() ) {
		return m_pDataValueMgr->GetValueCount() - 1;
	}
	else {
		iIndex = (0 < iIndex) ? (iIndex - 1) : 0;
		SydTime time = (m_pDataValueMgr->GetValue()->data() + iIndex)->time;

		time.AddMilliSecond(iDelayTime);
		iIndex = m_pDataValueMgr->FindValueAfter(time);
	}
	return iIndex;
}

bool ValuedHopperNode::IsExistOutputDoneAfterDelay()
{
	bool bDelay = false;
	int iOutPutDoneDelay = 0;
	for (vector<ST_AREA_CONF*>::iterator it = m_pAreaConf.begin(); it != m_pAreaConf.end();it++) {
		if (AREA_OUTPUT_END == (*it)->endArea && 0 < (*it)->endDelay && iOutPutDoneDelay < (*it)->endDelay) {
			iOutPutDoneDelay = (*it)->endDelay;
			bDelay = true;
		}
	}
	if (bDelay) {
		m_OutDoneDelayTime = (m_pDataValueMgr->GetValue()->data() + (m_pDataValueMgr->GetValueCount() - 1))->time;
		m_OutDoneDelayTime.AddMilliSecond(iOutPutDoneDelay);
	}

	return bDelay;
}

ST_MEASURE_VALUE ValuedHopperNode::GetMeasureValue(ST_AREA_CONF* stpAreaConf)
{
	int iStartIdx, iEndInx;
	ST_MEASURE_VALUE stmValue;
	stmValue.rewdata = "";
	stmValue.value = 0.0;

	iStartIdx = CalcIndex(stpAreaConf->startArea, stpAreaConf->startDelay);
	iStartIdx = (0 >= iStartIdx) ? 1 : iStartIdx;
	iEndInx = CalcIndex(stpAreaConf->endArea, stpAreaConf->endDelay);
	stmValue = stpAreaConf->pDataAlgorithm->Execute(m_pDataValueMgr->GetValue(), iStartIdx, iEndInx);

	return stmValue;
}
