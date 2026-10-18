#include "case.h"
#include <stdio.h>
#include <string.h>

void displayCase(struct Case c)
{
    printf("\n----- Case Details -----\n");
    printf("Case ID      : %d\n", c.caseId);
    printf("Victim Name  : %s\n", c.victimName);
    printf("Crime Type   : %s\n", c.crimeType);
    printf("Priority     : %d\n", c.priority);
    printf("Status       : %s\n", c.status);
    printf("Officer      : %s\n", c.officerName);
}