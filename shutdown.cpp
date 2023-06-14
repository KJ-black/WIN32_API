#include <windows.h>

const unsigned int SE_SHUTDOWN_PRIVILEGE = 0x13;

int main()
{
	HMODULE hDll = ::LoadLibrary("ntdll.dll");
	typedef int (* type_RtlAdjustPrivilege)(int, bool, bool, int*);
	typedef int (* type_ZwShutdownSystem)(int);
	type_RtlAdjustPrivilege RtlAdjustPrivilege = (type_RtlAdjustPrivilege)GetProcAddress(hDll, "RtlAdjustPrivilege");
	type_ZwShutdownSystem ZwShutdownSystem = (type_ZwShutdownSystem)GetProcAddress(hDll, "ZwShutdownSystem");
	int nEn = 0;
	int nResult = RtlAdjustPrivilege(SE_SHUTDOWN_PRIVILEGE, true, true, &nEn);
	if(nResult == 0x0c000007c){
		nResult = RtlAdjustPrivilege(SE_SHUTDOWN_PRIVILEGE, true, false, &nEn);
	}
	nResult = ZwShutdownSystem(2);
	FreeLibrary(hDll);
	return 0;

}