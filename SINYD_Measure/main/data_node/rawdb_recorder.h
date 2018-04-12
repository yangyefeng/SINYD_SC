/////////////////////////////////////////////////////////////////////////////
/// @File name: rawdb_recorder.h
/// @Author: h
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	File Raw Recorder Class
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef RAWDB_RECORDER_H
#define RAWDB_RECORDER_H

#include "../data_node.h"

#include "../../Tool/syd_record_file.h"
#include "DBModule.h"
#include "SYDDataTransfer.h"

/**
  @brief 保存原始数据的工具类
  */
class RawDBRecorder: public DataNode
{
public:
	RawDBRecorder();

	virtual ~RawDBRecorder();

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

    virtual void StartTransfer();

    /**
      @brief 写当前数据内容
      */
    virtual void RecordRaw();
    virtual void RecordADRaw();

	/**
	@brief int转string
	*/
	virtual string GetString(const int n);

	/**
	@brief double转string
	*/
	virtual string GetString(const double n);

	virtual void LoadSensor(map<int, int> &mapSensor);
	virtual void LoadSBSSensor(map<int, int> &mapSensor);
	virtual void LoadTankSensor(map<int, int> &mapSensor);

protected:

    SydRecordFile m_rf;
	IDBOperator* m_pdbo;
    IDBOperator* m_pdbo_ad;
    bool m_bNewFile;
	bool m_bNewDB;

    vector<int> m_vecDigitalNodes; //数字节点的ID清单
    vector<int> m_vecAnalogNodes;  //模拟量节点的ID清单

    CSYDDataTransfer *m_pdTrans;
};

#endif // RAWDB_RECORDER_H
