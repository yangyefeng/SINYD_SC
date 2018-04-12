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
	/// @brief:  ��ȡ����������
	///*******************************************************
	const char * GetAreaName() const;
	///*******************************************************
	/// @name:   ChildArea::GetAreaID
	/// @author: caiquan
	/// @return: int
	/// @brief:  ��ȡ������id
	///*******************************************************
	int GetAreaID() const;
	///*******************************************************
	/// @name:   ChildArea::GetAreaType
	/// @author: caiquan
	/// @return: int
	/// @brief:  ��ȡ����������
	///*******************************************************
	int GetAreaType() const;
	///*******************************************************
	/// @name:   ChildArea::GetAreaValueType
	/// @author: caiquan
	/// @return: int
	/// @brief:  ��ȡ������ֵ����
	///*******************************************************
	int GetAreaValueType() const;
	///*******************************************************
	/// @name:   ChildArea::GetAreaX
	/// @author: caiquan
	/// @return: int
	/// @brief:  ��ȡ���������X����
	///*******************************************************
	int GetAreaX() const;
	///*******************************************************
	/// @name:   ChildArea::GetAreaY
	/// @author: caiquan
	/// @return: int
	/// @brief:  ��ȡ���������Y����
	///*******************************************************
	int GetAreaY() const;
	///*******************************************************
	/// @name:   ChildArea::GetAreaHeight
	/// @author: caiquan
	/// @return: int
	/// @brief:  ��ȡ������߶�
	///*******************************************************
	int GetAreaHeight() const;
	///*******************************************************
	/// @name:   ChildArea::GetAreaWidth
	/// @author: caiquan
	/// @return: int
	/// @brief:  ��ȡ��������
	///*******************************************************
	int GetAreaWidth() const;

	///*******************************************************
	/// @name:   ChildArea::SetAreaName
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][const char *]areaName
	/// @param:  [in][int]strLen
	/// @brief:  ��������������
	///*******************************************************
	CONFIGError SetAreaName(const char * areaName, int strLen);
	///*******************************************************
	/// @name:   ChildArea::SetAreaType
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]areaType
	/// @brief:  ��������������
	///*******************************************************
	CONFIGError SetAreaType(int areaType);
	///*******************************************************
	/// @name:   ChildArea::SetAreaValueType
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]areaValueType
	/// @brief:  ����������ֵ����
	///*******************************************************
	CONFIGError SetAreaValueType(int areaValueType);
	///*******************************************************
	/// @name:   ChildArea::SetAreaX
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in/out/inout][int]areaX
	/// @brief:  �������������X����
	///*******************************************************
	CONFIGError SetAreaX(int areaX);
	///*******************************************************
	/// @name:   ChildArea::SetAreaY
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]areaY
	/// @brief:  �������������Y����
	///*******************************************************
	CONFIGError SetAreaY(int areaY);
	///*******************************************************
	/// @name:   ChildArea::SetAreaHeight
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]areaHeight
	/// @brief:  ����������߶�
	///*******************************************************
	CONFIGError SetAreaHeight(int areaHeight);
	///*******************************************************
	/// @name:   ChildArea::SetAreaWidth
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]areaWidth
	/// @brief:  ������������
	///*******************************************************
	CONFIGError SetAreaWidth(int areaWidth);
	///*******************************************************
	/// @name:   ChildArea::ModifyAreaID
	/// @author: caiquan
	/// @return: int
	/// @param:  [in][int]newID
	/// @brief:  �޸�������id
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
	/// @brief:  �ͷ��Լ����ڲ�����
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
	/// @brief:  ��ȡʶ���������
	///*******************************************************
	const char * GetParameterName() const;
	///*******************************************************
	/// @name:   Parameter::GetParameterValue
	/// @author: caiquan
	/// @return: const char *
	/// @brief:  ��ȡʶ�����ֵ
	///*******************************************************
	const char * GetParameterValue() const;
	//const char * GetParameterValue(const char * parameterName) const;

	///*******************************************************
	/// @name:   Parameter::SetParameterName
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][const char *]parameterName
	/// @param:  [in][int]strLen
	/// @brief:  ����ʶ���������
	///*******************************************************
	CONFIGError SetParameterName(const char * parameterName, int strLen);
	///*******************************************************
	/// @name:   Parameter::SetParameterValue
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][const char *]parameterValue
	/// @param:  [in][int]strLen
	/// @brief:  ����ʶ�����ֵ
	///*******************************************************
	CONFIGError SetParameterValue(const char * parameterValue, int strLen);

