/////////////////////////////////////////////////////////////////////////////
/// @File name: hopper_node.h
/// @Author: h
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	Hopper Node Base Class
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef HOPPER_NODE_H
#define HOPPER_NODE_H

#include <sys/types.h>
#include <string>
#include <map>
#include <list>
using namespace std;

#include "../data_node.h"
#include "hopper_product.h"

/**
  @brief 料仓类。用于描述一个具有放料阀的结构体。
  都应实际情况，料仓、中途斗都属于这一类型。
  */
class HopperNode : public DataNode
{
public:
    HopperNode();
    virtual ~HopperNode();

    ///*******************************************************
    /// @name:   HopperNode::Initialize
    /// @author: h
    /// @return: void
    /// @param:  [in][DataProvider *]pDataProvider
    /// @brief:  初始化函数
    ///*******************************************************
    virtual void Initialize(DataProvider *pDataProvider);

    ///*******************************************************
    /// @name:   HopperNode::Execute
    /// @author: h
    /// @return: void
    /// @brief:  开始执行
    ///*******************************************************
    virtual void Execute();

    ///*******************************************************
    /// @name:   HopperNode::Release
    /// @author: h
    /// @return: void
    /// @brief:  释放对象
    ///*******************************************************
    virtual void Release();

	///*******************************************************
    /// @name:   HopperNode::GetId
    /// @author: h
    /// @return: INT32
    /// @brief:  获取节点ID
    ///*******************************************************
    virtual INT32 GetId();

    ///*******************************************************
    /// @name:   HopperNode::GetChildren
    /// @author: h
    /// @return: vector<DataNode*>*
    /// @brief:  获取所有子节点的vector指针
    ///*******************************************************
    virtual vector<DataNode*>* GetChildren() ;

public:
    ///*******************************************************
    /// @name:   HopperNode::AddChild
    /// @author: h
    /// @return: void
    /// @param:  [in][HopperNode *]pNode
    /// @brief:  增加一个子节点
    ///*******************************************************
    virtual void AddChild(HopperNode* pNode);

    ///*******************************************************
    /// @name:   HopperNode::FindChild
    /// @author: h
    /// @return: HopperNode*
    /// @param:  [in][INT32]id
    /// @brief:  根据ID查找子对象
    ///*******************************************************
    virtual HopperNode* FindChild(INT32 id);

    ///*******************************************************
    /// @name:   HopperNode::AddTap
    /// @author: h
    /// @return: void
    /// @param:  [in][int]iTapId
    /// @brief:  增加一个开关量，的增加时进行数据有效性检查,并防止重复加入
    ///*******************************************************
    virtual void AddTap(int iTapId);

    ///*******************************************************
    /// @name:   HopperNode::GetTaps
    /// @author: h
    /// @return: vector<INT32>*
    /// @brief:  获取所有开关量的vector指针
    ///*******************************************************
    virtual vector<INT32>* GetTaps() ;

    ///*******************************************************
    /// @name:   HopperNode::GetData
    /// @author: h
    /// @return: INT32
    /// @brief:  获取Node Data值
    ///*******************************************************
    virtual INT32 GetData();

    ///*******************************************************
    /// @name:   HopperNode::GetName
    /// @author: h
    /// @return: std::string
    /// @brief:  获取Node名称
    ///*******************************************************
    virtual string GetName();

	///*******************************************************
	/// @name:   HopperNode::GetType
	/// @author: h
	/// @return: int
	/// @brief:  获取Node类型（秤、计算值、设定值、温度）
	///*******************************************************
	virtual int GetType();

    ///*******************************************************
    /// @name:   HopperNode::SetId
    /// @author: h
    /// @return: void
    /// @param:  [in][INT32]iNewId
    /// @brief:  设置Node的Id值
    ///*******************************************************
    virtual void SetId(INT32 iNewId);

    ///*******************************************************
    /// @name:   HopperNode::SetData
    /// @author: h
    /// @return: void
    /// @param:  [in][INT32]iNewData
    /// @brief:  设置Node的Data值
    ///*******************************************************
    virtual void SetData(INT32 iNewData);

    ///*******************************************************
    /// @name:   HopperNode::SetName
    /// @author: h
    /// @return: void
    /// @param:  [in][const string &]strNewName
    /// @brief:  设置Node的名称
    ///*******************************************************
    virtual void SetName(const string& strNewName);

	///*******************************************************
	/// @name:   HopperNode::SetType
	/// @author: h
	/// @return: void
	/// @param:  [in][int]type
	/// @brief:  设置Node的类型（秤、计算值、设定值、温度）
	///*******************************************************
	virtual void SetType(int type);

