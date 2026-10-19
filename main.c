
#include <stdio.h>
#include <string.h>

#include "C_Module/linked_list.h"
#include "Queue/queue.h"
#include "PriorityQueue/priority_queue.h"
#include "Stack/stack.h"
#include "HashTable/hash_table.h"
#include "Database/bridge.h"

struct LoadContext
{
    struct Node** head;
    struct HashTable* ht;
    int loaded;
    int notIndexed;
};

struct InvestigationLoadContext
{
    struct Stack* stack;
    int loaded;
};

static int hashHasSpace(struct HashTable* ht)
{
    int i;

    for (i = 0; i < HASH_SIZE; i++)
    {
        if (ht->occupied[i] == 0)
            return 1;
    }

    return 0;
}

static void loadCaseCallback(
    void* context,
    int caseId,
    const char* victimName,
    const char* crimeType,
    int priority,
    const char* status,
    const char* officerName)
{
    struct LoadContext* load = (struct LoadContext*)context;
    struct Case c;

    if (searchCase(*(load->head), caseId) != NULL)
        return;

    memset(&c, 0, sizeof(c));

    c.caseId = caseId;

    snprintf(c.victimName, sizeof(c.victimName),
             "%s", victimName ? victimName : "");

    snprintf(c.crimeType, sizeof(c.crimeType),
             "%s", crimeType ? crimeType : "");

    c.priority = priority;

    snprintf(c.status, sizeof(c.status),
             "%s", status ? status : "");

    snprintf(c.officerName, sizeof(c.officerName),
             "%s", officerName ? officerName : "Not Assigned");

    if (!addCase(load->head, c))
    {
        printf("WARNING: Could not load Case %d into linked list.\n",
               caseId);
        return;
    }

    load->loaded++;

    if (searchHashCase(load->ht, caseId) == -1)
    {
        if (hashHasSpace(load->ht))
            insertHashCase(load->ht, caseId);
        else
            load->notIndexed++;
    }
}

static void loadInvestigationCallback(
    void* context,
    int investigationId,
    int caseId,
    const char* activity)
{
    struct InvestigationLoadContext* load =
        (struct InvestigationLoadContext*)context;

    pushActivity(
        load->stack,
        investigationId,
        caseId,
        activity ? activity : ""
    );

    load->loaded++;
}

/*
   Remove activities belonging to a case deleted from MySQL.
   This keeps the in-memory stack consistent with database cascades.
*/
static void removeCaseActivitiesFromStack(
    struct Stack* s,
    int caseId)
{
    int readIndex;
    int writeIndex = 0;

    if (s == NULL)
        return;

    for (readIndex = 0; readIndex <= s->top; readIndex++)
    {
        if (s->caseId[readIndex] != caseId)
        {
            if (writeIndex != readIndex)
            {
                s->investigationId[writeIndex] =
                    s->investigationId[readIndex];

                s->caseId[writeIndex] =
                    s->caseId[readIndex];

                snprintf(
                    s->activity[writeIndex],
                    sizeof(s->activity[writeIndex]),
                    "%s",
                    s->activity[readIndex]
                );
            }

            writeIndex++;
        }
    }

    s->top = writeIndex - 1;
}

