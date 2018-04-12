#include <sstream>

#include "hopper_node.h"
#include "../../Include/DebugLogger.h"
#include "mixer_node.h"


#include "log.h"


HopperNode::HopperNode():DataNode()
{
    SetId(0);
    SetData(0);
	SetType(0);
    SetOpenDelay(0);
	SetCloseDelay(0);

    m_stStatus.bInputting=false;
    m_stStatus.bOutputing=false;
	m_bInAndOutOpen = false;
	m_isaddproduct = false;
    //m_stStatus.timestamp.Reset();

}

HopperNode::~HopperNode(){
    Release();
}

void HopperNode::Initialize(DataProvider *pDataProvider){
    DataNode::Initialize(pDataProvider);

    m_product.SetId(GetId());
    if( m_vecChildren.size()>0 ){
        //初始化子节点
        vector<DataNode*>::iterator it=m_vecChildren.begin();
        while(it!=m_vecChildren.end()){
            (*it)->Initialize(pDataProvider);
            it++;
        }
    }else{
        //对于没有上料阀的料仓，初始化表示开始生产
        m_product.Start();
		m_isaddproduct = true;
    }
}

/**
  释放子对象的内存空间
  */
void HopperNode::Release(){
    vector<DataNode*>::iterator it=m_vecChildren.begin();
    while(it!=m_vecChildren.end() ){
        delete (*it);
        it++;
    }

    m_vecChildren.clear();
    m_vecTaps.clear();
    m_mapTapValues.clear();    
    m_vecStatusObserver.clear();

    //执行上级类人方法
    DataNode::Release();
}
//第一次Execute
void HopperNode::Execute(){
    //LOG4CPLUS_INFO(logger,"1");
    if(m_pDataProvider==NULL || m_pDataProvider->IsDigitalAllOpen()){
        return;
    }
    if (IS_MIX_NODE(GetId()) && (m_pDataProvider->GetTimestamp() - m_stStatus.timestamp > 300 ) ) {
        //当获取的两次源数据间隔的时间超过5分钟的时候，重置搅拌机的Product的
        m_product.Reset();
	}

	if (IS_BKT_NODE(GetId()) && IsInputting()&& IsOutputting()) {
		vector<DataNode*>::iterator itChild;

		for (itChild = m_vecChildren.begin(); itChild != m_vecChildren.end(); itChild++) {
			if (((HopperNode*)(*itChild))->IsOutputting()) {
				((HopperNode*)(*itChild))->SetInAndOutOpenFlg(true);
				if (HPR_ID_TO_NODE_ID(222) == (((HopperNode*)(*itChild))->GetChildren()->at(0))->GetId() || 
					HPR_ID_TO_NODE_ID(221) == (((HopperNode*)(*itChild))->GetChildren()->at(0))->GetId()) {
					((MixerNode*)(m_vecStatusObserver[0]))->SetIllegaSpecial(true);
				}
			}
		}
    }

    vector<INT32>::iterator itTap;
    INT32 iTapId,iValue,iOldValue;

    //获取每个阀门的状态，如果发生了变化则进行保存
    for(itTap=m_vecTaps.begin(); itTap!=m_vecTaps.end(); itTap++){
        iTapId = *itTap;
        iValue = m_pDataProvider->GetDigitalData(iTapId);
        iOldValue = m_mapTapValues[iTapId];

        //判断阀门状态是否发生变化
        if( iValue!=iOldValue ){
            //保存最新状态值
            m_mapTapValues[iTapId]=iValue;//数组方式插入MAP数据
            //DebugLog("Execute::  %d[%d]: %d->%d\n",m_id,iTapId, iOldValue, iValue);
        }
    }

    //启动每个子节点进行处理  hopper_node的子结点
    vector<DataNode*>::iterator itChild;
    for(itChild=m_vecChildren.begin(); itChild!=m_vecChildren.end(); itChild++){
        (*itChild)->Execute();
    }

//    if(GetId()==80001){
//        SydTime tm=m_pDataProvider->GetTimestamp();
//        ostringstream os;
//        os<<GetName();

//        os<<"   ";
//        if(IsOutputting()){
//            os<<"=";
//        }else{
//            os<<".";
//        }
//        os<<"   ";
//        vector<DataNode*>::iterator itChild;
//        HopperNode *pChild;
//        for(itChild=m_vecChildren.begin(); itChild!=m_vecChildren.end(); itChild++){
//            pChild = (HopperNode*)(*itChild);
//            if( pChild->IsOutputting()){
//                os<<"+";
//            }else{
//                os<<".";
//            }
//        }
//        os<<endl;
//        string s=os.str();
//        DebugLogWithTm(tm,s.c_str());
//    }

    //更新状态
    RefreshStatus();  //发布主题
}

