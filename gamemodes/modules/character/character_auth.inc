static enum E_TEMP_CHARACTER_DATA
{
    char_UUID[UUID_LEN],
    char_Name[MAX_PLAYER_NAME + 1]
}

static temp_characterData[MAX_PLAYERS][MAX_CHARACTERS_PER_ACCOUNT][E_TEMP_CHARACTER_DATA];

stock Character_Show(const playerid)
{
    inline dialogCharacterListRespond(pid, dialogid, response, listitem, string:inputtext[])
    {
        #pragma unused dialogid, inputtext

        if(response)
        {
            if(isnull(temp_characterData[pid][listitem][char_UUID]))
            {
                Dialog_ShowCallback(pid, using public dialogCharName<iiiis>, DIALOG_STYLE_INPUT, "Character | Sign Up | Name", "Select {FFFFFF} Enter the last name (or enter the first name) for the name of the vat [code: Ho_Ten, due date: 31 years old]:", ">>", "<<");
            }
            else
            {
                CallLocalFunction("Character_OnLogin", "is", pid, temp_characterData[pid][listitem][char_UUID]);
                for(new i = 0; i != MAX_CHARACTERS_PER_ACCOUNT; i++)
                {
                    temp_characterData[pid][i][char_UUID][0] = EOS;
                    temp_characterData[pid][i][char_Name][0] = EOS;
                }
            }
        }
        else
        {
            Player_Kick(pid, -1, "You're canceled to select character from the list.");
        }
    }

    await mysql_aquery_s(Database_GetHandleId(), str_format("SELECT uuid, name FROM characters WHERE account_id = '%d'", AccLib_GetAccountId(playerid)));

    new rows = cache_num_rows(),
        string[256];

    strcat(string, "Character List\tUUID");

    for(new i = 0; i != MAX_CHARACTERS_PER_ACCOUNT; i++)
    {
        if(i < rows)
        {
            cache_get_value_name(i, "uuid", temp_characterData[playerid][i][char_UUID], UUID_LEN);
            cache_get_value_name(i, "name", temp_characterData[playerid][i][char_Name], MAX_PLAYER_NAME + 1);
        }
        else
        {
            strcopy(temp_characterData[playerid][i][char_Name], "No Have", MAX_PLAYER_NAME + 1);
        }
    }

    for(new i = 0; i != MAX_CHARACTERS_PER_ACCOUNT; i++)
    {
        format(string, sizeof(string), "%s\n%s\t%s", string, temp_characterData[playerid][i][char_Name], temp_characterData[playerid][i][char_UUID]);
    }

    Dialog_ShowCallback(playerid, using inline dialogCharacterListRespond, DIALOG_STYLE_TABLIST_HEADERS, "Character | List", string, "Select", "Cancel");
    return 1;
}

stock bool:Character_IsValidName(const name[], len)
{
    new first_name[MAX_PLAYER_NAME + 1],
        last_name[MAX_PLAYER_NAME + 1];

    if(sscanf(name, "p<_>s[32]s[32]", first_name, last_name))
    {
        return false;
    }

    if(strlen(first_name) == 0 || strlen(last_name) == 0)
    {
        return false;
    }

    while(len--)
    {
        switch(name[len])
        {
            case 'a' .. 'z', 'A' .. 'Z', '_': continue;
            default: return false;
        }
    }
    return true;
}

Character_IsAvailable(const username[]) {
    new 
        count = 0;
    
    await mysql_aquery_s(Database_GetHandleId(), str_format("SELECT COUNT(*) FROM characters WHERE name = '%e' LIMIT 1", username));
    cache_get_value_name_int(0, "COUNT(*)", count);
    return count;    
}

