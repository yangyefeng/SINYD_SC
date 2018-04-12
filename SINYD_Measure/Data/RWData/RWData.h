/////////////////////////////////////////////////////////////////////////////
/// @File name: RWData.h
/// @Author: h
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	Read/Write File Data Class
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef RWDATA_H
#define RWDATA_H

#include "../../Include/EmsasCommon.h"
#include "../../Include/SwDscuMmdef.h"
#include <stdio.h>

#define LINESIZE 128
#define MODE_DELETE_SECTION 1
#define MODE_DELETE_KEY     2
#define MODE_OVERWRITE_KEY  3
#define MODE_APPEND_SECTION 4
#define MODE_APPEND_KEY		5

class CRWData
{
public:
    CRWData();
    virtual ~CRWData();

    /**
    @brief 删除文件函数
    @param filename：要删除文件的名称（带路径）
    @return    void：无
    */
    void DeleteFile(char *filename);

	/**
	@brief 创建文件函数
	@param filename：要创建文件的名称（带路径）
	@return    bool：false:文件存在不需要创建；true：文件创建成功
	*/
	bool CreateFile(const char *filename, const char *wstr = NULL);

    /**
    @brief 清空文件中内容函数
    @param filename：要清空文件的名称（带路径）
    @return    void：无
    */
    void ClearFileContent(char *filename);

    /**
    @brief 将信息追加写入日志的函数
    @param logString：要写入的内容的字符串
    @param  filename：要写入文件的名称（带路径）
    @return     void：无
    */
    void InsertLog(char *logString,char *filePath);

    /**
    @brief 将信息追加写入日志的函数
    @param logString：要写入的内容的字符串
    @param  filename：要写入文件的名称（带路径）
    @return     void：无
    */
    void RepeatLog(char *logString,char *filePath);

    /**
    @brief 写配置文件的函数
    @param lpAppName：要想写入内容对应的标题（[App Name]）
    @param lpKeyName：要想写入内容对应的Key（Key Name=）
    @param  lpString：要想写入的内容（Key Name=lpString）
    @param  filename：要想写入文件名称（带路径）
    @return     bool：写入是否成功
    */
    bool WriteProfileString(char lpAppName[], char lpKeyName[], char lpString[], char lpFileName[]);

    /**
    @brief 从配置文件中获取String值
    @param        lpAppName：要想读取内容对应的标题（[App Name]）
    @param        lpKeyName：要想读取内容对应的Key（Key Name=）
    @param        lpDefault：默认字符串，未取出字符的，返回该字符串（表示获取失败的字符串）
    @param lpReturnedString：返回读取的内容字符串（Key Name=lpReturnedString）
    @param       lpFileName：要想读取文件名称（带路径）
    @return           INT32：返回获取字符串的长度
    */
    INT32 GetProfileString(const char lpAppName[], const char lpKeyName[], const char lpDefault[], char lpReturnedString[], const char lpFileName[]);

    /**
    @brief 从配置文件中获取Int值
    @param  lpAppName：要想读取内容对应的标题（[App Name]）
    @param  lpKeyName：要想读取内容对应的Key（Key Name=）
    @param   nDefault：默认数值，未取出int值时，返回该值（表示获取失败的数值）
    @param lpFileName：要想读取文件名称（带路径）
    @return     INT32：返回获取Int值
    */
    INT32 GetProfileInt(const char lpAppName[], const char lpKeyName[], int nDefault, const char lpFileName[]);

    /**
    @brief 将Debug的log信息写入到对应的文件中
    @param   logstr：要写入的log信息
    @param FileName：要写入文件名称（带路径）
    @return    void：无
    */
    void WriteDebugForLog(INT8 logstr[], INT8 FileName[]);

private:
    /**
    @brief 移除字符串中的空格
    @param     p：要移除字符串的指针
    @return void：返回移除空格后的字符串
    */
    char* remove_empty (char *p);
};

#endif //RWDATA_H
