#include "CCalibrationConfig.h"

ChildArea::ChildArea(int id)
{
	areaName = NULL;
	areaID = id;
	areaType = 0;
	areaValueType = 0;
	areaX = 0;
	areaY = 0;
	areaHeight = 0;
	areaWidth = 0;
}

ChildArea::~ChildArea()
{
	this->FreeSelf();
}

Parameter::Parameter()
{
	parameterName = NULL;
	parameterValue = NULL;
}

Parameter::~Parameter()
{
	this->FreeSelf();
}

MonitorArea::MonitorArea(int id)
{
	areaName = NULL;
	areaID = id;
	areaType = 0;
	areaValueType = 0;
	isMonitor = 0;
	checkPointX = 0;
	checkPointY = 0;
	checkPointType = 0;
	areaX = 0;
	areaY = 0;
	areaHeight = 0;
	areaWidth = 0;
	parameterCount = 0;
	childAreaCount = 0;

	measure_function = { 0 };
}

MonitorArea::~MonitorArea()
{
	this->FreeSelf();
}

CCalibrationConfig::CCalibrationConfig()
{
	softwarePath = NULL;
	softwareName = NULL;
	softwareType = 0;
	softwareVer = NULL;
	//exeName = NULL;
	windowName = NULL;
	windowClass = NULL;
	windowHeight = 0;
	windowWidth = 0;

	monitorAreaCount = 0;

	isAdmixtureToWater = 0;
	backgroundCaptureType = 1;		//Default use Print 
}

CCalibrationConfig::~CCalibrationConfig()
{
	this->FreeSelf();
}

void ChildArea::FreeSelf()
{
	if (areaName)
	{
		delete[] areaName;
		areaName = nullptr;
	}
}

void Parameter::FreeSelf()
{
	if (parameterName)
	{
		delete[] parameterName;
		parameterName = nullptr;
	}
	
	if (parameterValue)
	{
		delete[] parameterValue;
		parameterValue = nullptr;
	}
	
}

void MonitorArea::FreeSelf()
{
	if (areaName)
	{
		delete[] areaName;
		areaName = nullptr;
	}

	if (parameters.size() != 0)
	{
		for (int i = 0; i < parameters.size(); i++)
		{
			//parameters[i]->FreeSelf();
			if (parameters[i])
			{
				delete parameters[i];
				parameters[i] = nullptr;
			}
		}
	}
	parameters.clear();
	vector<Parameter *>(parameters).swap(parameters);
	if (childAreas.size() != 0)
	{
		for (int i = 0; i < childAreas.size(); i++)
		{
			//childAreas[i]->FreeSelf();
			if (childAreas[i])
			{
				delete childAreas[i];
				childAreas[i] = nullptr;
			}
		}
	}
	childAreas.clear();
	vector<ChildArea *>(childAreas).swap(childAreas);
}

void CCalibrationConfig::FreeSelf()
{
	if (softwarePath)
	{
		delete[] softwarePath;
		softwarePath = nullptr;
	}
	if (softwareName)
	{
		delete[] softwareName;
		softwareName = nullptr;
	}
	
	if (softwareVer)
	{
		delete[] softwareVer;
		softwareVer = nullptr;
	}
	
	//delete[] exeName;
	if (windowName)
	{
		delete[] windowName;
		windowName = nullptr;
	}
	
	if (windowClass)
	{
		delete[] windowClass;
		windowClass = nullptr;
	}
	

	if (monitorAreas.size() != 0)
	{
		for (int i = 0; i < monitorAreas.size(); i++)
		{
			//monitorAreas[i]->FreeSelf();
			if (monitorAreas[i])
			{
				delete monitorAreas[i];
				monitorAreas[i] = nullptr;
			}
			
		}
	}
	monitorAreas.clear();
	vector<MonitorArea *>(monitorAreas).swap(monitorAreas);
}

const MonitorArea * CCalibrationConfig::AddMonitorArea(const MonitorArea * newMonitor)
{
	try
	{
		int monitorID = newMonitor->GetMonitorAreaID();

		MonitorArea * add = new MonitorArea(monitorID);
		int strTempLen = strlen(newMonitor->GetMonitorAreaName());
		add->SetMonitorAreaName(newMonitor->GetMonitorAreaName(), strTempLen);
		add->SetMonitorAreaType(newMonitor->GetMonitorAreaType());
		add->SetMonitorAreaValueType(newMonitor->GetMonitorAreaValueType());
		add->SetMonitorIsMonitor(newMonitor->GetMonitorIsMonitor());
		add->SetMonitorCheckPointX(newMonitor->GetMonitorCheckPointX());
		add->SetMonitorCheckPointY(newMonitor->GetMonitorCheckPointY());
		add->SetMonitorCheckPointType(newMonitor->GetMonitorCheckPointType());
		add->SetMonitorAreaX(newMonitor->GetMonitorAreaX());
		add->SetMonitorAreaY(newMonitor->GetMonitorAreaY());
		add->SetMonitorAreaHeight(newMonitor->GetMonitorAreaHeight());
		add->SetMonitorAreaWidth(newMonitor->GetMonitorAreaWidth());
		add->SetMeasureFunction(newMonitor->GetMeasureFunction());
		int paraNum = newMonitor->GetMonitorParaCount();
		int childNum = newMonitor->GetMonitorChildCount();

		if (paraNum != 0)
		{
			/*vector<Parameter *>::iterator it;
			for (it = newMonitor->GetParameters().begin();it != newMonitor->GetParameters().end();)
			{
				add->AddParameter((*it)->GetParameterName(), (*it)->GetParameterValue());
			}*/
			for (int i = 0; i < paraNum; i++)
			{
				const vector<Parameter *>* temp = newMonitor->GetParameters();
				add->AddParameter((*temp)[i]->GetParameterName(), (*temp)[i]->GetParameterValue());
			}
		}
		if (childNum != 0)
		{
			for (int i = 0; i < childNum; i++)
			{
				const vector<ChildArea *>* temp = newMonitor->GetChildAreas();
				add->AddChildArea((*temp)[i]);
			}
		}

		monitorAreas.push_back(add);
		SetMonitorAreaCount(this->monitorAreaCount + 1);
		return add;
	}
	catch (exception* e)
	{
		throw e;
		return NULL;
	}
}