function dialogCharName(playerid, dialogid, response, listitem, string:inputtext[])
{
    #pragma unused dialogid

    if(response)
    {
        if(isnull(inputtext) || strlen(inputtext) >= MAX_PLAYER_NAME)
        {
            Dialog_ShowCallback(playerid, using public dialogCharName<iiiis>, DIALOG_STYLE_INPUT, "Character | Sign Up | Name", "{FFFFFF} Enter the name and the name (or enter the name of the name) for the name of the vat [code: Ho_Ten, due date: 31 years old]:", ">>", "<<");
            return 1;
        }

        if(Character_IsAvailable(inputtext)) {
            Dialog_ShowCallback(playerid, using public dialogCharName<iiiis>, DIALOG_STYLE_INPUT, "Character | Sign Up | Name", "{FFFFFF} Enter the name and the name (or enter the name of the name) for the name of the vat [code: Ho_Ten, due date: 31 years old]:", ">>", "<<");
            return 1;                    
        }

        new 
            count = 0;
        
        await mysql_aquery_s(Database_GetHandleId(), str_format("SELECT COUNT(*) FROM accounts WHERE name = '%e' LIMIT 1", inputtext));
        cache_get_value_name_int(0, "COUNT(*)", count);

        if(count) {
            Dialog_ShowCallback(playerid, using public dialogCharName<iiiis>, DIALOG_STYLE_INPUT, "Character | Sign Up | Name", "{FFFFFF} Enter the name and the name (or enter the name of the name) for the name of the vat [code: Ho_Ten, due date: 31 years old]:", ">>", "<<");
            return 1;                    
        }

        if(!Character_IsValidName(inputtext, strlen(inputtext)))
        {
            Dialog_ShowCallback(playerid, using public dialogCharName<iiiis>, DIALOG_STYLE_INPUT, "Character | Sign Up | Name", "{FFFFFF} Enter the name and the name (or enter the name of the name) for the name of the vat [code: Ho_Ten, due date: 31 years old]:", ">>", "<<");
            return 1;
        }

        SetPVarString(playerid, "charSignUp_name", inputtext);
        Dialog_ShowCallback(playerid, using public dialogCharGender<iiiis>, DIALOG_STYLE_LIST, "Character | Sign Up | Gender", "{FFFFFF}Male\nFemale\n{333333}Choose a gender", ">>", "<<");
    }
    else
    {
        Character_Show(playerid);
    }
    return 1;
}

function dialogCharGender(playerid, dialogid, response, listitem, string:inputtext[])
{
    #pragma unused dialogid

    if(response)
    {
        if(listitem == 2)
        {
            Dialog_ShowCallback(playerid, using public dialogCharGender<iiiis>, DIALOG_STYLE_LIST, "Character | Sign Up | Gender", "{FFFFFF}Male\nFemale\n{333333}Choose a gender", ">>", "<<");
            return 1;
        }

        SetPVarInt(playerid, "charSignUp_gender", listitem);
        Dialog_ShowCallback(playerid, using public dialogCharSkin<iiiis>, DIALOG_STYLE_INPUT, "Character | Sign Up | Skin", "{FFFFFF} Enter the skin ID for the vat:", ">>", "<<");
    }
    else
    {
        DeletePVar(playerid, "charSignUp_name");
        Dialog_ShowCallback(playerid, using public dialogCharName<iiiis>, DIALOG_STYLE_INPUT, "Character | Sign Up | Name", "{FFFFFF} Enter the name and name (or enter the name of the name) for the name [form: Ho_Ten, due date: 31 years old]:", ">>", "<<");
    }
    return 1;
}

function dialogCharSkin(playerid, dialogid, response, listitem, string:inputtext[])
{
    #pragma unused dialogid

    if(response)
    {
        new skin;
        if(sscanf(inputtext, "i", skin))
        {
            Dialog_ShowCallback(playerid, using public dialogCharSkin<iiiis>, DIALOG_STYLE_INPUT, "Character | Sign Up | Skin", "{FFFFFF} Enter the skin ID for the vat:", ">>", "<<");
            return 1;
        }

        if(skin < 1 || skin > 299)
        {
            Dialog_ShowCallback(playerid, using public dialogCharSkin<iiiis>, DIALOG_STYLE_INPUT, "Character | Sign Up | Skin", "{FFFFFF} Enter the skin ID for the vat:", ">>", "<<");
            return 1;
        }

        SetPVarInt(playerid, "charSignUp_skin", skin);
        Dialog_ShowCallback(playerid, using public dialogCharBirthday<iiiis>, DIALOG_STYLE_INPUT, "Character | Sign Up | Birthday", "{FFFFFF}Nhap ngay-thang-nam sinh cua nhan vat:", ">>", "<<");
    }
    else
    {
        DeletePVar(playerid, "charSignUp_gender");
        Dialog_ShowCallback(playerid, using public dialogCharGender<iiiis>, DIALOG_STYLE_LIST, "Character | Sign Up | Gender", "{FFFFFF}Male\nFemale\n{333333}Choose a gender", ">>", "<<");
    }
    return 1;
}

