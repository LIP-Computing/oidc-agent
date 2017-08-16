#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>

#include "api.h"
#include "ipc.h"
#include "json.h"


char* getProviderRequest() {
  char* fmt = "{\"request\":\"provider_list\"}";
  return fmt;
}

char* getAccessTokenRequest(const char* providername, unsigned long min_valid_period) {
  char* fmt = "{\"request\":\"access_token\", \"provider\":\"%s\", \"min_valid_period\":%lu}";
  char* request = calloc(sizeof(char), snprintf(NULL, 0, fmt, providername, min_valid_period)+1);
  sprintf(request, fmt, providername, min_valid_period);
  return request;
}

char* communicate(char* json_request) {
  static struct connection con;
  if(ipc_init(&con, OIDC_SOCK_ENV_NAME, 0)!=0) { 
    fprintf(stderr, "\n");
    return NULL; 
  }
  if(ipc_connect(con)<0) {
    fprintf(stderr, "Could not connect to oidc-agent\n");
    return NULL;
  }
  ipc_write(*(con.sock), "client:%s", json_request);
  char* response = ipc_read(*(con.sock));
  ipc_close(&con);
  if(NULL==response) {
    fprintf(stderr, "An unexpected error occured. It seems that oidc-agent has stopped.\nThat's not good.\n");
    exit(EXIT_FAILURE);
  }
  return response;

}

/** @fn char* getAccessToken(const char* providername, unsigned long min_valid_period) 
 * @brief gets an valid access token for a provider
 * @param providername the short name of the provider for whom an access token
 * should be returned
 * @param min_valid_period the minium period of time the access token has to be valid
 * in seconds
 * @return a pointer to the access token. Has to be freed after usage. On
 * failure NULL is returned and an error message is printed to stderr.
 */
char* getAccessToken(const char* providername, unsigned long min_valid_period) {
  char* request = getAccessTokenRequest(providername, min_valid_period);
  char* response = communicate(request);
  free(request);
  struct key_value pairs[3];
  pairs[0].key = "status";
  pairs[1].key = "error";
  pairs[2].key = "access_token";
  if(getJSONValues(response, pairs, sizeof(pairs)/sizeof(*pairs))<0) {
    fprintf(stderr, "Read malformed data. Please hand in bug report.\n");
    free(response);
    return NULL;
  }
  free(response);
  if(pairs[1].value) { // error
    fprintf(stderr, "Error: %s", pairs[1].value);
    free(pairs[0].value);
    free(pairs[1].value);
    free(pairs[2].value);
    return NULL;
  } else {
    free(pairs[0].value);
    return pairs[2].value;
  }
}

/** @fn char* getLoadedProvider()
 * @brief gets a a list of currently loaded providers
 * @return a pointer to the a comma separated list of the currently loaded
 * providers. Has to be freed after usage. On failure NULL is returned and
 * an error message is printed to stderr.
 */
char* getLoadedProvider() {
  char* request = getProviderRequest();
  char* response = communicate(request);
  struct key_value pairs[3];
  pairs[0].key = "status";
  pairs[1].key = "error";
  pairs[2].key = "provider_list";
  if(getJSONValues(response, pairs, sizeof(pairs)/sizeof(*pairs))<0) {
    fprintf(stderr, "Read malformed data. Please hand in bug report.\n");
    free(response);
    return NULL;
  }
  free(response);
  if(pairs[1].value) { // error
    fprintf(stderr, "Error: %s", pairs[1].value);
    free(pairs[0].value);
    free(pairs[1].value);
    free(pairs[2].value);
    return NULL;
  } else {
    free(pairs[0].value);
    return pairs[2].value;
  }
}