private:
	char * parameterName;
	char * parameterValue;

	///*******************************************************
	/// @name:   Parameter::FreeSelf
	/// @author: caiquan
	/// @return: void
	/// @brief:  �ͷ��Լ��ڲ�����
	///*******************************************************
	void FreeSelf();
};

struct AreaMeasureFunction
{
	int method_type;	//1:��ֵ  2:��ֵ
	int v1_start_at;	//1:���Ͽ�ʼ 2:���Ͻ��� 3:���Ͽ�ʼ 4:���Ͻ���
	int v1_start_delay; //ms
	int v1_end_at;		//1:���Ͽ�ʼ 2:���Ͻ��� 3:���Ͽ�ʼ 4:���Ͻ���
	int v1_end_delay;	//ms
	int v1_calc_method;	//1:��һ��ֵ 2:���һ��ֵ 3:���ֵ 4:��Сֵ 5:ƽ��ֵ
	int v2_start_at;	//1:���Ͽ�ʼ 2:���Ͻ��� 3:���Ͽ�ʼ 4:���Ͻ���
	int v2_start_delay;	//ms
	int v2_end_at;		//1:���Ͽ�ʼ 2:���Ͻ��� 3:���Ͽ�ʼ 4:���Ͻ���
	int v2_end_delay;	//ms
	int v2_calc_method;	//1:��һ��ֵ 2:���һ��ֵ 3:���ֵ 4:��Сֵ 5:ƽ��ֵ
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
	/// @brief:  ��ȡ�궨��������
	///*******************************************************
	const char * GetMonitorAreaName() const;
	///*******************************************************
	/// @name:   MonitorArea::GetMonitorAreaID
	/// @author: caiquan
	/// @return: int
	/// @brief:  ��ȡ�궨����id
	///*******************************************************
	int GetMonitorAreaID() const;
	///*******************************************************
	/// @name:   MonitorArea::GetMonitorAreaType
	/// @author: caiquan
	/// @return: int
	/// @brief:  ��ȡ�궨��������
	///*******************************************************
	int GetMonitorAreaType() const;
	///*******************************************************
	/// @name:   MonitorArea::GetMonitorAreaValueType
	/// @author: caiquan
	/// @return: int
	/// @brief:  ��ȡ�궨����ֵ����
	///*******************************************************
	int GetMonitorAreaValueType() const;
	///*******************************************************
	/// @name:   MonitorArea::GetMonitorIsMonitor
	/// @author: caiquan
	/// @return: int
	/// @brief:  ��ȡ�궨�����Ƿ���
	///*******************************************************
	int GetMonitorIsMonitor() const;
	///*******************************************************
	/// @name:   MonitorArea::GetMonitorCheckPointX
	/// @author: caiquan
	/// @return: int
	/// @brief:  ��ȡ�궨����ʶ���X����
	///*******************************************************
	int GetMonitorCheckPointX() const;
	///*******************************************************
	/// @name:   MonitorArea::GetMonitorCheckPointY
	/// @author: caiquan
	/// @return: int
	/// @brief:  ��ȡ�궨����ʶ���Y����
	///*******************************************************
	int GetMonitorCheckPointY() const;
	///*******************************************************
	/// @name:   MonitorArea::GetMonitorCheckPointType
	/// @author: caiquan
	/// @return: int
	/// @brief:  ��ȡ�궨����ʶ�������
	///*******************************************************
	int GetMonitorCheckPointType() const;
	///*******************************************************
	/// @name:   MonitorArea::GetMonitorAreaX
	/// @author: caiquan
	/// @return: int
	/// @brief:  ��ȡ�궨�������X����
	///*******************************************************
	int GetMonitorAreaX() const;
	///*******************************************************
	/// @name:   MonitorArea::GetMonitorAreaY
	/// @author: caiquan
	/// @return: int
	/// @brief:  ��ȡ�궨�������Y����
	///*******************************************************
	int GetMonitorAreaY() const;
	///*******************************************************
	/// @name:   MonitorArea::GetMonitorAreaHeight
	/// @author: caiquan
	/// @return: int
	/// @brief:  ��ȡ�궨����߶�
	///*******************************************************
	int GetMonitorAreaHeight() const;
	///*******************************************************
	/// @name:   MonitorArea::GetMonitorAreaWidth
	/// @author: caiquan
	/// @return: int
	/// @brief:  ��ȡ�궨������
	///*******************************************************
	int GetMonitorAreaWidth() const;
	///*******************************************************
	/// @name:   MonitorArea::GetMonitorParaCount
	/// @author: caiquan
	/// @return: int
	/// @brief:  ��ȡ�궨����ʶ���������
	///*******************************************************
	int GetMonitorParaCount() const;
	///*******************************************************
	/// @name:   MonitorArea::GetMonitorChildCount
	/// @author: caiquan
	/// @return: int
	/// @brief:  ��ȡ�궨��������������
	///*******************************************************
	int GetMonitorChildCount() const;

