/////////////////////////////////////////////////////////////////////////////
/// @File name: DataBreakPointStorage.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	传输断点保存和读取类
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef _DATA_BREAK_POINT_STORAGE_H_
#define _DATA_BREAK_POINT_STORAGE_H_

#include <string>
#include "CppSqlite3/CppSQLite3.h"
using namespace std;

#include "../TinyXML2/tinyxml2.h"
#include "ThreadTemplateCrossSystem/Mutex.h"


//目前使用固定结构，之后应该进行改造，将信息节点变为map形式的动态节点
//在Storage中通过遍历map进行断点属性写入
struct BreakPoint
{
	string DataFileName;
	int DataID;
};


class CDataBreakPointStorage
{
public:
	CDataBreakPointStorage();
	virtual ~CDataBreakPointStorage();

	///*******************************************************
	/// @name:   CDataBreakPointStorage::GetBreakPoint
	/// @author: YaoDi
	/// @return: BreakPoint
	/// @param:  [in][const string &]dataName
	/// @brief:  
	///*******************************************************
	BreakPoint GetBreakPoint(const string& dataName);
	///*******************************************************
	/// @name:   CDataBreakPointStorage::SaveBreakPoint
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][const string &]dataName
	/// @param:  [in][const BreakPoint &]breakPoint
	/// @brief:  
	///*******************************************************
	void SaveBreakPoint(const string& dataName, const BreakPoint& breakPoint);

private:
	static void Open();
	static void Save();

    static void CreateBreakPointDB(const char *fileName);

private:
	static tinyxml2::XMLDocument s_XMLDoc;
	static tinyxml2::XMLElement* s_FirstElement;
	static bool s_docOpened;
	static Mutex s_M;
    static CppSQLite3DB m_sqldb;
    static bool b_IsOpenDB;
};


#endif	//_DATA_BREAK_POINT_STORAGE_H_

