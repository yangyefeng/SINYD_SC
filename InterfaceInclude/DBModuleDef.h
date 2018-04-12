/////////////////////////////////////////////////////////////////////////////
/// @File name: DBModuleDef.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	数据存储模块Define头文件
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#if !defined(_DB_MODULE_DEF_H)
#define _DB_MODULE_DEF_H

//DBModule Error Code
#define DBModule_SUCCESS 1
#define DBModule_FAIL 0
#define DBModule_DBTableAlreadyExisted -1
#define DBModule_DBTableNotExisted -2
#define DBModule_UNKNOWNDATATYPE -3
#define DBModule_WRONGDATACOUNT -4
#define DBModule_NULLDBPTR -5

#define DBModule_SQLiteErrorBase -10000
#define SQLiteError2DBModuleError(SQLiteError) ( ( SQLiteError ) * -1 + DBModule_SQLiteErrorBase)
#define DBModuleError2SQLiteError(DBModuleError) ( ( ( DBModuleError ) - DBModule_SQLiteErrorBase ) * -1 )


//DB Name Define
#define DB_RAW_RECORD "Raw_record"
#define DB_Measure_RECORD "Measure_record"


enum FieldDataType
{
	DATATYPE_UNKNOWN,
	DATATYPE_INTEGER,
	DATATYPE_NUMERIC,
	DATATYPE_FLOAT,
	DATATYPE_REAL,
	DATATYPE_CHAR,
	DATATYPE_TEXT,
	DATATYPE_BOOLEAN,
	DATATYPE_DATETIME,
	DATATYPE_TIMESTAMP,
	DATATYPE_BLOB
};

#ifdef _WIN32
	#ifdef SC_DB_MODULE_DLL_EXPORT
		#define SC_DB_MODULE_API __declspec(dllexport)
	#else 
		#define SC_DB_MODULE_API __declspec(dllimport)
	#endif
#else
	#define SC_DB_MODULE_API
#endif

#ifdef _WIN32
	#define SC_DB_MODULE_EXPORT_CALLCONV _cdecl
#else
	#define SC_DB_MODULE_EXPORT_CALLCONV
#endif // 




#endif //_DB_MODULE_DEF_H
