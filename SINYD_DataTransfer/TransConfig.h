#pragma once



enum TransferFormatType
{
	TYPE_NORMAL = 0,
    TYPE_FLUME
};

class CTransConfig
{
public:
	CTransConfig();
	virtual ~CTransConfig();

	static int GetSendTimeout(int defalut);
	static int GetRecvTimeout(int defalut);
	static int GetRawRowCountPerBody(int defalut);
	static int GetMeasureRowCountPerBody(int defalut);
	static int GetDetailRowCountPerBody(int defalut);
	static TransferFormatType GetTransFormatType(TransferFormatType type);

private:
	static void CheckAndCreate();
};

