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
	/// @brief:  �Դ����ͼ��Buffer����ʶ�𣬲������������list�С�δʶ���ַ��ԡ�x�����б�ʶ
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
	/// @brief:  ����ģ����׷����ģ��Ϣ
	///*******************************************************
	SC_OCR_ERROR SetDTM(const DIGIT& d);
	///*******************************************************
	/// @name:   CSCDigitOCR::SetEqulaTolerance
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][int]tolerance
	/// @brief:  �趨ʶ���ַ�ʱ����ɫɫ����ֵ
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
	/// @brief:  �ж�����RGB��ɫ�Ƿ���ȡ�����Ϊ����RGB��ɫ���Ƿ�С���趨��ֵ��Ĭ����ֵĿǰΪ100��
	///*******************************************************
	bool RGB32Equal(unsigned long rgb1, unsigned long rgb2);
	///*******************************************************
	/// @name:   CSCColorOCR::GetTolerance
	/// @author: YaoDi
	/// @return: int
	/// @param:  [in][unsigned long]rgb1
	/// @param:  [in][unsigned long]rgb2
	/// @brief:  ��ȡ����RGB��ɫ��ɫ��
	///*******************************************************
	int GetTolerance(unsigned long rgb1, unsigned long rgb2);
	///*******************************************************
	/// @name:   CSCColorOCR::GetOTSUThreshold
	/// @author: YaoDi
	/// @return: int
	/// @param:  [in][unsigned long]rgb
	/// @param:  [in][unsigned long *]buf
	/// @param:  [in][long]bufLen
	/// @brief:  �����䷽����㣺��һ��ͼ�������У����һ��ǰ��RGB��ɫ������ǰ������ʶ������ɫ����ֵ
	///*******************************************************
	int GetOTSUThreshold(unsigned long rgb, unsigned long* buf, long bufLen);
	///*******************************************************
	/// @name:   CSCColorOCR::SetEqulaTolerance
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][int]tolerance
	/// @brief:  �趨��ɫ�Ƚ�ʱ��ɫ����ֵ
	///*******************************************************
	void SetEqulaTolerance(int tolerance);

private:
	CSCColorOCRImpl* m_pSColorCOCRImpl;
};

