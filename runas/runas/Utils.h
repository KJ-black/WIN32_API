#pragma once
#include <conio.h>
#pragma comment(lib, "advapi32.lib")

BOOL SetPrivilege(
    HANDLE hToken,          // access token handle
    LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
    BOOL bEnablePrivilege   // to enable or disable privilege
)
{
    TOKEN_PRIVILEGES tp;
    LUID luid;

    if (!LookupPrivilegeValue(
        NULL,            // lookup privilege on local system
        lpszPrivilege,   // privilege to lookup 
        &luid))        // receives LUID of privilege
    {
        printf("LookupPrivilegeValue error: %u\n", GetLastError());
        return FALSE;
    }

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    if (bEnablePrivilege)
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    else
        tp.Privileges[0].Attributes = 0;

    // Enable the privilege or disable all privileges.

    if (!AdjustTokenPrivileges(
        hToken,
        FALSE,
        &tp,
        sizeof(TOKEN_PRIVILEGES),
        (PTOKEN_PRIVILEGES)NULL,
        (PDWORD)NULL))
    {
        printf("AdjustTokenPrivileges error: %u\n", GetLastError());
        return FALSE;
    }

    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)

    {
        printf("The token does not have the specified privilege. \n");
        return FALSE;
    }

    return TRUE;
}

DWORD GetTokenInfoLength(HANDLE hTok, TOKEN_INFORMATION_CLASS tokClass) {
    DWORD dwRetLength = 0x0;

    GetTokenInformation(hTok, tokClass, NULL, 0x0, &dwRetLength);

    return dwRetLength;
}


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

// TASK
// parse the following format
// "/user:<ComputerName>\<AccountName>"
VOID getUser(const std::wstring& wsUserOption, std::wstring& wsUsername, std::wstring& wsDomain) {
    if (wsUserOption.find(L"\\") != std::string::npos) {
        DWORD dDomainEnd = wsUserOption.find(L"\\");
        wsDomain = wsUserOption.substr(0, dDomainEnd);
        
        DWORD dUsernameStart = wsUserOption.find(L"\\") + 1;
        DWORD dUsernameEnd = wsUserOption.length();
        wsUsername = wsUserOption.substr(dUsernameStart, dUsernameEnd - dUsernameStart);
    }
    else {
        std::wcout << L"ERROR: username wrong format" << std::endl;
        exit(1);
    }
}

std::wstring getPassword()
{
    std::wstring password;
    char ch;

    // Turn off console echo
    DWORD mode, count;
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(hInput, &mode);
    SetConsoleMode(hInput, mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));

    // Read password character by character
    while ((ch = _getch()) != '\r')
    {
        if (ch == '\b')
        {
            if (!password.empty())
            {
                std::cout << "\b \b";
                password.pop_back();
            }
        }
        else
        {
            password += ch;
            //std::cout << '*';
        }
    }

    // Restore console mode
    SetConsoleMode(hInput, mode);
    std::cout << std::endl;

    return password;
}
