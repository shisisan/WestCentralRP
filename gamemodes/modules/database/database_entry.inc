
// Entry bisa dipecah menjadi beberapa bagian
#include <database\database_connection>


public OnQueryError(errorid, const error[], const callback[], const query[], MySQL:handle)
{
    print("ERROR: OnQueryError");
    printf("     > errorid: %d", errorid);
    printf("     > error: %s", error);
    printf("     > callback: %s", callback);
    printf("     > query: %s", query);
    printf("     > handle: %d", _:handle);
    return 1;
}