#include <iostream>

#include "database.h"

using namespace std;

// ============================================================
// CONSTRUCTOR
// ============================================================

Database::Database()
{
    session = nullptr;
}

// ============================================================
// CONNECT
// ============================================================

bool Database::connect()
{
    try
    {
        session = new mysqlx::Session(
            "localhost",
            33060,
            "root",
            "1234"
        );

        mysqlx::Schema db =
            session->getSchema("cyber_crime_db");

        cout << "\nConnected to MySQL successfully!"
             << endl;

        cout << "Database: "
             << db.getName()
             << endl;

        return true;
    }
    catch (const mysqlx::Error& error)
    {
        cout << "\nMySQL Error: "
             << error.what()
             << endl;

        return false;
    }
}

// ============================================================
// CLOSE
// ============================================================

void Database::close()
{
    if (session != nullptr)
    {
        session->close();

        delete session;

        session = nullptr;

        cout << "\nDatabase connection closed."
             << endl;
    }
}

// ============================================================
// CASE - CREATE
// ============================================================

bool Database::addCase(
    const Case& caseObject
)
{
    try
    {
        if (session == nullptr)
        {
            cout << "\nDatabase is not connected!"
                 << endl;

            return false;
        }

        mysqlx::Schema db =
            session->getSchema("cyber_crime_db");

        mysqlx::Table cases =
            db.getTable("cases");

        cases.insert(
            "case_id",
            "victim_name",
            "crime_type",
            "priority",
            "status",
            "officer_id"
        )
        .values(
            caseObject.getCaseId(),
            caseObject.getVictimName(),
            caseObject.getCrimeType(),
            caseObject.getPriority(),
            caseObject.getStatus(),
            1
        )
        .execute();

        cout << "\nCase saved to MySQL successfully!"
             << endl;

        return true;
    }
    catch (const mysqlx::Error& error)
    {
        cout << "\nMySQL Error: "
             << error.what()
             << endl;

        return false;
    }
}

// ============================================================
// CASE - READ
// ============================================================

void Database::displayCases()
{
    try
    {
        if (session == nullptr)
        {
            cout << "\nDatabase is not connected!"
                 << endl;

            return;
        }

        mysqlx::Schema db =
            session->getSchema("cyber_crime_db");

        mysqlx::Table cases =
            db.getTable("cases");

        mysqlx::RowResult result =
            cases.select(
                "case_id",
                "victim_name",
                "crime_type",
                "priority",
                "status",
                "officer_id"
            )
            .execute();

        cout << "\n============================================================\n";
        cout << "                    ALL CYBER CRIME CASES\n";
        cout << "============================================================\n";

        mysqlx::Row row;

        while ((row = result.fetchOne()))
        {
            cout << "\nCase ID      : "
                 << row[0].get<int>()
                 << endl;

            cout << "Victim Name  : "
                 << row[1].get<string>()
                 << endl;

            cout << "Crime Type   : "
                 << row[2].get<string>()
                 << endl;

            cout << "Priority     : "
                 << row[3].get<int>()
                 << endl;

            cout << "Status       : "
                 << row[4].get<string>()
                 << endl;

            if (row[5].isNull())
            {
                cout << "Officer ID   : Not Assigned"
                     << endl;
            }
            else
            {
                cout << "Officer ID   : "
                     << row[5].get<int>()
                     << endl;
            }

            cout << "------------------------------------------------------------\n";
        }
    }
    catch (const mysqlx::Error& error)
    {
        cout << "\nMySQL Error: "
             << error.what()
             << endl;
    }
}

// ============================================================
// CASE - UPDATE STATUS
// ============================================================

bool Database::updateCaseStatus(
    int caseId,
    string newStatus
)
{
    try
    {
        if (session == nullptr)
        {
            cout << "\nDatabase is not connected!"
                 << endl;

            return false;
        }

        mysqlx::Schema db =
            session->getSchema("cyber_crime_db");

        mysqlx::Table cases =
            db.getTable("cases");

        mysqlx::Result result =
            cases.update()
            .set("status", newStatus)
            .where("case_id = :id")
            .bind("id", caseId)
            .execute();

        if (result.getAffectedItemsCount() == 0)
        {
            cout << "\nCase not found!"
                 << endl;

            return false;
        }

        cout << "\nCase status updated successfully!"
             << endl;

        return true;
    }
    catch (const mysqlx::Error& error)
    {
        cout << "\nMySQL Error: "
             << error.what()
             << endl;

        return false;
    }
}