bool HopperNode::RefreshStatus(){
    bool bInputting = IsInputting();
    bool bOutputting = IsOutputting();

    //如果状态未变化，则直接返回
    if(bInputting==m_stStatus.bInputting && bOutputting==m_stStatus.bOutputing){
        return false;
    }

    //触发相应的事件处理
    if(bOutputting!=m_stStatus.bOutputing){
        if(bOutputting){
            FireOutputStart();
        }else{
            FireOutputDone();
        }
    }

    if(bInputting!=m_stStatus.bInputting){
        if(bInputting){
            FireInputStart();
        }else{
            FireInputDone();
        }
    }

    //记录当前状态
    m_stStatus.bInputting=bInputting;
    m_stStatus.bOutputing=bOutputting;
    m_stStatus.timestamp=m_pDataProvider->GetTimestamp();

    return true;
}

/**
  @brief 进料开始时，开始生产一个新产品
  */
void HopperNode::FireInputStart(){
    //对于非叶子节点，进料开始表示生产开始
    if(m_vecChildren.size()>0){
        if(!m_product.IsRunning()&& 0 != m_vecStatusObserver.size()){
            m_product.Start();
        }
		m_isaddproduct = true;
    }


    //通知观察者
    vector<HopperNode*>::iterator it;
    HopperNode *pObserver;
    for( it=m_vecStatusObserver.begin(); it!=m_vecStatusObserver.end(); it++ ){
        pObserver = *it;
        pObserver->OnInputStart(this);//执行动作
    }
}

void HopperNode::FireInputDone(){
    //通知观察者
    vector<HopperNode*>::iterator it;
    HopperNode *pObserver;
    for( it=m_vecStatusObserver.begin(); it!=m_vecStatusObserver.end(); it++ ){
        pObserver = *it;
        pObserver->OnInputDone(this);
    }
}

/**
  @brief 放料开始时，结束生产当前产品
  */
void HopperNode::FireOutputStart(){
	if (IS_BKT_NODE(GetId())) {
		//printf("[OutPutStart]m_isaddproduct = true @@@@@@@ id[%d]@@[ %d ] running[%d]@@@@@@@@@@@@@@@@@ \n", GetId(), m_product.IsFinished(), m_product.IsRunning());
		m_isaddproduct = true;
		if (!m_product.IsRunning()) {
			m_product.Start();
		}
	}
    //对于所有节点，放料开始表示生产结束
    if( m_product.IsRunning() ){
        m_product.Finish();
    }

	if (m_vecChildren.size() == 0 && !m_vecStatusObserver[0]->IsOutputting() && m_vecStatusObserver[0]->GetHopStatus()->bOutputing) {
		m_isaddproduct = true;
		//printf("[OutPutStart]m_isaddproduct = true @@@@@@@ id[%d]@@@@@@@@@@@@@@@@@@@ \n", m_vecStatusObserver[0]->GetId());
	}

	if (m_isaddproduct) {
    //通知观察者
		vector<HopperNode*>::iterator it;
		HopperNode *pObserver;
		for (it = m_vecStatusObserver.begin(); it != m_vecStatusObserver.end(); it++) {
			pObserver = *it;
			pObserver->OnOutputStart(this);
		}
	}

	if (IS_BKT_NODE(GetId())) {
		//Task16208:中储斗放料开始的时候，清空子节点（称重料仓）中生产数据;
		vector<DataNode*>::iterator itChild;
		for (itChild = m_vecChildren.begin(); itChild != m_vecChildren.end(); itChild++) {
			((HopperNode*)(*itChild))->GetProduct()->ClearValue();
		}
	}
}

