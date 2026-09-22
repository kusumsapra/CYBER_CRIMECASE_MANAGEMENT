#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

void addCase(struct Node** head, struct Case c)
{
    struct Node* newNode;

    newNode = (struct Node*)malloc(sizeof(struct Node));

    if (newNode == NULL)
    {
        printf("Memory allocation failed!\n");
        return;
    }

    newNode->data = c;
    newNode->next = NULL;

    if (*head == NULL)
    {
        *head = newNode;
    }
    else
    {
        struct Node* temp = *head;

        while (temp->next != NULL)
        {
            temp = temp->next;
        }

        temp->next = newNode;
    }

    printf("Case added successfully!\n");
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
    struct Node* temp = *head;
    struct Node* prev = NULL;

    if (*head == NULL)
    {
        printf("\nNo cases available.\n");
        return;
    }

    if (temp->data.caseId == caseId)
    {
        *head = temp->next;
        free(temp);

        printf("\nCase deleted successfully!\n");
        return;
    }

    while (temp != NULL && temp->data.caseId != caseId)
    {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL)
    {
        printf("\nCase not found!\n");
        return;
    }

    prev->next = temp->next;
    free(temp);

    printf("\nCase deleted successfully!\n");
}

void updateCaseStatus(struct Node* head, int caseId)
{
    struct Node* temp = head;
    char newStatus[30];

    while (temp != NULL)
    {
        if (temp->data.caseId == caseId)
        {
            printf("\nCurrent Status: %s\n", temp->data.status);

            printf("Enter New Status: ");
            scanf(" %[^\n]", newStatus);

            snprintf(temp->data.status, sizeof(temp->data.status),
                     "%s", newStatus);

            printf("\nCase status updated successfully!\n");
            return;
        }

        temp = temp->next;
    }

    printf("\nCase not found!\n");
}