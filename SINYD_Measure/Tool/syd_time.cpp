#include "../Include/DebugLogger.h"
#include <time.h>
#if _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif
#include <stdio.h>

#include "syd_time.h"

#define abs(a) ((a)>=0?(a):(-(a)))

SydTime::SydTime()
{
    Now();
}

SydTime::SydTime(const SydTime &tm){
    m_time.tv_sec = tm.m_time.tv_sec;
    m_time.tv_usec = tm.m_time.tv_usec;
}

SydTime::SydTime(const char *szTime){
    INT32 year, month, day, hour, minute,second,millisecond=0;
    tm tmTime;

    sscanf(szTime, "%d-%d-%d %d:%d:%d.%d", &year, &month, &day, &hour, &minute, &second, &millisecond);

    tmTime.tm_year  = year-1900;
    tmTime.tm_mon   = month-1;
    tmTime.tm_mday  = day;
    tmTime.tm_hour  = hour;
    tmTime.tm_min   = minute;
    tmTime.tm_sec   = second;
    tmTime.tm_isdst = 0;

    m_time.tv_sec = mktime(&tmTime);
    m_time.tv_usec = millisecond*1000;
}

void SydTime::Reset(){
    m_time.tv_sec = 0;
    m_time.tv_usec = 0;
}

bool SydTime::IsZero(){
    return (m_time.tv_sec==0) && (m_time.tv_usec==0);
}

void SydTime::Now(){
#if _WIN32
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	m_time.tv_sec = time((time_t*)NULL);
	m_time.tv_usec = systime.wMilliseconds * 1000;    
#else
	gettimeofday(&m_time, NULL);
#endif
}

INT32 SydTime::GetYear(){
    tm* p=localtime(&(m_time.tv_sec));
    return  p->tm_year+1900;
}

INT32 SydTime::GetMonth(){
    tm* p=localtime(&(m_time.tv_sec));
    return  p->tm_mon+1;
}

INT32 SydTime::GetDay(){
    tm* p=localtime(&(m_time.tv_sec));
    return  p->tm_mday;
}

INT32 SydTime::GetHour(){
    tm* p=localtime(&(m_time.tv_sec));
    return  p->tm_hour;
}

INT32 SydTime::GetMinute(){
    tm* p=localtime(&(m_time.tv_sec));
    return  p->tm_min;
}

INT32 SydTime::GetSecond(){
    tm* p=localtime(&(m_time.tv_sec));
    return  p->tm_sec;
}

INT32 SydTime::GetMilliSecond(){
    return  (INT32)(m_time.tv_usec/1000);
}

INT64 SydTime::GetTotalSeconds(){
    return m_time.tv_sec;
}

void SydTime::ToString(char *szBuf, bool bIncludeMilliSeconds)const{
    tm* p=localtime(&(m_time.tv_sec));

    if(bIncludeMilliSeconds){
        sprintf(szBuf,"%d-%02d-%02d %02d:%02d:%02d.%03d",p->tm_year+1900,p->tm_mon+1,p->tm_mday, \
                p->tm_hour,p->tm_min,p->tm_sec,(int)(m_time.tv_usec/1000) );
    }else{
        sprintf(szBuf,"%d-%02d-%02d %02d:%02d:%02d",p->tm_year+1900,p->tm_mon+1,p->tm_mday, \
                p->tm_hour,p->tm_min,p->tm_sec);
    }
}

void SydTime::ToShortString(char *szBuf, bool bIncludeMilliSeconds)const{
    tm* p=localtime(&(m_time.tv_sec));

    if(bIncludeMilliSeconds){
        sprintf(szBuf,"%02d:%02d:%02d.%03d", p->tm_hour,p->tm_min,p->tm_sec,(int)(m_time.tv_usec/1000) );
    }else{
        sprintf(szBuf,"%02d:%02d:%02d", p->tm_hour,p->tm_min,p->tm_sec);
    }
}

string SydTime::ToString( bool bIncludeMilliSeconds) const{
    char szBuf[100];
    ToString(szBuf, bIncludeMilliSeconds);
    string s = szBuf;

    return s;
}

string SydTime::ToShortString( bool bIncludeMilliSeconds) const{
    char szBuf[100];
    ToShortString(szBuf, bIncludeMilliSeconds);
    string s = szBuf;

    return s;
}

string SydTime::ToDirString( bool bIncludeMilliSeconds) const{
    char szBuf[100];

    tm* p=localtime(&(m_time.tv_sec));

    if(bIncludeMilliSeconds){
        sprintf(szBuf,"%d-%02d-%02d_%02d_%02d_%02d_%03d",p->tm_year+1900,p->tm_mon+1,p->tm_mday, \
                p->tm_hour,p->tm_min,p->tm_sec,(int)(m_time.tv_usec/1000) );
    }else{
        sprintf(szBuf,"%d-%02d-%02d_%02d_%02d_%02d",p->tm_year+1900,p->tm_mon+1,p->tm_mday, \
                p->tm_hour,p->tm_min,p->tm_sec);
    }
    return string(szBuf);
}

double SydTime::operator-(const SydTime& another){
    if( this==&another){
        return 0.0;
    }

    double r= ( (float) (m_time.tv_sec - another.m_time.tv_sec)
            +(m_time.tv_usec - another.m_time.tv_usec)/1000000.0);

    return r;
}

bool SydTime::operator>(const SydTime& another){
    return (m_time.tv_sec > another.m_time.tv_sec) \
            || ((m_time.tv_sec == another.m_time.tv_sec) && (m_time.tv_usec > another.m_time.tv_usec));
}

bool SydTime::operator<(const SydTime& another){
    return (m_time.tv_sec < another.m_time.tv_sec) \
            || ((m_time.tv_sec == another.m_time.tv_sec) && (m_time.tv_usec < another.m_time.tv_usec));
}

bool SydTime::operator>=(const SydTime& another) {
	return ((m_time.tv_sec > another.m_time.tv_sec) \
		|| ((m_time.tv_sec == another.m_time.tv_sec) && (m_time.tv_usec > another.m_time.tv_usec))) \
		|| ((m_time.tv_sec == another.m_time.tv_sec) && (m_time.tv_usec == another.m_time.tv_usec));
}

bool SydTime::operator<=(const SydTime& another) {
	return ((m_time.tv_sec < another.m_time.tv_sec) \
		|| ((m_time.tv_sec == another.m_time.tv_sec) && (m_time.tv_usec < another.m_time.tv_usec))) \
		|| ((m_time.tv_sec == another.m_time.tv_sec) && (m_time.tv_usec == another.m_time.tv_usec));
}

bool SydTime::operator==(const SydTime& another) {
	return ((m_time.tv_sec == another.m_time.tv_sec) && (m_time.tv_usec == another.m_time.tv_usec));
}

void SydTime::AddMilliSecond(int Msec)
{
    int millsec= ((Msec % 1000)*1000);
    m_time.tv_sec += Msec / 1000;

    if ( 0 > Msec &&  1000 > m_time.tv_usec - abs(millsec) ) {
        m_time.tv_usec = (1000*1000)+m_time.tv_usec + millsec;
        m_time.tv_sec -= 1;
    } else if ( 0 < Msec && (1000*1000) < millsec + m_time.tv_usec ){
        m_time.tv_usec = millsec + m_time.tv_usec - (1000*1000);
        m_time.tv_sec += 1;
    } else {
        m_time.tv_usec += millsec;
    }
}
