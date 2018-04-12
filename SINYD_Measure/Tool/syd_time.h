/////////////////////////////////////////////////////////////////////////////
/// @File name: syd_time.h
/// @Author: h
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	Time Manage Class
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef SYD_TIME_H
#define SYD_TIME_H

#if _WIN32
#else
#include <sys/time.h>
#endif
#include <string>
using namespace std;

#include "../Include/SwDscuMmdef.h"

class SydTime
{
public:
    SydTime();
    SydTime(const SydTime& tm);
    SydTime(const char *szTime);

    ///*******************************************************
    /// @name:   SydTime::GetYear
    /// @author: h
    /// @return: INT32
    /// @brief:  获取年
    ///*******************************************************
    INT32 GetYear();

    ///*******************************************************
    /// @name:   SydTime::GetMonth
    /// @author: h
    /// @return: INT32
    /// @brief:  获取月
    ///*******************************************************
    INT32 GetMonth();

    ///*******************************************************
    /// @name:   SydTime::GetDay
    /// @author: h
    /// @return: INT32
    /// @brief:  获取日
    ///*******************************************************
    INT32 GetDay();

    ///*******************************************************
    /// @name:   SydTime::GetHour
    /// @author: h
    /// @return: INT32
    /// @brief:  获取小时数
    ///*******************************************************
    INT32 GetHour();

    ///*******************************************************
    /// @name:   SydTime::GetMinute
    /// @author: h
    /// @return: INT32
    /// @brief:  获取分钟
    ///*******************************************************
    INT32 GetMinute();

    ///*******************************************************
    /// @name:   SydTime::GetSecond
    /// @author: h
    /// @return: INT32
    /// @brief:  获取秒数
    ///*******************************************************
    INT32 GetSecond();

    ///*******************************************************
    /// @name:   SydTime::GetMilliSecond
    /// @author: h
    /// @return: INT32
    /// @brief:  获取微妙
    ///*******************************************************
    INT32 GetMilliSecond();

    ///*******************************************************
    /// @name:   SydTime::GetTotalSeconds
    /// @author: h
    /// @return: INT64
    /// @brief:  获取总的秒数
    ///*******************************************************
    INT64 GetTotalSeconds();

    ///*******************************************************
    /// @name:   SydTime::Now
    /// @author: h
    /// @return: void
    /// @brief: 将自身时间设置原来当前时间 
    ///*******************************************************
    void Now();

    ///*******************************************************
    /// @name:   SydTime::Reset
    /// @author: h
    /// @return: void
    /// @brief:  重置时间初始化
    ///*******************************************************
    void Reset();

    ///*******************************************************
    /// @name:   SydTime::IsZero
    /// @author: h
    /// @return: bool
    /// @brief:  判断时间是否为零
    ///*******************************************************
    bool IsZero();

    ///*******************************************************
    /// @name:   SydTime::ToString
    /// @author: h
    /// @return: void
    /// @param:  [in][char *]szBuf
    /// @param:  [in][bool]bIncludeMilliSeconds
    /// @brief:  已字符串形式表示时间（年-月-日 时：分：秒.毫秒）
    ///*******************************************************
    void ToString(char* szBuf, bool bIncludeMilliSeconds=true)const;
    string ToString(bool bIncludeMilliSeconds=true) const;

    ///*******************************************************
    /// @name:   SydTime::ToShortString
    /// @author: h
    /// @return: void
    /// @param:  [in][char *]szBuf
    /// @param:  [in][bool]bIncludeMilliSeconds
    /// @brief:  已字符串形式表示时间（时：分：秒.毫秒）
    ///*******************************************************
    void ToShortString(char* szBuf, bool bIncludeMilliSeconds=true)const;
    string ToShortString(bool bIncludeMilliSeconds=true) const;

    ///*******************************************************
    /// @name:   SydTime::ToDirString
    /// @author: h
    /// @return: std::string
    /// @param:  [in][bool]bIncludeMilliSeconds
    /// @brief: 已文件命名形式表示时间 （年-月-日_时_分_秒_毫秒）
    ///*******************************************************
    string ToDirString(bool bIncludeMilliSeconds=true) const;


    void AddMilliSecond(int Msec);
    /**
      @brief 计算时间差（从秒为单位）
      */
    double operator-(const SydTime& another);

    /**
      @brief 比较函数
      */
    bool operator>(const SydTime& another);
    bool operator<(const SydTime& another);
	bool operator>=(const SydTime& another);
	bool operator<=(const SydTime& another);
	bool operator==(const SydTime& another);

protected:
#if _WIN32
	struct timeval
	{
		time_t      tv_sec;      /* seconds */
		long long   tv_usec;     /* microseconds */
	};
	struct timeval m_time;
#else
	struct timeval m_time;
#endif
    
};

#endif // SYD_TIME_H