const Parameter * MonitorArea::AddParameter(const char * parameterName, const char * parameterValue)
{
	try
	{
		Parameter * result = new Parameter();
		int strTempLen = strlen(parameterName);
		result->SetParameterName(parameterName, strTempLen);
		strTempLen = strlen(parameterValue);
		result->SetParameterValue(parameterValue, strTempLen);

		parameters.push_back(result);
		SetParameterCount(parameterCount + 1);
		
		return result;
	}
	catch (exception* e)
	{
		throw e;
		return NULL;
	}
}

const ChildArea * MonitorArea::AddChildArea(const ChildArea * newChild)
{
	try
	{
		int childID = newChild->GetAreaID();
		
		ChildArea * add = new ChildArea(childID);
		int strTempLen = strlen(newChild->GetAreaName());
		add->SetAreaName(newChild->GetAreaName(), strTempLen);
		add->SetAreaType(newChild->GetAreaType());
		add->SetAreaValueType(newChild->GetAreaValueType());
		add->SetAreaX(newChild->GetAreaX());
		add->SetAreaY(newChild->GetAreaY());
		add->SetAreaHeight(newChild->GetAreaHeight());
		add->SetAreaWidth(newChild->GetAreaWidth());

		childAreas.push_back(add);
		SetChildCount(childAreaCount + 1);

		return add;
	}
	catch (exception* e)
	{
		throw e;
		return NULL;
	}
}

CONFIGError CCalibrationConfig::DelMonitorArea(int monitorID)
{
	if (this->monitorAreaCount == 0 || this->monitorAreas.size() == 0)
	{
		return CONFIG_ERROR_ELEMENT_MISMATCH;
	}
	vector<MonitorArea *>::iterator it;
	for (it = this->monitorAreas.begin();it != this->monitorAreas.end();)
	{
		if ((*it)->GetMonitorAreaID() == monitorID)
		{
			//(*it)->FreeSelf();
			delete (*it);
			this->monitorAreas.erase(it);

			SetMonitorAreaCount(this->monitorAreaCount - 1);

			return CONFIG_SUCCESS;
		}
		else
		{
			++it;
		}
	}

	return CONFIG_ERROR_ELEMENT_MISMATCH;
}

CONFIGError MonitorArea::DelParameter(const char * parameterName)
{
	if (this->parameterCount == 0 || this->parameters.size() == 0)
	{
		return CONFIG_ERROR_ELEMENT_MISMATCH;
	}
	vector<Parameter *>::iterator it;
	for (it = this->parameters.begin();it != this->parameters.end();)
	{
		if ((*it)->GetParameterName() == parameterName)
		{
			//(*it)->FreeSelf();
			delete (*it);
			this->parameters.erase(it);

			SetParameterCount(this->parameterCount - 1);

			return CONFIG_SUCCESS;
		}
		else
		{
			++it;
		}
	}

	//for (int i = 0; i < this->parameterCount; i++)
	//{
	//	if (this->parameters[i]->GetParameterName() == parameterName)
	//	{
	//		this->parameters[i]->FreeSelf();
	//		delete this->parameters[i];
	//		this->parameters.erase(this->parameters[i]);
	//		if (i == 0 && this->parameterCount == 1)
	//		{
	//			//this->parameters = NULL;
	//		}
	//		else if (i == 0)
	//		{
	//			//*this->parameters = this->parameters[i + 1];
	//		}
	//		else if (i == this->parameterCount - 1)
	//		{
	//			this->parameters[i - 1]->SetNext(NULL);
	//		}
	//		else
	//		{
	//			this->parameters[i - 1]->SetNext(this->parameters[i + 1]);
	//		}

	//		SetParameterCount(this->parameterCount - 1);
	//		
	//		return CONFIG_SUCCESS;
	//	}
	//}

	return CONFIG_ERROR_ELEMENT_MISMATCH;
}

CONFIGError MonitorArea::DelChildArea(int childID)
{
	if (this->childAreaCount == 0 || this->childAreas.size() == 0)
	{
		return CONFIG_ERROR_NULL_PTR;
	}
	vector<ChildArea *>::iterator it;
	for (it = this->childAreas.begin();it != this->childAreas.end();)
	{
		if ((*it)->GetAreaID() == childID)
		{
			//(*it)->FreeSelf();
			delete (*it);
			this->childAreas.erase(it);

			SetParameterCount(this->childAreaCount - 1);

			return CONFIG_SUCCESS;
		}
		else
		{
			++it;
		}
	}

	/*for (int i = 0; i < this->childAreaCount; i++)
	{
		if (this->childAreas[i]->GetAreaID() == childID)
		{
			this->childAreas[i]->FreeSelf();
			delete this->childAreas[i];
			if (i == 0 && this->childAreaCount == 1)
			{
				this->childAreas = NULL;
			}
			else if (i == 0)
			{
				*this->childAreas = this->childAreas[i + 1];
			}
			else if (i == this->childAreaCount - 1)
			{
				this->childAreas[i - 1]->SetNext(NULL);
			}
			else
			{
				this->childAreas[i - 1]->SetNext(this->childAreas[i + 1]);
			}
			
			int newCount = SetChildCount(this->childAreaCount - 1);
			if (newCount > 0)
			{
				for (int j = 0; j < newCount; j ++)
				{
					this->childAreas[j]->ModifyAreaID(j + 1);
				}
			}

			return CONFIG_SUCCESS;
		}
	}*/

	return CONFIG_ERROR_ELEMENT_MISMATCH;;
}

AreaMeasureFunction MonitorArea::GetMeasureFunction() const
{
	return measure_function;
}

CONFIGError MonitorArea::SetMeasureFunction(const AreaMeasureFunction & func)
{
	measure_function = func;
	return CONFIG_SUCCESS;
}

