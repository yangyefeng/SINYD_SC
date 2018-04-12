/////////////////////////////////////////////////////////////////////////////
/// @File name: DebugLogger.h
/// @Author: h
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	Log Manage Class
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef DEBUGLOGGER_H
#define DEBUGLOGGER_H

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#if _WIN32
#else
#include <sys/time.h>
#endif

#define RED "\33[;31m"
#define YELLOW "\33[;33m"
#define BLUE "\33[;34m"
#define COTROLCODE "\033[0m"

#define RELEASE

#ifdef  RELEASE
#ifdef __cplusplus
#if _WIN32
    #define PRN_TM0
#else
    #define PRN_TM0  {  \
            struct timeval __tv; \
            gettimeofday(&__tv, NULL); \
            tm* __t=localtime(&(__tv.tv_sec)); \
            printf("%d-%02d-%02d %02d:%02d:%02d.%03d ",__t->tm_year+1900,__t->tm_mon+1,__t->tm_mday,__t->tm_hour,__t->tm_min,__t->tm_sec,(int)(__tv.tv_usec/1000) ); \
        }
#endif
    #define PRN_TM(tv)  { \
            string s=tv.ToString(); \
            printf("%s ",s.c_str() ); \
        }
#else
    #define PRN_TM0
    #define PRN_TM
#endif

#define DebugLogWithTm(tm,format, ...) PRN_TM(tm); printf (format, ##__VA_ARGS__)
#define DebugLog(format, ...) PRN_TM0 printf (format, ##__VA_ARGS__)
#define DebugInfoLog(format, ...) PRN_TM0  printf (BLUE format COTROLCODE, ##__VA_ARGS__)
#define DebugErrLog(format, ...) PRN_TM0  printf (RED format COTROLCODE, ##__VA_ARGS__)
#define DebugWarningLog(format, ...) PRN_TM0  printf (YELLOW format COTROLCODE, ##__VA_ARGS__)
#else
#define DebugLog(format, ...) printf (format, ##__VA_ARGS__)
#define DebugInfoLog(format, ...) printf (format, ##__VA_ARGS__)
#define DebugErrLog(format, ...) printf (format, ##__VA_ARGS__)
#define DebugWarningLog(format, ...) printf (format, ##__VA_ARGS__)
#endif


#endif // DEBUGLOGGER_H
