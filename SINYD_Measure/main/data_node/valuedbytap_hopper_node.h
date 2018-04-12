/////////////////////////////////////////////////////////////////////////////
/// @File name: valuedbytap_hopper_node.h
/// @Author: h
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/07/28
/// @Description:
/// 	Tap Tap Measure Hopper Node
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef VALUED_BY_TAP_HOPPER_NODE_H
#define VALUED_BY_TAP_HOPPER_NODE_H

#include "hopper_node.h"
#include "../../Tool/syd_time.h"
#include "../../Data/Include/Operation_conf.h"
#include "../../Data/data_node_valuemgr.h"

/**
  @brief 可计量的料仓。计量方法使用阀门开关进行计算。
  */
class ValuedByTapHopperNode : public HopperNode
{
public:
	ValuedByTapHopperNode();

    ///*******************************************************
    /// @name:   TapValuedHopperNode::Initialize
    /// @author: h
    /// @return: void
    /// @param:  [in][DataProvider *]pDataProvider
    /// @brief:  初始化函数
    ///*******************************************************
    virtual void Initialize(DataProvider *pDataProvider);
    
    ///*******************************************************
    /// @name:   TapValuedHopperNode::Execute
    /// @author: h
    /// @return: void
    /// @brief:  执行料仓计量动作开始函数
    ///*******************************************************
    virtual void Execute();

	///*******************************************************
	/// @name:   TapValuedHopperNode::Release
	/// @author: h
	/// @return: void
	/// @brief:  释放对象
	///*******************************************************
	virtual void Release();

    ///*******************************************************
    /// @name:   TapValuedHopperNode::RefreshStatus
    /// @author: h
    /// @return: bool
    /// @brief:  更新料仓节点状态函数
    ///*******************************************************
    virtual bool RefreshStatus();

    ///*******************************************************
    /// @name:   TapValuedHopperNode::ToString
    /// @author: h
    /// @return: std::string
    /// @brief: 已字符形式表示当前料仓节点属性信息 
    ///*******************************************************
    virtual string ToString();

	///*******************************************************
	/// @name:   TapValuedHopperNode::SetLigninRatio
	/// @author: h
	/// @return: void
	/// @brief:  设置木质素的系数
	///*******************************************************
	virtual void SetCommonRatio(double CommRatio);


protected:
    /**
      @overload
      */
    virtual void FireOutputStart();

    /**
      @overload
      */
    virtual void FireOutputDone();
	/**
	@overload
	*/
	virtual void FireInputStart();

	/**
	@overload
	*/
	virtual void FireInputDone();

protected:

    ///*******************************************************
    /// @name:   TapValuedHopperNode::Measure
    /// @author: h
    /// @return: double
    /// @brief:  根据当前缓存的数据计算出一个综合计量结果
    ///*******************************************************
    virtual void Measure();

	virtual void CalcWeightByWind();
protected:
	SydTime m_OutputST;
	SydTime m_OutputET;
	SydTime m_InputST;
	SydTime m_InputET;
	double m_dLigninRatio;//木质素的计量系数：克/秒
	double m_dCommonRatio;//共同系数：（木质素的计量系数：克/秒表示；）
	bool m_bRunning;      //是否为工作中 
};

#endif // VALUED_BY_TAP_HOPPER_NODE_H
