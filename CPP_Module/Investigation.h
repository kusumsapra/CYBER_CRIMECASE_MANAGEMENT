#ifndef INVESTIGATION_H
#define INVESTIGATION_H

#include <string>

class Investigation
{
private:
    int investigationId;
    int caseId;
    int officerId;
    std::string activity;
    std::string date;
    std::string remarks;

public:
    Investigation();

    Investigation(
        int investigationId,
        int caseId,
        int officerId,
        std::string activity,
        std::string date,
        std::string remarks
    );

    void displayInvestigation() const;

    void setInvestigationId(int investigationId);
    void setCaseId(int caseId);
    void setOfficerId(int officerId);
    void setActivity(std::string activity);
    void setDate(std::string date);
    void setRemarks(std::string remarks);

    int getInvestigationId() const;
    int getCaseId() const;
    int getOfficerId() const;
    std::string getActivity() const;
    std::string getDate() const;
    std::string getRemarks() const;
};

#endif