#include <iostream>
#include "Investigation.h"

using namespace std;

Investigation::Investigation()
{
    investigationId = 0;
    caseId = 0;
    officerId = 0;
    activity = "";
    date = "";
    remarks = "";
}

Investigation::Investigation(
    int id,
    int cId,
    int oId,
    string investigationActivity,
    string investigationDate,
    string investigationRemarks
)
{
    investigationId = id;
    caseId = cId;
    officerId = oId;
    activity = investigationActivity;
    date = investigationDate;
    remarks = investigationRemarks;
}

void Investigation::displayInvestigation() const
{
    cout << "\n====================================\n";
    cout << "       INVESTIGATION DETAILS\n";
    cout << "====================================\n";

    cout << "Investigation ID : " << investigationId << endl;
    cout << "Case ID          : " << caseId << endl;
    cout << "Officer ID       : " << officerId << endl;
    cout << "Activity         : " << activity << endl;
    cout << "Date             : " << date << endl;
    cout << "Remarks          : " << remarks << endl;

    cout << "====================================\n";
}

void Investigation::setInvestigationId(int id)
{
    investigationId = id;
}

void Investigation::setCaseId(int id)
{
    caseId = id;
}

void Investigation::setOfficerId(int id)
{
    officerId = id;
}

void Investigation::setActivity(string investigationActivity)
{
    activity = investigationActivity;
}

void Investigation::setDate(string investigationDate)
{
    date = investigationDate;
}

void Investigation::setRemarks(string investigationRemarks)
{
    remarks = investigationRemarks;
}

int Investigation::getInvestigationId() const
{
    return investigationId;
}

int Investigation::getCaseId() const
{
    return caseId;
}

int Investigation::getOfficerId() const
{
    return officerId;
}

string Investigation::getActivity() const
{
    return activity;
}

string Investigation::getDate() const
{
    return date;
}

string Investigation::getRemarks() const
{
    return remarks;
}