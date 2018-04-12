/////////////////////////////////////////////////////////////////////////////
/// @File name: sc_data_provider.h
/// @Author: h
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	Screen Collection Data Provider
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef SCDATAPROVIDER_H
#define SCDATAPROVIDER_H

#include "../../Include/EmsasCommon.h"
#include "../../Include/SwDscuMmdef.h"

#include "../data_provider.h"
#include "analog_calibrator.h"
#include "json\json.h"
#include <map>
#include <string>
using namespace std;

class ScDataProvider : public DataProvider
{
public:
	ScDataProvider();
    virtual ~ScDataProvider();

    ///*******************************************************
    /// @name:   ScDataProvider::Initialize
    /// @author: h
    /// @return: void
    /// @brief:  初始化函数
    ///*******************************************************
    virtual void Initialize();

    ///*******************************************************
    /// @name:   ScDataProvider::Release
    /// @author: h
    /// @return: void
    /// @brief:  释放资源函数
    ///*******************************************************
    virtual void Release();

    /**
      @overload
      */
    ///*******************************************************
    /// @name:   ScDataProvider::LoadData
    /// @author: h
    /// @return: bool
    /// @param:  [in][bool]bWait
    /// @brief:  加载一帧数据函数
    ///*******************************************************
    virtual bool LoadData(bool bWait);
	
    ///*******************************************************
    /// @name:   ScDataProvider::GetPackageId
    /// @author: h
    /// @return: INT32
    /// @brief:  获取一帧数据的包号
    ///*******************************************************
    virtual INT32 GetPackageId();

    ///*******************************************************
    /// @name:   ScDataProvider::GetAnalogData
    /// @author: h
    /// @return: double
    /// @param:  [in][int]iNodeId
    /// @brief:  通过Node id获取模拟量Value Data
    ///*******************************************************
    virtual double GetAnalogData(int iNodeId);

    ///*******************************************************
    /// @name:   ScDataProvider::GetAnalogRawData
    /// @author: h
    /// @return: std::string
    /// @param:  [in][int]iNodeId
    /// @brief:  通过Node id获取模拟量Value的Raw Data
    ///*******************************************************
    virtual string GetAnalogRawData(int iNodeId);

    ///*******************************************************
    /// @name:   ScDataProvider::GetDigitalData
    /// @author: h
    /// @return: INT32
    /// @param:  [in][int]iNodeId
    /// @brief:  通过Node id获取数字量Value Data
    ///*******************************************************
    virtual INT32 GetDigitalData(int iNodeId);

    ///*******************************************************
    /// @name:   ScDataProvider::GetTimestamp
    /// @author: h
    /// @return: SydTime
    /// @brief:  获取一帧数据的时间戳
    ///*******************************************************
    virtual SydTime GetTimestamp();

    ///*******************************************************
    /// @name:   ScDataProvider::GetAnalogDataCount
    /// @author: h
    /// @return: INT32
    /// @brief:  获取模拟量数据的总数
    ///*******************************************************
    virtual INT32 GetAnalogDataCount();

    ///*******************************************************
    /// @name:   ScDataProvider::GetDigitalDataCount
    /// @author: h
    /// @return: INT32
    /// @brief:  获取数字量数据的总数
    ///*******************************************************
    virtual INT32 GetDigitalDataCount();

    ///*******************************************************
    /// @name:   ScDataProvider::GetAnalogNodeVector
    /// @author: h
    /// @return: void
    /// @param:  [out][vector<INT32> &]vec
    /// @brief:  获取模拟量节点的Vector结构
    ///*******************************************************
    virtual void GetAnalogNodeVector(vector<INT32>& vec);

    ///*******************************************************
    /// @name:   ScDataProvider::GetDigitalNodeVector
    /// @author: h
    /// @return: void
    /// @param:  [out][vector<INT32> &]vec
    /// @brief:  获取数字量节点的Vector结构
    ///*******************************************************
    virtual void GetDigitalNodeVector(vector<INT32>& vec);

    ///*******************************************************
    /// @name:   ScDataProvider::IsRunning
    /// @author: h
    /// @return: bool
    /// @brief:  判断设备是否处于运行状态
    ///*******************************************************
    virtual bool IsRunning();

	virtual int GetAnaDecimal(int canid);

	virtual bool IsSensorCollect();

	virtual bool IsDigitalAllOpen();

	virtual EN_MIX_MACHINE_TYPE GetMachineType();
	///*******************************************************
	/// @name:   ScDataProvider::SetRawData
	/// @author: h
	/// @return: void
	/// @param:  [in][const string &]rawdata
	/// @brief:  设置一个信号量数据
	///*******************************************************
	virtual void SetRawData(const string &rawdata);

private:
	virtual void ReadJson(map<string,string> &outrew, const string &rawdata);
	Json::Value GetJsonValue(const std::string &strContent, std::string& strKey);
protected:
    
    /**
      @brief 保存一个数字节点（通道）的数据
      */
    struct ST_DATA_NODE {
        int sensorId;
		double value;
		string raw_value;

		ST_DATA_NODE(){
			sensorId =0;
			value =0.0;
			raw_value = "";
        }

		ST_DATA_NODE(const ST_DATA_NODE& c){
			sensorId =c.sensorId;
            value=c.value;
			raw_value = c.raw_value;
        }
    };

	struct ST_SC_RAW_DATA {
		int ipacket_id;
		int isensor_id;
		int itype;
		string str_value;
		string str_time;
	};

protected:
    INT32   m_iTranId;
    SydTime m_time;

    //INT32 m_iDigitalDataChannelCount;  //一个数字量can节点包含的通道数量
    map<INT32, ST_DATA_NODE> *m_pmapAnalogData;    //当前所有模拟量数据
    map<INT32, int> *m_pmapDigitalData;  //当前所有数字量数据

    //map<INT32,AnalogCalibrator*> *m_pmapAnalogCalibrators;  //模拟量标定器

    //记录最后的状态（数字量变化与否）
    string  m_strLastStatus;
    SydTime m_timeLastStatus;
    bool    m_bRunning;
	EN_MIX_MACHINE_TYPE m_iMachineType;
};

#endif // SCDATAPROVIDER_H
