#include "TransDataReader.h"
#include "MeasureDef.h"
#include <string>
#include <algorithm>
#include "TRWData.h"
#include "log.h"

const std::map<string, const char*>::value_type raw_value[] =
{
	std::map<string, const char*>::value_type(STR_MIX_ID, "packet_id"),
	std::map<string, const char*>::value_type(STR_ILLEGAL, "sensor_id"),
	std::map<string, const char*>::value_type(STR_MIX_TIME, "value"),
	std::map<string, const char*>::value_type(STR_RECORD_TIME, "time"),
	std::map<string, const char*>::value_type(STR_DRY_MIX_TIME, "type")
};
static std::map<string, const char*> s_c_RawDataNode(raw_value, raw_value + 5);

const std::map<string, const char*>::value_type config_value[] =
{
	std::map<string, const char*>::value_type(STR_MIX_ID, "id"),
	std::map<string, const char*>::value_type(STR_ILLEGAL, "in"),
	std::map<string, const char*>::value_type(STR_MIX_TIME, "out"),
	std::map<string, const char*>::value_type(STR_RECORD_TIME, "bucket"),
	std::map<string, const char*>::value_type(STR_DRY_MIX_TIME, "s_real"),
	std::map<string, const char*>::value_type(STR_DRY_MIX_TIME, "s_calc"),
	std::map<string, const char*>::value_type(STR_DRY_MIX_TIME, "s_ideal")
};
static std::map<string, const char*> s_c_ConfigDataNode(config_value, config_value + 7);

const std::map<string, const char*>::value_type product_value[] =
{
	std::map<string, const char*>::value_type(STR_MIX_ID, "mixture_id"),
	std::map<string, const char*>::value_type(STR_ILLEGAL, "illegal_identificat"),
	std::map<string, const char*>::value_type(STR_MIX_TIME, "wet_mixing_time"),
	std::map<string, const char*>::value_type(STR_RECORD_TIME, "time"),
	std::map<string, const char*>::value_type(STR_DRY_MIX_TIME, "dry_mixing_time"),
	std::map<string, const char*>::value_type(STR_WET_MIX_TIME, "wet_mixing_time"),
	std::map<string, const char*>::value_type(AGGR1_HOPPER_ID, "aggregate1"),
	std::map<string, const char*>::value_type(AGGR2_HOPPER_ID, "aggregate2"),
	std::map<string, const char*>::value_type(AGGR3_HOPPER_ID, "aggregate3"),
	std::map<string, const char*>::value_type(AGGR4_HOPPER_ID, "aggregate4"),
	std::map<string, const char*>::value_type(AGGR5_HOPPER_ID, "aggregate5"),
	std::map<string, const char*>::value_type(AGGR6_HOPPER_ID, "aggregate6"),
	std::map<string, const char*>::value_type(POWDER1_HOPPER_ID, "powder1"),
	std::map<string, const char*>::value_type(POWDER2_HOPPER_ID, "powder2"),
	std::map<string, const char*>::value_type(POWDER3_HOPPER_ID, "powder3"),
	std::map<string, const char*>::value_type(POWDER4_HOPPER_ID, "powder4"),
	std::map<string, const char*>::value_type(POWDER5_HOPPER_ID, "powder5"),
	std::map<string, const char*>::value_type(POWDER6_HOPPER_ID, "powder6"),
	std::map<string, const char*>::value_type(ADMIXTURE1_HOPPER_ID, "admixture1"),
	std::map<string, const char*>::value_type(ADMIXTURE2_HOPPER_ID, "admixture2"),
	std::map<string, const char*>::value_type(LIGNIN_HOPPER_ID, "lignin"),
	std::map<string, const char*>::value_type(ASPHALT_HOPPER_ID, "asphalt"),
	std::map<string, const char*>::value_type(WATER_HOPPER_ID, "water"),
	std::map<string, const char*>::value_type(MIX_TEMPERATURE_ID, "mixing_temperature"),
	std::map<string, const char*>::value_type(ASPHALT_TEMPERATURE_ID, "asphalt_temperature"),
	std::map<string, const char*>::value_type(AGGR_TEMPERATURE_ID, "aggr_temperature")
};
static std::map<string, const char*> s_c_ProductDataNode(product_value, product_value + 26);

const std::map<string, const char*>::value_type detail_value[] =
{
	std::map<string, const char*>::value_type(STR_MIX_ID, "mixture_id"),
	std::map<string, const char*>::value_type(STR_REAL_SENSOR_ID, "sensor_id"),
	std::map<string, const char*>::value_type(STR_START_TIME, "start_time"),
	std::map<string, const char*>::value_type(STR_END_TIME, "end_time")
};
static std::map<string, const char*> s_c_DetailDataNode(detail_value, detail_value + 4);

#define MAX_RETRY_COUNT 3

///////////////////////////////////////////////////////
// DBDataReader
//
CDBDataReader::CDBDataReader(const char * szName) :
        m_strName(szName) {
    m_pDBO = NULL;
    m_pQuery = NULL;

 	//try
 	//{
 	//	Init();
 	//}
 	//catch (...)
 	//{
 	//}

	m_bp.DataID = 0;
}

CDBDataReader::~CDBDataReader() {
    Release();
}

