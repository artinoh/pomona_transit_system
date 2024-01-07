#include "../DatabaseAccess.h"

namespace transit::db
{
    DatabaseAccess::DatabaseAccess()
    {
        if (initialized)
            return;

        // Allocate an environment handle
        if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS)
        {
            handleDiagnosticRecord(hEnv, SQL_HANDLE_ENV);
            return;
        }
        SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);

        // Allocate a connection handle
        if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) != SQL_SUCCESS)
        {
            handleDiagnosticRecord(hEnv, SQL_HANDLE_ENV);
            return;
        }

        // Connect to the database
        std::string connectionString = "DRIVER={ODBC Driver 18 for SQL Server};"
            "SERVER=localhost;"
            "DATABASE=LAB4;"
            "UID=sa;"
            "PWD=someThingComplicated1234;"
            "TrustServerCertificate=yes;";
        retcode = SQLDriverConnect(hDbc, NULL, (SQLCHAR*)connectionString.c_str(), SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);

        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            handleDiagnosticRecord(hDbc, SQL_HANDLE_DBC);
            abort();
            return;
        }
        initialized = true;
    }

    void DatabaseAccess::handleDiagnosticRecord(SQLHANDLE handle, SQLSMALLINT handleType)
    {
        SQLSMALLINT i = 0;
        SQLINTEGER native;
        SQLCHAR state[7], text[256];
        SQLSMALLINT len;
        SQLRETURN ret;

        while ((ret = SQLGetDiagRec(handleType, handle, ++i, state, &native, text, sizeof(text), &len)) != SQL_NO_DATA)
        {
            std::cerr << "State: " << state << ", Native Error: " << native << ", Message: " << text << "\n";
        }
    }

    DatabaseAccess::~DatabaseAccess()
    {
        if (hStmt)
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        if (hDbc)
        {
            SQLDisconnect(hDbc);
            SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
        }
        if (hEnv)
            SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
    }

    void DatabaseAccess::getColumnInformation(SQLHSTMT hStmt, std::vector<std::string>& columnNames, std::vector<int>& columnWidths)
    {
        SQLSMALLINT columns;
        SQLRETURN retcode = SQLNumResultCols(hStmt, &columns);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << "Error in SQLNumResultCols" << std::endl;
            return;
        }

        columnNames.resize(columns);
        columnWidths.resize(columns, 0);

        for (SQLSMALLINT i = 0; i < columns; i++)
        {
            SQLCHAR columnName[128];
            SQLSMALLINT columnNameLength;

            retcode = SQLDescribeCol(hStmt, i + 1, columnName, sizeof(columnName), &columnNameLength, NULL, NULL, NULL, NULL);
            if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
            {
                std::cerr << "Error in SQLDescribeCol" << std::endl;
                return;
            }

            columnNames[i] = std::string(reinterpret_cast<char*>(columnName));
            columnWidths[i] = std::max(columnWidths[i], static_cast<int>(columnNames[i].length()));
        }
    }

    void DatabaseAccess::printHeader(const std::vector<std::string>& columnNames, const std::vector<int>& columnWidths)
    {
        for (size_t i = 0; i < columnNames.size(); i++)
        {
            std::cout << std::left << std::setw(columnWidths[i] + 2) << columnNames[i] << "|";
        }
        std::cout << std::endl;
    }

    void DatabaseAccess::printSeparator(const std::vector<int>& columnWidths)
    {
        for (size_t i = 0; i < columnWidths.size(); i++)
        {
            std::cout << std::string(columnWidths[i] + 2, '-') << "+";
        }
        std::cout << std::endl;
    }

    void DatabaseAccess::storeAndCalculateColumnWidths(SQLHSTMT hStmt, std::vector<int>& columnWidths, std::vector<std::vector<std::string>>& rowData)
    {
        SQLRETURN retcode;
        while (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            std::vector<std::string> row(columnWidths.size());
            for (size_t i = 0; i < columnWidths.size(); i++)
            {
                SQLCHAR columnData[256];
                SQLLEN columnDataLength;

                retcode = SQLGetData(hStmt, i + 1, SQL_C_CHAR, columnData, sizeof(columnData), &columnDataLength);
                if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
                {
                    std::cerr << "Error in SQLGetData" << std::endl;
                    return;
                }

                if (columnDataLength == SQL_NULL_DATA)
                {
                    row[i] = "NULL";
                }
                else
                {
                    row[i] = std::string(reinterpret_cast<char*>(columnData));
                }

                int dataLength = row[i].length();
                columnWidths[i] = std::max(columnWidths[i], dataLength);
            }
            rowData.push_back(row);
        }
    }

    void DatabaseAccess::printDataRows(const std::vector<std::vector<std::string>>& rowData, const std::vector<int>& columnWidths)
    {
        for (const auto& row : rowData)
        {
            for (size_t i = 0; i < row.size(); i++)
            {
                std::cout << std::left << std::setw(columnWidths[i] + 2) << row[i] << "|";
            }
            std::cout << std::endl;
        }
    }

    void DatabaseAccess::displayQueryResults(SQLHSTMT hStmt)
    {
        std::vector<std::string> columnNames;
        std::vector<int> columnWidths;
        std::vector<std::vector<std::string>> rowData;

        getColumnInformation(hStmt, columnNames, columnWidths);
        storeAndCalculateColumnWidths(hStmt, columnWidths, rowData);
        printHeader(columnNames, columnWidths);
        printSeparator(columnWidths);
        printDataRows(rowData, columnWidths);
    }

    bool DatabaseAccess::addBus(types::BusId busId, const std::string& model, uint16_t year)
    {
        std::stringstream query;
        query << "INSERT INTO dbo.Bus (BusId, Model, Year) VALUES (" << busId << ", '" << model << "', " << year << ")";

        // Allocate a statement handle
        SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
        retcode = SQLExecDirect(hStmt, (SQLCHAR*)query.str().c_str(), SQL_NTS);

        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            handleDiagnosticRecord(hStmt, SQL_HANDLE_STMT);
            return false;
        }
        return true;
    }

    bool DatabaseAccess::deleteBus(types::BusId busId)
    {
        std::stringstream query;
        query << "DELETE FROM dbo.Bus WHERE BusId = " << busId;

        // Allocate a statement handle
        SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
        retcode = SQLExecDirect(hStmt, (SQLCHAR*)query.str().c_str(), SQL_NTS);

        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            handleDiagnosticRecord(hStmt, SQL_HANDLE_STMT);
            return false;
        }
        return true;
    }

    bool DatabaseAccess::addDriver(const std::string& name, const std::string& phoneNumber)
    {
        std::stringstream query;
        query << "INSERT INTO dbo.Driver (Name, PhoneNumber) VALUES ('" << name << "', '" << phoneNumber << "')";

        // Allocate a statement handle
        SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
        retcode = SQLExecDirect(hStmt, (SQLCHAR*)query.str().c_str(), SQL_NTS);

        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            handleDiagnosticRecord(hStmt, SQL_HANDLE_STMT);
            return false;
        }
        return true;
    }

    void DatabaseAccess::displayTrips(const std::string& location, const std::string& destination, const std::string& date)
    {

        if (location.empty() || destination.empty() || date.empty())
        {
            std::cout << "Invalid input -- Could not display trips\n";
            return;
        }

        std::stringstream query;
        query << "SELECT * FROM dbo.Trip T "
            << " JOIN TripOffering TOF on T.TripNumber = TOF.TripNumber "
            << " WHERE T.StartLocationName = '" << location << "'"
            << " AND T.DestinationName = '" << destination << "'"
            << " AND TOF.Date = '" << date << "'";

        // Allocate a statement handle
        SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
        retcode = SQLExecDirect(hStmt, (SQLCHAR*)query.str().c_str(), SQL_NTS);

        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
        {
            displayQueryResults(hStmt);
        }
        else
        {
            handleDiagnosticRecord(hStmt, SQL_HANDLE_STMT);
        }
    }

    void DatabaseAccess::displayBuses()
    {
        std::stringstream query;
        query << "SELECT * FROM dbo.Bus";

        // Allocate a statement handle
        SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
        retcode = SQLExecDirect(hStmt, (SQLCHAR*)query.str().c_str(), SQL_NTS);

        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
        {
            displayQueryResults(hStmt);
        }
        else
        {
            handleDiagnosticRecord(hStmt, SQL_HANDLE_STMT);
        }
    }

    void DatabaseAccess::displayWeeklyScheduleForDriver(const std::string& driverName, const std::string& date)
    {
        if (driverName.empty() || date.empty())
        {
            std::cout << "Invalid input -- Could not display weekly schedule for driver\n";
            return;
        }

        std::stringstream query;
        query << "Declare @GivenDate DATE = '" << date << "' "
            << "DECLARE @StartofWeek DATE = DATEADD(DAY, 1-DATEPART(WEEKDAY, @GivenDate), @GivenDate) "
            << "DECLARE @EndofWeek DATE = DATEADD(DAY, 7-DATEPART(WEEKDAY, @GivenDate), @GivenDate)"
            << "SELECT * FROM TripOffering TOF "
            << " JOIN Driver D on TOF.DriverName = D.DriverName "
            << " WHERE D.DriverName = '" << driverName << "'"
            << " AND TOF.Date>= @StartofWeek AND TOF.Date<= @EndofWeek";

        // Allocate a statement handle
        SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
        retcode = SQLExecDirect(hStmt, (SQLCHAR*)query.str().c_str(), SQL_NTS);

        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
        {
            displayQueryResults(hStmt);
        }
        else
        {
            handleDiagnosticRecord(hStmt, SQL_HANDLE_STMT);
        }
    }

    void DatabaseAccess::displayTripStopInfo(types::TripNumber tripNumber)
    {
        std::stringstream query;
        query << "SELECT T.*, TSI.StopNumber, TSI.SequenceNumber, TSI.DrivingTime FROM TripStopInfo TSI "
            << " JOIN Trip T on TSI.TripNumber = T.TripNumber "
            << " WHERE T.TripNumber = " << tripNumber;

        // Allocate a statement handle
        SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
        retcode = SQLExecDirect(hStmt, (SQLCHAR*)query.str().c_str(), SQL_NTS);

        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
        {
            displayQueryResults(hStmt);
        }
        else
        {
            handleDiagnosticRecord(hStmt, SQL_HANDLE_STMT);
        }
    }

}