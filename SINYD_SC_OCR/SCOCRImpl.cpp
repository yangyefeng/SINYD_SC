/////////////////////////////////////////////////////////////////////////////
/// @File name: SCOCRImpl.cpp
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 		
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#include "SCOCRImpl.h"
#include <math.h>
#include <memory>
#include <list>
#include "DigitConfig.h"

using namespace std;

CSCDigitOCRImpl::CSCDigitOCRImpl()
{
}


CSCDigitOCRImpl::~CSCDigitOCRImpl()
{
}


SC_OCR_ERROR CSCDigitOCRImpl::RecognizeDigits(const unsigned long* pRGBBuffer,
											int width,
											int height,
											unsigned long lDigitRGB32,
											list<DIGIT>& listpDigit)
{
	if (!pRGBBuffer)
	{
		return SC_OCR_NULLPTR;
	}
	if (listpDigit.size() > 0)
	{
		return SC_OCR_INVALIDPARAM;
	}

	SC_OCR_ERROR err = SC_OCR_SUCCESS;

	char* pCharBuffer = new char[width*height];
	RGBBuffer2CharBuffer(pRGBBuffer, pCharBuffer, width*height, lDigitRGB32);

	int newWidth = width, newHeight = 0;
	char* pBufferNoTopBottomBlankRow = CutTopBottomBlankRow(pCharBuffer, width, height, &newHeight);

	PDIGITMODEL pDigitModel = nullptr;
	int endX = -1;
	GetFirstDigitModel(pBufferNoTopBottomBlankRow, newWidth, newHeight, &pDigitModel, endX);
	if (!pDigitModel)
	{
		err = SC_OCR_FAIL;
	}
	else
	{
		while (pDigitModel)
		{
			string cRes = "x";
			bool bRecog = Recognize(pDigitModel, &cRes);
			DIGIT Digit(pDigitModel, cRes);
			if (!bRecog)
			{
				err = SC_OCR_EXISTS_UNRECOGNIZE;
				Digit.model.id = DigitConfigIntance::GetInstance()->SetDTM(Digit);
			}
			listpDigit.push_back(Digit);

			GetNextDigitModel(pBufferNoTopBottomBlankRow, newWidth, newHeight, &pDigitModel, endX);
		}
	}

	if (pCharBuffer)
	{
		delete[] pCharBuffer;
		pCharBuffer = nullptr;
	}
	if (pBufferNoTopBottomBlankRow)
	{
		delete[] pBufferNoTopBottomBlankRow;
		pBufferNoTopBottomBlankRow = nullptr;
	}

	return err;
}

void CSCDigitOCRImpl::GetDigitModelbyID(const list<int>& listDigitID, list<DIGITMODEL>& listDigit)
{
	list<int>::const_iterator itEnd = listDigitID.end();
	DTA dta;
	string m;
	for (list<int>::const_iterator it = listDigitID.begin(); it != itEnd; ++it)
	{
		if (DigitConfigIntance::GetInstance()->FindModelAndDTA(*it, dta, m))
		{
			DIGITMODEL model(dta.w, dta.h, m.c_str());
			model.id = *it;
			listDigit.push_back(model);
		}
	}
}

SC_OCR_ERROR CSCDigitOCRImpl::SetDTM(const DIGIT & d)
{
	return DigitConfigIntance::GetInstance()->SetDTM(d) != -1 ? SC_OCR_SUCCESS : SC_OCR_FAIL;
}


void CSCDigitOCRImpl::RGBBuffer2CharBuffer(const unsigned long * pBuffer, char * pCharBuffer, int iBufferLength, unsigned long lDigitRGB32)
{
	for (int i = 0; i < iBufferLength; i++)
	{
		pCharBuffer[i] = m_ColorOCRImpl.RGB32Equal(pBuffer[i], lDigitRGB32) ^ '0';
	}
}


int CSCDigitOCRImpl::GetTopBlankRowCount(char* pCharBuffer, int width, int height)
{
	int iRes = 0;
	bool bIsBlankRow = true;

	//自上向下遍历每一行
	for (int y = 0; y < height; y++)
	{
		//遍历每一行的每个像素
		for (int x = 0; x < width; x++)
		{
			//如果其中有任一像素的值为1，则此行不是空行，不需要再确认此行中后面的像素点
			if (pCharBuffer[x + y * width] == '1')
			{
				bIsBlankRow = false;
				break;
			}
			
		}
		//当发现非空行出现时，则认为此行开始以下均为数字区域，遍历终止，并记录遍历发现的空行数
		if (!bIsBlankRow)
		{
			iRes = y;
			break;
		}
	}
	return iRes;
}

