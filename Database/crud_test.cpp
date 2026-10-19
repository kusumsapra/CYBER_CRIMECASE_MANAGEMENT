#include <iostream>
#include "database.h"

using namespace std;

int main()
{
    cout << "================================================\n";
    cout << "       CYBER CRIME CASE MANAGEMENT SYSTEM\n";
    cout << "                 MYSQL CRUD TEST\n";
    cout << "================================================\n";

    Database database;

    if (!database.connect())
    {
        cout << "\nDatabase connection failed!"
             << endl;

        return 1;
    }

    /* ================= CREATE ================= */

    cout << "\n\n[1] CREATE - Adding Case\n";

    database.addCase(
        200,
        "Rohit Sharma",
        "Online Banking Fraud",
        5,
        "Open",
        1
    );

    /* ================= READ ================= */

    cout << "\n\n[2] READ - Displaying Cases\n";

    database.displayCases();

    /* ================= SEARCH ================= */

    cout << "\n\n[3] SEARCH - Searching Case 200\n";

    database.searchCase(200);

    /* ================= UPDATE ================= */

    cout << "\n\n[4] UPDATE - Updating Case 200\n";

    database.updateCaseStatus(
        200,
        "Under Investigation"
    );

    cout << "\nChecking updated case:\n";

    database.searchCase(200);

    /* ================= DELETE ================= */

    cout << "\n\n[5] DELETE - Deleting Case 200\n";

    database.deleteCase(200);

    cout << "\nChecking after deletion:\n";

    database.searchCase(200);

    /* ================= CLOSE ================= */

    database.close();

    cout << "\n================================================\n";
    cout << "             CRUD TEST COMPLETED\n";
    cout << "================================================\n";

    return 0;
}