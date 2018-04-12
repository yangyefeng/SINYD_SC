/////////////////////////////////////////////////////////////////////////////
/// @File name: data_node.h
/// @Author: h
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	Data Node Base Class
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef DATANODE_H
#define DATANODE_H

#include <vector>
#include "data_provider.h"
using namespace std;

/**
  *数据节点类
  */
class DataNode
{
public:
    DataNode();
    virtual ~DataNode();

	///*******************************************************
	/// @name:   DataNode::Initialize
	/// @author: h
	/// @return: void
	/// @brief:  初始化函数，必要时加载配置文件
	///*******************************************************
    virtual void Initialize(DataProvider *pDataProvider);

	///*******************************************************
	/// @name:   DataNode::Release
	/// @author: h
	/// @return: void
	/// @brief:  释放对象
	///*******************************************************
    virtual void Release();

	///*******************************************************
	/// @name:   DataNode::Start
	/// @author: h
	/// @return: void
	/// @brief:  开始执行
	///*******************************************************
    virtual void Execute();

    ///*******************************************************
    /// @name:   DataNode::GetId
    /// @author: h
    /// @return: INT32
    /// @brief:  取得节点ID。每个DataNode应该有一个唯一的ID。
    ///*******************************************************
    virtual INT32 GetId()=0;
	
    virtual DataProvider* GetDataProvider();

    ///*******************************************************
    /// @name:   DataNode::GetChildren
    /// @author: h
    /// @return: vector<DataNode*>*
    /// @brief:  取得下级节点对象列表
    ///*******************************************************
    virtual vector<DataNode*>* GetChildren() =0 ;

protected:
    DataProvider *m_pDataProvider;
};

#endif // DATANODE_H
