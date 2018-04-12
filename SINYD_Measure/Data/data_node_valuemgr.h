#ifndef DATA_NODE_VALUEMGR_H
#define DATA_NODE_VALUEMGR_H
#include <vector>
#include <string>
using namespace std;
#include "Include/Operation_conf.h"

class DataNodeValueMgr
{
public:
	DataNodeValueMgr();
	virtual ~DataNodeValueMgr();

	void Initialize(int iAnaId = 0);
	void InitSlopeAlpha();
	void PushValue(const ST_HOPPER_VALUE& stHopValue);

	void ClearValue();

	int GetValueCount();

	vector<ST_HOPPER_VALUE>* GetValue();

	///*******************************************************
	/// @name:   DataNodeValueMgr::FindValue
	/// @author: h
	/// @return: int
	/// @param:  [in][const SydTime &]time
	/// @brief:  查找指定时间的一个值
	///*******************************************************
	virtual int FindValue(const SydTime& time);

	/**
	@brief 在指定的范围内找最小的值,返回该值的Index
	*/
	virtual int MinValue(int iStartIndex, int iEndIndex);

	///*******************************************************
	/// @name:   DataNodeValueMgr::MaxValue
	/// @author: h
	/// @return: int
	/// @param:  [in][int]iStartIndex
	/// @param:  [in][int]iEndIndex
	/// @brief:  在指定的范围内找最大的值,返回该值的Index
	///*******************************************************
	virtual int MaxValue(int iStartIndex, int iEndIndex);

	///*******************************************************
	/// @name:   DataNodeValueMgr::CalculateSlope
	/// @author: h
	/// @return: float
	/// @param:  [in][int]iStartIndex
	/// @param:  [in][int]iEndIndex
	/// @brief:  计算指定范围有点的斜率
	///*******************************************************
	virtual float CalculateSlope(int iStartIndex, int iEndIndex);

	///*******************************************************
	/// @name:   DataNodeValueMgr::MinSlope
	/// @author: h
	/// @return: int
	/// @param:  [in][int]iStartIndex
	/// @param:  [in][int]iEndIndex
	/// @brief:  在指定的范围内找最小的斜率值
	///*******************************************************
	virtual int MinSlope(int iStartIndex, int iEndIndex);

	///*******************************************************
	/// @name:   DataNodeValueMgr::FindDelayValue
	/// @author: h
	/// @return: int
	/// @param:  [in] iStart
	/// @param:  [in] iDelay
	/// @brief:  查找延时时间值的索引
	///*******************************************************
	virtual int FindValueAfter(const SydTime& time);
private:
	vector<ST_HOPPER_VALUE> m_vecValues;
	int m_iAlpha;
};

#endif //DATA_NODE_VALUEMGR_H
