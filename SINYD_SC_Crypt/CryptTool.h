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
	/// @brief:  返回当前机器的机器码和系统当前时间的密文
	///*******************************************************
	string GetSecretMachineCodeWithTimeFromLocal();

	///*******************************************************
	/// @name:   CCryptTool::SignText
	/// @author: YaoDi
	/// @return: std::string
	/// @param:  [in][const string &]PrivateKey
	/// @param:  [in][const string &]MachineCodeWithTime
	/// @brief:  返回一个机器码密文和到期时间密文的组合字符串
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
	/// @brief:  验证携带时间的机器码是否和注册Key相符,返回验证状态
	///*******************************************************
	VERIFIER_STATUS VerifierText(const string& PublicKey, const string& serialKey, const string& MachineCodeWithTime);

	///*******************************************************
	/// @name:   CCryptTool::GetDurationTimeFromSerialKey
	/// @author: YaoDi
	/// @return: std::string
	/// @param:  [in][const string &]serialKey
	/// @brief:  返回注册码中记录的到期日期
	///*******************************************************
	string GetDurationTimeFromSerialKey(const string& serialKey);

private:
	void SplitMachineCode(const string& machineCodeWithTime, string& strMachineCode, string& strTime);
	void SplitSerialKey(const string& serialKey, string& serialKeyMachine, string& serialKeyTime);
};