// ============================================================
// CASE - DELETE
// ============================================================

bool Database::deleteCase(
    int caseId
)
{
    try
    {
        if (session == nullptr)
        {
            cout << "\nDatabase is not connected!"
                 << endl;

            return false;
        }

        mysqlx::Schema db =
            session->getSchema("cyber_crime_db");

        mysqlx::Table cases =
            db.getTable("cases");

        mysqlx::Result result =
            cases.remove()
            .where("case_id = :id")
            .bind("id", caseId)
            .execute();

        if (result.getAffectedItemsCount() == 0)
        {
            cout << "\nCase not found!"
                 << endl;

            return false;
        }

        cout << "\nCase deleted successfully!"
             << endl;

        return true;
    }
    catch (const mysqlx::Error& error)
    {
        cout << "\nMySQL Error: "
             << error.what()
             << endl;

        return false;
    }
}

// ============================================================
// CASE - SEARCH
// ============================================================

bool Database::searchCase(
    int caseId
)
{
    try
    {
        if (session == nullptr)
        {
            cout << "\nDatabase is not connected!"
                 << endl;

            return false;
        }

        mysqlx::Schema db =
            session->getSchema("cyber_crime_db");

        mysqlx::Table cases =
            db.getTable("cases");

        mysqlx::RowResult result =
            cases.select(
                "case_id",
                "victim_name",
                "crime_type",
                "priority",
                "status",
                "officer_id"
            )
            .where("case_id = :id")
            .bind("id", caseId)
            .execute();

        mysqlx::Row row =
            result.fetchOne();

        if (row.isNull())
        {
            cout << "\nCase not found!"
                 << endl;

            return false;
        }

        cout << "\n====================================\n";
        cout << "             CASE FOUND\n";
        cout << "====================================\n";

        cout << "Case ID      : "
             << row[0].get<int>()
             << endl;

        cout << "Victim Name  : "
             << row[1].get<string>()
             << endl;

        cout << "Crime Type   : "
             << row[2].get<string>()
             << endl;

        cout << "Priority     : "
             << row[3].get<int>()
             << endl;

        cout << "Status       : "
             << row[4].get<string>()
             << endl;

        if (row[5].isNull())
        {
            cout << "Officer ID   : Not Assigned"
                 << endl;
        }
        else
        {
            cout << "Officer ID   : "
                 << row[5].get<int>()
                 << endl;
        }

        cout << "====================================\n";

        return true;
    }
    catch (const mysqlx::Error& error)
    {
        cout << "\nMySQL Error: "
             << error.what()
             << endl;

        return false;
    }
}

// ============================================================
// CASE - ASSIGN OFFICER
// ============================================================

bool Database::assignOfficerToCase(
    int caseId,
    int officerId
)
{
    try
    {
        if (session == nullptr)
        {
            cout << "\nDatabase is not connected!"
                 << endl;

            return false;
        }

        mysqlx::Schema db =
            session->getSchema("cyber_crime_db");

        mysqlx::Table officers =
            db.getTable("officers");

        mysqlx::RowResult officerResult =
            officers.select(
                "officer_id"
            )
            .where("officer_id = :id")
            .bind("id", officerId)
            .execute();

        mysqlx::Row officerRow =
            officerResult.fetchOne();

        if (officerRow.isNull())
        {
            cout << "\nOfficer not found!"
                 << endl;

            return false;
        }

        mysqlx::Table cases =
            db.getTable("cases");

        mysqlx::Result result =
            cases.update()
            .set("officer_id", officerId)
            .where("case_id = :id")
            .bind("id", caseId)
            .execute();

        if (result.getAffectedItemsCount() == 0)
        {
            cout << "\nCase not found!"
                 << endl;

            return false;
        }

        cout << "\nOfficer assigned to case successfully!"
             << endl;

        return true;
    }
    catch (const mysqlx::Error& error)
    {
        cout << "\nMySQL Error: "
             << error.what()
             << endl;

        return false;
    }
}

// ============================================================
// CASE - DISPLAY CASE WITH OFFICER
// ============================================================

