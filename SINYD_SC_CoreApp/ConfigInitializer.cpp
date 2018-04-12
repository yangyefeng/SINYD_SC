/////////////////////////////////////////////////////////////////////////////
/// @File name: ConfigInitializer.cpp
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/01/19
/// @Description:
/// 		
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#include "ConfigInitializer.h"


CConfigInitializer::CConfigInitializer()
{
	m_SettingConfig.ReloadConfig();
	m_CalibrationConfig.ReloadConfig();
}

CConfigInitializer::~CConfigInitializer()
{
}

const CSettingConfig* CConfigInitializer::SettingConfig() const
{
	return &m_SettingConfig;
}

const CCalibrationConfig* CConfigInitializer::CalibrationConfig() const
{
	return &m_CalibrationConfig;
}


