#include <windows.h>
#include <iphlpapi.h>
#include "MACAddress.h"

#pragma comment(lib, "Iphlpapi.lib")

string GetMACAddress()
{
	string strMacAddress;

	// Get the buffer length required for IP_ADAPTER_INFO.
	ULONG BufferLength = 0;
	BYTE* pBuffer = 0;
	if (ERROR_BUFFER_OVERFLOW == GetAdaptersInfo(0, &BufferLength))
	{
		// Now the BufferLength contain the required buffer length.
		// Allocate necessary buffer.
		pBuffer = new BYTE[BufferLength];
	}
	else
	{
		// Error occurred. handle it accordingly.
	}

	// Get the Adapter Information.
	PIP_ADAPTER_INFO pAdapterInfo =
		reinterpret_cast<PIP_ADAPTER_INFO>(pBuffer);
	GetAdaptersInfo(pAdapterInfo, &BufferLength);

	// Assuming pAdapterInfo->AddressLength is 6.
	if (pAdapterInfo)
	{
		char mac[18] = { 0 };
		sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x",
			pAdapterInfo->Address[0],
			pAdapterInfo->Address[1],
			pAdapterInfo->Address[2],
			pAdapterInfo->Address[3],
			pAdapterInfo->Address[4],
			pAdapterInfo->Address[5]);

		strMacAddress = mac;
	}

	return strMacAddress;
}
