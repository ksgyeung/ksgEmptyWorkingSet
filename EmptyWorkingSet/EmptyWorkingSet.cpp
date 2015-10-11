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

		hProcess = ksgOpenProcess(pid);
		if (hProcess != NULL)
		{
			ksgEmptyWorkingSet(hProcess);
			ksgCloseProcess(hProcess);
		}
	}
	_tprintf_s(_T("Working Set is emptied\n"));
}

static void doHelp(TCHAR* argv0)
{
	_tprintf_s(_T("Empty Working Set v%d\n"), VERSION);
	_tprintf_s(_T("Author: ksg Yeung\n"), argv0);
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

