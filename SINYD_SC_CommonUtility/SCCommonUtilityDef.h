#pragma once

#ifdef SC_COMMONUTILITY_DLL_EXPORT
#define SC_COMMONUTILITY_API __declspec(dllexport)
#else 
#define SC_COMMONUTILITY_API __declspec(dllimport)
#endif

#define SC_COMMONUTILITY_EXPORT_CALLCONV _cdecl
