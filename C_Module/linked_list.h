
#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "case.h"

struct Node
{
    struct Case data;
    struct Node* next;
};

/* Returns 1 on success and 0 on failure. */
int addCase(struct Node** head, struct Case c);

void displayCases(struct Node* head);

struct Node* searchCase(struct Node* head, int caseId);

void deleteCase(struct Node** head, int caseId);

void updateCaseStatus(struct Node* head, int caseId);

#endif
