#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct snmp_session snmp_session_t;

snmp_session_t* snmp_init(const char* user, const char* auth_pass, const char* priv_pass);
int snmp_get(snmp_session_t* session, const char* oid, char* value, size_t max_len);
int snmp_set(snmp_session_t* session, const char* oid, const char* value);
void snmp_close(snmp_session_t* session);

#ifdef __cplusplus
}
#endif
