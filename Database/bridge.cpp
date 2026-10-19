#include "bridge.h"
#include "database.h"

#include "../CPP_Module/Case.h"
#include "../CPP_Module/Evidence.h"

#include <mysqlx/xdevapi.h>

#include <iostream>
#include <string>
#include <exception>

static Database database;
static bool connected = false;
static mysqlx::Session* loaderSession = nullptr;

extern "C" {

int db_initialize(void)
{
    if (connected && loaderSession != nullptr)
        return 1;

    try
    {
        if (!database.connect())
        {
            std::cerr << "\nMySQL Database connection failed.\n";
            return 0;
        }

        loaderSession = new mysqlx::Session(
            "localhost",
            33060,
            "root",
            "1234"
        );
        loaderSession->sql("USE cyber_crime_db").execute();

        connected = true;

        std::cout << "\nMySQL bridge initialized successfully.\n";
        return 1;
    }
    catch (const std::exception& e)
    {
        std::cerr << "\nMySQL initialization error: "
                  << e.what() << '\n';

        if (loaderSession != nullptr)
        {
            delete loaderSession;
            loaderSession = nullptr;
        }

        try
        {
            database.close();
        }
        catch (...)
        {
        }

        connected = false;
        return 0;
    }
}

int db_add_case(
    int caseId,
    const char* victimName,
    const char* crimeType,
    int priority,
    const char* status,
    const char* officerName
)
{
    if (!connected ||
        victimName == nullptr ||
        crimeType == nullptr ||
        status == nullptr ||
        officerName == nullptr)
    {
        return 0;
    }

    try
    {
        Case caseObject(
            caseId,
            std::string(victimName),
            std::string(crimeType),
            priority,
            std::string(status),
            std::string(officerName)
        );

        return database.addCase(caseObject) ? 1 : 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "\nAdd case error: " << e.what() << '\n';
        return 0;
    }
}

void db_display_cases(void)
{
    if (!connected)
    {
        std::cout << "\nDatabase is not connected.\n";
        return;
    }

    try
    {
        database.displayCases();
    }
    catch (const std::exception& e)
    {
        std::cerr << "\nDisplay cases error: "
                  << e.what() << '\n';
    }
}

int db_search_case(int caseId)
{
    if (!connected)
        return 0;

    try
    {
        return database.searchCase(caseId) ? 1 : 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "\nSearch case error: "
                  << e.what() << '\n';
        return 0;
    }
}

int db_delete_case(int caseId)
{
    if (!connected)
        return 0;

    try
    {
        return database.deleteCase(caseId) ? 1 : 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "\nDelete case error: "
                  << e.what() << '\n';
        return 0;
    }
}

int db_update_case_status(int caseId, const char* status)
{
    if (!connected || status == nullptr)
        return 0;

    try
    {
        return database.updateCaseStatus(
            caseId,
            std::string(status)
        ) ? 1 : 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "\nUpdate status error: "
                  << e.what() << '\n';
        return 0;
    }
}

int db_load_cases(CaseRowCallback callback, void* context)
{
    if (!connected || callback == nullptr)
    {
        std::cerr << "Database not connected or callback is invalid.\n";
        return -1;
    }

    try
    {
        mysqlx::Session session(
            "localhost", 33060, "root", "1234"
        );

        session.sql("USE cyber_crime_db").execute();

        mysqlx::SqlResult result = session.sql(
            "SELECT case_id, victim_name, crime_type, priority, "
            "status, officer_id "
            "FROM cases ORDER BY case_id"
        ).execute();

        int count = 0;
        mysqlx::Row row;

        while ((row = result.fetchOne()))
        {
            int caseId = row[0].get<int>();
            std::string victimName = row[1].get<std::string>();
            std::string crimeType = row[2].get<std::string>();
            int priority = row[3].get<int>();
            std::string status = row[4].get<std::string>();

            // Your table stores officer_id, not officer_name.
            std::string officerName = "Not Assigned";

            if (!row[5].isNull())
            {
                officerName = std::to_string(row[5].get<int>());
            }

            callback(
                context,
                caseId,
                victimName.c_str(),
                crimeType.c_str(),
                priority,
                status.c_str(),
                officerName.c_str()
            );

            count++;
        }

        session.close();
        std::cout << "Cases loaded successfully: "
                  << count << '\n';

        return count;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Loading cases from MySQL failed: "
                  << e.what() << '\n';
        return -1;
    }
}

int db_add_investigation_activity(
    int caseId,
    const char* activity
)
{
    if (!connected ||
        loaderSession == nullptr ||
        activity == nullptr ||
        activity[0] == '\0')
    {
        return 0;
    }

    try
    {
        /*
          The investigation ID is AUTO_INCREMENT.
          CURDATE() uses the MySQL server's current date.
          Officer is optional; remarks start empty.
        */
        loaderSession->sql(
            "INSERT INTO investigation_history "
            "(case_id, activity, investigation_date, remarks) "
            "VALUES (?, ?, CURDATE(), '')"
        ).bind(caseId, std::string(activity)).execute();

        mysqlx::SqlResult idResult = loaderSession->sql(
            "SELECT LAST_INSERT_ID()"
        ).execute();

        mysqlx::Row row = idResult.fetchOne();

        if (!row)
        {
            std::cerr << "\nActivity saved, but its ID could not be retrieved.\n";
            return 0;
        }

        return row[0].get<int>();
    }
    catch (const std::exception& e)
    {
        std::cerr << "\nAdd investigation activity error: "
                  << e.what() << '\n';
        return 0;
    }
}

int db_load_investigation_activities(
    InvestigationRowCallback callback,
    void* context
)
{
    if (!connected || loaderSession == nullptr || callback == nullptr)
        return -1;

    try
    {
        mysqlx::SqlResult result = loaderSession->sql(
            "SELECT investigation_id, case_id, "
            "COALESCE(activity, '') "
            "FROM investigation_history "
            "ORDER BY investigation_id ASC"
        ).execute();

        int count = 0;
        mysqlx::Row row;

        while ((row = result.fetchOne()))
        {
            int investigationId = row[0].get<int>();
            int caseId = row[1].get<int>();
            std::string activity = row[2].get<std::string>();

            callback(
                context,
                investigationId,
                caseId,
                activity.c_str()
            );

            count++;
        }

        return count;
    }
    catch (const std::exception& e)
    {
        std::cerr << "\nLoad investigation history error: "
                  << e.what() << '\n';
        return -1;
    }
}

int db_delete_investigation_activity(int investigationId)
{
    if (!connected ||
        loaderSession == nullptr ||
        investigationId <= 0)
    {
        return 0;
    }

    try
    {
        mysqlx::SqlResult result = loaderSession->sql(
            "DELETE FROM investigation_history "
            "WHERE investigation_id = ?"
        ).bind(investigationId).execute();

        return result.getAffectedItemsCount() > 0 ? 1 : 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "\nDelete investigation activity error: "
                  << e.what() << '\n';
        return 0;
    }
}

void db_display_investigation_activities(void)
{
    if (!connected || loaderSession == nullptr)
    {
        std::cout << "\nDatabase is not connected.\n";
        return;
    }

    try
    {
        mysqlx::SqlResult result = loaderSession->sql(
            "SELECT investigation_id, case_id, "
            "COALESCE(activity, ''), "
            "COALESCE(DATE_FORMAT(investigation_date, '%Y-%m-%d'), 'N/A') "
            "FROM investigation_history "
            "ORDER BY investigation_id DESC"
        ).execute();

        mysqlx::Row row;
        int count = 0;

        std::cout << "\n===== SAVED INVESTIGATION HISTORY =====\n";

        while ((row = result.fetchOne()))
        {
            std::cout << "\nInvestigation ID : "
                      << row[0].get<int>();

            std::cout << "\nCase ID          : "
                      << row[1].get<int>();

            std::cout << "\nActivity         : "
                      << row[2].get<std::string>();

            std::cout << "\nDate             : "
                      << row[3].get<std::string>() << '\n';

            count++;
        }

        if (count == 0)
            std::cout << "\nNo investigation history found.\n";
        else
            std::cout << "\nTotal saved activities: " << count << '\n';
    }
    catch (const std::exception& e)
    {
        std::cerr << "\nDisplay investigation history error: "
                  << e.what() << '\n';
    }
}


/* ============================================================
   EVIDENCE MANAGEMENT BRIDGE
   ============================================================ */

int db_add_evidence(
    int evidenceId,
    int caseId,
    const char* evidenceType,
    const char* description,
    const char* status)
{
    if (!connected || evidenceId <= 0 || caseId <= 0 ||
        evidenceType == nullptr || description == nullptr || status == nullptr)
        return 0;

    try
    {
        Evidence evidence(evidenceId, caseId, std::string(evidenceType),
                          std::string(description), std::string(status));
        return database.addEvidence(evidence) ? 1 : 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "\nAdd evidence error: " << e.what() << '\n';
        return 0;
    }
}

void db_display_evidence(void)
{
    if (!connected)
    {
        std::cout << "\nDatabase is not connected.\n";
        return;
    }
    try { database.displayEvidence(); }
    catch (const std::exception& e)
    {
        std::cerr << "\nDisplay evidence error: " << e.what() << '\n';
    }
}

void db_display_evidence_by_case(int caseId)
{
    if (!connected || caseId <= 0)
    {
        std::cout << "\nInvalid case ID or database connection.\n";
        return;
    }
    try { database.displayEvidenceByCase(caseId); }
    catch (const std::exception& e)
    {
        std::cerr << "\nDisplay evidence by case error: " << e.what() << '\n';
    }
}

int db_update_evidence_status(int evidenceId, const char* status)
{
    if (!connected || evidenceId <= 0 || status == nullptr || status[0] == '\0')
        return 0;
    try
    {
        return database.updateEvidenceStatus(evidenceId, std::string(status)) ? 1 : 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "\nUpdate evidence status error: " << e.what() << '\n';
        return 0;
    }
}

int db_delete_evidence(int evidenceId)
{
    if (!connected || evidenceId <= 0)
        return 0;
    try { return database.deleteEvidence(evidenceId) ? 1 : 0; }
    catch (const std::exception& e)
    {
        std::cerr << "\nDelete evidence error: " << e.what() << '\n';
        return 0;
    }
}

void db_shutdown(void)
{
    if (loaderSession != nullptr)
    {
        try
        {
            delete loaderSession;
        }
        catch (...)
        {
        }

        loaderSession = nullptr;
    }

    if (connected)
    {
        try
        {
            database.close();
        }
        catch (const std::exception& e)
        {
            std::cerr << "\nDatabase shutdown error: "
                      << e.what() << '\n';
        }

        connected = false;
    }
}

}
