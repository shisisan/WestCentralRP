
// Include guard (versi simpel)
#if defined _INC_components_database
	#endinput
#endif

#define _INC_components_database

// Include Module
#include <a_mysql>

// Function or Callback
forward MySQL:Database_GetHandleId();
forward OnVersionInformationAvailable(MySQL:handle);

// Internal Module
#include <database\database_entry>