void CDBDataReader::Init() {
    Release();

    m_bp = m_bpStorage.GetBreakPoint(m_strName);

    m_fileGetter.RefreshList();
    m_fileGetter.GetDBListAfterBK(m_strName, m_bp.DataFileName, m_listDB);

    while (m_listDB.size() > 0) {
        m_bp.DataFileName = m_listDB.front();
		try
		{
			m_pDBO = OpenDBOperator(m_bp.DataFileName, true);
			if (m_pDBO) {
				m_listDB.pop_front();
				m_pQuery = m_pDBO->Query(++m_bp.DataID, -1);	//检索从DataID开始到DB的最后一条记录
				if (m_pQuery && !m_pQuery->eof()) {
					break; //数据准备就绪
				}
				else if (m_listDB.size() > 0) {
					m_bp.DataID = 0;
					Release(); //关闭当前数据库，继续检查下一个数据库
				}
			}
			else
			{
				m_bp.DataID = 0;
				Release(); //关闭当前数据库，继续检查下一个数据库
				m_listDB.pop_front();
			}
		}
		catch (CDBModuleException& e)
		{
			SCERROR("Trans::CDBDataReader--OpenDBOperator failed errorid = %d, errLog = %s dbpath = %s\n", e.errorCode(), e.errorCodeAsString(e.errorCode()), m_bp.DataFileName.c_str());

			m_bp.DataID = 0;
			Release(); //关闭当前数据库，继续检查下一个数据库
			m_listDB.pop_front();
		}
		catch (...)
		{
			SCERROR("Trans::CDBDataReader--OpenDBOperator failed dbpath = %s\n", m_bp.DataFileName.c_str());

			m_bp.DataID = 0;
			Release(); //关闭当前数据库，继续检查下一个数据库
			m_listDB.pop_front();
		}
    }
}

void CDBDataReader::Release() {
    if (m_pQuery) {
        m_pDBO->QueryFinalize(m_pQuery);
        m_pQuery = NULL;
    }

    if (m_pDBO) {
        ReleaseDBOperator(m_pDBO);
        m_pDBO = NULL;
    }
}

void CDBDataReader::next() {
	//printf("[CDBDataReader::next]%s [%s][%d]dataid[%d]\n", m_strName.c_str(), (m_pQuery) ? "true" : "false", m_pQuery->RowPK(), m_bp.DataID);
    if (m_pQuery && m_pQuery->RowPK() > 0) {
        m_bp.DataID = m_pQuery->RowPK();
        m_bpStorage.SaveBreakPoint(m_strName, m_bp);
    } else if (m_bp.DataID > 0) {
        m_bpStorage.SaveBreakPoint(m_strName, m_bp);
    }

    if (m_pQuery && !m_pQuery->eof()) {
        m_pQuery->nextRow();
        m_bp.DataID = m_pQuery->RowPK();
    } else {
        Init();
    }
	//printf("dataid[%d]\n",m_bp.DataID);
}