int CCalibrationConfig::ReloadConfig()
{
	int result = 0;
	
	try
	{
		string path = ConfigFile_Folder "/" CalibrationConfig_XML_Name;

		result = LoadConfig(path);
	}
	catch (exception * e)
	{
		;
	}

	return result;
}

int CCalibrationConfig::ReloadConfig(const string& path)
{
	int result = 0;
	
	try
	{
		result = LoadConfig(path);
	}
	catch (exception * e)
	{
		;
	}

	return result;
}

int CCalibrationConfig::LoadConfig(const string& path)
{
	int result = 0;
	tinyxml2::XMLDocument docSet;

	try
	{
		docSet.LoadFile(path.c_str());
		if (docSet.ErrorID() == tinyxml2::XML_ERROR_FILE_NOT_FOUND)
		{
			if (CreatCalibrationXML(&docSet))
			{
				return tinyxml2::XML_ERROR_FILE_NOT_FOUND;
			}
			docSet.LoadFile(path.c_str());
		}

		tinyxml2::XMLElement* pCaliConfig = docSet.FirstChildElement("App");
		if (!docSet.ErrorID() && pCaliConfig)
		{
			const char * tempStr;
			int strTempLen;
			tempStr = pCaliConfig->Attribute("name");
			strTempLen = tempStr == NULL ? 0 : strlen(tempStr);
			this->SetSoftwareName(tempStr, strTempLen);
			this->SetSoftwareType(pCaliConfig->IntAttribute("type"));
			tempStr = pCaliConfig->Attribute("version");
			strTempLen = tempStr == NULL ? 0 : strlen(tempStr);
			this->SetSoftwareVer(tempStr, strTempLen);
			//tempStr = pCaliConfig->FirstChildElement("exe")->FirstChildElement("name")->GetText();
			//strTempLen = strlen(tempStr);
			//this->SetExeName(tempStr, strTempLen);

			if (pCaliConfig->FirstChildElement("exe")->FirstChildElement("path")/*->FirstChild() */!= NULL)
			{
				tempStr = pCaliConfig->FirstChildElement("exe")->FirstChildElement("path")->GetText();
				strTempLen = tempStr == NULL ? 0 : strlen(tempStr);
				this->SetSoftwarePath(tempStr, strTempLen);
			}
			if (pCaliConfig->FirstChildElement("window")->FirstChildElement("name")/*->FirstChild() */!= NULL)
			{
				tempStr = pCaliConfig->FirstChildElement("window")->FirstChildElement("name")->GetText();
				strTempLen = tempStr == NULL ? 0 : strlen(tempStr);
				this->SetWindowName(tempStr, strTempLen);
			}
			if (pCaliConfig->FirstChildElement("window")->FirstChildElement("classname")/*->FirstChild() */!= NULL)
			{
				tempStr = pCaliConfig->FirstChildElement("window")->FirstChildElement("classname")->GetText();
				strTempLen = tempStr == NULL ? 0 : strlen(tempStr);
				this->SetWindowClass(tempStr, strTempLen);
			}
			int iTemp = 0;
			pCaliConfig->FirstChildElement("window")->FirstChildElement("resolution")->FirstChildElement("height")->QueryIntText(&iTemp);
			this->SetWindowHeight(iTemp);
			iTemp = 0;
			pCaliConfig->FirstChildElement("window")->FirstChildElement("resolution")->FirstChildElement("width")->QueryIntText(&iTemp);
			this->SetWindowWidth(iTemp);
			tinyxml2::XMLElement* pCali = pCaliConfig->FirstChildElement("caliConfig");
			if (pCali)
			{
				if (pCali->FirstChildElement("admixtureTo"))
				{
					iTemp = 0;
					pCali->FirstChildElement("admixtureTo")->QueryIntText(&iTemp);
					this->SetIsAdmixtureToWater(iTemp);
				}
				if (pCali->FirstChildElement("CaptureType"))
				{
					iTemp = 0;
					pCali->FirstChildElement("CaptureType")->QueryIntText(&iTemp);
					this->SetBackgroundCaptureType(iTemp);
				}
			}

			if (!pCaliConfig->FirstChildElement("monitored_areas")->NoChildren())
			{
				if (this->monitorAreas.size() != 0)
				{
					for (int i = 0; i < this->monitorAreas.size(); i++)
					{
						delete this->monitorAreas[i];
					}
				}
				this->monitorAreas.clear();
				vector<MonitorArea *>(this->monitorAreas).swap(this->monitorAreas);
				this->SetMonitorAreaCount(0);

				tinyxml2::XMLElement* pMonitor = pCaliConfig->FirstChildElement("monitored_areas")->FirstChildElement("area");
				while (pMonitor != NULL)
				{
					MonitorArea * newMonitor = new MonitorArea(pMonitor->IntAttribute("id"));
					tempStr = pMonitor->Attribute("name");
					strTempLen = tempStr == NULL ? 0 : strlen(tempStr);
					newMonitor->SetMonitorAreaName(tempStr, strTempLen);
					newMonitor->SetMonitorAreaType(pMonitor->IntAttribute("type"));
					newMonitor->SetMonitorAreaValueType(pMonitor->IntAttribute("valueType"));
					newMonitor->SetMonitorIsMonitor(pMonitor->IntAttribute("monitored"));
					newMonitor->SetMonitorCheckPointX(pMonitor->FirstChildElement("check_point")->IntAttribute("x"));
					newMonitor->SetMonitorCheckPointY(pMonitor->FirstChildElement("check_point")->IntAttribute("y"));
					newMonitor->SetMonitorCheckPointType(pMonitor->FirstChildElement("check_point")->IntAttribute("type"));
					newMonitor->SetMonitorAreaX(pMonitor->FirstChildElement("area_pos")->FirstChildElement("base_point")->IntAttribute("x"));
					newMonitor->SetMonitorAreaY(pMonitor->FirstChildElement("area_pos")->FirstChildElement("base_point")->IntAttribute("y"));
					iTemp = 0;
					pMonitor->FirstChildElement("area_pos")->FirstChildElement("height")->QueryIntText(&iTemp);
					newMonitor->SetMonitorAreaHeight(iTemp);
					iTemp = 0;
					pMonitor->FirstChildElement("area_pos")->FirstChildElement("width")->QueryIntText(&iTemp);
					newMonitor->SetMonitorAreaWidth(iTemp);

					tinyxml2::XMLElement* measureFunction = pMonitor->FirstChildElement("measure_function");
					if (measureFunction)
					{
						AreaMeasureFunction func = { 0 };
						func.method_type = measureFunction->IntAttribute("measure_method");
						func.v1_start_at = measureFunction->IntAttribute("value1_start_at");
						func.v1_start_delay = measureFunction->IntAttribute("value1_start_delay");
						func.v1_end_at = measureFunction->IntAttribute("value1_end_at");
						func.v1_end_delay = measureFunction->IntAttribute("value1_end_delay");
						func.v1_calc_method = measureFunction->IntAttribute("value1_calc_method");
						func.v2_start_at = measureFunction->IntAttribute("value2_start_at");
						func.v2_start_delay = measureFunction->IntAttribute("value2_start_delay");
						func.v2_end_at = measureFunction->IntAttribute("value2_end_at");
						func.v2_end_delay = measureFunction->IntAttribute("value2_end_delay");
						func.v2_calc_method = measureFunction->IntAttribute("value2_calc_method");

						newMonitor->SetMeasureFunction(func);
					}

					if (pMonitor->FirstChildElement("parameters")->NoChildren())
					{
						newMonitor->SetParameterCount(0);
					}
					else
					{
						tinyxml2::XMLElement* pParameter = pMonitor->FirstChildElement("parameters")->FirstChildElement("parameter");
						while (pParameter != NULL)
						{
							newMonitor->AddParameter(pParameter->Attribute("name"), pParameter->Attribute("value"));
							pParameter = pParameter->NextSiblingElement("parameter");
						}
					}
					if (pMonitor->FirstChildElement("child_areas")->NoChildren())
					{
						newMonitor->SetChildCount(0);
					}
					else
					{
						tinyxml2::XMLElement* pChildArea = pMonitor->FirstChildElement("child_areas")->FirstChildElement("child_area");
						while (pChildArea != NULL)
						{
							ChildArea * newChild = new ChildArea(pChildArea->IntAttribute("id"));
							tempStr = pChildArea->Attribute("name");
							strTempLen = tempStr == NULL ? 0 : strlen(tempStr);
							newChild->SetAreaName(tempStr, strTempLen);
							newChild->SetAreaType(pChildArea->IntAttribute("type"));
							newChild->SetAreaValueType(pChildArea->IntAttribute("valueType"));
							newChild->SetAreaX(pChildArea->FirstChild()->FirstChildElement("base_point")->IntAttribute("x"));
							newChild->SetAreaY(pChildArea->FirstChild()->FirstChildElement("base_point")->IntAttribute("y"));
							iTemp = 0;
							pChildArea->FirstChild()->FirstChildElement("height")->QueryIntText(&iTemp);
							newChild->SetAreaHeight(iTemp);
							iTemp = 0;
							pChildArea->FirstChild()->FirstChildElement("width")->QueryIntText(&iTemp);
							newChild->SetAreaWidth(iTemp);

							newMonitor->AddChildArea(newChild);
							delete newChild;
							pChildArea = pChildArea->NextSiblingElement("child_area");
						}
					}

					this->AddMonitorArea(newMonitor);
					delete newMonitor;
					pMonitor = pMonitor->NextSiblingElement("area");
				}
			}
			else
			{
				this->SetMonitorAreaCount(0);
			}
		}
	}
	catch (exception * e)
	{
		result = docSet.ErrorID();
	}

	return result;
}

