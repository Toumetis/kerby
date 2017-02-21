/**
 * Adopted from PyKerberos. Modified for use with Kerby.
 *
 * Copyright (c) 2006-2015 Apple Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/

#include <errno.h>
#include <unistd.h> // for close
#include <gssapi/gssapi.h>
#include <gssapi/gssapi_generic.h>
#include <gssapi/gssapi_krb5.h>

#define krb5_get_err_text(context,code) error_message(code)

#define AUTH_GSS_ERROR      -1
#define AUTH_GSS_COMPLETE    1
#define AUTH_GSS_CONTINUE    0

#define GSS_AUTH_P_NONE         1
#define GSS_AUTH_P_INTEGRITY    2
#define GSS_AUTH_P_PRIVACY      4
#define GSS_ERRBUF_SIZE         512

typedef struct {
    gss_ctx_id_t     context;
    gss_name_t       server_name;
    long int         gss_flags;
    gss_cred_id_t    client_creds;
    char*            username;
    char*            response;
    int              responseConf;
    OM_uint32        maj_stat;
    OM_uint32        min_stat;
} gss_client_state;

typedef struct {
    gss_ctx_id_t     context;
    gss_name_t       server_name;
    gss_name_t       client_name;
    gss_cred_id_t    server_creds;
    gss_cred_id_t    client_creds;
    char*            username;
    char*            targetname;
    char*            response;
    char*            ccname;
    OM_uint32        maj_stat;
    OM_uint32        min_stat;
} gss_server_state;

gss_client_state* new_gss_client_state();
void free_gss_client_state(gss_client_state *state);
gss_server_state* new_gss_server_state();
void free_gss_server_state(gss_server_state *state);
void get_gss_error(OM_uint32 err_maj, char *buf_maj, OM_uint32 err_min, char *buf_min);

int authenticate_gss_client_init(
    const char* service, const char* principal, long int gss_flags,
    gss_server_state* delegatestate, gss_client_state* state
);
int authenticate_gss_client_clean(
    gss_client_state *state
);
int authenticate_gss_client_step(
    gss_client_state *state, const char *challenge
);
int authenticate_gss_server_init(
    const char* service, gss_server_state* state
);
int authenticate_gss_server_clean(
    gss_server_state *state
);
int authenticate_gss_server_step(
    gss_server_state *state, const char *challenge
);
krb5_principal get_keytab_entry_principal(
	krb5_keytab_entry* keytab_entry
);