#include <iostream>

#include "Case.h"
#include "User.h"
#include "Officer.h"
#include "Evidence.h"
#include "Investigation.h"

using namespace std;

int main()
{
    cout << "================================================\n";
    cout << "       CYBER CRIME CASE MANAGEMENT SYSTEM\n";
    cout << "                C++ OOP MODULE\n";
    cout << "================================================\n";

    // ============================================
    // CASE
    // ============================================

    Case case1(
        105,
        "Rahul",
        "Phishing",
        4,
        "Under Investigation",
        "Amit Sharma"
    );

    cout << "\n\nCASE INFORMATION";

    case1.displayCase();

    // ============================================
    // USER
    // ============================================

    User user1(
        1,
        "Admin",
        "admin@cybercrime.com",
        "Administrator"
    );

    cout << "\n\nUSER INFORMATION";

    user1.displayUser();

    // ============================================
    // OFFICER
    // ============================================

    Officer officer1(
        101,
        "Amit Sharma",
        "amit@cybercrime.com",
        501,
        "Cyber Fraud",
        "Available"
    );

    cout << "\n\nOFFICER INFORMATION";

    officer1.displayUser();

    // ============================================
    // POLYMORPHISM
    // ============================================

    User* userPointer = &officer1;

    cout << "\n\nPOLYMORPHISM TEST";

    userPointer->displayUser();

    // ============================================
    // EVIDENCE
    // ============================================

    Evidence evidence1(
        1001,
        105,
        "Digital",
        "Phishing email screenshot",
        "Verified"
    );

    cout << "\n\nEVIDENCE INFORMATION";

    evidence1.displayEvidence();

    // ============================================
    // INVESTIGATION
    // ============================================

    Investigation investigation1(
        5001,
        105,
        501,
        "Collected phishing email evidence",
        "05-10-2026",
        "Evidence verified by investigating officer"
    );

    cout << "\n\nINVESTIGATION INFORMATION";

    investigation1.displayInvestigation();

    // ============================================
    // RELATIONSHIP
    // ============================================

    cout << "\n\n============================================\n";
    cout << "          CASE RELATIONSHIP\n";
    cout << "============================================\n";

    cout << "Case ID        : " << case1.getCaseId() << endl;
    cout << "Officer        : " << officer1.getName() << endl;
    cout << "Evidence ID    : " << evidence1.getEvidenceId() << endl;
    cout << "Investigation  : "
         << investigation1.getInvestigationId() << endl;

    cout << "\nAll modules working successfully!\n";

    return 0;
}