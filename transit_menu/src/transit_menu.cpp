#include "../transit_menu.h"
#include <sstream>
#include <optional>

template<typename T>
T get_input(std::optional<std::string> prompt = std::nullopt) {
    std::string line;
    T value;

    if (prompt.has_value()) {
        std::cout << prompt.value();
    }

    std::getline(std::cin, line);
    if constexpr (std::is_same_v<T, std::string>) {
        return line;
    }
    std::stringstream ss(line);
    ss >> value;
    if (ss.fail()) {
        throw std::runtime_error("Invalid input format.");
    }

    return value;
}

namespace transit
{
TransitMenu::TransitMenu()
{
    run();
}

void TransitMenu::run()
{
    types::MenuOption user_input = types::MenuOption::None;
    while (user_input != types::MenuOption::Exit)
    {
        display_menu();
        std::cout << "Please enter a selection: ";
        user_input = static_cast<types::MenuOption>(get_input<int>());
        handle_menu_option(user_input);
    }
}

void TransitMenu::display_menu()
{
    std::cout << "---------------------------------\n";
    std::cout << "Welcome to the Transit Menu\n";
    std::cout << "---------------------------------\n";
    for (int i = 1; i <= static_cast<int>(types::MenuOption::Exit); i++)
    {
        std::cout << i << ": " << static_cast<types::MenuOption>(i) << "\n";
    }
    std::cout << "---------------------------------\n";
}

void TransitMenu::handle_menu_option(types::MenuOption user_input)
{
    switch (user_input)
    {
    case types::MenuOption::DisplayTrips: display_trips(); break;
    case types::MenuOption::EditSchedule: edit_schedule(); break;
    case types::MenuOption::DisplayStops: display_stops(); break;
    case types::MenuOption::DisplayWeeklyScheduleForDriver: display_weekly_schedule_for_driver(); break;
    case types::MenuOption::AddDriver: add_driver(); break;
    case types::MenuOption::AddBus: add_bus(); break;
    case types::MenuOption::DeleteBus: delete_bus(); break;
    case types::MenuOption::RecordTripData: record_trip_data(); break;
    case types::MenuOption::Exit: exit(); break;
    default: std::cout << "Invalid option\n"; break;
    }
}


void TransitMenu::display_trips()
{
    std::cout << "You have chosen to display all trips for location, destination, and date\n";
    std::string location = get_input<std::string>("Please enter the starting location: ");
    std::string destination = get_input<std::string>("Please enter the destination: ");
    std::string date = get_input<std::string>("Please enter the date: (YYYY-MM-DD)");

    std::cout << "---------------------------------\n";
    db::DatabaseAccess::instance().displayTrips(location, destination, date);
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

void TransitMenu::add_driver()
{
    std::cout << "You have chosen to add a driver\n";
    std::string name = get_input<std::string>("Please enter the driver name: ");
    std::string phone_number = get_input<std::string>("Please enter the driver phone number: ");

    if (db::DatabaseAccess::instance().addDriver(name, phone_number))
    {
        std::cout << "Successfully added driver\n";
    }
    else
    {
        std::cout << "Failed to add driver\n";
    }
}

void TransitMenu::add_bus()
{
    std::cout << "You have chosen to add a bus\n";
    types::BusId bus_id = get_input<types::BusId>("Please enter the bus id: ");
    std::string model = get_input<std::string>("Please enter the bus model: ");
    uint16_t year = get_input<uint16_t>("Please enter the bus year: ");

    if (db::DatabaseAccess::instance().addBus(bus_id, model, year))
    {
        std::cout << "Successfully added bus\n";
    }
    else
    {
        std::cout << "Failed to add bus\n";
    }
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
    std::cout << "---------------------------------\n";
    std::cout << "Thanks for using the Transit Menu!\n";
    std::cout << "---------------------------------\n";
}


}