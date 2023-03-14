#include <character\character_timer>
#include <character\character_func>
#include <character\character_auth>

@init(.order = script_init) OnCharacterInit()
{
    mysql_query(Database_GetHandleId(), "CREATE TABLE IF NOT EXISTS characters (\
        uuid VARCHAR(37) NOT NULL PRIMARY KEY DEFAULT '',\
        name VARCHAR(33) NOT NULL DEFAULT '',\
        gender TINYINT UNSIGNED NOT NULL DEFAULT 0,\
        skin INT NOT NULL DEFAULT 0,\
        birthday VARCHAR(12) NOT NULL DEFAULT '',\
        health FLOAT(11, 4) NOT NULL DEFAULT 100.0000,\
        armour FLOAT(11, 4) NOT NULL DEFAULT 0.0000,\
        pos_x FLOAT(11, 4) NOT NULL DEFAULT 0.0000,\
        pos_y FLOAT(11, 4) NOT NULL DEFAULT 0.0000,\
        pos_z FLOAT(11, 4) NOT NULL DEFAULT 0.0000,\
        angle FLOAT(11, 4) NOT NULL DEFAULT 0.0000,\
        interior INT NOT NULL DEFAULT 0,\
        world INT NOT NULL DEFAULT 0,\
        created_date TIMESTAMP NULL DEFAULT CURRENT_TIMESTAMP,\
        account_id  INT NOT NULL,\
        FOREIGN KEY (account_id) REFERENCES accounts(id) ON UPDATE CASCADE ON DELETE CASCADE \
    );", false);
    
    for(new i = 0; i != MAX_PLAYERS; i++)
    {
        Character_Reset(i);
    }

    print("[+] Character module is successfully loaded.");
    return 1;
}
