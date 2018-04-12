/////////////////////////////////////////////////////////////////////////////
/// @File name: SCOCR.cpp
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 		
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#include "SCOCR.h"
#include "SCOCRImpl.h"


CSCDigitOCR::CSCDigitOCR()
	: m_pSDigitCOCRImpl(nullptr)
{
	m_pSDigitCOCRImpl = new CSCDigitOCRImpl();
}


CSCDigitOCR::~CSCDigitOCR()
{
	if (m_pSDigitCOCRImpl)
	{
		delete m_pSDigitCOCRImpl;
		m_pSDigitCOCRImpl = nullptr;
	}
}

SC_OCR_ERROR CSCDigitOCR::RecognizeDigits(const unsigned long* pRGBBuffer,
										int width,
										int height,
										unsigned long lDigitRGB32,
										list<DIGIT>& listpDigit) 
{
	return m_pSDigitCOCRImpl->RecognizeDigits(pRGBBuffer, width, height, lDigitRGB32, listpDigit);
}

void CSCDigitOCR::ClearRecogizeResList(list<DIGIT>& listpDigit)
{
	listpDigit.clear();
}

SC_OCR_ERROR CSCDigitOCR::SetDTM(const DIGIT & d)
{
	return m_pSDigitCOCRImpl->SetDTM(d);
}

void CSCDigitOCR::SetEqulaTolerance(int tolerance)
{
	m_pSDigitCOCRImpl->SetEqulaTolerance(tolerance);
}

void CSCDigitOCR::GetDigitModelbyID(const list<int>& listDigitID, list<DIGITMODEL>& listDigit)
{
	m_pSDigitCOCRImpl->GetDigitModelbyID(listDigitID, listDigit);
}

void CSCDigitOCR::ClearDigitModelResList(list<DIGITMODEL>& listpDigit)
{
	listpDigit.clear();
}

CSCColorOCR::CSCColorOCR()
	: m_pSColorCOCRImpl(nullptr)
{
	m_pSColorCOCRImpl = new CSCColorOCRImpl();
}


CSCColorOCR::~CSCColorOCR()
{
	if (m_pSColorCOCRImpl)
	{
		delete m_pSColorCOCRImpl;
		m_pSColorCOCRImpl = nullptr;
	}
}

bool CSCColorOCR::RGB32Equal(unsigned long rgb1, unsigned long rgb2)
{
	return m_pSColorCOCRImpl->RGB32Equal(rgb1, rgb2);
}

int CSCColorOCR::GetTolerance(unsigned long rgb1, unsigned long rgb2)
{
	return m_pSColorCOCRImpl->GetTolerance(rgb1, rgb2);
}

int CSCColorOCR::GetOTSUThreshold(unsigned long rgb, unsigned long * buf, long bufLen)
{
	return m_pSColorCOCRImpl->GetOTSUThreshold(rgb, buf, bufLen);
}

void CSCColorOCR::SetEqulaTolerance(int tolerance)
{
	m_pSColorCOCRImpl->SetEqulaTolerance(tolerance);
}
