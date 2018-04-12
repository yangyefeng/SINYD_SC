/////////////////////////////////////////////////////////////////////////////
/// @File name: machine_loader.h
/// @Author: h
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef MACHINE_LOADER_H
#define MACHINE_LOADER_H

#include "data_node.h"

/**
  @brief 从配置文件中构造机器设备对象
  */
class MachineLoader
{
public:
    MachineLoader();

    virtual DataNode* Load(const char* szConfigPath)=0;
};

#endif // MACHINE_LOADER_H
