#include "pch.h"

INT wmain(DWORD argc, LPWSTR argv[]) {

    if (argc < 3) {
        std::wcout << L"runas [{/profile | /noprofile}] [/env] [{/netonly | /savecred}] [/smartcard] [/showtrustlevels] [/trustlevel] /user:<UserAccountName> \"<ProgramName>\"" << std::endl;
        return 1;
    }

    // switch handling
    std::wstring wsUserOption = argv[argc - 2];
    std::wstring wsProgramOption = argv[argc - 1];
    BOOL bProfile = false;
    BOOL bNoprofile = false;
    BOOL bEnv = false;
    BOOL bNetonly = false;
    BOOL bSavecred = false;
    BOOL bSmartcard = false;
    BOOL bShowtrustlevels = false;
    std::wstring trustlevel;

    // Parsing the options
    for (int i = 2; i < argc - 2; ++i) {
        std::wstring option = argv[i];
        if (option == L"/profile") {
            bProfile = true;
        }
        else if (option == L"/noprofile") {
            bNoprofile = true;
        }
        else if (option == L"/env") {
            bEnv = true;
        }
        else if (option == L"/netonly") {
            bNetonly = true;
        }
        else if (option == L"/savecred") {
            bSavecred = true;
        }
        else if (option == L"/smartcard") {
            bSmartcard = true;
        }
        else if (option == L"/showtrustlevels") {
            bShowtrustlevels = true;
        }
        else if (option.find(L"/trustlevel:") == 0) {
            // Extracting trust level if specified
            trustlevel = option.substr(12);
        }
        else {
            std::wcout << L"Invalid option: " << option << std::endl;
            return 1;
        }
    }

    std::wstring wsUserFull = wsUserOption.substr(wsUserOption.find(L":") + 1, wsUserOption.length() - wsUserOption.find(L":") - 1);
    std::wstring wsProgram = wsProgramOption.substr(wsProgramOption.find(L":") + 1, wsProgramOption.length() - wsProgramOption.find(L":") - 1);
    wchar_t wProgramFull[MAX_PATH];
    if (!SearchPathW(NULL, wsProgram.c_str(), NULL, MAX_PATH, wProgramFull, NULL)) {
        PrintError(L"SearchPathW()");
    }

    // Enter Password
    std::wcout << L"Enter the password for " << wsUserFull << L":";
    std::wstring wsPassword = getPassword();

    // LogonUserW
    STARTUPINFO si = { 0 };
    PROCESS_INFORMATION pi = { 0 };
    LPVOID lpEnvironment = NULL;
    si.cb = sizeof(STARTUPINFO);
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.hStdError = si.hStdOutput;
    si.dwFlags |= STARTF_USESTDHANDLES;
    std::wstring wsUsername;
    std::wstring wsDomain;
    if (wsUserFull.find(L"\\") != std::string::npos) {
        getUser(wsUserFull, wsUsername, wsDomain);
        if (!CreateProcessWithLogonW(wsUsername.c_str(), wsDomain.c_str(), wsPassword.c_str(), 0, wProgramFull, nullptr, NULL, lpEnvironment, nullptr, &si, &pi)) {
            PrintError(L"CreateProcessWithLogonW()", false);
        }
    }
    else {
        if (!CreateProcessWithLogonW(wsUserFull.c_str(), NULL, wsPassword.c_str(), 0, wProgramFull, nullptr, NULL, lpEnvironment, nullptr, &si, &pi)) {
            PrintError(L"CreateProcessWithLogonW()", false);
        }
    }

    if (DEBUG) {
        std::wcout << L"wsUserFull: " << wsUserFull.c_str() << std::endl;
        std::wcout << L"wsUsername: " << wsUsername.c_str() << std::endl;
        std::wcout << L"wsDomain: " << wsDomain.c_str() << std::endl;
        std::wcout << L"wsPassword: " << wsPassword.c_str() << std::endl;
    }
    
    SecureZeroMemory(&wsPassword, sizeof(wsPassword));
    
    /*
    HANDLE hDuplicate;
    if (!DuplicateTokenEx(hTok, TOKEN_ALL_ACCESS, NULL, SecurityImpersonation, TokenPrimary, &hDuplicate)) {
        PrintError(L"DuplicateTokenEx");
    }  */
}