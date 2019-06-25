#ifndef OIDC_JWK_H
#define OIDC_JWK_H

#include "account/account.h"
#include "utils/keySet.h"

#include "../../../cjose/include/cjose/cjose.h"

void               secFreeJWK(cjose_jwk_t* jwk);
struct keySetPPstr createSigningKey();
struct keySetPPstr createEncryptionKey();
cjose_jwk_t*       createRSAKey();
cjose_jwk_t*       import_jwk(const char* key);
char* jwk_fromURI(const char* jwk_uri, const char* cert_path, const char* use);
void  obtainIssuerJWKS(struct oidc_account* account);
cjose_jwk_t* import_jwk_fromURI(const char* jwk_uri, const char* cert_path,
                                const char* use);
cjose_jwk_t* import_jwk_enc_fromURI(const char* jwk_uri, const char* cert_path);
cjose_jwk_t* import_jwk_sign_fromURI(const char* jwk_uri,
                                     const char* cert_path);
char* export_jwk(const cjose_jwk_t* jwk, int withPrivate, const char* use);
char* export_jwk_sig(const cjose_jwk_t* jwk, int withPrivate);
char* export_jwk_enc(const cjose_jwk_t* jwk, int withPrivate);

#endif  // OIDC_JWK_H
