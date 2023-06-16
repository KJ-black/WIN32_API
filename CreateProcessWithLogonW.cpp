#include <windows.h>
#include <stdio.h>
#include <userenv.h>

void DisplayError(LPCWSTR pszAPI)
{
    LPVOID lpvMessageBuffer;

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, GetLastError(), 
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
        (LPSTR)&lpvMessageBuffer, 0, NULL);

    //
    //... now display this string
    //
    wprintf(L"ERROR: API        = %s.\n", pszAPI);
    wprintf(L"       error code = %d.\n", GetLastError());
    wprintf(L"       message    = %s.\n", (LPWSTR)lpvMessageBuffer);

    //
    // Free the buffer allocated by the system
    //
    LocalFree(lpvMessageBuffer);

    ExitProcess(GetLastError());
}

int main(int argc, LPCSTR argv[])
{
    DWORD     dwSize;
    HANDLE    hToken;
    LPVOID    lpvEnv;
    PROCESS_INFORMATION pi = {0};
    STARTUPINFO         si = {0};
    char               szUserProfile[256] = "";

    si.cb = sizeof(STARTUPINFO);
    
    if (argc != 4)
    {
        wprintf(L"Usage: %s [user@domain] [password] [cmd]", argv[0]);
        wprintf(L"\n\n");
        return 0;
    }

    //
    // TO DO: change NULL to '.' to use local account database
    //
    if (!LogonUser(argv[1], (LPCSTR)'.', argv[2], LOGON32_LOGON_INTERACTIVE, 
            LOGON32_PROVIDER_DEFAULT, &hToken))
        DisplayError(L"LogonUser");

    if (!CreateEnvironmentBlock(&lpvEnv, hToken, TRUE))
        DisplayError(L"CreateEnvironmentBlock");

    dwSize = sizeof(szUserProfile)/sizeof(WCHAR);

    if (!GetUserProfileDirectory(hToken, szUserProfile, &dwSize))
        DisplayError(L"GetUserProfileDirectory");

    //
    // TO DO: change NULL to '.' to use local account database
    //
    if (!CreateProcessWithLogonW((wchar_t*)argv[1], NULL, (wchar_t*)argv[2], 
            LOGON_WITH_PROFILE, NULL, (wchar_t*)argv[3], 
            CREATE_UNICODE_ENVIRONMENT, lpvEnv, (wchar_t*)szUserProfile, 
            (_STARTUPINFOW*)&si, &pi))
        DisplayError(L"CreateProcessWithLogonW");

    if (!DestroyEnvironmentBlock(lpvEnv))
        DisplayError(L"DestroyEnvironmentBlock");

    CloseHandle(hToken);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
	return 0;
}
