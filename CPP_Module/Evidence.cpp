#include <iostream>
#include "Evidence.h"

using namespace std;

Evidence::Evidence()
{
    evidenceId = 0;
    caseId = 0;
    evidenceType = "";
    description = "";
    status = "Collected";
}

Evidence::Evidence(
    int id,
    int cId,
    string type,
    string desc,
    string evidenceStatus
)
{
    evidenceId = id;
    caseId = cId;
    evidenceType = type;
    description = desc;
    status = evidenceStatus;
}

void Evidence::displayEvidence() const
{
    cout << "\n====================================\n";
    cout << "          EVIDENCE DETAILS\n";
    cout << "====================================\n";

    cout << "Evidence ID  : " << evidenceId << endl;
    cout << "Case ID      : " << caseId << endl;
    cout << "Type         : " << evidenceType << endl;
    cout << "Description  : " << description << endl;
    cout << "Status       : " << status << endl;

    cout << "====================================\n";
}

void Evidence::setEvidenceId(int id)
{
    evidenceId = id;
}

void Evidence::setCaseId(int id)
{
    caseId = id;
}

void Evidence::setEvidenceType(string type)
{
    evidenceType = type;
}

void Evidence::setDescription(string desc)
{
    description = desc;
}

void Evidence::setStatus(string evidenceStatus)
{
    status = evidenceStatus;
}

int Evidence::getEvidenceId() const
{
    return evidenceId;
}

int Evidence::getCaseId() const
{
    return caseId;
}

string Evidence::getEvidenceType() const
{
    return evidenceType;
}

string Evidence::getDescription() const
{
    return description;
}

string Evidence::getStatus() const
{
    return status;
}