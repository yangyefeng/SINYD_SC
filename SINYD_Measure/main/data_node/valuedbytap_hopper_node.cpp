#include <sstream>

#include "valuedbytap_hopper_node.h"
#include "../../Include/DebugLogger.h"
#include "../../Data/RWData/RWData.h"

ValuedByTapHopperNode::ValuedByTapHopperNode()
{

}

void ValuedByTapHopperNode::Initialize(DataProvider *pDataProvider){
    HopperNode::Initialize(pDataProvider);
	m_bRunning = false;
}

void ValuedByTapHopperNode::Execute(){
    HopperNode::Execute();
}

void ValuedByTapHopperNode::Release()
{
	HopperNode::Release();
}

bool ValuedByTapHopperNode::RefreshStatus(){
    return HopperNode::RefreshStatus();
}

void ValuedByTapHopperNode::FireOutputStart(){
	if (!m_bRunning && 0 == (int)GetChildren()->size()) {
		m_OutputST = m_pDataProvider->GetTimestamp();
        DebugLog("@@@@@@@@@@@@@starttime:%s \n", m_OutputST.ToString().c_str());
		m_bRunning = true;
		HopperNode::FireOutputStart();
	}
	else if (m_bRunning) {
		HopperNode::FireOutputStart();
	}	
}

void ValuedByTapHopperNode::FireOutputDone(){
	if (m_bRunning) {
		m_OutputET = m_pDataProvider->GetTimestamp();
        DebugLog("@@@@@@@@@@@@@m_EndTime:%s \n", m_OutputET.ToString().c_str());
		Measure();
		m_bRunning = false;
		HopperNode::FireOutputDone();
	}
}

void ValuedByTapHopperNode::FireInputStart()
{
	if (!m_bRunning) {
		m_InputST = m_pDataProvider->GetTimestamp();
        DebugLog("@@@@@@@@@@@@@m_Input start time:%s \n", m_InputST.ToString().c_str());
		m_bRunning = true;
		HopperNode::FireInputStart();
	}
}

void ValuedByTapHopperNode::FireInputDone()
{
	if (!m_bRunning) {
		m_InputET = m_pDataProvider->GetTimestamp();
		HopperNode::FireInputDone();
	}
}

void ValuedByTapHopperNode::Measure(){
	CalcWeightByWind();
	m_bRunning = false;
}

string ValuedByTapHopperNode::ToString(){
    string str="[O2O] ";
    str += HopperNode::ToString();

    return str;
}


void ValuedByTapHopperNode::CalcWeightByWind()
{
	double timediff = 0.0;
	if (1 == GetChildren()->size()) {
		timediff = m_OutputET - m_InputST;
	}
	else {
		timediff = m_OutputET - m_OutputST;
	}

	double wValue = timediff*m_dCommonRatio / 1000;
	DebugLog("@@wValue:[%lf],radtio[%lf] time[%lf] ---[%lf]\n", wValue, m_dCommonRatio, timediff, m_product.GetValue());
	if (m_product.IsValueReady()) {
		wValue += m_product.GetValue();
	}
	m_product.SetValue(wValue);
	char newstr[100] = { 0 };
	sprintf(newstr, "%lf", wValue);
	m_product.SetRawValue(string(newstr));
}

void ValuedByTapHopperNode::SetCommonRatio(double CommRatio)
{
	m_dCommonRatio = CommRatio;
}