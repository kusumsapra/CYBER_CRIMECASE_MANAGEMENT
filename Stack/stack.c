
#include <stdio.h>
#include <string.h>
#include "stack.h"

void initializeStack(struct Stack* s)
{
    if (s != NULL)
        s->top = -1;
}

int isStackEmpty(struct Stack* s)
{
    return s == NULL || s->top == -1;
}

int isStackFull(struct Stack* s)
{
    return s != NULL && s->top == MAX_STACK - 1;
}

void pushActivity(
    struct Stack* s,
    int investigationId,
    int caseId,
    const char activity[]
)
{
    int i;

    if (s == NULL || activity == NULL)
        return;

    /*
       Keep the newest MAX_STACK activities in memory.
       Older records remain stored in MySQL.
    */
    if (isStackFull(s))
    {
        for (i = 1; i <= s->top; i++)
        {
            s->investigationId[i - 1] = s->investigationId[i];
            s->caseId[i - 1] = s->caseId[i];

            snprintf(
                s->activity[i - 1],
                sizeof(s->activity[i - 1]),
                "%s",
                s->activity[i]
            );
        }

        s->top--;
    }

    s->top++;

    s->investigationId[s->top] = investigationId;
    s->caseId[s->top] = caseId;

    snprintf(
        s->activity[s->top],
        sizeof(s->activity[s->top]),
        "%s",
        activity
    );
}

void popActivity(struct Stack* s)
{
    if (isStackEmpty(s))
    {
        printf("\nNo investigation history available.\n");
        return;
    }

    printf("\nLatest activity removed:\n");
    printf("Investigation ID : %d\n",
           s->investigationId[s->top]);
    printf("Case ID          : %d\n",
           s->caseId[s->top]);
    printf("Activity         : %s\n",
           s->activity[s->top]);

    s->top--;

    printf("\nLatest activity removed successfully.\n");
}

void displayHistory(struct Stack* s)
{
    int i;

    if (isStackEmpty(s))
    {
        printf("\nNo investigation history available in memory.\n");
        return;
    }

    printf("\n===== INVESTIGATION STACK (NEWEST FIRST) =====\n");

    for (i = s->top; i >= 0; i--)
    {
        printf("\nInvestigation ID : %d",
               s->investigationId[i]);

        printf("\nCase ID          : %d",
               s->caseId[i]);

        printf("\nActivity         : %s\n",
               s->activity[i]);
    }
}
