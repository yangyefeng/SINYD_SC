#pragma once

#include <string>
#include "ConfigDef.h"
#include <vector>

using namespace std;

#define CalibrationConfig_XML_Name "SCCalibrationConfig.xml"

class CONFIG_LIB ChildArea
{
public:
	ChildArea(int id);
	virtual ~ChildArea();

	///*******************************************************
	/// @name:   ChildArea::GetAreaName
	/// @author: caiquan
	/// @return: const char *
	/// @brief:  获取子区域名称
	///*******************************************************
	const char * GetAreaName() const;
	///*******************************************************
	/// @name:   ChildArea::GetAreaID
	/// @author: caiquan
	/// @return: int
	/// @brief:  获取子区域id
	///*******************************************************
	int GetAreaID() const;
	///*******************************************************
	/// @name:   ChildArea::GetAreaType
	/// @author: caiquan
	/// @return: int
	/// @brief:  获取子区域类型
	///*******************************************************
	int GetAreaType() const;
	///*******************************************************
	/// @name:   ChildArea::GetAreaValueType
	/// @author: caiquan
	/// @return: int
	/// @brief:  获取子区域值类型
	///*******************************************************
	int GetAreaValueType() const;
	///*******************************************************
	/// @name:   ChildArea::GetAreaX
	/// @author: caiquan
	/// @return: int
	/// @brief:  获取子区域起点X坐标
	///*******************************************************
	int GetAreaX() const;
	///*******************************************************
	/// @name:   ChildArea::GetAreaY
	/// @author: caiquan
	/// @return: int
	/// @brief:  获取子区域起点Y坐标
	///*******************************************************
	int GetAreaY() const;
	///*******************************************************
	/// @name:   ChildArea::GetAreaHeight
	/// @author: caiquan
	/// @return: int
	/// @brief:  获取子区域高度
	///*******************************************************
	int GetAreaHeight() const;
	///*******************************************************
	/// @name:   ChildArea::GetAreaWidth
	/// @author: caiquan
	/// @return: int
	/// @brief:  获取子区域宽度
	///*******************************************************
	int GetAreaWidth() const;

	///*******************************************************
	/// @name:   ChildArea::SetAreaName
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][const char *]areaName
	/// @param:  [in][int]strLen
	/// @brief:  设置子区域名称
	///*******************************************************
	CONFIGError SetAreaName(const char * areaName, int strLen);
	///*******************************************************
	/// @name:   ChildArea::SetAreaType
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]areaType
	/// @brief:  设置子区域类型
	///*******************************************************
	CONFIGError SetAreaType(int areaType);
	///*******************************************************
	/// @name:   ChildArea::SetAreaValueType
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]areaValueType
	/// @brief:  设置子区域值类型
	///*******************************************************
	CONFIGError SetAreaValueType(int areaValueType);
	///*******************************************************
	/// @name:   ChildArea::SetAreaX
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in/out/inout][int]areaX
	/// @brief:  设置子区域起点X坐标
	///*******************************************************
	CONFIGError SetAreaX(int areaX);
	///*******************************************************
	/// @name:   ChildArea::SetAreaY
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]areaY
	/// @brief:  设置子区域起点Y坐标
	///*******************************************************
	CONFIGError SetAreaY(int areaY);
	///*******************************************************
	/// @name:   ChildArea::SetAreaHeight
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]areaHeight
	/// @brief:  设置子区域高度
	///*******************************************************
	CONFIGError SetAreaHeight(int areaHeight);
	///*******************************************************
	/// @name:   ChildArea::SetAreaWidth
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]areaWidth
	/// @brief:  设置子区域宽度
	///*******************************************************
	CONFIGError SetAreaWidth(int areaWidth);
	///*******************************************************
	/// @name:   ChildArea::ModifyAreaID
	/// @author: caiquan
	/// @return: int
	/// @param:  [in][int]newID
	/// @brief:  修改子区域id
	///*******************************************************
	int ModifyAreaID(int newID);
	
private:
	char * areaName;
	int areaID;
	int areaType;
	int areaValueType;
	int areaX;
	int areaY;
	int areaHeight;
	int areaWidth;

	///*******************************************************
	/// @name:   ChildArea::FreeSelf
	/// @author: caiquan
	/// @return: void
	/// @brief:  释放自己的内部参数
	///*******************************************************
	void FreeSelf();
};

