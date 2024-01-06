#pragma once

#include <iostream>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <string>
#include "types.h"

namespace transit::db
{
class DatabaseAccess
{
public:

    static DatabaseAccess& instance() {
        static DatabaseAccess instance;
        return instance;
    }

    bool addBus(types::BusId busId, const std::string& model, uint16_t year);
    bool addDriver(const std::string& name, const std::string& phoneNumber);

    void displayTrips(const std::string& location, const std::string& destination, const std::string& date);

private:
    DatabaseAccess();
    ~DatabaseAccess();
    DatabaseAccess(const DatabaseAccess&) = delete;
    DatabaseAccess& operator=(const DatabaseAccess&) = delete;

    void handleDiagnosticRecord(SQLHANDLE handle, SQLSMALLINT handleType);
    void displayQueryResults(SQLHANDLE handle);


    SQLHENV hEnv;        // Environment handle
    SQLHDBC hDbc;        // Connection handle
    SQLHSTMT hStmt;      // Statement handle
    SQLRETURN retcode;   // Return code
    bool initialized = false;

};
}

