# WIN32_API

## Authentication

- [runas](https://kj-black.notion.site/runas-ae4c587290d6411eb9133b69930b19a9?pvs=4)
    - Purpose: Reproduce the Microsfot built-in tools [Runas](https://learn.microsoft.com/en-us/previous-versions/windows/it-pro/windows-server-2012-r2-and-2012/cc771525(v=ws.11))
    - API:
        - CreateProcessWithLogonW

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

- [TokenEnumeration](https://www.notion.so/kj-black/TokenEnumeration-with-WIN32-API-de4cf04b4ec84bbe8e176ee6e202f9c9)
	- Purpose: Enumerate all information in the access token. 

### Privilege
- shutdown
	- Purpose: Enable the privilege `SeShutdownPrivilege` ( if already have it but disable ) and shut down the host. 
	- API
		- GetProcAddress
		- RtlAdjustPrivilege(advapi32.lib)
		- ZwShutdownSystem
	