void HopperNode::FireOutputDone(){
	m_product.SetEndTime();
    //通知观察者
    vector<HopperNode*>::iterator it;
    HopperNode *pObserver;
    for( it=m_vecStatusObserver.begin(); it!=m_vecStatusObserver.end(); it++ ){
        pObserver = *it;
        pObserver->OnOutputDone(this);
    }

    //对于叶子节点，阀门关闭表示生产开始
    if(m_vecChildren.size()==0){
        if(!m_product.IsRunning()){
            m_product.Start();
        }
		m_isaddproduct = true;
    }
}

void HopperNode::OnInputStart(HopperNode *pHopper){
    //do nothing
}

void HopperNode::OnInputDone(HopperNode *pHopper){
    //do nothing
}

/**
  @brief 监听子节点的生产过程，将子节点的产品增加到自身产品列表中
  */
void HopperNode::OnOutputStart(HopperNode *pHopper){
//    string s=pHopper->m_product.ToString();
//    const char* ps=s.c_str();

    if(pHopper->m_product.IsFinished()){
        if( m_product.IsNewer()){
//            DebugLog("ProductFinished %s:: %s is arriving, I'm newer, start myself.\n",m_strName.c_str(), ps);
            m_product.Start();
            m_product.AddChild(pHopper->m_product);
			pHopper->SetIsAddProduct(false);
        }else if( m_product.IsRunning()){
            m_product.AddChild(pHopper->m_product);
			pHopper->SetIsAddProduct(false);
//            DebugLog("ProductFinished %s:: %s is arriving, I'm running.\n",m_strName.c_str(),ps);
        }else{
            m_product.Start();
            m_product.AddChild(pHopper->m_product);
			pHopper->SetIsAddProduct(false);
//            DebugLog("ProductFinished %s:: %s is arriving, I'm finished, restart myself.\n",m_strName.c_str(),ps);
        }
		pHopper->m_product.SetIllMark(false);
    }else if( pHopper->GetChildren()->size()>0){
//        DebugLog("ProductFinished %s:: un-finished product is arriving, %s.\n",m_strName.c_str(),ps);
    }
}

void HopperNode::OnOutputDone(HopperNode *pHopper){
    //do nothing
	
	if (IS_BKT_NODE(GetId()) && NULL != m_vecStatusObserver[0] && pHopper->m_bInAndOutOpen) {
		pHopper->SetInAndOutOpenFlg(false);
		if (AddBKTProductValue(pHopper,true)) {
			return;
		}
	}
	vector<HopperProduct> *pChildren = m_product.GetChildren();
	HopperProduct* pStart = pChildren->data();
	INT32 iCount = pChildren->size();
	SydTime stime;
	
	for (int i = iCount - 1; i >= 0; i--) {
		stime = pHopper->GetProduct()->GetEndTime();
		if ( pHopper->GetId() == pStart[i].GetId())
		{
			pStart[i].SetEndTime(&stime);
			if (IS_ANA_NODE(pHopper->GetId()) || IS_NOANA_NODE(pHopper->GetId())) {
				string rValue = pHopper->GetProduct()->GetRawValue();
				double value = pHopper->GetProduct()->GetValue();
				pStart[i].SetRawValue(rValue);
				pStart[i].SetValue(value);
			}
			break;
		}
		
	}
}

