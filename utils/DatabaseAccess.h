#pragma once

#include <iostream>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <string>
#include <sstream>
#include <iomanip>

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

    inline std::ostream &operator<<(std::ostream &os, const MenuOption &option)
    {
        switch (option)
        {
        case MenuOption::None:
            os << "None";
            break;
        case MenuOption::DisplayTrips:
            os << "DisplayTrips";
            break;
        case MenuOption::EditSchedule:
            os << "EditSchedule";
            break;
        case MenuOption::DisplayStops:
            os << "DisplayStops";
            break;
        case MenuOption::DisplayWeeklyScheduleForDriver:
            os << "DisplayWeeklyScheduleForDriver";
            break;
        case MenuOption::AddDriver:
            os << "AddDriver";
            break;
        case MenuOption::AddBus:
            os << "AddBus";
            break;
        case MenuOption::DeleteBus:
            os << "DeleteBus";
            break;
        case MenuOption::RecordTripData:
            os << "RecordTripData";
            break;
        case MenuOption::Exit:
            os << "Exit";
            break;
        default:
            os << "Invalid option";
            break;
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

    inline std::ostream &operator<<(std::ostream &os, const EditScheduleOption &option)
    {
        switch (option)
        {
        case EditScheduleOption::None:
            os << "None";
            break;
        case EditScheduleOption::DeleteTrip:
            os << "DeleteTrip";
            break;
        case EditScheduleOption::AddTrip:
            os << "AddTrip";
            break;
        case EditScheduleOption::ChangeDriver:
            os << "ChangeDriver";
            break;
        case EditScheduleOption::ChangeBus:
            os << "ChangeBus";
            break;
        case EditScheduleOption::Exit:
            os << "Exit";
            break;
        default:
            os << "Invalid option";
            break;
        }
        return os;
    }
}

namespace transit::db
{
    class DatabaseAccess
    {
    public:
        static DatabaseAccess &instance()
        {
            static DatabaseAccess instance;
            return instance;
        }

        bool addBus(types::BusId busId, const std::string &model, uint16_t year);
        bool deleteBus(types::BusId busId);
        bool addDriver(const std::string &name, const std::string &phoneNumber);
        bool recordTripData(types::TripNumber tripNumber, 
                            const std::string &date,
                            int stop_number,
                            const std::string &start_time,
                            const std::string &arrival_time,
                            int number_passengers_in,
                            int number_passengers_out);

        void displayBuses();
        void displayTrips(const std::string &location, const std::string &destination, const std::string &date);
        void displayWeeklyScheduleForDriver(const std::string &driverName, const std::string &date);
        void displayTripStopInfo(types::TripNumber tripNumber);

    private:
        DatabaseAccess();
        ~DatabaseAccess();
        DatabaseAccess(const DatabaseAccess &) = delete;
        DatabaseAccess &operator=(const DatabaseAccess &) = delete;

        void handleDiagnosticRecord(SQLHANDLE handle, SQLSMALLINT handleType);
        void displayQueryResults(SQLHANDLE handle);

        void getColumnInformation(SQLHSTMT hStmt, std::vector<std::string> &columnNames, std::vector<int> &columnWidths);
        void storeAndCalculateColumnWidths(SQLHSTMT hStmt, std::vector<int> &columnWidths, std::vector<std::vector<std::string>> &rowData);
        void printHeader(const std::vector<std::string> &columnNames, const std::vector<int> &columnWidths);
        void printDataRows(const std::vector<std::vector<std::string>> &rowData, const std::vector<int> &columnWidths);

        SQLHENV hEnv;      // Environment handle
        SQLHDBC hDbc;      // Connection handle
        SQLHSTMT hStmt;    // Statement handle
        SQLRETURN retcode; // Return code
        bool initialized = false;
    };
}
