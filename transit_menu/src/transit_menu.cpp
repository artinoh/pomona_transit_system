#include "../transit_menu.h"


template<typename T>
T get_input(std::optional<std::string> prompt = std::nullopt) {
    std::string line;
    T value;

    if (prompt.has_value()) {
        std::cout << prompt.value();
    }

    std::getline(std::cin, line);

    if (line.empty()) {
        std::cout << "Invalid input format.\n";
        return value;
    }

    if constexpr (std::is_same_v<T, std::string>) {
        return line;
    }
    std::stringstream ss(line);
    ss >> value;
    if (ss.fail()) {
        std::cout << "Invalid input format.\n";
        return value;
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
            system("clear");
            display_menu();
            try {
                user_input = static_cast<types::MenuOption>(get_input<int>("Please enter a selection: "));
            }
            catch (const std::exception& e) {
                std::cout << e.what() << std::endl;
                continue;
            }
            handle_menu_option(user_input);
            if (user_input != types::MenuOption::Exit)
                get_input<std::string>("Press enter to continue...");
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
        db::DatabaseAccess::instance().displayTrips(location, destination, date);
    }

    types::TripOffering TransitMenu::get_trip_offering_from_user()
    {
        types::TripOffering trip_offering;
        std::cout << "Please enter the trip offering information\n";
        trip_offering.tripNumber = static_cast<types::TripNumber>(get_input<int>("Please enter the trip number: "));
        trip_offering.date = get_input<std::string>("Please enter the date: (YYYY-MM-DD)");
        trip_offering.scheduledStartTime = get_input<std::string>("Please enter the scheduled start time: (HH:MM:SS)");
        return trip_offering;
    }

    void TransitMenu::edit_schedule()
    {
        std::cout << "You have chosen to edit the schedule\n";
        std::cout << "---------------------------------\n";
        for (int i = 1; i <= static_cast<int>(types::EditScheduleOption::Exit); i++)
        {
            std::cout << i << ": " << static_cast<types::EditScheduleOption>(i) << "\n";
        }
        std::cout << "---------------------------------\n";
        types::EditScheduleOption user_input = static_cast<types::EditScheduleOption>(get_input<int>("Please enter a selection: "));
        std::cout << "You have chosen: " << user_input << "\n";
        if (user_input >= types::EditScheduleOption::Exit)
            return;
        
        bool transaction_successful = false;
        types::TripOffering trip_offering = get_trip_offering_from_user();
        switch (user_input)
        {
        case types::EditScheduleOption::DeleteTrip:
        {
            transaction_successful = db::DatabaseAccess::instance().deleteTripOffering(trip_offering);
            break;
        }
        case types::EditScheduleOption::AddTrip:
        {
            bool keep_adding = true;
            while (keep_adding)
            {
                trip_offering.scheduledArrivalTime = get_input<std::string>("Please enter the scheduled arrival time: (HH:MM:SS)");
                trip_offering.driverName = get_input<std::string>("Please enter the driver name: ");
                trip_offering.busId = static_cast<types::BusId>(get_input<int>("Please enter the bus id: "));
                transaction_successful = db::DatabaseAccess::instance().addTripOffering(trip_offering);
                std::cout << "Transaction successful: " << (transaction_successful ? "true" : "false") << "\n";
                char user_input = get_input<char>("Would you like to add another trip offering? (y/n): ");
                keep_adding = user_input == 'y';
                if (keep_adding) 
                    trip_offering = get_trip_offering_from_user();
            }
            break;
        }
        case types::EditScheduleOption::ChangeDriver:
        {
            std::string driver_name = get_input<std::string>("Please enter the driver name: ");
            transaction_successful = db::DatabaseAccess::instance().changeDriverForTripOffering(trip_offering, driver_name);
            break;
        }
        case types::EditScheduleOption::ChangeBus:
        {
            types::BusId bus_id = static_cast<types::BusId>(get_input<int>("Please enter the bus id: "));
            transaction_successful = db::DatabaseAccess::instance().changeBusForTripOffering(trip_offering, bus_id);
            break;
        }
        case types::EditScheduleOption::Exit:
        default:
        {
            break;
        }
        }
        if (transaction_successful)
        {
            std::cout << "Successfully updated trip offering\n";
        }
        else
        {
            std::cout << "Failed to update trip offering\n";
        }
    }

    void TransitMenu::display_stops()
    {
        std::cout << "You have chosen to display the stops for a trip\n";
        types::TripNumber trip_number = get_input<types::TripNumber>("Please enter the trip number: ");
        db::DatabaseAccess::instance().displayTripStopInfo(trip_number);
    }

    void TransitMenu::display_weekly_schedule_for_driver()
    {
        std::cout << "You have chosen to display the weekly schedule for a driver\n";
        std::string driver_name = get_input<std::string>("Please enter the driver name: ");
        std::string date = get_input<std::string>("Please enter the date: (YYYY-MM-DD)");
        db::DatabaseAccess::instance().displayWeeklyScheduleForDriver(driver_name, date);

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
        db::DatabaseAccess::instance().displayBuses();
        std::cout << "You have chosen to delete a bus\n";
        types::BusId bus_id = get_input<types::BusId>("Please enter the bus id: ");
        if (bus_id == 0)
        {
            std::cout << "Invalid bus id\n";
            return;
        }
        db::DatabaseAccess::instance().deleteBus(bus_id);
    }

    void TransitMenu::record_trip_data()
    {
        std::cout << "You have chosen to record trip data\n";
        auto trip_number = get_input<types::TripNumber>("Please enter the trip number: ");
        auto date = get_input<std::string>("Please enter the date: (YYYY-MM-DD)");
        auto stop_number = get_input<int>("Please enter the stop number: ");
        auto actual_start_time = get_input<std::string>("Please enter the actual start time: (HH:MM:SS)");
        auto actual_arrival_time = get_input<std::string>("Please enter the actual arrival time: (HH:MM:SS)");
        auto number_passengers_in = get_input<int>("Please enter the number of passengers in: ");
        auto number_passengers_out = get_input<int>("Please enter the number of passengers out: ");

        if (db::DatabaseAccess::instance().recordTripData(trip_number, date, stop_number, actual_start_time, actual_arrival_time, number_passengers_in, number_passengers_out))
        {
            std::cout << "Successfully recorded trip data\n";
        }
        else
        {
            std::cout << "Failed to record trip data\n";
        }
    }

    void TransitMenu::exit()
    {
        std::cout << "---------------------------------\n";
        std::cout << "Thanks for using the Transit Menu!\n";
        std::cout << "---------------------------------\n";
    }


}