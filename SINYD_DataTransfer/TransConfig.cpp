#include "TransConfig.h"
#include "TRWData.h"
#include "MeasureDef.h"

#define TRANS_CONFIG_INI "Trans.conf"
#define TRANS_CONFIG_PATH CONFIG_BASE_PATH TRANS_CONFIG_INI

CTransConfig::CTransConfig()
{
}


CTransConfig::~CTransConfig()
{
}

int CTransConfig::GetSendTimeout(int defalut)
{
	CheckAndCreate();
	CTRWData rw;
	return rw._GetProfileInt("Trans", "SendTimeout", defalut, TRANS_CONFIG_PATH);
}

int CTransConfig::GetRecvTimeout(int defalut)
{
	CheckAndCreate();
	CTRWData rw;
	return rw._GetProfileInt("Trans", "RecvTimeout", defalut, TRANS_CONFIG_PATH);
}

int CTransConfig::GetRawRowCountPerBody(int defalut)
{
	CheckAndCreate();
	CTRWData rw;
	return rw._GetProfileInt("Trans", "RawRowCountPerBody", defalut, TRANS_CONFIG_PATH);
}

int CTransConfig::GetMeasureRowCountPerBody(int defalut)
{
	CheckAndCreate();
	CTRWData rw;
	return rw._GetProfileInt("Trans", "MeasureRowCountPerBody", defalut, TRANS_CONFIG_PATH);
}

int CTransConfig::GetDetailRowCountPerBody(int defalut)
{
	CheckAndCreate();
	CTRWData rw;
	return rw._GetProfileInt("Trans", "DetailRowCountPerBody", defalut, TRANS_CONFIG_PATH);
}

TransferFormatType CTransConfig::GetTransFormatType(TransferFormatType type)
{
	CheckAndCreate();
	CTRWData rw;
	return (TransferFormatType)rw._GetProfileInt("Trans", "TransferFormatType", (int)type, TRANS_CONFIG_PATH);
}

void CTransConfig::CheckAndCreate()
{
	CTRWData rw;
	if (!rw.IsFileExist(TRANS_CONFIG_PATH))
	{
		rw._CreateFile(TRANS_CONFIG_PATH);
		rw._WriteProfileString("Trans", "SendTimeout", "1000", TRANS_CONFIG_PATH);
		rw._WriteProfileString("Trans", "RecvTimeout", "1000", TRANS_CONFIG_PATH);
		rw._WriteProfileString("Trans", "RawRowCountPerBody", "10", TRANS_CONFIG_PATH);
		rw._WriteProfileString("Trans", "MeasureRowCountPerBody", "10", TRANS_CONFIG_PATH);
		rw._WriteProfileString("Trans", "DetailRowCountPerBody", "10", TRANS_CONFIG_PATH);
		rw._WriteProfileString("Trans", "TransferFormatType", "1", TRANS_CONFIG_PATH);
	}
}
