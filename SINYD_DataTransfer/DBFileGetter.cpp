#include "DBFileGetter.h"
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <direct.h>
#include <io.h>
#else
#include <stdarg.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#endif

#ifdef _WIN32
#define ACCESS _access
#define MKDIR(a) _mkdir((a))
#else
#define ACCESS access
#define MKDIR(a) mkdir((a),0777)
#endif

#include "MeasureDef.h"
#include <algorithm>
#include <time.h>

CDBFileGetter::CDBFileGetter()
{
}


CDBFileGetter::~CDBFileGetter()
{
}

bool DBNameCompare(const DBFileInfo& pfirst, const DBFileInfo& psecond)
{
	return pfirst.time < psecond.time;
}

void CDBFileGetter::RefreshList()
{
	_vecDBFile.clear();
#ifdef _WIN32
	_finddata_t fileDir;
	long lfDir;

	if ((lfDir = _findfirst(".\\DB\\*.db", &fileDir)) != -1l)
	{
		do {
			if (fileDir.attrib & _A_SUBDIR) {
				continue;
			}
			if (strstr(fileDir.name, ".db") != NULL)
			{
				tm tm_ = { 0 };
				sscanf(fileDir.name + (strlen(fileDir.name) - 22), "%d-%d-%d %d_%d_%d.db", &tm_.tm_year, &tm_.tm_mon, &tm_.tm_mday, &tm_.tm_hour, &tm_.tm_min, &tm_.tm_sec);
				tm_.tm_year = tm_.tm_year - 1900;
				tm_.tm_mon = tm_.tm_mon - 1;
				time_t time = mktime(&tm_);

				string fullname(fileDir.name);
				string fullnameWithPath = ".\\DB\\" + fullname;

				_M.Lock();
				//修改为使用带全路径的DB路径，用于针对新采集箱需要将DB保存在内置内存，并备份到SD卡的操作，保证多路径的传输读取
				_vecDBFile.push_back(DBFileInfo(fullname.substr(0, fullname.size() - 23), /*fullname*/fullnameWithPath, time));
				_M.Unlock();
			}
		} while (_findnext(lfDir, &fileDir) == 0);
	}
	_findclose(lfDir);
#else
    char fileDir[256];
	DIR *dir;
	struct dirent *ptr;
	dir = opendir(CONFIG_DB_PATH);

	while (dir != NULL && (ptr = readdir(dir)) != NULL) {
        strcpy(fileDir,ptr->d_name);
        if (strcmp(fileDir, ".") == 0 || strcmp(fileDir, "..") == 0)
			continue;
		if (ptr->d_type == DT_DIR) {
			continue;
		}
        if (strstr(fileDir, ".db") != NULL)
		{
			tm tm_ = { 0 };
            sscanf(fileDir + (strlen(fileDir) - 22), "%d-%d-%d %d_%d_%d.db", &tm_.tm_year, &tm_.tm_mon, &tm_.tm_mday, &tm_.tm_hour, &tm_.tm_min, &tm_.tm_sec);
			tm_.tm_year = tm_.tm_year - 1900;
			tm_.tm_mon = tm_.tm_mon - 1;
			time_t time = mktime(&tm_);

            string prefix(fileDir);
            prefix =prefix.substr(0, prefix.size() - 23);

            string fullname(CONFIG_DB_PATH);
            if( fullname.at(fullname.length()-1)!='/'){
                fullname +="/";
            }
            fullname += fileDir;

            _M.Lock();
			//修改为使用带全路径的DB路径，用于针对新采集箱需要将DB保存在内置内存，并备份到SD卡的操作，保证多路径的传输读取
            _vecDBFile.push_back(DBFileInfo(prefix, /*fileDir*/fullname, time));
			_M.Unlock();
		}
	}
	closedir(dir);

	dir = opendir(CONFIG_BACKUP_DB_PATH);

	while (dir != NULL && (ptr = readdir(dir)) != NULL) {
		strcpy(fileDir, ptr->d_name);
		if (strcmp(fileDir, ".") == 0 || strcmp(fileDir, "..") == 0)
			continue;
		if (ptr->d_type == DT_DIR) {
			continue;
		}
		if (strstr(fileDir, ".db") != NULL)
		{
			tm tm_ = { 0 };
			sscanf(fileDir + (strlen(fileDir) - 22), "%d-%d-%d %d_%d_%d.db", &tm_.tm_year, &tm_.tm_mon, &tm_.tm_mday, &tm_.tm_hour, &tm_.tm_min, &tm_.tm_sec);
			tm_.tm_year = tm_.tm_year - 1900;
			tm_.tm_mon = tm_.tm_mon - 1;
			time_t time = mktime(&tm_);

			string prefix(fileDir);
			prefix = prefix.substr(0, prefix.size() - 23);

			string fullname(CONFIG_BACKUP_DB_PATH);
			if (fullname.at(fullname.length() - 1) != '/') {
				fullname += "/";
			}
			fullname += fileDir;

			_M.Lock();
			//修改为使用带全路径的DB路径，用于针对新采集箱需要将DB保存在内置内存，并备份到SD卡的操作，保证多路径的传输读取
			_vecDBFile.push_back(DBFileInfo(prefix, /*fileDir*/fullname, time));
			_M.Unlock();
		}
	}
	closedir(dir);
#endif
	sort(_vecDBFile.begin(), _vecDBFile.end(), DBNameCompare);
}

