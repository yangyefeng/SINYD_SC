#pragma once
#include <string>
#include "../SINYD_SC_Mesurelogic/Data/RWData/RWData.h"
#include "../SINYD_SC_Mesurelogic/Measurementlogic/Interface/Measurementlogic.h"

using namespace std;
#define SC_MESURELOGIC_API __declspec(dllexport)
#ifdef SC_MESURELOGIC_DLL_EXPORT
#define SC_MESURELOGIC_API __declspec(dllexport)
#else 
//#define SC_MESURELOGIC_API __declspec(dllimport)
#endif

enum MESURELOGIC_ERROR
{
	ML_ERROR_OK,
	ML_ERROR_INIT_CONFIG,
	ML_ERROR_INIT_CREATE_STRUCT,
};

class SC_MESURELOGIC_API CMeasureLogic
{
public:
	CMeasureLogic();
	virtual ~CMeasureLogic();

	MESURELOGIC_ERROR Initialize();
	void SetSrcData(const string &srcdata) ;
	void EndFrame();
	void SetIPConfig(const string &ip, int port);
	void ClearProcessData();

private:
	CMeasurementlogic *mp_ml;
};

class SC_MESURELOGIC_API CConfigRWData
{
public:
	CConfigRWData();
	~CConfigRWData();

	void DeleteFile(char *filename);
	void ClearFileContent(char *filename);
	bool WriteProfileString(char lpAppName[], char lpKeyName[], char lpString[], char lpFileName[]);
	bool CreateFile(char *filename, char *wstr = NULL);

private:
	CRWData *mp_rwdata;
};


