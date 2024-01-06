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

    void display_all_trips_for_location_destination_and_date();
    void edit_schedule();
    void display_stops();
    void display_weekly_schedule_for_driver();
    void add_drive();
    void add_bus();
    void delete_bus();
    void record_trip_data();
    void exit();

};
}