void CDBFileGetter::GetDBList(const string & prefix, list<string>& listFullName)
{
	_M.Lock();
	int total = _vecDBFile.size();
	for (int i = 0; i < total; i++)
	{
		if (_vecDBFile[i].prefix == prefix)
		{
			listFullName.push_back(_vecDBFile[i].fullname);
		}
	}
	_M.Unlock();
}

void CDBFileGetter::GetDBListAfterBK(const string & prefix, const string & breakPointFullname, list<string>& listFullName)
{
	listFullName.clear();
	_M.Lock();
	bool bFindBK = false;
	int total = _vecDBFile.size();
	for (int i = 0; i < total; i++)
	{
		if (breakPointFullname == "")
		{
			bFindBK = true;
		}
#if _WIN32
		string bpFilename = breakPointFullname.substr(breakPointFullname.find_last_of('\\') + 1);
		string vecFilename = _vecDBFile[i].fullname.substr(_vecDBFile[i].fullname.find_last_of('\\') + 1);
#else
		string bpFilename = breakPointFullname.substr(breakPointFullname.find_last_of('/') + 1);
		string vecFilename = _vecDBFile[i].fullname.substr(_vecDBFile[i].fullname.find_last_of('/') + 1);
#endif
		if (bpFilename == vecFilename)
		//if (!bFindBK && breakPointFullname == _vecDBFile[i].fullname)
		{
			bFindBK = true;
		}
		if (bFindBK)
		{
			if (_vecDBFile[i].prefix == prefix)
			{
				listFullName.push_back(_vecDBFile[i].fullname);
			}
		}
	}
	if (!bFindBK)
	{
		tm tmBP = { 0 };
		sscanf(breakPointFullname.c_str() + (breakPointFullname.size() - 22), "%d-%d-%d %d_%d_%d.db", &tmBP.tm_year, &tmBP.tm_mon, &tmBP.tm_mday, &tmBP.tm_hour, &tmBP.tm_min, &tmBP.tm_sec);
		tmBP.tm_year = tmBP.tm_year - 1900;
		tmBP.tm_mon = tmBP.tm_mon - 1;
		time_t timeBP = mktime(&tmBP);

		for (int i = 0; i < total; i++)
		{
			if (_vecDBFile[i].time >= timeBP)
			{
				bFindBK = true;
			}
			if (bFindBK)
			{
				if (_vecDBFile[i].prefix == prefix)
				{
					listFullName.push_back(_vecDBFile[i].fullname);
				}
			}
		}
	}
	_M.Unlock();
}
