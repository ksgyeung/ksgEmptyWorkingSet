#define KSG_SCLASS_SYSTEMCACHEINFORMATION					0x15
#define KSG_SCLASS_SYSTEMMEMORYLISTINFORMATION				80

#define KSG_PROMO_SE_PROFILE_SINGLE_PROCESS_NAME			_T("SeProfileSingleProcessPrivilege")
#define KSG_PROMO_SE_INCREASE_QUOTA_NAME 					_T("SeIncreaseQuotaPrivilege")

struct KSG_SYSTEM_CACHE_INFORMATION
{
	ULONG_PTR	CurrentSize;
	ULONG_PTR	PeakSize;
	ULONG_PTR	PageFaultCount;
	ULONG_PTR	MinimumWorkingSet;
	ULONG_PTR	MaximumWorkingSet;
	ULONG_PTR	TransitionSharedPages;
	ULONG_PTR	PeakTransitionSharedPages;
	DWORD       Unused[2];
};

struct KSG_SYSTEM_MEMORY_LIST_INFOMATION
{
	long field1;
};

const int ksgC_MAX_PROCESS_ARRAY_SIZE = 1024;

void ksgEmptyWorkingSet(HANDLE hProcess);
HANDLE ksgOpenProcess(DWORD pid);
void ksgCloseProcess(HANDLE hProcess);
vector<DWORD> ksgGetAllProcessPids();

bool ksgPromoPrivilege(LPCTSTR lpszPrivilege);
long ksgNTSetSystemInformation(int SystemInformationClass, void* SystemInformation, int SystemInformationLength);
long ksgNTQuerySystemInformation(int SystemInformationClass, void* SystemInformation, int SystemInformationLength, PULONG ReturnLength);