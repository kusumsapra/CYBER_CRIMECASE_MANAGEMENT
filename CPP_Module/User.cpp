#include <iostream>
#include "User.h"

using namespace std;

User::User()
{
    userId = 0;
    name = "";
    email = "";
    role = "";
}

User::User(
    int id,
    string userName,
    string userEmail,
    string userRole
)
{
    userId = id;
    name = userName;
    email = userEmail;
    role = userRole;
}

void User::displayUser() const
{
    cout << "\n====================================\n";
    cout << "             USER DETAILS\n";
    cout << "====================================\n";

    cout << "User ID      : " << userId << endl;
    cout << "Name         : " << name << endl;
    cout << "Email        : " << email << endl;
    cout << "Role         : " << role << endl;

    cout << "====================================\n";
}

void User::setUserId(int id)
{
    userId = id;
}

void User::setName(string userName)
{
    name = userName;
}

void User::setEmail(string userEmail)
{
    email = userEmail;
}

void User::setRole(string userRole)
{
    role = userRole;
}

int User::getUserId() const
{
    return userId;
}

string User::getName() const
{
    return name;
}

string User::getEmail() const
{
    return email;
}

string User::getRole() const
{
    return role;
}

User::~User()
{
}