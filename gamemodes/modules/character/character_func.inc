enum
{
    CHARACTER_INVALID_SPAWN_TYPE,
    CHARACTER_SPAWN,
    CHARACTER_SWITCH,
    CHARACTER_SPECTATE,
    CHARACTER_DEAD
}

static enum E_CHARACTER_DATA
{
    bool:char_Spawned,
    char_Spawn_Type,
    
    char_UUID[UUID_LEN],
    char_Account_ID,
    char_Name[MAX_PLAYER_NAME + 1],
    char_Skin,
}

static characterData[MAX_PLAYERS][E_CHARACTER_DATA], Float:defaultSpawnCoords[4] = {1642.1681, -2333.3689, 13.5469, 0.0};

stock bool:Character_IsSpawned(const playerid)
{
    return characterData[playerid][char_Spawned];
}

stock Character_GetUUID(const playerid, uuid[], const size = UUID_LEN)
{
    uuid[0] = EOS;
    strcat(uuid, characterData[playerid][char_UUID], size);
    return 1;
}

stock Character_GetAccountID(const playerid)
{
    return characterData[playerid][char_Account_ID];
}

stock Character_GetName(const playerid, name[], size = MAX_PLAYER_NAME + 1)
{
    name[0] = EOS;
    strcat(name, characterData[playerid][char_Name], size);
    return 1;
}

stock Character_Reset(const playerid)
{
    characterData[playerid][char_UUID][0] = EOS;
    characterData[playerid][char_Account_ID] = 0;
    characterData[playerid][char_Spawned] = false;
    characterData[playerid][char_Skin] = 0;
    return 1;
}

stock Character_Spawn(const playerid, const uuid[])
{
    task_unblock(1);
    await mysql_aquery_s(Database_GetHandleId(), str_format("SELECT * FROM characters WHERE uuid = '%e' LIMIT 1", uuid));

    if(cache_num_rows())
    {
        new Float:pos[3],
            Float:angle,
            Float:health,
            Float:armour;
        
        cache_get_value_name(0, "uuid", characterData[playerid][char_UUID], UUID_LEN);
        cache_get_value_name_int(0, "account_id", characterData[playerid][char_Account_ID]);
        cache_get_value_name(0, "name", characterData[playerid][char_Name], MAX_PLAYER_NAME + 1);
        
        cache_get_value_name_int(0, "skin", characterData[playerid][char_Skin]);
        
        cache_get_value_name_float(0, "pos_x", pos[0]);
        cache_get_value_name_float(0, "pos_y", pos[1]);
        cache_get_value_name_float(0, "pos_z", pos[2]);
        cache_get_value_name_float(0, "angle", angle);
        cache_get_value_name_float(0, "health", health);
        cache_get_value_name_float(0, "armour", armour);

        TogglePlayerSpectating(playerid, false);

        characterData[playerid][char_Spawned] = true;
        characterData[playerid][char_Spawn_Type] = CHARACTER_SPAWN;

        if(pos[0] == 0.0000 && pos[1] == 0.0000 && pos[2] == 0.0000)
        {
            pos[0] = defaultSpawnCoords[0];
            pos[1] = defaultSpawnCoords[1];
            pos[2] = defaultSpawnCoords[2];
            angle = defaultSpawnCoords[3];
        }

        Player_SetName(playerid, characterData[playerid][char_Name]);

        SetSpawnInfo(playerid, 1, characterData[playerid][char_Skin], pos[0], pos[1], pos[2], angle, 0, 0, 0, 0, 0, 0);
        SpawnPlayer(playerid);

        SetPlayerHealth(playerid, health);
        SetPlayerArmour(playerid, armour);
    }
    else
    {
        SendClientMessage(playerid, -1, "Cannot receive your data from database");
        DelayedKick(playerid, 3000);
    }
    return 1;
}

public Character_OnLogin(const playerid, const uuid[])
{
    Character_Reset(playerid);
    Character_Spawn(playerid, uuid);
    return 1;
}

public Character_OnLogout(const playerid)
{
    new Float:pos[3],
        Float:health,
        Float:armour,
        Float:angle;
    
    GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
    GetPlayerFacingAngle(playerid, angle);
    GetPlayerHealth(playerid, health);
    GetPlayerArmour(playerid, armour);

    await mysql_aquery_s(Database_GetHandleId(), str_format("UPDATE characters SET pos_x = %.4f, pos_y = %.4f, pos_z = %.4f, angle = %.4f, health = %.4f, armour = %.4f WHERE uuid = '%e' LIMIT 1", pos[0], pos[1], pos[2], angle, health, armour, characterData[playerid][char_UUID]), true);

    if(cache_affected_rows() != 0)
    {
        printf("[!]: Character_OnLogout - character's stats saved (%s)", characterData[playerid][char_UUID]);
    }
    else
    {
        printf("[!]: Character_OnLogout - character's stats failed to save (%s)", characterData[playerid][char_UUID]);
    }

    Character_Reset(playerid);
    return 1;
}

public Character_OnSpawn(const playerid, const type)
{
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    if(characterData[playerid][char_Spawned])
    {
        CallLocalFunction("Character_OnLogout", "i", playerid);
    }
    return 1;
}

hook OnPlayerSpawn(playerid)
{
    CallLocalFunction("Character_OnSpawn", "ii", playerid, characterData[playerid][char_Spawn_Type]);
    characterData[playerid][char_Spawn_Type] = CHARACTER_INVALID_SPAWN_TYPE;
    return 1;
}