int CCalibrationConfig::SaveConfig()
{
	int result = 0;

	try
	{
		string path = ConfigFile_Folder "/" CalibrationConfig_XML_Name;

		result = Save(path);
	}
	catch (exception * e)
	{
		;
	}

	return result;
}

int CCalibrationConfig::SaveConfig(const string& path)
{
	int result = 0;

	try
	{
		result = Save(path);
	}
	catch (exception * e)
	{
		;
	}

	return result;
}

int CCalibrationConfig::Save(const string& path)
{
	int result = 0;
	tinyxml2::XMLDocument docSet;

	try
	{
		docSet.LoadFile(path.c_str());
		if (docSet.ErrorID() == tinyxml2::XML_ERROR_FILE_NOT_FOUND)
		{
			if (CreatCalibrationXML(&docSet))
			{
				//MessageBox(_T("找不到配置文件，并且创建配置文件失败"), _T("错误"), MB_OK);
				return tinyxml2::XML_ERROR_FILE_NOT_FOUND;
			}
		}

		tinyxml2::XMLElement * pCaliConfig = docSet.FirstChildElement("App");
		pCaliConfig->SetAttribute("name", this->softwareName);
		//pCaliConfig->SetAttribute("name", "拌和机控制程序");
		pCaliConfig->SetAttribute("type", this->softwareType);
		pCaliConfig->SetAttribute("version", this->softwareVer);
		//pCaliConfig->FirstChildElement("exe")->FirstChildElement("name")->SetText(this->exeName);
		pCaliConfig->FirstChildElement("exe")->FirstChildElement("path")->SetText(this->softwarePath);
		//pCaliConfig->FirstChildElement("exe")->FirstChildElement("path")->SetText("E:\SVN\技术中心项目库\02_拌和站控制系统界面数据采集项目\01_过程管理");
		pCaliConfig->FirstChildElement("window")->FirstChildElement("name")->SetText(this->windowName);
		//pCaliConfig->FirstChildElement("window")->FirstChildElement("name")->SetText("拌和机控制程序窗口");
		pCaliConfig->FirstChildElement("window")->FirstChildElement("classname")->SetText(this->windowClass);
		pCaliConfig->FirstChildElement("window")->FirstChildElement("resolution")->FirstChildElement("height")->SetText(this->windowHeight);
		pCaliConfig->FirstChildElement("window")->FirstChildElement("resolution")->FirstChildElement("width")->SetText(this->windowWidth);
		tinyxml2::XMLElement* pCali = pCaliConfig->FirstChildElement("caliConfig");
		if (pCali)
		{
			if (pCali->FirstChildElement("admixtureTo"))
			    pCali->FirstChildElement("admixtureTo")->SetText(this->isAdmixtureToWater);
			if (pCali->FirstChildElement("CaptureType"))
			{
				pCali->FirstChildElement("CaptureType")->SetText(this->backgroundCaptureType);
			}
			else
			{
				tinyxml2::XMLElement* captureType = docSet.NewElement("CaptureType");
				captureType->SetText(this->backgroundCaptureType);
				pCali->InsertEndChild(captureType);
			}
				
		}

		pCaliConfig->FirstChildElement("monitored_areas")->DeleteChildren();
		if (this->monitorAreas.size() != 0)
		{
			tinyxml2::XMLElement * pMonitors = pCaliConfig->FirstChildElement("monitored_areas");
			int lenM = this->monitorAreas.size();
			for (int i = 0; i < lenM; i++)
			{
				tinyxml2::XMLElement * monitor = docSet.NewElement("area");
				tinyxml2::XMLElement * checkPoint = docSet.NewElement("check_point");
				tinyxml2::XMLElement * areaPos = docSet.NewElement("area_pos");
				tinyxml2::XMLElement * basePoint = docSet.NewElement("base_point");
				tinyxml2::XMLElement * height = docSet.NewElement("height");
				tinyxml2::XMLElement * width = docSet.NewElement("width");
				tinyxml2::XMLElement * parameters = docSet.NewElement("parameters");
				tinyxml2::XMLElement * childAreas = docSet.NewElement("child_areas");
				tinyxml2::XMLElement * measureFunction = docSet.NewElement("measure_function");
				pMonitors->InsertEndChild(monitor);
				pMonitors->LastChildElement("area")->InsertFirstChild(checkPoint);
				pMonitors->LastChildElement("area")->InsertEndChild(measureFunction);
				pMonitors->LastChildElement("area")->InsertEndChild(areaPos);
				pMonitors->LastChildElement("area")->InsertEndChild(parameters);
				pMonitors->LastChildElement("area")->InsertEndChild(childAreas);
				pMonitors->LastChildElement("area")->FirstChildElement("area_pos")->InsertFirstChild(basePoint);
				pMonitors->LastChildElement("area")->FirstChildElement("area_pos")->InsertEndChild(height);
				pMonitors->LastChildElement("area")->FirstChildElement("area_pos")->InsertEndChild(width);
				pMonitors->LastChildElement("area")->SetAttribute("id", this->monitorAreas[i]->GetMonitorAreaID());
				pMonitors->LastChildElement("area")->SetAttribute("name", this->monitorAreas[i]->GetMonitorAreaName());
				//if (i == 0)
				//    pMonitors->LastChildElement("area")->SetAttribute("name", "骨料");
				//else
				//{
				//    pMonitors->LastChildElement("area")->SetAttribute("name", "外加剂");
				//}
				pMonitors->LastChildElement("area")->SetAttribute("type", this->monitorAreas[i]->GetMonitorAreaType());
				pMonitors->LastChildElement("area")->SetAttribute("valueType", this->monitorAreas[i]->GetMonitorAreaValueType());
				pMonitors->LastChildElement("area")->SetAttribute("monitored", this->monitorAreas[i]->GetMonitorIsMonitor());
				pMonitors->LastChildElement("area")->FirstChildElement("check_point")->SetAttribute("x", this->monitorAreas[i]->GetMonitorCheckPointX());
				pMonitors->LastChildElement("area")->FirstChildElement("check_point")->SetAttribute("y", this->monitorAreas[i]->GetMonitorCheckPointY());
				pMonitors->LastChildElement("area")->FirstChildElement("check_point")->SetAttribute("type", this->monitorAreas[i]->GetMonitorCheckPointType());
				pMonitors->LastChildElement("area")->FirstChildElement("area_pos")->FirstChildElement("base_point")->SetAttribute("x", this->monitorAreas[i]->GetMonitorAreaX());
				pMonitors->LastChildElement("area")->FirstChildElement("area_pos")->FirstChildElement("base_point")->SetAttribute("y", this->monitorAreas[i]->GetMonitorAreaY());
				pMonitors->LastChildElement("area")->FirstChildElement("area_pos")->FirstChildElement("height")->SetText(this->monitorAreas[i]->GetMonitorAreaHeight());
				pMonitors->LastChildElement("area")->FirstChildElement("area_pos")->FirstChildElement("width")->SetText(this->monitorAreas[i]->GetMonitorAreaWidth());

				measureFunction->SetAttribute("measure_method", this->monitorAreas[i]->GetMeasureFunction().method_type);
				measureFunction->SetAttribute("value1_start_at", this->monitorAreas[i]->GetMeasureFunction().v1_start_at);
				measureFunction->SetAttribute("value1_start_delay", this->monitorAreas[i]->GetMeasureFunction().v1_start_delay);
				measureFunction->SetAttribute("value1_end_at", this->monitorAreas[i]->GetMeasureFunction().v1_end_at);
				measureFunction->SetAttribute("value1_end_delay", this->monitorAreas[i]->GetMeasureFunction().v1_end_delay);
				measureFunction->SetAttribute("value1_calc_method", this->monitorAreas[i]->GetMeasureFunction().v1_calc_method);
				measureFunction->SetAttribute("value2_start_at", this->monitorAreas[i]->GetMeasureFunction().v2_start_at);
				measureFunction->SetAttribute("value2_start_delay", this->monitorAreas[i]->GetMeasureFunction().v2_start_delay);
				measureFunction->SetAttribute("value2_end_at", this->monitorAreas[i]->GetMeasureFunction().v2_end_at);
				measureFunction->SetAttribute("value2_end_delay", this->monitorAreas[i]->GetMeasureFunction().v2_end_delay);
				measureFunction->SetAttribute("value2_calc_method", this->monitorAreas[i]->GetMeasureFunction().v2_calc_method);

				tinyxml2::XMLElement * pParameters = pMonitors->LastChildElement("area")->FirstChildElement("parameters");
				const vector<Parameter *>*  parametersV = this->monitorAreas[i]->GetParameters();
				int lenP = parametersV->size();
				for (int j = 0; j < lenP; j++)
				{
					tinyxml2::XMLElement * parameter = docSet.NewElement("parameter");
					pParameters->InsertEndChild(parameter);
					pParameters->LastChildElement("parameter")->SetAttribute("name", (*parametersV)[j]->GetParameterName());
					pParameters->LastChildElement("parameter")->SetAttribute("value", (*parametersV)[j]->GetParameterValue());
				}

				tinyxml2::XMLElement * pChildAreas = pMonitors->LastChildElement("area")->FirstChildElement("child_areas");
				const vector<ChildArea *>*  childAreasV = this->monitorAreas[i]->GetChildAreas();
				int lenC = childAreasV->size();
				for (int k = 0; k < lenC; k++)
				{
					tinyxml2::XMLElement * childArea = docSet.NewElement("child_area");
					tinyxml2::XMLElement * areaPos = docSet.NewElement("area_pos");
					tinyxml2::XMLElement * basePointC = docSet.NewElement("base_point");
					tinyxml2::XMLElement * heightC = docSet.NewElement("height");
					tinyxml2::XMLElement * widthC = docSet.NewElement("width");
					pChildAreas->InsertEndChild(childArea);
					pChildAreas->LastChildElement("child_area")->InsertFirstChild(areaPos);
					pChildAreas->LastChildElement("child_area")->FirstChildElement("area_pos")->InsertEndChild(basePointC);
					pChildAreas->LastChildElement("child_area")->FirstChildElement("area_pos")->InsertEndChild(heightC);
					pChildAreas->LastChildElement("child_area")->FirstChildElement("area_pos")->InsertEndChild(widthC);
					pChildAreas->LastChildElement("child_area")->SetAttribute("id", (*childAreasV)[k]->GetAreaID());
					pChildAreas->LastChildElement("child_area")->SetAttribute("name", (*childAreasV)[k]->GetAreaName());
					//if (k == 0)
					//    pChildAreas->LastChildElement("child_area")->SetAttribute("name", "设定值");
					//else
					//    pChildAreas->LastChildElement("child_area")->SetAttribute("name", "上料值");
					pChildAreas->LastChildElement("child_area")->SetAttribute("type", (*childAreasV)[k]->GetAreaType());
					pChildAreas->LastChildElement("child_area")->SetAttribute("valueType", (*childAreasV)[k]->GetAreaValueType());
					pChildAreas->LastChildElement("child_area")->FirstChildElement("area_pos")->FirstChildElement("base_point")->SetAttribute("x", (*childAreasV)[k]->GetAreaX());
					pChildAreas->LastChildElement("child_area")->FirstChildElement("area_pos")->FirstChildElement("base_point")->SetAttribute("y", (*childAreasV)[k]->GetAreaY());
					pChildAreas->LastChildElement("child_area")->FirstChildElement("area_pos")->FirstChildElement("height")->SetText((*childAreasV)[k]->GetAreaHeight());
					pChildAreas->LastChildElement("child_area")->FirstChildElement("area_pos")->FirstChildElement("width")->SetText((*childAreasV)[k]->GetAreaWidth());
				}
			}
		}

		docSet.SaveFile(path.c_str());
	}
	catch (exception* e)
	{
		result = docSet.ErrorID();
	}

	return result;
}

