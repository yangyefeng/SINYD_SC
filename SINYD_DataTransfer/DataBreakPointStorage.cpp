#include "DataBreakPointStorage.h"
#include "MeasureDef.h"
#include "TRWData.h"

#define BREAKPOINTXML "SC_TransBreakPoint.xml"

tinyxml2::XMLDocument CDataBreakPointStorage::s_XMLDoc;
tinyxml2::XMLElement* CDataBreakPointStorage::s_FirstElement = NULL;
bool CDataBreakPointStorage::s_docOpened = false;
Mutex CDataBreakPointStorage::s_M;
CppSQLite3DB CDataBreakPointStorage::m_sqldb;
bool CDataBreakPointStorage::b_IsOpenDB = false;

CDataBreakPointStorage::CDataBreakPointStorage()
{
	CDataBreakPointStorage::Open();
}


CDataBreakPointStorage::~CDataBreakPointStorage()
{
	CDataBreakPointStorage::Save();
}

BreakPoint CDataBreakPointStorage::GetBreakPoint(const string & dataName)
{
	BreakPoint bp;
	bp.DataFileName = "";
	bp.DataID = 0;

	s_M.Lock();

    try
    {
        string selectSql = "select * from BreakPointData where FieldName='"+ dataName+"';";
        CppSQLite3Query sqlQuery= m_sqldb.execQuery(selectSql.c_str());
        //const char* retValue = NULL;
        if ( !sqlQuery.eof() ) {
            bp.DataFileName = string(sqlQuery.getStringField(1));
            bp.DataID = sqlQuery.getIntField(2);
        }
    }
    catch (CppSQLite3Exception& e)
    {
        printf("[CDataBreakPointStorage::CreateBreakPointDB] Create Breakpoint Table Error[%d]\n",e.errorCode());
    }

    if (s_FirstElement && "" == bp.DataFileName)
    {
        tinyxml2::XMLElement* dataElm = s_FirstElement->FirstChildElement(dataName.c_str());
        if (dataElm)
        {
            bp.DataFileName = dataElm->Attribute("DataFileName");
            bp.DataID = dataElm->IntAttribute("DataID");
        }
    }
	s_M.Unlock();

	return bp;
}

void CDataBreakPointStorage::SaveBreakPoint(const string & dataName, const BreakPoint & breakPoint)
{
	s_M.Lock();

    try
    {
		string selectSql = "select * from BreakPointData where FieldName='" + dataName + "';";
		CppSQLite3Query sqlQuery = m_sqldb.execQuery(selectSql.c_str());
        //const char* retValue = NULL;
		char dataId[10] = { 0 };
		sprintf(dataId, "%d", breakPoint.DataID);
		if (sqlQuery.eof()) {
			string insertSql = "insert into BreakPointData values('"+ dataName +"','"+ breakPoint.DataFileName+"','" + string(dataId) +"');";
			m_sqldb.execDML(insertSql.c_str());
		}
		else {
			string updateSql = "update BreakPointData set DataFileName='" + breakPoint.DataFileName + "',  DataID=";
			updateSql = updateSql+string(dataId) + " where FieldName == '"+ dataName + "';";
			m_sqldb.execDML(updateSql.c_str());
		}
    }
    catch (CppSQLite3Exception& e)
    {
        printf("[CDataBreakPointStorage::CreateBreakPointDB] Update Breakpoint Table Error[%d]\n",e.errorCode());
    }

	if (s_FirstElement)
	{
		tinyxml2::XMLElement* dataElm = s_FirstElement->FirstChildElement(dataName.c_str());
		if (dataElm)
		{
			dataElm->SetAttribute("DataFileName", breakPoint.DataFileName.c_str());
			dataElm->SetAttribute("DataID", breakPoint.DataID);
		}
		else
		{
			dataElm = s_XMLDoc.NewElement(dataName.c_str());
			dataElm->SetAttribute("DataFileName", breakPoint.DataFileName.c_str());
			dataElm->SetAttribute("DataID", breakPoint.DataID);
			s_FirstElement->InsertEndChild(dataElm);
		}
		Save();
	}

	s_M.Unlock();

}

void CDataBreakPointStorage::Open()
{
	s_M.Lock();
    if ( !b_IsOpenDB ) {
        string fileName = string(CONFIG_RUNTIME_PATH)+"SC_TransBreakPoint.db";
        CTRWData trw;
        try {
            if ( trw.IsFileExist(fileName.c_str())){
                m_sqldb.open(fileName.c_str());
            } else {
                CreateBreakPointDB(fileName.c_str());
            }
        }catch (...){
            printf("Open Or Create SC_TransBreakPoint.db Error!!!!!!\n");
        }
        b_IsOpenDB = true;
    }

	if (!s_docOpened)
	{
		bool bNeedCreate = false;

        string strFilePath = CONFIG_RUNTIME_PATH;
        strFilePath = strFilePath + string(BREAKPOINTXML);
		tinyxml2::XMLError err = s_XMLDoc.LoadFile(strFilePath.c_str());
		if (err == tinyxml2::XML_SUCCESS)
		{
			if (s_XMLDoc.NoChildren())
			{
				bNeedCreate = true;
			}
			else
			{
				s_FirstElement = s_XMLDoc.FirstChildElement("SC_TransBreakPoint");
				if (!s_FirstElement)
				{
					bNeedCreate = true;
				}
			}
		}
		else if (err == tinyxml2::XML_ERROR_FILE_NOT_FOUND || err == tinyxml2::XML_ERROR_EMPTY_DOCUMENT)
		{
			bNeedCreate = true;
		}


		if (bNeedCreate)
		{
			s_XMLDoc.Clear();
			s_FirstElement = s_XMLDoc.NewElement("SC_TransBreakPoint");
			if (s_FirstElement)
			{
				s_XMLDoc.InsertFirstChild(s_FirstElement);
				Save();
				s_docOpened = true;
			}
		}
	}
	s_M.Unlock();
}

void CDataBreakPointStorage::Save()
{
    string strFilePath = CONFIG_RUNTIME_PATH;
    strFilePath = strFilePath + string(BREAKPOINTXML);
	s_XMLDoc.SaveFile(strFilePath.c_str());
}

void CDataBreakPointStorage::CreateBreakPointDB(const char *fileName)
{
    try
    {
        m_sqldb.open(fileName);
        m_sqldb.execDML("PRAGMA synchronous = FULL;");
        string tableSql = "create table BreakPointData (FieldName REAL, DataFileName REAL, DataID INTEGER);";
        m_sqldb.execDML(tableSql.c_str());
        //string insertSql = "insert into BreakPointData values('raw_record',NULL,NULL);";
        //m_sqldb.execDML(insertSql.c_str());
        //insertSql = "insert into BreakPointData values('product',NULL,NULL);";
        //m_sqldb.execDML(insertSql.c_str());
        //insertSql = "insert into BreakPointData values('detail',NULL,NULL);";
        //m_sqldb.execDML(insertSql.c_str());
    }
    catch (CppSQLite3Exception& e)
    {
        printf("[CDataBreakPointStorage::CreateBreakPointDB] Create Breakpoint Table Error[%d]\n",e.errorCode());
    }

}
