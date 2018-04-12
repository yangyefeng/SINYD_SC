/////////////////////////////////////////////////////////////////////////////
/// @File name: mixer_node.h
/// @Author: h
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	Mixer Hopper Node Class
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef MIXER_NODE_H
#define MIXER_NODE_H

#include "hopper_node.h"

#include "../../Tool/syd_record_file.h"
#include "DBModule.h"

class MixerNode : public HopperNode
{
public:
    MixerNode();
	virtual ~MixerNode();

	///*******************************************************
	/// @name:   MixerNode::SetSpecialHandleFlg
	/// @author: h
	/// @return: void
	/// @param:  [in][bool]is_speflg
	/// @brief:  设置是否存在外参剂进水的情况
	///*******************************************************
	void SetSpecialHandleFlg(bool is_speflg = false);
	void SetIllegaSpecial(bool bIllSpe);
protected:
    ///*******************************************************
    /// @name:   MixerNode::Initialize
    /// @author: h
    /// @return: void
    /// @param:  [in][DataProvider *]pDataProvider
    /// @brief:  初始化函数
    ///*******************************************************
    virtual void Initialize(DataProvider *pDataProvider);

    ///*******************************************************
    /// @name:   MixerNode::Release
    /// @author: h
    /// @return: void
    /// @brief:  释放对象
    ///*******************************************************
    virtual void Release();

    ///*******************************************************
    /// @name:   MixerNode::FireOutputStart
    /// @author: h
    /// @return: void
    /// @brief:  放料开始的处理时序
    ///*******************************************************
    virtual void FireOutputStart();

    virtual void OnOutputStart(HopperNode *pHopper);

protected:
//    virtual bool PrepareRawStream();
//    virtual bool WriteRawHeader(HopperNode*pNode);
//    virtual bool SaveRawData(HopperNode *pNode);
	struct ST_PRODUCT_DATA
	{
		double value;
		string rawData;
	};
	/**
	@brief 保存每盘数据的结果到DB
	*/

	///*******************************************************
	/// @name:   MixerNode::CreateProductDB
	/// @author: h
	/// @return: void
	/// @brief:  创建生产数据的数据库
	///*******************************************************
	virtual void CreateProductDB();

	///*******************************************************
	/// @name:   MixerNode::PrepareProductInfoDB
	/// @author: h
	/// @return: void
	/// @param:  [in][HopperNode *]pNode
	/// @param:  [out][DBInfo &]tinfo
	/// @brief:  准备要创建生产数据的信息
	///*******************************************************
	virtual void PrepareProductInfoDB(HopperNode*pNode, DBInfo &tinfo);

    ///*******************************************************
    /// @name:   MixerNode::SetFieldInfo
    /// @author: h
    /// @return: void
    /// @param:  [in][string]fieldname
    /// @param:  [in][string]orgname
    /// @param:  [in][FieldDataType]dtype
    /// @param:  [out][FieldInfo *]outinfo
    /// @brief:  设置创建数据的Field信息
    ///*******************************************************
    virtual void SetFieldInfo(string fieldname, string orgname, FieldDataType dtype, FieldInfo *outinfo);

    ///*******************************************************
    /// @name:   MixerNode::CollectDetaiInfo
    /// @author: h
    /// @return: void
    /// @param:  [in][HopperProduct *]pProduct
    /// @brief:  收集生产关系数据的信息
    ///*******************************************************
    virtual void CollectDetaiInfo(HopperProduct* pProduct);

    ///*******************************************************
    /// @name:   MixerNode::CollectProductDBValue
    /// @author: h
    /// @return: void
    /// @param:  [in][HopperProduct *]pProduct
    /// @param:  [in][map<string]
    /// @param:  [out][double> &]mapDBVal
    /// @brief:  收集生产数据的要储存到数据库中的重量信息
    ///*******************************************************
    virtual void CollectProductDBValue(HopperProduct* pProduct, map<string, ST_PRODUCT_DATA> &mapDBVal);

	///*******************************************************
	/// @name:   MixerNode::SpecialValueHandle
	/// @author: h
	/// @return: void
	/// @param:  [in][map<string]
	/// @param:  [in][double> &]mapDBVal
	/// @brief:  计算外参剂进水情况的水重量
	///*******************************************************
	virtual void SpecialValueHandle(map<string, ST_PRODUCT_DATA> &mapDBVal);

