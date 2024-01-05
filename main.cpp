#include <iostream>
#include "utils/db/DatabaseAccess.h"

int main()
{
    std::cout << "Hello World!" << std::endl;
    db::DatabaseAccess::instance().query();
    return 0;
}
