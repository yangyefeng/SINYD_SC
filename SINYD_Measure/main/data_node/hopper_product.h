/////////////////////////////////////////////////////////////////////////////
/// @File name: hopper_product.h
/// @Author: h
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	Hopper Product Class
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <vector>
#include <fstream>

using namespace std;

#include "../../Include/EmsasCommon.h"
#include "../../Include/SwDscuMmdef.h"
#include "../../Tool/syd_time.h"

/**
  @brief 产品类。用于对一个料斗的上料、放料、称重进行计算
  */
class HopperProduct
{
public:
    HopperProduct();
    virtual ~HopperProduct();

    ///*******************************************************
    /// @name:   HopperProduct::Start
    /// @author: h
    /// @return: void
    /// @brief:  开始生产，进行计时
    ///*******************************************************
    virtual void Start();

    ///*******************************************************
    /// @name:   HopperProduct::Finish
    /// @author: h
    /// @return: void
    /// @brief:  完成生产，结束计时
    ///*******************************************************
    virtual void Finish();

    ///*******************************************************
    /// @name:   HopperProduct::Reset
    /// @author: h
    /// @return: void
    /// @brief:  重置生产状态
    ///*******************************************************
    virtual void Reset();

    ///*******************************************************
    /// @name:   HopperProduct::SetValue
    /// @author: h
    /// @return: void
    /// @param:  [in/out/inout][double]value
    /// @brief:  设置生产重量值
    ///*******************************************************
    virtual void SetValue(double value);

	///*******************************************************
	/// @name:   HopperProduct::SetRawValue
	/// @author: h
	/// @return: void
	/// @param:  [in][string]rawValue
	/// @brief:  设置Value对应的源数据值
	///*******************************************************
	virtual void SetRawValue(string rawValue);

    ///*******************************************************
    /// @name:   HopperProduct::ClearValue
    /// @author: h
    /// @return: void
    /// @brief:  清除生产重量值
    ///*******************************************************
    virtual void ClearValue();

    ///*******************************************************
    /// @name:   HopperProduct::IsValueReady
    /// @author: h
    /// @return: bool
    /// @brief:  判断是否设置了生产重量值
    ///*******************************************************
    virtual bool IsValueReady() const;

    ///*******************************************************
    /// @name:   HopperProduct::GetValue
    /// @author: h
    /// @return: double
    /// @brief:  获取生产重量值
    ///*******************************************************
    virtual double GetValue() const;

	///*******************************************************
	/// @name:   HopperProduct::GetRawValue
	/// @author: h
	/// @return: std::string
	/// @brief:  获取Value对应的源数据值
	///*******************************************************
	virtual string GetRawValue() const;

    ///*******************************************************
    /// @name:   HopperProduct::GetStartTime
    /// @author: h
    /// @return: SydTime
    /// @brief:  取得生产开始时间
    ///*******************************************************
    virtual SydTime GetStartTime();

    ///*******************************************************
    /// @name:   HopperProduct::GetFinishTime
    /// @author: h
    /// @return: SydTime
    /// @brief:  取得生产结束时间
    ///*******************************************************
    virtual SydTime GetFinishTime();

    ///*******************************************************
    /// @name:   HopperProduct::GetDuration
    /// @author: h
    /// @return: float
    /// @brief:  取得生产持续时间，精确到秒。
    ///*******************************************************
    virtual float GetDuration();

    ///*******************************************************
    /// @name:   HopperProduct::IsNewer
    /// @author: h
    /// @return: bool
    /// @brief:  判断是否为新创建生产对象
    ///*******************************************************
    virtual bool IsNewer();

    ///*******************************************************
    /// @name:   HopperProduct::IsRunning
    /// @author: h
    /// @return: bool
    /// @brief:  判断是否为运行中生产对象
    ///*******************************************************
    virtual bool IsRunning();

    ///*******************************************************
    /// @name:   HopperProduct::IsFinished
    /// @author: h
    /// @return: bool
    /// @brief:  判断是否未生产的对象
    ///*******************************************************
    virtual bool IsFinished();

    ///*******************************************************
    /// @name:   HopperProduct::SetId
    /// @author: h
    /// @return: void
    /// @param:  [in][int]iId
    /// @brief:  设置Node id值
    ///*******************************************************
    virtual void SetId(int iId);

    ///*******************************************************
    /// @name:   HopperProduct::GetId
    /// @author: h
    /// @return: int
    /// @brief:  获取Node id值
    ///*******************************************************
    virtual int GetId() const;