void Database::displayCaseWithOfficer(
    int caseId
)
{
    try
    {
        if (session == nullptr)
        {
            cout << "\nDatabase is not connected!"
                 << endl;

            return;
        }

        mysqlx::Schema db =
            session->getSchema("cyber_crime_db");

        mysqlx::Table cases =
            db.getTable("cases");

        mysqlx::RowResult result =
            cases.select(
                "case_id",
                "victim_name",
                "crime_type",
                "priority",
                "status",
                "officer_id"
            )
            .where("case_id = :id")
            .bind("id", caseId)
            .execute();

        mysqlx::Row row =
            result.fetchOne();

        if (row.isNull())
        {
            cout << "\nCase not found!"
                 << endl;

            return;
        }

        cout << "\n==============================================\n";
        cout << "           CASE + OFFICER DETAILS\n";
        cout << "==============================================\n";

        cout << "Case ID     : "
             << row[0].get<int>()
             << endl;

        cout << "Victim      : "
             << row[1].get<string>()
             << endl;

        cout << "Crime       : "
             << row[2].get<string>()
             << endl;

        cout << "Priority    : "
             << row[3].get<int>()
             << endl;

        cout << "Status      : "
             << row[4].get<string>()
             << endl;

        if (row[5].isNull())
        {
            cout << "Officer     : Not Assigned"
                 << endl;
        }
        else
        {
            int officerId =
                row[5].get<int>();

            mysqlx::Table officers =
                db.getTable("officers");

            mysqlx::RowResult officerResult =
                officers.select(
                    "officer_id",
                    "user_id",
                    "specialization",
                    "availability"
                )
                .where("officer_id = :id")
                .bind("id", officerId)
                .execute();

            mysqlx::Row officerRow =
                officerResult.fetchOne();

            if (!officerRow.isNull())
            {
                cout << "\nOfficer ID       : "
                     << officerRow[0].get<int>()
                     << endl;

                cout << "User ID          : "
                     << officerRow[1].get<int>()
                     << endl;

                cout << "Specialization   : "
                     << officerRow[2].get<string>()
                     << endl;

                cout << "Availability     : "
                     << officerRow[3].get<string>()
                     << endl;
            }
        }

        cout << "==============================================\n";
    }
    catch (const mysqlx::Error& error)
    {
        cout << "\nMySQL Error: "
             << error.what()
             << endl;
    }
}

// ============================================================
// OFFICER - CREATE
// ============================================================

bool Database::addOfficer(
    const Officer& officerObject
)
{
    try
    {
        if (session == nullptr)
        {
            cout << "\nDatabase is not connected!"
                 << endl;

            return false;
        }

        mysqlx::Schema db =
            session->getSchema("cyber_crime_db");

        // ----------------------------------------------------
        // First create USER
        // ----------------------------------------------------

        mysqlx::Table users =
            db.getTable("users");

        users.insert(
            "user_id",
            "name",
            "email",
            "password",
            "role"
        )
        .values(
            officerObject.getUserId(),
            officerObject.getName(),
            officerObject.getEmail(),
            "temporary_password",
            "Officer"
        )
        .execute();

        // ----------------------------------------------------
        // Then create OFFICER
        // ----------------------------------------------------

        mysqlx::Table officers =
            db.getTable("officers");

        officers.insert(
            "officer_id",
            "user_id",
            "specialization",
            "availability"
        )
        .values(
            officerObject.getOfficerId(),
            officerObject.getUserId(),
            officerObject.getSpecialization(),
            officerObject.getAvailability()
        )
        .execute();

        cout << "\nOfficer added successfully!"
             << endl;

        return true;
    }
    catch (const mysqlx::Error& error)
    {
        cout << "\nMySQL Error: "
             << error.what()
             << endl;

        return false;
    }
}

// ============================================================
// OFFICER - DISPLAY
// ============================================================

void Database::displayOfficers()
{
    try
    {
        if (session == nullptr)
        {
            cout << "\nDatabase is not connected!"
                 << endl;

            return;
        }

        mysqlx::Schema db =
            session->getSchema("cyber_crime_db");

        mysqlx::Table officers =
            db.getTable("officers");

        mysqlx::RowResult result =
            officers.select(
                "officer_id",
                "user_id",
                "specialization",
                "availability"
            )
            .execute();

        cout << "\n============================================================\n";
        cout << "                     ALL OFFICERS\n";
        cout << "============================================================\n";

        mysqlx::Row row;

        while ((row = result.fetchOne()))
        {
            cout << "\nOfficer ID     : "
                 << row[0].get<int>()
                 << endl;

            cout << "User ID        : "
                 << row[1].get<int>()
                 << endl;

            cout << "Specialization : "
                 << row[2].get<string>()
                 << endl;

            cout << "Availability   : "
                 << row[3].get<string>()
                 << endl;

            cout << "------------------------------------------------------------\n";
        }
    }
    catch (const mysqlx::Error& error)
    {
        cout << "\nMySQL Error: "
             << error.what()
             << endl;
    }
}

