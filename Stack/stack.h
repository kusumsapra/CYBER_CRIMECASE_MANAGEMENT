#ifndef STACK_H
#define STACK_H

#define MAX_STACK 100

struct Stack
{
    int caseId[MAX_STACK];
    char activity[MAX_STACK][100];
    int top;
};

void initializeStack(struct Stack* s);
int isStackEmpty(struct Stack* s);
int isStackFull(struct Stack* s);
void pushActivity(struct Stack* s, int caseId, char activity[]);
void popActivity(struct Stack* s);
void displayHistory(struct Stack* s);

#endif