int CSCDigitOCRImpl::GetBottomBlankRowCount(char* pCharBuffer, int width, int height)
{
	int iRes = 0;
	bool bIsBlankRow = true;
	//自下向上遍历每一行
	for (int y = height - 1; y > 0; y--)
	{
		//遍历每一行的每个像素
		for (int x = 0; x < width; x++)
		{
			//如果其中有任一像素的值为1，则此行不是空行，不需要再确认此行中后面的像素点
			if (pCharBuffer[x + y * width] == '1')
			{
				bIsBlankRow = false;
				break;
			}

		}
		//当发现非空行出现时，则认为此行开始以上均为数字区域，遍历终止，并记录遍历发现的空行数
		if (!bIsBlankRow)
		{
			iRes = height - 1 - y;
			break;
		}
	}
	return iRes;
}

void CSCDigitOCRImpl::GetFirstDigitModel(char* pCharBuffer, int width, int height, PDIGITMODEL* pDigitModel, int& endX)
{
	int startX = -1;
	bool isFindDigitStart = false;
	//遍历每一列
	//查找第一个字模时，从Buffer的第一列开始进行查找
	for (int x = 0; x < width; x++)
	{
		bool bIsBlankColumn = true;
		//遍历列中的每个像素
		for (int y = 0; y < height; y++)
		{
			//如果此列中存在值为1的像素，则此列不是空列
			if (pCharBuffer[x + y*width] == '1')
			{
				bIsBlankColumn = false;
				break;
			}
		}
		//当发现非空列，并且还没发现一个字模的开始时，则此非空列作为本次字模的开始列
		if (!bIsBlankColumn && !isFindDigitStart)
		{
			startX = x;
			isFindDigitStart = true;
		}
		//当发现空列，并且已经发现一个字模的开始时，则此空列为两个字模中间的分割列，此非空列的前一列为本次字模的结束列
		if (bIsBlankColumn && isFindDigitStart)
		{
			endX = x - 1;
			break;
		}
	}
	//当已经发现字模的开始列，但是没有发现结束列时，则开始列之后的全部列均为本次字模的内容
	if (isFindDigitStart && endX < startX)
	{
		endX = width - 1;
	}

	if (*pDigitModel)
	{
		delete *pDigitModel;
		*pDigitModel = nullptr;
	}

	//当查找到本次的字模时
	if (isFindDigitStart)
	{
		//根据本次字模的宽、高信息以及字模内容生成字模模型
		int BufW = endX - startX + 1;
		char* pDigitBuf = new char[BufW*height + 1];
		for (int i = 0; i < height; i++)
		{
			for (int j = startX; j < endX + 1; j++)
			{
				pDigitBuf[j - startX + i * BufW] = pCharBuffer[j + i * width];
			}
		}
		pDigitBuf[BufW*height] = '\0';

		*pDigitModel = new DIGITMODEL(BufW, height, pDigitBuf);

		delete pDigitBuf;
		pDigitBuf = nullptr;
	}

}

bool CSCDigitOCRImpl::Recognize(PDIGITMODEL pDigitModel, string* pRes)
{
	bool bRes = false;

	DTA dta;
	if (DigitConfigIntance::GetInstance()->FindDTA(pDigitModel->m, dta))
	{
		bRes = true;
		*pRes = dta.c;
		pDigitModel->id = dta.id;
	}
	else
	{
		*pRes = "x";
	}

	return bRes;
}

void CSCDigitOCRImpl::GetNextDigitModel(char* pCharBuffer, int width, int height, /*inout*/PDIGITMODEL* pDigitModel, /*inout*/int& endX)
{
	int startX = 0;
	bool isFindDigitStart = false;
	//遍历每一列
	//根据第一个字模信息，查找下一个字模时，从上一个字模的下一列开始查找
	for (int x = endX + 1; x < width; x++)
	{
		bool bIsBlankColumn = true;
		//遍历列中的每个像素
		for (int y = 0; y < height; y++)
		{
			//如果此列中存在值为1的像素，则此列不是空列
			if (pCharBuffer[x + y*width] == '1')
			{
				bIsBlankColumn = false;
				break;
			}
		}
		//当发现非空列，并且还没发现一个字模的开始时，则此非空列作为本次字模的开始列
		if (!bIsBlankColumn && !isFindDigitStart)
		{
			startX = x;
			isFindDigitStart = true;
		}
		//当发现空列，并且已经发现一个字模的开始时，则此空列为两个字模中间的分割列，此非空列的前一列为本次字模的结束列
		if (bIsBlankColumn && isFindDigitStart)
		{
			endX = x - 1;
			break;
		}
	}
	//当已经发现字模的开始列，但是没有发现结束列时，则开始列之后的全部列均为本次字模的内容
	if (isFindDigitStart && endX < startX)
	{
		endX = width - 1;
	}
	
	if (*pDigitModel)
	{
		delete *pDigitModel;
		*pDigitModel = nullptr;
	}

	//当查找到本次的字模时
	if (isFindDigitStart)
	{
		//根据本次字模的宽、高信息以及字模内容生成字模模型
		int BufW = endX - startX + 1;
		char* pDigitBuf = new char[BufW*height + 1];
		for (int i = 0; i < height; i++)
		{
			for (int j = startX; j < endX + 1; j++)
			{
				pDigitBuf[j - startX + i * BufW] = pCharBuffer[j + i * width];
			}
		}
		pDigitBuf[BufW*height] = '\0';

		*pDigitModel = new DIGITMODEL(BufW, height, pDigitBuf);

		delete pDigitBuf;
		pDigitBuf = nullptr;
	}
}

