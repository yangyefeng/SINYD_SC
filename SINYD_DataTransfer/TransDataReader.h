/////////////////////////////////////////////////////////////////////////////
/// @File name: TransDataReader.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	传输模块数据读取
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef _Trans_Data_Reader_h_
#define _Trans_Data_Reader_h_

#include <string>
#include <map>
#include "DBFileGetter.h"
#include "DataBreakPointStorage.h"
#include "JsonRW.h"
#include "DBModule.h"


using namespace std;

class ITransDataReader
{
public:
	virtual ~ITransDataReader(){};

	///*******************************************************
	/// @name:   ITransDataReader::getData
	/// @author: YaoDi
	/// @return: const string*
	/// @brief:  获取当前传输记录JSON格式字符串
	///*******************************************************
	virtual const string* getData() = 0;	//取得当前记录的JSON表示
	///*******************************************************
	/// @name:   ITransDataReader::next
	/// @author: YaoDi
	/// @return: void
	/// @brief:  移动传输记录光标到吓一条记录
	///*******************************************************
	virtual void next() = 0;		//将记录光标移动到下一行
};

class CDBDataReader : public ITransDataReader{
public:
	CDBDataReader(const char* szName); //szName: 用于定位数据库文件的路径、记录读取的位置
	virtual ~CDBDataReader();
	/**
	 * @brief override
	 */
	virtual void next();

protected:
 	/**
	 * @brief 初始化数据库连接，为读取第一条数据做准备
	 */
	virtual void Init( );

	/**
	 * @brief 释放资源
	 */
	virtual void Release();

protected:
	/**
	 * @brief 通过列明查找对应数据在记录中的位置(index)
	 */
	int static FindField(const list<FieldInfo>& fields, const char* szFieldName);

protected:
	string m_strName;

	CDBFileGetter m_fileGetter;
	list<string> m_listDB;

	IDBOperator* m_pDBO;
	IDBQueryRow* m_pQuery;

	CDataBreakPointStorage m_bpStorage;
	BreakPoint m_bp;
};


/**
 * 原始数据的读取类
 */
class CRawDataReader : public CDBDataReader
{
public:
	CRawDataReader();
	virtual ~CRawDataReader();

	virtual const string* getData();
	virtual void next();

private:
	struct ST_BUCKET;
	struct BUCKET_COMPARE_FUNC;
	void AddConfigInfoIntoJson(CJsonRW& jsonRW);
	int GetHopperCount(const char * szConfigFile);
	int GetBucketCount(const char * szConfigFile);
	void LoadBucket(list<ST_BUCKET>& Buckets, const char* szConfigFile);
	void AddSBSTank(int iContextIndex, CJsonRW& jsonRW, string szConfigFile);
	void AddSBSConfigInfoIntoJson(int iContextIndex,CJsonRW& jsonRW, string szConfigFile);
	void AddTankConfigInfoIntoJson(int iContextIndex, CJsonRW& jsonRW, string szConfigFile);
private:
	CJsonRW m_json;

	const string* m_curData;
	int m_curGetCount;
};
//
//class CMeasureDataReader : public ITransDataReader
//{
//public:
//	CMeasureDataReader();
//	virtual ~CMeasureDataReader();
//
//	virtual const char* getData(/*out*/long& outLen);
//	virtual void next();
//
//private:
//	void InitDBO();
//    void InitColMap();
//	void CreateCurData();
//
//	void queryDataRecurse();
//private:
//	const char* m_curData;
//	long m_curLen;
//	CJsonRW m_jsonRW;
//	CDataBreakPointStorage m_bpStorage;
//	CDBFileGetter m_fileGetter;
//	list<string> m_listDB;
//	BreakPoint m_bp;
//	bool m_isFirstGet;
//	IDBOperator* m_pDBO;
//	IDBQueryRow* m_pQuery;
//
//
//    map<int, const char*> m_RealCol;
//    map<int, const char*> m_DesignCol;
//    bool m_bDesigned; //是否有设定值
//
//	const static string m_dataName;
//};



class CMeasureDataReader : public CDBDataReader
{
public:
	CMeasureDataReader();
	virtual ~CMeasureDataReader();

	virtual const string* getData();
	virtual void next();

private:
    void InitColMap();
	struct _R_C_COMPARE_FUNC;

private:
	CJsonRW m_json;

	const string* m_curData;

	typedef map<int, const char*> ColMapType;
	ColMapType m_RealCol;
	ColMapType m_DesignCol;
    bool m_bDesigned; //是否有设定值
	int m_curGetCount;
	bool m_bIsColMapInited;
};

class CDetailDataReader : public CDBDataReader
{
public:
	CDetailDataReader();
	virtual ~CDetailDataReader();

	virtual const string* getData();
	virtual void next();

private:
	CJsonRW m_json;

	string m_sLastPkgId;

	const string* m_curData;
	int m_curGetCount;
};

#endif	//	_Trans_Data_Reader_h_

