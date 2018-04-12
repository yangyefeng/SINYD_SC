//#include "sstream"

#include "mixer_node.h"
#include "../../Include/DebugLogger.h"
#include "../../Data/RWData/RWData.h"
#include "../../Data/Include/Operation_conf.h"
#include "MeasureDef.h"
#include "../../Data/RWData/RWData.h"
#include <sstream>
#include <stdlib.h>

MixerNode::MixerNode()
{
	m_bspecial = false;
	m_billegalSpecial = false;
}

MixerNode::~MixerNode()
{
	Release();
}

void MixerNode::Initialize(DataProvider *pDataProvider){
    HopperNode::Initialize( pDataProvider);

	m_pdbo = NULL;
	m_pdetaildbo = NULL;
    m_iProductId=GetProductID();

    PrepareProductStream();
    PrepareProductDetailStream();
}

void MixerNode::Release(){
    //关闭已经打开的文件
    m_rfRaw.SafeClose();
    m_rfProduct.SafeClose();
    m_rfDetail.SafeClose();
	ReleaseDBOperator(m_pdbo);
	ReleaseDBOperator(m_pdetaildbo);
    //执行父类方法
    HopperNode::Release();
}


void MixerNode::FireOutputStart(){
	if (!m_product.IsRunning()) {
		return;
	}

	HopperNode::FireOutputStart();

    //搅拌时长小于2秒，无效搅拌锅数据
    if ( 2 > m_product.GetDuration() || m_pDataProvider->IsDigitalAllOpen())
    {
        DebugLog("[MixerNode::FireOutputStart]Duration[%.2f][%d]\n",m_product.GetDuration(),m_pDataProvider->IsDigitalAllOpen());
        return;
    }

    m_iProductId++;

	if (SaveProduct()) {
		//SaveProductDetail();
		CollectDetaiInfo(GetProduct());

		DebugLog("MIXER product is ready , number= %d -------------------------------\n", m_iProductId);
		m_product.DumpTree();
    } else {
        m_iProductId--;
    }
}

/*
bool MixerNode::PrepareRawStream(){
    bool bRet =  m_rfRaw.CreateFile("raw");
    if(  bRet ){
        //首先写入文件头
        m_rfRaw<<"timestamp,package_id";
        WriteRawHeader(this);
        m_rfRaw<<endl;
    }

    return bRet;
}


bool MixerNode::WriteRawHeader(HopperNode *pNode){
    //输出当前节点的阀门数据
    INT32 *pTaps=pNode->GetTaps()->data();
    INT32 iTapCount=pNode->GetTaps()->size();
    INT32 iTapId;
    for(INT32 i=0; i<iTapCount; i++){
        iTapId = *(pTaps+i);
        m_rfRaw<<",d"<<iTapId;
    }

    //如果当前节点是模拟量节点，则输出该ID
    if(IS_ANA_NODE(pNode->GetId()) ){
        m_rfRaw<<",a"<<NODE_ID_TO_ANA_ID( pNode->GetId() );
    }

    //输出下级节点的数据
    vector<DataNode*> *pChildren = pNode->GetChildren();
    INT32 nChildrenCount=pChildren->size();
    for(INT32 i=0; i<nChildrenCount; i++){
        WriteRawHeader((HopperNode*)(pChildren->at(i)));
    }

    return true;
}


bool MixerNode::SaveRawData(HopperNode *pNode){
    DataProvider* pProvider=pNode->GetDataProvider();

    //ROOT节点，首先输出时间戳和package id
    if(pNode==this){
        string strTime = pProvider->GetTimestamp().ToString();
        INT32 iPackageId = pProvider->GetPackageId();
        m_rfRaw<< strTime;
        m_rfRaw<<","<<iPackageId;
    }

    //输出当前节点的阀门数据
    INT32 *pTaps=pNode->GetTaps()->data();
    INT32 iTapCount=pNode->GetTaps()->size();
    INT32 iTapId,iValue;
    for(INT32 i=0; i<iTapCount; i++){
        iTapId = *(pTaps+i);
        iValue = pProvider->GetDigitalData(iTapId);
        m_rfRaw<<","<<iValue;
    }

    //输出当前节点的阀门模拟量数据
    if( IS_ANA_NODE(pNode->GetId()) ){
        iValue=pProvider->GetAnalogData(pNode->GetId());
        m_rfRaw<<","<<iValue;
    }

    //输出下级节点的数据
    vector<DataNode*> *pChildren = pNode->GetChildren();
    INT32 nChildrenCount=pChildren->size();
    for(INT32 i=0; i<nChildrenCount; i++){
        SaveRawData((HopperNode*)(pChildren->at(i)));
    }

    //ROOT节点，输出还行符号
    if(pNode==this){
        m_rfRaw<< endl;
    }

    return true;
}
*/

