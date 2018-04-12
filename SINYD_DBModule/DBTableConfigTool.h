/////////////////////////////////////////////////////////////////////////////
/// @File name: DBTableConfigTool.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	数据存储文件配置信息读取类
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
	/// @brief:  重命名配置表的表名
	///*******************************************************
	void RenameTableConfig(const string & orgTableName, const string & newTableName);
	///*******************************************************
	/// @name:   CDBTableConfigTool::SetTableConfigToDB
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][const string &]tableName
	/// @param:  [in][const DBInfo &]info
	/// @brief:  设置配置表信息到数据存储文件中
	///*******************************************************
	void SetTableConfigToDB(const string& tableName, const DBInfo& info);
	///*******************************************************
	/// @name:   CDBTableConfigTool::GetTableConfigFromDB
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][const string &]tableName
	/// @param:  [out][DBInfo *]pInfo
	/// @brief:  从数据存储文件中获取配置表信息
	///*******************************************************
	void GetTableConfigFromDB(const string& tableName, DBInfo* pInfo);
	///*******************************************************
	/// @name:   CDBTableConfigTool::SetDBConfigPath
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][const ConfigInfo &]configInfo
	/// @brief:  设置配置文件信息
	///*******************************************************
	void SetDBConfigPath(const ConfigInfo& configInfo);
	///*******************************************************
	/// @name:   CDBTableConfigTool::GetDBConfigPath
	/// @author: YaoDi
	/// @return: ConfigInfo
	/// @brief:  获取配置文件信息
	///*******************************************************
	ConfigInfo GetDBConfigPath();

private:
	CppSQLite3DB* m_sqlite3DB;
};

#endif  //_CDBTABLECONFIGTOOL_H
