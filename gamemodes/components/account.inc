
// Include guard (versi simpel)
#if defined _INC_components_account
	#endinput
#endif

#define _INC_components_account

// Module configuration
#define MAX_LOGIN_TIMEOUT			60 // in seconds
#define MAX_WRONG_PASSWORD          3
#define MINIMUM_REQUIRED_PASSWORD	8
#define ACCLIB_AUTO_KICK_ON_ERROR
#define INVALID_ACCOUNT_ID   		-1

enum ACCOUNT_PROMPT_TYPE
{
	ACCOUNT_LOGIN,
	ACCOUNT_REGISTER
}

// Include Module
#include <account-lib>
#include <YSI_Coding\y_inline>
#include <YSI_Coding\y_timers>
#include <YSI_Visual\y_dialog>

#include <components\character>

// Function or Callback
forward bool:AccLib_LogoutPlayer(playerid, refetch = false);
forward AccLib_GetAccountId(playerid);
forward bool:AccLib_GetAccountName(playerid, string:name[], len = sizeof(name));
forward bool:AccLib_SetAccountName(playerid, const string:name[]);
forward bool:AccLib_SetAccountPassword(playerid, const string:password[]);

// Internal Module
#include <account\account_entry>
