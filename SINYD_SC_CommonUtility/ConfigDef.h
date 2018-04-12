#pragma once

#include "..\TinyXML2\tinyxml2.h"

#ifdef _WIN32
#   ifdef CONFIG_EXPORT
#       define CONFIG_LIB __declspec(dllexport)
#   elif defined(CONFIG_IMPORT)
#       define CONFIG_LIB __declspec(dllimport)
#   else
#       define CONFIG_LIB
#   endif
#else
#   define CONFIG_LIB
#endif

#if _DEBUG
#define ConfigFile_Folder "ConfigFile" //"../../ConfigFile"
#else
#define ConfigFile_Folder "ConfigFile"
#endif

#if _DEBUG
#define ConfigFile_Existing_Folder "ConfigFile/Existing" //"../../ConfigFile/Existing"
#else
#define ConfigFile_Existing_Folder "ConfigFile/Existing"
#endif

enum CONFIGError {
	CONFIG_SET_INT_FAILURE = -1, 
	CONFIG_SUCCESS = 0, 
	CONFIG_ERROR_NULL_PTR, 
	CONFIG_ERROR_ELEMENT_MISMATCH 
};
