/////////////////////////////////////////////////////////////////////////////
/// @File name: syd_record_file.h
/// @Author: h
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	File Operation Class
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef SYD_RECORD_FILE_H
#define SYD_RECORD_FILE_H

#include <string>
#include <fstream>

using namespace std;

class SydRecordFile:public ofstream
{
public:
    SydRecordFile();

    virtual bool CreateFile(const char *szFileNamePrefix);

    virtual bool SafeClose();

};

#endif // SYD_RECORD_FILE_H