// ============================================================
// OFFICER - SEARCH
// ============================================================

bool Database::searchOfficer(
    int officerId
)
{
    try
    {
        mysqlx::Schema db =
            session->getSchema("cyber_crime_db");

        mysqlx::Table officers =
            db.getTable("officers");

        mysqlx::RowResult result =
            officers.select(
                "officer_id",
                "user_id",
                "specialization",
                "availability"
            )
            .where("officer_id = :id")
            .bind("id", officerId)
            .execute();

        mysqlx::Row row =
            result.fetchOne();

        if (row.isNull())
        {
            cout << "\nOfficer not found!"
                 << endl;

            return false;
        }

        cout << "\n====================================\n";
        cout << "          OFFICER FOUND\n";
        cout << "====================================\n";

        cout << "Officer ID     : "
             << row[0].get<int>()
             << endl;

        cout << "User ID        : "
             << row[1].get<int>()
             << endl;

        cout << "Specialization : "
             << row[2].get<string>()
             << endl;

        cout << "Availability   : "
             << row[3].get<string>()
             << endl;

        cout << "====================================\n";

        return true;
    }
    catch (const mysqlx::Error& error)
    {
        cout << "\nMySQL Error: "
             << error.what()
             << endl;

        return false;
    }
}

// ============================================================
// OFFICER - UPDATE AVAILABILITY
// ============================================================

bool Database::updateOfficerAvailability(
    int officerId,
    string newAvailability
)
{
    try
    {
        mysqlx::Schema db =
            session->getSchema("cyber_crime_db");

        mysqlx::Table officers =
            db.getTable("officers");

        mysqlx::Result result =
            officers.update()
            .set("availability", newAvailability)
            .where("officer_id = :id")
            .bind("id", officerId)
            .execute();

        if (result.getAffectedItemsCount() == 0)
        {
            cout << "\nOfficer not found!"
                 << endl;

            return false;
        }

        cout << "\nOfficer availability updated!"
             << endl;

        return true;
    }
    catch (const mysqlx::Error& error)
    {
        cout << "\nMySQL Error: "
             << error.what()
             << endl;

        return false;
    }
}

// ============================================================
// OFFICER - DELETE
// ============================================================

bool Database::deleteOfficer(
    int officerId
)
{
    try
    {
        mysqlx::Schema db =
            session->getSchema("cyber_crime_db");

        mysqlx::Table officers =
            db.getTable("officers");

        mysqlx::RowResult result =
            officers.select(
                "user_id"
            )
            .where("officer_id = :id")
            .bind("id", officerId)
            .execute();

        mysqlx::Row row =
            result.fetchOne();

        if (row.isNull())
        {
            cout << "\nOfficer not found!"
                 << endl;

            return false;
        }

        int userId =
            row[0].get<int>();

        officers.remove()
        .where("officer_id = :id")
        .bind("id", officerId)
        .execute();

        mysqlx::Table users =
            db.getTable("users");

        users.remove()
        .where("user_id = :id")
        .bind("id", userId)
        .execute();

        cout << "\nOfficer deleted successfully!"
             << endl;

        return true;
    }
    catch (const mysqlx::Error& error)
    {
        cout << "\nMySQL Error: "
             << error.what()
             << endl;

        return false;
    }
}

// ============================================================
// EVIDENCE - CREATE
// ============================================================

bool Database::addEvidence(
    const Evidence& evidenceObject
)
{
    try
    {
        mysqlx::Schema db =
            session->getSchema("cyber_crime_db");

        mysqlx::Table evidence =
            db.getTable("evidence");

        evidence.insert(
            "evidence_id",
            "case_id",
            "evidence_type",
            "description",
            "status"
        )
        .values(
            evidenceObject.getEvidenceId(),
            evidenceObject.getCaseId(),
            evidenceObject.getEvidenceType(),
            evidenceObject.getDescription(),
            evidenceObject.getStatus()
        )
        .execute();

        cout << "\nEvidence saved to MySQL successfully!"
             << endl;

        return true;
    }
    catch (const mysqlx::Error& error)
    {
        cout << "\nMySQL Error: "
             << error.what()
             << endl;

        return false;
    }
}