class CONFIG_LIB Parameter
{
public:
	Parameter();
	virtual ~Parameter();

	///*******************************************************
	/// @name:   Parameter::GetParameterName
	/// @author: caiquan
	/// @return: const char *
	/// @brief:  获取识别参数名称
	///*******************************************************
	const char * GetParameterName() const;
	///*******************************************************
	/// @name:   Parameter::GetParameterValue
	/// @author: caiquan
	/// @return: const char *
	/// @brief:  获取识别参数值
	///*******************************************************
	const char * GetParameterValue() const;
	//const char * GetParameterValue(const char * parameterName) const;

	///*******************************************************
	/// @name:   Parameter::SetParameterName
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][const char *]parameterName
	/// @param:  [in][int]strLen
	/// @brief:  设置识别参数名称
	///*******************************************************
	CONFIGError SetParameterName(const char * parameterName, int strLen);
	///*******************************************************
	/// @name:   Parameter::SetParameterValue
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][const char *]parameterValue
	/// @param:  [in][int]strLen
	/// @brief:  设置识别参数值
	///*******************************************************
	CONFIGError SetParameterValue(const char * parameterValue, int strLen);

private:
	char * parameterName;
	char * parameterValue;

	///*******************************************************
	/// @name:   Parameter::FreeSelf
	/// @author: caiquan
	/// @return: void
	/// @brief:  释放自己内部参数
	///*******************************************************
	void FreeSelf();
};

struct AreaMeasureFunction
{
	int method_type;	//1:单值  2:差值
	int v1_start_at;	//1:上料开始 2:上料结束 3:放料开始 4:放料结束
	int v1_start_delay; //ms
	int v1_end_at;		//1:上料开始 2:上料结束 3:放料开始 4:放料结束
	int v1_end_delay;	//ms
	int v1_calc_method;	//1:第一个值 2:最后一个值 3:最大值 4:最小值 5:平均值
	int v2_start_at;	//1:上料开始 2:上料结束 3:放料开始 4:放料结束
	int v2_start_delay;	//ms
	int v2_end_at;		//1:上料开始 2:上料结束 3:放料开始 4:放料结束
	int v2_end_delay;	//ms
	int v2_calc_method;	//1:第一个值 2:最后一个值 3:最大值 4:最小值 5:平均值
};

class CONFIG_LIB MonitorArea
{
public:
	MonitorArea(int id);
	virtual ~MonitorArea();

	///*******************************************************
	/// @name:   MonitorArea::GetMonitorAreaName
	/// @author: caiquan
	/// @return: const char *
	/// @brief:  获取标定区域名称
	///*******************************************************
	const char * GetMonitorAreaName() const;
	///*******************************************************
	/// @name:   MonitorArea::GetMonitorAreaID
	/// @author: caiquan
	/// @return: int
	/// @brief:  获取标定区域id
	///*******************************************************
	int GetMonitorAreaID() const;
	///*******************************************************
	/// @name:   MonitorArea::GetMonitorAreaType
	/// @author: caiquan
	/// @return: int
	/// @brief:  获取标定区域类型
	///*******************************************************
	int GetMonitorAreaType() const;
	///*******************************************************
	/// @name:   MonitorArea::GetMonitorAreaValueType
	/// @author: caiquan
	/// @return: int
	/// @brief:  获取标定区域值类型
	///*******************************************************
	int GetMonitorAreaValueType() const;
	///*******************************************************
	/// @name:   MonitorArea::GetMonitorIsMonitor
	/// @author: caiquan
	/// @return: int
	/// @brief:  获取标定区域是否监控
	///*******************************************************
	int GetMonitorIsMonitor() const;
	///*******************************************************
	/// @name:   MonitorArea::GetMonitorCheckPointX
	/// @author: caiquan
	/// @return: int
	/// @brief:  获取标定区域识别点X坐标
	///*******************************************************
	int GetMonitorCheckPointX() const;
	///*******************************************************
	/// @name:   MonitorArea::GetMonitorCheckPointY
	/// @author: caiquan
	/// @return: int
	/// @brief:  获取标定区域识别点Y坐标
	///*******************************************************
	int GetMonitorCheckPointY() const;
	///*******************************************************
	/// @name:   MonitorArea::GetMonitorCheckPointType
	/// @author: caiquan
	/// @return: int
	/// @brief:  获取标定区域识别点类型
	///*******************************************************
	int GetMonitorCheckPointType() const;
	///*******************************************************
	/// @name:   MonitorArea::GetMonitorAreaX
	/// @author: caiquan
	/// @return: int
	/// @brief:  获取标定区域起点X坐标
	///*******************************************************
	int GetMonitorAreaX() const;
	///*******************************************************
	/// @name:   MonitorArea::GetMonitorAreaY
	/// @author: caiquan
	/// @return: int
	/// @brief:  获取标定区域起点Y坐标
	///*******************************************************
	int GetMonitorAreaY() const;
	///*******************************************************
	/// @name:   MonitorArea::GetMonitorAreaHeight
	/// @author: caiquan
	/// @return: int
	/// @brief:  获取标定区域高度
	///*******************************************************
	int GetMonitorAreaHeight() const;
	///*******************************************************
	/// @name:   MonitorArea::GetMonitorAreaWidth
	/// @author: caiquan
	/// @return: int
	/// @brief:  获取标定区域宽度
	///*******************************************************
	int GetMonitorAreaWidth() const;
	///*******************************************************
	/// @name:   MonitorArea::GetMonitorParaCount
	/// @author: caiquan
	/// @return: int
	/// @brief:  获取标定区域识别参数数量
	///*******************************************************
	int GetMonitorParaCount() const;
	///*******************************************************
	/// @name:   MonitorArea::GetMonitorChildCount
	/// @author: caiquan
	/// @return: int
	/// @brief:  获取标定区域子区域数量
	///*******************************************************
	int GetMonitorChildCount() const;

