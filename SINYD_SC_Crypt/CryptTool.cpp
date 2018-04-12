#include "CryptTool.h"
#include "cryptohash.h"
#include "MACAddress.h"
#include "HardDriveSerialNumer.h"
#include "cpuid.h"
#include "CryptoppWrapper.h"
#include <time.h>

#define StringCryptPass "MzA4MjAxNTIwMjAxMDAzMDBEMDYwOTJB"//ODY0ODg2RjcwRDAxMDEwMTA1MDAwNDgyMDEzQzMwODIwMTM4MDIwMTAwMDI0MTAwQ0RCM0FERDE1NTE2NDBGQTEzREZFOUE0N0EyMDYzMDcyMzFFODVDMEQ0RDgwOEVCMjlCQjBBRjQ1RTcwNjQ4N0M5MjA0RTI1OThCOUFBQUFEODdCODYxN0M2NTU0RTBDOTk1ODcwRkZEOTg3RUI0MTFEN"
#define __INFINITE_KEY_STR "__INFINITE_KEY"

CCryptTool::CCryptTool()
{
}


CCryptTool::~CCryptTool()
{
}

string CCryptTool::GetSecretMachineCodeWithTimeFromLocal()
{
	//MAC   //�����ֳ�����MAC��ַ���ܴ��ڸ��ĵ������ȡ����MAC��ַ��ȷ��
	//��Ϊ�̶��ַ�����Ϊ��ֵ
	//string macAddr = GetMACAddress();
	string macAddr = "C1A9C1E0-A94D-4F30-BF4A-56576AE8666C";

	//Ӳ�����к�
	string hwSerial;
	std::vector<char> hwId;
	MasterHardDiskSerial diskSerial;
	diskSerial.GetSerialNo(hwId);
	for (int i = 0; i < hwId.size(); i++)
	{
		hwSerial = hwSerial + hwId[i];
	}
	//CPU name
	CPUID *cpuid = CPUID::Instance();
	string cpuBrand = cpuid->GetBrand();

	//����MD5
	crypto::md5_helper_t hhelper;
	string machineCode = hhelper.hexdigesttext(
		hhelper.hexdigesttext(hhelper.hexdigesttext(macAddr.c_str()) + hhelper.hexdigesttext(hwSerial.c_str())) +
		hhelper.hexdigesttext(hhelper.hexdigesttext(hwSerial.c_str()) + hhelper.hexdigesttext(cpuBrand.c_str())) + 
		hhelper.hexdigesttext(hhelper.hexdigesttext(macAddr.c_str()) + hhelper.hexdigesttext(cpuBrand.c_str())));

	//��ȡ��ǰϵͳʱ��
	char ctime[20] = { 0 };
	time_t st_timet = time(NULL);
	tm *systime = localtime(&st_timet);
	//sprintf(ctime, "%04d-%02d-%02d %02d-%02d-%02d.db", systime->tm_year + 1900, systime->tm_mon + 1, systime->tm_mday, systime->tm_hour, systime->tm_min, systime->tm_sec);
	strftime(ctime, sizeof(ctime), "%Y-%m-%d-%H-%M-%S", systime);

	//�ϲ�������͵�ǰϵͳʱ��
	machineCode = machineCode + ctime;

	//����Я��ʱ��Ļ�����
	machineCode = EncryptString(machineCode.c_str(), StringCryptPass);

	return machineCode;
}

void CCryptTool::SplitMachineCode(const string& machineCodeWithTime, string& strMachineCode, string& strTime)
{
	//����Я��ʱ��Ļ�����
	string realCode = DecryptString(machineCodeWithTime.c_str(), StringCryptPass);

	//�ָ�������ʱ��
	strMachineCode = realCode.substr(0, 32);
	strTime = realCode.substr(32);
}

void CCryptTool::SplitSerialKey(const string & serialKey, string & serialKeyMachine, string& serialKeyTime)
{
	//�ָ���������ĺ�ʱ������
	serialKeyMachine = serialKey.substr(0, 256);
	serialKeyTime = serialKey.substr(256);

}

