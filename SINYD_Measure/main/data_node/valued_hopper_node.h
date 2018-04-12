/////////////////////////////////////////////////////////////////////////////
/// @File name: valued_hopper_node.h
/// @Author: h
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	Oto Measure Hopper Node
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef VALUED_HOPPER_NODE_H
#define VALUED_HOPPER_NODE_H

#include "hopper_node.h"
#include "../../Tool/syd_time.h"
#include "../../Data/Include/Operation_conf.h"
#include "../data_algorithm/data_algorithm.h"
#include "../../Data/data_node_valuemgr.h"

/**
  @brief 可计量的料仓。计量方法包括称重、测温等。
  特点：有一个计量单元（模拟量传感器），有零个或多个入料仓（child）
  */
class ValuedHopperNode : public HopperNode
{
public:
    ValuedHopperNode();

    ///*******************************************************
    /// @name:   ValuedHopperNode::Initialize
    /// @author: h
    /// @return: void
    /// @param:  [in][DataProvider *]pDataProvider
    /// @brief:  初始化函数
    ///*******************************************************
    virtual void Initialize(DataProvider *pDataProvider);
    
    ///*******************************************************
    /// @name:   ValuedHopperNode::Execute
    /// @author: h
    /// @return: void
    /// @brief:  执行料仓计量动作开始函数
    ///*******************************************************
    virtual void Execute();

	///*******************************************************
	/// @name:   ValuedHopperNode::Release
	/// @author: h
	/// @return: void
	/// @brief:  释放对象
	///*******************************************************
	virtual void Release();

    ///*******************************************************
    /// @name:   ValuedHopperNode::RefreshStatus
    /// @author: h
    /// @return: bool
    /// @brief:  更新料仓节点状态函数
    ///*******************************************************
    virtual bool RefreshStatus();

    ///*******************************************************
    /// @name:   ValuedHopperNode::ToString
    /// @author: h
    /// @return: std::string
    /// @brief: 已字符形式表示当前料仓节点属性信息 
    ///*******************************************************
    virtual string ToString();

	///*******************************************************
	/// @name:   ValuedHopperNode::JudgeIllegal
	/// @author: h
	/// @return: bool
	/// @brief:  计算违规（补料）动作
	///*******************************************************
	virtual bool JudgeIllegal();

	virtual void AddArea(ST_AREA_CONF * st_pArea);
	virtual void AddMeasureMethod(EN_ANA_MEASURE_TYPE meaType);

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
    /// @name:   ValuedHopperNode::PushValue
    /// @author: h
    /// @return: void
    /// @brief:  将Value储存vector结构中，同时计算该对应的斜率
    ///*******************************************************
    //virtual void PushValue();
    ///*******************************************************
    /// @name:   ValuedHopperNode::GetValueCount
    /// @author: h
    /// @return: INT32
    /// @brief:  获取储存vector结构中Value的总数
    ///*******************************************************
    //virtual INT32 GetValueCount();

    ///*******************************************************
    /// @name:   ValuedHopperNode::FindValue
    /// @author: h
    /// @return: INT32
    /// @param:  [in][const SydTime &]time
    /// @brief:  查找指定时间的一个值
    ///*******************************************************
   // virtual INT32 FindValue(const SydTime& time);

    /**
      @brief 在指定的范围内找最小的值,返回该值的Index
      */
    //virtual INT32 MinValue(INT32 iStartIndex,INT32 iEndIndex);

     ///*******************************************************
    /// @name:   ValuedHopperNode::MaxValue
    /// @author: h
    /// @return: INT32
    /// @param:  [in][INT32]iStartIndex
    /// @param:  [in][INT32]iEndIndex
    /// @brief:  在指定的范围内找最大的值,返回该值的Index
    ///*******************************************************
    //virtual INT32 MaxValue(INT32 iStartIndex,INT32 iEndIndex);

    ///*******************************************************
    /// @name:   ValuedHopperNode::CalculateSlope
    /// @author: h
    /// @return: float
    /// @param:  [in][INT32]iStartIndex
    /// @param:  [in][INT32]iEndIndex
    /// @brief:  计算指定范围有点的斜率
    ///*******************************************************
   // virtual float CalculateSlope(INT32 iStartIndex,INT32 iEndIndex);

    ///*******************************************************
    /// @name:   ValuedHopperNode::MinSlope
    /// @author: h
    /// @return: INT32
    /// @param:  [in][INT32]iStartIndex
    /// @param:  [in][INT32]iEndIndex
    /// @brief:  在指定的范围内找最小的斜率值
    ///*******************************************************
   // virtual INT32 MinSlope(INT32 iStartIndex,INT32 iEndIndex);

    ///*******************************************************
    /// @name:   ValuedHopperNode::Measure
    /// @author: h
    /// @return: double
    /// @brief:  根据当前缓存的数据计算出一个综合计量结果
    ///*******************************************************
    virtual void Measure();


    ///*******************************************************
    /// @name:   ValuedHopperNode::FindDelayValue
    /// @author: h
    /// @return: int
    /// @param:  [in] iStart
    /// @param:  [in] iDelay
    /// @brief:  查找延时时间值的索引
    ///*******************************************************
    //virtual int FindValueAfter(const SydTime& time);

	virtual int CalcIndex(EN_AREA_TYPE enArea, int iDelay);
	virtual bool IsExistOutputDoneAfterDelay();
	virtual ST_MEASURE_VALUE GetMeasureValue(ST_AREA_CONF* stpAreaConf);

protected:
    /**
      @brief 用于记录一个时间点的料仓数据的结构体
      */
//    struct ST_HOPPER_VALUE{
//        SydTime time;
////        INT32 pkgId;
////        INT32 id;
//        double analog;
//        double value;
//        float slope; //斜率，评估数据走向
//
//        ST_HOPPER_VALUE(INT32 iId,DataProvider* pProvider){
//            time=pProvider->GetTimestamp();
////            id=iId;
////            pkgId = pProvider->GetPackageId();
//            value= pProvider->GetAnalogData(iId);
//            analog=0.0;
//            slope=0.0;
//        }
//        ST_HOPPER_VALUE(const ST_HOPPER_VALUE& a){
//            time=a.time;
////            pkgId=a.pkgId;
////            id=a.id;
//            value=a.value;
//            analog=a.analog;
//            slope=a.slope;
//        }
//    };
	struct ST_PROCESS_INDEX {
		int InStartIdx;
		int InEndIdx;
		int OutStartIdx;
		int OutEndIdx;
		void SetComIndex(int iComIdx) {
			InStartIdx = iComIdx;
			InEndIdx = iComIdx;
			OutStartIdx = iComIdx;
			OutEndIdx = iComIdx;
		}
	};

protected:
//    list<ST_HOPPER_STATUS> m_lstStatus;     //历史状态，不包含当前状态

	DataNodeValueMgr *m_pDataValueMgr;
    //vector<ST_HOPPER_VALUE> m_vecValues;
	bool m_bRepeatOutputStatus;         //是否为反复放料状态
	bool m_bInAndOutTogetherAction;     //是否为上料开始时，放料阀还在放料的状态
	bool m_bIsExistOutDoneDelay;        //是否存在放料阀门关闭的延时
	SydTime m_OutDoneDelayTime;        //记录放料延时后的时间

	vector<ST_AREA_CONF*> m_pAreaConf;       //计量的区域和取值算法
	EN_ANA_MEASURE_TYPE m_enMeasureType;     //计量方式
	ST_PROCESS_INDEX m_stProcessIdx;         //记录料仓进、放量状态的Index
};

#endif // VALUED_HOPPER_NODE_H