	///*******************************************************
	/// @name:   MonitorArea::SetMonitorAreaName
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][const char *]monitorName
	/// @param:  [in][int]strLen
	/// @brief:  设置标定区域名称
	///*******************************************************
	CONFIGError SetMonitorAreaName(const char * monitorName, int strLen);
	///*******************************************************
	/// @name:   MonitorArea::SetMonitorAreaType
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]areaType
	/// @brief:  设置标定区域类型，数值、阀门或汉字等，目前不支持汉字
	///*******************************************************
	CONFIGError SetMonitorAreaType(int areaType);
	///*******************************************************
	/// @name:   MonitorArea::SetMonitorAreaValueType
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]areaValueType
	/// @brief:  设置标定区域值类型，数字或开关
	///*******************************************************
	CONFIGError SetMonitorAreaValueType(int areaValueType);
	///*******************************************************
	/// @name:   MonitorArea::SetMonitorIsMonitor
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]isMonitor
	/// @brief:  设置标定区域是否监控
	///*******************************************************
	CONFIGError SetMonitorIsMonitor(int isMonitor);
	///*******************************************************
	/// @name:   MonitorArea::SetMonitorCheckPointX
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]checkPointX
	/// @brief:  设置标定区域识别点X坐标
	///*******************************************************
	CONFIGError SetMonitorCheckPointX(int checkPointX);
	///*******************************************************
	/// @name:   MonitorArea::SetMonitorCheckPointY
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]checkPointY
	/// @brief:  设置标定区域识别点Y坐标
	///*******************************************************
	CONFIGError SetMonitorCheckPointY(int checkPointY);
	///*******************************************************
	/// @name:   MonitorArea::SetMonitorCheckPointType
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]checkPointType
	/// @brief:  设置标定区域识别点类型
	///*******************************************************
	CONFIGError SetMonitorCheckPointType(int checkPointType);
	///*******************************************************
	/// @name:   MonitorArea::SetMonitorAreaX
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]areaX
	/// @brief:  设置标定区域起点X坐标
	///*******************************************************
	CONFIGError SetMonitorAreaX(int areaX);
	///*******************************************************
	/// @name:   MonitorArea::SetMonitorAreaY
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]areaY
	/// @brief:  设置标定区域起点Y坐标
	///*******************************************************
	CONFIGError SetMonitorAreaY(int areaY);
	///*******************************************************
	/// @name:   MonitorArea::SetMonitorAreaHeight
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in/out/inout][int]height
	/// @brief:  设置标定区域高度
	///*******************************************************
	CONFIGError SetMonitorAreaHeight(int height);
	///*******************************************************
	/// @name:   MonitorArea::SetMonitorAreaWidth
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]width
	/// @brief:  设置标定区域宽度
	///*******************************************************
	CONFIGError SetMonitorAreaWidth(int width);
	///*******************************************************
	/// @name:   MonitorArea::SetParameterCount
	/// @author: caiquan
	/// @return: int
	/// @param:  [in][int]newCount
	/// @brief:  设置标定区域的识别参数数量
	///*******************************************************
	int SetParameterCount(int newCount);
	///*******************************************************
	/// @name:   MonitorArea::SetChildCount
	/// @author: caiquan
	/// @return: int
	/// @param:  [in][int]newCount
	/// @brief:  设置标定区域的子区域数量
	///*******************************************************
	int SetChildCount(int newCount);
	///*******************************************************
	/// @name:   MonitorArea::ModifyMonitorAreaID
	/// @author: caiquan
	/// @return: int
	/// @param:  [][int]newID
	/// @brief:  修改标定区域id
	///*******************************************************
	int ModifyMonitorAreaID(int newID);

	///*******************************************************
	/// @name:   MonitorArea::GetParameters
	/// @author: caiquan
	/// @return: const vector<Parameter *>*
	/// @brief:  获取标定区域的全部识别参数
	///*******************************************************
	const vector<Parameter *>* GetParameters() const;
	///*******************************************************
	/// @name:   MonitorArea::GetChildAreas
	/// @author: caiquan
	/// @return: const vector<ChildArea *>*
	/// @brief:  获取标定区域中的全部子区域
	///*******************************************************
	const vector<ChildArea *>* GetChildAreas() const;
	
	///*******************************************************
	/// @name:   MonitorArea::AddParameter
	/// @author: caiquan
	/// @return: const Parameter *
	/// @param:  [in][const char *]parameterName
	/// @param:  [in][const char *]parameterValue
	/// @brief:  增加标定区域的识别参数
	///*******************************************************
	const Parameter * AddParameter(const char * parameterName, const char * parameterValue);
	///*******************************************************
	/// @name:   MonitorArea::AddChildArea
	/// @author: caiquan
	/// @return: const ChildArea *
	/// @param:  [in][const ChildArea *]newChild
	/// @brief:  增加标定子区域（目前标定文件中，子区域无实际内容）
	///*******************************************************
	const ChildArea * AddChildArea(const ChildArea * newChild);
	///*******************************************************
	/// @name:   MonitorArea::DelParameter
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][const char *]parameterName
	/// @brief:  根据名称删除标定区域的识别参数列表项
	///*******************************************************
	CONFIGError DelParameter(const char * parameterName);
	///*******************************************************
	/// @name:   MonitorArea::DelChildArea
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]childID
	/// @brief:  根据id删除标定子区域（目前标定文件中，子区域无实际内容）
	///*******************************************************
	CONFIGError DelChildArea(int childID);

	AreaMeasureFunction GetMeasureFunction() const;
	CONFIGError SetMeasureFunction(const AreaMeasureFunction& func);

