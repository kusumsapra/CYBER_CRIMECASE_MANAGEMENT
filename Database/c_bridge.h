#ifndef C_BRIDGE_H
#define C_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif

int db_initialize(void);

int db_add_case(
    int id,
    const char* victim,
    const char* crime,
    int priority,
    const char* status,
    const char* officer
);

void db_display_cases(void);
int db_search_case(int id);
int db_delete_case(int id);
int db_update_status(int id, const char* status);
void db_close(void);

#ifdef __cplusplus
}
#endif

#endif