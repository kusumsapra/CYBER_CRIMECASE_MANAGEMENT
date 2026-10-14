#ifndef OFFICER_H
#define OFFICER_H

#include "User.h"

class Officer : public User
{
private:
    int officerId;
    std::string specialization;
    std::string availability;

public:
    Officer();

    Officer(
        int userId,
        std::string name,
        std::string email,
        int officerId,
        std::string specialization,
        std::string availability
    );

    void displayUser() const override;

    void setOfficerId(int officerId);
    void setSpecialization(std::string specialization);
    void setAvailability(std::string availability);

    int getOfficerId() const;
    std::string getSpecialization() const;
    std::string getAvailability() const;
};

#endif