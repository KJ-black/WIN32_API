#pragma once
#include "Utils.h"
#include "ErrorHandling.h"

VOID PrintTokenRestrictedSids(HANDLE hTok) {
	DWORD dwTokLen = GetTokenInfoLength(hTok, TokenRestrictedSids);
	DWORD dwRetLen;

	PTOKEN_GROUPS ptokgroup = (PTOKEN_GROUPS)malloc(dwTokLen);

	if (!GetTokenInformation(hTok, TokenRestrictedSids, (LPVOID)ptokgroup, dwTokLen, &dwRetLen)) {
		PrintError(L"GetTokenInformation()");
		return;
	}
	std::wcout << L"[+] Token Restricted Sids:";

	if (!ptokgroup->GroupCount)
		std::wcout << L" None" << std::endl;
	else
		std::wcout << std::endl;
	for (DWORD i = 0; i < ptokgroup->GroupCount; i++) {
		std::wcout << L"\t[" << std::setw(2) << i + 1 << L"] ";

		// Print sid
		LPWSTR lpsid = SIDSerialize(ptokgroup->Groups[i].Sid);
		if (!lpsid) {
			PrintError(L"SIDSerialize()");
		}
		else {
			std::wcout << L"SID: " << lpsid << std::endl;
		}
		LocalFree(lpsid);
		lpsid = nullptr;

		// Print sid name
		LPWSTR lpName, lpDomainName;
		PSID_NAME_USE psidnameuse;
		lpName = (LPWSTR)VirtualAlloc(NULL, 512, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		lpDomainName = (LPWSTR)VirtualAlloc(NULL, 512, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		psidnameuse = (PSID_NAME_USE)malloc(sizeof(PSID_NAME_USE));
		if (GetSIDName(ptokgroup->Groups[i].Sid, lpName, lpDomainName, psidnameuse)) {
			if (lpDomainName[0] != L'\0' && lpName[0] != L'\0')
				std::wcout << L"\t     Domain\\Account (Type): " << lpDomainName << L"\\" << lpName << L" (" << getSidNameUse(*psidnameuse) << L")" << std::endl;
			else if (lpName)
				std::wcout << L"\t     Domain\\Account (Type): " << lpName << L" (" << getSidNameUse(*psidnameuse) << L")" << std::endl;
			VirtualFree(lpName, 0x0, MEM_RELEASE);
			VirtualFree(lpDomainName, 0x0, MEM_RELEASE);
			lpName = lpDomainName = nullptr;
		}
	}


	// Clean up
	VirtualFree(ptokgroup, 0x0, MEM_RELEASE);
	ptokgroup = nullptr;
}