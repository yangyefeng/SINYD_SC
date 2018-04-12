/////////////////////////////////////////////////////////////////////////////
/// @File name: CommonDef.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 		Common define something for solution
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include <string>
#include <tchar.h>


using namespace std;

//Debug Switch
#ifndef SC_PLUGIN_NO_UNDEFINECHAR_RESULT
#define SC_PLUGIN_NO_UNDEFINECHAR_RESULT 0
#endif // !SC_PLUGIN_NO_UNDEFINECHAR_RESULT
#ifndef DEBUG_SAVE_PIC
#define DEBUG_SAVE_PIC 0
#endif // !DEBUG_SAVE_PIC




///*******************************************************
/// @name:   ws2s
/// @author: YaoDi
/// @return: std::string
/// @param:  [in/out/inout][const wstring &]ws
/// @param:  [in/out/inout][const char *]clocale
/// @brief:  convert wstring to string
///*******************************************************
inline string ws2s(const wstring& ws, const char* clocale = "")
{
	string curLocale = setlocale(LC_ALL, NULL);

	setlocale(LC_ALL, clocale);

	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = 2 * ws.size() + 1;
	char *_Dest = new char[_Dsize];
	memset(_Dest, 0, _Dsize);
	size_t ReturnValue = 0;
	wcstombs_s(&ReturnValue, _Dest, _Dsize, _Source, _Dsize);
	string result = _Dest;
	delete[]_Dest;

	setlocale(LC_ALL, curLocale.c_str());

	return result;
}

///*******************************************************
/// @name:   s2ws
/// @author: YaoDi
/// @return: std::wstring
/// @param:  [in][const string &]s
/// @param:  [in][const char *]clocale
/// @brief:  convert string to wstring
///*******************************************************
inline wstring s2ws(const string& s, const char* clocale = "")
{
	string curLocale = setlocale(LC_ALL, NULL);

	setlocale(LC_ALL, clocale);

	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;
	wchar_t *_Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	size_t ReturnValue = 0;
	mbstowcs_s(&ReturnValue, _Dest, _Dsize, _Source, _Dsize);
	wstring result = _Dest;
	delete[]_Dest;

	setlocale(LC_ALL, curLocale.c_str());

	return result;
}