void MixerNode::SetFieldInfo(string fieldname, string orgname, FieldDataType dtype, FieldInfo *outinfo)
{
	outinfo->fieldName = fieldname;
	outinfo->fieldOriginalName = orgname;
	outinfo->dataType = dtype;
}


bool MixerNode::PrepareProductStream(){
    bool bRet = m_rfProduct.CreateFile("product");

    if( bRet ){
        //首先写入文件头
        m_rfProduct<<"timestamp,product_id,duration,Illegal";
		
        WriteProductHeader(this);
		
		m_rfProduct << endl;
		m_rfProduct.flush();

    }

	//创建生产数据库
	CreateProductDB();

    return bRet;
}

bool MixerNode::WriteProductHeader(HopperNode *pNode){
    //如果当前节点的产品需要输出（ID>0），则输出当前产品名称
    HopperProduct *pProduct=pNode->GetProduct();

    if( IS_HPR_NODE(pProduct->GetId()) ){
        m_rfProduct<<","<< pProduct->GetName();
        m_vecProductedHoppers.push_back(pProduct->GetId());
	}
	else if (IS_NOANA_NODE(pProduct->GetId()) && 0 >= pProduct->GetChildren()->size() ) {
		m_rfProduct << "," << pProduct->GetName();
		m_vecProductedHoppers.push_back(HPR_ID_TO_NODE_ID(NODE_ID_TO_NOANA_ID(pProduct->GetId())));
	}

    //输出下级节点的信息
    vector<DataNode*> *pChildren = pNode->GetChildren();
    INT32 nChildrenCount=pChildren->size();
    for(INT32 i=0; i<nChildrenCount; i++){
        WriteProductHeader((HopperNode*)(pChildren->at(i)));
    }

    return true;
}
bool MixerNode::SaveProduct(){
    SydTime now;
    m_rfProduct<<now.ToString()<<","<<m_iProductId;

    map<INT32,double> mapValues;
    INT32 iHopperId;

    //CollectProductValue(GetProduct(),mapValues);

	//向DB写入数据
	map<string, string> mapProduct;
	bool is_illegal = IsIllegal(GetProduct());
	mapProduct[STR_ILLEGAL] = GetString(is_illegal ? 1:0);
	mapProduct[STR_MIX_ID] = GetString(m_iProductId);
	mapProduct[STR_RECORD_TIME] = now.ToString();
    mapProduct[STR_RECORD_CURRENT] = GetString(10.234);
	if (MACHINE_ASPHALT == m_iMachineType) {
		double drytime = GetDryTime(GetProduct());
		mapProduct[STR_DRY_MIX_TIME] = GetString(drytime);
		mapProduct[STR_WET_MIX_TIME] = GetString(GetProduct()->GetDuration() - drytime);
	}
	else {
		mapProduct[STR_MIX_TIME] = GetString(GetProduct()->GetDuration());
	}


    map<string, ST_PRODUCT_DATA> mapDBVal;
    CollectProductDBValue(GetProduct(),mapDBVal);
    if (IsRinsePot(mapDBVal) || !IsValidDBVal(mapDBVal)) {
        //涮锅或者没有统计出盘数据的不记录到盘信息中
		return false;
	}

	if (m_bspecial) {
		SpecialValueHandle(mapDBVal);
	}
    map<string, ST_PRODUCT_DATA>::iterator it;
	int hopid;
    for(it=mapDBVal.begin();it!=mapDBVal.end();++it) {
		hopid = atoi((it->first).substr(0, 3).c_str());
		if (NULL != strstr( (it->second).rawData.c_str(), "[") || (it->second).rawData.empty()) {
			DebugErrLog("Raw[%d]%s----%s,decimal[%d]\n", hopid, (it->first).c_str(), it->second.rawData.c_str(), m_pDataProvider->GetAnaDecimal(hopid));
			mapProduct[it->first] = (it->second).rawData;
		}
		else {
			DebugErrLog("[%d]%s----%.3lf,decimal[%d]\n", hopid, (it->first).c_str(), it->second.value, m_pDataProvider->GetAnaDecimal(hopid));
			mapProduct[it->first] = GetString( (it->second).value / m_pDataProvider->GetAnaDecimal(hopid));
		}
        mapValues[HPR_ID_TO_NODE_ID(hopid)] = (it->second).value;
    }
//	for (INT32 i = 0; i < m_vecProductedHoppers.size(); i++) {
//		iHopperId = m_vecProductedHoppers[i];
//		if (mapValues.find(iHopperId) != mapValues.end()) {
//			mapProduct[GetString(NODE_ID_TO_HPR_ID(iHopperId))] = mapValues[iHopperId];
//		}
//	}


	try {
		m_pdbo->Insert(mapProduct);
	}
	catch (CDBModuleException &ex) {
        CRWData rw;
        char ch[1024] = {0};
        string pah = CONFIG_LOG_PATH;
        pah += ERROR_LOG_FILE;
        sprintf(ch,"Product Insert Error[%d][%s]!\n", ex.errorCode(), ex.errorCodeAsString(ex.errorCode()));
        rw.RepeatLog(ch, (char*)pah.c_str());
		DebugErrLog("Product Insert Error[%d][%s]!\n", ex.errorCode(), ex.errorCodeAsString(ex.errorCode()));
		return false;
	}
	SetProductID(m_iProductId);


	//向File写入数据
    m_rfProduct<<","<<GetProduct()->GetDuration()<<","<< is_illegal ? 1:0;
    for(INT32 i=0; i<m_vecProductedHoppers.size(); i++){
        iHopperId = m_vecProductedHoppers[i];
        m_rfProduct <<",";
        if( mapValues.find(iHopperId)!=mapValues.end() ){
			//DebugErrLog("file:%d----%d\n", iHopperId, mapValues[iHopperId]);
            m_rfProduct <<mapValues[iHopperId]/ m_pDataProvider->GetAnaDecimal(NODE_ID_TO_HPR_ID(iHopperId));
        }
    }
    m_rfProduct<<endl;

    m_rfProduct.flush();

    return true;
}