// ============================================================
// EVIDENCE - READ
// ============================================================

void Database::displayEvidence()
{
    try
    {
        mysqlx::Schema db =
            session->getSchema("cyber_crime_db");

        mysqlx::Table evidence =
            db.getTable("evidence");

        mysqlx::RowResult result =
            evidence.select(
                "evidence_id",
                "case_id",
                "evidence_type",
                "description",
                "status"
            )
            .execute();

        cout << "\n============================================================\n";
        cout << "                     ALL EVIDENCE\n";
        cout << "============================================================\n";

        mysqlx::Row row;

        while ((row = result.fetchOne()))
        {
            cout << "\nEvidence ID  : "
                 << row[0].get<int>()
                 << endl;

            cout << "Case ID      : "
                 << row[1].get<int>()
                 << endl;

            cout << "Type         : "
                 << row[2].get<string>()
                 << endl;

            cout << "Description  : "
                 << row[3].get<string>()
                 << endl;

            cout << "Status       : "
                 << row[4].get<string>()
                 << endl;

            cout << "------------------------------------------------------------\n";
        }
    }
    catch (const mysqlx::Error& error)
    {
        cout << "\nMySQL Error: "
             << error.what()
             << endl;
    }
}

// ============================================================
// EVIDENCE - BY CASE
// ============================================================

void Database::displayEvidenceByCase(
    int caseId
)
{
    try
    {
        mysqlx::Schema db =
            session->getSchema("cyber_crime_db");

        mysqlx::Table evidence =
            db.getTable("evidence");

        mysqlx::RowResult result =
            evidence.select(
                "evidence_id",
                "case_id",
                "evidence_type",
                "description",
                "status"
            )
            .where("case_id = :id")
            .bind("id", caseId)
            .execute();

        cout << "\n===== EVIDENCE FOR CASE "
             << caseId
             << " =====\n";

        mysqlx::Row row;

        while ((row = result.fetchOne()))
        {
            cout << "\nEvidence ID : "
                 << row[0].get<int>()
                 << endl;

            cout << "Type        : "
                 << row[2].get<string>()
                 << endl;

            cout << "Description : "
                 << row[3].get<string>()
                 << endl;

            cout << "Status      : "
                 << row[4].get<string>()
                 << endl;
        }
    }
    catch (const mysqlx::Error& error)
    {
        cout << "\nMySQL Error: "
             << error.what()
             << endl;
    }
}

// ============================================================
// EVIDENCE - UPDATE
// ============================================================

bool Database::updateEvidenceStatus(
    int evidenceId,
    string newStatus
)
{
    try
    {
        mysqlx::Schema db =
            session->getSchema("cyber_crime_db");

        mysqlx::Table evidence =
            db.getTable("evidence");

        mysqlx::Result result =
            evidence.update()
            .set("status", newStatus)
            .where("evidence_id = :id")
            .bind("id", evidenceId)
            .execute();

        if (result.getAffectedItemsCount() == 0)
        {
            cout << "\nEvidence not found!"
                 << endl;

            return false;
        }

        cout << "\nEvidence status updated successfully!"
             << endl;

        return true;
    }
    catch (const mysqlx::Error& error)
    {
        cout << "\nMySQL Error: "
             << error.what()
             << endl;

        return false;
    }
}

// ============================================================
// EVIDENCE - DELETE
// ============================================================

bool Database::deleteEvidence(
    int evidenceId
)
{
    try
    {
        mysqlx::Schema db =
            session->getSchema("cyber_crime_db");

        mysqlx::Table evidence =
            db.getTable("evidence");

        mysqlx::Result result =
            evidence.remove()
            .where("evidence_id = :id")
            .bind("id", evidenceId)
            .execute();

        if (result.getAffectedItemsCount() == 0)
        {
            cout << "\nEvidence not found!"
                 << endl;

            return false;
        }

        cout << "\nEvidence deleted successfully!"
             << endl;

        return true;
    }
    catch (const mysqlx::Error& error)
    {
        cout << "\nMySQL Error: "
             << error.what()
             << endl;

        return false;
    }
}

