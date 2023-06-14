#pragma once
#include "ErrorHandling.h"

#include <Windows.h>
#include <winnt.h>
#include <securitybaseapi.h>

BOOL EnableSeDebugPrivilege() {
    // Get the current process handle
    HANDLE hCurrentProcess = GetCurrentProcess();

    // Get the token handle with query information and adjust privileges access
    HANDLE hTok = INVALID_HANDLE_VALUE;
    if (!OpenProcessToken(hCurrentProcess, TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &hTok)) {
        PrintError(L"OpenProcessToken()");
        return false;
    }
    else if (hTok == NULL || hTok == INVALID_HANDLE_VALUE) {
        PrintError(L"OpenProcessToken()");
        return false;
    }

    // Get the value of SeDebugPrivilege from text
    LUID pDebugPriv;
    if (!LookupPrivilegeValueA(nullptr, "SeDebugPrivilege", &pDebugPriv)) {
        PrintError(L"LookupPrivilegeValueA");
        return false;
    }

    // Adjust token privilege
    TOKEN_PRIVILEGES tokPrivs;
    tokPrivs.PrivilegeCount = 1;
    tokPrivs.Privileges[0].Luid = pDebugPriv;
    tokPrivs.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    if (!AdjustTokenPrivileges(hTok, false, &tokPrivs, NULL, nullptr, nullptr)) {
        PrintError(L"AdjustTokenPrivileges()");
        return false;
    }

    // Query token privileges to confirm 
    BOOL bResult;
    PRIVILEGE_SET privSet;
    privSet.PrivilegeCount = 1;
    privSet.Control = PRIVILEGE_SET_ALL_NECESSARY;
    privSet.Privilege[0].Luid = pDebugPriv;
    if (!PrivilegeCheck(hTok, &privSet, &bResult)) {
        PrintError(L"PrivilegeCheck()");
        return false;
    }

    // Clean up 
    CloseHandle(hCurrentProcess);
    CloseHandle(hTok);
    hCurrentProcess = nullptr;
    hTok = nullptr;

    return bResult;
}

VOID SpawnElevatedProcess() {
    // Get current process image file path
    CHAR lpFileName[MAX_PATH];
    if (!GetModuleFileNameA(nullptr, lpFileName, MAX_PATH)) {
        PrintError(L"GetModuleFileNameA()", true);
    }

    // Craft the file execution information for ShellExecuteExA
    SHELLEXECUTEINFOA si;
    si.cbSize = sizeof(SHELLEXECUTEINFOA);
    si.fMask = SEE_MASK_DEFAULT;
    si.hwnd = nullptr;
    si.lpVerb = "runas";
    si.lpFile = lpFileName;
    si.lpParameters = nullptr;
    si.lpDirectory = nullptr;
    si.nShow = SW_NORMAL;

    // Start the process with elevated UAC
    if (!ShellExecuteExA(&si)) {
        PrintError(L"ShellExecuteExA()", true);
    }

    if (!ShellExecuteA)

        // Exit the current process as it is no longer needed
        exit(1);
}