	///*******************************************************
	/// @name:   HopperNode::SetIsAddProduct
	/// @author: h
	/// @return: void
	/// @param:  [in/out/inout][bool]addp
	/// @brief:  设置是否添加了产品对象
	///*******************************************************
	virtual void SetIsAddProduct(bool addp);

    virtual void SetOpenDelay(int delay);
    virtual int GetOpenDelay();
    virtual void SetCloseDelay(int delay);
    virtual int GetCloseDelay();
	
    ///*******************************************************
    /// @name:   HopperNode::GetProduct
    /// @author: h
    /// @return: HopperProduct*
    /// @brief:  取得当前产品对象的指针
    ///*******************************************************
    virtual HopperProduct* GetProduct();

    ///*******************************************************
    /// @name:   HopperNode::ToString
    /// @author: h
    /// @return: std::string
    /// @brief:  已字符形式表示节点属性值
    ///*******************************************************
    virtual string ToString();

    ///*******************************************************
    /// @name:   HopperNode::DumpTree
    /// @author: h
    /// @return: void
    /// @param:  [in][INT32]iLevel
    /// @brief:  打印树型结构到控制台
    ///*******************************************************
    virtual void DumpTree(INT32 iLevel=0);

    ///*******************************************************
    /// @name:   HopperNode::IsInputting
    /// @author: h
    /// @return: bool
    /// @brief:  判断当前是否处于进料的状态
    ///*******************************************************
    virtual bool IsInputting();

    ///*******************************************************
    /// @name:   HopperNode::IsOutputting
    /// @author: h
    /// @return: bool
    /// @brief:  判断当前是否处于放料的状态
    ///*******************************************************
    virtual bool IsOutputting();
protected:
    ///*******************************************************
    /// @name:   HopperNode::RefreshStatus
    /// @author: h
    /// @return: bool：true->料仓状态发生了变化， false->料仓状态未发生变化
    /// @brief:  更新当前状态
    ///*******************************************************
    virtual bool RefreshStatus();

    ///*******************************************************
    /// @name:   HopperNode::AddStatusObserver
    /// @author: h
    /// @return: void
    /// @param:  [in][HopperNode *]pHopper
    /// @brief:  设置观察者
    ///*******************************************************
    virtual void AddStatusObserver(HopperNode *pHopper);

    ///*******************************************************
    /// @name:   HopperNode::RemoveStatusObserver
    /// @author: h
    /// @return: void
    /// @param:  [in][HopperNode *]pHopper
    /// @brief:  移除观察者
    ///*******************************************************
    virtual void RemoveStatusObserver(HopperNode *pHopper);

	virtual bool AddBKTProductValue(HopperNode *pHopper,bool bFind = false);

protected:
    /**
      @brief 触发状态变化事件
      */
    virtual void FireInputStart();
    virtual void FireInputDone();
    virtual void FireOutputStart();
    virtual void FireOutputDone();

protected:
    /**
      @brief 状态变化事件的响应函数(监听子节点的状态变化)
      */
    virtual void OnInputStart(HopperNode *pHopper);
    virtual void OnInputDone(HopperNode *pHopper);
    virtual void OnOutputStart(HopperNode *pHopper);
    virtual void OnOutputDone(HopperNode *pHopper);

public:
    /**
      @brief 记录当前料仓的生产状态和开始时间
      */
    struct ST_HOPPER_STATUS{
        SydTime timestamp;  //状态开始时间
        bool    bInputting; //
        bool    bOutputing; //
    };
	///*******************************************************
	/// @name:   HopperNode::GetHopStatus
	/// @author: h
	/// @return: HopperNode::ST_HOPPER_STATUS *
	/// @brief:  获取前料仓的生产状态
	///*******************************************************
	HopperNode::ST_HOPPER_STATUS *GetHopStatus();

	void InitInAndOutOpenFlg();
	void SetInAndOutOpenFlg(bool bFlg);
protected:
    //料仓基础信息
    INT32  m_iId;        //节点ID
    string m_strName;   //节点名称
    INT32  m_iData;     //节点自定义数据（应用数据）
	int m_itype;        //节点类型
	bool m_isaddproduct;
    int m_iOpenDelay;
    int m_iCloseDelay;

    vector<INT32>       m_vecTaps;    //放料阀的ID（canID+通道号）
    vector<DataNode*>   m_vecChildren;  //子节点集合

    //阀门实时数据
    map<INT32,INT32>    m_mapTapValues;   //放料阀最后的状态

    //料仓状态
    ST_HOPPER_STATUS    m_stStatus;             //当前生产状态
    vector<HopperNode*> m_vecStatusObserver;      //状态观察者

    //生产信息
    HopperProduct       m_product;
	bool m_bInAndOutOpen;

};

#endif // HOPPER_NODE_H