int CCalibrationConfig::GetBackgroundCaptureType() const
{
	return backgroundCaptureType;
}

CONFIGError CCalibrationConfig::SetBackgroundCaptureType(int type)
{
	backgroundCaptureType = type;
	return CONFIG_SUCCESS;
}

const char * CCalibrationConfig::GetSoftwarePath() const
{
	return this->softwarePath;
}

const char * CCalibrationConfig::GetSoftwareName() const
{
	return this->softwareName;
}

int CCalibrationConfig::GetSoftwareType() const
{
	return this->softwareType;
}
const char * CCalibrationConfig::GetSoftwareVer() const
{
	return this->softwareVer;
}

//const char * CCalibrationConfig::GetExeName() const
//{
//	return this->exeName;
//}

const char * CCalibrationConfig::GetWindowName() const
{
	return this->windowName;
}

const char * CCalibrationConfig::GetWindowClass() const
{
	return this->windowClass;
}

int CCalibrationConfig::GetWindowHeight() const
{
	return this->windowHeight;
}

int CCalibrationConfig::GetWindowWidth() const
{
	return this->windowWidth;
}

int CCalibrationConfig::GetMonitorAreaCount() const
{
	return this->monitorAreaCount;
}

MonitorArea * CCalibrationConfig::GetMonitorArea(int id) const
{
	MonitorArea * monitor = NULL;
	for (int i = 0; i < this->monitorAreaCount; i++)
	{
		if (this->monitorAreas[i]->GetMonitorAreaID() == id)
			monitor = this->monitorAreas[i];
	}
	return monitor;
}

