#include "TransDataReader_Flume.h"
#include "MeasureDef.h"
#include "TransConfig.h"
#include "ConfigGetter.h"

CRawDataReader_Flume::CRawDataReader_Flume()
	: CFlumeDBDataReader(STR_RAW_BD_NAME)
{

}

CRawDataReader_Flume::~CRawDataReader_Flume()
{

}

const string* CRawDataReader_Flume::getData()
{
	if (!m_curData)
	{
		next();
	}
	return m_curData;
}

void CRawDataReader_Flume::next()
{
	CFlumeDBDataReader::next();

	//���͵������У���һ��������ID��1�������η��͵��������ڵ����ݿ�
	if (/*m_bp.DataID <= m_CountPerBody && */m_curData && m_curDataPKs[0] == 1)
	{
		AddConfigInfoToData();
	}
}

void CRawDataReader_Flume::AddConfigInfoToData()
{
	CFlumeDataRW configDataRW;
	string config_id;
	//header��Ϣ
	{
		//��������
		configDataRW.AddHeaderInfo("DATA_TYPE", "CONFIG");

		//����ID��ʹ�ñ��������ļ����ļ�������ΪID��
		const DBInfo& dbInfo = m_pDBO->GetDBInfo();
		string configFilePath = dbInfo.configInfo.DBConfigPath;
		if (configFilePath[configFilePath.length() - 1] == '\\' || configFilePath[configFilePath.length() - 1] == '/')
		{
			configFilePath.erase(configFilePath.length() - 1);
		}
#if _WIN32
		config_id = configFilePath.substr(configFilePath.find_last_of('\\') + 1);
#else
		config_id = configFilePath.substr(configFilePath.find_last_of('/') + 1);
#endif
		configDataRW.AddHeaderInfo("CONFIG_ID", config_id);

		//DB����
		string table_name;
		switch (CFlumeDataRW::getSourceInfo()->source_type)
		{
		case 1:	//����ɼ�
			table_name = "SC";
			break;
		case 2:	//�������ɼ�
			table_name = "SS";
			break;
		default:
			break;
		}
		table_name = table_name + "_" + "CONFIG" + "_" + config_id;

		configDataRW.AddHeaderInfo("TABLE_NAME", table_name);

		//DB����ֶ��б�
		string col_name_list = "id";
		string col_type_list = "VCHAR(100)";
		col_name_list = col_name_list + ",content";
		col_type_list = col_type_list + ",TEXT";

		configDataRW.AddHeaderInfo("COL_NAME_ARR", col_name_list);
		configDataRW.AddHeaderInfo("COL_TYPE_ARR", col_type_list);
	}

	//body��Ϣ
	{
		string config_str;
		CConfigGetter configGetter;
		configGetter.Read(m_pDBO->GetDBInfo().configInfo.DBConfigPath, CFlumeDataRW::getSourceInfo()->source_type, config_str);

		configDataRW.NewlineInBody();
		configDataRW.AddValueToLastBodyRow(config_id);
		configDataRW.AddValueToLastBodyRow(config_str);
	}

	const string* config_body = configDataRW.GetFinal();
	m_curDataWithConfig = *config_body;
	m_curDataWithConfig.append(*m_curData);

	m_curData = &m_curDataWithConfig;
}

CMeasureDataReader_Flume::CMeasureDataReader_Flume()
	: CFlumeDBDataReader(STR_PRODUCT_BD_NAME)
{

}

CMeasureDataReader_Flume::~CMeasureDataReader_Flume()
{

}

const string* CMeasureDataReader_Flume::getData()
{
	if (!m_curData)
	{
		next();
	}
	return m_curData;
}

void CMeasureDataReader_Flume::next()
{
	CFlumeDBDataReader::next();
}

CDetailDataReader_Flume::CDetailDataReader_Flume()
	: CFlumeDBDataReader(STR_DETAIL_BD_NAME)
{

}

CDetailDataReader_Flume::~CDetailDataReader_Flume()
{

}

const string* CDetailDataReader_Flume::getData()
{
	if (!m_curData)
	{
		next();
	}
	return m_curData;
}

