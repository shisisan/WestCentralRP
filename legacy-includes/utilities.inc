
/*
	@Title: Utilities module
	@Version: 0.0.1
	@Author: pawn-id

	Module utilitiy untuk template gamemode
*/

#if defined _INC_utilities
	#endinput
#endif

#define _INC_utilities

#include <YSI_Coding\y_inline>
#include <YSI_Extra\y_inline_timers>

stock DelayedKick(playerid, interval = 1000)
{
	inline onKicked()
	{
		Kick(playerid);
	}
	Timer_CreateCallback(using inline onKicked, interval, 1);
	return 1;
}

stock Player_SetName(const playerid, const name[])
{
    return SetPlayerName(playerid, name);
}

stock Player_Kick(const playerid, const color = -1, const message[] = "")
{
    if(isnull(message))
    {
        Kick(playerid);
    }
    else
    {
		SendClientMessage(playerid, color, message);
        DelayedKick(playerid, 5000);
    }
    return 1;
}

stock GetFebruaryDays(year) {
    if(!(year % 4)) return 29;
    else return 28;
}

stock GetMonthDays(year, month) {
    switch(month) {
        case 0:     return 31;
        case 1:     return 31;
        case 2:     return GetFebruaryDays(year);
        case 3:     return 31;
        case 4:     return 30;
        case 5:     return 31;
        case 6:     return 30;
        case 7:     return 31;
        case 8:     return 31;
        case 9:     return 30;
        case 10:    return 31;
        case 11:    return 30;
        case 12:    return 31;
        default:    return -1;
    }
    return -1;
}
