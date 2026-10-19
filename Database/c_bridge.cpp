
#include "bridge.h"
#include "database.h"

#include "../CPP_Module/Case.h"

#include <mysqlx/xdevapi.h>

#include <iostream>
#include <string>
#include <exception>

static Database database;
static bool connected = false;
static mysqlx::Session* loaderSession = nullptr;

extern "C" {

/* =========================================================
   INITIALIZE DATABASE CONNECTION
   ========================================================= */

int db_initialize(void)
{
    if (connected && loaderSession != nullptr)
        return 1;

    try
    {
        if (!database.connect())
        {
            std::cout << "\nMySQL database connection failed.\n";
            return 0;
        }

        loaderSession = new mysqlx::Session(
            "localhost",
            33060,
            "root",
            "1234"
        );

        // Explicitly select the project database.
        loaderSession->sql("USE cyber_crime_db").execute();

        connected = true;

        std::cout << "\nMySQL database connected successfully.\n";
        return 1;
    }
    catch (const std::exception& e)
    {
        std::cerr << "\nDatabase initialization error: "
                  << e.what() << '\n';

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

        database.close();
        connected = false;

        return 0;
    }
}


/* =========================================================
   ADD CASE
   ========================================================= */

int db_add_case(
    int caseId,
    const char* victimName,
    const char* crimeType,
    int priority,
    const char* status,
    const char* officerName)
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


/* =========================================================
   LOAD ALL SAVED CASES INTO THE C PROGRAM
   ========================================================= */


int db_load_cases(
    CaseRowCallback callback,
    void* context)
{
    if (!connected || loaderSession == nullptr || callback == nullptr)
        return -1;

    try
    {
        mysqlx::SqlResult result = loaderSession->sql(
            "SELECT case_id, "
            "COALESCE(victim_name, ''), "
            "COALESCE(crime_type, ''), "
            "COALESCE(priority, 1), "
            "COALESCE(status, ''), "
            "COALESCE(CAST(officer_id AS CHAR), 'Not Assigned') "
            "FROM cases ORDER BY case_id"
        ).execute();

        int count = 0;

        for (mysqlx::Row row : result)
        {
            int caseId = row[0].get<int>();
            std::string victimName = row[1].get<std::string>();
            std::string crimeType = row[2].get<std::string>();
            int priority = row[3].get<int>();
            std::string status = row[4].get<std::string>();
            std::string officerName = row[5].get<std::string>();

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

        return count;
    }
    catch (const std::exception& e)
    {
        std::cerr << "\nLoad cases error: " << e.what() << '\n';
        return -1;
    }
}


/* =========================================================
   DISPLAY CASES
   ========================================================= */

void db_display_cases(void)
{
    if (!connected)
    {
        std::cout << "\nDatabase is not connected.\n";
        return;
    }

    database.displayCases();
}


/* =========================================================
   SEARCH CASE
   ========================================================= */

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
        std::cerr << "\nSearch case error: " << e.what() << '\n';
        return 0;
    }
}


/* =========================================================
   DELETE CASE
   ========================================================= */

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
        std::cerr << "\nDelete case error: " << e.what() << '\n';
        return 0;
    }
}


/* =========================================================
   UPDATE CASE STATUS
   ========================================================= */

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
        std::cerr << "\nUpdate status error: " << e.what() << '\n';
        return 0;
    }
}


/* =========================================================
   ADD INVESTIGATION ACTIVITY
   Returns the new investigation ID, or -1 on failure.
   ========================================================= */

int db_add_investigation_activity(
    int caseId,
    const char* activity)
{
    if (!connected ||
        loaderSession == nullptr ||
        activity == nullptr)
    {
        return -1;
    }

    try
    {
        // Confirm that the case exists in the database.
        mysqlx::SqlResult check = loaderSession->sql(
            "SELECT case_id FROM cases WHERE case_id = ?"
        ).bind(caseId).execute();

        if (check.fetchOne().isNull())
        {
            std::cout << "\nCase ID " << caseId
                      << " does not exist in MySQL.\n";
            return -1;
        }

        loaderSession->sql(
            "INSERT INTO investigation_history "
            "(case_id, activity, investigation_date, remarks) "
            "VALUES (?, ?, CURDATE(), '')"
        ).bind(caseId, std::string(activity)).execute();

        mysqlx::SqlResult idResult = loaderSession->sql(
            "SELECT LAST_INSERT_ID()"
        ).execute();

        mysqlx::Row idRow = idResult.fetchOne();

        if (idRow.isNull())
            return -1;

        return idRow[0].get<int>();
    }
    catch (const std::exception& e)
    {
        std::cerr << "\nAdd investigation activity error: "
                  << e.what() << '\n';
        return -1;
    }
}


/* =========================================================
   LOAD INVESTIGATION HISTORY INTO THE STACK
   ========================================================= */

int db_load_investigation_activities(
    InvestigationRowCallback callback,
    void* context)
{
    if (!connected ||
        loaderSession == nullptr ||
        callback == nullptr)
    {
        return -1;
    }

    try
    {
        mysqlx::SqlResult result = loaderSession->sql(
            "SELECT investigation_id, case_id, "
            "COALESCE(activity, '') "
            "FROM investigation_history "
            "ORDER BY investigation_id ASC"
        ).execute();

        int count = 0;

        for (mysqlx::Row row : result)
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


/* =========================================================
   DELETE ONE INVESTIGATION ACTIVITY BY ID
   ========================================================= */

int db_delete_investigation_activity(int investigationId)
{
    if (!connected || loaderSession == nullptr)
        return 0;

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


/* =========================================================
   DISPLAY ALL SAVED INVESTIGATION ACTIVITIES
   ========================================================= */

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
            "COALESCE(DATE_FORMAT(investigation_date, '%Y-%m-%d'), "
            "'Not recorded') "
            "FROM investigation_history "
            "ORDER BY investigation_id DESC"
        ).execute();

        int count = 0;

        std::cout << "\n===== SAVED INVESTIGATION HISTORY =====\n";

        for (mysqlx::Row row : result)
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
            std::cout << "\nNo investigation activities found.\n";

        std::cout << "\nTotal saved activities: " << count << '\n';
    }
    catch (const std::exception& e)
    {
        std::cerr << "\nDisplay investigation history error: "
                  << e.what() << '\n';
    }
}


/* =========================================================
   CLOSE DATABASE CONNECTIONS
   ========================================================= */

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
        database.close();
        connected = false;
    }

    std::cout << "\nDatabase connection closed.\n";
}

} 