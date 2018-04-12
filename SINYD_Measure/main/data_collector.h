/////////////////////////////////////////////////////////////////////////////
/// @File name: data_collector.h
/// @Author: h
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	Measure Node Data Collector Class
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef DATACOLLECTOR_H
#define DATACOLLECTOR_H

#include "data_provider.h"
#include "data_node.h"

class DataCollector
{
public:
    DataCollector();

    virtual ~DataCollector();

    ///*******************************************************
    /// @name:   DataCollector::Initialize
    /// @author: h
    /// @return: void
    /// @brief:  初始化函数，必要时加载配置文件
    ///*******************************************************
    void Initialize();

    ///*******************************************************
    /// @name:   DataCollector::Release
    /// @author: h
    /// @return: void
    /// @brief:  释放对象
    ///*******************************************************
    virtual void Release();

    ///*******************************************************
    /// @name:   DataCollector::Start
    /// @author: h
    /// @return: void
    /// @brief:  开始执行
    ///*******************************************************
    void Start();

    ///*******************************************************
    /// @name:   DataCollector::Stop
    /// @author: h
    /// @return: void
    /// @brief:  终止执行
    ///*******************************************************
    void Stop();

	bool IsRunning();
#if _WIN32
	void SetRawData(const string &srcdata);
#endif
protected:
    DataNode* LoadSysDataNode(DataProvider *pProvider);
    DataNode* LoadRawRecorder(DataProvider *pProvider);

protected:
    DataProvider *m_pDataProvider;

    vector<DataNode*> m_vecDataNodes;
};

#endif // DATACOLLECTOR_H