void MixerNode::CollectProductDBValue(HopperProduct* pProduct, map<string, ST_PRODUCT_DATA> &mapDBVal)
{
    string strid;
    //DebugWarningLog("[CollectProductDBValue]id:%d--[%s],value[%d]\n",pProduct->GetId(),pProduct->IsValueReady()?"true":"false",pProduct->GetValue());
    vector<HopperProduct> *pChildren=pProduct->GetChildren();
    HopperProduct* pStart = pChildren->data();
    INT32 iCount = pChildren->size();
	//DebugWarningLog("[CollectProductDBValue]id:%d[count:%d]--[%s],value[%.3lf] illegal[%d]\n", pProduct->GetId(), iCount,pProduct->IsValueReady() ? "true" : "false", pProduct->GetValue(), pProduct->GetIllMark());
    if( pProduct->IsValueReady()){
        if ( 1 == iCount && !IS_BKT_NODE(pProduct->GetId())) {
            strid = GetString(NODE_ID_TO_HPR_ID(pStart->GetId()));
        } else if ( IS_HPR_NODE(pProduct->GetId())) {
            strid = GetString(NODE_ID_TO_HPR_ID(pProduct->GetId()));
		}
		else if (2 <= iCount ){
			//DebugLog("count:[%d][%d]====[%d]\n", iCount, pStart->GetId(), (pStart + 1)->GetId());
			bool b_is = true;
			for (INT32 i = 0; i < iCount; i++) {
				//DebugLog("[%d]====[%d]\n", pStart->GetId(), (pStart + i)->GetId());
				if (pStart->GetId() != (pStart + i)->GetId()) {
					b_is = false;
					break;
				}
			}
			if (b_is) {
				strid = GetString(NODE_ID_TO_HPR_ID(pStart->GetId()));
			}
		}
		else if (IS_NOANA_NODE(pProduct->GetId())) {
			strid = GetString(NODE_ID_TO_NOANA_ID(pProduct->GetId()));
		}
		else {
			strid = "";
		}

        if ( "" != strid ) {
            if (CALC_VALUE_TPYE == pProduct->GetType()) {
                strid += CALC_SUFFIX;
            } else if ( DEAL_VALUE_TPYE == pProduct->GetType() ) {
                strid += DESIGN_SUFFIX;
            } else {
                strid += REAL_SUFFIX;
            }
			if (REAL_WEIGHT_TPYE == pProduct->GetType()) {
				//mapDBVal[strid] += pProduct->GetValue();
				mapDBVal[strid].value += pProduct->GetValue();
				mapDBVal[strid].rawData += pProduct->GetRawValue();
				ClearProductValue(this, pProduct->GetId());
			}
			else {
				mapDBVal[strid].value = pProduct->GetValue();
				mapDBVal[strid].rawData = pProduct->GetRawValue();
				ClearProductValue(this, pProduct->GetId());
			}
        }
    }

    for(INT32 i=0; i<iCount; i++){
        CollectProductDBValue(pStart+i,mapDBVal);
    }
}

