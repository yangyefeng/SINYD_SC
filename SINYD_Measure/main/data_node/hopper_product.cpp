#include <sstream>
using namespace std;

#include "hopper_product.h"
#include "../../Include/DebugLogger.h"

HopperProduct::HopperProduct()
{
    m_iId=0;
    Reset();
}

HopperProduct::~HopperProduct()
{

}

void HopperProduct::Reset(){
    m_bRunning=false;
    m_bFinished=false;

    SydTime now;
    m_tmStart=now;
    m_tmFinish=now;

    m_iValue=0;
	m_sRawValue = "";
    m_bValueIsReady=false;
	m_billmark = false;

    m_vecChildren.clear();
}

void HopperProduct::SetStartTime(const SydTime* pTime){
    if(pTime){
        m_tmStart=*pTime;
    }else{
        m_tmStart.Now();
    }
}

SydTime HopperProduct::GetStartTime(){
    return m_tmStart;
}

void HopperProduct::SetFinishTime(const SydTime* pTime){
    if(pTime){
        m_tmFinish=*pTime;
    }else{
        m_tmFinish.Now();
    }
}

SydTime HopperProduct::GetFinishTime(){
    return m_tmFinish;
}

float HopperProduct::GetDuration(){
    return m_tmFinish - m_tmStart;
}

void HopperProduct::Start(){
    if(!m_bRunning){
        m_bRunning=true;
        m_bFinished=false;
        m_vecChildren.clear();
		SetIllMark(false);
        SetStartTime();

        ClearValue();
		//m_sRawValue = "";
    }
}

void HopperProduct::Finish(){
    if(m_bRunning){
        m_bRunning=false;
        m_bFinished=true;
        SetFinishTime();
    }
}

bool HopperProduct::IsNewer(){
    return (!m_bRunning) && (!m_bFinished);
}

bool HopperProduct::IsRunning(){
    return m_bRunning;
}

bool HopperProduct::IsFinished(){
    return m_bFinished;
}

void HopperProduct::SetId(int iId){
    m_iId = iId;
}

int HopperProduct::GetId() const{
    return m_iId;
}

void HopperProduct::SetName(const string &strName){
    m_strName = strName;
}

string HopperProduct::GetName()const {
    return m_strName;
}

void HopperProduct::SetTyep(int itype) {
	m_itype = itype;
}

int HopperProduct::GetType() const {
	return m_itype;
}

void HopperProduct::SetIllMark(bool illmark) {
	m_billmark = illmark;
}

bool HopperProduct::GetIllMark() const {
	return m_billmark;
}

void HopperProduct::SetValue(double value){
    m_iValue = value;
    m_bValueIsReady = true;
}

void HopperProduct::SetRawValue(string rawValue)
{
	m_sRawValue = rawValue;
}

void HopperProduct::ClearValue(){
    m_bValueIsReady = false;
}

bool HopperProduct::IsValueReady() const{
    return m_bValueIsReady;
}

double HopperProduct::GetValue() const{
    return m_iValue;
}

string HopperProduct::GetRawValue() const {
	return m_sRawValue;
}

void HopperProduct::AddChild(const HopperProduct& product){
    m_vecChildren.push_back(product);
}

vector<HopperProduct> * HopperProduct:: GetChildren()  {
    return &m_vecChildren;
}

void HopperProduct::RemoveChildren(HopperProduct *product) {
	vector<HopperProduct>::iterator it;

	for (it = m_vecChildren.begin(); it != m_vecChildren.end(); it++) {
		if ((*it).GetId() == product->GetId()) {
			m_vecChildren.erase(it);
			break;
		}
	}
}

SydTime HopperProduct::GetEndTime() {
	return m_tmEnd;
}

void HopperProduct::SetEndTime(const SydTime* pTime) {
	if (pTime) {
		m_tmEnd = *pTime;
	}
	else {
		m_tmEnd.Now();
	}
}

string HopperProduct::ToString(){
    ostringstream os;

    os<<"product "<<m_strName<<"["<<m_iId<<"]";

    if(IsNewer()){
        os<<", newer";
    }else if(IsRunning()){
        os<<", running, start at "<<m_tmStart.ToShortString();
    }else if(IsFinished()){
        os<<", from " << m_tmStart.ToShortString();
        os<<", to " << m_tmFinish.ToShortString();
		os << ", end " << m_tmEnd.ToShortString();
        os<<", duration="<< (int)GetDuration() <<"(s)";
    }
    if( IsValueReady() ){
        os<<",values="<<m_iValue;
    }

    string s=os.str();
    return s;
}

void HopperProduct::DumpTree(INT32 iLevel){
    ostringstream os;

    for(int i=0; i<iLevel; i++){
        os<<"    ";
    }

    os<<ToString();

    string str=os.str();
    DebugLog("%s\n",str.c_str());

    //子产品的信息
    vector<HopperProduct>::iterator it=m_vecChildren.begin();
    HopperProduct* pChild;
    while(it!=m_vecChildren.end()){
        pChild = &(*it);
        if(pChild->m_vecChildren.size()>0  || pChild->IsValueReady()){
            pChild->DumpTree(iLevel+1);
        }
        it++;
    }
}

void HopperProduct::DumpJson(ofstream &ofTarget,  int iProductId ){
    if(!ofTarget.is_open()){
        DebugErrLog("target product file isn't open.........\n");
    }else{
        ostringstream os;
        os<<"\"product_"<<iProductId<<"\":{"<<endl;
        DumpJsonNode(os,1);
        os<<"};"<<endl;

        ofTarget<<os.str();
    }
}


void HopperProduct::DumpJsonNode(ostringstream &os , INT32 iLevel){
    string strIndent;

    for(INT32 i=0; i<iLevel;i++){
        strIndent +="    ";
    }

    os<<strIndent<<"\"hopperId\":"<<GetId()<<","<<endl;
    os<<strIndent<<"\"name\":\""<<m_strName<<"\","<<endl;
    if(IsValueReady()){
        os<<strIndent<<"\"value\":"<<m_iValue<<endl;
    }
    if( IsNewer() ){
        os<<strIndent<<"\"status\":\"newer\"";
    }else if( IsRunning() ){
        os<<strIndent<<"\"status\":\"running\","<<endl;
        os<<strIndent<<"\"startTime\":\""<<m_tmStart.ToString()<<"\"";
    }else{
        os<<strIndent<<"\"status\":'finished',"<<endl;
        os<<strIndent<<"\"startTime\":\""<<m_tmStart.ToString()<<"\","<<endl;
        os<<strIndent<<"\"finishTime\":\""<<m_tmFinish.ToString()<<"\"";
    }

    if(m_vecChildren.size()>0){
        os<<","<<endl;

        //输出子产品的信息
        os<<strIndent<<"\"children\":[{"<<endl;

        vector<HopperProduct>::iterator it=m_vecChildren.begin();
        HopperProduct* pChild;
        while(it!=m_vecChildren.end()){
            pChild = &(*it);

            if(it!=m_vecChildren.begin()){
                os<<strIndent<<"},{"<<endl;
            }
            pChild->DumpJsonNode(os,iLevel+1);

            it++;
        }
        os<<strIndent<<"}]"<<endl;
    }else{
        os<<endl;
    }
}