    ///*******************************************************
    /// @name:   MixerNode::PrepareProductStream
    /// @author: h
    /// @return: bool
    /// @brief:  准备要创建生产数据的头信息
    ///*******************************************************
    virtual bool PrepareProductStream();

    ///*******************************************************
    /// @name:   MixerNode::WriteProductHeader
    /// @author: h
    /// @return: bool
    /// @param:  [in][HopperNode *]pNode
    /// @brief:  写生产数据的头信息
    ///*******************************************************
    virtual bool WriteProductHeader(HopperNode*pNode);

	///*******************************************************
	/// @name:   MixerNode::SaveProduct
	/// @author: h
	/// @return: bool
	/// @brief:  保存生产数据的结果到File和DB中
	///*******************************************************
    virtual bool SaveProduct();

    ///*******************************************************
    /// @name:   MixerNode::CollectProductValue
    /// @author: h
    /// @return: void
    /// @param:  [in][HopperProduct *]pProduct
    /// @param:  [out][map<INT32，double> &]mapValues
    /// @brief:  收集生产数据的要储存到File中的重量信息
    ///*******************************************************
    virtual void CollectProductValue(HopperProduct* pProduct, map<INT32,double> &mapValues);

    /**
      @brief 保存每盘数据的详细分析情况
      */
    ///*******************************************************
    /// @name:   MixerNode::PrepareProductDetailStream
    /// @author: h
    /// @return: bool
    /// @brief:  准备以及创建关系数据的信息
    ///*******************************************************
    virtual bool PrepareProductDetailStream();

    ///*******************************************************
    /// @name:   MixerNode::SaveProductDetail
    /// @author: h
    /// @return: bool
    /// @brief:  保存生产关系数据的信息
    ///*******************************************************
    virtual bool SaveProductDetail();

	/**
	@brief int转string
	*/
	virtual string GetString(const int n);

	virtual string GetString(const double f);

    ///*******************************************************
    /// @name:   MixerNode::GetProductID
    /// @author: h
    /// @return: int
    /// @brief:  获取生产ID（盘号）
    ///*******************************************************
    virtual int GetProductID();

    ///*******************************************************
    /// @name:   MixerNode::SetProductID
    /// @author: h
    /// @return: void
    /// @param:  [in][int]iproductid
    /// @brief:  设置生产ID（盘号）
    ///*******************************************************
    virtual void SetProductID(int iproductid);

	///*******************************************************
	/// @name:   MixerNode::GetDryTime
	/// @author: h
	/// @return: double
	/// @param:  [in][HopperProduct *]pProduct
	/// @brief:  获取沥青干拌时间
	///*******************************************************
	virtual double GetDryTime(HopperProduct* pProduct);

	///*******************************************************
	/// @name:   MixerNode::IsIllegal
	/// @author: h
	/// @return: bool
	/// @param:  [in][HopperProduct *]pProduct
	/// @brief:  判断是否存在违规信息
	///*******************************************************
	virtual bool IsIllegal(HopperProduct* pProduct);

	///*******************************************************
	/// @name:   MixerNode::IsRinsePot
	/// @author: h
	/// @return: bool
	/// @param:  [in][map<string]
	/// @param:  [in][double> &]mapDBVal
	/// @brief:  判断是否为涮锅
	///*******************************************************
	virtual bool IsRinsePot(map<string, ST_PRODUCT_DATA> &mapDBVal);

	///*******************************************************
	/// @name:   MixerNode::ClearProductValue
	/// @author: h
	/// @return: void
	/// @param:  [in][HopperNode *]pNode
	/// @param:  [in][int]iAndId
	/// @brief:  初始化生产节点的Value的状态
	///*******************************************************
	void ClearProductValue(HopperNode*pNode, int iAndId);

	bool IsValidDBVal(map<string, ST_PRODUCT_DATA> &mapDBVal);

protected:
    INT32 m_iProductId;

    SydRecordFile m_rfRaw;
    SydRecordFile m_rfProduct;
    SydRecordFile m_rfDetail;
	IDBOperator *m_pdbo;
	IDBOperator *m_pdetaildbo;
	EN_MIX_MACHINE_TYPE m_iMachineType;
	bool m_bspecial;
	bool m_billegalSpecial; //是否为违规的外参剂入水（外参剂入水的过程，水放料阀门开启）

    vector<INT32>    m_vecProductedHoppers; //记录所有需要保存数据的HOPPER的ID
};

#endif // MIXER_NODE_H
