#include "CSettingConfig.h"

CSettingConfig::CSettingConfig()
{
	soecketPort = 0;
	cacheUsedSpace = 0;
	cacheSaveTime = 0;
	mixType = -1;
	collThreadCount = 2;
	collFreq = 2;

	socketIP = NULL;
	cacheAdress = NULL;
	//targetSoftwarePath = NULL;
	//targetSoftwareName = NULL;
	//targetExeName = NULL;
	mixingStationID = NULL;
	mixingProjectID = NULL;
	mixingSectionID = NULL;
}

CSettingConfig::~CSettingConfig()
{
	if (socketIP)
	{
		delete[] socketIP;
		socketIP = nullptr;
	}
	if (cacheAdress)
	{
		delete[] cacheAdress;
		cacheAdress = nullptr;
	}
	if (mixingStationID)
	{
		delete[] mixingStationID;
		mixingStationID = nullptr;
	}
	if (mixingProjectID)
	{
		delete[] mixingProjectID;
		mixingProjectID = nullptr;
	}
	if (mixingSectionID)
	{
		delete[] mixingSectionID;
		mixingSectionID = nullptr;
	}
	/*if (targetExeName)
	{
		delete[] targetExeName;
		targetExeName = nullptr;
	}
	if (targetSoftwareName)
	{
		delete[] targetSoftwareName;
		targetSoftwareName = nullptr;
	}
	if (targetSoftwarePath)
	{
		delete[] targetSoftwarePath;
		targetSoftwarePath = nullptr;
	}*/
}

int CSettingConfig::ReloadConfig()
{
	string path = ConfigFile_Folder "/" SettingConfig_XML_Name;
	return ReloadConfig(path);
}