const char * MonitorArea::GetMonitorAreaName() const
{
	return this->areaName;
}

int MonitorArea::GetMonitorAreaID() const
{
	return this->areaID;
}

int MonitorArea::GetMonitorAreaType() const
{
	return this->areaType;
}

int MonitorArea::GetMonitorAreaValueType() const
{
	return this->areaValueType;
}

int MonitorArea::GetMonitorIsMonitor() const
{
	return this->isMonitor;
}

int MonitorArea::GetMonitorCheckPointX() const
{
	return this->checkPointX;
}

int MonitorArea::GetMonitorCheckPointY() const
{
	return this->checkPointY;
}

int MonitorArea::GetMonitorCheckPointType() const
{
	return this->checkPointType;
}

int MonitorArea::GetMonitorAreaX() const
{
	return this->areaX;
}

int MonitorArea::GetMonitorAreaY() const
{
	return this->areaY;
}

int MonitorArea::GetMonitorAreaHeight() const
{
	return this->areaHeight;
}

int MonitorArea::GetMonitorAreaWidth() const
{
	return this->areaWidth;
}

int MonitorArea::GetMonitorParaCount() const
{
	return this->parameterCount;
}

int MonitorArea::GetMonitorChildCount() const
{
	return this->childAreaCount;
}

const char * ChildArea::GetAreaName() const
{
	return this->areaName;
}