private:
	char * areaName;
	int areaID;
	int areaType;
	int areaValueType;
	int isMonitor;
	int checkPointX;
	int checkPointY;
	int checkPointType;
	int areaX;
	int areaY;
	int areaHeight;
	int areaWidth;
	
	int parameterCount;
	int childAreaCount;

	vector<Parameter *>  parameters;
	vector<ChildArea *>  childAreas;

	AreaMeasureFunction measure_function;

	///*******************************************************
	/// @name:   MonitorArea::FreeSelf
	/// @author: caiquan
	/// @return: void
	/// @brief:  释放自己的内部参数
	///*******************************************************
	void FreeSelf();
};

class CONFIG_LIB CCalibrationConfig
{
public:
	CCalibrationConfig();
	virtual ~CCalibrationConfig();

	///*******************************************************
	/// @name:   CCalibrationConfig::GetSoftwarePath
	/// @author: caiquan
	/// @return: const char *
	/// @brief:  获取监控软件路径和exe名称
	///*******************************************************
	const char * GetSoftwarePath() const;
	///*******************************************************
	/// @name:   CCalibrationConfig::GetSoftwareName
	/// @author: caiquan
	/// @return: const char *
	/// @brief:  获取监控软件名称
	///*******************************************************
	const char * GetSoftwareName() const;
	///*******************************************************
	/// @name:   CCalibrationConfig::GetSoftwareType
	/// @author: caiquan
	/// @return: int
	/// @brief:  获取监控软件类型
	///*******************************************************
	int GetSoftwareType() const;
	///*******************************************************
	/// @name:   CCalibrationConfig::GetSoftwareVer
	/// @author: caiquan
	/// @return: const char *
	/// @brief:  获取监控软件版本
	///*******************************************************
	const char * GetSoftwareVer() const;
	//const char * GetExeName() const;
	///*******************************************************
	/// @name:   CCalibrationConfig::GetWindowName
	/// @author: caiquan
	/// @return: const char *
	/// @brief:  获取窗口名称
	///*******************************************************
	const char * GetWindowName() const;
	///*******************************************************
	/// @name:   CCalibrationConfig::GetWindowClass
	/// @author: caiquan
	/// @return: const char *
	/// @brief:  获取窗口类名
	///*******************************************************
	const char * GetWindowClass() const;
	///*******************************************************
	/// @name:   CCalibrationConfig::GetWindowHeight
	/// @author: caiquan
	/// @return: int
	/// @brief:  获取窗口高度
	///*******************************************************
	int GetWindowHeight() const;
	///*******************************************************
	/// @name:   CCalibrationConfig::GetWindowWidth
	/// @author: caiquan
	/// @return: int
	/// @brief:  获取窗口宽度
	///*******************************************************
	int GetWindowWidth() const;
	///*******************************************************
	/// @name:   CCalibrationConfig::GetMonitorAreaCount
	/// @author: caiquan
	/// @return: int
	/// @brief:  获取标定区域数量
	///*******************************************************
	int GetMonitorAreaCount() const;
	///*******************************************************
	/// @name:   CCalibrationConfig::GetMonitorArea
	/// @author: caiquan
	/// @return: const MonitorArea *
	/// @param:  [in][int]id
	/// @brief:  根据id获取标定区域
	///*******************************************************
	MonitorArea * GetMonitorArea(int id) const;
	///*******************************************************
	/// @name:   CCalibrationConfig::GetIsAdmixtureToWater
	/// @author: caiquan
	/// @return: int
	/// @brief:  获取外加剂是否入水
	///*******************************************************
	int GetIsAdmixtureToWater() const;

