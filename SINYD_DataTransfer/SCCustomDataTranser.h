#ifndef _SC_CUSTOM_DATA_TRANSER_H_
#define _SC_CUSTOM_DATA_TRANSER_H_

#ifdef _WIN32
#ifdef SC_DATA_TRANSFER_DLL_EXPORT
#define SC_DATA_TRANSFER __declspec(dllexport)
#else 
#define SC_DATA_TRANSFER __declspec(dllimport)
#endif
#else
#define SC_DATA_TRANSFER
#endif

#include <string>
#include <list>
#include <map>
#include "SYDDataTransferDef.h"
using namespace std;
class SC_DATA_TRANSFER CSCCustomDataTranser
{
public:
	CSCCustomDataTranser(const std::string& ip, int port);
	~CSCCustomDataTranser();

	void Init(const SourceInfo & info);
	bool TransData(int contentType, int dataType, const map<string, string>& data);
	//bool TransData(int contentType, int dataType, const list<map<string, string>>& data);

private:
	std::string m_ip;
	int m_port;
};

#endif	_SC_CUSTOM_DATA_TRANSER_H_
