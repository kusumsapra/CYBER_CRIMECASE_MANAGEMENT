#include <stdio.h>
#include <string.h>
#include "stack.h"

void initializeStack(struct Stack* s)
{
    s->top = -1;
}

int isStackEmpty(struct Stack* s)
{
    if (s->top == -1)
    {
        return 1;
    }

    return 0;
}

int isStackFull(struct Stack* s)
{
    if (s->top == MAX_STACK - 1)
    {
        return 1;
    }

    return 0;
}

void pushActivity(struct Stack* s, int caseId, char activity[])
{
    if (isStackFull(s))
    {
        printf("\nInvestigation history is full!\n");
        return;
    }

    s->top++;

    s->caseId[s->top] = caseId;
    strcpy(s->activity[s->top], activity);

    printf("\nInvestigation activity added successfully.\n");
}

void popActivity(struct Stack* s)
{
    if (isStackEmpty(s))
    {
        printf("\nNo investigation history available.\n");
        return;
    }

    printf("\nRemoving latest activity:\n");
    printf("Case ID  : %d\n", s->caseId[s->top]);
    printf("Activity : %s\n", s->activity[s->top]);

    s->top--;

    printf("\nLatest activity removed successfully.\n");
}

void displayHistory(struct Stack* s)
{
    int i;

    if (isStackEmpty(s))
    {
        printf("\nNo investigation history available.\n");
        return;
    }

    printf("\n===== INVESTIGATION HISTORY =====\n");

    for (i = s->top; i >= 0; i--)
    {
        printf("\nCase ID  : %d", s->caseId[i]);
        printf("\nActivity : %s\n", s->activity[i]);
    }
}