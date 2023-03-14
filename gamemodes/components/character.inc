
// Include guard (versi simpel)
#if defined _INC_components_character
	#endinput
#endif

#define _INC_components_character

// Configuration
#define MAX_CHARACTERS_PER_ACCOUNT	3

// Include Module
#include <PawnPlus>
#include <sscanf2>
#include <uuid>
#include <pp-mysql>
#include <strlib>

#include <YSI_Coding\y_hooks>
#include <YSI_Coding\y_inline>
#include <YSI_Visual\y_dialog>

// Function or Callback
forward Character_OnLogin(const playerid, const uuid[]);
forward Character_OnLogout(const playerid);
forward Character_OnSpawn(const playerid, const type);

// Internal Module
#include <character\character_entry>
