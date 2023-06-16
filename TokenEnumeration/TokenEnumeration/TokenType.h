#pragma once
#include "Utils.h"
#include "ErrorHandling.h"

LPCWSTR getImpersonationLevel(DWORD value) {
	switch (value) {
	case SecurityAnonymous:
		return L"SecurityAnonymous";
	case SecurityIdentification:
		return L"SecurityIdentification";
	case SecurityImpersonation:
		return L"SecurityImpersonation";
	case SecurityDelegation:
		return L"SecurityDelegation";
	}
}


VOID PrintTokenType(HANDLE hTok) {
	DWORD dwTokLen = GetTokenInfoLength(hTok, TokenType);
	DWORD dwRetLen;

	PTOKEN_TYPE ptoktype = (PTOKEN_TYPE)malloc(dwTokLen);

	if (!GetTokenInformation(hTok, TokenType, (LPVOID)ptoktype, dwTokLen, &dwRetLen)) {
		PrintError(L"GetTokenInformation()");
		return;
	}
	std::wcout << L"[+] Token Type: ";
	switch (*ptoktype) {
	case TokenPrimary:
		std::wcout << L"Primary Token" << std::endl;
		break;
	case TokenImpersonation:
		dwTokLen = GetTokenInfoLength(hTok, TokenImpersonationLevel);
		PSECURITY_IMPERSONATION_LEVEL pimpersonlevel = (PSECURITY_IMPERSONATION_LEVEL)malloc(dwTokLen);
		if (!GetTokenInformation(hTok, TokenImpersonationLevel, (LPVOID)pimpersonlevel, dwTokLen, &dwRetLen)) {
			PrintError(L"GetTokenInformation()");
			return;
		}

		std::wcout << L"Impersonate Token with " << getImpersonationLevel(*pimpersonlevel) << L" level" << std::endl;

		VirtualFree(pimpersonlevel, 0x0, MEM_RELEASE);
		pimpersonlevel = nullptr; 
		break;
	}


	VirtualFree(ptoktype, 0x0, MEM_RELEASE);
	ptoktype = nullptr;
}