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
	/// @brief:  ��ȡ��λ��IP��ַ
	///*******************************************************
	const char * GetSocketIP() const;
	///*******************************************************
	/// @name:   CSettingConfig::GetSoecketPort
	/// @author: caiquan
	/// @return: int
	/// @brief:  ��ȡ��λ��socket�˿�
	///*******************************************************
	int GetSoecketPort() const;
	///*******************************************************
	/// @name:   CSettingConfig::GetCacheAdress
	/// @author: caiquan
	/// @return: const char *
	/// @brief:  ��ȡ��ͼ����·��
	///*******************************************************
	const char * GetCacheAdress() const;
	///*******************************************************
	/// @name:   CSettingConfig::GetCacheUsedSpace
	/// @author: caiquan
	/// @return: int
	/// @brief:  ��ȡ��ͼ�������ռ�ÿռ�
	///*******************************************************
	int GetCacheUsedSpace() const;
	///*******************************************************
	/// @name:   CSettingConfig::GetCacheSaveTime
	/// @author: caiquan
	/// @return: int
	/// @brief:  ��ȡ��ͼ�����ʱ��
	///*******************************************************
	int GetCacheSaveTime() const;
	//const char * GetTargetSoftwarePath() const;
	//const char * GetTargetSoftwareName() const;
	//const char * GetTargetExeName() const;
	///*******************************************************
	/// @name:   CSettingConfig::GetMixingStationID
	/// @author: caiquan
	/// @return: const char *
	/// @brief:  ��ȡ��ͻ�id
	///*******************************************************
	const char * GetMixingStationID() const;
	///*******************************************************
	/// @name:   CSettingConfig::GetMixingProjectID
	/// @author: caiquan
	/// @return: const char *
	/// @brief:  ��ȡ��Ŀid
	///*******************************************************
	const char * GetMixingProjectID() const;
	///*******************************************************
	/// @name:   CSettingConfig::GetMixingSectionID
	/// @author: caiquan
	/// @return: const char *
	/// @brief:  ��ȡ���id
	///*******************************************************
	const char * GetMixingSectionID() const;
	///*******************************************************
	/// @name:   CSettingConfig::GetMixType
	/// @author: caiquan
	/// @return: int
	/// @brief:  ��ȡ��ͻ�����
	///*******************************************************
	int GetMixType() const;
	///*******************************************************
	/// @name:   CSettingConfig::GetCollThreadCount
	/// @author: caiquan
	/// @return: int
	/// @brief:  ��ȡ�ɼ��߳�����
	///*******************************************************
	int GetCollThreadCount() const;
	///*******************************************************
	/// @name:   CSettingConfig::GetCollFreq
	/// @author: caiquan
	/// @return: int
	/// @brief:  ��ȡ�ɼ�Ƶ��
	///*******************************************************
	int GetCollFreq() const;
	
	///*******************************************************
	/// @name:   CSettingConfig::SetSocketIP
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][const char *]socketIP
	/// @param:  [in][int]strLen
	/// @brief:  ������λ����IP��ַ
	///*******************************************************
	CONFIGError SetSocketIP(const char * socketIP, int strLen);
	///*******************************************************
	/// @name:   CSettingConfig::SetSoecketPort
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]soecketPort
	/// @brief:  ������λ����socket�˿�
	///*******************************************************
	CONFIGError SetSoecketPort(int soecketPort);
	///*******************************************************
	/// @name:   CSettingConfig::SetCacheAdress
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][const char *]cacheAdress
	/// @param:  [in][int]strLen
	/// @brief:  ���ý�ͼ����·��
	///*******************************************************
	CONFIGError SetCacheAdress(const char * cacheAdress, int strLen);
	///*******************************************************
	/// @name:   CSettingConfig::SetCacheUsedSpace
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]cacheUsedSpace
	/// @brief:  ���ý�ͼ��������ռ�ÿռ�
	///*******************************************************
	CONFIGError SetCacheUsedSpace(int cacheUsedSpace);
	///*******************************************************
	/// @name:   CSettingConfig::SetCacheSaveTime
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]cacheSaveTime
	/// @brief:  ���ý�ͼ��������ʱ��
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
	/// @brief:  ���ð�ͻ�id
	///*******************************************************
	CONFIGError SetMixingStationID(const char * mixingStationID, int strLen);
	///*******************************************************
	/// @name:   CSettingConfig::SetMixingProjectID
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][const char *]mixingProjectID
	/// @param:  [in][int]strLen
	/// @brief:  ������Ŀid
	///*******************************************************
	CONFIGError SetMixingProjectID(const char * mixingProjectID, int strLen);
	///*******************************************************
	/// @name:   CSettingConfig::SetMixingSectionID
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][const char *]mixingSectionID
	/// @param:  [in][int]strLen
	/// @brief:  ���ñ��id
	///*******************************************************
	CONFIGError SetMixingSectionID(const char * mixingSectionID, int strLen);
	///*******************************************************
	/// @name:   CSettingConfig::SetMixType
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]mixType
	/// @brief:  ���ð�ͻ�����
	///*******************************************************
	CONFIGError SetMixType(int mixType);
	///*******************************************************
	/// @name:   CSettingConfig::SetCollThreadCount
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]threadCount
	/// @brief:  ���òɼ��߳�����
	///*******************************************************
	CONFIGError SetCollThreadCount(int threadCount);
	///*******************************************************
	/// @name:   CSettingConfig::SetCollFreq
	/// @author: caiquan
	/// @return: CONFIGError
	/// @param:  [in][int]freq
	/// @brief:  ���òɼ�Ƶ��
	///*******************************************************
	CONFIGError SetCollFreq(int freq);
	
	///*******************************************************
	/// @name:   CSettingConfig::ReloadConfig
	/// @author: caiquan
	/// @return: int
	/// @brief:  ���趨XML�ĵ������趨��Ϣ
	///*******************************************************
	virtual int ReloadConfig();
	virtual int ReloadConfig(const string& path);
	///*******************************************************
	/// @name:   CSettingConfig::SaveConfig
	/// @author: caiquan
	/// @return: int
	/// @brief:  �����趨��Ϣ��XML
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
	/// @brief:  �����趨XML�ĵ�
	///*******************************************************
	int CreatSettingXML(tinyxml2::XMLDocument* pDoc);
	//wstring s2ws(string s);
	//string ws2s(wstring cs);
	void ConstCharCopy(const char **targetChar, const char *sourceChar);
};
