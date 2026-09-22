#include <stdio.h>
#include <string.h>

#include "C_Module/linked_list.h"
#include "Queue/queue.h"
#include "PriorityQueue/priority_queue.h"
#include "Stack/stack.h"
#include "HashTable/hash_table.h"

int main()
{
    struct Node* head = NULL;

    struct Queue q;
    struct PriorityQueue pq;
    struct Stack s;
    struct HashTable ht;

    int choice;

    initializeQueue(&q);
    initializePriorityQueue(&pq);
    initializeStack(&s);
    initializeHashTable(&ht);

    do
    {
        printf("\n==============================\n");
        printf(" CYBER CRIME CASE MANAGEMENT\n");
        printf("==============================\n");

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
        printf("18. Exit\n");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            /* ================= ADD CASE ================= */
            case 1:
            {
                struct Case c;

                printf("\nEnter Case ID: ");
                scanf("%d", &c.caseId);

                printf("Enter Victim Name: ");
                scanf(" %[^\n]", c.victimName);

                printf("Enter Crime Type: ");
                scanf(" %[^\n]", c.crimeType);

                printf("Enter Priority (1-5): ");
                scanf("%d", &c.priority);

                printf("Enter Status: ");
                scanf(" %[^\n]", c.status);

                printf("Enter Officer Name: ");
                scanf(" %[^\n]", c.officerName);

                addCase(&head, c);

                insertHashCase(&ht, c.caseId);

                break;
            }

            /* ================= DISPLAY CASES ================= */
            case 2:
                displayCases(head);
                break;

            /* ================= SEARCH CASE ================= */
            case 3:
            {
                int caseId;
                struct Node* result;

                printf("\nEnter Case ID to search: ");
                scanf("%d", &caseId);

                result = searchCase(head, caseId);

                if (result != NULL)
                {
                    printf("\nCase Found!\n");
                    displayCase(result->data);
                }
                else
                {
                    printf("\nCase not found!\n");
                }

                break;
            }

            /* ================= DELETE CASE ================= */
            case 4:
            {
                int caseId;

                printf("\nEnter Case ID to delete: ");
                scanf("%d", &caseId);

                deleteCase(&head, caseId);

                break;
            }

            /* ================= UPDATE STATUS ================= */
            case 5:
            {
                int caseId;

                printf("\nEnter Case ID to update status: ");
                scanf("%d", &caseId);

                updateCaseStatus(head, caseId);

                break;
            }

            /* ================= ADD TO NORMAL QUEUE ================= */
            case 6:
            {
                int caseId;
                struct Node* result;

                printf("\nEnter Case ID to add to pending queue: ");
                scanf("%d", &caseId);

                result = searchCase(head, caseId);

                if (result != NULL)
                {
                    enqueue(&q, caseId);
                }
                else
                {
                    printf("\nCase not found!\n");
                }

                break;
            }

            /* ================= PROCESS NORMAL QUEUE ================= */
            case 7:
            {
                int caseId;

                caseId = dequeue(&q);

                if (caseId != -1)
                {
                    printf("\nProcessing Case ID: %d\n", caseId);
                }

                break;
            }

            /* ================= DISPLAY NORMAL QUEUE ================= */
            case 8:
                displayQueue(&q);
                break;

            /* ================= ADD TO PRIORITY QUEUE ================= */
            case 9:
            {
                int caseId;
                struct Node* result;

                printf("\nEnter Case ID to add to priority queue: ");
                scanf("%d", &caseId);

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
                    printf("\nCase not found!\n");
                }

                break;
            }

            /* ================= PROCESS PRIORITY QUEUE ================= */
            case 10:
            {
                int caseId;

                caseId = processPriorityCase(&pq);

                if (caseId != -1)
                {
                    printf("Highest priority case processed successfully.\n");
                }

                break;
            }

            /* ================= DISPLAY PRIORITY QUEUE ================= */
            case 11:
                displayPriorityQueue(&pq);
                break;

            /* ================= ADD INVESTIGATION ACTIVITY ================= */
            case 12:
            {
                int caseId;
                char activity[100];
                struct Node* result;

                printf("\nEnter Case ID: ");
                scanf("%d", &caseId);

                result = searchCase(head, caseId);

                if (result != NULL)
                {
                    printf("Enter Investigation Activity: ");
                    scanf(" %[^\n]", activity);

                    pushActivity(&s, caseId, activity);
                }
                else
                {
                    printf("\nCase not found!\n");
                }

                break;
            }

            /* ================= REMOVE LATEST ACTIVITY ================= */
            case 13:
                popActivity(&s);
                break;

            /* ================= DISPLAY HISTORY ================= */
            case 14:
                displayHistory(&s);
                break;

            /* ================= HASH TABLE SEARCH ================= */
            case 15:
            {
                int caseId;
                int index;

                printf("\nEnter Case ID to search using Hash Table: ");
                scanf("%d", &caseId);

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

            /* ================= HASH TABLE DELETE ================= */
            case 16:
            {
                int caseId;

                printf("\nEnter Case ID to delete from Hash Table: ");
                scanf("%d", &caseId);

                deleteHashCase(&ht, caseId);

                break;
            }

            /* ================= DISPLAY HASH TABLE ================= */
            case 17:
                displayHashTable(&ht);
                break;

            /* ================= EXIT ================= */
            case 18:
                printf("\nExiting program...\n");
                break;

            default:
                printf("\nInvalid choice!\n");
        }

    } while(choice != 18);

    return 0;
}
