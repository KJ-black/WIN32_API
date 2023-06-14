#pragma once
#include <tlhelp32.h>


VOID PrintBasicProcessDetails(DWORD dwPID) {

    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (!hSnapShot) {
        PrintError(L"CreateToolhelp32Snapshot()");
        return;
    }

    PROCESSENTRY32 peProcessEntry;
    peProcessEntry.dwSize = sizeof(PROCESSENTRY32);

    DWORD dwPPID, dwThreads;
    PWCHAR szExeFile = (PWCHAR)VirtualAlloc(NULL, 256, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!Process32First(hSnapShot, &peProcessEntry)) {
        PrintError(L"Process32First()");
        return;
    }
    else {
        do {
            if (peProcessEntry.th32ProcessID == dwPID) {
                dwPPID = peProcessEntry.th32ParentProcessID;
                dwThreads = peProcessEntry.cntThreads;
                szExeFile = peProcessEntry.szExeFile;
            }
        } while (Process32Next(hSnapShot, &peProcessEntry));
    }

    std::wcout << L"[+] Basic Process Details" << std::endl;
    std::wcout << L"\tProcess ID: " << dwPID << std::endl;
    std::wcout << L"\tParent Process ID: " << dwPPID << std::endl;
    std::wcout << L"\tProcess Name: " << szExeFile << std::endl;
    std::wcout << L"\t# Threads: " << dwThreads << std::endl;
    //Li stProcessThreads(dwPID);
    //ListProcessModules(dwPID);

    // Clean
    VirtualFree(szExeFile, 256, MEM_RELEASE);
    szExeFile = NULL;
    CloseHandle(hSnapShot);
    hSnapShot = NULL;
}
