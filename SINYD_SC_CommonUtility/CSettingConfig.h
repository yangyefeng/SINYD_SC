#pragma once

#include <string>
#include "ConfigDef.h"

using namespace std;

#define SettingConfig_XML_Name "SCSettingConfig.xml"

class CONFIG_LIB CSettingConfig
{
public:
	CSettingConfig();
	virtual ~CSettingConfig();

	///*******************************************************
	/// @name:   CSettingConfig::GetSocketIP
	/// @author: caiquan
	/// @return: const char *
	/// @brief:  获取上位机IP地址
	///*******************************************************
	const char * GetSocketIP() const;
	///*******************************************************
	/// @name:   CSettingConfig::GetSoecketPort
	/// @author: caiquan
	/// @return: int
	/// @brief:  获取上位机socket端口
	///*******************************************************
	int GetSoecketPort() const;
	///*******************************************************
	/// @name:   CSettingConfig::GetCacheAdress
	/// @author: caiquan
	/// @return: const char *
	/// @brief:  获取截图保存路径
	///*******************************************************
	const char * GetCacheAdress() const;
	///*******************************************************
	/// @name:   CSettingConfig::GetCacheUsedSpace
	/// @author: caiquan
	/// @return: int
	/// @brief:  获取截图保存最大占用空间
	///*******************************************************
	int GetCacheUsedSpace() const;
	///*******************************************************
	/// @name:   CSettingConfig::GetCacheSaveTime
	/// @author: caiquan
	/// @return: int
	/// @brief:  获取截图保存最长时间
	///*******************************************************
	int GetCacheSaveTime() const;
	//const char * GetTargetSoftwarePath() const;
	//const char * GetTargetSoftwareName() const;
	//const char * GetTargetExeName() const;
	///*******************************************************
	/// @name:   CSettingConfig::GetMixingStationID
	/// @author: caiquan
	/// @return: const char *
	/// @brief:  获取拌和机id
	///*******************************************************
	const char * GetMixingStationID() const;
	///*******************************************************
	/// @name:   CSettingConfig::GetMixingProjectID
	/// @author: caiquan
	/// @return: const char *
	/// @brief:  获取项目id
	///*******************************************************
	const char * GetMixingProjectID() const;
	///*******************************************************
	/// @name:   CSettingConfig::GetMixingSectionID
	/// @author: caiquan
	/// @return: const char *
	/// @brief:  获取标段id
	///*******************************************************
	const char * GetMixingSectionID() const;
	///*******************************************************
	/// @name:   CSettingConfig::GetMixType
	/// @author: caiquan
	/// @return: int
	/// @brief:  获取拌和机类型
	///*******************************************************
	int GetMixType() const;
	///*******************************************************
	/// @name:   CSettingConfig::GetCollThreadCount
	/// @author: caiquan
	/// @return: int
	/// @brief:  获取采集线程数量
	///*******************************************************
	int GetCollThreadCount() const;
	///*******************************************************
	/// @name:   CSettingConfig::GetCollFreq
	/// @author: caiquan
	/// @return: int
	/// @brief:  获取采集频率
	///*******************************************************
	int GetCollFreq() const;
	