char* CSCDigitOCRImpl::CutTopBottomBlankRow(char* pCharBuffer, int width, int height, int* newHeight)
{
	//获取图像上面的空行数
	int TopBlankRowCount = GetTopBlankRowCount(pCharBuffer, width, height);
	//获取图像下面的空行数
	int BottomBlankRowCount = GetBottomBlankRowCount(pCharBuffer, width, height);

	//生成并返回删除上下空行后的图像buffer
	*newHeight = height - TopBlankRowCount - BottomBlankRowCount;
	char* pBufferNoTopBottomBlankRow = new char[width*(*newHeight)];
	memcpy_s(pBufferNoTopBottomBlankRow, width*(*newHeight), pCharBuffer + width*TopBlankRowCount, width*(*newHeight));

	return pBufferNoTopBottomBlankRow;
}

void CSCDigitOCRImpl::SetEqulaTolerance(int tolerance)
{
	m_ColorOCRImpl.SetEqulaTolerance(tolerance);
}

#define _GetRValue(c)	((unsigned char)(((c) & 0x00FF0000) >> 16))		// Red color component
#define _GetGValue(c)	((unsigned char)(((c) & 0x0000FF00) >> 8))		// Green color component
#define _GetBValue(c)	((unsigned char)((c) & 0x000000FF))				// Blue color component


CSCColorOCRImpl::CSCColorOCRImpl()
	: m_iEqualTolerance(DEFAULT_TOLERANCE)
{
}


CSCColorOCRImpl::~CSCColorOCRImpl()
{
}

bool CSCColorOCRImpl::RGB32Equal(unsigned long rgb1, unsigned long rgb2)
{
	bool bRes = false;
	int tolerance = GetTolerance(rgb1, rgb2);
	if (tolerance <= m_iEqualTolerance)
	{
		bRes = true;
	}

	return bRes;
}

void CSCColorOCRImpl::SetEqulaTolerance(int tolerance)
{
	m_iEqualTolerance = tolerance;
}

int CSCColorOCRImpl::GetTolerance(unsigned long rgb1, unsigned long rgb2)
{
	//RGB色差公式：T = 开方（平方（R的差值绝对值） + 平方（G的差值绝对值） + 平方（B的差值绝对值））
	int distanceR = abs(_GetRValue(rgb1) - _GetRValue(rgb2));
	int distanceG = abs(_GetGValue(rgb1) - _GetGValue(rgb2));
	int distanceB = abs(_GetBValue(rgb1) - _GetBValue(rgb2));
	int tolerance = sqrt(distanceR*distanceR + distanceG*distanceG + distanceB*distanceB);

	return tolerance;
}

int CSCColorOCRImpl::GetOTSUThreshold(unsigned long rgb, unsigned long * buf, long bufLen)
{
	/*计算大津阈值*/

	const int ToleranceScale = 442;		// 色差范围0-441
	int ToleranceCount[ToleranceScale];
	float TolerancePro[ToleranceScale];
	int i, j, threshold = 0;

	for (i = 0; i < ToleranceScale; i++)
	{
		ToleranceCount[i] = 0;
		TolerancePro[i] = 0;
	}

	//统计每个像素与表定点颜色的色差在整幅图像中的个数  
	for (long l = 0; l < bufLen; l++)
	{
		ToleranceCount[GetTolerance(buf[l], rgb)]++;  //将像素值作为计数数组的下标
	}

	//计算每个像素色差在整幅图像中的比例  
	float maxPro = 0.0;
	int kk = 0;
	for (i = 0; i < ToleranceScale; i++)
	{
		TolerancePro[i] = (float)ToleranceCount[i] / bufLen;
		if (TolerancePro[i] > maxPro)
		{
			maxPro = TolerancePro[i];
			kk = i;
		}
	}

	//遍历色差[0,441]  
	float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
	for (i = 0; i < ToleranceScale; i++)     // i作为阈值
	{
		w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
		for (j = 0; j < ToleranceScale; j++)
		{
			if (j <= i)   //前景部分  
			{
				w0 += TolerancePro[j];
				u0tmp += j * TolerancePro[j];
			}
			else   //背景部分  
			{
				w1 += TolerancePro[j];
				u1tmp += j * TolerancePro[j];
			}
		}
		u0 = u0tmp / w0;
		u1 = u1tmp / w1;
		u = u0tmp + u1tmp;
		deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
		if (deltaTmp > deltaMax)
		{
			deltaMax = deltaTmp;
			threshold = i;
		}
	}

// 	//temp code
// 	if (threshold > 150)
// 	{
// 		threshold = 150;
// 	}
// 	else if (threshold < 50)
// 	{
// 		threshold = 50;
// 	}
	return threshold;
}
