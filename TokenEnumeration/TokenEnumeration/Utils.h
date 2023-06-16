#pragma once
#include <Windows.h>
#include <sddl.h>

// Output Chinese
#include <io.h>
#include <fcntl.h>
DWORD mode = _setmode(_fileno(stdout), _O_U16TEXT); 


LPCWSTR getSidNameUse(SID_NAME_USE sidnameuse) {
	switch (sidnameuse) {
	case SidTypeUser:
		return L"User";
	case SidTypeGroup:
		return L"Group";
	case SidTypeAlias:
		return L"Alias";
	case SidTypeWellKnownGroup:
		return L"Well Known Group";
	case SidTypeDeletedAccount:
		return L"Deleted Account";
	case SidTypeInvalid:
		return L"Invalid";
	case SidTypeUnknown:
		return L"Unknown";
	case SidTypeComputer:
		return L"Computer";
	case SidTypeLabel:
		return L"Label";
	case SidTypeLogonSession:
		return L"Logon Session";
	}
}

DWORD GetTokenInfoLength(HANDLE hTok, TOKEN_INFORMATION_CLASS tokClass) {
	DWORD dwRetLength = 0x0;

	GetTokenInformation(hTok, tokClass, NULL, 0x0, &dwRetLength);

	return dwRetLength;
}

LPWSTR SIDSerialize(PSID psid) {
	LPWSTR lpsid;
	if (!ConvertSidToStringSidW(psid, &lpsid)) {
		PrintError(L"ConvertSidToStringSidW()");
		return nullptr;
	}
	return lpsid;
}

BOOL GetSIDName(PSID psid, LPWSTR lpName, LPWSTR lpDomainName, PSID_NAME_USE psidnameuse) {
	DWORD dwName, dwDomainName;
	dwName = dwDomainName = 512;
	if (!LookupAccountSidW(nullptr, psid, lpName, &dwName, lpDomainName, &dwDomainName, psidnameuse)){
		PrintError(L"LookupAccountSidW()");
		VirtualFree(lpName, 0x0, MEM_RELEASE);
		VirtualFree(lpDomainName, 0x0, MEM_RELEASE);
		lpName = lpDomainName = nullptr;
		return false;
	}
	return true;
}

VOID PrintSID(PSID pSID) {
	// Convert Sid to StingSid which sutable for dispaly
	LPWSTR lpsid = SIDSerialize(pSID);
	if (!lpsid) {
		PrintError(L"SIDSerizlize()");
	}
	else {
		std::wcout << L"\t     SID: " << lpsid << std::endl;
	}
	LocalFree(lpsid);
	lpsid = nullptr;

	// Look up the account name associated with the SID
	LPWSTR lpName, lpDomainName;
	PSID_NAME_USE psidnameuse;
	lpName = (LPWSTR)VirtualAlloc(NULL, 126, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	lpDomainName = (LPWSTR)VirtualAlloc(NULL, 126, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	psidnameuse = (PSID_NAME_USE)malloc(sizeof(PSID_NAME_USE));
	if (GetSIDName(pSID, lpName, lpDomainName, psidnameuse)) {
		std::wcout << L"\t     Domain\\Account (Type): " << lpDomainName << L"\\" << lpName << L" (" << getSidNameUse(*psidnameuse) << L")" << std::endl;
		VirtualFree(lpName, 0x0, MEM_RELEASE);
		VirtualFree(lpDomainName, 0x0, MEM_RELEASE);
		lpName = lpDomainName = nullptr;
	}
}