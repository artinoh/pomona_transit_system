#include <iostream>
#include "transit_menu/transit_menu.h"

int main()
{
    //prime the database
    transit::db::DatabaseAccess::instance();
    transit::TransitMenu menu;
    return 0;
}
