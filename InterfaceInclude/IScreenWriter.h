/////////////////////////////////////////////////////////////////////////////
/// @File name: IScreenWriter.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 	Screen writer interface include
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include <Windows.h>

class IScreenWriter
{
public:
	virtual ~IScreenWriter(){}
	///*******************************************************
	/// @name:   IScreenWriter::SetScreen
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][HBITMAP]hBitmap
	/// @brief:  
	///*******************************************************
	virtual void SetScreen(HBITMAP hBitmap) = 0;
	///*******************************************************
	/// @name:   IScreenWriter::SetScreenRatio
	/// @author: YaoDi
	/// @return: void
	/// @param:  [in][float]widthRatio
	/// @param:  [in][float]heightRatio
	/// @brief:  
	///*******************************************************
	virtual void SetScreenRatio(float widthRatio, float heightRatio) = 0;
};
