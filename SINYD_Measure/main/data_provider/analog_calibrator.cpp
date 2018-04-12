#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <sstream>
using namespace std;

#include "../../Include/DebugLogger.h"
#include "../../Data/RWData/RWData.h"

#include "analog_calibrator.h"

AnalogCalibrator::AnalogCalibrator()
{
    m_piCalibration=NULL;
    m_iCalibrationCount=0;
    m_iDecimal=1;
}

AnalogCalibrator::~AnalogCalibrator(){
    if(m_piCalibration){
        delete m_piCalibration;
        m_piCalibration=NULL;
        m_iCalibrationCount=0;
    }
}

int CompareCalibration ( const void *a , const void *b ){
    double ia=((const double*)a)[0];
    double ib=((const double*)b)[0];

    if(ia>ib){
        return 1;
    }else if(ia==ib){
        return 0;
    }else{
        return -1;
    }
}
//读取标定配置文件
void AnalogCalibrator::LoadConfiguration( const char * szConfigurationFileName, const char * szAppName){
    if(m_piCalibration){
        delete m_piCalibration;
        m_piCalibration=NULL;
    }

    CRWData rw;
    char szBuf[LINESIZE];
    INT32 iCanId, iCount,i;
    double iAnalog,iValue;

    iCanId = rw.GetProfileInt(szAppName,"can_id",-1,szConfigurationFileName);

    //判断标定值的个数
    iCount=0;
    while(1){
        sprintf(szBuf,"calibration%d",(iCount+1));
        iAnalog=rw.GetProfileInt(szAppName,szBuf,-1,szConfigurationFileName);
        if(iAnalog<0){
            break;
        }
        iCount++;
    }
    //分配内存
    m_piCalibration=new double[iCount+iCount];
    m_iCalibrationCount=iCount;

    if(iCount<2){
        DebugErrLog("Invalid calibration node count: %d, appname=%s, canId=%d,config=%s\n",iCount,szAppName,iCanId,szConfigurationFileName);
    }

    //读取标定值
    char calibValue[LINESIZE];
    ostringstream osLog;
    for(i=0; i<iCount;i++){
        sprintf(szBuf,"calibration%d",(i+1));
        iAnalog=rw.GetProfileInt(szAppName,szBuf,-1,szConfigurationFileName);
        sprintf(szBuf,"calib_value%d",(i+1));
        rw.GetProfileString(szAppName,szBuf,"0",calibValue,szConfigurationFileName);
        iValue = atof(calibValue);
        //iValue=rw.GetProfileInt(szAppName,szBuf,0,szConfigurationFileName);
        m_piCalibration[i+i]=iAnalog;
        m_piCalibration[i+i+1]=iValue;
        DebugWarningLog("analog[%lf],value[%.3lf]\n",iAnalog,iValue);
    }
    //对标定值按从小到大排序
    qsort(m_piCalibration,iCount,sizeof(double)*2,CompareCalibration);

    //读取小数的位数
    iCount=rw.GetProfileInt(szAppName,"decimal",-1,szConfigurationFileName);
    m_iDecimal=1;
    for(i=0;i<iCount;i++){
        m_iDecimal *=10;
    }

    //输出LOG
    osLog<<"load calibration for "<<szAppName<<",canId="<<iCanId <<",decimal="<<m_iDecimal;
    //osLog<<",count="<<m_iCalibrationCount<<":";
    for(i=0; i<m_iCalibrationCount; i++){
        osLog<<" ("<<m_piCalibration[i+i]<<"->"<<m_piCalibration[i+i+1]<<")";
    }
    osLog<<endl;
    string strLog=osLog.str();
    DebugLog(strLog.c_str());

}

double AnalogCalibrator::Calibrate(double iAnalogValue)const{
    if( m_iCalibrationCount<2 ){
        return iAnalogValue;
    }

    INT32 decimal=m_iDecimal;
    INT32 iCalibrateIndex=m_iCalibrationCount; //

    //定位需要计算的模拟量在哪个区间内
    for ( UINT16 i = 0; i < m_iCalibrationCount; i++) {
        if ( iAnalogValue<m_piCalibration[i+i]  ) {
            iCalibrateIndex = i-1;
            break;
        }
    }

    if ( 0 == m_piCalibration[iCalibrateIndex+iCalibrateIndex] &&
         0 == m_piCalibration[iCalibrateIndex+iCalibrateIndex+1] &&
         0 >= m_piCalibration[iCalibrateIndex+iCalibrateIndex+3] ) {
        ++iCalibrateIndex;
//        DebugWarningLog("idx[%d][%lf]---ad1[%lf]value1[%lf] ad2[%lf]value2[%lf]\n",iCalibrateIndex,iAnalogValue,m_piCalibration[iCalibrateIndex+iCalibrateIndex],
//                        m_piCalibration[iCalibrateIndex+iCalibrateIndex+1],m_piCalibration[iCalibrateIndex+iCalibrateIndex+2],m_piCalibration[iCalibrateIndex+iCalibrateIndex+3]);
    }

    if(iCalibrateIndex>=(m_iCalibrationCount-1)){
        iCalibrateIndex=m_iCalibrationCount-2;
    }
    if(iCalibrateIndex<0){
        iCalibrateIndex=0;
    }

    //计算重量值
    //重量值 = ( 当前节点真实AD值 - 最低点AD值 ) * （ 最高点V - 最低点的V ） * 小数位 / ( 最高点AD - 最低点的AD ) + ( 最低点的V * 小数位 )
    double ad1=m_piCalibration[iCalibrateIndex+iCalibrateIndex];
    double value1=m_piCalibration[iCalibrateIndex+iCalibrateIndex+1];
    double ad2=m_piCalibration[iCalibrateIndex+iCalibrateIndex+2];
    double value2=m_piCalibration[iCalibrateIndex+iCalibrateIndex+3];
    double adf = ad2 - ad1;
	if (adf == 0)
		adf = 1;
    double ret_value = ( ( iAnalogValue - ad1 ) * ( value2 - value1 ) *decimal ) / adf + ( value1 * decimal );

    return ret_value;
}

int AnalogCalibrator::GetDecimal() const {
	return m_iDecimal;
}
