/////////////////////////////////////////////////////////////////////////////
/// @File name: JsonRW.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	����JSON��ʽƴд��
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef _JSON_RW_H_
#define _JSON_RW_H_

#include "SYDDataTransferDef.h"
#include "json/json.h"
#include <map>
#include <string>
#include <list>

using namespace std;

class CJsonRW
{
public:
	CJsonRW();
	~CJsonRW();

	///*******************************************************
	/// @name:   CJsonRW::Clear
	/// @author: YaoDi
	/// @return: void
	/// @brief:  ��յ�ǰƴд����
	///*******************************************************
	void Clear();
	///*******************************************************
	/// @name:   CJsonRW::CreateContent
	/// @author: YaoDi
	/// @return: int
	/// @param:  [in][int]contentType
	/// @brief:  ����Content�ڵ㣬���ؽڵ�Index
	///*******************************************************
	int CreateContent(int contentType);
	///*******************************************************
	/// @name:   CJsonRW::CreateContentDataIntoContent
	/// @author: YaoDi
	/// @return: int
	/// @param:  [in][int]ContentIndex
	/// @param:  [in][int]dataType
	/// @brief:  ����ContentData�ڵ㵽Content�ڵ��У����ؽڵ�Index
	///*******************************************************
	int CreateContentDataIntoContent(int ContentIndex, int dataType);
	///*******************************************************
	/// @name:   CJsonRW::CreateContentDataIntoContent
	/// @author: YaoDi
	/// @return: int
	/// @param:  [in][int]ContentIndex
	/// @param:  [in][int]dataType
	/// @param:  [in][const string &]timeStamp
	/// @brief:  ��������ʱ�����ContentData�ڵ㵽Content�ڵ��У����ؽڵ�Index
	///*******************************************************
	int CreateContentDataIntoContent(int ContentIndex, int dataType, const string& timeStamp );
	///*******************************************************
	/// @name:   CJsonRW::CreateDataIntoContentData
	/// @author: YaoDi
	/// @return: int
	/// @param:  [in][int]ContentDataIndex
	/// @brief:  ����Data�ڵ㵽ContentData�ڵ��У����ؽڵ�Index
	///*******************************************************
	int CreateDataIntoContentData(int ContentDataIndex);
	///*******************************************************
	/// @name:   CJsonRW::AddDataInfoIntoData
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][int]DataIndex
	/// @param:  [in][const]map<string
	/// @param:  [in][T> &]mapDataProps
	/// @brief:  ��Data�ڵ����������
	///*******************************************************
	template<typename T>
	void AddDataInfoIntoData(int DataIndex, const map<string, T>& mapDataProps)
	{
        typename map<string, T>::const_iterator itEnd = mapDataProps.end();
        for (typename map<string, T>::const_iterator it = mapDataProps.begin(); it != itEnd; ++it)
		{
			m_vecData[DataIndex][it->first] = it->second;
		}
	}
	///*******************************************************
	/// @name:   CJsonRW::AddDataInfoIntoData
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][int]DataIndex
	/// @param:  [in][const string &]name
	/// @param:  [in][const T &]value
	/// @brief:  ��Data�ڵ����������
	///*******************************************************
	template<typename T>
	void AddDataInfoIntoData(int DataIndex, const string& name, const T& value)
	{
		m_vecData[DataIndex][name] = value;
	}
	///*******************************************************
	/// @name:   CJsonRW::AddDataInfoIntoData
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][int]DataIndex
	/// @param:  [in][const string &]name
	/// @param:  [in][const list<T> &]values
	/// @brief:  ��Data�ڵ����������
	///*******************************************************
	template<typename T>
	void AddDataInfoIntoData(int DataIndex, const string& name, const list<T>& values)
	{
        typename list<T>::const_iterator itEnd = values.end();
        for (typename list<T>::const_iterator it = values.begin(); it != itEnd; ++it)
		{
			m_vecData[DataIndex][name].append(*it);
		}
	}
	///*******************************************************
	/// @name:   CJsonRW::GetFinal
	/// @author: YaoDi
	/// @return: const string*
	/// @brief:  ��ȡ����ƴд���
	///*******************************************************
	const string* GetFinal();

	///*******************************************************
	/// @name:   CJsonRW::initSourceInfo
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][const SourceInfo &]info
	/// @brief:  ��ʼ��JSONЭ���SourceInfo
	///*******************************************************
	static void initSourceInfo(const SourceInfo& info);

private:
	static Json::Value s_source;

	Json::Value m_root;
	vector<Json::Value> m_vecContent;
    map<int, vector<int> > m_ContentChild;
	vector<Json::Value> m_vecContentData;
    map<int, vector<int> > m_ContentDataChild;
	vector<Json::Value> m_vecData;
	string m_finalJson;
};


#endif	//_JSON_RW_H_
