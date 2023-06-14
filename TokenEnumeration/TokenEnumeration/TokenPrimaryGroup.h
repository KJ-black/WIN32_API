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

	PrintSID(ptokprimarygroup->PrimaryGroup);

	// Clean up
	VirtualFree(ptokprimarygroup, 0x0, MEM_RELEASE);
	ptokprimarygroup = nullptr;

}