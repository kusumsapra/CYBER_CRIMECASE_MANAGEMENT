#ifndef DATABASE_BRIDGE_H
#define DATABASE_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*CaseRowCallback)(void*, int, const char*, const char*, int, const char*, const char*);
typedef void (*InvestigationRowCallback)(void*, int, int, const char*);

int db_initialize(void);
int db_add_case(int caseId, const char* victimName, const char* crimeType, int priority, const char* status, const char* officerName);
void db_display_cases(void);
int db_search_case(int caseId);
int db_delete_case(int caseId);
int db_update_case_status(int caseId, const char* status);
int db_load_cases(CaseRowCallback callback, void* context);

int db_add_investigation_activity(int caseId, const char* activity);
int db_load_investigation_activities(InvestigationRowCallback callback, void* context);
int db_delete_investigation_activity(int investigationId);
void db_display_investigation_activities(void);

int db_add_evidence(int evidenceId, int caseId, const char* evidenceType, const char* description, const char* status);
void db_display_evidence(void);
void db_display_evidence_by_case(int caseId);
int db_update_evidence_status(int evidenceId, const char* status);
int db_delete_evidence(int evidenceId);

void db_shutdown(void);

#ifdef __cplusplus
}
#endif

#endif