int CSettingConfig::ReloadConfig(const string& path)
{
	int result = 0;
	tinyxml2::XMLDocument docSet;

	try
	{
		//string path = ConfigFile_Folder "/" SettingConfig_XML_Name;

		docSet.LoadFile(path.c_str());
		if (docSet.ErrorID() == tinyxml2::XML_ERROR_FILE_NOT_FOUND)
		{
			if (CreatSettingXML(&docSet))
			{
				return tinyxml2::XML_ERROR_FILE_NOT_FOUND;
			}
			docSet.LoadFile(path.c_str());
		}

		tinyxml2::XMLElement* pSettingConfig = docSet.FirstChildElement("SC_SettingConfig");
		if (!docSet.ErrorID() && pSettingConfig)
		{
			const char * tempStr;
			int strTempLen;
			tinyxml2::XMLElement* pTransConfig = pSettingConfig->FirstChildElement("TransConfig");
			if (pTransConfig)
			{
				pTransConfig->SetAttribute("select", "Socket");
				if (!strcmp(pTransConfig->Attribute("select"), "Socket"))
				{
					tinyxml2::XMLElement* pSocket = pTransConfig->FirstChildElement("Socket");
					if (pSocket)
					{
						tempStr = pSocket->Attribute("IP");
						strTempLen = tempStr == NULL ? 0 : strlen(tempStr);
						this->SetSocketIP(tempStr, strTempLen);
						this->SetSoecketPort(pSocket->IntAttribute("Port"));
					}
				}
			}

			tinyxml2::XMLElement* pCacheConfig = pSettingConfig->FirstChildElement("CacheConfig");
			if (pCacheConfig)
			{
				tinyxml2::XMLElement* pAdress = pCacheConfig->FirstChildElement("Adress");
				if (pAdress)
				{
					if (pAdress->FirstChild() != NULL)
					{
						tempStr = pAdress->FirstChild()->ToText()->Value();
						strTempLen = tempStr == NULL ? 0 : strlen(tempStr);
						this->SetCacheAdress(tempStr, strTempLen);
					}
				}
				if (pCacheConfig->FirstChildElement("UsedSpace"))
				{
					this->SetCacheUsedSpace(pCacheConfig->FirstChildElement("UsedSpace")->IntAttribute("Big"));
				}
				if (pCacheConfig->FirstChildElement("SaveTime"))
				{
					this->SetCacheSaveTime(pCacheConfig->FirstChildElement("SaveTime")->IntAttribute("Long"));
				}
			}
			
			tinyxml2::XMLElement* pMixingStationConfig = pSettingConfig->FirstChildElement("MixingStationConfig");
			if (pMixingStationConfig)
			{
				if (pMixingStationConfig->FirstChildElement("StationID"))
				{
					if (pMixingStationConfig->FirstChildElement("StationID")->FirstChild() != NULL)
					{
						tempStr = pMixingStationConfig->FirstChildElement("StationID")->FirstChild()->ToText()->Value();
						strTempLen = tempStr == NULL ? 0 : strlen(tempStr);
						this->SetMixingStationID(tempStr, strTempLen);
					}
				}
				if (pMixingStationConfig->FirstChildElement("ProjectID"))
				{
					if (pMixingStationConfig->FirstChildElement("ProjectID")->FirstChild() != NULL)
					{
						tempStr = pMixingStationConfig->FirstChildElement("ProjectID")->FirstChild()->ToText()->Value();
						strTempLen = tempStr == NULL ? 0 : strlen(tempStr);
						this->SetMixingProjectID(tempStr, strTempLen);
					}
				}
				else
				{
					tinyxml2::XMLElement* ProjectID = docSet.NewElement("ProjectID");
					pMixingStationConfig->InsertEndChild(ProjectID);

					docSet.SaveFile(ConfigFile_Folder "/" SettingConfig_XML_Name);
				}
				if (pMixingStationConfig->FirstChildElement("SectionID"))
				{
					if (pMixingStationConfig->FirstChildElement("SectionID")->FirstChild() != NULL)
					{
						tempStr = pMixingStationConfig->FirstChildElement("SectionID")->FirstChild()->ToText()->Value();
						strTempLen = tempStr == NULL ? 0 : strlen(tempStr);
						this->SetMixingSectionID(tempStr, strTempLen);
					}
				}
				else
				{
					tinyxml2::XMLElement* SectionID = docSet.NewElement("SectionID");
					pMixingStationConfig->InsertEndChild(SectionID);

					docSet.SaveFile(ConfigFile_Folder "/" SettingConfig_XML_Name);
				}
				if (pMixingStationConfig->FirstChildElement("MixType"))
				{
					if (pMixingStationConfig->FirstChildElement("MixType")->FirstChild() != NULL)
					{
						int iTemp = 0;
						pMixingStationConfig->FirstChildElement("MixType")->QueryIntText(&iTemp);
						this->SetMixType(iTemp);
					}
				}
			}
			
			tinyxml2::XMLElement* pCollectConfig = pSettingConfig->FirstChildElement("CollectConfig");
			if (pCollectConfig)
			{
				if (pCollectConfig->FirstChildElement("ThreadCount"))
				{
					if (pCollectConfig->FirstChildElement("ThreadCount")->FirstChild() != NULL)
					{
						int iTemp = 0;
						pCollectConfig->FirstChildElement("ThreadCount")->QueryIntText(&iTemp);
						this->SetCollThreadCount(iTemp);
					}
				}
				if (pCollectConfig->FirstChildElement("Frequency"))
				{
					if (pCollectConfig->FirstChildElement("Frequency")->FirstChild() != NULL)
					{
						int iTemp = 0;
						pCollectConfig->FirstChildElement("Frequency")->QueryIntText(&iTemp);
						this->SetCollFreq(iTemp);
					}
				}
			}
		}
	}
	catch (exception * e)
	{
		result = docSet.ErrorID();
	}

	return result;
}

int CSettingConfig::SaveConfig()
{
	string path = ConfigFile_Folder "/" SettingConfig_XML_Name;
	return SaveConfig(path);
}

