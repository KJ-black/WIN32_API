#pragma once
#include <windows.h>
#include <errhandlingapi.h>
#include <system_error>
#include <iostream>

VOID PrintError(LPCWSTR lpFunctionName, BOOL bExit = false) {

    // Get the latest error id 
    DWORD dwErrId = GetLastError();
    std::wcout << L"[ERR:" << dwErrId << L"]" << lpFunctionName << L": ";

    if (dwErrId == 0) {
        std::wcout << L"Something went wrong";
    }
    else {
        std::cout << std::system_category().message(dwErrId);
    }

    std::wcout << std::endl;
    if (bExit)
        exit(1);
}