bool HopperNode::AddBKTProductValue(HopperNode *pHopper, bool bFind)
{
	vector<HopperProduct> *pChild = m_vecStatusObserver[0]->GetProduct()->GetChildren();
	HopperProduct* pBktStart = pChild->data();
	int iTotal = pChild->size();
	SydTime stime;
	bool bRet = false;
	bool b_fBktID = false; //查找到中储斗，没有找到子节点为true
	for (int i = iTotal - 1; i >= 0; i--) {
		if (pBktStart[i].GetId() == GetId()) {
			vector<HopperProduct> *pSecondChild = pBktStart[i].GetChildren();
			HopperProduct* pAnaStart = pSecondChild->data();
			int iNum = pSecondChild->size();
			b_fBktID = true;
			for (int j = iNum - 1; j >= 0; j--)
			{
				if (pAnaStart[j].GetId() == pHopper->GetId()) {
					pAnaStart[j].SetEndTime(&stime);
					string rVal = pHopper->GetProduct()->GetRawValue();
					double val = pHopper->GetProduct()->GetValue();

					pAnaStart[j].SetRawValue(rVal);
					pAnaStart[j].SetValue(val);
					bRet = true;
					return bRet;
				}
			}
			if ( b_fBktID && bFind) { 
				//查找到中储斗，没有找到子节点时候，将现有的中储斗节点添加到统计数据队列上
				for (int i = 0; i < m_product.GetChildren()->size();i++) {
					if ((m_product.GetChildren())->at(i).GetId() == pHopper->GetId()) {
						//查找到当前中储斗生产树形（m_product）子节点中如果包含了对应要添加的子节点（pHopper->GetId()），将现有的中储斗节点添加到统计数据队列上
						//printf("[%d] end=============\n", pHopper->GetId());
						(m_vecStatusObserver[0]->GetProduct())->RemoveChildren(&m_product);
						(m_vecStatusObserver[0]->GetProduct())->AddChild(m_product);
						return AddBKTProductValue(pHopper);
					}
				}
			}
		}
	}
	return bRet;
}

bool HopperNode::IsOutputting(){
    vector<INT32>::iterator it;

    //如果有一个阀门处于开的状态，则认为当前处于放料状态
    for(it=m_vecTaps.begin(); it!=m_vecTaps.end(); it++){
        if(m_mapTapValues[*it]>0){
            return true;
        }
    }

    return false;
}

bool HopperNode::IsInputting(){
    vector<DataNode*>::iterator it;
    HopperNode* pChild;

    //如果有一个子节点处于放料状态，则认为本节点处于进料过程
    for(it=m_vecChildren.begin(); it!=m_vecChildren.end(); it++){
        try{
            pChild = (HopperNode*) *it;
            if(pChild->IsOutputting()){
                return true;
            }
        }catch(...){}
    }

    return false;
}

void HopperNode::AddStatusObserver(HopperNode *pHopper){
    m_vecStatusObserver.push_back(pHopper);
}

void HopperNode::RemoveStatusObserver(HopperNode *pHopper){
    vector<HopperNode*>::iterator it;

    for( it=m_vecStatusObserver.begin(); it!=m_vecStatusObserver.end(); it++ ){
        if( *it == pHopper){
            m_vecStatusObserver.erase(it);
            break;
        }
    }
}

vector<DataNode*>* HopperNode::GetChildren() {
    return &m_vecChildren;
}

vector<INT32>* HopperNode::GetTaps(){
    return &m_vecTaps;
}
//填加观察者
void HopperNode::AddChild(HopperNode *pNode){
    m_vecChildren.push_back(pNode);

    //监听子节点的状态变化
    pNode->AddStatusObserver(this);
}

HopperNode* HopperNode::FindChild(int id){
    HopperNode *pNode;
    vector<DataNode*>::iterator it;

    for(it=m_vecChildren.begin(); it!=m_vecChildren.end(); it++){
        pNode = (HopperNode*)*it;
        if( pNode->GetId()==id){
            return pNode;
        }
    }

    return NULL;
}

