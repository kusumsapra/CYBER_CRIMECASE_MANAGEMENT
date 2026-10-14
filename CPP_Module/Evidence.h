#ifndef EVIDENCE_H
#define EVIDENCE_H

#include <string>

class Evidence
{
private:
    int evidenceId;
    int caseId;
    std::string evidenceType;
    std::string description;
    std::string status;

public:
    Evidence();

    Evidence(
        int evidenceId,
        int caseId,
        std::string evidenceType,
        std::string description,
        std::string status
    );

    void displayEvidence() const;

    void setEvidenceId(int evidenceId);
    void setCaseId(int caseId);
    void setEvidenceType(std::string evidenceType);
    void setDescription(std::string description);
    void setStatus(std::string status);

    int getEvidenceId() const;
    int getCaseId() const;
    std::string getEvidenceType() const;
    std::string getDescription() const;
    std::string getStatus() const;
};

#endif