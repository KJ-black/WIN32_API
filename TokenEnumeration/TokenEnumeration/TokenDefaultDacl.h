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
		PSID pSID;
		DWORD accessMask;
        if (GetAce(ptokdacl->DefaultDacl, i, &pACE)) {
            ACE_HEADER* pAceHeader = reinterpret_cast<ACE_HEADER*>(pACE);
			switch (pAceHeader->AceType) {
			case ACCESS_ALLOWED_ACE_TYPE: {
				std::wcout << "Type: ACCESS_ALLOWED_ACE_TYPE" << std::endl;
				ACCESS_ALLOWED_ACE* pAccessAce0 = reinterpret_cast<ACCESS_ALLOWED_ACE*>(pACE);
				accessMask = pAccessAce0->Mask;
				pSID = reinterpret_cast<PSID>(&pAccessAce0->SidStart);
				PrintSID(pSID);
				std::wcout << L"\t     Mask: " << std::hex << accessMask << std::endl;
				checkAccessMask(accessMask);
				break;
			}
			case ACCESS_ALLOWED_CALLBACK_ACE_TYPE: {
				std::wcout << "Type: ACCESS_ALLOWED_CALLBACK_ACE_TYPE" << std::endl;
				ACCESS_ALLOWED_CALLBACK_ACE* pAccessAce9 = reinterpret_cast<ACCESS_ALLOWED_CALLBACK_ACE*>(pACE);
				accessMask = pAccessAce9->Mask;
				pSID = reinterpret_cast<PSID>(&pAccessAce9->SidStart);
				PrintSID(pSID);
				std::wcout << L"\t     Mask: " << std::hex << accessMask << std::endl;
				checkAccessMask(accessMask);
				break;
			}
			case ACCESS_ALLOWED_CALLBACK_OBJECT_ACE_TYPE: {
				std::wcout << "Type: ACCESS_ALLOWED_CALLBACK_OBJECT_ACE_TYPE" << std::endl;
				ACCESS_ALLOWED_CALLBACK_OBJECT_ACE* pAccessAceB = reinterpret_cast<ACCESS_ALLOWED_CALLBACK_OBJECT_ACE*>(pACE);
				accessMask = pAccessAceB->Mask;
				pSID = reinterpret_cast<PSID>(&pAccessAceB->SidStart);
				PrintSID(pSID);
				std::wcout << L"\t     Mask: " << std::hex << accessMask << std::endl;
				checkAccessMask(accessMask);
				break;
			}
			case ACCESS_ALLOWED_COMPOUND_ACE_TYPE: {
				std::wcout << "Type: ACCESS_ALLOWED_COMPOUND_ACE_TYPE" << std::endl;
				break;
			}
			case ACCESS_ALLOWED_OBJECT_ACE_TYPE: {
				std::wcout << "Type: ACCESS_ALLOWED_OBJECT_ACE_TYPE" << std::endl;
				ACCESS_ALLOWED_OBJECT_ACE* pAccessAce5 = reinterpret_cast<ACCESS_ALLOWED_OBJECT_ACE*>(pACE);
				accessMask = pAccessAce5->Mask;
				pSID = reinterpret_cast<PSID>(&pAccessAce5->SidStart);
				PrintSID(pSID);
				std::wcout << L"\t     Mask: " << std::hex << accessMask << std::endl;
				checkAccessMask(accessMask);
				break;
			}
			case ACCESS_DENIED_ACE_TYPE: {
				std::wcout << "Type: ACCESS_DENIED_ACE_TYPE" << std::endl;
				ACCESS_DENIED_ACE* pAccessAce1 = reinterpret_cast<ACCESS_DENIED_ACE*>(pACE);
				accessMask = pAccessAce1->Mask;
				pSID = reinterpret_cast<PSID>(&pAccessAce1->SidStart);
				PrintSID(pSID);
				std::wcout << L"\t     Mask: " << std::hex << accessMask << std::endl;
				checkAccessMask(accessMask);
				break;
			}
			case ACCESS_DENIED_CALLBACK_ACE_TYPE: {
				std::wcout << "Type: ACCESS_DENIED_CALLBACK_ACE_TYPE" << std::endl;
				ACCESS_DENIED_CALLBACK_ACE* pAccessAceA = reinterpret_cast<ACCESS_DENIED_CALLBACK_ACE*>(pACE);
				accessMask = pAccessAceA->Mask;
				pSID = reinterpret_cast<PSID>(&pAccessAceA->SidStart);
				PrintSID(pSID);
				std::wcout << L"\t     Mask: " << std::hex << accessMask << std::endl;
				checkAccessMask(accessMask);
				break;
			}
			case ACCESS_DENIED_CALLBACK_OBJECT_ACE_TYPE: {
				std::wcout << "Type: ACCESS_DENIED_CALLBACK_OBJECT_ACE_TYPE" << std::endl;
				ACCESS_DENIED_CALLBACK_OBJECT_ACE* pAccessAceC = reinterpret_cast<ACCESS_DENIED_CALLBACK_OBJECT_ACE*>(pACE);
				accessMask = pAccessAceC->Mask;
				pSID = reinterpret_cast<PSID>(&pAccessAceC->SidStart);
				PrintSID(pSID);
				std::wcout << L"\t     Mask: " << std::hex << accessMask << std::endl;
				checkAccessMask(accessMask);
				break;
			}
			case ACCESS_DENIED_OBJECT_ACE_TYPE: {
				std::wcout << "Type: ACCESS_DENIED_OBJECT_ACE_TYPE" << std::endl;
				ACCESS_DENIED_OBJECT_ACE* pAccessAce6 = reinterpret_cast<ACCESS_DENIED_OBJECT_ACE*>(pACE);
				accessMask = pAccessAce6->Mask;
				pSID = reinterpret_cast<PSID>(&pAccessAce6->SidStart);
				PrintSID(pSID);
				std::wcout << L"\t     Mask: " << std::hex << accessMask << std::endl;
				checkAccessMask(accessMask);
				break;
			}
			case ACCESS_MAX_MS_ACE_TYPE: { // SYSTEM_ALARM_OBJECT_ACE_TYPE
				std::wcout << "Type: ACCESS_MAX_MS_ACE_TYPE" << std::endl;
				SYSTEM_ALARM_OBJECT_ACE* pAccessAce8 = reinterpret_cast<SYSTEM_ALARM_OBJECT_ACE*>(pACE);
				accessMask = pAccessAce8->Mask;
				pSID = reinterpret_cast<PSID>(&pAccessAce8->SidStart);
				PrintSID(pSID);
				std::wcout << L"\t     Mask: " << std::hex << accessMask << std::endl;
				checkAccessMask(accessMask);
				break;
			}
			case ACCESS_MAX_MS_V2_ACE_TYPE: { // SYSTEM_ALARM_ACE_TYPE
				std::wcout << "Type: ACCESS_MAX_MS_V2_ACE_TYPE" << std::endl;
				SYSTEM_ALARM_ACE* pAccessAce3 = reinterpret_cast<SYSTEM_ALARM_ACE*>(pACE);
				accessMask = pAccessAce3->Mask;
				pSID = reinterpret_cast<PSID>(&pAccessAce3->SidStart);
				PrintSID(pSID);
				std::wcout << L"\t     Mask: " << std::hex << accessMask << std::endl;
				checkAccessMask(accessMask);
				break;
			}
			case SYSTEM_ALARM_CALLBACK_ACE_TYPE: {
				std::wcout << "Type: SYSTEM_ALARM_CALLBACK_ACE_TYPE" << std::endl;
				SYSTEM_ALARM_CALLBACK_ACE* pAccessAceE = reinterpret_cast<SYSTEM_ALARM_CALLBACK_ACE*>(pACE);
				accessMask = pAccessAceE->Mask;
				pSID = reinterpret_cast<PSID>(&pAccessAceE->SidStart);
				PrintSID(pSID);
				std::wcout << L"\t     Mask: " << std::hex << accessMask << std::endl;
				checkAccessMask(accessMask);
				break;
			}
			case SYSTEM_ALARM_CALLBACK_OBJECT_ACE_TYPE: {
				std::wcout << "Type: SYSTEM_ALARM_CALLBACK_OBJECT_ACE_TYPE" << std::endl;
				SYSTEM_ALARM_CALLBACK_OBJECT_ACE* pAccessAce10 = reinterpret_cast<SYSTEM_ALARM_CALLBACK_OBJECT_ACE*>(pACE);
				accessMask = pAccessAce10->Mask;
				pSID = reinterpret_cast<PSID>(&pAccessAce10->SidStart);
				PrintSID(pSID);
				std::wcout << L"\t     Mask: " << std::hex << accessMask << std::endl;
				checkAccessMask(accessMask);
				break;
			}
			case SYSTEM_AUDIT_ACE_TYPE: {
				std::wcout << "Type: SYSTEM_AUDIT_ACE_TYPE" << std::endl;
				SYSTEM_AUDIT_ACE* pAccessAce2 = reinterpret_cast<SYSTEM_AUDIT_ACE*>(pACE);
				accessMask = pAccessAce2->Mask;
				pSID = reinterpret_cast<PSID>(&pAccessAce2->SidStart);
				PrintSID(pSID);
				std::wcout << L"\t     Mask: " << std::hex << accessMask << std::endl;
				checkAccessMask(accessMask);
				break;			
			}
			case SYSTEM_AUDIT_CALLBACK_ACE_TYPE: {
				std::wcout << "Type: SYSTEM_AUDIT_CALLBACK_ACE_TYPE" << std::endl;
				SYSTEM_AUDIT_CALLBACK_ACE* pAccessAceD = reinterpret_cast<SYSTEM_AUDIT_CALLBACK_ACE*>(pACE);
				accessMask = pAccessAceD->Mask;
				pSID = reinterpret_cast<PSID>(&pAccessAceD->SidStart);
				PrintSID(pSID);
				std::wcout << L"\t     Mask: " << std::hex << accessMask << std::endl;
				checkAccessMask(accessMask);
				break;				
			}
			case SYSTEM_AUDIT_CALLBACK_OBJECT_ACE_TYPE: {
				std::wcout << "Type: SYSTEM_AUDIT_CALLBACK_OBJECT_ACE_TYPE" << std::endl;
				SYSTEM_AUDIT_CALLBACK_OBJECT_ACE* pAccessAceF = reinterpret_cast<SYSTEM_AUDIT_CALLBACK_OBJECT_ACE*>(pACE);
				accessMask = pAccessAceF->Mask;
				pSID = reinterpret_cast<PSID>(&pAccessAceF->SidStart);
				PrintSID(pSID);
				std::wcout << L"\t     Mask: " << std::hex << accessMask << std::endl;
				checkAccessMask(accessMask);
				break;
			}
			case SYSTEM_AUDIT_OBJECT_ACE_TYPE: {
				std::wcout << "Type: SYSTEM_AUDIT_OBJECT_ACE_TYPE" << std::endl;
				SYSTEM_AUDIT_OBJECT_ACE* pAccessAce7 = reinterpret_cast<SYSTEM_AUDIT_OBJECT_ACE*>(pACE);
				accessMask = pAccessAce7->Mask;
				pSID = reinterpret_cast<PSID>(&pAccessAce7->SidStart);
				PrintSID(pSID);
				std::wcout << L"\t     Mask: " << std::hex << accessMask << std::endl;
				checkAccessMask(accessMask);
				break;
			}
			case SYSTEM_MANDATORY_LABEL_ACE_TYPE: {
				std::wcout << "Type: SYSTEM_MANDATORY_LABEL_ACE_TYPE" << std::endl;
				SYSTEM_MANDATORY_LABEL_ACE* pAccessAce11 = reinterpret_cast<SYSTEM_MANDATORY_LABEL_ACE*>(pACE);
				accessMask = pAccessAce11->Mask;
				pSID = reinterpret_cast<PSID>(&pAccessAce11->SidStart);
				PrintSID(pSID);
				std::wcout << L"\t     Mask: " << std::hex << accessMask << std::endl;
				checkAccessMask(accessMask);
				break;
			}
			}			
        }
	}
}