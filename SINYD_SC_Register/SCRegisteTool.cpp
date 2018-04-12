#include "SCRegisteTool.h"
#include <windows.h>


#define KEY_FILE_NAME "SOFTDAT"

CSCRegisteTool::CSCRegisteTool()
{
}


CSCRegisteTool::~CSCRegisteTool()
{
}

VERIFIER_STATUS CSCRegisteTool::GetSoftRegistStatus()
{
	CCryptTool tool;

	VERIFIER_STATUS status = UnRegisted;

	FILE* keyFile = fopen(KEY_FILE_NAME, "rt");
	if (keyFile)
	{
		char cKey[SERIAL_KEY_STR_LEN + 1] = { 0 };
		int len = fread(cKey, sizeof (char), SERIAL_KEY_STR_LEN, keyFile);
		if (len == SERIAL_KEY_STR_LEN)
		{
			status = tool.VerifierText(publicKey, cKey, tool.GetSecretMachineCodeWithTimeFromLocal());
		}
		fclose(keyFile);
	}

	return status;
}

string CSCRegisteTool::GetSoftRegistDurationTime()
{
	CCryptTool tool;

	string time;

	FILE* keyFile = fopen(KEY_FILE_NAME, "rt");
	if (keyFile)
	{
		char cKey[SERIAL_KEY_STR_LEN + 1] = { 0 };
		int len = fread(cKey, sizeof(char), SERIAL_KEY_STR_LEN, keyFile);
		if (len == SERIAL_KEY_STR_LEN)
		{
			time = tool.GetDurationTimeFromSerialKey(cKey);
		}
		fclose(keyFile);
	}

	return time;
}

VERIFIER_STATUS CSCRegisteTool::RegistSoft(const string & strKey)
{
	CCryptTool tool;
	VERIFIER_STATUS stat = tool.VerifierText(publicKey, strKey, tool.GetSecretMachineCodeWithTimeFromLocal());
	if (stat == HasRegisted)
	{
		FILE* keyFile = fopen(KEY_FILE_NAME, "w+");
		if (keyFile)
		{
			int len = fprintf(keyFile, strKey.c_str());
			if (len != strKey.size())
			{
				stat = UnRegisted;
			}
			else
			{
				stat = HasRegisted;
				OpenAppAutoStart();
			}
			fclose(keyFile);
		}
	}

	return stat;
}

#define CORE_APP_EXE_NAME L"SINYD_SC_CoreApp.exe"
#define APP_MANAGER_EXE_NAME L"SINYD_SC_AppManager.exe"
#define CORE_APP_AUTOVALUE_NAME L"SINYD_SC_COREAPP"
#define APP_MANAGER_AUTOVALUE_NAME L"SINYD_SC_APPMGR"

void CSCRegisteTool::OpenAppAutoStart()
{
	// �������Ӽ����ơ��͵��Ӽ��ľ��
	HKEY hRoot = HKEY_LOCAL_MACHINE;
	TCHAR szSubKey[MAX_PATH] = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
	HKEY hKey;

	// ��ָ���Ӽ�
	LONG lRet = ::RegOpenKeyEx(hRoot, szSubKey, 0, KEY_ALL_ACCESS, &hKey);
	if (lRet == ERROR_SUCCESS)
	{
		// �õ���ǰִ���ļ����ļ�·��
		TCHAR szCurDir[MAX_PATH] = { 0 };
		::GetCurrentDirectory(MAX_PATH, szCurDir);

		wstring strAppDir = szCurDir;
		char cEnd = *(strAppDir.crbegin());
		if (cEnd != L'\\')
		{
			strAppDir = strAppDir + L'\\';
		}

		wstring strCoreAppPath = strAppDir + CORE_APP_EXE_NAME;
		wstring strAppMgrPath = strAppDir + APP_MANAGER_EXE_NAME;


		// ����һ���µļ�ֵ�����ü�ֵ����Ϊ�ļ���
		::RegSetValueEx(hKey, CORE_APP_AUTOVALUE_NAME, 0, REG_SZ, (const BYTE*)strCoreAppPath.c_str(), strCoreAppPath.size()*sizeof(wchar_t));
		::RegSetValueEx(hKey, APP_MANAGER_AUTOVALUE_NAME, 0, REG_SZ, (const BYTE*)strAppMgrPath.c_str(), strAppMgrPath.size()*sizeof(wchar_t));

		// �ر��Ӽ����
		::RegCloseKey(hKey);
	}
}

void CSCRegisteTool::CloseAppAutoStart()
{
	// �������Ӽ����ơ��͵��Ӽ��ľ��
	HKEY hRoot = HKEY_LOCAL_MACHINE;
	TCHAR szSubKey[MAX_PATH] = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
	HKEY hKey;

	// ��ָ���Ӽ�
	LONG lRet = ::RegOpenKeyEx(hRoot, szSubKey, 0, KEY_ALL_ACCESS, &hKey);
	if (lRet == ERROR_SUCCESS)
	{
		// ɾ���Զ�����ע�����
		::RegDeleteValue(hKey, CORE_APP_AUTOVALUE_NAME);
		::RegDeleteValue(hKey, APP_MANAGER_AUTOVALUE_NAME);

		// �ر��Ӽ����
		::RegCloseKey(hKey);
	}
}
