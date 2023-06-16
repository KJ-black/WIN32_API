#pragma once
#include "Utils.h"
#include "ErrorHandling.h"

VOID PrintTokenStatistics(HANDLE hTok) {
	DWORD dwTokLen = GetTokenInfoLength(hTok, TokenStatistics);
	DWORD dwRetLen;

	PTOKEN_STATISTICS ptokstatistics = (PTOKEN_STATISTICS)malloc(dwTokLen);

	if (!GetTokenInformation(hTok, TokenStatistics, (LPVOID)ptokstatistics, dwTokLen, &dwRetLen)) {
		PrintError(L"GetTokenInformation()");
		return;
	}
	std::wcout << L"[+] Token Statisticss" << std::endl;

	std::wcout << L"\t TokenId: " << std::hex << std::uppercase << ptokstatistics->TokenId.HighPart << L":" << ptokstatistics->TokenId.LowPart << std::dec << std::nouppercase << std::endl;
	std::wcout << L"\t AuthenticationId: " << std::hex << std::uppercase << ptokstatistics->AuthenticationId.HighPart << L":" << ptokstatistics->AuthenticationId.LowPart << std::dec << std::nouppercase << std::endl;
	//std::wcout << L"\t ExpirationTime: " << ptokstatistics->ExpirationTime.LowPart << std::endl;
	std::wcout << L"\t Dynamic Charged: " << ptokstatistics->DynamicCharged << std::endl;
	std::wcout << L"\t Dynamic Available: " << ptokstatistics->DynamicAvailable << std::endl;
	std::wcout << L"\t ModifiedId: " << std::hex << std::uppercase << ptokstatistics->ModifiedId.HighPart << L":" << ptokstatistics->ModifiedId.LowPart << std::dec << std::nouppercase << std::endl;

	VirtualFree(ptokstatistics, 0x0, MEM_RELEASE);
	ptokstatistics = nullptr;
}