// ============================================================
// INVESTIGATION - CREATE
// ============================================================

bool Database::addInvestigation(
    const Investigation& investigationObject
)
{
    try
    {
        mysqlx::Schema db =
            session->getSchema("cyber_crime_db");

        mysqlx::Table investigation =
            db.getTable("investigation_history");

        investigation.insert(
            "investigation_id",
            "case_id",
            "officer_id",
            "activity",
            "investigation_date",
            "remarks"
        )
        .values(
            investigationObject.getInvestigationId(),
            investigationObject.getCaseId(),
            investigationObject.getOfficerId(),
            investigationObject.getActivity(),
            investigationObject.getDate(),
            investigationObject.getRemarks()
        )
        .execute();

        cout << "\nInvestigation saved to MySQL successfully!"
             << endl;

        return true;
    }
    catch (const mysqlx::Error& error)
    {
        cout << "\nMySQL Error: "
             << error.what()
             << endl;

        return false;
    }
}

// ============================================================
// INVESTIGATION - READ
// ============================================================

void Database::displayInvestigations()
{
    try
    {
        mysqlx::Schema db =
            session->getSchema("cyber_crime_db");

        mysqlx::Table investigation =
            db.getTable("investigation_history");

        mysqlx::RowResult result =
            investigation.select(
                "investigation_id",
                "case_id",
                "officer_id",
                "activity",
                "investigation_date",
                "remarks"
            )
            .execute();

        cout << "\n============================================================\n";
        cout << "                 INVESTIGATION HISTORY\n";
        cout << "============================================================\n";

        mysqlx::Row row;

        while ((row = result.fetchOne()))
        {
            cout << "\nInvestigation ID : "
                 << row[0].get<int>()
                 << endl;

            cout << "Case ID          : "
                 << row[1].get<int>()
                 << endl;

            if (row[2].isNull())
            {
                cout << "Officer ID       : Not Assigned"
                     << endl;
            }
            else
            {
                cout << "Officer ID       : "
                     << row[2].get<int>()
                     << endl;
            }

            cout << "Activity         : "
                 << row[3].get<string>()
                 << endl;

            cout << "Date             : "
                 << row[4].get<string>()
                 << endl;

            cout << "Remarks          : "
                 << row[5].get<string>()
                 << endl;

            cout << "------------------------------------------------------------\n";
        }
    }
    catch (const mysqlx::Error& error)
    {
        cout << "\nMySQL Error: "
             << error.what()
             << endl;
    }
}

// ============================================================
// INVESTIGATION - BY CASE
// ============================================================

void Database::displayInvestigationsByCase(
    int caseId
)
{
    try
    {
        mysqlx::Schema db =
            session->getSchema("cyber_crime_db");

        mysqlx::Table investigation =
            db.getTable("investigation_history");

        mysqlx::RowResult result =
            investigation.select(
                "investigation_id",
                "case_id",
                "officer_id",
                "activity",
                "investigation_date",
                "remarks"
            )
            .where("case_id = :id")
            .bind("id", caseId)
            .execute();

        cout << "\n===== INVESTIGATION FOR CASE "
             << caseId
             << " =====\n";

        mysqlx::Row row;

        while ((row = result.fetchOne()))
        {
            cout << "\nInvestigation ID : "
                 << row[0].get<int>()
                 << endl;

            if (row[2].isNull())
            {
                cout << "Officer ID       : Not Assigned"
                     << endl;
            }
            else
            {
                cout << "Officer ID       : "
                     << row[2].get<int>()
                     << endl;
            }

            cout << "Activity         : "
                 << row[3].get<string>()
                 << endl;

            cout << "Date             : "
                 << row[4].get<string>()
                 << endl;

            cout << "Remarks          : "
                 << row[5].get<string>()
                 << endl;
        }
    }
    catch (const mysqlx::Error& error)
    {
        cout << "\nMySQL Error: "
             << error.what()
             << endl;
    }
}

// ============================================================
// INVESTIGATION - DELETE
// ============================================================

