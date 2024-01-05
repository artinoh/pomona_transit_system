#include <iostream>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <string>

namespace db
{
class DatabaseAccess
{
public:

    static DatabaseAccess& instance() {
        static DatabaseAccess instance;
        if (!instance.initialized) {
            instance.init();
        }
        return instance;
    }

    void query()
    {
        // Allocate a statement handle
        SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

        // Execute the query
        retcode = SQLExecDirect(hStmt, (SQLCHAR*)"SELECT * FROM dbo.Trip", SQL_NTS);


        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
        {
            int trip_number;
            char start_location[255] = { 0 };
            char destination_name[255] = { 0 };
            // Bind columns 1, 2, and 3
            SQLBindCol(hStmt, 1, SQL_C_LONG, &trip_number, 0, NULL);
            SQLBindCol(hStmt, 2, SQL_C_CHAR, &start_location, 255, NULL);
            SQLBindCol(hStmt, 3, SQL_C_CHAR, &destination_name, 255, NULL);


            while (SQLFetch(hStmt) == SQL_SUCCESS)
            {
                std::cout << "Trip number: " << trip_number << " Start location: " << start_location << " Destination name: " << destination_name << "\n";
            }
        }
        else
        {
            handleDiagnosticRecord(hStmt, SQL_HANDLE_STMT);
        }
    }


private:

    void init()
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
            return;
        }

        std::cout << "Connection successful\n";
        initialized = true;
    }

    void handleDiagnosticRecord(SQLHANDLE handle, SQLSMALLINT handleType)
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

    ~DatabaseAccess() {
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

    DatabaseAccess() = default ;
    DatabaseAccess(const DatabaseAccess&) = delete;
    DatabaseAccess& operator=(const DatabaseAccess&) = delete;

    SQLHENV hEnv;        // Environment handle
    SQLHDBC hDbc;        // Connection handle
    SQLHSTMT hStmt;      // Statement handle
    SQLRETURN retcode;   // Return code
    bool initialized = false;

};
}

