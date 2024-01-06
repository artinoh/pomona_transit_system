#pragma once

#include <vector>
#include <iostream>
#include "../utils/DatabaseAccess.h"

namespace transit 
{
class TransitMenu
{
public:

    TransitMenu();
    void display_menu();

private:

    void run();
    void handle_menu_option(types::MenuOption user_input);

    void display_trips();
    void edit_schedule();
    void display_stops();
    void display_weekly_schedule_for_driver();
    void add_driver();
    void add_bus();
    void delete_bus();
    void record_trip_data();
    void exit();

};
}