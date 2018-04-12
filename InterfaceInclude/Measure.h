/////////////////////////////////////////////////////////////////////////////
/// @File name: Measure.h
/// @Author: h
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	Measure Interface
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include <string>
#include "../SINYD_Measure/main/data_collector.h"
using namespace std;

#if _WIN32
#ifdef MESURELOGIC_DLL_EXPORT
#define MESURELOGIC_API __declspec(dllexport)
#else 
#define MESURELOGIC_API __declspec(dllimport)
#endif
#else
#define MESURELOGIC_API
#endif

enum MESURELOGIC_ERROR
{
	ML_ERROR_OK,
	ML_ERROR_INIT_CONFIG,
	ML_ERROR_INIT_CREATE_STRUCT,
};

class MESURELOGIC_API CMeasureLogic
{
public:
	CMeasureLogic();
	virtual ~CMeasureLogic();


	///*******************************************************
	/// @name:   CMeasureLogic::Initialize
	/// @author: h
	/// @return: MESURELOGIC_ERROR
	/// @brief:  计量逻辑初始化函数，先备份配置文件，初始化计量
	///          逻辑相关信息
	///*******************************************************
	MESURELOGIC_ERROR Initialize();

	///*******************************************************
	/// @name:   CMeasureLogic::SetSrcData
	/// @author: h
	/// @return: void
	/// @param:  [in/out/inout][const string &]srcdata
	/// @brief:  接收每个信号量的信息的Json结构接口
	///*******************************************************
	void SetSrcData(const string &srcdata) ;

	///*******************************************************
	/// @name:   CMeasureLogic::EndFrame
	/// @author: h
	/// @return: void
	/// @brief:  接收一帧数据结束后执行计量动作
	///*******************************************************
	void EndFrame();

	///*******************************************************
	/// @name:   CMeasureLogic::IsRunning
	/// @author: h
	/// @return: bool
	/// @brief:  判断计量是否处于运行状态
	///*******************************************************
	bool IsRunning();

private:
	DataCollector *mp_dc;
};



