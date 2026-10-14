#include <iostream>
#include "Case.h"

using namespace std;

Case::Case()
{
    caseId = 0;
    victimName = "";
    crimeType = "";
    priority = 0;
    status = "Pending";
    officerName = "";
}

Case::Case(
    int id,
    string victim,
    string crime,
    int p,
    string s,
    string officer
)
{
    caseId = id;
    victimName = victim;
    crimeType = crime;
    priority = p;
    status = s;
    officerName = officer;
}

void Case::displayCase() const
{
    cout << "\n====================================\n";
    cout << "          CASE DETAILS\n";
    cout << "====================================\n";

    cout << "Case ID      : " << caseId << endl;
    cout << "Victim Name  : " << victimName << endl;
    cout << "Crime Type   : " << crimeType << endl;
    cout << "Priority     : " << priority << endl;
    cout << "Status       : " << status << endl;
    cout << "Officer      : " << officerName << endl;

    cout << "====================================\n";
}

void Case::setCaseId(int id)
{
    caseId = id;
}

void Case::setVictimName(string name)
{
    victimName = name;
}

void Case::setCrimeType(string crime)
{
    crimeType = crime;
}

void Case::setPriority(int p)
{
    priority = p;
}

void Case::setStatus(string s)
{
    status = s;
}

void Case::setOfficerName(string officer)
{
    officerName = officer;
}

int Case::getCaseId() const
{
    return caseId;
}

string Case::getVictimName() const
{
    return victimName;
}

string Case::getCrimeType() const
{
    return crimeType;
}

int Case::getPriority() const
{
    return priority;
}

string Case::getStatus() const
{
    return status;
}

string Case::getOfficerName() const
{
    return officerName;
}