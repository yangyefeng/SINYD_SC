/////////////////////////////////////////////////////////////////////////////
/// @File name: SCOCRImpl.h
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
#include <map>

using namespace std;

#define DEFAULT_TOLERANCE 10

class CSCColorOCRImpl
{
public:
	CSCColorOCRImpl();
	virtual ~CSCColorOCRImpl();

	bool RGB32Equal(unsigned long rgb1, unsigned long rgb2);
	void SetEqulaTolerance(int tolerance);
	int GetTolerance(unsigned long rgb1, unsigned long rgb2);
	int GetOTSUThreshold(unsigned long rgb, unsigned long* buf, long bufLen);
private:
	int m_iEqualTolerance;
};



class CSCDigitOCRImpl
{
public:
	CSCDigitOCRImpl();
	virtual ~CSCDigitOCRImpl();

	SC_OCR_ERROR RecognizeDigits(const unsigned long* pRGBBuffer,
								int width,
								int height,
								unsigned long lDigitRGB32,
								list<DIGIT>& listpDigit);
	SC_OCR_ERROR SetDTM(const DIGIT& d);
	void SetEqulaTolerance(int tolerance);
	void GetDigitModelbyID(const list<int>& listDigitID, list<DIGITMODEL>& listDigit);

private:
	void RGBBuffer2CharBuffer(const unsigned long* pBuffer, char* pCharBuffer, int iBufferLength, unsigned long lDigitRGB32);
	int GetTopBlankRowCount(char* pCharBuffer, int width, int height);
	int GetBottomBlankRowCount(char* pCharBuffer, int width, int height);
	void GetFirstDigitModel(char* pCharBuffer, int width, int height, PDIGITMODEL* pDigitModel, int& endX);
	bool Recognize(PDIGITMODEL pDigitModel, string* iRes);
	void GetNextDigitModel(char* pCharBuffer, int width, int height, /*inout*/PDIGITMODEL* pDigitModel, /*inout*/int& endX);
	char* CutTopBottomBlankRow(char* pCharBuffer, int width, int height, int* newHeight);
	
private:
	CSCColorOCRImpl m_ColorOCRImpl;
};



