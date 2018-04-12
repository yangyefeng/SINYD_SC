#include "SINYD_Measure/main/data_collector.h"
#include "CopyFile.h"
#include "SINYD_Measure/Tool/syd_time.h"
#include "MeasureDef.h"
#include "SINYD_Measure/Data/RWData/RWData.h"
#include "log.h"

#include "SYDDataTransfer.h"
using namespace std;
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
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace log4cplus;
using namespace std;

Logger logger;
int log_init(void)
{
        char *str="hello";
        int a=10;
        SharedAppenderPtr appendPtr(new RollingFileAppender("log_text.txt",1*1024*1024,1,true));  //文件名 文件的最大尺寸 最大记录文件数 缓冲刷新标志
        logger = Logger::getInstance("test");
        std::string pattern = "%D{%y/%m/%d %H:%M:%S-%Q}{ThreadID:%t} ->%m\r\n";
        std::auto_ptr<Layout> layout_(new PatternLayout(pattern));
        appendPtr->setLayout(layout_);     //把Layout对象附着在Appender对象上
        logger.addAppender(appendPtr);
        logger.setLogLevel(DEBUG_LOG_LEVEL);     //设置输出消息等级
    //    LOG4CPLUS_INFO(logger,"Info message");
     //   LOG4CPLUS_INFO(logger,a);
    //    LOG4CPLUS_DEBUG(logger,"Debug message");
     //   LOG4CPLUS_TRACE(logger,"Trace message");
    //    LOG4CPLUS_ERROR(logger,"Error message");
    //    LOG4CPLUS_WARN(logger,"Warning message");
     //   LOG4CPLUS_FATAL(logger,"fatal message");
        return 0;
}
*/

string createtime;

int main(int argc, char *argv[])
{
    //log_init();
    if ( 2 == argc && ( 0 == strcmp("-v",argv[1]) || 0 == strcmp("-V",argv[1])  ) ) {
        printf("Version: %s\n",VERSION_NUMBER);
        return 0;
    }
    SydTime tm;
    createtime = tm.ToString();
    CRWData rw;
    std::string strSysConfig = CONFIG_BASE_PATH;
    strSysConfig += CONFIG_SYS_FILE;
    int mac_type = 1;
    if ( 0 == (mac_type = rw.GetProfileInt("mix_machine_1","mix_machine_type",0,strSysConfig.c_str()))){
        return 0;
    }
    BackupConfig(tm.ToDirString(), mac_type);
    printf("[%d]Dir_Path:%s\n",mac_type,dirpath.c_str());


    DataCollector dc;
    dc.Initialize();
    dc.Start();

    return 0;
}

