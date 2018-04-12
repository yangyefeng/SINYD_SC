#pragma once

#ifndef __SC_LOG_HEADER_
#define __SC_LOG_HEADER_

#include <stdio.h>
#include <time.h>
#include <string>
#include <stdarg.h>

using namespace std;

#define  __SC_LOG
#define  __SC_FILE_LOG
#define  __SC_LOG_INFO	0

#ifdef __SC_LOG
	#ifdef __SC_FILE_LOG
		#define __FILE_LOG

		#ifdef _WIN32
			#define LOG_FILE_NAME "SC.log"
		#else
			#define LOG_FILE_NAME "/media/mmcblk0p1/SC.log"
		#endif
	#else
		#define __OUTPUT_LOG
	#endif
#endif

#define MAXLEN 20480

inline void _InfoTraceA(const char* format, ...)
{
#ifdef __SC_LOG
#if __SC_LOG_INFO
	char ctime[26] = { 0 };
	time_t st_timet = time(NULL);
	tm *systime = localtime(&st_timet);
	strftime(ctime, sizeof(ctime), "%Y-%m-%d %H_%M_%S", systime);

	char clogHeader[MAXLEN] = { 0 };
	sprintf_s(clogHeader, MAXLEN, "[SC_LOG][%s]", ctime);
	char cArg[MAXLEN] = { 0 };

	va_list ap;
	va_start(ap, format);
	vsprintf_s(cArg, format, ap);
	va_end(ap);
	string strLog = string(clogHeader) + string(cArg) + '\n';
#ifdef __OUTPUT_LOG
	OutputDebugStringA(strLog.c_str());
#elif defined(__FILE_LOG)
	FILE* file = fopen(LOG_FILE_NAME, "a");
	if (file)
	{
		fprintf(file, strLog.c_str());
		fclose(file);
	}
#endif // __OUTPUT_LOG
#endif // __SC_LOG_INFO
#endif // __SC_LOG
}

inline void _ErrTraceA(const char* format, ...)
{
#ifdef __SC_LOG
	char ctime[26] = { 0 };
	time_t st_timet = time(NULL);
	tm *systime = localtime(&st_timet);
	strftime(ctime, sizeof(ctime), "%Y-%m-%d %H_%M_%S", systime);

	char clogHeader[MAXLEN] = { 0 };
    sprintf(clogHeader, "[SC_ERR][%s]", ctime);
	char cArg[MAXLEN] = { 0 };

	va_list ap;
	va_start(ap, format);
    vsprintf(cArg, format, ap);
	va_end(ap);
	string strLog = string(clogHeader) + string(cArg) + '\n';
#ifdef __OUTPUT_LOG
	OutputDebugStringA(strLog.c_str());
#elif defined(__FILE_LOG)
	FILE* file = NULL;
	file = fopen(LOG_FILE_NAME, "r");
	if (file)
	{
		fclose(file);
		file = fopen(LOG_FILE_NAME, "a");
		if (file)
		{
			fprintf(file, strLog.c_str());
			fclose(file);
		}
	}
	
#endif // __OUTPUT_LOG
#endif // __SC_LOG
}

#define SCINFO _InfoTraceA
#define SCERROR _ErrTraceA
/*
#include <iostream>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <iomanip>
#include <log4cplus/logger.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>
using namespace log4cplus;
extern Logger logger;
*/
#endif