    ///*******************************************************
    /// @name:   HopperProduct::SetName
    /// @author: h
    /// @return: void
    /// @param:  [in][const string &]strName
    /// @brief:  设置生产对象名称
    ///*******************************************************
    virtual void SetName(const string &strName);

    ///*******************************************************
    /// @name:   HopperProduct::GetName
    /// @author: h
    /// @return: std::string
    /// @brief:  获取生产对象名称
    ///*******************************************************
    virtual string GetName() const;

	///*******************************************************
	/// @name:   HopperProduct::SetTyep
	/// @author: h
	/// @return: void
	/// @param:  [in][int]itype
	/// @brief:  设置生产对象类型
	///*******************************************************
	virtual void SetTyep(int itype);

	///*******************************************************
	/// @name:   HopperProduct::GetType
	/// @author: h
	/// @return: int
	/// @brief:  获取生产对象类型
	///*******************************************************
	virtual int GetType() const;

	///*******************************************************
	/// @name:   HopperProduct::SetIllMark
	/// @author: h
	/// @return: void
	/// @param:  [in][bool]illmark
	/// @brief:  设置违规标识
	///*******************************************************
	virtual void SetIllMark(bool illmark);

	///*******************************************************
	/// @name:   HopperProduct::GetIllMark
	/// @author: h
	/// @return: bool
	/// @brief:  获取违规标识
	///*******************************************************
	virtual bool GetIllMark() const;

    ///*******************************************************
    /// @name:   HopperProduct::ToString
    /// @author: h
    /// @return: std::string
    /// @brief:  已字符形式表示生产对象的属性
    ///*******************************************************
    virtual string ToString();

	///*******************************************************
	/// @name:   HopperProduct::SetEndTime
	/// @author: h
	/// @return: void
	/// @param:  [in/out/inout][const SydTime *]pTime
	/// @brief:  设置生产对象放料完毕的时间
	///*******************************************************
	virtual void SetEndTime(const SydTime* pTime = NULL);

	///*******************************************************
	/// @name:   HopperProduct::GetEndTime
	/// @author: h
	/// @return: SydTime
	/// @brief:  获取生产对象放料完毕的时间
	///*******************************************************
	virtual SydTime GetEndTime();
public:
    ///*******************************************************
    /// @name:   HopperProduct::AddChild
    /// @author: h
    /// @return: void
    /// @param:  [in][const HopperProduct &]pProduct
    /// @brief:  添加子生产对象
    ///*******************************************************
    virtual void AddChild(const HopperProduct& pProduct);

    ///*******************************************************
    /// @name:   HopperProduct::GetChildren
    /// @author: h
    /// @return: vector<HopperProduct> *
    /// @brief:  获取子生产对象的指针
    ///*******************************************************
    virtual vector<HopperProduct> * GetChildren();

	void RemoveChildren(HopperProduct *pHopper);
    ///*******************************************************
    /// @name:   HopperProduct::DumpTree
    /// @author: h
    /// @return: void
    /// @param:  [in][INT32]iLevel
    /// @brief:  打印树型结构到控制台
    ///*******************************************************
    virtual void DumpTree(INT32 iLevel=0);

    /**
      @brief
      */
    ///*******************************************************
    /// @name:   HopperProduct::DumpJson
    /// @author: h
    /// @return: void
    /// @param:  [out][ofstream &]ofTarget
    /// @param:  [in][int]iProductId
    /// @brief:  已Json结构表示一盘的生产数据
    ///*******************************************************
    virtual void DumpJson(ofstream& ofTarget , int iProductId);

protected:
    virtual void DumpJsonNode(ostringstream &os, INT32 iLevel=0);


protected:
    virtual void SetStartTime(const SydTime* pTime=NULL);

    virtual void SetFinishTime(const SydTime* pTime=NULL);

protected:
    SydTime m_tmStart;  //生产开始时间
    SydTime m_tmFinish; //生产结束时间
	SydTime m_tmEnd;    //阀门关闭时间
    bool m_bRunning;    //正在生产
    bool m_bFinished;   //生产完成

    double m_iValue;
	string m_sRawValue;
    bool m_bValueIsReady;

    INT32  m_iId;
    string m_strName;
	int m_itype;
	bool m_billmark;
    vector<HopperProduct> m_vecChildren;   //子产品列表
};

#endif // PRODUCT_H
