#include <stdio.h>
#include "priority_queue.h"

void initializePriorityQueue(struct PriorityQueue* pq)
{
    pq->size = 0;
}

int isPriorityQueueEmpty(struct PriorityQueue* pq)
{
    if (pq->size == 0)
    {
        return 1;
    }

    return 0;
}

int isPriorityQueueFull(struct PriorityQueue* pq)
{
    if (pq->size == MAX_PRIORITY_QUEUE)
    {
        return 1;
    }

    return 0;
}

void insertPriorityCase(struct PriorityQueue* pq, int caseId, int priority)
{
    int i;

    if (isPriorityQueueFull(pq))
    {
        printf("\nPriority Queue is full!\n");
        return;
    }

    i = pq->size;

    while (i > 0 && pq->priority[i - 1] < priority)
    {
        pq->caseId[i] = pq->caseId[i - 1];
        pq->priority[i] = pq->priority[i - 1];

        i--;
    }

    pq->caseId[i] = caseId;
    pq->priority[i] = priority;

    pq->size++;

    printf("\nCase %d added to priority queue.\n", caseId);
}

int processPriorityCase(struct PriorityQueue* pq)
{
    int caseId;
    int priority;
    int i;

    if (isPriorityQueueEmpty(pq))
    {
        printf("\nPriority Queue is empty!\n");
        return -1;
    }

    caseId = pq->caseId[0];
    priority = pq->priority[0];

    for (i = 0; i < pq->size - 1; i++)
    {
        pq->caseId[i] = pq->caseId[i + 1];
        pq->priority[i] = pq->priority[i + 1];
    }

    pq->size--;

    printf("\nProcessing Case ID: %d", caseId);
    printf(" | Priority: %d\n", priority);

    return caseId;
}

void displayPriorityQueue(struct PriorityQueue* pq)
{
    int i;

    if (isPriorityQueueEmpty(pq))
    {
        printf("\nNo cases in priority queue.\n");
        return;
    }

    printf("\n===== PRIORITY CASE QUEUE =====\n");

    for (i = 0; i < pq->size; i++)
    {
        printf("Case ID: %d | Priority: %d\n",
               pq->caseId[i],
               pq->priority[i]);
    }
}