#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <mysqlx/xdevapi.h>

#include "../CPP_Module/Case.h"
#include "../CPP_Module/Evidence.h"
#include "../CPP_Module/Investigation.h"
#include "../CPP_Module/Officer.h"

class Database
{
private:
    mysqlx::Session* session;

public:

    Database();

    bool connect();
    void close();

    // ========================================================
    // CASE
    // ========================================================

    bool addCase(const Case& caseObject);

    void displayCases();

    bool updateCaseStatus(
        int caseId,
        std::string newStatus
    );

    bool deleteCase(int caseId);

    bool searchCase(int caseId);

    bool assignOfficerToCase(
        int caseId,
        int officerId
    );

    void displayCaseWithOfficer(
        int caseId
    );

    // ========================================================
    // OFFICER
    // ========================================================

    bool addOfficer(
        const Officer& officerObject
    );

    void displayOfficers();

    bool searchOfficer(
        int officerId
    );

    bool updateOfficerAvailability(
        int officerId,
        std::string newAvailability
    );

    bool deleteOfficer(
        int officerId
    );

    // ========================================================
    // EVIDENCE
    // ========================================================

    bool addEvidence(
        const Evidence& evidenceObject
    );

    void displayEvidence();

    void displayEvidenceByCase(
        int caseId
    );

    bool updateEvidenceStatus(
        int evidenceId,
        std::string newStatus
    );

    bool deleteEvidence(
        int evidenceId
    );

    // ========================================================
    // INVESTIGATION
    // ========================================================

    bool addInvestigation(
        const Investigation& investigationObject
    );

    void displayInvestigations();

    void displayInvestigationsByCase(
        int caseId
    );

    bool deleteInvestigation(
        int investigationId
    );

    // ========================================================
    // REPORTS
    // ========================================================

    bool addReport(
        int caseId,
        std::string reportType,
        std::string description
    );

    void displayReports();

    void displayReportsByCase(
        int caseId
    );

    bool deleteReport(
        int reportId
    );
};

#endif