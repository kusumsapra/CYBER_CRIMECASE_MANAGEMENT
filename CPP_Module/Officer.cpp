#include <iostream>
#include "Officer.h"

using namespace std;

Officer::Officer()
    : User()
{
    officerId = 0;
    specialization = "";
    availability = "Available";
}

Officer::Officer(
    int id,
    string officerName,
    string officerEmail,
    int offId,
    string spec,
    string avail
)
    : User(id, officerName, officerEmail, "Officer")
{
    officerId = offId;
    specialization = spec;
    availability = avail;
}

void Officer::displayUser() const
{
    cout << "\n====================================\n";
    cout << "           OFFICER DETAILS\n";
    cout << "====================================\n";

    cout << "User ID          : " << userId << endl;
    cout << "Officer ID       : " << officerId << endl;
    cout << "Name             : " << name << endl;
    cout << "Email            : " << email << endl;
    cout << "Role             : " << role << endl;
    cout << "Specialization   : " << specialization << endl;
    cout << "Availability     : " << availability << endl;

    cout << "====================================\n";
}

void Officer::setOfficerId(int id)
{
    officerId = id;
}

void Officer::setSpecialization(string spec)
{
    specialization = spec;
}

void Officer::setAvailability(string avail)
{
    availability = avail;
}

int Officer::getOfficerId() const
{
    return officerId;
}

string Officer::getSpecialization() const
{
    return specialization;
}

string Officer::getAvailability() const
{
    return availability;
}