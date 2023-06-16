#include "pch.h"

INT wmain(int argc, LPWSTR argv[]) {

    if (argc < 2) {
        std::wcout << L"Usage: " << argv[0] << L" <PID>" << std::endl;
        return 1;
    }

    DWORD dwPID = _wtol(argv[1]);
    
    HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION, false, dwPID);
    if (hProc == NULL || hProc == INVALID_HANDLE_VALUE) {
        hProc = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, dwPID);
        if (hProc == NULL || hProc == INVALID_HANDLE_VALUE) {
            PrintError(L"OpenProcess()", false);
        }
        else {
            std::wcout << L"[+] OpenProcess() with PROCESS_QUERY_LIMITED_INFORMATION access right" << std::endl;
        }
    }
    else {
        std::wcout << L"[+] OpenProcess() with PROCESS_QUERY_INFORMATION access right" << std::endl;
    }

    if (EnableSeDebugPrivilege()){
        std::wcout << L"[+] EnableSeDebugPrivilege() success!" << std::endl;
    }

    PrintBasicProcessDetails(dwPID);

    // Open the token from the process
    HANDLE hTok;
    if (!OpenProcessToken(hProc, TOKEN_QUERY | TOKEN_QUERY_SOURCE, &hTok)) {
        PrintError(L"OpenProcessToken()", TRUE);
    }

    // Print each of the token information class
    // https://learn.microsoft.com/en-us/windows/win32/api/winnt/ne-winnt-token_information_class
    PrintTokenUser(hTok); 
    PrintTokenGroups(hTok); 
    PrintTokenPrivileges(hTok);
    PrintTokenOwner(hTok); 
    PrintTokenPrimaryGroup(hTok);
    PrintTokenDefaultDacl(hTok);
    PrintTokenSource(hTok); 
    PrintTokenType(hTok);
    PrintTokenStatistics(hTok);
    PrintTokenRestrictedSids(hTok);
    PrintTokenSessionId(hTok);
    //PrintTokenGroupsAndPrivileges(hTok);
    //PrintTokenSessionReference(hTok);
    PrintTokenSandBoxInert(hTok);
    //PrintTokenAuditPolicy(hTok);
    PrintTokenOrigin(hTok);
        /*PrintTokenElevationType,
    PrintTokenLinkedToken,
    PrintTokenElevation,
    PrintTokenHasRestrictions,
    PrintTokenAccessInformation,
    PrintTokenVirtualizationAllowed,
    PrintTokenVirtualizationEnabled,
    PrintTokenIntegrityLevel,
    PrintTokenUIAccess,
    PrintTokenMandatoryPolicy,
    PrintTokenLogonSid,
    PrintTokenIsAppContainer,
    PrintTokenCapabilities,
    PrintTokenAppContainerSid,
    PrintTokenAppContainerNumber,
    PrintTokenUserClaimAttributes,
    PrintTokenDeviceClaimAttributes,
    PrintTokenRestrictedUserClaimAttributes,
    PrintTokenRestrictedDeviceClaimAttributes,
    PrintTokenDeviceGroups,
    PrintTokenRestrictedDeviceGroups,
    PrintTokenSecurityAttributes,
    PrintTokenIsRestricted,
    PrintTokenProcessTrustLevel,
    PrintTokenPrivateNameSpace,
    PrintTokenSingletonAttributes,
    PrintTokenBnoIsolation,
    PrintTokenChildProcessFlags,
    PrintTokenIsLessPrivilegedAppContainer,
    PrintTokenIsSandboxed,
    PrintTokenIsAppSilo,
    PrintTokenLoggingInformation,
    PrintMaxTokenInfoClass*/
    return 0;
}

