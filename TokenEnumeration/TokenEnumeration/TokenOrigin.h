#pragma once
#include "Utils.h"
#include "ErrorHandling.h"

VOID PrintTokenOrigin(HANDLE hTok) {
	DWORD dwTokLen = GetTokenInfoLength(hTok, TokenOrigin);
	DWORD dwRetLen;

	PTOKEN_ORIGIN ptokorigin= (PTOKEN_ORIGIN)malloc(dwTokLen);

	if (!GetTokenInformation(hTok, TokenOrigin, (LPVOID)ptokorigin, dwTokLen, &dwRetLen)) {
		PrintError(L"GetTokenInformation()");
		return;
	}
	std::wcout << L"[+] Token Origin LogonId: " << std::hex << std::uppercase << ptokorigin->OriginatingLogonSession.HighPart << L":" << ptokorigin->OriginatingLogonSession.LowPart << std::dec << std::nouppercase << std::endl;

	
	// Clean up
	VirtualFree(ptokorigin, 0x0, MEM_RELEASE);
	ptokorigin = nullptr;
}