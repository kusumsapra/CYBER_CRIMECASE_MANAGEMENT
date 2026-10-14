#include <iostream>

#include "../CPP_Module/Case.h"
#include "../CPP_Module/Evidence.h"
#include "../CPP_Module/Investigation.h"
#include "database.h"

using namespace std;

int main()
{
    cout << "========================================================\n";
    cout << "       CYBER CRIME CASE MANAGEMENT SYSTEM\n";
    cout << "              COMPLETE DATABASE TEST\n";
    cout << "========================================================\n";

    Database database;

    // ========================================================
    // DATABASE CONNECTION
    // ========================================================

    if (!database.connect())
    {
        cout << "\nDatabase connection failed!"
             << endl;

        return 1;
    }

    // ========================================================
    // CASE MODULE
    // ========================================================

    cout << "\n\n******** CASE MODULE ********\n";

    Case case1(
        400,
        "Vikas Sharma",
        "Online Banking Fraud",
        5,
        "Open",
        "Amit Sharma"
    );

    cout << "\nCreating Case object:\n";

    case1.displayCase();

    cout << "\nSaving Case to MySQL...\n";

    database.addCase(case1);

    cout << "\nSearching Case...\n";

    database.searchCase(400);

    cout << "\nUpdating Case Status...\n";

    database.updateCaseStatus(
        400,
        "Under Investigation"
    );

    cout << "\nSearching Updated Case...\n";

    database.searchCase(400);

    // ========================================================
    // EVIDENCE MODULE
    // ========================================================

    cout << "\n\n******** EVIDENCE MODULE ********\n";

    Evidence evidence1(
        5001,
        400,
        "Digital",
        "Bank transaction screenshot",
        "Collected"
    );

    cout << "\nCreating Evidence object:\n";

    evidence1.displayEvidence();

    cout << "\nSaving Evidence to MySQL...\n";

    database.addEvidence(evidence1);

    cout << "\nDisplaying Evidence:\n";

    database.displayEvidence();

    cout << "\nEvidence belonging to Case 400:\n";

    database.displayEvidenceByCase(400);

    cout << "\nUpdating Evidence Status...\n";

    database.updateEvidenceStatus(
        5001,
        "Verified"
    );

    // ========================================================
    // INVESTIGATION MODULE
    // ========================================================

    cout << "\n\n******** INVESTIGATION MODULE ********\n";

    Investigation investigation1(
        6001,
        400,
        1,
        "Verified suspicious bank transaction",
        "2026-10-06",
        "Transaction confirmed as fraudulent"
    );

    cout << "\nCreating Investigation object:\n";

    investigation1.displayInvestigation();

    cout << "\nSaving Investigation to MySQL...\n";

    database.addInvestigation(
        investigation1
    );

    cout << "\nDisplaying Investigation History:\n";

    database.displayInvestigations();

    cout << "\nInvestigation belonging to Case 400:\n";

    database.displayInvestigationsByCase(400);

    // ========================================================
    // REPORT MODULE
    // ========================================================

    cout << "\n\n******** REPORT MODULE ********\n";

    cout << "\nCreating Case Report...\n";

    database.addReport(
        400,
        "Investigation Report",
        "Case investigation completed. "
        "Evidence verified and suspicious transaction confirmed."
    );

    cout << "\nDisplaying Reports:\n";

    database.displayReports();

    cout << "\nReports belonging to Case 400:\n";

    database.displayReportsByCase(400);

    // ========================================================
    // DISPLAY ALL CASES
    // ========================================================

    cout << "\n\n******** ALL CASES ********\n";

    database.displayCases();

    // ========================================================
    // CLOSE DATABASE
    // ========================================================

    database.close();

    cout << "\n========================================================\n";
    cout << "          COMPLETE DATABASE TEST SUCCESSFUL\n";
    cout << "========================================================\n";

    return 0;
}