int ChildArea::GetAreaID() const
{
	return this->areaID;
}

int ChildArea::GetAreaType() const
{
	return this->areaType;
}

int ChildArea::GetAreaValueType() const
{
	return this->areaValueType;
}

int ChildArea::GetAreaX() const
{
	return this->areaX;
}

int ChildArea::GetAreaY() const
{
	return this->areaY;
}

int ChildArea::GetAreaHeight() const
{
	return this->areaHeight;
}

int ChildArea::GetAreaWidth() const
{
	return this->areaWidth;
}

const char * Parameter::GetParameterName() const
{
	return this->parameterName;
}

const char * Parameter::GetParameterValue() const
{
	return this->parameterValue;
}

int CCalibrationConfig::SetMonitorAreaCount(int newCount)
{
	this->monitorAreaCount = newCount;
	return newCount;
}

int MonitorArea::SetParameterCount(int newCount)
{
	this->parameterCount = newCount;
	return newCount;
}

int MonitorArea::SetChildCount(int newCount)
{
	this->childAreaCount = newCount;
	return newCount;
}

CONFIGError CCalibrationConfig::SetSoftwarePath(const char * path, int strLen)
{
	if (this->softwarePath != NULL)
		delete[] this->softwarePath;
	this->softwarePath = new char[strLen + 1];
	strncpy(this->softwarePath, path, strLen);
	this->softwarePath[strLen] = '\0';

	return CONFIG_SUCCESS;
}

CONFIGError CCalibrationConfig::SetSoftwareName(const char * name, int strLen)
{
	if (this->softwareName != NULL)
		delete[] this->softwareName;
	this->softwareName = new char[strLen + 1];
	strncpy(this->softwareName, name, strLen);
	this->softwareName[strLen] = '\0';

	return CONFIG_SUCCESS;
}

CONFIGError CCalibrationConfig::SetSoftwareType(int type)
{
	this->softwareType = type;
	return CONFIG_SUCCESS;
}

CONFIGError CCalibrationConfig::SetSoftwareVer(const char * version, int strLen)
{
	if (this->softwareVer != NULL)
		delete[] this->softwareVer;
	this->softwareVer = new char[strLen + 1];
	strncpy(this->softwareVer, version, strLen);
	this->softwareVer[strLen] = '\0';

	return CONFIG_SUCCESS;
}

//CONFIGError CCalibrationConfig::SetExeName(const char * name, int strLen)
//{
//	if (this->exeName != NULL)
//		delete[] this->exeName;
//	this->exeName = new char[strLen + 1];
//	strncpy(this->exeName, name, strLen);
//	this->exeName[strLen] = '\0';
//
//	return CONFIG_SUCCESS;
//}

CONFIGError CCalibrationConfig::SetWindowName(const char * name, int strLen)
{
	if (this->windowName != NULL)
		delete[] this->windowName;
	this->windowName = new char[strLen + 1];
	strncpy(this->windowName, name, strLen);
	this->windowName[strLen] = '\0';

	return CONFIG_SUCCESS;
}

CONFIGError CCalibrationConfig::SetWindowClass(const char * className, int strLen)
{
	if (this->windowClass != NULL)
		delete[] this->windowClass;
	this->windowClass = new char[strLen + 1];
	strncpy(this->windowClass, className, strLen);
	this->windowClass[strLen] = '\0';

	return CONFIG_SUCCESS;
}

CONFIGError CCalibrationConfig::SetWindowHeight(int height)
{
	this->windowHeight = height;
	return CONFIG_SUCCESS;
}

CONFIGError CCalibrationConfig::SetWindowWidth(int width)
{
	this->windowWidth = width;
	return CONFIG_SUCCESS;
}

CONFIGError ChildArea::SetAreaName(const char * areaName, int strLen)
{
	if (this->areaName != NULL)
		delete[] this->areaName;
	this->areaName = new char[strLen + 1];
	strncpy(this->areaName, areaName, strLen);
	this->areaName[strLen] = '\0';

	return CONFIG_SUCCESS;
}

CONFIGError ChildArea::SetAreaType(int areaType)
{
	this->areaType = areaType;
	return CONFIG_SUCCESS;
}

CONFIGError ChildArea::SetAreaValueType(int areaValueType)
{
	this->areaValueType = areaValueType;
	return CONFIG_SUCCESS;
}

CONFIGError ChildArea::SetAreaX(int areaX)
{
	this->areaX = areaX;
	return CONFIG_SUCCESS;
}

CONFIGError ChildArea::SetAreaY(int areaY)
{
	this->areaY = areaY;
	return CONFIG_SUCCESS;
}

CONFIGError ChildArea::SetAreaHeight(int areaHeight)
{
	this->areaHeight = areaHeight;
	return CONFIG_SUCCESS;
}

CONFIGError ChildArea::SetAreaWidth(int areaWidth)
{
	this->areaWidth = areaWidth;
	return CONFIG_SUCCESS;
}

int ChildArea::ModifyAreaID(int newID)
{
	this->areaID = newID;
	return this->areaID;
}

