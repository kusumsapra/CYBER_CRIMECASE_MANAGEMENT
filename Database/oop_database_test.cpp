#include <iostream>

#include "../CPP_Module/Case.h"
#include "database.h"

using namespace std;

int main()
{
    cout << "================================================\n";
    cout << "       CYBER CRIME CASE MANAGEMENT SYSTEM\n";
    cout << "          OOP + MYSQL INTEGRATION\n";
    cout << "================================================\n";

    // Create C++ Case object
    Case case1(
        300,
        "Ankit Verma",
        "Cyber Fraud",
        5,
        "Open",
        "Amit Sharma"
    );

    cout << "\nC++ Case Object Created:\n";

    case1.displayCase();

    // Create database object
    Database database;

    if (!database.connect())
    {
        cout << "\nDatabase connection failed!"
             << endl;

        return 1;
    }

    // Save C++ object into MySQL
    cout << "\nSaving Case object to MySQL..."
         << endl;

    database.addCase(case1);

    // Display database records
    cout << "\nFetching cases from MySQL..."
         << endl;

    database.displayCases();

    database.close();

    cout << "\n================================================\n";
    cout << "        OOP + MYSQL TEST COMPLETED\n";
    cout << "================================================\n";

    return 0;
}