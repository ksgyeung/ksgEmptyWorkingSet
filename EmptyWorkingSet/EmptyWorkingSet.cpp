// EmptyWorkingSet.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"

#include "api.h"

const int VERSION = 1;

static void doEmpty()
{
	DWORD pid;
	HANDLE hProcess;

	vector<DWORD> pids = ksgGetAllProcessPids();
	for (vector<DWORD>::iterator it = pids.begin(); it != pids.end(); it++)
	{
		pid = *it;
		if (pid == 0 || pid == 4)
			continue;

		hProcess = ksgOpenProcess(pid);
		if (hProcess != NULL)
		{
			ksgEmptyWorkingSet(hProcess);
			ksgCloseProcess(hProcess);
		}
	}

	SetLastError(ERROR_SUCCESS);
	bool somethingsError = false;

	if (ksgPromoPrivilege(KSG_PROMO_SE_INCREASE_QUOTA_NAME))
	{
		KSG_SYSTEM_CACHE_INFORMATION sci = { 0 };
		sci.MaximumWorkingSet = -1;
		sci.MinimumWorkingSet = -1;
		long result = ksgNTSetSystemInformation(KSG_SCLASS_SYSTEMCACHEINFORMATION, &sci, sizeof(KSG_SYSTEM_CACHE_INFORMATION));
		if (FAILED(result))
		{
			_tprintf_s(_T("ksgNTSetSystemInformation(SYSTEMCACHEINFORMATION) failed (0x%X)\n"), result);
			somethingsError = true;
		}
	}
	else
	{
		_tprintf_s(_T("ksgPromoPrivilege(SE_INCREASE_QUOTA_NAME) failed\n"));
		somethingsError = true;
	}

	if (ksgPromoPrivilege(KSG_PROMO_SE_PROFILE_SINGLE_PROCESS_NAME))
	{
		KSG_SYSTEM_MEMORY_LIST_INFOMATION mli;
		mli.field1 = 0;
		long result = ksgNTSetSystemInformation(KSG_SCLASS_SYSTEMMEMORYLISTINFORMATION, &mli, sizeof(KSG_SYSTEM_MEMORY_LIST_INFOMATION));
		if (FAILED(result))
		{
			_tprintf_s(_T("ksgNTSetSystemInformation(SYSTEMMEMORYLISTINFORMATION) failed (0x%X)\n"), result);
			somethingsError = true;
		}
	}
	else
	{
		_tprintf_s(_T("ksgPromoPrivilege(SE_PROFILE_SINGLE_PROCESS_NAME) failed\n"));
		somethingsError = true;
	}

	if (!somethingsError)
	{
		_tprintf_s(_T("Working Set is emptied\n"));
	}
	else
	{
		_tprintf_s(_T("Working Set is emptied with some errors\n"));
	}
}

static void doHelp(TCHAR* argv0)
{
#ifdef _M_X64
	TCHAR x64[] = _T(" x64");
#else
	TCHAR x64[] = _T("");
#endif

#ifdef _DEBUG
	_tprintf_s(_T("Empty Working Set%s [DEBUG] v%d\n"), x64, VERSION);
#else
	_tprintf_s(_T("Empty Working Set%s v%d\n"), x64, VERSION);
#endif
	_tprintf_s(_T("Author: ksg Yeung\n"), argv0);
	_tprintf_s(_T("Compilation time: %s %s\n"), _T(__DATE__), _T(__TIME__));
	_tprintf_s(_T("Usage: %s [switch]\n"), argv0);
	_tprintf_s(_T("\n"));
	_tprintf_s(_T("Switches:\n"));
	_tprintf_s(_T(" e\tEmpty Working Set\n"));
	_tprintf_s(_T(" h\tShow this help\n"));
	_tprintf_s(_T("\n"));
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc == 1)
	{
		doHelp(argv[0]);
	}
	else
	{
		int len = lstrlen(argv[1]);
		if (len != 1)
		{
			doHelp(argv[0]);
		}
		else
		{
			TCHAR _switch = argv[1][0];

			switch (_switch)
			{
			case 'e':
				doEmpty();
				break;
			case 'h':
			default:
				doHelp(argv[0]);
			}
		}
	}

	return 0;
}