function dialogCharBirthday(playerid, dialogid, response, listitem, string:inputtext[])
{
    #pragma unused dialogid
    
    if(response)
    {
        new day, month, year, day2, month2, year2;
        getdate(year, month, day);

        if(sscanf(inputtext, "p<->iii", day2, month2, year2)) {
            return Dialog_ShowCallback(playerid, using public dialogCharBirthday<iiiis>, DIALOG_STYLE_INPUT, "Character | Sign Up | Birthday", "{FFFFFF}Nhap ngay-thang-nam sinh cua nhan vat:", ">>", "<<");
        }

        new check = year - year2;
        if(check > 50 || check < 15 || day2 > GetMonthDays(year2, month2) || day2 < 1 || month2 > 12 || month2 < 1) {
            if(check > 50 || check < 15) SendClientMessage(playerid, -1, sprintf("Nam sinh gioi han tu %d - %d.", year - 15, year - 50));
            if(month2 > 12 || month2 < 1) SendClientMessage(playerid, -1, "Thang sinh gioi han tu 1 - 12.");
            if(day2 > GetMonthDays(year2, month2) || day2 < 1) SendClientMessage(playerid, -1, sprintf("Thang %02d co ngay gioi han tu 1 - %d.", GetMonthDays(year2, month2)));
            return Dialog_ShowCallback(playerid, using public dialogCharBirthday<iiiis>, DIALOG_STYLE_INPUT, "Character | Sign Up | Birthday", "{FFFFFF}Nhap ngay-thang-nam sinh cua nhan vat:", ">>", "<<");
        }

        new name[MAX_PLAYER_NAME + 1],
            birthday[12],
            string[180];

        GetPVarString(playerid, "charSignUp_name", name, sizeof(name));
        format(birthday, sizeof(birthday), "%02d-%02d-%d", day2, month2, year2);
        SetPVarString(playerid, "charSignUp_birthday", birthday);

        format(string, sizeof(string), "\n\
        {FFFFFF}Name: %s\n\
        Gender: %s\n\
        Skin ID: %d\n\
        Birthday: %s\n\n\
        Please confirm the information you have entered, then we will continue to be 'acknowledged'.", name, (GetPVarInt(playerid, "charSignUp_gender") == 0) ? "Male" : "Female", GetPVarInt(playerid, "charSignUp_skin"), birthday);

        Dialog_ShowCallback(playerid, using public dialogCharConfirm<iiiis>, DIALOG_STYLE_MSGBOX, "Character | Sign Up | Confirm", string, "Confirm", "<<");
    }
    else {
        DeletePVar(playerid, "charSignUp_Skin");
        Dialog_ShowCallback(playerid, using public dialogCharSkin<iiiis>, DIALOG_STYLE_INPUT, "Character | Sign Up | Skin", "{FFFFFF} Enter the skin ID for the vat:", ">>", "<<");
    }
    return 1;
}

function dialogCharConfirm(playerid, dialogid, response, listitem, string:inputtext[])
{
    #pragma unused dialogid

    if(response)
    {
        new uuid[UUID_LEN],
            name[MAX_PLAYER_NAME + 1],
            birthday[12];

        UUID(uuid, sizeof(uuid));
        GetPVarString(playerid, "charSignUp_name", name, sizeof(name));
        GetPVarString(playerid, "charSignUp_birthday", birthday, sizeof(birthday));
        
        await mysql_aquery_s(Database_GetHandleId(), str_format("INSERT INTO characters (uuid, account_id, name, gender, skin, birthday) VALUES ('%e', '%d', '%e', %d, %d, '%e')", uuid, AccLib_GetAccountId(playerid), name, GetPVarInt(playerid, "charSignUp_gender"), GetPVarInt(playerid, "charSignUp_skin"), birthday));

        if(cache_affected_rows() == 0)
        {
            Player_Kick(playerid, -1, "I didn't get it done in the first place, please feel free to report or re-enter.");
            return 1;
        }
        
        Character_Show(playerid);
    }
    else
    {
        DeletePVar(playerid, "charSignUp_birthday");
        Dialog_ShowCallback(playerid, using public dialogCharBirthday<iiiis>, DIALOG_STYLE_INPUT, "Character | Sign Up | Birthday", "{FFFFFF}Nhap ngay-thang-nam sinh cua nhan vat:", ">>", "<<");
    }
    return 1;
}