string CCryptTool::SignText(const string& PrivateKey, const string & MachineCodeWithTime, int KeyDuration)
{
	string machineCode, time;
	SplitMachineCode(MachineCodeWithTime, machineCode, time);

	if (KeyDuration == INFINITE_KEY)
	{
		time = __INFINITE_KEY_STR;
	}
	else
	{
		tm tm_ = { 0 };
		sscanf(time.c_str(), "%d-%d-%d-%d-%d-%d", &tm_.tm_year, &tm_.tm_mon, &tm_.tm_mday, &tm_.tm_hour, &tm_.tm_min, &tm_.tm_sec);
		tm_.tm_year = tm_.tm_year - 1900;
		tm_.tm_mon = tm_.tm_mon - 1;

		//���ݵ�ǰʱ�����Ȩʱ�����㵽��ʱ��
		time_t tmCalc = mktime(&tm_);
		tmCalc = tmCalc + KeyDuration * 86400;

		tm* tmCalc_ = localtime(&tmCalc);

		char ctime[20] = { 0 };
		strftime(ctime, sizeof(ctime), "%Y-%m-%d-%H-%M-%S", tmCalc_);
		time = ctime;
	}


	//���ܻ�����
	string serialKeyMachine = RSASignMessage(PrivateKey, machineCode);
	//���ܵ���ʱ��
	string serialKeyTime = EncryptString(time.c_str(), StringCryptPass);

	return serialKeyMachine + serialKeyTime;
}

string CCryptTool::SignText(const string & PrivateKey, const string & MachineCodeWithTime, const char* DueTime)
{
	string machineCode, time;
	SplitMachineCode(MachineCodeWithTime, machineCode, time);

	//���ܻ�����
	string serialKeyMachine = RSASignMessage(PrivateKey, machineCode);
	//���ܵ���ʱ��
	string serialKeyTime = EncryptString(DueTime, StringCryptPass);

	return serialKeyMachine + serialKeyTime;
}

VERIFIER_STATUS CCryptTool::VerifierText(const string& PublicKey, const string & serialKey, const string& MachineCodeWithTime)
{
	VERIFIER_STATUS status = UnRegisted;

	string serialKeyMachine, serialKeyTime;
	SplitSerialKey(serialKey, serialKeyMachine, serialKeyTime);

	string strMachineCode, strTime;
	SplitMachineCode(MachineCodeWithTime, strMachineCode, strTime);

	bool res1 = false, res2 = false;
	try
	{
		//��֤����������
		res1 = RSAVerifyMessage(PublicKey, strMachineCode, serialKeyMachine);
		//��֤ʱ�������Ƿ���
		string strDueTime = DecryptString(serialKeyTime.c_str(), StringCryptPass);
		if (strDueTime == __INFINITE_KEY_STR)
		{
			res2 = true;
		}
		else
		{
			tm tmDue = { 0 };
			sscanf(strDueTime.c_str(), "%d-%d-%d-%d-%d-%d", &tmDue.tm_year, &tmDue.tm_mon, &tmDue.tm_mday, &tmDue.tm_hour, &tmDue.tm_min, &tmDue.tm_sec);
			tmDue.tm_year = tmDue.tm_year - 1900;
			tmDue.tm_mon = tmDue.tm_mon - 1;
			time_t timeDue = mktime(&tmDue);

			tm tmCur = { 0 };
			sscanf(strTime.c_str(), "%d-%d-%d-%d-%d-%d", &tmCur.tm_year, &tmCur.tm_mon, &tmCur.tm_mday, &tmCur.tm_hour, &tmCur.tm_min, &tmCur.tm_sec);
			tmCur.tm_year = tmCur.tm_year - 1900;
			tmCur.tm_mon = tmCur.tm_mon - 1;
			time_t timeCur = mktime(&tmCur);

			res2 = timeCur <= timeDue ? true : false;
		}
	}
	catch (...)
	{
		return UnRegisted;
	}

	if (res1)
	{
		if (res2)
		{
			status = HasRegisted;
		}
		else
		{
			status = OutTime;
		}
	}

	return status;
}

string CCryptTool::GetDurationTimeFromSerialKey(const string & serialKey)
{
	string time;

	string serialKeyMachine, serialKeyTime;
	SplitSerialKey(serialKey, serialKeyMachine, serialKeyTime);

	time = DecryptString(serialKeyTime.c_str(), StringCryptPass);

	if (time != __INFINITE_KEY_STR)
	{
		time.at(10) = ' ';
		time.at(13) = ':';
		time.at(16) = ':';
	}
	else
	{
		time = INFINITE_KEY_STR;
	}


	return time;
}
