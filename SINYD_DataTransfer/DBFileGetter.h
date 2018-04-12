/////////////////////////////////////////////////////////////////////////////
/// @File name: DBFileGetter.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	DB文件检索类
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef _DB_File_Getter_h_
#define _DB_File_Getter_h_
#include <vector>
#include <string>
#include "ThreadTemplateCrossSystem/Mutex.h"
#include <list>

using namespace std;

struct DBFileInfo
{
	DBFileInfo(const char* pname, const char* pfullname, time_t t)
		: prefix(pname)
		, fullname(pfullname)
		, time(t)
	{}
	DBFileInfo(const string& pname, const string& pfullname, time_t t)
		: prefix(pname)
		, fullname(pfullname)
		, time(t)
	{}
	string prefix;
	string fullname;
	time_t time;
};

class CDBFileGetter
{
public:
	CDBFileGetter();
	virtual ~CDBFileGetter();

	///*******************************************************
	/// @name:   CDBFileGetter::RefreshList
	/// @author: YaoDi
	/// @return: void
	/// @brief:  DB文件列表刷新
	///*******************************************************
	void RefreshList();
	///*******************************************************
	/// @name:   CDBFileGetter::GetDBList
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][const string &]prefix
	/// @param:  [out][list<string> &]listFullName
	/// @brief:  获取所有的DB文件列表
	///*******************************************************
	void GetDBList(const string& prefix, list<string>& listFullName);
	///*******************************************************
	/// @name:   CDBFileGetter::GetDBListAfterBK
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][const string &]prefix
	/// @param:  [in][const string &]breakPointFullname
	/// @param:  [out][list<string> &]listFullName
	/// @brief:  获取断点开始的DB文件列表
	///*******************************************************
	void GetDBListAfterBK(const string& prefix, const string& breakPointFullname, list<string>& listFullName);

private:
	vector<DBFileInfo> _vecDBFile;
	Mutex _M;
};



#endif	//_DB_File_Getter_h_
