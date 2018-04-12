#pragma once
#include "TransDataReader.h"
#include "FlumeDataRW.h"

class CFlumeDBDataReader : public CDBDataReader
{
public:
	CFlumeDBDataReader(const char* szName);
	virtual ~CFlumeDBDataReader();

	virtual void next();

protected:
    string GetString(int i);
	const string* m_curData;
	vector<int> m_curDataPKs;
	CFlumeDataRW m_data_rw;
	string m_DATA_TYPE;
	int m_CountPerBody;
};

class CRawDataReader_Flume : public CFlumeDBDataReader
{
public:
	CRawDataReader_Flume();
	virtual ~CRawDataReader_Flume();

	virtual const string* getData();
	virtual void next();

protected:
	virtual void AddConfigInfoToData();

protected:
	string m_curDataWithConfig;
};


class CMeasureDataReader_Flume : public CFlumeDBDataReader
{
public:
	CMeasureDataReader_Flume();
	virtual ~CMeasureDataReader_Flume();

	virtual const string* getData();
	virtual void next();

protected:

};

class CDetailDataReader_Flume : public CFlumeDBDataReader
{
public:
	CDetailDataReader_Flume();
	virtual ~CDetailDataReader_Flume();

	virtual const string* getData();
	virtual void next();

protected:

};

