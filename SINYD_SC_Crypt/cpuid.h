#ifndef CPUID_H
#define CPUID_H

#include <string>
#include <map>
using namespace std;

typedef unsigned long DWORD;
typedef unsigned char BYTE;
typedef unsigned short WORD;

// Cache Information 
struct CacheInfo
{
	int level;				// level
	int size;				// size
	int way;				// way
	int linesize;			// linesize

	CacheInfo()				// constructor
	{
		level = 0;
		size = 0;
		way = 0;
		linesize = 0;
	}

	CacheInfo(int clevel, int csize, int cway, int clinesize) //constructor
	{
		level = clevel;
		size = csize;
		way = cway;
		linesize = clinesize;
	}
};

// CPU SerialNumber
// comprise 6 WORD nibble, fommatting xxxx-xxxx-xxxx-xxxx-xxxx-xxxx
struct SerialNumber
{
	WORD nibble[6];			// 6 WORD nibble;

	SerialNumber()			//constructor
	{
		memset(nibble, 0, sizeof(nibble));
	}
};


// singleton class CPUID
// Get CPU information
class CPUID
{
public:	
	static CPUID *Instance();					// Get instance pointer

	string GetVID();							// Get Vender ID String
	string GetBrand();							// Get Brand String

	bool GetSerialNumber(SerialNumber& serial);	// Get CPU Serial Number

	bool IsHyperThreading();					// Check if hyper-threading is supported
	bool IsEST();								// Check if Enhanced Speed Step is supported
	bool IsMMX();								// Check if MMX is supported

	// Get cache information
	DWORD GetCacheInfo(CacheInfo& L1, CacheInfo& L2, CacheInfo& L3);

private:
	CPUID();			// constructor
	virtual ~CPUID();	// d-constructor

	DWORD m_eax;		// value eax after cpuid
	DWORD m_ebx;		// value ebx after cpuid
	DWORD m_ecx;		// value ecx after cpuid
	DWORD m_edx;		// value edx after cpuid

	map<int, CacheInfo> m_cache;	// Cache information table

	static CPUID * m_pInstance;		// Instance pointer

	void Executecpuid(DWORD eax);	// Execute cpuid
};

#endif