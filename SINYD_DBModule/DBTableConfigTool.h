/////////////////////////////////////////////////////////////////////////////
/// @File name: DBTableConfigTool.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	���ݴ洢�ļ�������Ϣ��ȡ��
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#if !defined(_CDBTABLECONFIGTOOL_H)
#define _CDBTABLECONFIGTOOL_H

#include "DBModule.h"
#include "CppSqlite3/CppSQLite3.h"

class CDBTableConfigTool
{
public:
	CDBTableConfigTool(CppSQLite3DB* pSqliteDB);
	~CDBTableConfigTool();

	///*******************************************************
	/// @name:   CDBTableConfigTool::RenameTableConfig
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][const string &]orgTableName
	/// @param:  [in][const string &]newTableName
	/// @brief:  ���������ñ�ı���
	///*******************************************************
	void RenameTableConfig(const string & orgTableName, const string & newTableName);
	///*******************************************************
	/// @name:   CDBTableConfigTool::SetTableConfigToDB
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][const string &]tableName
	/// @param:  [in][const DBInfo &]info
	/// @brief:  �������ñ���Ϣ�����ݴ洢�ļ���
	///*******************************************************
	void SetTableConfigToDB(const string& tableName, const DBInfo& info);
	///*******************************************************
	/// @name:   CDBTableConfigTool::GetTableConfigFromDB
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][const string &]tableName
	/// @param:  [out][DBInfo *]pInfo
	/// @brief:  �����ݴ洢�ļ��л�ȡ���ñ���Ϣ
	///*******************************************************
	void GetTableConfigFromDB(const string& tableName, DBInfo* pInfo);
	///*******************************************************
	/// @name:   CDBTableConfigTool::SetDBConfigPath
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][const ConfigInfo &]configInfo
	/// @brief:  ���������ļ���Ϣ
	///*******************************************************
	void SetDBConfigPath(const ConfigInfo& configInfo);
	///*******************************************************
	/// @name:   CDBTableConfigTool::GetDBConfigPath
	/// @author: YaoDi
	/// @return: ConfigInfo
	/// @brief:  ��ȡ�����ļ���Ϣ
	///*******************************************************
	ConfigInfo GetDBConfigPath();

private:
	CppSQLite3DB* m_sqlite3DB;
};

#endif  //_CDBTABLECONFIGTOOL_H
