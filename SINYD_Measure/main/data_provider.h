/////////////////////////////////////////////////////////////////////////////
/// @File name: data_provider.h
/// @Author: h
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	Data Provider Base Class
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef DATAPROVIDER_H
#define DATAPROVIDER_H

#include <vector>
using namespace std;

#include "../Include/SwDscuMmdef.h"
#include "../Tool/syd_time.h"
#include "MeasureDef.h"

class DataProvider
{
public:
    DataProvider();

    ///*******************************************************
    /// @name:   DataProvider::Initialize
    /// @author: h
    /// @return: void
    /// @brief:  初始化
    ///*******************************************************
    virtual void Initialize()=0;

    ///*******************************************************
    /// @name:   DataProvider::Release
    /// @author: h
    /// @return: void
    /// @brief:  释放对象
    ///*******************************************************
    virtual void Release()=0;

    ///*******************************************************
    /// @name:   DataProvider::LoadData
    /// @author: h
    /// @return: bool
    /// @param:  [in][bool]bWait
    /// @brief:  加载最新数据
    ///*******************************************************
    virtual bool LoadData(bool bWait)=0;

    ///*******************************************************
    /// @name:   DataProvider::GetPackageId
    /// @author: h
    /// @return: INT32
    /// @brief: 获取当前数据包的ID 
    ///*******************************************************
    virtual INT32 GetPackageId()=0;

    ///*******************************************************
    /// @name:   DataProvider::GetTimestamp
    /// @author: h
    /// @return: SydTime
    /// @brief:  获取当前数据的时间戳
    ///*******************************************************
    virtual SydTime GetTimestamp()=0;

    ///*******************************************************
    /// @name:   DataProvider::GetAnalogData
    /// @author: h
    /// @return: double
    /// @param:  [in][int]iNodeId
    /// @brief:  获取一个模拟量数据
    ///*******************************************************
    virtual double GetAnalogData(int iNodeId)=0;

    ///*******************************************************
    /// @name:   DataProvider::GetAnalogRawData
    /// @author: h
    /// @return: std::string
    /// @param:  [in][int]iNodeId
    /// @brief:  获取一个模拟量的原始数据
    ///*******************************************************
    virtual string GetAnalogRawData(int iNodeId)=0;

    ///*******************************************************
    /// @name:   DataProvider::GetDigitalData
    /// @author: h
    /// @return: INT32
    /// @param:  [in][int]iNodeId
    /// @brief:  获取一个数字量数据
    ///*******************************************************
    virtual INT32 GetDigitalData(int iNodeId)=0;

    ///*******************************************************
    /// @name:   DataProvider::GetDigitalDataCount
    /// @author: h
    /// @return: INT32
    /// @brief:  获取数字量总数
    ///*******************************************************
    virtual INT32 GetDigitalDataCount()=0;

    ///*******************************************************
    /// @name:   DataProvider::GetAnalogDataCount
    /// @author: h
    /// @return: INT32
    /// @brief:  获取模拟量总数
    ///*******************************************************
    virtual INT32 GetAnalogDataCount()=0;

    ///*******************************************************
    /// @name:   DataProvider::GetAnalogNodeVector
    /// @author: h
    /// @return: void
    /// @param:  [in][vector<INT32> &]vec
    /// @brief:  取得模拟量的节点ID的列表
    ///*******************************************************
    virtual void GetAnalogNodeVector(vector<INT32> &vec)=0 ;

    ///*******************************************************
    /// @name:   DataProvider::GetDigitalNodeVector
    /// @author: h
    /// @return: void
    /// @param:  [in][vector<INT32> &]vec
    /// @brief:  取得数字量的节点ID的列表
    ///*******************************************************
    virtual void GetDigitalNodeVector(vector<INT32> &vec)=0;

    ///*******************************************************
    /// @name:   DataProvider::IsRunning
    /// @author: h
    /// @return: bool
    /// @brief:  判断设备是否处于运行状态
    ///*******************************************************
    virtual bool IsRunning()=0;

	virtual int GetAnaDecimal(int canid) = 0;

    virtual bool IsSensorCollect() = 0;

    virtual bool IsDigitalAllOpen() = 0;

    virtual EN_MIX_MACHINE_TYPE GetMachineType() = 0;
};

#endif // DATAPROVIDER_H
