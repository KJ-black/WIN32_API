#pragma once
#include "Utils.h"
#include "ErrorHandling.h"

VOID PrintTokenPrimaryGroup(HANDLE hTok) {
	DWORD dwTokLen = GetTokenInfoLength(hTok, TokenPrimaryGroup);
	DWORD dwRetLen;

	PTOKEN_PRIMARY_GROUP ptokprimarygroup = (PTOKEN_PRIMARY_GROUP)malloc(dwTokLen);

	if (!GetTokenInformation(hTok, TokenPrimaryGroup, (LPVOID)ptokprimarygroup, dwTokLen, &dwRetLen)) {
		PrintError(L"GetTokenInformation()");
		return;
	}
	
	std::wcout << L"[+] Token Primary Group" << std::endl;

	// print sid
	LPWSTR lpsid = SIDSerialize(ptokprimarygroup->PrimaryGroup);
	if (!lpsid) {
		PrintError(L"SIDSerizlize()");
	}
	else {
		std::wcout << L"\tSID: " << lpsid << std::endl;
	}
	LocalFree(lpsid);
	lpsid = nullptr;

	// print sid name
	LPWSTR lpName, lpDomainName;
	PSID_NAME_USE psidnameuse;
	lpName = (LPWSTR)VirtualAlloc(NULL, 126, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	lpDomainName = (LPWSTR)VirtualAlloc(NULL, 126, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	psidnameuse = (PSID_NAME_USE)malloc(sizeof(PSID_NAME_USE));
	if (GetSIDName(ptokprimarygroup->PrimaryGroup, lpName, lpDomainName, psidnameuse)) {
		std::wcout << L"\tDomain\\Account (Type):\t" << lpDomainName << L"\\" << lpName << L" (" << getSidNameUse(*psidnameuse) << L")" << std::endl;
		VirtualFree(lpName, 0x0, MEM_RELEASE);
		VirtualFree(lpDomainName, 0x0, MEM_RELEASE);
		lpName = lpDomainName = nullptr;
	}

	// Clean up
	VirtualFree(ptokprimarygroup, 0x0, MEM_RELEASE);
	ptokprimarygroup = nullptr;

}