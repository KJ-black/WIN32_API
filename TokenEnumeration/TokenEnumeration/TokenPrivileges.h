#pragma once
#include "Utils.h"
#include "ErrorHandling.h"
#include <iomanip>

VOID PrintTokenPrivileges(HANDLE hTok) {
	DWORD dwTokLen = GetTokenInfoLength(hTok, TokenPrivileges);
	DWORD dwRetLen;

	PTOKEN_PRIVILEGES ptokpriv = (PTOKEN_PRIVILEGES)malloc(dwTokLen); // (NULL, dwTokLen, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	if (!GetTokenInformation(hTok, TokenPrivileges, (LPVOID)ptokpriv, dwTokLen, &dwRetLen)) {
		PrintError(L"GetTokenInformation()");
		return;
	}

	std::wcout << L"[+] Token Privileges: " << std::endl;

	// Iterate each privileges to print out their names, details and status
	for (DWORD i = 0; i < ptokpriv->PrivilegeCount; i++) {
		DWORD dwName, dwLangId, dwDisplayName;
		dwName = dwDisplayName = 1000;
		LPWSTR lpName = (LPWSTR)VirtualAlloc(NULL, dwName, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		LPWSTR lpDisplayName = (LPWSTR)VirtualAlloc(NULL, dwDisplayName, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		// Align 
		std::wcout << L"\t[" << std::setw(2) << i+1 << L"] ";

		// Get the name of the privilege from LUID
		if (!LookupPrivilegeNameW(NULL, &ptokpriv->Privileges[i].Luid, lpName, &dwName)) {
			PrintError(L"LookupPrivilegeNameW()");
			continue;
		}
		std::wcout << L"Name: " << lpName << std::endl;

		// Get the description for the privilege by its name
		if (!LookupPrivilegeDisplayNameW(NULL, lpName, lpDisplayName, &dwDisplayName, &dwLangId)) {
			PrintError(L"LookupPrivilegeNameW()");
			continue;
		}
		std::wcout << L"\t     Description: " << lpDisplayName << std::endl;

		// Get the status of the privilege from Attributes
		switch (ptokpriv->Privileges[i].Attributes) {
		case SE_PRIVILEGE_ENABLED:
			std::wcout << L"\t     Status: Enabled\n";
			break;
		case SE_PRIVILEGE_ENABLED_BY_DEFAULT:
			std::wcout << L"\t     Status: Enabled by Default\n";
			break;
		case SE_PRIVILEGE_ENABLED | SE_PRIVILEGE_ENABLED_BY_DEFAULT:
			std::wcout << L"\t     Status: Enabled by Default\n";
			break;
		case SE_PRIVILEGE_REMOVED:
			std::wcout << L"\t     Status: Removed\n";
			break;
		case SE_PRIVILEGE_USED_FOR_ACCESS:
			std::wcout << L"\t     Status: Used for Access\n";
			break;
		case 0x0:
			std::wcout << L"\t     Status: Disabled\n";
			break;
		default:
			std::wcout << L"\t     Status: N/A\n";
		}

		VirtualFree(lpName, 0x0, MEM_RELEASE);
		VirtualFree(lpDisplayName, 0x0, MEM_RELEASE);
		lpName = nullptr;
		lpDisplayName = nullptr;
	}

	VirtualFree(ptokpriv, 0x0, MEM_RELEASE);
	ptokpriv = nullptr;
}