void CDetailDataReader_Flume::next()
{
	CFlumeDBDataReader::next();
}

CFlumeDBDataReader::CFlumeDBDataReader(const char* szName)
	: CDBDataReader(szName)
{
	m_curData = NULL;
	if (strcmp(szName, STR_DETAIL_BD_NAME) == 0)
	{
		m_DATA_TYPE = "MAPPING";
		m_CountPerBody = CTransConfig::GetDetailRowCountPerBody(10);
	}
	else if (strcmp(szName, STR_PRODUCT_BD_NAME) == 0)
	{
		m_DATA_TYPE = "PRODUCT";
		m_CountPerBody = CTransConfig::GetMeasureRowCountPerBody(10);
	}
	else if (strcmp(szName, STR_RAW_BD_NAME) == 0)
	{
		m_DATA_TYPE = "RAW";
		m_CountPerBody = CTransConfig::GetRawRowCountPerBody(10);
	}
	else
	{
		m_DATA_TYPE = "";
	}
}

CFlumeDBDataReader::~CFlumeDBDataReader()
{

}

void CFlumeDBDataReader::next()
{
	CDBDataReader::next();

	if (m_pQuery == NULL || m_pQuery->eof()) {
		m_curData = NULL;
		m_bp.DataID = 0;
		return;
	}

	m_data_rw.Clear();
	m_curDataPKs.clear();

	//header��Ϣ
	{
		//��������
		m_data_rw.AddHeaderInfo("DATA_TYPE", m_DATA_TYPE);
		
		//����ID��ʹ�ñ��������ļ����ļ�������ΪID��
		const DBInfo& dbInfo = m_pDBO->GetDBInfo();
		string configFilePath = dbInfo.configInfo.DBConfigPath;
		if (configFilePath[configFilePath.length() - 1] == '\\' || configFilePath[configFilePath.length() - 1] == '/')
		{
			configFilePath.erase(configFilePath.length() - 1);
		}
#if _WIN32
		string config_id = configFilePath.substr(configFilePath.find_last_of('\\') + 1);
#else
		string config_id = configFilePath.substr(configFilePath.find_last_of('/') + 1);
#endif
		m_data_rw.AddHeaderInfo("CONFIG_ID", config_id);

		//DB����
		string table_name;
		switch (CFlumeDataRW::getSourceInfo()->source_type)
		{
		case 1:	//����ɼ�
			table_name = "SC";
			break;
		case 2:	//�������ɼ�
			table_name = "SS";
			break;
		default:
			break;
		}
		table_name = table_name + "_" + m_DATA_TYPE + "_" + config_id;

		m_data_rw.AddHeaderInfo("TABLE_NAME", table_name);

		//DB����ֶ��б�
		string col_name_list = "PKA";
		string col_type_list = "VCHAR(100)";
		for (int i = 0; i < m_pQuery->numDataFields(); i++)
		{
			col_type_list = col_type_list + "," + "VCHAR(100)";
			col_name_list = col_name_list + "," + "col_" + m_pQuery->DataFieldName(i);
		}

		m_data_rw.AddHeaderInfo("COL_NAME_ARR", col_name_list);
		m_data_rw.AddHeaderInfo("COL_TYPE_ARR", col_type_list);
	}

	//body��Ϣ
	{
		int while_count = 0;
		while (while_count < m_CountPerBody)
		{
			m_data_rw.NewlineInBody();
            m_data_rw.AddValueToLastBodyRow(GetString(m_pQuery->RowPK()));
			for (int i = 0; i < m_pQuery->numDataFields(); i++)
			{
				m_data_rw.AddValueToLastBodyRow(m_pQuery->GetStringDataField(i));
			}

			while_count++;
			m_curDataPKs.push_back(m_pQuery->RowPK());

			if (while_count < m_CountPerBody)
			{
				m_pQuery->nextRow();
				if (!m_pQuery->eof()) {
					m_bp.DataID = m_pQuery->RowPK();
				}
				else
				{
					break;
				}
			}
		} 
	}

	m_curData = m_data_rw.GetFinal();
}

string CFlumeDBDataReader::GetString(int i)
{
    stringstream newstr;
    newstr << i;
    return newstr.str();
}
