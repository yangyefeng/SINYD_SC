/////////////////////////////////////////////////////////////////////////////
/// @File name: analog_calibrator.h
/// @Author: h
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
///     ��ȡģ�����ı궨��Ϣ
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef ANALOG_CALIBRATION_H
#define ANALOG_CALIBRATION_H

class AnalogCalibrator
{
public:
    AnalogCalibrator();
    virtual ~AnalogCalibrator();

    ///*******************************************************
    /// @name:   AnalogCalibrator::LoadConfiguration
    /// @author: h
    /// @return: void
    /// @param:  [in][const char *]szConfigurationFileName
    /// @param:  [in][const char *]szAppName
    /// @brief:  ����ģ�����궨��Ϣ
    ///*******************************************************
    virtual void LoadConfiguration(const char * szConfigurationFileName, const char * szAppName);

    ///*******************************************************
    /// @name:   AnalogCalibrator::Calibrate
    /// @author: h
    /// @return: double
    /// @param:  [in][double]iAnalogValue
    /// @brief:  ���ݱ궨����ģ����ת��
    ///*******************************************************
    virtual double Calibrate(double iAnalogValue)const;

	virtual int GetDecimal() const;
protected:
    double *m_piCalibration;
    INT32 m_iCalibrationCount;
    INT32 m_iDecimal;
};

#endif // ANALOG_CALIBRATION_H
