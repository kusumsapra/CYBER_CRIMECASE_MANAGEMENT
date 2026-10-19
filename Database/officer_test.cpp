#include <iostream>

#include "../CPP_Module/Officer.h"
#include "../CPP_Module/Case.h"

#include "database.h"

using namespace std;

int main()
{
    cout << "========================================================\n";
    cout << "       CYBER CRIME CASE MANAGEMENT SYSTEM\n";
    cout << "              OFFICER MANAGEMENT TEST\n";
    cout << "========================================================\n";

    Database database;

    // ========================================================
    // CONNECT DATABASE
    // ========================================================

    if (!database.connect())
    {
        cout << "\nDatabase connection failed!"
             << endl;

        return 1;
    }

    // ========================================================
    // CREATE OFFICER OBJECT
    // ========================================================

    Officer officer1(
        10,
        "Rahul Verma",
        "rahul@cybercrime.com",
        504,
        "Digital Forensics",
        "Available"
    );

    cout << "\n\n******** OFFICER OBJECT ********\n";

    officer1.displayUser();

    // ========================================================
    // ADD OFFICER
    // ========================================================

    cout << "\nAdding Officer to MySQL...\n";

    database.addOfficer(
        officer1
    );

    // ========================================================
    // DISPLAY OFFICERS
    // ========================================================

    cout << "\n\nDisplaying All Officers...\n";

    database.displayOfficers();

    // ========================================================
    // SEARCH OFFICER
    // ========================================================

    cout << "\n\nSearching Officer 504...\n";

    database.searchOfficer(
        504
    );

    // ========================================================
    // UPDATE AVAILABILITY
    // ========================================================

    cout << "\n\nUpdating Officer Availability...\n";

    database.updateOfficerAvailability(
        504,
        "Busy"
    );

    cout << "\nChecking Updated Officer...\n";

    database.searchOfficer(
        504
    );

    // ========================================================
    // CREATE CASE
    // ========================================================

    cout << "\n\n******** CASE ASSIGNMENT ********\n";

    Case case1(
        401,
        "Priya Singh",
        "Identity Theft",
        4,
        "Open",
        "Rahul Verma"
    );

    cout << "\nCreating Case:\n";

    case1.displayCase();

    // ========================================================
    // ADD CASE
    // ========================================================

    cout << "\nAdding Case to MySQL...\n";

    database.addCase(
        case1
    );

    // ========================================================
    // ASSIGN OFFICER
    // ========================================================

    cout << "\nAssigning Officer 504 to Case 401...\n";

    database.assignOfficerToCase(
        401,
        504
    );

    // ========================================================
    // DISPLAY CASE + OFFICER
    // ========================================================

    cout << "\n\nDisplaying Case with Officer...\n";

    database.displayCaseWithOfficer(
        401
    );

    // ========================================================
    // CLOSE DATABASE
    // ========================================================

    database.close();

    cout << "\n========================================================\n";
    cout << "          OFFICER MANAGEMENT TEST COMPLETE\n";
    cout << "========================================================\n";

    return 0;
}