int CSettingConfig::SaveConfig(const string& path)
{
	int result = 0;
	tinyxml2::XMLDocument docSet;

    try
    {
		//string path = ConfigFile_Folder "/" SettingConfig_XML_Name;
		docSet.LoadFile(path.c_str());
		if (docSet.ErrorID() == tinyxml2::XML_ERROR_FILE_NOT_FOUND)
		{
			if (CreatSettingXML(&docSet))
			{
				//MessageBox(_T("找不到配置文件，并且创建配置文件失败"), _T("错误"), MB_OK);
				return tinyxml2::XML_ERROR_FILE_NOT_FOUND;
			}
		}

		tinyxml2::XMLElement* pSettingConfig = docSet.FirstChildElement("SC_SettingConfig");
		if (pSettingConfig)
		{
			tinyxml2::XMLElement* pTransConfig = pSettingConfig->FirstChildElement("TransConfig");
			if (pTransConfig)
			{
				pTransConfig->SetAttribute("select", "Socket");
				if (pTransConfig->FirstChildElement("Socket"))
				{
					pTransConfig->FirstChildElement("Socket")->SetAttribute("IP", this->socketIP);
					pTransConfig->FirstChildElement("Socket")->SetAttribute("Port", this->soecketPort);
				}
			}
			
			tinyxml2::XMLElement* pCacheConfig = pSettingConfig->FirstChildElement("CacheConfig");
			if (pCacheConfig)
			{
				if (pCacheConfig->FirstChildElement("Adress"))
				    pCacheConfig->FirstChildElement("Adress")->SetText(this->cacheAdress);
				if (pCacheConfig->FirstChildElement("UsedSpace"))
					pCacheConfig->FirstChildElement("UsedSpace")->SetAttribute("Big", this->cacheUsedSpace);
				if (pCacheConfig->FirstChildElement("SaveTime"))
					pCacheConfig->FirstChildElement("SaveTime")->SetAttribute("Long", this->cacheSaveTime);
			}
			
			//pSettingConfig->FirstChildElement("TargetSoftwareConfig")->FirstChildElement("Path")->SetText(this->targetSoftwarePath);
			//pSettingConfig->FirstChildElement("TargetSoftwareConfig")->FirstChildElement("SoftwareName")->SetText(this->targetSoftwareName);
			//pSettingConfig->FirstChildElement("TargetSoftwareConfig")->FirstChildElement("ExeName")->SetText(this->targetExeName);

			tinyxml2::XMLElement* pMixingStationConfig = pSettingConfig->FirstChildElement("MixingStationConfig");
			if (pMixingStationConfig)
			{
				if (pMixingStationConfig->FirstChildElement("StationID"))
					pMixingStationConfig->FirstChildElement("StationID")->SetText(this->mixingStationID);
				if (pMixingStationConfig->FirstChildElement("ProjectID"))
					pMixingStationConfig->FirstChildElement("ProjectID")->SetText(this->mixingProjectID);
				if (pMixingStationConfig->FirstChildElement("SectionID"))
					pMixingStationConfig->FirstChildElement("SectionID")->SetText(this->mixingSectionID);
				if (pMixingStationConfig->FirstChildElement("MixType"))
					pMixingStationConfig->FirstChildElement("MixType")->SetText(this->mixType);
			}

			tinyxml2::XMLElement* pCollectConfig = pSettingConfig->FirstChildElement("CollectConfig");
			if (pCollectConfig)
			{
				if (pCollectConfig->FirstChildElement("ThreadCount"))
					pCollectConfig->FirstChildElement("ThreadCount")->SetText(this->collThreadCount);
				if (pCollectConfig->FirstChildElement("Frequency"))
					pCollectConfig->FirstChildElement("Frequency")->SetText(this->collFreq);
			}

			docSet.SaveFile(path.c_str());
		}
		else
		{
			result = tinyxml2::XML_ERROR_PARSING_ELEMENT;
		}
    }
    catch (exception* e)
    {
		result = docSet.ErrorID();
    }

	return result;
}

