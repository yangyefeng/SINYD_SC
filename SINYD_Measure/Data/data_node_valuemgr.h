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
	/// @brief:  ����ָ��ʱ���һ��ֵ
	///*******************************************************
	virtual int FindValue(const SydTime& time);

	/**
	@brief ��ָ���ķ�Χ������С��ֵ,���ظ�ֵ��Index
	*/
	virtual int MinValue(int iStartIndex, int iEndIndex);

	///*******************************************************
	/// @name:   DataNodeValueMgr::MaxValue
	/// @author: h
	/// @return: int
	/// @param:  [in][int]iStartIndex
	/// @param:  [in][int]iEndIndex
	/// @brief:  ��ָ���ķ�Χ��������ֵ,���ظ�ֵ��Index
	///*******************************************************
	virtual int MaxValue(int iStartIndex, int iEndIndex);

	///*******************************************************
	/// @name:   DataNodeValueMgr::CalculateSlope
	/// @author: h
	/// @return: float
	/// @param:  [in][int]iStartIndex
	/// @param:  [in][int]iEndIndex
	/// @brief:  ����ָ����Χ�е��б��
	///*******************************************************
	virtual float CalculateSlope(int iStartIndex, int iEndIndex);

	///*******************************************************
	/// @name:   DataNodeValueMgr::MinSlope
	/// @author: h
	/// @return: int
	/// @param:  [in][int]iStartIndex
	/// @param:  [in][int]iEndIndex
	/// @brief:  ��ָ���ķ�Χ������С��б��ֵ
	///*******************************************************
	virtual int MinSlope(int iStartIndex, int iEndIndex);

	///*******************************************************
	/// @name:   DataNodeValueMgr::FindDelayValue
	/// @author: h
	/// @return: int
	/// @param:  [in] iStart
	/// @param:  [in] iDelay
	/// @brief:  ������ʱʱ��ֵ������
	///*******************************************************
	virtual int FindValueAfter(const SydTime& time);
private:
	vector<ST_HOPPER_VALUE> m_vecValues;
	int m_iAlpha;
};

#endif //DATA_NODE_VALUEMGR_H
