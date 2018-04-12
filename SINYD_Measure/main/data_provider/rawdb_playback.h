/////////////////////////////////////////////////////////////////////////////
/// @File name: rawdb_playback.h
/// @Author: h
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	DB Raw PlayBack Class
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef RAWDB_PLAYBACK_H
#define RAWDB_PLAYBACK_H

#include <fstream>
#include <map>
#include <vector>
#include "DBModule.h"
#include "../data_provider.h"

class Rawdb_Playback: public DataProvider
{
public:
	Rawdb_Playback(const char *szRawFilename);

	virtual ~Rawdb_Playback();
    /**
      @overload
      */
    virtual void Initialize();

    /**
      @overload
      */
    virtual void Release();

    /**
      @overload
      */
    virtual bool LoadData(bool bWait);

    /**
      *@overload
      */
    virtual INT32 GetPackageId();

    /**
      @overload
      */
    virtual double GetAnalogData(int iNodeId);

    virtual string GetAnalogRawData(int iNodeId);

    /**
      @overload
      */
    virtual INT32 GetDigitalData(int iNodeId);

    /**
      @overload
      */
    virtual SydTime GetTimestamp();

    /**
      @overload
      */
    virtual INT32 GetAnalogDataCount();

    /**
      @overload
      */
    virtual INT32 GetDigitalDataCount();

    /**
      @overload
      */
    virtual INT32 GetDigitalDataChannelCount();

    /**
      @overload
      */
    virtual void GetAnalogNodeVector(vector<INT32>& vec);

    /**
      @overload
      */
    virtual void GetDigitalNodeVector(vector<INT32>& vec);

    /**
      @overload
      */
    virtual bool IsRunning();

	virtual int GetAnaDecimal(int canid);

    virtual bool IsSensorCollect();

    virtual bool IsDigitalAllOpen();

    virtual EN_MIX_MACHINE_TYPE GetMachineType();

protected:
	int FindField(const list<FieldInfo>& fields, const char* szFieldName);
	struct ST_DATA_PBNODE {
		double value;
		string raw_value;

		ST_DATA_PBNODE() {
			value = 0.0;
			raw_value = "";
		}

		ST_DATA_PBNODE(const ST_DATA_PBNODE& c) {
			value = c.value;
			raw_value = c.raw_value;
		}
	};
protected :

    map<int, ST_DATA_PBNODE> *m_pmapAnaDatas;
	map<int, int> *m_pmapDigDatas;

	string  m_strRawFilename;
    SydTime  m_timestamp;
    int  m_iPackageId;
	bool m_bRunning;

	IDBOperator* m_pDBO;
	IDBQueryRow* m_pQuery;
	int m_iPAK;
    SydTime m_timeOn;
    bool m_bTapOpen;
};

#endif // RAWDB_PLAYBACK_H
