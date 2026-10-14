#ifndef CASE_H
#define CASE_H

#include <string>

class Case
{
private:
    int caseId;
    std::string victimName;
    std::string crimeType;
    int priority;
    std::string status;
    std::string officerName;

public:
    Case();

    Case(
        int caseId,
        std::string victimName,
        std::string crimeType,
        int priority,
        std::string status,
        std::string officerName
    );

    void displayCase() const;

    void setCaseId(int caseId);
    void setVictimName(std::string victimName);
    void setCrimeType(std::string crimeType);
    void setPriority(int priority);
    void setStatus(std::string status);
    void setOfficerName(std::string officerName);

    int getCaseId() const;
    std::string getVictimName() const;
    std::string getCrimeType() const;
    int getPriority() const;
    std::string getStatus() const;
    std::string getOfficerName() const;
};

#endif