#include <stdio.h>
#include "queue.h"

void initializeQueue(struct Queue* q)
{
    q->front = -1;
    q->rear = -1;
}

int isEmpty(struct Queue* q)
{
    if (q->front == -1)
    {
        return 1;
    }

    return 0;
}

int isFull(struct Queue* q)
{
    if (q->rear == MAX_QUEUE - 1)
    {
        return 1;
    }

    return 0;
}

void enqueue(struct Queue* q, int caseId)
{
    if (isFull(q))
    {
        printf("\nQueue is full!\n");
        return;
    }

    if (q->front == -1)
    {
        q->front = 0;
    }

    q->rear++;
    q->cases[q->rear] = caseId;

    printf("\nCase %d added to pending queue.\n", caseId);
}

int dequeue(struct Queue* q)
{
    int caseId;

    if (isEmpty(q))
    {
        printf("\nQueue is empty!\n");
        return -1;
    }

    caseId = q->cases[q->front];

    if (q->front == q->rear)
    {
        q->front = -1;
        q->rear = -1;
    }
    else
    {
        q->front++;
    }

    return caseId;
}

void displayQueue(struct Queue* q)
{
    int i;

    if (isEmpty(q))
    {
        printf("\nNo pending cases in queue.\n");
        return;
    }

    printf("\n===== PENDING CASE QUEUE =====\n");

    for (i = q->front; i <= q->rear; i++)
    {
        printf("Case ID: %d\n", q->cases[i]);
    }
}