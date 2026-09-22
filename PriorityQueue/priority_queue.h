#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#define MAX_PRIORITY_QUEUE 100

struct PriorityQueue
{
    int caseId[MAX_PRIORITY_QUEUE];
    int priority[MAX_PRIORITY_QUEUE];
    int size;
};

void initializePriorityQueue(struct PriorityQueue* pq);
int isPriorityQueueEmpty(struct PriorityQueue* pq);
int isPriorityQueueFull(struct PriorityQueue* pq);
void insertPriorityCase(struct PriorityQueue* pq, int caseId, int priority);
int processPriorityCase(struct PriorityQueue* pq);
void displayPriorityQueue(struct PriorityQueue* pq);

#endif