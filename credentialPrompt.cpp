#include <windows.h>
#include <wincred.h>
#include <tchar.h>
#include <iostream>
#pragma comment(lib, "credui.lib")
using namespace std; 

int main()
{
    CREDUI_INFO cui;
    TCHAR pszName[CREDUI_MAX_USERNAME_LENGTH + 1];
    TCHAR pszPwd[CREDUI_MAX_PASSWORD_LENGTH + 1];
    BOOL fSave;
    DWORD dwErr;

    cui.cbSize = sizeof(CREDUI_INFO);
    cui.hwndParent = NULL;
    cui.pszMessageText = TEXT("Enter administrator account information");
    cui.pszCaptionText = TEXT("CredUITest");
    cui.hbmBanner = NULL;
    fSave = FALSE;

    SecureZeroMemory(pszName, sizeof(pszName));
    SecureZeroMemory(pszPwd, sizeof(pszPwd));

    dwErr = CredUIPromptForCredentials(
        &cui,                         // CREDUI_INFO structure
        TEXT("TheServer"),            // Target for credentials (usually a server)
        NULL,                         // Reserved
        0,                            // Reason
        pszName,                      // User name
        CREDUI_MAX_USERNAME_LENGTH + 1, // Max number of chars for user name
        pszPwd,                       // Password
        CREDUI_MAX_PASSWORD_LENGTH + 1, // Max number of chars for password
        &fSave,                       // State of save check box
        CREDUI_FLAGS_GENERIC_CREDENTIALS |  // Flags
        CREDUI_FLAGS_ALWAYS_SHOW_UI |
        CREDUI_FLAGS_DO_NOT_PERSIST);

    if (!dwErr)
    {
        // Code that uses the credentials can be added here.
		cout << "Username: " << pszName << endl;
		cout << "Password: " << pszPwd << endl;

        // When you have finished using the credentials,
        // erase them from memory.
        SecureZeroMemory(pszName, sizeof(pszName));
        SecureZeroMemory(pszPwd, sizeof(pszPwd));
    }
	
    return 0;
}
