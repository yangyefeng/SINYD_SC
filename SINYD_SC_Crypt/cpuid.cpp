#include "cpuid.h"

CPUID* CPUID::m_pInstance = NULL;

CPUID * CPUID::Instance()
{
	if (NULL == m_pInstance)		// if instance is not present, create a new instance
	{
		m_pInstance = new CPUID();
	}

	return m_pInstance;
}

CPUID::CPUID()
:m_eax(0)
,m_ebx(0)
,m_ecx(0)
,m_edx(0)
{
	// create cache information table at constructint time

	m_cache[0x06] =  CacheInfo(1, 8, 4, 32);
	m_cache[0x08] =  CacheInfo(1, 16, 4, 32);
	m_cache[0x0a] =  CacheInfo(1, 8, 2, 32);
	m_cache[0x0c] =  CacheInfo(1, 16, 4, 32);
	m_cache[0x2c] =  CacheInfo(1, 32, 8, 64);
	m_cache[0x30] =  CacheInfo(1, 32, 8, 64);
	m_cache[0x60] =  CacheInfo(1, 16, 8, 64);
	m_cache[0x66] =  CacheInfo(1, 8, 4, 64);
	m_cache[0x67] =  CacheInfo(1, 16, 4, 64);
	m_cache[0x68] =  CacheInfo(1, 32, 4, 64);

	m_cache[0x39] =  CacheInfo(2, 128, 4, 64);
	m_cache[0x3b] =  CacheInfo(2, 128, 2, 64);
	m_cache[0x3c] =  CacheInfo(2, 256, 4, 64);
	m_cache[0x41] =  CacheInfo(2, 128, 4, 32);
	m_cache[0x42] =  CacheInfo(2, 256, 4, 32);
	m_cache[0x43] =  CacheInfo(2, 512, 4, 32);
	m_cache[0x44] =  CacheInfo(2, 1024, 4, 32);
	m_cache[0x45] =  CacheInfo(2, 2048, 4, 32);
	m_cache[0x79] =  CacheInfo(2, 128, 8, 64);
	m_cache[0x7a] =  CacheInfo(2, 256, 8, 64);
	m_cache[0x7b] =  CacheInfo(2, 512, 8, 64);
	m_cache[0x7c] =  CacheInfo(2, 1024, 8, 64);
	m_cache[0x82] =  CacheInfo(2, 256, 8, 32);
	m_cache[0x83] =  CacheInfo(2, 512, 8, 32);
	m_cache[0x84] =  CacheInfo(2, 1024, 8, 32);
	m_cache[0x85] =  CacheInfo(2, 2048, 8, 32);
	m_cache[0x86] =  CacheInfo(2, 512, 4, 64);
	m_cache[0x87] =  CacheInfo(2, 1024, 8, 64);

	m_cache[0x22] =  CacheInfo(3, 512, 4, 64);
	m_cache[0x23] =  CacheInfo(3, 1024, 8, 64);
	m_cache[0x25] =  CacheInfo(3, 2048, 8, 64);
	m_cache[0x29] =  CacheInfo(3, 4096, 8, 64);

}

CPUID::~CPUID()
{
	delete m_pInstance;		// delete instance
	m_pInstance = NULL;		// set the pointer to NULL
}

string CPUID::GetVID()
{
	char cVID[13];				// Store Vender ID String, contain 12 chars
	memset(cVID, 0, 13);		// Initialize all chars to 0
	Executecpuid(0);			// Execute cpuid instruction with eax = 0
	memcpy(cVID, &m_ebx, 4);	// ebx contains the first 4 chars
	memcpy(cVID+4, &m_edx, 4);	// edx contains the middle 4 chars
	memcpy(cVID+8, &m_ecx, 4);	// ecx contains the last 4 chars;

	return string(cVID);		// convert to string and return
}

string CPUID::GetBrand()
{
	const DWORD BRANDID = 0x80000002;		// brand parameter start at 0x80000002
	char cBrand[49];						// Store Brand String, contain 48 chars
	memset(cBrand, 0, 49);					// Initialize all chars to 0

	for (DWORD i = 0; i < 3; i++)			// Execute cpuid instruction with 
	{										// eax = 0x80000002 to 0x80000004
		Executecpuid(BRANDID + i);			
		memcpy(cBrand + i*16, &m_eax, 16);	// eax, ebx, ecx, edx contain the brand string
	}

	return string(cBrand);		// convert to string and return
}

DWORD CPUID::GetCacheInfo(CacheInfo& L1, CacheInfo& L2, CacheInfo& L3)
{
	BYTE cValues[16];						// Totally 16 byte information
	DWORD result = 0;						// Cache number
	Executecpuid(2);						// Execute cpuid instruction with eax = 2
	memcpy(cValues, &m_eax, 16);			// copy return value

	for (int i = 1; i < 16; i++)
	{
		if (m_cache.find(cValues[i]) != m_cache.end())	// if this information is about cache
		{
			switch (m_cache[cValues[i]].level)			// Get cache information from table
			{
			case 1:		// L1 cache
				L1 = m_cache[cValues[i]];
				break;
			case 2:		// L2 cache
				L2 = m_cache[cValues[i]];
				break;
			case 3:		// L3 cache
				L3 = m_cache[cValues[i]];
				break;
			default:
				break;
			}
			result++;
		}
		
	}

	return result;
}

bool CPUID::GetSerialNumber(SerialNumber& serial)
{
	Executecpuid(1);	// Execute cpuid instruction with eax = 1
	bool isSupport = m_edx & (1<<18);	// Check if Serial Number is supported
	if (false == isSupport)	// not support
	{
		return false;
	}
	memcpy(&serial.nibble[4], &m_eax, 4);	// eax is the higher 2 nibble

	Executecpuid(3);	// Execute cpuid instruction with eax = 3
	memcpy(&serial.nibble[0], &m_ecx, 8);	// ecx and edx constains the middle and first 4 nibble

	return true;
}

void CPUID::Executecpuid(DWORD veax)
{
	// The following 4 DWORD variable store the four registers values after cpuid
	DWORD deax;
	DWORD debx;
	DWORD decx;
	DWORD dedx;

	__asm
	{
		mov eax, veax
		cpuid
		mov deax, eax
		mov debx, ebx
		mov decx, ecx
		mov dedx, edx
	}

	m_eax = deax;	// store in class member
	m_ebx = debx;
	m_ecx = decx;
	m_edx = dedx;
}

bool CPUID::IsHyperThreading()
{
	Executecpuid(1);			// Execute cpuid instruction with eax = 1

	return m_edx & (1<<28);		// bit 28 be set represent hyper-threading is supported
}

bool CPUID::IsEST()
{
	Executecpuid(1);			// Execute cpuid instruction with eax = 1

	return m_ecx & (1<<7);		// bit 7 be set represent Enhanced Speed Step is supported
}

bool CPUID::IsMMX()
{
	Executecpuid(1);			// Execute cpuid instruction with eax = 1

	return m_edx & (1<<23);		// bit 28 be set represent MMX is supported
}