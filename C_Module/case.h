#ifndef CASE_H
#define CASE_H

struct Case
{
    int caseId;
    char victimName[50];
    char crimeType[50];
    int priority;
    char status[30];
    char officerName[50];
};

void displayCase(struct Case c);

#endif