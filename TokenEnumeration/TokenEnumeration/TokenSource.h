#pragma once
#include "Utils.h"
#include "ErrorHandling.h"

VOID PrintTokenSource(HANDLE hTok) {
	DWORD dwTokLen = GetTokenInfoLength(hTok, TokenSource);
	DWORD dwRetLen;

	PTOKEN_SOURCE ptoksource = (PTOKEN_SOURCE)malloc(dwTokLen);

	if (!GetTokenInformation(hTok, TokenSource, (LPVOID)ptoksource, dwTokLen, &dwRetLen)) {
		PrintError(L"GetTokenInformation()");
		return;
	}
	std::wcout << L"[+] Token Source" << std::endl;
	std::wcout << L"\t Source Name: " << ptoksource->SourceName << std::endl;
	std::wcout << L"\t Source Identifier: " << std::hex << ptoksource->SourceIdentifier.HighPart << L":" << ptoksource->SourceIdentifier.LowPart << std::dec << std::endl;

	// Clean up
	VirtualFree(ptoksource, 0x0, MEM_RELEASE);
	ptoksource = nullptr;
}