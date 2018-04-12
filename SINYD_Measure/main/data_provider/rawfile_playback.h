/////////////////////////////////////////////////////////////////////////////
/// @File name: rawfile_playback.h
/// @Author: h
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	File Raw PlayBack Class
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef RAWFILE_PLAYBACK_H
#define RAWFILE_PLAYBACK_H

#include <fstream>
#include <map>
#include <vector>

#include "../data_provider.h"

class Rawfile_Playback: public DataProvider
{
public:
    Rawfile_Playback(const char *szRawFilename);


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

protected:
    struct ST_NODE_ITEM{
        INT32 nodeId;
        INT32 nodeType;

        ST_NODE_ITEM(){
            nodeId=0;
            nodeType=0;
        }
        ST_NODE_ITEM(const ST_NODE_ITEM &a){
            nodeId=a.nodeId;
            nodeType=a.nodeType;
        }
    };

protected :

    string m_strRawFilename;
    ifstream m_ifRawStream;


    vector<ST_NODE_ITEM> m_vecNodes;
    vector<INT32> m_vecValues;

    SydTime          m_timestamp;
    INT32            m_iPackageId;
};

#endif // RAWFILE_PLAYBACK_H
