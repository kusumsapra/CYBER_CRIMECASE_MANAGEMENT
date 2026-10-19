#ifndef USER_H
#define USER_H

#include <string>

class User
{
protected:
    int userId;
    std::string name;
    std::string email;
    std::string role;

public:
    User();

    User(
        int userId,
        std::string name,
        std::string email,
        std::string role
    );

    virtual void displayUser() const;

    void setUserId(int userId);
    void setName(std::string name);
    void setEmail(std::string email);
    void setRole(std::string role);

    int getUserId() const;
    std::string getName() const;
    std::string getEmail() const;
    std::string getRole() const;

    virtual ~User();
};

#endif