#pragma once
#include "Utils.h"
#include "ErrorHandling.h"

VOID PrintTokenSandBoxInert(HANDLE hTok) {
	DWORD dwTokLen = GetTokenInfoLength(hTok, TokenSandBoxInert);
	DWORD dwRetLen;

	PDWORD sandbox_inert = (PDWORD)malloc(dwTokLen);

	if (!GetTokenInformation(hTok, TokenSandBoxInert, (LPVOID)sandbox_inert, dwTokLen, &dwRetLen)) {
		PrintError(L"GetTokenInformation()");
		return;
	}
	std::wcout << L"[+] Token SandBox Inert: " << *sandbox_inert << std::endl;

	

	// Clean up
	VirtualFree(sandbox_inert, 0x0, MEM_RELEASE);
	sandbox_inert = nullptr;
}