#pragma once

#include <iostream>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <string>
#include "types.h"

namespace db
{
class DatabaseAccess
{
public:

    static DatabaseAccess& instance() {
        static DatabaseAccess instance;
        return instance;
    }

    void query();

private:
    DatabaseAccess();
    ~DatabaseAccess();
    DatabaseAccess(const DatabaseAccess&) = delete;
    DatabaseAccess& operator=(const DatabaseAccess&) = delete;

    void handleDiagnosticRecord(SQLHANDLE handle, SQLSMALLINT handleType);


    SQLHENV hEnv;        // Environment handle
    SQLHDBC hDbc;        // Connection handle
    SQLHSTMT hStmt;      // Statement handle
    SQLRETURN retcode;   // Return code
    bool initialized = false;

};
}