int CDBDataReader::FindField(const list<FieldInfo>& fields, const char* szFieldName) {
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
///////////////////////////////////////////////////////
// RawDataReader
//

CRawDataReader::CRawDataReader() :
        CDBDataReader(STR_RAW_BD_NAME) {
    m_curData = NULL;
	m_curGetCount = 0;
}

CRawDataReader::~CRawDataReader() {
}

const string* CRawDataReader::getData() {

	if (m_curGetCount > MAX_RETRY_COUNT)
	{
		m_curData = NULL;
		m_curGetCount = 0;
	}

	if (!m_curData)
	{
		next();
		m_curGetCount = 0;
	}

	m_curGetCount++;
    return m_curData;
}

void CRawDataReader::next()
{
	CDBDataReader::next();
	m_curGetCount = 0;

	if (m_pQuery == NULL || m_pQuery->eof() || 0 == m_bp.DataID) {
		m_curData = NULL;
		m_bp.DataID = 0;
		return;
	}

	const list<FieldInfo>& fields = m_pDBO->GetDBInfo().fields;
	int iPkgId = FindField(fields, "packet_id");
	int iTimestamp = FindField(fields, "timestamp");

	//如果没有必需的字段，这返回NULL
	if (iPkgId < 0 || iTimestamp < 0) {
		m_curData = NULL;
		return;
	}
	//printf("[CRawDataReader]m_bp.DataID == [%d] @@@@@@@@@@@@@@\n", m_bp.DataID);
	m_json.Clear();
	if (m_bp.DataID == 1)	//当获取的是一个DB文件的第一条数据时，需要传输这个DB对应的配置信息
	{
		//printf("[CRawDataReader]m_bp.DataID == 1 @@@@@@@@@@@@@@\n");
		AddConfigInfoIntoJson(m_json);
	}

	int contentIndex = m_json.CreateContent(1);		// Protocol: 1 = DB type

													//DB数据ContentData
	int contentDataIndex = m_json.CreateContentDataIntoContent(contentIndex, 2);
	int dataIndex;

	string sPkgId = m_pQuery->GetStringDataField(iPkgId);
	string sTimestamp = m_pQuery->GetStringDataField(iTimestamp);
	const char *szValue;
	int iCol = -1;

	list<FieldInfo>::const_iterator itEnd = fields.end();
	for (list<FieldInfo>::const_iterator it = fields.begin(); it != itEnd; ++it) {
		iCol++;
		if (iCol == iPkgId || (iCol == iTimestamp)) {
			continue;
		}
		szValue = m_pQuery->GetStringDataField(iCol);
		if (szValue && *szValue) {
			dataIndex = m_json.CreateDataIntoContentData(contentDataIndex);
			m_json.AddDataInfoIntoData(dataIndex, "packet_id", sPkgId);
			m_json.AddDataInfoIntoData(dataIndex, "sensor_id", it->fieldName);
			m_json.AddDataInfoIntoData(dataIndex, "time", sTimestamp);
			m_json.AddDataInfoIntoData(dataIndex, "value", szValue);
			if (it->fieldOriginalName.find("dig") == 0) {
				m_json.AddDataInfoIntoData(dataIndex, "type", "1"); 	//数字量
			}
			else {
				m_json.AddDataInfoIntoData(dataIndex, "type", "2");		//模拟量
			}
		}
	}

	m_curData = m_json.GetFinal();
	//CTRWData rw;
	//rw._WriteDebugForLog((char*)m_curData->c_str(), "d:\\test.log");
    //printf("RawDataReader:: getData: %s\n", m_curData->c_str());
}

struct CRawDataReader::ST_BUCKET
{
	int blowingCanID;
	int blowingChannelID;
	int bucketID;
};

struct CRawDataReader::BUCKET_COMPARE_FUNC
{
	BUCKET_COMPARE_FUNC(int bucketID)
		: _bucketID(bucketID)	{
	}

	bool operator()(const ST_BUCKET& bucket)
	{
		if (bucket.bucketID == _bucketID)
		{
			return true;
		}
		return false;
	}

	int _bucketID;
};

#define DIG_ID_TO_NODE_ID(canId,channel)  ((canId)*100+channel)

void CRawDataReader::AddConfigInfoIntoJson(CJsonRW& jsonRW)
{
    int hop_id[] = {201,202,203,204,205,206,211,212,213,214,215,221,222,231,232,233,401,402,403};
	const DBInfo& dbInfo = m_pDBO->GetDBInfo();
	string configFilePath = dbInfo.configInfo.DBConfigPath;
    //printf("@@@@@config_path:{%s}@@@@\n",configFilePath.c_str());
#if _WIN32
	char cpathend = *(configFilePath.rbegin());
    if (cpathend != '\\')
    {
        configFilePath = configFilePath + '\\';
    }
#endif

    configFilePath = configFilePath + string(CONFIG_HOPPER_FILE);

	CTRWData rw;
	int contentIndex = jsonRW.CreateContent(2);			//配置信息类型
	int contentDataIndex = jsonRW.CreateContentDataIntoContent(contentIndex, 4, dbInfo.configInfo.ConfigTimeStamp);		//原始数据类型的配置信息

#if _WIN32
#else
    string szMixConfig = dbInfo.configInfo.DBConfigPath + string(CONFIG_SYS_FILE);
	int iMixType = rw._GetProfileInt("mix_machine_1", "mix_machine_type", 0, szMixConfig.c_str());
	if (MACHINE_MODIFIED_ASPHALT == iMixType )
	{
		AddSBSConfigInfoIntoJson(contentDataIndex,jsonRW, configFilePath);
		return;
	}
	else if (MACHINE_STORAGE_TANK == iMixType) {
		AddTankConfigInfoIntoJson(contentDataIndex, jsonRW, configFilePath);
		return;
	}
#endif

	//Add Mix
	list<int> mixOutTap;
	{
		char szBuf[LINESIZE] = { 0 };
		int iCanId, iChannel;

		//从配置文件中读取拌和机基础信息
#if _WIN32
		iCanId = rw._GetProfileInt("mixer", "blowing_tap_id", 0, configFilePath.c_str());
		iChannel = 0;
#else
		iCanId = rw._GetProfileInt("mixer", "blowing_tap_can_id", 0, configFilePath.c_str());
		iChannel = rw._GetProfileInt("mixer", "blowing_tap_id", 0, configFilePath.c_str());
#endif
		int mixData = jsonRW.CreateDataIntoContentData(contentDataIndex);
		mixOutTap.push_back(DIG_ID_TO_NODE_ID(iCanId, iChannel));
		jsonRW.AddDataInfoIntoData(mixData, "hopper_id", "101");
		jsonRW.AddDataInfoIntoData(mixData, "out", mixOutTap);
	}

	//Add hopper
	list<int> aggregateoutTap, asphaltInTap, asphaltoutTap;		//for temperature
	bool hasAggTemp = false, hasAspTemp = false;				//for temperature
	int AggTempSensorID = 0, AspTempSensorID = 0;				//for temperature
	{
		list<ST_BUCKET> Buckets;
		LoadBucket(Buckets, configFilePath.c_str());

		int HopperCount = GetHopperCount(configFilePath.c_str());
		char szAppName[LINESIZE] = { 0 };
        int count = 0;
		for (int i = 0; i < HopperCount; i++)
		{
#if _WIN32
			sprintf(szAppName, "hopper_%d", i + 1);
            int hopperID = rw._GetProfileInt(szAppName, "hopper_id", 0, configFilePath.c_str());
#else
            sprintf(szAppName, "hopper_%d", hop_id[count]);
            count++;

            int hopperID = rw._GetProfileInt(szAppName, "hopper_id", 0, configFilePath.c_str());
            if ( 0 >= hopperID ){
                i--;
                continue;
            }
#endif
			int hopperData = jsonRW.CreateDataIntoContentData(contentDataIndex);

			//id
            //int hopperID = rw._GetProfileInt(szAppName, "hopper_id", 0, configFilePath.c_str());
			jsonRW.AddDataInfoIntoData(hopperData, "hopper_id", hopperID);

			list<int> inTap, outTap;
			int iCanId, iChannel;
			char szKeyName[LINESIZE] = {0};
			int num = 0;
			//inTap
			while (1) {
#if _WIN32
				sprintf(szKeyName, "loading_tap%d_id", num + 1);
				iCanId = rw._GetProfileInt(szAppName, szKeyName, -1, configFilePath.c_str());
				iChannel = 0;
				if (iCanId > 0) {
#else
				sprintf(szKeyName, "loading_tap%d_can_id", num + 1);
				iCanId = rw._GetProfileInt(szAppName, szKeyName, -1, configFilePath.c_str());
				sprintf(szKeyName, "loading_tap%d_channel_id", num + 1);
				iChannel = rw._GetProfileInt(szAppName, szKeyName, -1, configFilePath.c_str());
				if (iCanId > 0 && iChannel > 0) {
#endif
					inTap.push_back(DIG_ID_TO_NODE_ID(iCanId, iChannel));
				}
				else {
					break;
				}
				num++;
			}
			jsonRW.AddDataInfoIntoData(hopperData, "in", inTap);

			//outTap
			num = 0;
			while (1) {
#if _WIN32
				sprintf(szKeyName, "blowing_tap%d_id", num + 1);
				iCanId = rw._GetProfileInt(szAppName, szKeyName, -1, configFilePath.c_str());
				iChannel = 0;
				if (iCanId > 0) {
#else
				sprintf(szKeyName, "blowing_tap%d_can_id", num + 1);
				iCanId = rw._GetProfileInt(szAppName, szKeyName, -1, configFilePath.c_str());
				sprintf(szKeyName, "blowing_tap%d_channel_id", num + 1);
				iChannel = rw._GetProfileInt(szAppName, szKeyName, -1, configFilePath.c_str());
				if (iCanId > 0 && iChannel > 0) {
#endif
					outTap.push_back(DIG_ID_TO_NODE_ID(iCanId, iChannel));
				}
				else {
					break;
				}
				num++;
			}
			jsonRW.AddDataInfoIntoData(hopperData, "out", outTap);

			//bucket
			int bucketID = rw._GetProfileInt(szAppName, "bucket_id", 0, configFilePath.c_str());
			list<ST_BUCKET>::iterator it = find_if(Buckets.begin(), Buckets.end(), BUCKET_COMPARE_FUNC(bucketID));
			if (it != Buckets.end())
			{
				jsonRW.AddDataInfoIntoData(hopperData, "bucket", DIG_ID_TO_NODE_ID(it->blowingCanID, it->blowingChannelID));
			}

			//weight
			int weightID = rw._GetProfileInt(szAppName, "weighing_cell_id", 0, configFilePath.c_str());
			if (weightID > 0)
			{
				jsonRW.AddDataInfoIntoData(hopperData, "s_real", weightID);
			}

			//calc
			int calcID = rw._GetProfileInt(szAppName, "calc_id", 0, configFilePath.c_str());
			if (calcID > 0)
			{
				jsonRW.AddDataInfoIntoData(hopperData, "s_calc", calcID);
			}

			//set
			int setID = rw._GetProfileInt(szAppName, "set_id", 0, configFilePath.c_str());
			if (setID > 0)
			{
				jsonRW.AddDataInfoIntoData(hopperData, "s_ideal", setID);
			}

			//temp check
			int tempID = rw._GetProfileInt(szAppName, "temperature_id", 0, configFilePath.c_str());
			if (tempID > 0)
			{
				if (hopperID >= 201 && hopperID <= 206 )		//骨料
				{
					hasAggTemp == true;
					aggregateoutTap.merge(outTap);
					AggTempSensorID = tempID;
				}
				if (hopperID == 232)		//沥青
				{
					hasAspTemp == true;
					asphaltInTap.swap(inTap);
					AspTempSensorID = tempID;
				}
			}

			//save 沥青 outTap for 混合料温度。混合料温度的上料阀门是沥青的放料阀门
			if (hopperID == 232)	//232 is 沥青. this code is not good
			{
				asphaltoutTap.swap(outTap);
			}
		}
	}

	//Add temperature
	{
		//mix temp
#if _WIN32
		int mixtemp = rw._GetProfileInt("mixer", "temperature_id", 0, configFilePath.c_str());
#else
		int mixtemp = 0;
#endif
		if (mixtemp > 0)
		{
			int mixTempData = jsonRW.CreateDataIntoContentData(contentDataIndex);
			jsonRW.AddDataInfoIntoData(mixTempData, "hopper_id", "401");
			jsonRW.AddDataInfoIntoData(mixTempData, "in", asphaltoutTap);
			jsonRW.AddDataInfoIntoData(mixTempData, "out", mixOutTap);
			jsonRW.AddDataInfoIntoData(mixTempData, "s_real", mixtemp);
		}

		//Asp temp
		if (hasAspTemp)
		{
			int AspTempData = jsonRW.CreateDataIntoContentData(contentDataIndex);
			jsonRW.AddDataInfoIntoData(AspTempData, "hopper_id", "402");
			jsonRW.AddDataInfoIntoData(AspTempData, "in", asphaltInTap);
			jsonRW.AddDataInfoIntoData(AspTempData, "out", asphaltoutTap);
			jsonRW.AddDataInfoIntoData(AspTempData, "s_real", AspTempSensorID);
		}

		//Agg temp
		if (hasAggTemp)
		{
			int AggTempData = jsonRW.CreateDataIntoContentData(contentDataIndex);
			jsonRW.AddDataInfoIntoData(AggTempData, "hopper_id", "403");
			jsonRW.AddDataInfoIntoData(AggTempData, "in", aggregateoutTap);
			jsonRW.AddDataInfoIntoData(AggTempData, "out", asphaltInTap);
			jsonRW.AddDataInfoIntoData(AggTempData, "s_real", AggTempSensorID);
		}
	}
}

void CRawDataReader::AddSBSConfigInfoIntoJson(int iContextIndex,CJsonRW& jsonRW, string szConfigFile)
{
	CTRWData rw;
	//Add hopper
	{
		int count = 0;
		char szAppName[LINESIZE] = { 0 };
		int i;

		while (1) {
			sprintf(szAppName, "hopper_%d", count + 1);
			i = rw._GetProfileInt(szAppName, "hopper_id", -1, szConfigFile.c_str());
			if (i == -1) {
				break;
			}
			count++;
		}
		int HopperCount = count;
		count = 0;
		for (int i = 0; i < HopperCount; i++)
		{

			sprintf(szAppName, "hopper_%d", i + 1);
			int hopperID = rw._GetProfileInt(szAppName, "hopper_id", 0, szConfigFile.c_str());
			int hopperData = jsonRW.CreateDataIntoContentData(iContextIndex);

			//id
			//int hopperID = rw._GetProfileInt(szAppName, "hopper_id", 0, configFilePath.c_str());
			jsonRW.AddDataInfoIntoData(hopperData, "hopper_id", hopperID);

			list<int> inTap, outTap;
			int iCanId, iChannel;
			char szKeyName[LINESIZE] = { 0 };
			int num = 0;
			//inTap
			while (1) {
				sprintf(szKeyName, "loading_tap%d_can_id", num + 1);
				iCanId = rw._GetProfileInt(szAppName, szKeyName, -1, szConfigFile.c_str());
				sprintf(szKeyName, "loading_tap%d_channel_id", num + 1);
				iChannel = rw._GetProfileInt(szAppName, szKeyName, -1, szConfigFile.c_str());
				if (iCanId > 0 && iChannel > 0) {
					inTap.push_back(DIG_ID_TO_NODE_ID(iCanId, iChannel));
				}
				else {
					break;
				}
				num++;
			}
			jsonRW.AddDataInfoIntoData(hopperData, "in", inTap);

			//outTap
		    sprintf(szKeyName, "blowing_tap_can_id", num + 1);
			iCanId = rw._GetProfileInt(szAppName, szKeyName, -1, szConfigFile.c_str());
			sprintf(szKeyName, "blowing_tap_channel_id", num + 1);
			iChannel = rw._GetProfileInt(szAppName, szKeyName, -1, szConfigFile.c_str());
			if (iCanId > 0 && iChannel > 0) {
				outTap.push_back(DIG_ID_TO_NODE_ID(iCanId, iChannel));
				jsonRW.AddDataInfoIntoData(hopperData, "out", outTap);
			}

				//weight
			int weightID = rw._GetProfileInt(szAppName, "weighing_cell_id", 0, szConfigFile.c_str());
			if (weightID > 0)
			{
				jsonRW.AddDataInfoIntoData(hopperData, "s_real", weightID);
			}
		}
	}

	//Add Tank
	{
		AddSBSTank(iContextIndex, jsonRW, szConfigFile);
	}
}

void CRawDataReader::AddSBSTank(int iContextIndex, CJsonRW& jsonRW, string szConfigFile)
{
	CTRWData rw;
	char szAppName[50] = {0};
	int TempHopid[] = { 420,-1,430 };
	string AppName[] = { "retorttank","grindtank","developtank" };
	int iNum = 0;
	int iTankId;
	int iMark = 0;
	while (3 > iMark)
	{
		sprintf(szAppName, "%s_%d", AppName[iMark].c_str(),iNum + 1);
		iTankId = rw._GetProfileInt(szAppName, "tank_id", -1, szConfigFile.c_str());
		if (0 >= iTankId) {
			iMark++;
			iNum = 0;
			continue;
		}
		int hopperData = jsonRW.CreateDataIntoContentData(iContextIndex);
		jsonRW.AddDataInfoIntoData(hopperData, "hopper_id", iTankId);

		list<int> inTap, outTap;
		int iCanId, iChannel;
		char szKeyName[LINESIZE] = { 0 };
		int num = 0;
		//inTap
		while (1) {
			sprintf(szKeyName, "loading_tap%d_can_id", num + 1);
			iCanId = rw._GetProfileInt(szAppName, szKeyName, -1, szConfigFile.c_str());
			sprintf(szKeyName, "loading_tap%d_channel_id", num + 1);
			iChannel = rw._GetProfileInt(szAppName, szKeyName, -1, szConfigFile.c_str());
			if (iCanId > 0 && iChannel > 0) {
				inTap.push_back(DIG_ID_TO_NODE_ID(iCanId, iChannel));
			}
			else {
				break;
			}
			num++;
		}
		jsonRW.AddDataInfoIntoData(hopperData, "in", inTap);

		//outTap
		sprintf(szKeyName, "blowing_tap_can_id", num + 1);
		iCanId = rw._GetProfileInt(szAppName, szKeyName, -1, szConfigFile.c_str());
		sprintf(szKeyName, "blowing_tap_channel_id", num + 1);
		iChannel = rw._GetProfileInt(szAppName, szKeyName, -1, szConfigFile.c_str());
		if (iCanId > 0 && iChannel > 0) {
			outTap.push_back(DIG_ID_TO_NODE_ID(iCanId, iChannel));
			jsonRW.AddDataInfoIntoData(hopperData, "out", outTap);
		}

		//flow
		int iFlowID = rw._GetProfileInt(szAppName, "flowing_tap_id", 0, szConfigFile.c_str());
		if (iFlowID > 0)
		{
			jsonRW.AddDataInfoIntoData(hopperData, "s_real", iFlowID);
		}

		//Temperature
		int iTempID = rw._GetProfileInt(szAppName, "temperature_id", 0, szConfigFile.c_str());
		if (iTempID > 0 && TempHopid[iMark] > 0 )
		{
			hopperData = jsonRW.CreateDataIntoContentData(iContextIndex);
			jsonRW.AddDataInfoIntoData(hopperData, "hopper_id", TempHopid[iMark] + iNum + 1);
			jsonRW.AddDataInfoIntoData(hopperData, "in", inTap);
			jsonRW.AddDataInfoIntoData(hopperData, "out", outTap);
			jsonRW.AddDataInfoIntoData(hopperData, "s_real", iTempID);
		}
		iNum++;
	}
}

void CRawDataReader::AddTankConfigInfoIntoJson(int iContextIndex, CJsonRW& jsonRW, string szConfigFile)
{
	CTRWData rw;
	char szAppName[50] = { 0 };
	int iNum = 0;
	int iConfigId;
	while (1)
	{
		sprintf(szAppName, "hopper_%d", iNum + 1);
		iConfigId = rw._GetProfileInt(szAppName, "hopper_id", -1, szConfigFile.c_str());
		if (0 >= iConfigId) {
			break;
		}
		int hopperData = jsonRW.CreateDataIntoContentData(iContextIndex);
		jsonRW.AddDataInfoIntoData(hopperData, "hopper_id", iConfigId);

		//inFlow\incumuFlow
		list<int> inTap, outTap;
		iConfigId = rw._GetProfileInt(szAppName, "in_flow", 0, szConfigFile.c_str());
		iConfigId = (0 > iConfigId) ? 0 : iConfigId;
		inTap.push_back(iConfigId);
		iConfigId = rw._GetProfileInt(szAppName, "in_cumuflow", 0, szConfigFile.c_str());
		iConfigId = (0 > iConfigId) ? 0 : iConfigId;
		inTap.push_back(iConfigId);

		jsonRW.AddDataInfoIntoData(hopperData, "in", inTap);

		//outFlow\outcumuFlow
		iConfigId = rw._GetProfileInt(szAppName, "out_flow", 0, szConfigFile.c_str());
		iConfigId = (0 > iConfigId) ? 0 : iConfigId;
		outTap.push_back(iConfigId);
		iConfigId = rw._GetProfileInt(szAppName, "out_cumuflow", 0, szConfigFile.c_str());
		iConfigId = (0 > iConfigId) ? 0 : iConfigId;
		outTap.push_back(iConfigId);

		jsonRW.AddDataInfoIntoData(hopperData, "out", outTap);

		//liquid
		iConfigId = rw._GetProfileInt(szAppName, "liquid_level", 0, szConfigFile.c_str());
		if (iConfigId > 0)
		{
			jsonRW.AddDataInfoIntoData(hopperData, "s_real", iConfigId);
		}
		//temperature
		iConfigId = rw._GetProfileInt(szAppName, "temperature", 0, szConfigFile.c_str());
		if (iConfigId > 0)
		{
			jsonRW.AddDataInfoIntoData(hopperData, "s_calc", iConfigId);
		}
		iNum++;
	}
}

int CRawDataReader::GetHopperCount(const char* szConfigFile) {
    int hop_id[] = {201,202,203,204,205,206,211,212,213,214,215,221,222,231,232,233,401,402,403};
	CTRWData rw;
	int count = 0;
	char szAppName[LINESIZE];
	int i;

#if _WIN32
    while(1){
        sprintf(szAppName, "hopper_%d", count + 1);
        i = rw._GetProfileInt(szAppName, "hopper_id", -1, szConfigFile);
        if (i == -1) {
            break;
        }
        count++;
    }
#else
    int num = 0;
    while(num < 19){
        sprintf(szAppName, "hopper_%d", hop_id[num]);
        num++;

        i=rw._GetProfileInt(szAppName,"hopper_id",-1,szConfigFile);
        if(i==-1){
            continue;
        }
        count++;
    }
#endif
	return count;
}

int CRawDataReader::GetBucketCount(const char* szConfigFile) {
	CTRWData rw;
	int count = 0;
	char szAppName[LINESIZE];
	int i;

#if _WIN32
    while(1){
        sprintf(szAppName,"bucket_%d",count+1);
        i=rw._GetProfileInt(szAppName,"bucket_id",-1,szConfigFile);
        if(i<=0){
            break;
        }
        count++;
    }
#else
    int num = 500;
    while(num < 504 ){
        sprintf(szAppName,"bucket_%d",++num);
        i=rw._GetProfileInt(szAppName,"bucket_id",-1,szConfigFile);
        if(i<=0){
            continue;
        }
        count++;
    }
#endif
	return count;
}

void CRawDataReader::LoadBucket(list<ST_BUCKET>& Buckets, const char* szConfigFile)
{
	CTRWData rw;
	char szBuf[LINESIZE] = { 0 };
	int BucketCount = GetBucketCount(szConfigFile);
	for (int i = 0; i < BucketCount; i++)
	{
#if _WIN32
		sprintf(szBuf, "bucket_%d", i + 1);
#else
        sprintf(szBuf, "bucket_%d", i + 1+500);
#endif
		ST_BUCKET bucket;
		bucket.bucketID = rw._GetProfileInt(szBuf, "bucket_id", 0, szConfigFile);
#if _WIN32
		bucket.blowingCanID = rw._GetProfileInt(szBuf, "blowing_tap1_id", 0, szConfigFile);
		bucket.blowingChannelID = 0;
#else
		bucket.blowingCanID = rw._GetProfileInt(szBuf, "blowing_tap1_can_id", 0, szConfigFile);
		bucket.blowingChannelID = rw._GetProfileInt(szBuf, "blowing_tap1_channel_id", 0, szConfigFile);
#endif
		Buckets.push_back(bucket);
	}
}

///////////////////////////////////////////////////////
// MeasureDataReader
//

CMeasureDataReader::CMeasureDataReader() :
        CDBDataReader(STR_PRODUCT_BD_NAME) {
    m_curData = NULL;
	m_curGetCount = 0;
    m_bDesigned = false;
	m_bIsColMapInited = false;

    //InitColMap();
}

CMeasureDataReader::~CMeasureDataReader() {
    m_RealCol.clear();
    m_DesignCol.clear();
}


struct CMeasureDataReader::_R_C_COMPARE_FUNC
{
// 	_R_C_COMPARE_FUNC(const string& col)
// 		: m_col(col)
	_R_C_COMPARE_FUNC(const char* col)
		: m_col()
	{
		if (col != NULL)
		{
			m_col = col;
		}
		else
		{
			m_col = "";
		}
	}

	bool operator()(const ColMapType::value_type& col)
	{
		if (m_col == col.second)
		{
			return true;
		}

		return false;
	}

	string m_col;
};

void CMeasureDataReader::InitColMap() {
    if (!m_pDBO) {
        return;
    }

	m_DesignCol.clear();
	m_RealCol.clear();

    const DBInfo& dbInfo = m_pDBO->GetDBInfo();
    list<FIELDINFO>::const_iterator itEnd = dbInfo.fields.end();
    string colName;
    string colPostfix;
    int colNum = 0;

    m_bDesigned = false;
    for (list<FIELDINFO>::const_iterator it = dbInfo.fields.begin(); it != itEnd; ++it) {
        colName = it->fieldName.substr(0, it->fieldName.length() - 2);
        colPostfix = it->fieldName.substr(it->fieldName.length() - 2, 2);

        if (colPostfix == DESIGN_SUFFIX) {
            //设定上料值
            m_DesignCol[colNum] = s_c_ProductDataNode[colName];
            m_bDesigned = true;
        } else if (colPostfix == REAL_SUFFIX) {
            //实际上料值（通过传感器进行计算得到的值）。如果有计算上料值，则优先使用计算上料值，不再添加实际上料值
			ColMapType::iterator it = find_if(m_RealCol.begin(), m_RealCol.end(), _R_C_COMPARE_FUNC(s_c_ProductDataNode[colName]));
			if (it == m_RealCol.end())
			{
				m_RealCol[colNum] = s_c_ProductDataNode[colName];
			}
        } else if (colPostfix == CALC_SUFFIX) {
            //计算上料值（通过UI采集得到的已经计算完成的值）。如果有计算上料值，则优先使用计算上料值，并删除已经存在的实际上料值
            if (m_RealCol.find(colNum) == m_RealCol.end()) {
                m_RealCol[colNum] = s_c_ProductDataNode[colName];
            }
			ColMapType::iterator it = find_if(m_RealCol.begin(), m_RealCol.end(), _R_C_COMPARE_FUNC(s_c_ProductDataNode[colName]));
			if (it != m_RealCol.end())
			{
				m_RealCol.erase(it);
			}
			m_RealCol[colNum] = s_c_ProductDataNode[colName];
        } else {
            //其他字段
            m_RealCol[colNum] = s_c_ProductDataNode[it->fieldName];
            m_DesignCol[colNum] = s_c_ProductDataNode[it->fieldName];
        }

        colNum++;
    }
	m_bIsColMapInited = true;
}

const string* CMeasureDataReader::getData() {
	if (m_curGetCount > MAX_RETRY_COUNT)
	{
		m_curData = NULL;
		m_curGetCount = 0;
	}

	if (!m_curData)
	{
		next();
		m_curGetCount = 0;
	}

	m_curGetCount++;
	return m_curData;
}

void CMeasureDataReader::next()
{
	CDBDataReader::next();
	m_curGetCount = 0;

	if (m_pQuery == NULL || m_pQuery->eof()) {
		m_curData = NULL;
		m_bp.DataID = 0;
		return;
	}

	m_json.Clear();
	if (m_bp.DataID == 1)	//当获取的是一个DB文件的第一条数据时，需要初始化这个DB对应的ColMap
	{
		//printf("[CRawDataReader]m_bp.DataID == 1 @@@@@@@@@@@@@@\n");
		InitColMap();
	}
	else if (!m_bIsColMapInited)
	{
		InitColMap();
	}

	int cIndex = m_json.CreateContent(1);		// Protocol: 1 = DB type
	ColMapType::iterator itEnd;

	//如果有设定上料值，则输出设定数据
	if (m_bDesigned) {
		int cdIndex_Set = m_json.CreateContentDataIntoContent(cIndex, 3);	// Protocol: 3 = 配合比 type
		int dIndex_Set = m_json.CreateDataIntoContentData(cdIndex_Set);
		itEnd = m_DesignCol.end();
		for (ColMapType::iterator it = m_DesignCol.begin(); it != itEnd; ++it) {
			const char* value = m_pQuery->GetStringDataField(it->first);
			if (value && *value) {
				m_json.AddDataInfoIntoData(dIndex_Set, it->second, value);
			}
		}
	}

	//输出实际上料数据
	int cdIndex_Mix = m_json.CreateContentDataIntoContent(cIndex, 1);	// Protocol: 1 = 盘数据 type
	int dIndex_Mix = m_json.CreateDataIntoContentData(cdIndex_Mix);
	itEnd = m_RealCol.end();
	for (ColMapType::iterator it = m_RealCol.begin(); it != itEnd; ++it) {
		const char* value = m_pQuery->GetStringDataField(it->first);
		if (value && *value) {
			m_json.AddDataInfoIntoData(dIndex_Mix, it->second, value);
		}
	}

	m_curData = m_json.GetFinal();
	//CTRWData rw;
	//rw._WriteDebugForLog((char*)m_curData->c_str(), "d:\\product.log");
	//printf("getdata:: getData: %s\n", m_curData->c_str());
}

///////////////////////////////////////////////////////
// DetailDataReader
//

CDetailDataReader::CDetailDataReader() :
        CDBDataReader(STR_DETAIL_BD_NAME) {
    m_curData = NULL;
	m_curGetCount = 0;
}

CDetailDataReader::~CDetailDataReader() {
}

const string* CDetailDataReader::getData() {
	if (m_curGetCount > MAX_RETRY_COUNT)
	{
		m_curData = NULL;
		m_curGetCount = 0;
	}

	if (!m_curData)
	{
		next();
		m_curGetCount = 0;
	}

	m_curGetCount++;
	return m_curData;
}

void CDetailDataReader::next()
{
	CDBDataReader::next();
	m_curGetCount = 0;

	if (m_pQuery == NULL || m_pQuery->eof()) {
		m_curData = NULL;
		m_bp.DataID = 0;
		return;
	}

	//detail个应该有四个字段：pkg_id, sensor_id, start_time, end_time
	list<FieldInfo> fields = m_pDBO->GetDBInfo().fields;
	if (fields.size() != 4) {
		m_curData = NULL;
		return;
	}

	m_json.Clear();
	int contentIndex = m_json.CreateContent(1);		// Protocol: 1 = DB type
	int contentDataIndex = m_json.CreateContentDataIntoContent(contentIndex, 4);
	int dataIndex;
	int iRow = 0;

	while (!m_pQuery->eof()) {
		dataIndex = m_json.CreateDataIntoContentData(contentDataIndex);
		m_json.AddDataInfoIntoData(dataIndex, "mixture_id", m_pQuery->GetStringDataField(0));
		m_json.AddDataInfoIntoData(dataIndex, "sensor_id", m_pQuery->GetStringDataField(1));
		m_json.AddDataInfoIntoData(dataIndex, "start_time", m_pQuery->GetStringDataField(2));
		m_json.AddDataInfoIntoData(dataIndex, "end_time", m_pQuery->GetStringDataField(3));

		iRow++;
		if (iRow >= 10) {
			break;
		}
		m_pQuery->nextRow();
		if (m_pQuery->eof()) {
			break;
		}
		else {
			m_bp.DataID = m_pQuery->RowPK();
		}
	}

	m_curData = m_json.GetFinal();

	//CTRWData rw;
	//rw._WriteDebugForLog((char*)m_curData->c_str(), "d:\\detail.log");
	//printf("CDetailDataReader:: getData: %s\n", m_curData->c_str());
}