	///*******************************************************
	/// @name:   CCalibrationConfig::SetSoftwarePath
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][const char *]path
	/// @param:  [in][int]strLen
	/// @brief:  设置监控软件路径和exe名称
	///*******************************************************
	CONFIGError SetSoftwarePath(const char * path, int strLen);
	///*******************************************************
	/// @name:   CCalibrationConfig::SetSoftwareName
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][const char *]name
	/// @param:  [in][int]strLen
	/// @brief:  设置监控软件名称
	///*******************************************************
	CONFIGError SetSoftwareName(const char * name, int strLen);
	///*******************************************************
	/// @name:   CCalibrationConfig::SetSoftwareType
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]type
	/// @brief:  设置监控软件类型
	///*******************************************************
	CONFIGError SetSoftwareType(int type);
	///*******************************************************
	/// @name:   CCalibrationConfig::SetSoftwareVer
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][const char *]name
	/// @param:  [in][int]strLen
	/// @brief:  设置监视软件版本
	///*******************************************************
	CONFIGError SetSoftwareVer(const char * name, int strLen);
	//CONFIGError SetExeName(const char * name, int strLen);
	///*******************************************************
	/// @name:   CCalibrationConfig::SetWindowName
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][const char *]name
	/// @param:  [in][int]strLen
	/// @brief:  设置窗口名称
	///*******************************************************
	CONFIGError SetWindowName(const char * name, int strLen);
	///*******************************************************
	/// @name:   CCalibrationConfig::SetWindowClass
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][const char *]className
	/// @param:  [in][int]strLen
	/// @brief:  设置窗口类名
	///*******************************************************
	CONFIGError SetWindowClass(const char * className, int strLen);
	///*******************************************************
	/// @name:   CCalibrationConfig::SetWindowHeight
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]height
	/// @brief:  设置窗口高度
	///*******************************************************
	CONFIGError SetWindowHeight(int height);
	///*******************************************************
	/// @name:   CCalibrationConfig::SetWindowWidth
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]width
	/// @brief:  设置窗口宽度
	///*******************************************************
	CONFIGError SetWindowWidth(int width);
	///*******************************************************
	/// @name:   CCalibrationConfig::SetIsAdmixtureToWater
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]isAdmixtureToWater
	/// @brief:  设置外加剂是否入水
	///*******************************************************
	CONFIGError SetIsAdmixtureToWater(int isAdmixtureToWater);

	///*******************************************************
	/// @name:   CCalibrationConfig::SetMonitorAreaCount
	/// @author: caiquan
	/// @return: int
	/// @param:  [in/out/inout][int]newCount
	/// @brief:  设置标定操作类中的标定区域计数
	///*******************************************************
	int SetMonitorAreaCount(int newCount);

	///*******************************************************
	/// @name:   CCalibrationConfig::GetMonitorAreas
	/// @author: caiquan
	/// @return: const vector<MonitorArea *>*
	/// @brief:  获取操作类中的全部标定区域
	///*******************************************************
	const vector<MonitorArea *>* GetMonitorAreas() const;

	///*******************************************************
	/// @name:   CCalibrationConfig::AddMonitorArea
	/// @author: caiquan
	/// @return: const MonitorArea *
	/// @param:  [in][const MonitorArea *]newMonitor
	/// @brief:  增加操作类中的标定区域
	///*******************************************************
	const MonitorArea * AddMonitorArea(const MonitorArea * newMonitor);
	///*******************************************************
	/// @name:   CCalibrationConfig::DelMonitorArea
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]monitorID
	/// @brief:  删除操作类中的标定区域
	///*******************************************************
	CONFIGError DelMonitorArea(int monitorID);
	
	///*******************************************************
	/// @name:   CCalibrationConfig::ReloadConfig
	/// @author: caiquan
	/// @return: int
	/// @brief:  从默认路径加载标定文件信息
	///*******************************************************
	virtual int ReloadConfig();
	///*******************************************************
	/// @name:   CCalibrationConfig::ReloadConfig
	/// @author: caiquan
	/// @return: int
	/// @param:  [in][const string &]path
	/// @brief:  从指定路径加载标定文件信息
	///*******************************************************
	virtual int ReloadConfig(const string& path);
	///*******************************************************
	/// @name:   CCalibrationConfig::SaveConfig
	/// @author: caiquan
	/// @return: int
	/// @brief:  保存标定文件到默认路径
	///*******************************************************
	virtual int SaveConfig();
	///*******************************************************
	/// @name:   CCalibrationConfig::SaveConfig
	/// @author: caiquan
	/// @return: int
	/// @param:  [in][const string &]path
	/// @brief:  保存标定文件到指定路径
	///*******************************************************
	virtual int SaveConfig(const string& path);

	///*******************************************************
	/// @name:   CCalibrationConfig::LoadConfig
	/// @author: caiquan
	/// @return: int
	/// @param:  [in][const string &]path
	/// @brief:  加载标定文件信息
	///*******************************************************
	int LoadConfig(const string& path);
	///*******************************************************
	/// @name:   CCalibrationConfig::Save
	/// @author: caiquan
	/// @return: int
	/// @param:  [in][const string &]path
	/// @brief:  保存标定文件
	///*******************************************************
	int Save(const string& path);

	int GetBackgroundCaptureType() const;
	CONFIGError SetBackgroundCaptureType(int type);

	
private:
	char * softwareName;
	char * softwareVer;
	char * softwarePath;
	//char * exeName;
	int softwareType;

	char * windowName;
	char * windowClass;
	int windowHeight;
	int windowWidth;

	int isAdmixtureToWater;

	int monitorAreaCount;
	vector<MonitorArea *>  monitorAreas;

	int backgroundCaptureType;

	///*******************************************************
	/// @name:   CCalibrationConfig::FreeSelf
	/// @author: caiquan
	/// @return: void
	/// @brief:  释放自己的内部参数
	///*******************************************************
	void FreeSelf();
	///*******************************************************
	/// @name:   CCalibrationConfig::CreatCalibrationXML
	/// @author: caiquan
	/// @return: int
	/// @param:  [in][tinyxml2::XMLDocument *]pDoc
	/// @brief:  创建标定XML文档
	///*******************************************************
	int CreatCalibrationXML(tinyxml2::XMLDocument* pDoc);

};