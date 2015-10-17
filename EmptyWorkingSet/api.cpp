#include "stdafx.h"

#include "api.h"

//#pragma comment(lib,"Psapi.lib")
#pragma comment(lib,"ntdll.lib")

void ksgEmptyWorkingSet(HANDLE hProcess)
{
	//SetProcessWorkingSetSize(hProcess, (SIZE_T)-1, (SIZE_T)-1);
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

bool ksgPromoPrivilege(LPCTSTR lpszPrivilege)
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tp;
	LUID luid;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken))
	{
		return false;
	}

	if (!LookupPrivilegeValue(
		NULL,            // lookup privilege on local system
		lpszPrivilege,   // privilege to lookup 
		&luid))        // receives LUID of privilege
	{
		printf("LookupPrivilegeValue error: %u\n", GetLastError());
		CloseHandle(hToken);
		return FALSE;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// Enable the privilege or disable all privileges.

	if (!AdjustTokenPrivileges(
		hToken,
		FALSE,
		&tp,
		sizeof(TOKEN_PRIVILEGES),
		(PTOKEN_PRIVILEGES)NULL,
		(PDWORD)NULL))
	{
		printf("AdjustTokenPrivileges error: %u\n", GetLastError());
		CloseHandle(hToken);
		return false;
	}

	CloseHandle(hToken);

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)

	{
		printf("The token does not have the specified privilege. \n");
		return false;
	}

	return true;
}

long ksgNTSetSystemInformation(int SystemInformationClass, void* SystemInformation, int SystemInformationLength)
{
	long result = LONG_MIN;

	HMODULE hLib = LoadLibrary(_T("ntdll.dll"));
	if (hLib)
	{
		void* addr = GetProcAddress(hLib, "NtSetSystemInformation");
		if (addr)
		{
			NTSTATUS(NTAPI *PNTSetSystemInformation)(IN int, IN PVOID, IN ULONG) = (NTSTATUS(NTAPI *)(IN int, IN PVOID, IN ULONG))addr;
			result = PNTSetSystemInformation(SystemInformationClass, SystemInformation, SystemInformationLength);
		}

		FreeLibrary(hLib);
	}

	return result;
}

long ksgNTQuerySystemInformation(int SystemInformationClass, void* SystemInformation, int SystemInformationLength, PULONG ReturnLength)
{
	long result = LONG_MIN;

	HMODULE hLib = LoadLibrary(_T("ntdll.dll"));
	if (hLib)
	{
		void* addr = GetProcAddress(hLib, "NtQuerySystemInformation");
		if (addr)
		{
			NTSTATUS(NTAPI *PNtQuerySystemInformation)(IN int, IN PVOID, IN ULONG, PULONG) = (NTSTATUS(NTAPI *)(IN int, IN PVOID, IN ULONG, PULONG))addr;
			result = PNtQuerySystemInformation(SystemInformationClass, SystemInformation, SystemInformationLength, ReturnLength);
		}

		FreeLibrary(hLib);
	}

	return result;
}