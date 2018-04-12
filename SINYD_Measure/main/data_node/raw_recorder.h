/////////////////////////////////////////////////////////////////////////////
/// @File name: raw_recorder.h
/// @Author: h
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	File Raw Recorder Class
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef RAW_RECORDER_H
#define RAW_RECORDER_H

#include "../data_node.h"

#include "../../Tool/syd_record_file.h"


/**
  @brief 保存原始数据的工具类
  */
class RawRecorder: public DataNode
{
public:
    RawRecorder();

    /**
    @overload
    */
    virtual void Initialize(DataProvider *pDataProvider);

    /**
    @overload
    */
    virtual void Execute();

    /**
    @overload
    */
    virtual void Release();


    /**
    @overload
    */
    virtual INT32 GetId();

    /**
    @overload
    */
    virtual vector<DataNode*>* GetChildren() ;

protected:
    /**
      @brief 写文件头
      */
    virtual void RecordRawHeader();

    /**
      @brief 写当前数据内容
      */
    virtual void RecordRaw();

protected:

    SydRecordFile m_rf;
    bool m_bNewFile;

    vector<INT32> m_vecDigitalNodes; //数字节点的ID清单
    vector<INT32> m_vecAnalogNodes;  //模拟量节点的ID清单
};

#endif // RAW_RECORDER_H
