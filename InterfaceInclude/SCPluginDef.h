/////////////////////////////////////////////////////////////////////////////
/// @File name: SCPluginDef.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 	Plugin common define include
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#pragma once

#ifdef SC_PLUGIN_DLL_EXPORT
#define SC_PLUGIN_API __declspec(dllexport)
#else 
#define SC_PLUGIN_API __declspec(dllimport)
#endif

#define SC_PLUGIN_EXPORT_CALLCONV _cdecl

//Plugin type
#define SC_PLUGIN_TYPE_UNKNOWN 0
#define SC_PLUGIN_CAPABILITY_TYPE_VALVE 1
#define SC_PLUGIN_CAPABILITY_TYPE_LOADCELL 2


//Plugin Param Name
#define SC_LOADCELL_FONT_COLOR_PARAMNAME "FontColor"
#define SC_VALVE_OPEN_COLOR_PARAMNAME "OpenColor"
#define SC_COMMON_COLOR_THRESHOLD_PARAMNAME "ColorThreshold"
#define SC_VALVE_IS_COLOR_INVERT "IsColorInvert"

//Check type
#define SC_PLUGIN_CHECK_TYPE_UNKNOWN 0
#define SC_PLUGIN_CHECK_TYPE_RGBEQUAL 1

//Type define
typedef int SC_PLUGIN_ERROR;


//Error code define
#define SC_PLUGIN_SUCCESS 1
#define SC_PLUGIN_FAIL 0
#define SC_PLUGIN_NULLPTR -1
#define SC_PLUGIN_INVALIDPARAM -2
#define SC_PLUGIN_TASKNOTFOUND -10
#define SC_PLUGIN_THREADSTARTFAIL -11
#define SC_PLUGIN_ZEROTASKCOUNT -12





