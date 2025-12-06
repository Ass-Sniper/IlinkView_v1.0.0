#include "snmp_manager.h"
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <stdlib.h>
#include <string.h>

struct snmp_session {
    struct snmp_session session;
    struct snmp_session *ss;
};

snmp_session_t* snmp_init(const char* user, const char* auth_pass, const char* priv_pass) {
    snmp_session_t *s = (snmp_session_t*)malloc(sizeof(snmp_session_t));
    if (!s) return NULL;

    init_snmp("iLinkView");

    snmp_sess_init(&s->session);
    s->session.version = SNMP_VERSION_3;
    s->session.securityName = strdup(user);
    s->session.securityNameLen = strlen(user);
    s->session.securityLevel = SNMP_SEC_LEVEL_AUTHPRIV;

    // Auth & Priv params
    s->session.securityAuthProto = usmHMACMD5AuthProtocol;
    s->session.securityAuthProtoLen = USM_AUTH_PROTO_MD5_LEN;
    s->session.securityAuthKeyLen = USM_AUTH_KU_LEN;
    if (generate_Ku(s->session.securityAuthProto, s->session.securityAuthProtoLen,
                    (u_char*)auth_pass, strlen(auth_pass), s->session.securityAuthKey,
                    &s->session.securityAuthKeyLen) != SNMPERR_SUCCESS) {
        free(s);
        return NULL;
    }
    s->session.securityPrivProto = usmDESPrivProtocol;
    s->session.securityPrivProtoLen = USM_PRIV_PROTO_DES_LEN;
    s->session.securityPrivKeyLen = USM_PRIV_KU_LEN;
    if (generate_Ku(s->session.securityPrivProto, s->session.securityPrivProtoLen,
                    (u_char*)priv_pass, strlen(priv_pass), s->session.securityPrivKey,
                    &s->session.securityPrivKeyLen) != SNMPERR_SUCCESS) {
        free(s);
        return NULL;
    }

    s->ss = snmp_open(&s->session);
    if (!s->ss) {
        free(s);
        return NULL;
    }
    return s;
}

int snmp_get(snmp_session_t* s, const char* oid_str, char* value, size_t max_len) {
    oid anOID[MAX_OID_LEN];
    size_t anOID_len = MAX_OID_LEN;
    if (!snmp_parse_oid(oid_str, anOID, &anOID_len)) return -1;

    struct snmp_pdu *pdu = snmp_pdu_create(SNMP_MSG_GET);
    snmp_add_null_var(pdu, anOID, anOID_len);

    struct snmp_pdu *response;
    int status = snmp_synch_response(s->ss, pdu, &response);
    if (status == STAT_SUCCESS && response->errstat == SNMP_ERR_NOERROR) {
        struct variable_list *vars = response->variables;
        snprintf(value, max_len, "%s", vars->val.string);
        snmp_free_pdu(response);
        return 0;
    }
    if (response) snmp_free_pdu(response);
    return -1;
}

int snmp_set(snmp_session_t* s, const char* oid_str, const char* value) {
    // 简化示例，可根据类型扩展
    return 0;
}

void snmp_close(snmp_session_t* s) {
    if (!s) return;
    if (s->ss) snmp_close(s->ss);
    free(s);
}
