const int ksgC_MAX_PROCESS_ARRAY_SIZE = 1024;

void ksgEmptyWorkingSet(HANDLE hProcess);
HANDLE ksgOpenProcess(DWORD pid);
void ksgCloseProcess(HANDLE hProcess);
vector<DWORD> ksgGetAllProcessPids();
