#include <iostream>
#include <cstdio>

extern "C"
{
#include "../C_Module/case.h"
#include "../C_Module/linked_list.h"
}

#include "database.h"

using namespace std;

int main()
{
    cout << "========================================================\n";
    cout << "       CYBER CRIME CASE MANAGEMENT SYSTEM\n";
    cout << "             DSA + MYSQL INTEGRATION TEST\n";
    cout << "========================================================\n";

    // ----------------------------------------------------
    // STEP 1: Connect to MySQL
    // ----------------------------------------------------

    Database database;

    if (!database.connect())
    {
        cout << "\nDatabase connection failed!" << endl;
        return 1;
    }

    cout << "\nDatabase connected successfully.\n";


    // ----------------------------------------------------
    // STEP 2: Display cases stored in MySQL
    // ----------------------------------------------------

    cout << "\n\n******** CASES STORED IN MYSQL ********\n";

    database.displayCases();


    // ----------------------------------------------------
    // STEP 3: Create C Linked List
    // ----------------------------------------------------

    cout << "\n\n******** C LINKED LIST ********\n";

    struct Node* head = NULL;


    // ----------------------------------------------------
    // STEP 4: Create C structures
    // ----------------------------------------------------

    struct Case case1;

    case1.caseId = 105;

    snprintf(
        case1.victimName,
        sizeof(case1.victimName),
        "%s",
        "Amit Sharma"
    );

    snprintf(
        case1.crimeType,
        sizeof(case1.crimeType),
        "%s",
        "Online Fraud"
    );

    case1.priority = 5;

    snprintf(
        case1.status,
        sizeof(case1.status),
        "%s",
        "Open"
    );

    snprintf(
        case1.officerName,
        sizeof(case1.officerName),
        "%s",
        "Amit Sharma"
    );


    // ----------------------------------------------------
    // CASE 2
    // ----------------------------------------------------

    struct Case case2;

    case2.caseId = 25;

    snprintf(
        case2.victimName,
        sizeof(case2.victimName),
        "%s",
        "Neha Singh"
    );

    snprintf(
        case2.crimeType,
        sizeof(case2.crimeType),
        "%s",
        "Identity Theft"
    );

    case2.priority = 3;

    snprintf(
        case2.status,
        sizeof(case2.status),
        "%s",
        "Under Investigation"
    );

    snprintf(
        case2.officerName,
        sizeof(case2.officerName),
        "%s",
        "Neha Singh"
    );


    // ----------------------------------------------------
    // CASE 3
    // ----------------------------------------------------

    struct Case case3;

    case3.caseId = 17;

    snprintf(
        case3.victimName,
        sizeof(case3.victimName),
        "%s",
        "Rahul Verma"
    );

    snprintf(
        case3.crimeType,
        sizeof(case3.crimeType),
        "%s",
        "Cyber Harassment"
    );

    case3.priority = 2;

    snprintf(
        case3.status,
        sizeof(case3.status),
        "%s",
        "Open"
    );

    snprintf(
        case3.officerName,
        sizeof(case3.officerName),
        "%s",
        "Amit Sharma"
    );


    // ----------------------------------------------------
    // STEP 5: Insert cases into Linked List
    // ----------------------------------------------------

    cout << "\nLoading cases into Linked List...\n\n";

    addCase(&head, case1);

    addCase(&head, case2);

    addCase(&head, case3);


    // ----------------------------------------------------
    // STEP 6: Display Linked List
    // ----------------------------------------------------

    cout << "\n\n******** LINKED LIST DATA ********\n";

    displayCases(head);


    // ----------------------------------------------------
    // STEP 7: Search Case
    // ----------------------------------------------------

    cout << "\n\n******** SEARCH CASE ********\n";

    int searchId = 25;

    cout << "\nSearching Case ID: "
         << searchId
         << endl;

    struct Node* found = searchCase(head, searchId);

    if (found != NULL)
    {
        cout << "\nCase found successfully!\n";

        cout << "\nCase ID     : "
             << found->data.caseId;

        cout << "\nVictim Name : "
             << found->data.victimName;

        cout << "\nCrime Type  : "
             << found->data.crimeType;

        cout << "\nPriority    : "
             << found->data.priority;

        cout << "\nStatus      : "
             << found->data.status;

        cout << "\nOfficer     : "
             << found->data.officerName
             << "\n";
    }
    else
    {
        cout << "\nCase not found.\n";
    }


    // ----------------------------------------------------
    // STEP 8: Update Case Status
    // ----------------------------------------------------

    cout << "\n\n******** UPDATE CASE STATUS ********\n";

    cout << "\nUpdating Case ID 25...\n";

    updateCaseStatus(head, 25);


    // ----------------------------------------------------
    // STEP 9: Display after update
    // ----------------------------------------------------

    cout << "\n\n******** LINKED LIST AFTER UPDATE ********\n";

    displayCases(head);


    // ----------------------------------------------------
    // STEP 10: Delete Case
    // ----------------------------------------------------

    cout << "\n\n******** DELETE CASE ********\n";

    cout << "\nDeleting Case ID 17...\n";

    deleteCase(&head, 17);


    // ----------------------------------------------------
    // STEP 11: Display after deletion
    // ----------------------------------------------------

    cout << "\n\n******** LINKED LIST AFTER DELETION ********\n";

    displayCases(head);


    // ----------------------------------------------------
    // STEP 12: Close Database
    // ----------------------------------------------------

    database.close();


    cout << "\n========================================================\n";
    cout << "       DSA + MYSQL INTEGRATION TEST COMPLETE\n";
    cout << "========================================================\n";

    return 0;
}