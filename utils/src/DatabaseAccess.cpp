#include "../DatabaseAccess.h"

namespace db
{
    void DatabaseAccess::query()
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
}