bool Database::deleteInvestigation(
    int investigationId
)
{
    try
    {
        mysqlx::Schema db =
            session->getSchema("cyber_crime_db");

        mysqlx::Table investigation =
            db.getTable("investigation_history");

        mysqlx::Result result =
            investigation.remove()
            .where("investigation_id = :id")
            .bind("id", investigationId)
            .execute();

        if (result.getAffectedItemsCount() == 0)
        {
            cout << "\nInvestigation not found!"
                 << endl;

            return false;
        }

        cout << "\nInvestigation deleted successfully!"
             << endl;

        return true;
    }
    catch (const mysqlx::Error& error)
    {
        cout << "\nMySQL Error: "
             << error.what()
             << endl;

        return false;
    }
}

// ============================================================
// REPORT - CREATE
// ============================================================

bool Database::addReport(
    int caseId,
    string reportType,
    string description
)
{
    try
    {
        mysqlx::Schema db =
            session->getSchema("cyber_crime_db");

        mysqlx::Table reports =
            db.getTable("reports");

        reports.insert(
            "case_id",
            "report_type",
            "report_description"
        )
        .values(
            caseId,
            reportType,
            description
        )
        .execute();

        cout << "\nReport added successfully!"
             << endl;

        return true;
    }
    catch (const mysqlx::Error& error)
    {
        cout << "\nMySQL Error: "
             << error.what()
             << endl;

        return false;
    }
}

// ============================================================
// REPORT - READ
// ============================================================

void Database::displayReports()
{
    try
    {
        mysqlx::Schema db =
            session->getSchema("cyber_crime_db");

        mysqlx::Table reports =
            db.getTable("reports");

        mysqlx::RowResult result =
            reports.select(
                "report_id",
                "case_id",
                "report_type",
                "report_description",
                "created_at"
            )
            .execute();

        cout << "\n============================================================\n";
        cout << "                       REPORTS\n";
        cout << "============================================================\n";

        mysqlx::Row row;

        while ((row = result.fetchOne()))
        {
            cout << "\nReport ID    : "
                 << row[0].get<int>()
                 << endl;

            cout << "Case ID      : "
                 << row[1].get<int>()
                 << endl;

            cout << "Report Type  : "
                 << row[2].get<string>()
                 << endl;

            cout << "Description  : "
                 << row[3].get<string>()
                 << endl;

            cout << "Created At   : "
                 << row[4].get<string>()
                 << endl;

            cout << "------------------------------------------------------------\n";
        }
    }
    catch (const mysqlx::Error& error)
    {
        cout << "\nMySQL Error: "
             << error.what()
             << endl;
    }
}

// ============================================================
// REPORT - BY CASE
// ============================================================

void Database::displayReportsByCase(
    int caseId
)
{
    try
    {
        mysqlx::Schema db =
            session->getSchema("cyber_crime_db");

        mysqlx::Table reports =
            db.getTable("reports");

        mysqlx::RowResult result =
            reports.select(
                "report_id",
                "case_id",
                "report_type",
                "report_description",
                "created_at"
            )
            .where("case_id = :id")
            .bind("id", caseId)
            .execute();

        cout << "\n===== REPORTS FOR CASE "
             << caseId
             << " =====\n";

        mysqlx::Row row;

        while ((row = result.fetchOne()))
        {
            cout << "\nReport ID   : "
                 << row[0].get<int>()
                 << endl;

            cout << "Report Type : "
                 << row[2].get<string>()
                 << endl;

            cout << "Description : "
                 << row[3].get<string>()
                 << endl;

            cout << "Created At  : "
                 << row[4].get<string>()
                 << endl;
        }
    }
    catch (const mysqlx::Error& error)
    {
        cout << "\nMySQL Error: "
             << error.what()
             << endl;
    }
}

// ============================================================
// REPORT - DELETE
// ============================================================

bool Database::deleteReport(
    int reportId
)
{
    try
    {
        mysqlx::Schema db =
            session->getSchema("cyber_crime_db");

        mysqlx::Table reports =
            db.getTable("reports");

        mysqlx::Result result =
            reports.remove()
            .where("report_id = :id")
            .bind("id", reportId)
            .execute();

        if (result.getAffectedItemsCount() == 0)
        {
            cout << "\nReport not found!"
                 << endl;

            return false;
        }

        cout << "\nReport deleted successfully!"
             << endl;

        return true;
    }
    catch (const mysqlx::Error& error)
    {
        cout << "\nMySQL Error: "
             << error.what()
             << endl;

        return false;
    }
}