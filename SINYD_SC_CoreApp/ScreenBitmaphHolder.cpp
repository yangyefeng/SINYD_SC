/////////////////////////////////////////////////////////////////////////////
/// @File name: ScreenBitmaphHolder.cpp
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 		
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#include "ScreenBitmaphHolder.h"
#include <tchar.h>
#include "CommonDef.h"

CScreenBitmaphHolder::CScreenBitmaphHolder()
{
}


CScreenBitmaphHolder::~CScreenBitmaphHolder()
{
}

void CScreenBitmaphHolder::GetScreenArea(long x, long y, long w, long h, unsigned long* pdwBuf, long* inoutBufLength, long* outWidth, long* outHeight)
{
	CriticalSectionLock lock(m_bitmapCS);	//***unfinished*** can update to readwrite lock
	m_bitmap.GetPixels(x, y, w, h, pdwBuf, inoutBufLength, outWidth, outHeight);

#if DEBUG_SAVE_PIC
	{
		CBitmapEx bit;
		bit.Create(*outWidth, *outHeight);
		for (int i = 0; i < *outHeight; i++)
		{
			for (int j = 0; j < *outWidth; j++)
			{
				bit.SetPixel(j, i, pdwBuf[i* (*outWidth) + j]);
			}
		}
		WCHAR pstr[256] = { 0 };
		_stprintf_s(pstr, 256, L"D:\\tempBMP\\x%dy%dw%dh%d.bmp", x, y, w, h);
		bit.Save(pstr);
		
	}
#endif
}

unsigned long CScreenBitmaphHolder::GetPixel(long x, long y)
{
	CriticalSectionLock lock(m_bitmapCS);	//***unfinished*** can update to readwrite lock
	return m_bitmap.GetPixel(x, y);
}

void CScreenBitmaphHolder::SetScreen(HBITMAP hBitmap)
{
	CriticalSectionLock lock(m_bitmapCS);
	m_bitmap.Load(hBitmap);
#if DEBUG_SAVE_PIC
	m_bitmap.Save(L"D:\\tempBMP\\1.bmp");
#endif
}

long CScreenBitmaphHolder::GetWidth()
{
	return m_bitmap.GetWidth();
}

long CScreenBitmaphHolder::GetHeight()
{
	return m_bitmap.GetHeight();
}

void CScreenBitmaphHolder::GetScreenRatio(float * widthRatio, float * heightRatio)
{
	*widthRatio = m_widthRatio;
	*heightRatio = m_heightRatio;
}

void CScreenBitmaphHolder::SetScreenRatio(float widthRatio, float heightRatio)
{
	m_widthRatio = widthRatio;
	m_heightRatio = heightRatio;
}