void HopperNode::AddTap(int iTapId){
    vector<INT32>::iterator it;

    if(iTapId>0){
        for(it=m_vecTaps.begin();it!=m_vecTaps.end(); it++)
        {
            if(*it==iTapId){
                DebugLog("HopperNode::AddTap - Try to add duplicated tap %d for hopper %d\n", iTapId, GetId());
                return;
            }
        }
        m_vecTaps.push_back(iTapId);
        m_mapTapValues[iTapId] = -1;
    }else if (iTapId<0){
        DebugErrLog("HopperNode::AddTap - Try to add invalid tap %d for hopper %d\n", iTapId, GetId());
    }
}

INT32 HopperNode::GetId(){
    return m_iId;
}

INT32 HopperNode::GetData(){
    return m_iData;
}

string HopperNode::GetName(){
    return m_strName;
}

int HopperNode::GetType()
{
	return m_itype;
}
void HopperNode::SetId(INT32 iNewId){
    m_iId=iNewId;
}

void HopperNode::SetData(INT32 iNewData){
    m_iData=iNewData;
}

void HopperNode::SetType(int type)
{
	m_itype = type;
	m_product.SetTyep(type);
}

void HopperNode::SetName(const string& strNewName){
    m_strName=strNewName;
    m_product.SetName(strNewName);
}

void HopperNode::SetIsAddProduct(bool addp) {
	m_isaddproduct = addp;
}
//设置延时
void HopperNode::SetOpenDelay(int delay)
{
    m_iOpenDelay = delay;
}

int HopperNode::GetOpenDelay()
{
    return m_iOpenDelay;
}

void HopperNode::SetCloseDelay(int delay)
{
    m_iCloseDelay = delay;
}

int HopperNode::GetCloseDelay()
{
    return m_iCloseDelay;
}

HopperProduct* HopperNode::GetProduct(){
    return &m_product;
}

string HopperNode::ToString(){
    ostringstream os;

    os<<m_iId<<", "<<m_strName;

    if(GetData()!=0){
        os<<", data("<<GetData()<<")";
    }

	if (GetType() != 0) {
		os << ", type(" << GetType() << ")";
	}

    if (GetOpenDelay() > 0) {
        os << ", OpenDelay(" << GetOpenDelay() << ")";
    }

    if (GetCloseDelay() > 0) {
        os << ", CloseDelay(" << GetCloseDelay() << ")";
    }

    if(m_vecTaps.size()>0){
        vector<INT32>::iterator it=m_vecTaps.begin();
        os<<", taps(";
        while(it!=m_vecTaps.end()){
            if(it!=m_vecTaps.begin()){
                os<<",";
            }
            os<< *it;
            it++;
        }
        os<<")";
    }

    string str=os.str();

    return str;
}

void HopperNode::DumpTree(INT32 iLevel){
    ostringstream os;

    //打印自身的内容
    os<<"# ";
    for(INT32 i=0; i<iLevel; i++){
        os<<"    ";
    }
    if(m_vecChildren.size()>0){
        os<<"* ";
    }else{
        os<<"- ";
    }
    os<<ToString()<<endl<<ends;

    string str=os.str();
    const char *szLog=str.c_str();
    DebugLog(szLog);

    //打印子节点的内容
    vector<DataNode*>::iterator it;
    HopperNode *pNode;
    for(it=m_vecChildren.begin(); it!=m_vecChildren.end(); it++){
        pNode=(HopperNode*)*it;
        pNode->DumpTree(iLevel+1);
    }
}

HopperNode::ST_HOPPER_STATUS* HopperNode::GetHopStatus()
{
	return &m_stStatus;
}

void HopperNode::InitInAndOutOpenFlg()
{
	m_bInAndOutOpen = false;
}

void HopperNode::SetInAndOutOpenFlg(bool bFlg)
{
	m_bInAndOutOpen = bFlg;
}
