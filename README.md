# WIN32_API

## Authentication

### Winlogon
- [credentialPrompt](https://learn.microsoft.com/en-us/windows/win32/secbp/asking-the-user-for-credentials)
	- Purpose: Create a simple credential prompt
	- API:
		- CredUIPromptForCredentials (credui.lib)

## Authorization

### Access Token
- [searchingSID](https://learn.microsoft.com/en-us/windows/win32/secauthz/searching-for-a-sid-in-an-access-token-in-c--)
	- Purpose: Identifing whether the SID of the token is the well-known SID of the administrator group and whether it is enabled.
	- API
		- OpenProcessToken
		- GetTokenInformation 
		- AllocateAndInitializeSid
		- EqualSid
		- LookupAccountSid

- TokenEnumeration
	- Purpose: Enumerate all information in the access token. 

### Privilege
- shutdown
	- Purpose: Enable the privilege `SeShutdownPrivilege` ( if already have it but disable ) and shut down the host. 
	- API
		- GetProcAddress
		- RtlAdjustPrivilege(advapi32.lib)
		- ZwShutdownSystem
	