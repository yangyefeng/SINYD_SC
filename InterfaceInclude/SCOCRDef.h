/////////////////////////////////////////////////////////////////////////////
/// @File name: SCOCRDef.h
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
#include <memory>
#include <string>

using namespace std;

#ifdef SC_OCR_DLL_EXPORT
#define SC_OCR_API __declspec(dllexport)
#else 
#define SC_OCR_API __declspec(dllimport)
#endif

typedef int SC_OCR_ERROR;
#define SC_OCR_SUCCESS 1
#define SC_OCR_FAIL 0
#define SC_OCR_NULLPTR -1
#define SC_OCR_INVALIDPARAM -2
#define SC_OCR_EXISTS_UNRECOGNIZE -11

typedef struct stDigitModel
{
	stDigitModel(int w, int h, const char* pBuf) : width(w), height(h), m()
	{
		m = pBuf;
		id = -1;
	}

	int width;
	int height;
	string m;
	int id;
}DIGITMODEL, *PDIGITMODEL;

typedef struct stDigit
{
	stDigit(PDIGITMODEL pm, const string& c) : model(*pm), digit(c) {}
	//stDigit(const stDigit& d) : model(d.model), digit(d.digit) {}
	//stDigit& operator=(const stDigit& d){ model = d.model; digit = d.digit;}
	DIGITMODEL model;
	const string digit;
}DIGIT, *PDIGIT;