CONFIGError Parameter::SetParameterName(const char * parameterName, int strLen)
{
	if (this->parameterName != NULL)
		delete[] this->parameterName;
	this->parameterName = new char[strLen + 1];
	strncpy(this->parameterName, parameterName, strLen);
	this->parameterName[strLen] = '\0';

	return CONFIG_SUCCESS;
}

CONFIGError Parameter::SetParameterValue(const char * parameterValue, int strLen)
{
	if (this->parameterValue != NULL)
		delete[] this->parameterValue;
	this->parameterValue = new char[strLen + 1];
	strncpy(this->parameterValue, parameterValue, strLen);
	this->parameterValue[strLen] = '\0';

	return CONFIG_SUCCESS;
}

CONFIGError MonitorArea::SetMonitorAreaName(const char * monitorName, int strLen)
{
	if (this->areaName != NULL)
		delete[] this->areaName;
	this->areaName = new char[strLen + 1];
	strncpy(this->areaName, monitorName, strLen);
	this->areaName[strLen] = '\0';

	return CONFIG_SUCCESS;
}

CONFIGError MonitorArea::SetMonitorAreaType(int areaType)
{
	this->areaType = areaType;
	return CONFIG_SUCCESS;
}

CONFIGError MonitorArea::SetMonitorAreaValueType(int areaValueType)
{
	this->areaValueType = areaValueType;
	return CONFIG_SUCCESS;
}

CONFIGError MonitorArea::SetMonitorIsMonitor(int isMonitor)
{
	this->isMonitor = isMonitor;
	return CONFIG_SUCCESS;
}

CONFIGError MonitorArea::SetMonitorCheckPointX(int checkPointX)
{
	this->checkPointX = checkPointX;
	return CONFIG_SUCCESS;
}

CONFIGError MonitorArea::SetMonitorCheckPointY(int checkPointY)
{
	this->checkPointY = checkPointY;
	return CONFIG_SUCCESS;
}

CONFIGError MonitorArea::SetMonitorCheckPointType(int checkPointType)
{
	this->checkPointType = checkPointType;
	return CONFIG_SUCCESS;
}

CONFIGError MonitorArea::SetMonitorAreaX(int areaX)
{
	this->areaX = areaX;
	return CONFIG_SUCCESS;
}

CONFIGError MonitorArea::SetMonitorAreaY(int areaY)
{
	this->areaY = areaY;
	return CONFIG_SUCCESS;
}

CONFIGError MonitorArea::SetMonitorAreaHeight(int height)
{
	this->areaHeight = height;
	return CONFIG_SUCCESS;
}

CONFIGError MonitorArea::SetMonitorAreaWidth(int width)
{
	this->areaWidth = width;
	return CONFIG_SUCCESS;
}

int MonitorArea::ModifyMonitorAreaID(int newID)
{
	this->areaID = newID;
	return this->areaID;
}

const vector<Parameter *>* MonitorArea::GetParameters() const
{
	return &(this->parameters);
}

const vector<ChildArea *>* MonitorArea::GetChildAreas() const
{
	return &(this->childAreas);
}

const vector<MonitorArea *>* CCalibrationConfig::GetMonitorAreas() const
{
	return &(this->monitorAreas);
}

int CCalibrationConfig::GetIsAdmixtureToWater() const
{
	return this->isAdmixtureToWater;
}

CONFIGError CCalibrationConfig::SetIsAdmixtureToWater(int isAdmixtureToWater)
{
	this->isAdmixtureToWater = isAdmixtureToWater;
	return CONFIG_SUCCESS;
}

int CCalibrationConfig::CreatCalibrationXML(tinyxml2::XMLDocument* pDoc)
{
	int result = 0;
	try
	{
		//pDoc->SetBOM(true);
		//tinyxml2::XMLDeclaration* pDeclear = pDoc->NewDeclaration();
		//pDoc->InsertFirstChild(pDeclear);

		tinyxml2::XMLElement* parent = pDoc->NewElement("App");
		pDoc->InsertEndChild(parent);

		tinyxml2::XMLElement* Exe = pDoc->NewElement("exe");
		tinyxml2::XMLElement* Path = pDoc->NewElement("path");

		tinyxml2::XMLElement* Window = pDoc->NewElement("window");
		tinyxml2::XMLElement* Name = pDoc->NewElement("name");
		tinyxml2::XMLElement* Classname = pDoc->NewElement("classname");
		tinyxml2::XMLElement* Resolution = pDoc->NewElement("resolution");
		tinyxml2::XMLElement* Height = pDoc->NewElement("height");
		tinyxml2::XMLElement* Width = pDoc->NewElement("width");

		tinyxml2::XMLElement* CaliConfig = pDoc->NewElement("caliConfig");
		tinyxml2::XMLElement* AdmixtureTo = pDoc->NewElement("admixtureTo");
		tinyxml2::XMLElement* CaptureType = pDoc->NewElement("CaptureType");

		tinyxml2::XMLElement* MonitoredAreas = pDoc->NewElement("monitored_areas");

		tinyxml2::XMLNode* childNode = parent->InsertFirstChild(Exe);
		parent->InsertEndChild(Window);
		parent->InsertEndChild(CaliConfig);
		parent->InsertEndChild(MonitoredAreas);

		parent->FirstChildElement("exe")->InsertFirstChild(Path);

		parent->FirstChildElement("window")->InsertFirstChild(Name);
		parent->FirstChildElement("window")->InsertEndChild(Classname);
		parent->FirstChildElement("window")->InsertEndChild(Resolution);
		parent->FirstChildElement("window")->FirstChildElement("resolution")->InsertFirstChild(Height);
		parent->FirstChildElement("window")->FirstChildElement("resolution")->InsertEndChild(Width);

		parent->FirstChildElement("caliConfig")->InsertFirstChild(AdmixtureTo);
		parent->FirstChildElement("caliConfig")->InsertEndChild(CaptureType);

		string path = ConfigFile_Folder "/" CalibrationConfig_XML_Name;
		pDoc->SaveFile(path.c_str());
	}
	catch (exception* e)
	{
		result = pDoc->ErrorID();
	}
	return result;
}
