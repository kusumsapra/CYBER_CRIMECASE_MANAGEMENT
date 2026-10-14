
#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

int addCase(struct Node** head, struct Case c)
{
    struct Node* newNode;
    struct Node* temp;

    if (head == NULL)
    {
        return 0;
    }

    newNode = (struct Node*)malloc(sizeof(struct Node));

    if (newNode == NULL)
    {
        printf("\nMemory allocation failed!\n");
        return 0;
    }

    newNode->data = c;
    newNode->next = NULL;

    if (*head == NULL)
    {
        *head = newNode;
    }
    else
    {
        temp = *head;

        while (temp->next != NULL)
        {
            temp = temp->next;
        }

        temp->next = newNode;
    }

    return 1;
}

void displayCases(struct Node* head)
{
    struct Node* temp = head;

    if (head == NULL)
    {
        printf("\nNo cases available.\n");
        return;
    }

    printf("\n===== ALL CYBER CRIME CASES =====\n");

    while (temp != NULL)
    {
        printf("\nCase ID     : %d", temp->data.caseId);
        printf("\nVictim Name : %s", temp->data.victimName);
        printf("\nCrime Type  : %s", temp->data.crimeType);
        printf("\nPriority    : %d", temp->data.priority);
        printf("\nStatus      : %s", temp->data.status);
        printf("\nOfficer     : %s\n", temp->data.officerName);

        temp = temp->next;
    }
}

struct Node* searchCase(struct Node* head, int caseId)
{
    struct Node* temp = head;

    while (temp != NULL)
    {
        if (temp->data.caseId == caseId)
        {
            return temp;
        }

        temp = temp->next;
    }

    return NULL;
}

void deleteCase(struct Node** head, int caseId)
{
    struct Node* temp;
    struct Node* prev = NULL;

    if (head == NULL || *head == NULL)
    {
        printf("\nNo cases available.\n");
        return;
    }

    temp = *head;

    if (temp->data.caseId == caseId)
    {
        *head = temp->next;
        free(temp);
        printf("\nCase deleted from linked list.\n");
        return;
    }

    while (temp != NULL && temp->data.caseId != caseId)
    {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL)
    {
        printf("\nCase not found in linked list.\n");
        return;
    }

    prev->next = temp->next;
    free(temp);

    printf("\nCase deleted from linked list.\n");
}

void updateCaseStatus(struct Node* head, int caseId)
{
    struct Node* temp = searchCase(head, caseId);
    char newStatus[30];

    if (temp == NULL)
    {
        printf("\nCase not found!\n");
        return;
    }

    printf("\nCurrent Status: %s\n", temp->data.status);
    printf("Enter New Status: ");

    if (scanf(" %29[^\n]", newStatus) != 1)
    {
        printf("\nInvalid status.\n");
        return;
    }

    snprintf(temp->data.status, sizeof(temp->data.status),
             "%s", newStatus);

    printf("\nCase status updated in linked list.\n");
}
