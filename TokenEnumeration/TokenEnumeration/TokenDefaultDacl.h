#pragma once
#include "Utils.h"
#include "ErrorHandling.h"
#include <iomanip>

VOID checkAccessMask(DWORD mask) {

	if ( (mask & DELETE) == DELETE)
		std::wcout << L"\t           DELETE" << std::endl;
	if ((mask & READ_CONTROL) == READ_CONTROL)
		std::wcout << L"\t          READ_CONTROL" << std::endl;
	if ((mask & WRITE_DAC) == WRITE_DAC)
		std::wcout << L"\t           WRITE_DAC" << std::endl;
	if ((mask & WRITE_OWNER) == WRITE_OWNER)
		std::wcout << L"\t           WRITE_OWNER" << std::endl;
	if ((mask & SYNCHRONIZE) == SYNCHRONIZE)
		std::wcout << L"\t           SYNCHRONIZE" << std::endl;
	if ((mask & STANDARD_RIGHTS_REQUIRED) == STANDARD_RIGHTS_REQUIRED)
		std::wcout << L"\t           STANDARD_RIGHTS_REQUIRED" << std::endl;
	if ((mask & STANDARD_RIGHTS_ALL) == STANDARD_RIGHTS_ALL)
		std::wcout << L"\t           STANDARD_RIGHTS_ALL" << std::endl;
	if ((mask & SPECIFIC_RIGHTS_ALL) == SPECIFIC_RIGHTS_ALL)
		std::wcout << L"\t           SPECIFIC_RIGHTS_ALL" << std::endl;
	if ((mask & GENERIC_ALL) == GENERIC_ALL)
		std::wcout << L"\t           GENERIC_ALL" << std::endl;
	if ((mask & GENERIC_EXECUTE) == GENERIC_EXECUTE)
		std::wcout << L"\t           GENERIC_EXECUTE" << std::endl;
	if ((mask & GENERIC_WRITE) == GENERIC_WRITE)
		std::wcout << L"\t           GENERIC_WRITE" << std::endl;
	if ((mask & GENERIC_READ) == GENERIC_READ)
		std::wcout << L"\t           GENERIC_READ" << std::endl;
}

VOID PrintTokenDefaultDacl(HANDLE hTok) {
	DWORD dwTokLen = GetTokenInfoLength(hTok, TokenDefaultDacl);
	DWORD dwRetLen;

	PTOKEN_DEFAULT_DACL ptokdacl = (PTOKEN_DEFAULT_DACL)malloc(dwTokLen);

	if (!GetTokenInformation(hTok, TokenDefaultDacl, (LPVOID)ptokdacl, dwTokLen, &dwRetLen)) {
		PrintError(L"GetTokenInformation()");
		return;
	}

	std::wcout << L"[+] Token Default DACL" << std::endl;

	for (DWORD i = 0; i < ptokdacl->DefaultDacl->AceCount; i++) {

		std::wcout << L"\t[" << std::setw(2) << i + 1 << L"] ";

        LPVOID pACE;
        if (GetAce(ptokdacl->DefaultDacl, i, &pACE)) {
            ACE_HEADER* pAceHeader = reinterpret_cast<ACE_HEADER*>(pACE);
			switch (pAceHeader->AceType) {
			case ACCESS_ALLOWED_ACE_TYPE:
				std::wcout << "Type: ACCESS_ALLOWED_ACE_TYPE" << std::endl;
				ACCESS_ALLOWED_ACE* pAccessAce = reinterpret_cast<ACCESS_ALLOWED_ACE*>(pACE);
				DWORD accessMask = pAccessAce->Mask;
				PSID pSID = reinterpret_cast<PSID>(&pAccessAce->SidStart);
				std::wcout << L"\t     Mask: " << std::hex << accessMask << std::endl;
				checkAccessMask(accessMask);
				PrintSID(pSID);
				break;
			case ACCESS_ALLOWED_CALLBACK_ACE_TYPE:
				std::wcout << "Type: ACCESS_ALLOWED_CALLBACK_ACE_TYPE" << std::endl;
				ACCESS_ALLOWED_ACE* pAccessAce = reinterpret_cast<ACCESS_ALLOWED_ACE*>(pACE);
				DWORD accessMask = pAccessAce->Mask;
				PSID pSID = reinterpret_cast<PSID>(&pAccessAce->SidStart);
				std::wcout << L"\t     Mask: " << std::hex << accessMask << std::endl;
				checkAccessMask(accessMask);
				PrintSID(pSID);
				break;
			case ACCESS_ALLOWED_CALLBACK_OBJECT_ACE_TYPE:
				return L"ACCESS_ALLOWED_CALLBACK_ACE_TYPE";
			case ACCESS_ALLOWED_COMPOUND_ACE_TYPE:
				return L"ACCESS_ALLOWED_COMPOUND_ACE_TYPE";
			case ACCESS_ALLOWED_OBJECT_ACE_TYPE:
				return L"ACCESS_ALLOWED_OBJECT_ACE_TYPE";
			case ACCESS_DENIED_ACE_TYPE:
				return L"ACCESS_DENIED_ACE_TYPE";
			case ACCESS_DENIED_CALLBACK_ACE_TYPE:
				return L"ACCESS_DENIED_CALLBACK_ACE_TYPE";
			case ACCESS_DENIED_CALLBACK_OBJECT_ACE_TYPE:
				return L"ACCESS_DENIED_CALLBACK_OBJECT_ACE_TYPE";
			case ACCESS_DENIED_OBJECT_ACE_TYPE:
				return L"ACCESS_DENIED_OBJECT_ACE_TYPE";
			case ACCESS_MAX_MS_ACE_TYPE:
				return L"ACCESS_MAX_MS_ACE_TYPE";
			case ACCESS_MAX_MS_V2_ACE_TYPE:
				return L"ACCESS_MAX_MS_V2_ACE_TYPE";
			case SYSTEM_ALARM_CALLBACK_ACE_TYPE:
				return L"SYSTEM_ALARM_CALLBACK_ACE_TYPE";
			case SYSTEM_ALARM_CALLBACK_OBJECT_ACE_TYPE:
				return L"SYSTEM_ALARM_CALLBACK_OBJECT_ACE_TYPE";
			case SYSTEM_AUDIT_ACE_TYPE:
				return L"SYSTEM_AUDIT_ACE_TYPE";
			case SYSTEM_AUDIT_CALLBACK_ACE_TYPE:
				return L"SYSTEM_AUDIT_CALLBACK_ACE_TYPE";
			case SYSTEM_AUDIT_CALLBACK_OBJECT_ACE_TYPE:
				return L"SYSTEM_AUDIT_CALLBACK_OBJECT_ACE_TYPE";
			case SYSTEM_AUDIT_OBJECT_ACE_TYPE:
				return L"SYSTEM_AUDIT_OBJECT_ACE_TYPE";
			case SYSTEM_MANDATORY_LABEL_ACE_TYPE:
				return L"SYSTEM_MANDATORY_LABEL_ACE_TYPE";
			}

			// Print ACE Content
			
        }
	}
}