	///*******************************************************
	/// @name:   MonitorArea::SetMonitorAreaName
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][const char *]monitorName
	/// @param:  [in][int]strLen
	/// @brief:  ���ñ궨��������
	///*******************************************************
	CONFIGError SetMonitorAreaName(const char * monitorName, int strLen);
	///*******************************************************
	/// @name:   MonitorArea::SetMonitorAreaType
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]areaType
	/// @brief:  ���ñ궨�������ͣ���ֵ�����Ż��ֵȣ�Ŀǰ��֧�ֺ���
	///*******************************************************
	CONFIGError SetMonitorAreaType(int areaType);
	///*******************************************************
	/// @name:   MonitorArea::SetMonitorAreaValueType
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]areaValueType
	/// @brief:  ���ñ궨����ֵ���ͣ����ֻ򿪹�
	///*******************************************************
	CONFIGError SetMonitorAreaValueType(int areaValueType);
	///*******************************************************
	/// @name:   MonitorArea::SetMonitorIsMonitor
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]isMonitor
	/// @brief:  ���ñ궨�����Ƿ���
	///*******************************************************
	CONFIGError SetMonitorIsMonitor(int isMonitor);
	///*******************************************************
	/// @name:   MonitorArea::SetMonitorCheckPointX
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]checkPointX
	/// @brief:  ���ñ궨����ʶ���X����
	///*******************************************************
	CONFIGError SetMonitorCheckPointX(int checkPointX);
	///*******************************************************
	/// @name:   MonitorArea::SetMonitorCheckPointY
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]checkPointY
	/// @brief:  ���ñ궨����ʶ���Y����
	///*******************************************************
	CONFIGError SetMonitorCheckPointY(int checkPointY);
	///*******************************************************
	/// @name:   MonitorArea::SetMonitorCheckPointType
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]checkPointType
	/// @brief:  ���ñ궨����ʶ�������
	///*******************************************************
	CONFIGError SetMonitorCheckPointType(int checkPointType);
	///*******************************************************
	/// @name:   MonitorArea::SetMonitorAreaX
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]areaX
	/// @brief:  ���ñ궨�������X����
	///*******************************************************
	CONFIGError SetMonitorAreaX(int areaX);
	///*******************************************************
	/// @name:   MonitorArea::SetMonitorAreaY
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]areaY
	/// @brief:  ���ñ궨�������Y����
	///*******************************************************
	CONFIGError SetMonitorAreaY(int areaY);
	///*******************************************************
	/// @name:   MonitorArea::SetMonitorAreaHeight
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in/out/inout][int]height
	/// @brief:  ���ñ궨����߶�
	///*******************************************************
	CONFIGError SetMonitorAreaHeight(int height);
	///*******************************************************
	/// @name:   MonitorArea::SetMonitorAreaWidth
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]width
	/// @brief:  ���ñ궨������
	///*******************************************************
	CONFIGError SetMonitorAreaWidth(int width);
	///*******************************************************
	/// @name:   MonitorArea::SetParameterCount
	/// @author: caiquan
	/// @return: int
	/// @param:  [in][int]newCount
	/// @brief:  ���ñ궨�����ʶ���������
	///*******************************************************
	int SetParameterCount(int newCount);
	///*******************************************************
	/// @name:   MonitorArea::SetChildCount
	/// @author: caiquan
	/// @return: int
	/// @param:  [in][int]newCount
	/// @brief:  ���ñ궨���������������
	///*******************************************************
	int SetChildCount(int newCount);
	///*******************************************************
	/// @name:   MonitorArea::ModifyMonitorAreaID
	/// @author: caiquan
	/// @return: int
	/// @param:  [][int]newID
	/// @brief:  �޸ı궨����id
	///*******************************************************
	int ModifyMonitorAreaID(int newID);

	///*******************************************************
	/// @name:   MonitorArea::GetParameters
	/// @author: caiquan
	/// @return: const vector<Parameter *>*
	/// @brief:  ��ȡ�궨�����ȫ��ʶ�����
	///*******************************************************
	const vector<Parameter *>* GetParameters() const;
	///*******************************************************
	/// @name:   MonitorArea::GetChildAreas
	/// @author: caiquan
	/// @return: const vector<ChildArea *>*
	/// @brief:  ��ȡ�궨�����е�ȫ��������
	///*******************************************************
	const vector<ChildArea *>* GetChildAreas() const;
	
	///*******************************************************
	/// @name:   MonitorArea::AddParameter
	/// @author: caiquan
	/// @return: const Parameter *
	/// @param:  [in][const char *]parameterName
	/// @param:  [in][const char *]parameterValue
	/// @brief:  ���ӱ궨�����ʶ�����
	///*******************************************************
	const Parameter * AddParameter(const char * parameterName, const char * parameterValue);
	///*******************************************************
	/// @name:   MonitorArea::AddChildArea
	/// @author: caiquan
	/// @return: const ChildArea *
	/// @param:  [in][const ChildArea *]newChild
	/// @brief:  ���ӱ궨������Ŀǰ�궨�ļ��У���������ʵ�����ݣ�
	///*******************************************************
	const ChildArea * AddChildArea(const ChildArea * newChild);
	///*******************************************************
	/// @name:   MonitorArea::DelParameter
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][const char *]parameterName
	/// @brief:  ��������ɾ���궨�����ʶ������б���
	///*******************************************************
	CONFIGError DelParameter(const char * parameterName);
	///*******************************************************
	/// @name:   MonitorArea::DelChildArea
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]childID
	/// @brief:  ����idɾ���궨������Ŀǰ�궨�ļ��У���������ʵ�����ݣ�
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
	/// @brief:  �ͷ��Լ����ڲ�����
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
	/// @brief:  ��ȡ������·����exe����
	///*******************************************************
	const char * GetSoftwarePath() const;
	///*******************************************************
	/// @name:   CCalibrationConfig::GetSoftwareName
	/// @author: caiquan
	/// @return: const char *
	/// @brief:  ��ȡ����������
	///*******************************************************
	const char * GetSoftwareName() const;
	///*******************************************************
	/// @name:   CCalibrationConfig::GetSoftwareType
	/// @author: caiquan
	/// @return: int
	/// @brief:  ��ȡ����������
	///*******************************************************
	int GetSoftwareType() const;
	///*******************************************************
	/// @name:   CCalibrationConfig::GetSoftwareVer
	/// @author: caiquan
	/// @return: const char *
	/// @brief:  ��ȡ�������汾
	///*******************************************************
	const char * GetSoftwareVer() const;
	//const char * GetExeName() const;
	///*******************************************************
	/// @name:   CCalibrationConfig::GetWindowName
	/// @author: caiquan
	/// @return: const char *
	/// @brief:  ��ȡ��������
	///*******************************************************
	const char * GetWindowName() const;
	///*******************************************************
	/// @name:   CCalibrationConfig::GetWindowClass
	/// @author: caiquan
	/// @return: const char *
	/// @brief:  ��ȡ��������
	///*******************************************************
	const char * GetWindowClass() const;
	///*******************************************************
	/// @name:   CCalibrationConfig::GetWindowHeight
	/// @author: caiquan
	/// @return: int
	/// @brief:  ��ȡ���ڸ߶�
	///*******************************************************
	int GetWindowHeight() const;
	///*******************************************************
	/// @name:   CCalibrationConfig::GetWindowWidth
	/// @author: caiquan
	/// @return: int
	/// @brief:  ��ȡ���ڿ��
	///*******************************************************
	int GetWindowWidth() const;
	///*******************************************************
	/// @name:   CCalibrationConfig::GetMonitorAreaCount
	/// @author: caiquan
	/// @return: int
	/// @brief:  ��ȡ�궨��������
	///*******************************************************
	int GetMonitorAreaCount() const;
	///*******************************************************
	/// @name:   CCalibrationConfig::GetMonitorArea
	/// @author: caiquan
	/// @return: const MonitorArea *
	/// @param:  [in][int]id
	/// @brief:  ����id��ȡ�궨����
	///*******************************************************
	MonitorArea * GetMonitorArea(int id) const;
	///*******************************************************
	/// @name:   CCalibrationConfig::GetIsAdmixtureToWater
	/// @author: caiquan
	/// @return: int
	/// @brief:  ��ȡ��Ӽ��Ƿ���ˮ
	///*******************************************************
	int GetIsAdmixtureToWater() const;

	///*******************************************************
	/// @name:   CCalibrationConfig::SetSoftwarePath
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][const char *]path
	/// @param:  [in][int]strLen
	/// @brief:  ���ü�����·����exe����
	///*******************************************************
	CONFIGError SetSoftwarePath(const char * path, int strLen);
	///*******************************************************
	/// @name:   CCalibrationConfig::SetSoftwareName
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][const char *]name
	/// @param:  [in][int]strLen
	/// @brief:  ���ü���������
	///*******************************************************
	CONFIGError SetSoftwareName(const char * name, int strLen);
	///*******************************************************
	/// @name:   CCalibrationConfig::SetSoftwareType
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]type
	/// @brief:  ���ü���������
	///*******************************************************
	CONFIGError SetSoftwareType(int type);
	///*******************************************************
	/// @name:   CCalibrationConfig::SetSoftwareVer
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][const char *]name
	/// @param:  [in][int]strLen
	/// @brief:  ���ü�������汾
	///*******************************************************
	CONFIGError SetSoftwareVer(const char * name, int strLen);
	//CONFIGError SetExeName(const char * name, int strLen);
	///*******************************************************
	/// @name:   CCalibrationConfig::SetWindowName
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][const char *]name
	/// @param:  [in][int]strLen
	/// @brief:  ���ô�������
	///*******************************************************
	CONFIGError SetWindowName(const char * name, int strLen);
	///*******************************************************
	/// @name:   CCalibrationConfig::SetWindowClass
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][const char *]className
	/// @param:  [in][int]strLen
	/// @brief:  ���ô�������
	///*******************************************************
	CONFIGError SetWindowClass(const char * className, int strLen);
	///*******************************************************
	/// @name:   CCalibrationConfig::SetWindowHeight
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]height
	/// @brief:  ���ô��ڸ߶�
	///*******************************************************
	CONFIGError SetWindowHeight(int height);
	///*******************************************************
	/// @name:   CCalibrationConfig::SetWindowWidth
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]width
	/// @brief:  ���ô��ڿ��
	///*******************************************************
	CONFIGError SetWindowWidth(int width);
	///*******************************************************
	/// @name:   CCalibrationConfig::SetIsAdmixtureToWater
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]isAdmixtureToWater
	/// @brief:  ������Ӽ��Ƿ���ˮ
	///*******************************************************
	CONFIGError SetIsAdmixtureToWater(int isAdmixtureToWater);

	///*******************************************************
	/// @name:   CCalibrationConfig::SetMonitorAreaCount
	/// @author: caiquan
	/// @return: int
	/// @param:  [in/out/inout][int]newCount
	/// @brief:  ���ñ궨�������еı궨�������
	///*******************************************************
	int SetMonitorAreaCount(int newCount);

	///*******************************************************
	/// @name:   CCalibrationConfig::GetMonitorAreas
	/// @author: caiquan
	/// @return: const vector<MonitorArea *>*
	/// @brief:  ��ȡ�������е�ȫ���궨����
	///*******************************************************
	const vector<MonitorArea *>* GetMonitorAreas() const;

	///*******************************************************
	/// @name:   CCalibrationConfig::AddMonitorArea
	/// @author: caiquan
	/// @return: const MonitorArea *
	/// @param:  [in][const MonitorArea *]newMonitor
	/// @brief:  ���Ӳ������еı궨����
	///*******************************************************
	const MonitorArea * AddMonitorArea(const MonitorArea * newMonitor);
	///*******************************************************
	/// @name:   CCalibrationConfig::DelMonitorArea
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]monitorID
	/// @brief:  ɾ���������еı궨����
	///*******************************************************
	CONFIGError DelMonitorArea(int monitorID);
	
	///*******************************************************
	/// @name:   CCalibrationConfig::ReloadConfig
	/// @author: caiquan
	/// @return: int
	/// @brief:  ��Ĭ��·�����ر궨�ļ���Ϣ
	///*******************************************************
	virtual int ReloadConfig();
	///*******************************************************
	/// @name:   CCalibrationConfig::ReloadConfig
	/// @author: caiquan
	/// @return: int
	/// @param:  [in][const string &]path
	/// @brief:  ��ָ��·�����ر궨�ļ���Ϣ
	///*******************************************************
	virtual int ReloadConfig(const string& path);
	///*******************************************************
	/// @name:   CCalibrationConfig::SaveConfig
	/// @author: caiquan
	/// @return: int
	/// @brief:  ����궨�ļ���Ĭ��·��
	///*******************************************************
	virtual int SaveConfig();
	///*******************************************************
	/// @name:   CCalibrationConfig::SaveConfig
	/// @author: caiquan
	/// @return: int
	/// @param:  [in][const string &]path
	/// @brief:  ����궨�ļ���ָ��·��
	///*******************************************************
	virtual int SaveConfig(const string& path);

	///*******************************************************
	/// @name:   CCalibrationConfig::LoadConfig
	/// @author: caiquan
	/// @return: int
	/// @param:  [in][const string &]path
	/// @brief:  ���ر궨�ļ���Ϣ
	///*******************************************************
	int LoadConfig(const string& path);
	///*******************************************************
	/// @name:   CCalibrationConfig::Save
	/// @author: caiquan
	/// @return: int
	/// @param:  [in][const string &]path
	/// @brief:  ����궨�ļ�
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
	/// @brief:  �ͷ��Լ����ڲ�����
	///*******************************************************
	void FreeSelf();
	///*******************************************************
	/// @name:   CCalibrationConfig::CreatCalibrationXML
	/// @author: caiquan
	/// @return: int
	/// @param:  [in][tinyxml2::XMLDocument *]pDoc
	/// @brief:  �����궨XML�ĵ�
	///*******************************************************
	int CreatCalibrationXML(tinyxml2::XMLDocument* pDoc);

};