int main(void)
{
    struct Node* head = NULL;

    struct Queue q;
    struct PriorityQueue pq;
    struct Stack s;
    struct HashTable ht;

    struct LoadContext load;
    struct InvestigationLoadContext historyLoad;

    int choice;
    int dbConnected;

    initializeQueue(&q);
    initializePriorityQueue(&pq);
    initializeStack(&s);
    initializeHashTable(&ht);

    dbConnected = db_initialize();

    if (dbConnected)
    {
        int loadedCount;

        printf("\nMySQL database connected successfully.\n");

        load.head = &head;
        load.ht = &ht;
        load.loaded = 0;
        load.notIndexed = 0;

        loadedCount = db_load_cases(loadCaseCallback, &load);

        if (loadedCount >= 0)
        {
            printf("\nDatabase case loading complete.\n");
            printf("Saved cases found in MySQL: %d\n", loadedCount);
            printf("Cases loaded into linked list: %d\n", load.loaded);
            printf("Hash table capacity: %d\n", HASH_SIZE);

            if (load.notIndexed > 0)
            {
                printf("WARNING: %d case(s) could not be indexed "
                       "because the hash table is full.\n",
                       load.notIndexed);
            }
        }
        else
        {
            printf("\nWARNING: Loading cases from MySQL failed.\n");
        }

        historyLoad.stack = &s;
        historyLoad.loaded = 0;

        loadedCount = db_load_investigation_activities(
            loadInvestigationCallback,
            &historyLoad
        );

        if (loadedCount >= 0)
        {
            printf("\nInvestigation history loading complete.\n");
            printf("Saved activities found in MySQL: %d\n",
                   loadedCount);
            printf("Activities retained in memory: %d\n",
                   historyLoad.loaded < MAX_STACK
                       ? historyLoad.loaded
                       : MAX_STACK);
        }
        else
        {
            printf("\nWARNING: Could not load investigation history.\n");
        }
    }
    else
    {
        printf("\nWARNING: MySQL connection failed.\n");
        printf("Database operations will be unavailable.\n");
        printf("The DSA menu will still run.\n");
    }

    do
    {
        printf("\n========================================\n");
        printf("      CYBER CRIME CASE MANAGEMENT\n");
        printf("========================================\n");

        printf("1. Add Case\n");
        printf("2. Display Cases\n");
        printf("3. Search Case\n");
        printf("4. Delete Case\n");
        printf("5. Update Case Status\n");
        printf("6. Add Case to Pending Queue\n");
        printf("7. Process Next Pending Case\n");
        printf("8. Display Pending Queue\n");
        printf("9. Add Case to Priority Queue\n");
        printf("10. Process Highest Priority Case\n");
        printf("11. Display Priority Queue\n");
        printf("12. Add Investigation Activity\n");
        printf("13. Remove Latest Investigation Activity\n");
        printf("14. Display Investigation History\n");
        printf("15. Search Case Using Hash Table\n");
        printf("16. Delete Case From Hash Table\n");
        printf("17. Display Hash Table\n");
        printf("18. Add Evidence\n");
        printf("19. Display All Evidence\n");
        printf("20. Display Evidence By Case\n");
        printf("21. Update Evidence Status\n");
        printf("22. Delete Evidence\n");
        printf("23. Exit\n");

        printf("\nEnter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            int ch;

            while ((ch = getchar()) != '\n' && ch != EOF)
            {
            }

            printf("\nInvalid input. Enter a number from 1 to 23.\n");
            continue;
        }

        switch (choice)
        {
            case 1:
            {
                struct Case c;

                memset(&c, 0, sizeof(c));

                printf("\nEnter Case ID: ");

                if (scanf("%d", &c.caseId) != 1 || c.caseId <= 0)
                {
                    printf("\nInvalid Case ID. Use a positive integer.\n");
                    break;
                }

                if (searchCase(head, c.caseId) != NULL)
                {
                    printf("\nThis Case ID already exists in memory.\n");
                    break;
                }

                if (!hashHasSpace(&ht))
                {
                    printf("\nHash table is full. Case was not added.\n");
                    printf("Increase its capacity before adding more cases.\n");
                    break;
                }

                printf("Enter Victim Name: ");
                if (scanf(" %49[^\n]", c.victimName) != 1)
                    break;

                printf("Enter Crime Type: ");
                if (scanf(" %49[^\n]", c.crimeType) != 1)
                    break;

                printf("Enter Priority (1-5): ");

                if (scanf("%d", &c.priority) != 1 ||
                    c.priority < 1 || c.priority > 5)
                {
                    printf("\nPriority must be between 1 and 5.\n");
                    break;
                }

                printf("Enter Status: ");
                if (scanf(" %29[^\n]", c.status) != 1)
                    break;

                printf("Enter Officer Name: ");
                if (scanf(" %49[^\n]", c.officerName) != 1)
                    break;

                if (!dbConnected)
                {
                    printf("\nCannot add case: database is not connected.\n");
                    break;
                }

                if (!db_add_case(
                        c.caseId,
                        c.victimName,
                        c.crimeType,
                        c.priority,
                        c.status,
                        c.officerName))
                {
                    printf("\nCase was not added to MySQL.\n");
                    break;
                }

                if (!addCase(&head, c))
                {
                    printf("\nCould not add case to linked list.\n");
                    printf("Attempting database rollback...\n");

                    if (!db_delete_case(c.caseId))
                    {
                        printf("CRITICAL: Rollback failed. "
                               "Check Case ID %d in MySQL.\n", c.caseId);
                    }

                    break;
                }

                insertHashCase(&ht, c.caseId);

                if (searchHashCase(&ht, c.caseId) == -1)
                {
                    printf("\nCRITICAL: Hash-table insertion failed.\n");

                    deleteCase(&head, c.caseId);

                    if (!db_delete_case(c.caseId))
                    {
                        printf("CRITICAL: Database rollback failed. "
                               "Check Case ID %d manually.\n", c.caseId);
                    }

                    break;
                }

                printf("\nCase added successfully to MySQL, linked list, "
                       "and hash table.\n");
                break;
            }

            case 2:
                if (dbConnected)
                    db_display_cases();
                else
                    printf("\nDatabase is not connected.\n");
                break;

            case 3:
            {
                int caseId;

                printf("\nEnter Case ID to search: ");

                if (scanf("%d", &caseId) != 1)
                {
                    printf("\nInvalid Case ID.\n");
                    break;
                }

                if (dbConnected)
                    db_search_case(caseId);
                else
                    printf("\nDatabase is not connected.\n");

                break;
            }

            case 4:
            {
                int caseId;

                printf("\nEnter Case ID to delete: ");

                if (scanf("%d", &caseId) != 1)
                {
                    printf("\nInvalid Case ID.\n");
                    break;
                }

                if (!dbConnected)
                {
                    printf("\nDatabase is not connected.\n");
                    break;
                }

                if (db_delete_case(caseId))
                {
                    deleteCase(&head, caseId);

                    if (searchHashCase(&ht, caseId) != -1)
                        deleteHashCase(&ht, caseId);

                    removeCaseActivitiesFromStack(&s, caseId);

                    printf("\nCase deletion completed.\n");
                }
                else
                {
                    printf("\nDatabase deletion failed. "
                           "In-memory records were not changed.\n");
                }

                break;
            }

            case 5:
            {
                int caseId;
                char newStatus[30];
                struct Node* result;

                printf("\nEnter Case ID to update status: ");

                if (scanf("%d", &caseId) != 1)
                {
                    printf("\nInvalid Case ID.\n");
                    break;
                }

                result = searchCase(head, caseId);

                if (result == NULL)
                {
                    printf("\nCase not found in the linked list.\n");
                    break;
                }

                printf("Enter new status: ");

                if (scanf(" %29[^\n]", newStatus) != 1)
                {
                    printf("\nInvalid status.\n");
                    break;
                }

                if (!dbConnected)
                {
                    printf("\nDatabase is not connected.\n");
                    break;
                }

                if (db_update_case_status(caseId, newStatus))
                {
                    snprintf(
                        result->data.status,
                        sizeof(result->data.status),
                        "%s",
                        newStatus
                    );

                    printf("\nCase status updated successfully.\n");
                }
                else
                {
                    printf("\nDatabase update failed. "
                           "In-memory status was not changed.\n");
                }

                break;
            }

            case 6:
            {
                int caseId;

                printf("\nEnter Case ID to add to pending queue: ");

                if (scanf("%d", &caseId) != 1)
                    break;

                if (searchCase(head, caseId) != NULL)
                    enqueue(&q, caseId);
                else
                    printf("\nCase not found in the current session.\n");

                break;
            }

            case 7:
            {
                int caseId = dequeue(&q);

                if (caseId != -1)
                    printf("\nProcessing Case ID: %d\n", caseId);

                break;
            }

            case 8:
                displayQueue(&q);
                break;

            case 9:
            {
                int caseId;
                struct Node* result;

                printf("\nEnter Case ID to add to priority queue: ");

                if (scanf("%d", &caseId) != 1)
                    break;

                result = searchCase(head, caseId);

                if (result != NULL)
                {
                    insertPriorityCase(
                        &pq,
                        result->data.caseId,
                        result->data.priority
                    );
                }
                else
                {
                    printf("\nCase not found in the current session.\n");
                }

                break;
            }

            case 10:
            {
                int caseId = processPriorityCase(&pq);

                if (caseId != -1)
                {
                    printf("\nHighest-priority case processed: %d\n",
                           caseId);
                }

                break;
            }

            case 11:
                displayPriorityQueue(&pq);
                break;

            case 12:
            {
                int caseId;
                int investigationId;
                char activity[100];

                printf("\nEnter Case ID: ");

                if (scanf("%d", &caseId) != 1)
                {
                    printf("\nInvalid Case ID.\n");
                    break;
                }

                if (searchCase(head, caseId) == NULL)
                {
                    printf("\nCase not found in the current session.\n");
                    break;
                }

                if (!dbConnected)
                {
                    printf("\nCannot save activity: database is not connected.\n");
                    break;
                }

                printf("Enter Investigation Activity: ");

                if (scanf(" %99[^\n]", activity) != 1)
                {
                    printf("\nInvalid activity.\n");
                    break;
                }

                investigationId = db_add_investigation_activity(
                    caseId,
                    activity
                );

                if (investigationId <= 0)
                {
                    printf("\nActivity could not be saved to MySQL.\n");
                    break;
                }

                pushActivity(
                    &s,
                    investigationId,
                    caseId,
                    activity
                );

                printf("\nInvestigation activity saved successfully.\n");
                printf("Investigation ID: %d\n", investigationId);
                printf("Case ID: %d\n", caseId);
                break;
            }

            case 13:
            {
                int investigationId;

                if (isStackEmpty(&s))
                {
                    printf("\nNo investigation activity is available "
                           "in the in-memory stack.\n");
                    break;
                }

                if (!dbConnected)
                {
                    printf("\nDatabase is not connected. "
                           "The activity was not removed.\n");
                    break;
                }

                investigationId = s.investigationId[s.top];

                printf("\nDeleting latest activity, Investigation ID %d...\n",
                       investigationId);

                if (db_delete_investigation_activity(investigationId))
                {
                    popActivity(&s);
                }
                else
                {
                    printf("\nMySQL deletion failed. "
                           "The in-memory stack was not changed.\n");
                }

                break;
            }

            case 14:
                if (dbConnected)
                    db_display_investigation_activities();
                else
                    displayHistory(&s);
                break;

            case 15:
            {
                int caseId;
                int index;

                printf("\nEnter Case ID to search using Hash Table: ");

                if (scanf("%d", &caseId) != 1)
                    break;

                index = searchHashCase(&ht, caseId);

                if (index != -1)
                {
                    printf("\nCase found in Hash Table!\n");
                    printf("Case ID : %d\n", caseId);
                    printf("Index   : %d\n", index);
                }
                else
                {
                    printf("\nCase not found in Hash Table!\n");
                }

                break;
            }

            case 16:
            {
                int caseId;

                printf("\nEnter Case ID to delete from Hash Table: ");

                if (scanf("%d", &caseId) != 1)
                    break;

                deleteHashCase(&ht, caseId);
                break;
            }

            case 17:
                displayHashTable(&ht);
                break;

            case 18:
            {
                int evidenceId, caseId;
                char evidenceType[100], description[500], status[50];

                printf("\nEnter Evidence ID: ");
                if (scanf("%d", &evidenceId) != 1) {
                    printf("\nInvalid Evidence ID.\n");
                    while (getchar() != '\n') {}
                    break;
                }

                printf("Enter Case ID: ");
                if (scanf("%d", &caseId) != 1) {
                    printf("\nInvalid Case ID.\n");
                    while (getchar() != '\n') {}
                    break;
                }

                if (evidenceId <= 0 || caseId <= 0) {
                    printf("\nIDs must be positive integers.\n");
                    break;
                }

                printf("Enter Evidence Type: ");
                if (scanf(" %99[^\n]", evidenceType) != 1) {
                    printf("\nInvalid evidence type.\n");
                    break;
                }

                printf("Enter Description: ");
                if (scanf(" %499[^\n]", description) != 1) {
                    printf("\nInvalid description.\n");
                    break;
                }

                printf("Enter Evidence Status: ");
                if (scanf(" %49[^\n]", status) != 1) {
                    printf("\nInvalid status.\n");
                    break;
                }

                if (db_add_evidence(evidenceId, caseId, evidenceType,
                                    description, status)) {
                    printf("\nEvidence added successfully.\n");
                } else {
                    printf("\nFailed to add evidence. Check the Case ID and database output.\n");
                }
                break;
            }

            case 19:
                db_display_evidence();
                break;

            case 20:
            {
                int caseId;
                printf("\nEnter Case ID: ");
                if (scanf("%d", &caseId) != 1 || caseId <= 0) {
                    printf("\nInvalid Case ID.\n");
                    while (getchar() != '\n') {}
                    break;
                }
                db_display_evidence_by_case(caseId);
                break;
            }

            case 21:
            {
                int evidenceId;
                char status[50];

                printf("\nEnter Evidence ID: ");
                if (scanf("%d", &evidenceId) != 1 || evidenceId <= 0) {
                    printf("\nInvalid Evidence ID.\n");
                    while (getchar() != '\n') {}
                    break;
                }

                printf("Enter New Status: ");
                if (scanf(" %49[^\n]", status) != 1) {
                    printf("\nInvalid status.\n");
                    break;
                }

                if (db_update_evidence_status(evidenceId, status)) {
                    printf("\nEvidence status updated successfully.\n");
                } else {
                    printf("\nCould not update evidence status.\n");
                }
                break;
            }

            case 22:
            {
                int evidenceId;
                printf("\nEnter Evidence ID to delete: ");
                if (scanf("%d", &evidenceId) != 1 || evidenceId <= 0) {
                    printf("\nInvalid Evidence ID.\n");
                    while (getchar() != '\n') {}
                    break;
                }

                if (db_delete_evidence(evidenceId)) {
                    printf("\nEvidence deleted successfully.\n");
                } else {
                    printf("\nCould not delete evidence.\n");
                }
                break;
            }
            case 23:
                printf("\nExiting program...\n");
                break;

            default:
                printf("\nInvalid choice. Enter 1 to 23.\n");
        }

    } while (choice != 23);

    db_shutdown();

    return 0;
}

