#pragma once

#include <vector>
#include <iostream>
#include "../utils/DatabaseAccess.h"

namespace transit 
{
class TransitMenu
{
public:

    TransitMenu()
    {
        std::cout << "Transit Menu constructor\n";
        display_menu();
    }

    void display_menu()
    {
        //display all options in enum
        //get user input
        //call appropriate function
        std::cout << "Displaying menu\n";
        std::cout << "1. Display all trips for location, destination, and date\n";
        std::cout << "2. Edit schedule\n";
        std::cout << "3. Display stops\n";
        std::cout << "4. Display weekly schedule for driver\n";
        std::cout << "5. Add drive\n";
        std::cout << "6. Add bus\n";
        std::cout << "7. Delete bus\n";
        std::cout << "8. Record trip data\n";
        std::cout << "9. Exit\n";
        std::cout << "Enter option: ";

        int user_input;
        std::cin >> user_input;
        types::MenuOption option = static_cast<types::MenuOption>(user_input);

        std::cout << "You entered: " << option << "\n";
        switch (option)
        {
        case types::MenuOption::DisplayAllTripsForLocationDestinationAndDate:
            display_all_trips_for_location_destination_and_date();
            break;
        default:
            std::cout << "Invalid option\n";
            break;
        }

    }




private:

    void display_all_trips_for_location_destination_and_date()
    {
        std::cout << "Displaying all trips for location, destination, and date\n";
        db::DatabaseAccess::instance().query();
    }

};
}