int CSettingConfig::CreatSettingXML(tinyxml2::XMLDocument* pDoc)
{
	int result = 0;
	try
	{
		//pDoc->SetBOM(true);
		//tinyxml2::XMLDeclaration* pDeclear = pDoc->NewDeclaration();
		//pDoc->InsertFirstChild(pDeclear);

		tinyxml2::XMLElement* parent = pDoc->NewElement("SC_SettingConfig");
		pDoc->InsertEndChild(parent);

		tinyxml2::XMLElement* TransConfig = pDoc->NewElement("TransConfig");
		tinyxml2::XMLElement* CacheConfig = pDoc->NewElement("CacheConfig");
		tinyxml2::XMLElement* TargetSoftwareConfig = pDoc->NewElement("TargetSoftwareConfig");
		tinyxml2::XMLElement* MixingStationConfig = pDoc->NewElement("MixingStationConfig");
		tinyxml2::XMLElement* CollectConfig = pDoc->NewElement("CollectConfig");

		tinyxml2::XMLElement* Socket = pDoc->NewElement("Socket");
		tinyxml2::XMLElement* Adress = pDoc->NewElement("Adress");
		tinyxml2::XMLElement* UsedSpace = pDoc->NewElement("UsedSpace");
		tinyxml2::XMLElement* SaveTime = pDoc->NewElement("SaveTime");
		tinyxml2::XMLElement* Path = pDoc->NewElement("Path");
		tinyxml2::XMLElement* SoftwareName = pDoc->NewElement("SoftwareName");
		tinyxml2::XMLElement* ExeName = pDoc->NewElement("ExeName");
		tinyxml2::XMLElement* StationID = pDoc->NewElement("StationID");
		tinyxml2::XMLElement* ProjectID = pDoc->NewElement("ProjectID");
		tinyxml2::XMLElement* SectionID = pDoc->NewElement("SectionID");
		tinyxml2::XMLElement* MixType = pDoc->NewElement("MixType");
		tinyxml2::XMLElement* ThreadCount = pDoc->NewElement("ThreadCount");
		tinyxml2::XMLElement* Frequency = pDoc->NewElement("Frequency");

		tinyxml2::XMLNode* childNode = parent->InsertFirstChild(TransConfig);
		parent->InsertEndChild(CacheConfig);
		parent->InsertEndChild(TargetSoftwareConfig);
		parent->InsertEndChild(MixingStationConfig);
		parent->InsertEndChild(CollectConfig);

		parent->FirstChildElement("TransConfig")->InsertFirstChild(Socket);
		parent->FirstChildElement("CacheConfig")->InsertFirstChild(Adress);
		parent->FirstChildElement("CacheConfig")->InsertEndChild(UsedSpace);
		parent->FirstChildElement("CacheConfig")->InsertEndChild(SaveTime);
		//parent->FirstChildElement("TargetSoftwareConfig")->InsertFirstChild(Path);
		//parent->FirstChildElement("TargetSoftwareConfig")->InsertEndChild(SoftwareName);
		//parent->FirstChildElement("TargetSoftwareConfig")->InsertEndChild(ExeName);
		parent->FirstChildElement("MixingStationConfig")->InsertFirstChild(StationID);
		parent->FirstChildElement("MixingStationConfig")->InsertFirstChild(ProjectID);
		parent->FirstChildElement("MixingStationConfig")->InsertFirstChild(SectionID);
		parent->FirstChildElement("MixingStationConfig")->InsertEndChild(MixType);
		parent->FirstChildElement("CollectConfig")->InsertFirstChild(ThreadCount);
		parent->FirstChildElement("CollectConfig")->InsertEndChild(Frequency);

		string path = ConfigFile_Folder "/" SettingConfig_XML_Name;
        pDoc->SaveFile(path.c_str());
	}
	catch (exception* e)
	{
		result = pDoc->ErrorID();
	}
	return result;
}

const char * CSettingConfig::GetSocketIP() const
{
	return this->socketIP;
}

int CSettingConfig::GetSoecketPort() const
{
	return this->soecketPort;
}

const char * CSettingConfig::GetCacheAdress() const
{
	return this->cacheAdress;
}

int CSettingConfig::GetCacheUsedSpace() const
{
	return this->cacheUsedSpace;
}

int CSettingConfig::GetCacheSaveTime() const
{
	return this->cacheSaveTime;
}

//const char * CSettingConfig::GetTargetSoftwarePath() const
//{
//	return this->targetSoftwarePath;
//}
//
//const char * CSettingConfig::GetTargetSoftwareName() const
//{
//	return this->targetSoftwareName;
//}
//
//const char * CSettingConfig::GetTargetExeName() const
//{
//	return this->targetExeName;
//}

const char * CSettingConfig::GetMixingStationID() const
{
	return this->mixingStationID;
}

const char * CSettingConfig::GetMixingProjectID() const
{
	return this->mixingProjectID;
}

const char * CSettingConfig::GetMixingSectionID() const
{
	return this->mixingSectionID;
}

int CSettingConfig::GetMixType() const
{
	return this->mixType;
}

int CSettingConfig::GetCollThreadCount() const
{
	return this->collThreadCount;
}

int CSettingConfig::GetCollFreq() const
{
	return this->collFreq;
}

CONFIGError CSettingConfig::SetSocketIP(const char * socketIP, int strLen)
{
	if (this->socketIP != NULL)
		delete[] this->socketIP;
	this->socketIP = new char[strLen + 1];
	strncpy(this->socketIP, socketIP, strLen);
	this->socketIP[strLen] = '\0';
	
	return CONFIG_SUCCESS;
}

