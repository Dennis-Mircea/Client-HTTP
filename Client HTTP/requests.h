// Ciupitu Dennis-Mircea 323CA
#include <vector>
#include <string>

#ifndef _REQUESTS_
#define _REQUESTS_

char *compute_post_request(char *host, char *url, char *application, char *form_data, std::vector<std::string>& cookies, char* authorization);
char *compute_get_request(char *host, char *url, char *form_data, std::vector<std::string>& cookies, char* authorization);
#endif