#include "syd_record_file.h"
#include "syd_time.h"
#include "MeasureDef.h"

SydRecordFile::SydRecordFile():ofstream()
{
}

bool SydRecordFile::CreateFile(const char *szFileNamePrefix){
    SydTime now;
    char szFile[128];
    INT32 counter=0;

    //查找一个不存在的文件名
    while( 1 ){
        counter++;
        sprintf(szFile,"%s%s-%d%02d%02d-%03d.txt",CONFIG_LOG_PATH,szFileNamePrefix,now.GetYear(),now.GetMonth(),now.GetDay(),counter);
        open(szFile,ios::in);
        if( is_open() ){
            //文件已经存在，关闭之,继续尝试打开下一个文件
            close();
        }else{
            //文件不存在，退出循环
            break;
        }
    }

    open(szFile,ios::app);

    return is_open();
}

bool SydRecordFile::SafeClose(){
    if(is_open()){
        flush();
        close();
    }

    return true;
}
