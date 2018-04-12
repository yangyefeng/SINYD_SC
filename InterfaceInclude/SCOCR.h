/////////////////////////////////////////////////////////////////////////////
/// @File name: SCOCR.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 		
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "SCOCRDef.h"
#include <list>

using namespace std;

class CSCDigitOCRImpl;
class SC_OCR_API CSCDigitOCR
{
public:
	CSCDigitOCR();
	~CSCDigitOCR();

	///*******************************************************
	/// @name:   CSCDigitOCR::RecognizeDigits
	/// @author: YaoDi
	/// @return: SC_OCR_ERROR
	/// @param:  [in][const unsigned long *]pRGBBuffer
	/// @param:  [in][int]width
	/// @param:  [in][int]height
	/// @param:  [in][unsigned long]lDigitRGB32
	/// @param:  [out][list<DIGIT> &]listpDigit
	/// @brief:  对传入的图像Buffer进行识别，并将结果保存在list中。未识别字符以“x”进行标识
	///*******************************************************
	SC_OCR_ERROR RecognizeDigits(const unsigned long* pRGBBuffer, 
								int width, 
								int height, 
								unsigned long lDigitRGB32,
								list<DIGIT>& listpDigit);
	void ClearRecogizeResList(list<DIGIT>& listpDigit);

	///*******************************************************
	/// @name:   CSCDigitOCR::SetDTM
	/// @author: YaoDi
	/// @return: SC_OCR_ERROR
	/// @param:  [in][const DIGIT &]d
	/// @brief:  向字模库中追加字模信息
	///*******************************************************
	SC_OCR_ERROR SetDTM(const DIGIT& d);
	///*******************************************************
	/// @name:   CSCDigitOCR::SetEqulaTolerance
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][int]tolerance
	/// @brief:  设定识别字符时的颜色色差阈值
	///*******************************************************
	void SetEqulaTolerance(int tolerance);

	void GetDigitModelbyID(const list<int>& listDigitID, list<DIGITMODEL>& listDigit);
	void ClearDigitModelResList(list<DIGITMODEL>& listpDigit);

private:
	CSCDigitOCRImpl* m_pSDigitCOCRImpl;
};

class CSCColorOCRImpl;
class SC_OCR_API CSCColorOCR
{
public:
	CSCColorOCR();
	~CSCColorOCR();

	///*******************************************************
	/// @name:   CSCColorOCR::RGB32Equal
	/// @author: YaoDi
	/// @return: bool
	/// @param:  [in][unsigned long]rgb1
	/// @param:  [in][unsigned long]rgb2
	/// @brief:  判断两个RGB颜色是否相等。条件为两个RGB的色差是否小于设定阈值（默认阈值目前为100）
	///*******************************************************
	bool RGB32Equal(unsigned long rgb1, unsigned long rgb2);
	///*******************************************************
	/// @name:   CSCColorOCR::GetTolerance
	/// @author: YaoDi
	/// @return: int
	/// @param:  [in][unsigned long]rgb1
	/// @param:  [in][unsigned long]rgb2
	/// @brief:  获取两个RGB颜色的色差
	///*******************************************************
	int GetTolerance(unsigned long rgb1, unsigned long rgb2);
	///*******************************************************
	/// @name:   CSCColorOCR::GetOTSUThreshold
	/// @author: YaoDi
	/// @return: int
	/// @param:  [in][unsigned long]rgb
	/// @param:  [in][unsigned long *]buf
	/// @param:  [in][long]bufLen
	/// @brief:  最大类间方差法计算：在一块图像区域中，针对一个前景RGB颜色，进行前景背景识别的最佳色差阈值
	///*******************************************************
	int GetOTSUThreshold(unsigned long rgb, unsigned long* buf, long bufLen);
	///*******************************************************
	/// @name:   CSCColorOCR::SetEqulaTolerance
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][int]tolerance
	/// @brief:  设定颜色比较时的色差阈值
	///*******************************************************
	void SetEqulaTolerance(int tolerance);

private:
	CSCColorOCRImpl* m_pSColorCOCRImpl;
};

