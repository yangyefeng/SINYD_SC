/////////////////////////////////////////////////////////////////////////////
/// @File name: ConfigInitializer.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 	������Ϣ��ʼ�����ڴ汣��
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ThreadUtil.h"
#include "..\SINYD_SC_CommonUtility\CSettingConfig.h"
#include "..\SINYD_SC_CommonUtility\CCalibrationConfig.h"


class CConfigInitializer
{
	DECLARE_SINGLETON_CLASS(CConfigInitializer);

private:
	CConfigInitializer(const CConfigInitializer&);
	CConfigInitializer& operator=(const CConfigInitializer&);

	CConfigInitializer();
	virtual ~CConfigInitializer();

public:
	///*******************************************************
	/// @name:   CConfigInitializer::SettingConfig
	/// @author: YaoDi
	/// @return: const CSettingConfig*
	/// @brief:  ��ȡ�趨������Ϣ����
	///*******************************************************
	const CSettingConfig* SettingConfig() const;
	///*******************************************************
	/// @name:   CConfigInitializer::CalibrationConfig
	/// @author: YaoDi
	/// @return: const CCalibrationConfig*
	/// @brief:  ��ȡ�궨������Ϣ����
	///*******************************************************
	const CCalibrationConfig* CalibrationConfig() const;

private:
	CSettingConfig m_SettingConfig;
	CCalibrationConfig m_CalibrationConfig;
};
typedef CSingleton<CConfigInitializer> ConfiginitializerInstance;

