#pragma once
#include <string>
using namespace std;

#define INFINITE_KEY 0xFFFFFFFF
#define MACHINE_CODE_LEN	176
#define SERIAL_KEY_STR_LEN	368
#define INFINITE_KEY_STR "INFINITE"

enum VERIFIER_STATUS
{
	UnRegisted,
	HasRegisted,
	OutTime
};

class CCryptTool
{
public:
	CCryptTool();
	virtual ~CCryptTool();

public:
	///*******************************************************
	/// @name:   CCryptTool::GetMachineCodeWithTimeFromLocal
	/// @author: YaoDi
	/// @return: std::string
	/// @brief:  ���ص�ǰ�����Ļ������ϵͳ��ǰʱ�������
	///*******************************************************
	string GetSecretMachineCodeWithTimeFromLocal();

	///*******************************************************
	/// @name:   CCryptTool::SignText
	/// @author: YaoDi
	/// @return: std::string
	/// @param:  [in][const string &]PrivateKey
	/// @param:  [in][const string &]MachineCodeWithTime
	/// @brief:  ����һ�����������ĺ͵���ʱ�����ĵ�����ַ���
	///*******************************************************
	string SignText(const string& PrivateKey, const string& MachineCodeWithTime, int KeyDuration);
	string SignText(const string& PrivateKey, const string& MachineCodeWithTime, const char* DueTime);

	///*******************************************************
	/// @name:   CCryptTool::VerifierText
	/// @author: YaoDi
	/// @return: bool
	/// @param:  [in][const string &]PublicKey
	/// @param:  [in][const string &]SerialKey
	/// @param:  [in][const string &]MachineCodeWithTime
	/// @brief:  ��֤Я��ʱ��Ļ������Ƿ��ע��Key���,������֤״̬
	///*******************************************************
	VERIFIER_STATUS VerifierText(const string& PublicKey, const string& serialKey, const string& MachineCodeWithTime);

	///*******************************************************
	/// @name:   CCryptTool::GetDurationTimeFromSerialKey
	/// @author: YaoDi
	/// @return: std::string
	/// @param:  [in][const string &]serialKey
	/// @brief:  ����ע�����м�¼�ĵ�������
	///*******************************************************
	string GetDurationTimeFromSerialKey(const string& serialKey);

private:
	void SplitMachineCode(const string& machineCodeWithTime, string& strMachineCode, string& strTime);
	void SplitSerialKey(const string& serialKey, string& serialKeyMachine, string& serialKeyTime);
};