void MixerNode::CollectProductValue(HopperProduct* pProduct, map<INT32,double> &mapValues){
    vector<HopperProduct> *pChildren=pProduct->GetChildren();
    HopperProduct* pStart = pChildren->data();
    INT32 iCount = pChildren->size();
    if( pProduct->IsValueReady()){
        if ( 1 == iCount ) {
			//DebugErrLog("Getfile[%d][%d]:%d----%d\n", pProduct->GetType(), pStart->GetType(),pStart->GetId(), pProduct->GetValue());
            mapValues[pStart->GetId()]=pProduct->GetValue();
        } else if ( IS_HPR_NODE(pProduct->GetId())) {
			//DebugErrLog("GetChildfile:%d----%d\n", pProduct->GetId(), pProduct->GetValue());
            mapValues[pProduct->GetId()]=pProduct->GetValue();
		}
    }

//    vector<HopperProduct> *pChildren=pProduct->GetChildren();
//    HopperProduct* pStart = pChildren->data();
//    INT32 iCount = pChildren->size();

    for(INT32 i=0; i<iCount; i++){
        CollectProductValue(pStart+i,mapValues);
    }
}

bool MixerNode::PrepareProductDetailStream(){
    bool bRet = true;//m_rfDetail.CreateFile("detail");
	DBInfo tinfo;
	tinfo.DBNamePrefix = STR_DETAIL_BD_NAME;

	FieldInfo cinfo;
	SetFieldInfo(STR_MIX_ID, "盘号", DATATYPE_INTEGER, &cinfo);
	tinfo.fields.push_back(cinfo);
	SetFieldInfo(STR_REAL_SENSOR_ID, "秤信号量", DATATYPE_INTEGER, &cinfo);
	tinfo.fields.push_back(cinfo);
    SetFieldInfo(STR_START_TIME, "开始时间", DATATYPE_REAL, &cinfo);
	tinfo.fields.push_back(cinfo);
    SetFieldInfo(STR_END_TIME, "结束时间", DATATYPE_REAL, &cinfo);
	tinfo.fields.push_back(cinfo);

    tinfo.configInfo.ConfigTimeStamp = createtime;
    tinfo.configInfo.DBConfigPath = dirpath;
	m_pdetaildbo = CreateDBOperator(tinfo);
    return bRet;
}

bool MixerNode::SaveProductDetail(){
    m_product.DumpJson(m_rfDetail,m_iProductId);
    m_rfDetail.flush();

    return true;
}

