#include "stdafx.h"

#include "api.h"

//#pragma comment(lib,"Psapi.lib")

void ksgEmptyWorkingSet(HANDLE hProcess)
{
	EmptyWorkingSet(hProcess);
}

HANDLE ksgOpenProcess(DWORD pid)
{
	return OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_SET_QUOTA, false, pid);
}

void ksgCloseProcess(HANDLE hProcess)
{
	CloseHandle(hProcess);
}

vector<DWORD> ksgGetAllProcessPids()
{
	DWORD* pids = new DWORD[ksgC_MAX_PROCESS_ARRAY_SIZE];
	DWORD count = 0;
	EnumProcesses(pids, ksgC_MAX_PROCESS_ARRAY_SIZE, &count);

	vector<DWORD> vPids;
	for (unsigned i = 0; i < count; i++)
	{
		vPids.push_back(pids[i]);
	}

	delete[] pids;
	return vPids;
}

