#pragma once
#include "Utils.h"
#include "ErrorHandling.h"

VOID PrintTokenOwner(HANDLE hTok) {
	DWORD dwTokLen = GetTokenInfoLength(hTok, TokenOwner);
	DWORD dwRetLen;

	PTOKEN_OWNER ptokowner = (PTOKEN_OWNER)malloc(dwTokLen); // (NULL, dwTokLen, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	if (!GetTokenInformation(hTok, TokenOwner, (LPVOID)ptokowner, dwTokLen, &dwRetLen)) {
		PrintError(L"GetTokenInformation()");
		return;
	}

	std::wcout << L"[+] Token Owner" << std::endl;

	// print sid
	LPWSTR lpsid = SIDSerialize(ptokowner->Owner);
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
	if (GetSIDName(ptokowner->Owner, lpName, lpDomainName, psidnameuse)) {
		std::wcout << L"\tDomain\\Account (Type):\t" << lpDomainName << L"\\" << lpName << L" (" << getSidNameUse(*psidnameuse) << L")" << std::endl;
		VirtualFree(lpName, 0x0, MEM_RELEASE);
		VirtualFree(lpDomainName, 0x0, MEM_RELEASE);
		lpName = lpDomainName = nullptr;
	}

	// Clean up
	VirtualFree(ptokowner, 0x0, MEM_RELEASE);
	ptokowner = nullptr;
}
