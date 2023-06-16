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

	PrintSID(ptokowner->Owner);

	// Clean up
	VirtualFree(ptokowner, 0x0, MEM_RELEASE);
	ptokowner = nullptr;
}
