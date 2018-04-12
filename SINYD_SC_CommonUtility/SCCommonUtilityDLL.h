#pragma once

//#include "ISCSettingConfig.h"
//#include "ISCDistortConfig.h"

#ifdef SC_COMMONUTILITY_DLL_EXPORT
#define SC_COMMONUTILITY_DLL_API __declspec(dllexport)
#else 
#define SC_COMMONUTILITY_DLL_API __declspec(dllimport)
#endif

extern "C"
{
	//SC_COMMONUTILITY_DLL_API ISCSettingConfig* SC_COMMONUTILITY_EXPORT_CALLCONV GetSCSettingConfig();

	//SC_COMMONUTILITY_DLL_API ISCDistortConfig* SC_COMMONUTILITY_EXPORT_CALLCONV GetSCDistortConfig();
}
