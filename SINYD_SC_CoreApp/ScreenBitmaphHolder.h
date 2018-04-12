/////////////////////////////////////////////////////////////////////////////
/// @File name: ScreenBitmaphHolder.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 	Bitmap holder Header. Using BitmapEx. 
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "IScreenReader.h"
#include "IScreenWriter.h"
#include "..\SINYD_SC_CommonUtility\BitmapEx.h"
#include "ThreadUtil.h"

class CScreenBitmaphHolder : public IScreenReader, public IScreenWriter
{
public:
	CScreenBitmaphHolder();
	~CScreenBitmaphHolder();

	///*******************************************************
	/// @name:   CScreenBitmaphHolder::GetScreenArea
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][long]x
	/// @param:  [in][long]y
	/// @param:  [in][long]w
	/// @param:  [in][long]h
	/// @param:  [out][unsigned long *]pdwBuf
	/// @param:  [inout][long *]inoutBufLength
	/// @param:  [out][long *]outWidth
	/// @param:  [out][long *]outHeight
	/// @brief:  ��ȡ��ͼ�е���������
	///*******************************************************
	virtual void GetScreenArea(long x, long y, long w, long h, unsigned long* pdwBuf, long* inoutBufLength, long* outWidth, long* outHeight) override;
	///*******************************************************
	/// @name:   CScreenBitmaphHolder::GetPixel
	/// @author: YaoDi
	/// @return: unsigned long
	/// @param:  [in][long]x
	/// @param:  [in][long]y
	/// @brief:  ��ȡ��ͼ��һ������
	///*******************************************************
	virtual unsigned long GetPixel(long x, long y) override;
	///*******************************************************
	/// @name:   CScreenBitmaphHolder::SetScreen
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][HBITMAP]hBitmap
	/// @brief:  set��ͼ����
	///*******************************************************
	virtual void SetScreen(HBITMAP hBitmap) override;
	///*******************************************************
	/// @name:   CScreenBitmaphHolder::GetWidth
	/// @author: YaoDi
	/// @return: long
	/// @brief:  ��ȡ��ͼ���
	///*******************************************************
	virtual long GetWidth() override;
	///*******************************************************
	/// @name:   CScreenBitmaphHolder::GetHeight
	/// @author: YaoDi
	/// @return: long
	/// @brief:  ��ȡ��ͼ�߶�
	///*******************************************************
	virtual long GetHeight() override;
	///*******************************************************
	/// @name:   CScreenBitmaphHolder::GetScreenRatio
	/// @author: YaoDi
	/// @return: void
	/// @param:  [out][float *]widthRatio
	/// @param:  [out][float *]heightRatio
	/// @brief:  ��ȡ��ͼ��߱���ϵ��
	///*******************************************************
	virtual void GetScreenRatio(float* widthRatio, float* heightRatio) override;
	///*******************************************************
	/// @name:   CScreenBitmaphHolder::SetScreenRatio
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][float]widthRatio
	/// @param:  [in][float]heightRatio
	/// @brief:  ���ý�ͼ��߱���ϵ��
	///*******************************************************
	virtual void SetScreenRatio(float widthRatio, float heightRatio) override;

private:
	CCriticalSection m_bitmapCS;
	CBitmapEx m_bitmap;
	float m_widthRatio;
	float m_heightRatio;
};

