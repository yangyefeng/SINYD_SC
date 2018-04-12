/////////////////////////////////////////////////////////////////////////////
/// @File name: can_data_provider.h
/// @Author: h
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	Manage Can Data Provider
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef CANDATAPROVIDER_H
#define CANDATAPROVIDER_H

#include "../../Include/EmsasCommon.h"
#include "../../Include/SwDscuMmdef.h"

#include "../data_provider.h"
#include "analog_calibrator.h"

#include <map>
#include <string>
using namespace std;

class CanDataProvider : public DataProvider
{
public:
    CanDataProvider();
    virtual ~CanDataProvider();

    ///*******************************************************
    /// @name:   CanDataProvider::Initialize
    /// @author: h
    /// @return: void
    /// @brief:  初始化函数
    ///*******************************************************
    virtual void Initialize();

    ///*******************************************************
    /// @name:   CanDataProvider::Release
    /// @author: h
    /// @return: void
    /// @brief:  释放资源函数
    ///*******************************************************
    virtual void Release();

    ///*******************************************************
    /// @name:   CanDataProvider::LoadData
    /// @author: h
    /// @return: bool
    /// @param:  [in][bool]bWait
    /// @brief:  加载共享内存中信号量信息
    ///*******************************************************
    virtual bool LoadData(bool bWait);

    ///*******************************************************
    /// @name:   CanDataProvider::GetPackageId
    /// @author: h
    /// @return: INT32
    /// @brief:  获取包号信息
    ///*******************************************************
    virtual INT32 GetPackageId();

	///*******************************************************
	/// @name:   CanDataProvider::GetAnalogData
	/// @author: h
	/// @return: double
	/// @param:  [in][int]iNodeId
	/// @brief:  通过Node id获取模拟量Value Data
	///*******************************************************
    virtual double GetAnalogData(int iNodeId);

	///*******************************************************
	/// @name:   CanDataProvider::GetAnalogRawData
	/// @author: h
	/// @return: std::string
	/// @param:  [in][int]iNodeId
	/// @brief:  通过Node id获取模拟量Value的Raw Data
	///*******************************************************
    virtual string GetAnalogRawData(int iNodeId);

	///*******************************************************
	/// @name:   CanDataProvider::GetDigitalData
	/// @author: h
	/// @return: INT32
	/// @param:  [in][int]iNodeId
	/// @brief:  通过Node id获取数字量Value Data
	///*******************************************************
    virtual INT32 GetDigitalData(int iNodeId);

	///*******************************************************
	/// @name:   CanDataProvider::GetTimestamp
	/// @author: h
	/// @return: SydTime
	/// @brief:  获取一条数据的时间戳
	///*******************************************************
    virtual SydTime GetTimestamp();

	///*******************************************************
	/// @name:   CanDataProvider::GetAnalogDataCount
	/// @author: h
	/// @return: INT32
	/// @brief:  获取模拟量数据的总数
	///*******************************************************
    virtual INT32 GetAnalogDataCount();

	///*******************************************************
	/// @name:   CanDataProvider::GetDigitalDataCount
	/// @author: h
	/// @return: INT32
	/// @brief:  获取数字量数据的总数
	///*******************************************************
    virtual INT32 GetDigitalDataCount();

	///*******************************************************
	/// @name:   CanDataProvider::GetDigitalDataCount
	/// @author: h
	/// @return: INT32
	/// @brief:  获取数字量Channel数据的总数
	///*******************************************************
    virtual INT32 GetDigitalDataChannelCount();

	///*******************************************************
	/// @name:   CanDataProvider::GetAnalogNodeVector
	/// @author: h
	/// @return: void
	/// @param:  [out][vector<INT32> &]vec
	/// @brief:  获取模拟量节点的Vector结构
	///*******************************************************
    virtual void GetAnalogNodeVector(vector<INT32>& vec);

	///*******************************************************
	/// @name:   CanDataProvider::GetDigitalNodeVector
	/// @author: h
	/// @return: void
	/// @param:  [out][vector<INT32> &]vec
	/// @brief:  获取数字量节点的Vector结构
	///*******************************************************
    virtual void GetDigitalNodeVector(vector<INT32>& vec);

	///*******************************************************
	/// @name:   CanDataProvider::IsRunning
	/// @author: h
	/// @return: bool
	/// @brief:  判断设备是否处于运行状态
	///*******************************************************
    virtual bool IsRunning();

	virtual int GetAnaDecimal(int hopid);

    virtual int GetAnaDecimalFromCanID(int canid);

    virtual bool IsSensorCollect();

    virtual int GetDigADDataCount();
    virtual map<int,int> * GetDigitalADData();

    virtual bool IsDigitalAllOpen();

    virtual EN_MIX_MACHINE_TYPE GetMachineType();

protected:
    virtual bool LockSharedMemory();
    virtual bool UnlockSharedMemory();
    virtual const AnalogCalibrator* GetCalibrator(int iCanId);

    virtual bool LoadDataFromMemory(const ST_RECEIVE_CAN_DATA *pData);

    virtual void CalcRunningStatus();

    virtual void LoadFlowMinChackValue();
    virtual bool IsExistFlowOffset(int iAnaId, double dValue, double dLastVal = 0.0);
    virtual void WriteRunning(bool bRunning);
protected:
    /**
      @brief 保存一个模拟节点的数据
      */
    struct ST_ANALOG_NODE{
        INT32 canId;
        INT32 analog;
        double value;

        ST_ANALOG_NODE(){
            canId=0;
            analog=0;
            value=0.0;
        }

        ST_ANALOG_NODE(const ST_ANALOG_NODE& c){
            canId=c.canId;
            analog=c.analog;
            value=c.value;
        }
    };

    /**
      @brief 保存一个数字节点（通道）的数据
      */
    struct ST_DIGITAL_NODE{
        INT32 canId;
        INT8  value;

        ST_DIGITAL_NODE(){
            canId=0;
            value=0;
        }

        ST_DIGITAL_NODE(const ST_DIGITAL_NODE& c){
            canId=c.canId;
            value=c.value;
        }
    };

protected:
    INT32   m_iTranId;
    SydTime m_time;

    INT32 m_iDigitalDataChannelCount;  //一个数字量can节点包含的通道数量
    map<INT32,ST_ANALOG_NODE> *m_pmapAnalogData;    //当前所有模拟量数据
    map<INT32,ST_DIGITAL_NODE> *m_pmapDigitalData;  //当前所有数字量数据
    map<int,int> *m_pmapDigADdata;  //当前所有数字量AD数据

    map<INT32,AnalogCalibrator*> *m_pmapAnalogCalibrators;  //模拟量标定器

    //记录最后的状态（数字量变化与否）
    string  m_strLastStatus;
    SydTime m_timeLastStatus;
    bool    m_bRunning;
    EN_MIX_MACHINE_TYPE m_iMachineType;
    double m_dTimeSpan;   //记录生产时间间隔
    bool m_bIsTankFdata;  //储罐是否记录第一条数据的标志位

    map<int ,double> *m_pmapFlowMixCheckValue; //沥青/储罐485流量计对应最小偏移量
    bool m_bIsExistFlowOverOffset;   //是否存在沥青/储罐流量计超出偏移量
	
	SydTime m_timeOn; //记录所有阀门处在Open状态的时间
    bool m_bTapOpen; //是否所有阀门处在Open状态
};

#endif // CANDATAPROVIDER_H