CONFIGError CSettingConfig::SetSoecketPort(int soecketPort)
{
	this->soecketPort = soecketPort;
	return CONFIG_SUCCESS;
}

CONFIGError CSettingConfig::SetCacheAdress(const char * cacheAdress, int strLen)
{
	if (this->cacheAdress != NULL)
		delete[] this->cacheAdress;
	this->cacheAdress = new char[strLen + 1];
	strncpy(this->cacheAdress, cacheAdress, strLen);
	this->cacheAdress[strLen] = '\0';

	return CONFIG_SUCCESS;
}

CONFIGError CSettingConfig::SetCacheUsedSpace(int cacheUsedSpace)
{
	this->cacheUsedSpace = cacheUsedSpace;
	return CONFIG_SUCCESS;
}

CONFIGError CSettingConfig::SetCacheSaveTime(int cacheSaveTime)
{
	this->cacheSaveTime = cacheSaveTime;
	return CONFIG_SUCCESS;
}

//CONFIGError CSettingConfig::SetTargetSoftwarePath(const char * targetSoftwarePath, int strLen)
//{
//	if (this->targetSoftwarePath != NULL)
//		delete[] this->targetSoftwarePath;
//	this->targetSoftwarePath = new char[strLen + 1];
//	strncpy(this->targetSoftwarePath, targetSoftwarePath, strLen);
//	this->targetSoftwarePath[strLen] = '\0';
//
//	return CONFIG_SUCCESS;
//}
//
//CONFIGError CSettingConfig::SetTargetSoftwareName(const char * targetSoftwareName, int strLen)
//{
//	if (this->targetSoftwareName != NULL)
//		delete[] this->targetSoftwareName;
//	this->targetSoftwareName = new char[strLen + 1];
//	strncpy(this->targetSoftwareName, targetSoftwareName, strLen);
//	this->targetSoftwareName[strLen] = '\0';
//
//	return CONFIG_SUCCESS;
//}
//
//CONFIGError CSettingConfig::SetTargetExeName(const char * targetExeName, int strLen)
//{
//	if (this->targetExeName != NULL)
//		delete[] this->targetExeName;
//	this->targetExeName = new char[strLen + 1];
//	strncpy(this->targetExeName, targetExeName, strLen);
//	this->targetExeName[strLen] = '\0';
//
//	return CONFIG_SUCCESS;
//}

CONFIGError CSettingConfig::SetMixingStationID(const char * mixingStationID, int strLen)
{
	if (this->mixingStationID != NULL)
		delete[] this->mixingStationID;
	this->mixingStationID = new char[strLen + 1];
	strncpy(this->mixingStationID, mixingStationID, strLen);
	this->mixingStationID[strLen] = '\0';

	return CONFIG_SUCCESS;
}

CONFIGError CSettingConfig::SetMixingProjectID(const char * mixingProjectID, int strLen)
{
	if (this->mixingProjectID != NULL)
		delete[] this->mixingProjectID;
	this->mixingProjectID = new char[strLen + 1];
	strncpy(this->mixingProjectID, mixingProjectID, strLen);
	this->mixingProjectID[strLen] = '\0';

	return CONFIG_SUCCESS;
}

CONFIGError CSettingConfig::SetMixingSectionID(const char * mixingSectionID, int strLen)
{
	if (this->mixingSectionID != NULL)
		delete[] this->mixingSectionID;
	this->mixingSectionID = new char[strLen + 1];
	strncpy(this->mixingSectionID, mixingSectionID, strLen);
	this->mixingSectionID[strLen] = '\0';

	return CONFIG_SUCCESS;
}

CONFIGError CSettingConfig::SetMixType(int mixType)
{
	this->mixType = mixType;
	return CONFIG_SUCCESS;
}

CONFIGError CSettingConfig::SetCollThreadCount(int threadCount)
{
	this->collThreadCount = threadCount;
	return CONFIG_SUCCESS;
}

CONFIGError CSettingConfig::SetCollFreq(int freq)
{
	this->collFreq = freq;
	return CONFIG_SUCCESS;
}

void CSettingConfig::ConstCharCopy(const char **targetChar, const char *sourceChar)
{
	const size_t len = strlen(sourceChar);
	char * tmp_char = new char[len + 1];
	strncpy(tmp_char, sourceChar, len);
	tmp_char[len] = '\0'; // I'm paranoid, maybe someone has changed something in _filename :-)
	*targetChar = tmp_char;
}
