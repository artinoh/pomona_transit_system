#include "../DatabaseAccess.h"
#include <sstream>

namespace transit::db
{
    DatabaseAccess::DatabaseAccess()
    {
        if (initialized) return;

        // Allocate an environment handle
        if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS) {
            handleDiagnosticRecord(hEnv, SQL_HANDLE_ENV);
            return;
        }
        SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);

        // Allocate a connection handle
        if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) != SQL_SUCCESS) {
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

        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
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

        std::cerr << "The driver reported the following diagnostics:\n";

        while ((ret = SQLGetDiagRec(handleType, handle, ++i, state, &native, text, sizeof(text), &len)) != SQL_NO_DATA) {
            std::cerr << "State: " << state << ", Native Error: " << native << ", Message: " << text << "\n";
        }
    }

    DatabaseAccess::~DatabaseAccess()
    {
        // Free the statement handle
        if (hStmt) SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        // Disconnect and free connection handle
        if (hDbc) {
            SQLDisconnect(hDbc);
            SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
        }
        // Free the environment handle
        if (hEnv) SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
    }

    void DatabaseAccess::displayQueryResults(SQLHSTMT hStmt)
    {
        SQLRETURN retcode;
        SQLSMALLINT columns;

        // Get the number of columns in the result set
        retcode = SQLNumResultCols(hStmt, &columns);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
            std::cerr << "Error in SQLNumResultCols" << std::endl;
            return;
        }

        // Iterate through each column to get column names
        for (SQLSMALLINT i = 1; i <= columns; i++) {
            SQLCHAR columnName[128];
            SQLSMALLINT columnNameLength;

            retcode = SQLDescribeCol(hStmt, i, columnName, sizeof(columnName), &columnNameLength, NULL, NULL, NULL, NULL);
            if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
                std::cerr << "Error in SQLDescribeCol" << std::endl;
                return;
            }

            std::cout << columnName << "\t";
        }
        std::cout << std::endl;

        // Fetch and display each row of the result set
        while (SQLFetch(hStmt) == SQL_SUCCESS) {
            for (SQLSMALLINT i = 1; i <= columns; i++) {
                SQLCHAR columnData[256];
                SQLLEN columnDataLength;

                retcode = SQLGetData(hStmt, i, SQL_C_CHAR, columnData, sizeof(columnData), &columnDataLength);
                if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
                    std::cerr << "Error in SQLGetData" << std::endl;
                    return;
                }

                if (columnDataLength == SQL_NULL_DATA) {
                    std::cout << "NULL\t";
                } else {
                    std::cout << columnData << "\t";
                }
            }
            std::cout << std::endl;
        }
    }

    bool DatabaseAccess::addBus(types::BusId busId, const std::string& model, uint16_t year)
    {
        std::stringstream query;
        query << "INSERT INTO dbo.Bus (BusId, Model, Year) VALUES (" << busId << ", '" << model << "', " << year << ")";

        // Allocate a statement handle
        SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
        retcode = SQLExecDirect(hStmt, (SQLCHAR*)query.str().c_str(), SQL_NTS);

        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
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

        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
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
        query   << "SELECT * FROM dbo.Trip T "
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





}