#pragma once
#include "Utils.h"

VOID PrintTokenUser(HANDLE hTok) {
	DWORD dwTokLen = GetTokenInfoLength(hTok, TokenUser);
	DWORD dwRetLen;

	PTOKEN_USER ptokuser = (PTOKEN_USER)malloc(dwTokLen);

	if (!GetTokenInformation(hTok, TokenUser, (LPVOID)ptokuser, dwTokLen, &dwRetLen)) {
		PrintError(L"GetTokenInformation()");
		return;
	}
	std::wcout << L"[+] User Details" << std::endl;

	// Convert Sid to StingSid which sutable for dispaly
	LPWSTR lpsid = SIDSerialize(ptokuser->User.Sid);
	if (!lpsid) {
		PrintError(L"SIDSerizlize()");
	}
	else {
		std::wcout << L"\tSID: " << lpsid << std::endl;
	}
	LocalFree(lpsid);
	lpsid = nullptr;

	// Look up the account name associated with the SID
	LPWSTR lpName, lpDomainName;
	PSID_NAME_USE psidnameuse;
	lpName = (LPWSTR)VirtualAlloc(NULL, 126, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	lpDomainName = (LPWSTR)VirtualAlloc(NULL, 126, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	psidnameuse = (PSID_NAME_USE)malloc(sizeof(PSID_NAME_USE));
	if (GetSIDName(ptokuser->User.Sid, lpName, lpDomainName, psidnameuse)) {
		std::wcout << L"\tDomain\\Account (Type):\t" << lpDomainName << L"\\" << lpName << L" (" << getSidNameUse(*psidnameuse) << L")" << std::endl;
		VirtualFree(lpName, 0x0, MEM_RELEASE);
		VirtualFree(lpDomainName, 0x0, MEM_RELEASE);
		lpName = lpDomainName = nullptr;
	}
	
	// Clean up
	VirtualFree(ptokuser, 0x0, MEM_RELEASE);
	ptokuser = nullptr;
}