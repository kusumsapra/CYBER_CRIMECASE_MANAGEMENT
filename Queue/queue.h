#ifndef QUEUE_H
#define QUEUE_H

#define MAX_QUEUE 100

struct Queue
{
    int cases[MAX_QUEUE];
    int front;
    int rear;
};

void initializeQueue(struct Queue* q);
int isEmpty(struct Queue* q);
int isFull(struct Queue* q);
void enqueue(struct Queue* q, int caseId);
int dequeue(struct Queue* q);
void displayQueue(struct Queue* q);

#endif