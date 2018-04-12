/////////////////////////////////////////////////////////////////////////////
/// @File name: IScreenReader.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 	Screen reader interface include
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#pragma once

class IScreenReader
{
public:
	virtual ~IScreenReader(){}
	///*******************************************************
	/// @name:   IScreenReader::GetScreenRatio
	/// @author: YaoDi
	/// @return: void
	/// @param:  [out][float *]widthRatio
	/// @param:  [out][float *]heightRatio
	/// @brief:  Get the resolution ratio between config and really 
	///*******************************************************
	virtual void GetScreenRatio(float* widthRatio, float* heightRatio) = 0;
	///*******************************************************
	/// @name:   IScreenReader::GetScreenArea
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
	/// @brief:  Get area buffer from screen capture. Afferent buffer' length must Not less than w * h. 
	///*******************************************************
	virtual void GetScreenArea(long x, long y, long w, long h, unsigned long* pdwBuf, long* inoutBufLength, long* outWidth, long* outHeight) = 0;

	///*******************************************************
	/// @name:   IScreenReader::GetWidth
	/// @author: YaoDi
	/// @return: long
	/// @brief:  Get the screen capture width
	///*******************************************************
	virtual long GetWidth() = 0;

	///*******************************************************
	/// @name:   IScreenReader::GetHeight
	/// @author: YaoDi
	/// @return: long
	/// @brief:  Get the screen capture height
	///*******************************************************
	virtual long GetHeight() = 0;

	///*******************************************************
	/// @name:   IScreenReader::GetPixel
	/// @author: YaoDi
	/// @return: unsigned long
	/// @param:  [in][long]x
	/// @param:  [in][long]y
	/// @brief:  Get one pixel in screen capture
	///*******************************************************
	virtual unsigned long GetPixel(long x, long y) = 0;
};