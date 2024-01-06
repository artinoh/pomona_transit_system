#pragma once

#include <iostream>
#include <ostream>

namespace transit::types
{

    using TripNumber = uint64_t;
    using BusId = uint64_t;

    enum class MenuOption
    {
        None,
        DisplayTrips,
        EditSchedule,
        DisplayStops,
        DisplayWeeklyScheduleForDriver,
        AddDriver,
        AddBus,
        DeleteBus,
        RecordTripData,
        Exit
    };


    inline std::ostream& operator<<(std::ostream& os, const MenuOption& option)
    {
        switch (option)
        {
        case MenuOption::None: os << "None"; break;
        case MenuOption::DisplayTrips: os << "DisplayTrips"; break;
        case MenuOption::EditSchedule: os << "EditSchedule"; break;
        case MenuOption::DisplayStops: os << "DisplayStops"; break;
        case MenuOption::DisplayWeeklyScheduleForDriver: os << "DisplayWeeklyScheduleForDriver"; break;
        case MenuOption::AddDriver: os << "AddDriver"; break;
        case MenuOption::AddBus: os << "AddBus"; break;
        case MenuOption::DeleteBus: os << "DeleteBus"; break;
        case MenuOption::RecordTripData: os << "RecordTripData"; break;
        case MenuOption::Exit: os << "Exit"; break;
        default: os << "Invalid option"; break;
        }
        return os;
    }


    enum class EditScheduleOption
    {
        None,
        DeleteTrip,
        AddTrip,
        ChangeDriver,
        ChangeBus,
        Exit
    };

    inline std::ostream& operator<<(std::ostream& os, const EditScheduleOption& option)
    {
        switch (option)
        {
        case EditScheduleOption::None: os << "None"; break;
        case EditScheduleOption::DeleteTrip: os << "DeleteTrip"; break;
        case EditScheduleOption::AddTrip: os << "AddTrip"; break;
        case EditScheduleOption::ChangeDriver: os << "ChangeDriver"; break;
        case EditScheduleOption::ChangeBus: os << "ChangeBus"; break;
        case EditScheduleOption::Exit: os << "Exit"; break;
        default: os << "Invalid option"; break;
        }
        return os;
    }


}