void MixerNode::CollectDetaiInfo(HopperProduct* pProduct)
{
    map<string, string> mapDetail;
    if(IS_ANA_NODE(pProduct->GetId())){
        mapDetail[STR_MIX_ID] = GetString(m_iProductId);
        mapDetail[STR_REAL_SENSOR_ID] = GetString(NODE_ID_TO_ANA_ID(pProduct->GetId()));
        mapDetail[STR_START_TIME] = pProduct->GetStartTime().ToString();
        mapDetail[STR_END_TIME] = pProduct->GetEndTime().ToString();

		try {
			m_pdetaildbo->Insert(mapDetail);
		}
		catch (CDBModuleException &ex) {
			DebugErrLog("Detail Insert Error[%d][%s]!\n", ex.errorCode(), ex.errorCodeAsString(ex.errorCode()));
		}

    }

    vector<HopperProduct> *pChildren=pProduct->GetChildren();
    HopperProduct* pStart = pChildren->data();
    INT32 iCount = pChildren->size();

    for(INT32 i=0; i<iCount; i++){
        CollectDetaiInfo(pStart+i);
    }
}

string MixerNode::GetString(const int n)
{
	stringstream newstr;
	newstr << n;
	return newstr.str();
}

string MixerNode::GetString(const double f)
{
    char newstr[100] = {0};
    sprintf(newstr,"%lf",f);
    return string(newstr);
//    stringstream newstr;
//    newstr << f;
//    return newstr.str();

}

void MixerNode::CreateProductDB()
{
	DBInfo tinfo;
	tinfo.DBNamePrefix = STR_PRODUCT_BD_NAME;

	FieldInfo cinfo;
	SetFieldInfo(STR_ILLEGAL, "违规标识", DATATYPE_INTEGER, &cinfo);
	tinfo.fields.push_back(cinfo);
	SetFieldInfo(STR_MIX_ID, "盘号", DATATYPE_INTEGER, &cinfo);
	tinfo.fields.push_back(cinfo);
	SetFieldInfo(STR_RECORD_TIME, "记录时间", DATATYPE_REAL, &cinfo);
	tinfo.fields.push_back(cinfo);

	CRWData rw;
    //string strSysConfig = string(CONFIG_BASE_PATH) + string(CONFIG_SYS_FILE);
    string strSysConfig = dirpath + string(CONFIG_SYS_FILE);
	m_iMachineType = (EN_MIX_MACHINE_TYPE)rw.GetProfileInt("mix_machine_1", "mix_machine_type", 0, strSysConfig.c_str());
	if (MACHINE_ASPHALT == m_iMachineType) {
		SetFieldInfo(STR_DRY_MIX_TIME, "干拌时长", DATATYPE_INTEGER, &cinfo);
		tinfo.fields.push_back(cinfo);

		SetFieldInfo(STR_WET_MIX_TIME, "湿拌时长", DATATYPE_INTEGER, &cinfo);
		tinfo.fields.push_back(cinfo);
	}
	else {
		SetFieldInfo(STR_MIX_TIME, "拌和时长", DATATYPE_INTEGER, &cinfo);
		tinfo.fields.push_back(cinfo);
	}
	PrepareProductInfoDB(this,tinfo);

    tinfo.configInfo.ConfigTimeStamp = createtime;
    tinfo.configInfo.DBConfigPath = dirpath;

    try{
        m_pdbo = CreateDBOperator(tinfo);
    } catch(CDBModuleException &ex){
        DebugErrLog("product Create Error[%d][%s]!\n", ex.errorCode(), ex.errorCodeAsString(ex.errorCode()));
    }
}

