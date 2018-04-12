#include "Measure.h"
//#include <Windows.h>
#include "CopyFile.h"
#include "Tool/syd_time.h"
#include "MeasureDef.h"
#include "../SINYD_Measure/Data/RWData/RWData.h"

std::string createtime;

CMeasureLogic::CMeasureLogic():mp_dc(NULL)
{
	
}


CMeasureLogic::~CMeasureLogic()
{
	if (NULL != mp_dc) {
		delete mp_dc;
		mp_dc = NULL;
	}
}

MESURELOGIC_ERROR CMeasureLogic::Initialize()
{
	printf("==============initialize======\n");
	SydTime tm;
	createtime = tm.ToString();
	CRWData rw;
	std::string strSysConfig = CONFIG_BASE_PATH;
	strSysConfig += CONFIG_SYS_FILE;
	int mac_type = 1;
	if (0 == (mac_type = rw.GetProfileInt("mix_machine_1", "mix_machine_type", 0, strSysConfig.c_str()))) {
		return ML_ERROR_INIT_CREATE_STRUCT;
	}
	BackupConfig(tm.ToDirString(), mac_type);
	printf("[%s]DirPath:%s\n", createtime.c_str(),dirpath.c_str());

	mp_dc = new DataCollector();
	mp_dc->Initialize();

	return ML_ERROR_OK;
}

void CMeasureLogic::SetSrcData(const string &srcdata)
{
    if (mp_dc)
	{
		mp_dc->SetRawData(srcdata);
	}
}

void CMeasureLogic::EndFrame()
{
	if (mp_dc)
	{
		mp_dc->Start();
	}
}

bool CMeasureLogic::IsRunning()
{
	if (mp_dc)
	{
		return mp_dc->IsRunning();
	}
	return false;
}

