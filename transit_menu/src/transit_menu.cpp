#include "../transit_menu.h"

template<typename T>
inline std::string get_input()
{
    std::string input;
    std::getline(std::cin, input);
    return static_cast<T>(input);
}


namespace transit
{
TransitMenu::TransitMenu()
{
    std::cout << "Transit Menu constructor\n";
    display_menu();
}

void TransitMenu::display_menu()
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


void TransitMenu::display_all_trips_for_location_destination_and_date()
{
    std::cout << "Displaying all trips for location, destination, and date\n";
    db::DatabaseAccess::instance().query();
}

void TransitMenu::edit_schedule()
{
    std::cout << "Editing schedule\n";
}

void TransitMenu::display_stops()
{
    std::cout << "Displaying stops\n";
}

void TransitMenu::display_weekly_schedule_for_driver()
{
    std::cout << "Displaying weekly schedule for driver\n";
}

void TransitMenu::add_drive()
{
    std::cout << "Adding drive\n";
}

void TransitMenu::add_bus()
{
    std::cout << "Adding bus\n";
}

void TransitMenu::delete_bus()
{
    std::cout << "Deleting bus\n";
}

void TransitMenu::record_trip_data()
{
    std::cout << "Recording trip data\n";
}

void TransitMenu::exit()
{
    std::cout << "Exiting\n";
}


}