void MixerNode::PrepareProductInfoDB(HopperNode*pNode, DBInfo &tinfo)
{
	//如果当前节点的产品需要输出（ID>0），则输出当前产品名称
	HopperProduct *pProduct = pNode->GetProduct();
	FieldInfo cinfo;

	if (IS_HPR_NODE(pProduct->GetId()) ){
		string fieldname = GetString(NODE_ID_TO_HPR_ID(pProduct->GetId()));
        //DebugErrLog("id:%d ;type:%d \n", NODE_ID_TO_HPR_ID(pProduct->GetId()),pProduct->GetType());
        if (CALC_VALUE_TPYE == pProduct->GetType()) {
            fieldname += CALC_SUFFIX;
        } else if ( DEAL_VALUE_TPYE == pProduct->GetType() ) {
            fieldname += DESIGN_SUFFIX;
        }
		else {
			fieldname += REAL_SUFFIX;
		}
		SetFieldInfo(fieldname, pProduct->GetName(), DATATYPE_REAL, &cinfo);
		tinfo.fields.push_back(cinfo);
	}
	else if (IS_NOANA_NODE(pProduct->GetId()) && 0 >= pNode->GetChildren()->size() ) {
		string fieldname = GetString(NODE_ID_TO_NOANA_ID(pProduct->GetId()));
		DebugLog("==id:%d ;type:%d \n", pProduct->GetId(),pProduct->GetType());
		fieldname += REAL_SUFFIX;
		SetFieldInfo(fieldname, pProduct->GetName(), DATATYPE_REAL, &cinfo);
		tinfo.fields.push_back(cinfo);
	}

	//查找下级节点的信息
	vector<DataNode*> *pChildren = pNode->GetChildren();
	INT32 nChildrenCount = pChildren->size();
	for (INT32 i = 0; i < nChildrenCount; i++) {
		PrepareProductInfoDB((HopperNode*)(pChildren->at(i)), tinfo);
	}

}


int MixerNode::GetProductID()
{
    CRWData rw;
    string path = CONFIG_RUNTIME_PATH;
    path += "Product.ini";
    int id = rw.GetProfileInt("product","mixid",-1,path.c_str());
    return (id < 0)?0:id;
}

void MixerNode::SetProductID(int iproductid)
{
    CRWData rw;
    string path = CONFIG_RUNTIME_PATH;
    path += "Product.ini";
	printf("%s\n", path.c_str());
	fstream _file;
	_file.open(path.c_str(), ios::in);
	if (_file){
		string pid = GetString(iproductid);
		rw.WriteProfileString("product", "mixid", (char *)pid.c_str(), (char *)path.c_str());
	}
	else {
		string context = "[product]\nmixid=" + GetString(iproductid);
		rw.CreateFile(path.c_str(), context.c_str());
	}
	_file.close();
}

double MixerNode::GetDryTime(HopperProduct* pProduct)
{
	vector<HopperProduct> *pChildren = pProduct->GetChildren();
	HopperProduct* pStart = pChildren->data();
	INT32 iCount = pChildren->size();
	SydTime stime;
	bool b_find = false;
	for (int i = 0; i < iCount; i++) {
		//printf("anaid:[%d],Hopid[%d] [%s] \n", (pStart+i)->GetId(), NODE_ID_TO_HPR_ID(((((pStart + i)->GetChildren())->data())->GetId())), (pStart + i)->IsValueReady()?"true":"false");
		HopperProduct * pCur = (pStart + i)->GetChildren()->data();
		if (!pCur) {
			continue;
		}
		if ( 232 == NODE_ID_TO_HPR_ID(pCur->GetId())) {			
			stime = (pStart + i)->GetFinishTime();
			b_find = true;
			break;
		}
	}
	//printf("starttime:%s\n", stime.ToString().c_str());
	if (b_find)
	{
		return stime - m_product.GetStartTime();
	}

	return 0.0;
}

bool MixerNode::IsIllegal(HopperProduct* pProduct)
{
	//bool b_ret = false;
	if (pProduct->GetIllMark()) {
		return true;
	}
	vector<HopperProduct> *pChildren = pProduct->GetChildren();
	HopperProduct* pStart = pChildren->data();
	INT32 iCount = pChildren->size();
	for (int i = 0; i < iCount; i++) {
		if (IsIllegal(pStart + i)) {
			return true;
		}
	}
    return false;
}