	///*******************************************************
	/// @name:   CSettingConfig::SetSocketIP
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][const char *]socketIP
	/// @param:  [in][int]strLen
	/// @brief:  设置上位机的IP地址
	///*******************************************************
	CONFIGError SetSocketIP(const char * socketIP, int strLen);
	///*******************************************************
	/// @name:   CSettingConfig::SetSoecketPort
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]soecketPort
	/// @brief:  设置上位机的socket端口
	///*******************************************************
	CONFIGError SetSoecketPort(int soecketPort);
	///*******************************************************
	/// @name:   CSettingConfig::SetCacheAdress
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][const char *]cacheAdress
	/// @param:  [in][int]strLen
	/// @brief:  设置截图保存路径
	///*******************************************************
	CONFIGError SetCacheAdress(const char * cacheAdress, int strLen);
	///*******************************************************
	/// @name:   CSettingConfig::SetCacheUsedSpace
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]cacheUsedSpace
	/// @brief:  设置截图保存的最大占用空间
	///*******************************************************
	CONFIGError SetCacheUsedSpace(int cacheUsedSpace);
	///*******************************************************
	/// @name:   CSettingConfig::SetCacheSaveTime
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]cacheSaveTime
	/// @brief:  设置截图保存的最大时间
	///*******************************************************
	CONFIGError SetCacheSaveTime(int cacheSaveTime);
	//CONFIGError SetTargetSoftwarePath(const char * targetSoftwarePath, int strLen);
	//CONFIGError SetTargetSoftwareName(const char * targetSoftwareName, int strLen);
	//CONFIGError SetTargetExeName(const char * targetExeName, int strLen);
	///*******************************************************
	/// @name:   CSettingConfig::SetMixingStationID
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][const char *]mixingStationID
	/// @param:  [in][int]strLen
	/// @brief:  设置拌和机id
	///*******************************************************
	CONFIGError SetMixingStationID(const char * mixingStationID, int strLen);
	///*******************************************************
	/// @name:   CSettingConfig::SetMixingProjectID
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][const char *]mixingProjectID
	/// @param:  [in][int]strLen
	/// @brief:  设置项目id
	///*******************************************************
	CONFIGError SetMixingProjectID(const char * mixingProjectID, int strLen);
	///*******************************************************
	/// @name:   CSettingConfig::SetMixingSectionID
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][const char *]mixingSectionID
	/// @param:  [in][int]strLen
	/// @brief:  设置标段id
	///*******************************************************
	CONFIGError SetMixingSectionID(const char * mixingSectionID, int strLen);
	///*******************************************************
	/// @name:   CSettingConfig::SetMixType
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]mixType
	/// @brief:  设置拌和机类型
	///*******************************************************
	CONFIGError SetMixType(int mixType);
	///*******************************************************
	/// @name:   CSettingConfig::SetCollThreadCount
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]threadCount
	/// @brief:  设置采集线程数量
	///*******************************************************
	CONFIGError SetCollThreadCount(int threadCount);
	///*******************************************************
	/// @name:   CSettingConfig::SetCollFreq
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]freq
	/// @brief:  设置采集频率
	///*******************************************************
	CONFIGError SetCollFreq(int freq);
	
	///*******************************************************
	/// @name:   CSettingConfig::ReloadConfig
	/// @author: caiquan
	/// @return: int
	/// @brief:  从设定XML文档加载设定信息
	///*******************************************************
	virtual int ReloadConfig();
	virtual int ReloadConfig(const string& path);
	///*******************************************************
	/// @name:   CSettingConfig::SaveConfig
	/// @author: caiquan
	/// @return: int
	/// @brief:  保存设定信息到XML
	///*******************************************************
	virtual int SaveConfig();
	virtual int SaveConfig(const string& path);

private:
	char * socketIP;
	int soecketPort;
	char * cacheAdress;
	int cacheUsedSpace;
	int cacheSaveTime;
	//char * targetSoftwarePath;
	//char * targetSoftwareName;
	//char * targetExeName;
	char * mixingStationID;
	char * mixingProjectID;
	char * mixingSectionID;
	int mixType;
	int collThreadCount;
	int collFreq;

	///*******************************************************
	/// @name:   CSettingConfig::CreatSettingXML
	/// @author: caiquan
	/// @return: int
	/// @param:  [in][tinyxml2::XMLDocument *]pDoc
	/// @brief:  创建设定XML文档
	///*******************************************************
	int CreatSettingXML(tinyxml2::XMLDocument* pDoc);
	//wstring s2ws(string s);
	//string ws2s(wstring cs);
	void ConstCharCopy(const char **targetChar, const char *sourceChar);
};
