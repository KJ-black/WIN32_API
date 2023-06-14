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
	PrintSID(ptokuser->User.Sid);
	
	// Clean up
	VirtualFree(ptokuser, 0x0, MEM_RELEASE);
	ptokuser = nullptr;
}