void MixerNode::SpecialValueHandle(map<string, ST_PRODUCT_DATA> &mapDBVal)
{
	if (m_billegalSpecial) {
		m_billegalSpecial = false;
		return;
	}

	//water and admixture weight handle
	double real = 0.0;
	string rawReal;
	map<string, ST_PRODUCT_DATA>::iterator it;
	bool bHasWater = false;
	bool bHasAdmixture = false;
	for (it = mapDBVal.begin();it != mapDBVal.end();++it) {
	    if ( "221_R" == it->first || "222_R" == it->first ) {
			real += (it->second).value;
			rawReal += (it->second).rawData;
			bHasAdmixture = true;
		} else if ("233_R" == it->first) {
			bHasWater = true;
		}
	}
	//DebugLog("waterReal[%0.3lf], admixture[%0.3lf]\n", mapDBVal["233_R"], real);
	if (bHasAdmixture && bHasWater) {
		mapDBVal["233_R"].value -= real;
		mapDBVal["233_R"].rawData = mapDBVal["233_R"].rawData + "-" + rawReal;
	}
}

void MixerNode::SetSpecialHandleFlg(bool is_speflg)
{
	m_bspecial = is_speflg;
}

bool MixerNode::IsRinsePot(map<string, ST_PRODUCT_DATA> &mapDBVal)
{
	bool b_ret = false;
	if ( MACHINE_CEMENT == m_iMachineType )
	{
		map<string, ST_PRODUCT_DATA>::iterator it;
		string valueKey;
		for (it = mapDBVal.begin();it != mapDBVal.end();++it) {
			valueKey = (it->first).substr(0, 3);
			if (WATER_HOPPER_ID == valueKey) {
				b_ret = true;
			}
			else {
				b_ret = false;
				break;
			}
		}
	}

	return b_ret;
}

void MixerNode::ClearProductValue(HopperNode*pNode, int iAndId)
{
	HopperNode *pChildrenNode = NULL;
	for (int i = 0; i < pNode->GetChildren()->size(); i++)
	{
		pChildrenNode = (HopperNode *)(pNode->GetChildren()->at(i));
		if (iAndId == pChildrenNode->GetId())
		{
			pChildrenNode->GetProduct()->ClearValue();
			break;
		}
		else if (IS_BKT_NODE(pChildrenNode->GetId()) )
		{
			for (INT32 j = 0; j < pChildrenNode->GetChildren()->size(); j++) {
				ClearProductValue((HopperNode *)(pChildrenNode->GetChildren())->at(j), iAndId);
			}
		}
		else if (IS_NOANA_NODE(pChildrenNode->GetId()))
		{
			pChildrenNode->GetProduct()->ClearValue();
			break;
		}
	}
}

void MixerNode::OnOutputStart(HopperNode *pHopper){
    if ( IsOutputting() && (pHopper->GetTaps())->front() != GetTaps()->front()){
        return;
    }

    HopperNode::OnOutputStart(pHopper);
}

bool MixerNode::IsValidDBVal(map<string, ST_PRODUCT_DATA> &mapDBVal)
{
	bool b_ret = false;
	if (mapDBVal.size()>0)
	{
		string valueKey;
		map<string, ST_PRODUCT_DATA>::iterator it;

		for (it = mapDBVal.begin();it != mapDBVal.end();++it) {
			valueKey = it->first;
			if ( 0 == (it->first.substr(0,1)).compare("4") ) {
				//如果是温度话，不进行有效性的判断
				continue;
			}
			if ((0 == valueKey.substr(3, 2).compare(CALC_SUFFIX) || 0 == valueKey.substr(3, 2).compare(REAL_SUFFIX) ) 
				&& (0 != it->second.value || NULL != strstr(it->second.rawData.c_str(), "[") )) {
				DebugLog("[Mixer] id[%s] value[%lf][%s]\n", valueKey.c_str(), it->second.value, it->second.rawData.c_str());
				//判断所有称重值或计算值不为0或者含有字模的时候，返回TRUE（表示为有效DB数据）
				b_ret = true;
				break;
			}
		}
	}
	else {
		b_ret = false;
	}

	return b_ret;
}

void MixerNode::SetIllegaSpecial(bool bIllSpe)
{
	m_billegalSpecial = bIllSpe;
}
