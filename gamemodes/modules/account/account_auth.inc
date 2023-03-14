
static
	g_sRequestClassPassed[MAX_PLAYERS],
	Timer:g_sPlayerLoginTimer[MAX_PLAYERS],
	g_sPlayerLoginAttempts[MAX_PLAYERS];

PromptDialog(playerid, ACCOUNT_PROMPT_TYPE:type)
{
	new 
		string:name[MAX_PLAYER_NAME + 1];

	GetPlayerName(playerid, name, sizeof(name));

	switch (type)
	{
		case ACCOUNT_LOGIN:
		{
			inline onLoginResponse(response, listitem, string:inputtext[])
			{
				#pragma unused listitem

				if (!response)
				{
					Kick(playerid);
					return 0;
				}

				if (!IsNull(inputtext))
				{
					AccLib_LoginPlayer(playerid, inputtext);
					return 1;
				}

				if (++g_sPlayerLoginAttempts[playerid] >= MAX_WRONG_PASSWORD)
				{
					g_sPlayerLoginAttempts[playerid] = 0;
					Dialog_Show(playerid, DIALOG_STYLE_MSGBOX, "NAMAPROJECTMU - Message Notice", "{FFFFFF}Anda telah memasukkan password yang salah sebanyak 3 kali!", "Okay", "");
					DelayedKick(playerid);
				}
				return 0;
			}
			new string:loginFmt[64];
			format(loginFmt, sizeof(loginFmt), "{FFFFFF}Hallo {00FF00}%s{FFFFFFFF}, selamat datang kembali di NAMAPROJECTMU!\nAnda diberi waktu "#MAX_LOGIN_TIMEOUT" untuk memasukkan password dibawah untuk melanjutkan.", ReturnPlayerName(playerid));
			Dialog_ShowCallback(playerid, using inline onLoginResponse, DIALOG_STYLE_PASSWORD, "NAMAPROJECTMU - Login Screen", loginFmt, "Login", "Abort");
		}

		case ACCOUNT_REGISTER:
		{
			inline dialogRegisterRespond(response, listitem, string:inputtext[])
			{
				// We must use this, if the argument is not used
				#pragma unused listitem

				// Kick player if they pressed "Abort" or pressed "Esc" on their keyboard
				if (!response)
				{
					return DelayedKick(playerid);
				}

				// If the input is less than minimum required password that we set, tell them to put more effort making strong password!
				if (strlen(inputtext) <= MINIMUM_REQUIRED_PASSWORD)
				{
					Dialog_ShowCallback(playerid, using inline dialogRegisterRespond, DIALOG_STYLE_PASSWORD, "Registration", "Your password must be longer than "#MINIMUM_REQUIRED_PASSWORD" characters!\nPlease enter your password in the field below:", "Register", "Abort");
					return 1;
				}

				// After all of this done, if you think it's safe, you can call this function for the AccLib to process registration.
				AccLib_RegisterPlayer(playerid, inputtext);
			}

			// We format it first, and then call the dialog and put the results into the inline above.
			new string:szDialogFormat[64];
			format(szDialogFormat, sizeof szDialogFormat, "Welcome %s, you can register by entering your password in the field below:", name);
			Dialog_ShowCallback(playerid, using inline dialogRegisterRespond, DIALOG_STYLE_PASSWORD, "Registration", szDialogFormat, "Register", "Abort");
		}
	}
	return 1;
}

#include <YSI_Coding\y_hooks>
hook OnPlayerRequestClass(playerid, classid)
{
	// Untuk mencegah F3
	if (!g_sRequestClassPassed[playerid])
	{
		new jam, menit;
		gettime(jam, menit, _);

		TogglePlayerSpectating(playerid, true);
		SetPlayerTime(playerid, jam, menit);

		AccLib_FetchAccount(playerid);
		return 1;
	}
	return 0;
}

hook OnAccountFetched(playerid, bool:success)
{
	if (success) // jika pemain datanya ada, loginkan
	{
		PromptDialog(playerid, ACCOUNT_LOGIN);
		g_sPlayerLoginTimer[playerid] = defer OnPlayerLoginTimeout(playerid);
	}
	else
	{
		PromptDialog(playerid, ACCOUNT_REGISTER);
		g_sPlayerLoginTimer[playerid] = defer OnPlayerLoginTimeout(playerid);
	}
	return 0;
}

hook OnAccountLogin(playerid, bool:success)
{
	if (!success)
	{
		if (++g_sPlayerLoginAttempts[playerid] >= MAX_WRONG_PASSWORD)
		{
				g_sPlayerLoginAttempts[playerid] = 0;
				Dialog_Show(playerid, DIALOG_STYLE_MSGBOX, "NAMAPROJECTMU - Message Notice", "{FFFFFF}Anda telah memasukkan password yang salah sebanyak 3 kali!", "Okay", "");
				DelayedKick(playerid);
				return 0;
		}
		
		SendClientMessage(playerid, -1, "{FF0000}Error:{FFFFFF} Invalid login specified!");
		PromptDialog(playerid, ACCOUNT_LOGIN);
		return 0;
	}

	// Jika sukses, set spawn
	Character_Show(playerid);

	// Matikan timernya
	stop g_sPlayerLoginTimer[playerid];
	return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
	// Reset variable jika sudah keluar
	g_sPlayerLoginAttempts[playerid] = 0;
	stop g_sPlayerLoginTimer[playerid];
	g_sRequestClassPassed[playerid] = 0;
	return 1;
}
