// Ciupitu Dennis-Mircea 323CA
#include <stdlib.h>   
#include <cstdlib>
#include <stdio.h>
#include <unistd.h>     
#include <string.h>     
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <netdb.h>      
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include <vector>
#include <string>

char *compute_post_request(char *host, char *url, char* type, char *form_data, std::vector<std::string>& cookies, char* authorization)
{

    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));
    
    // adaugare url si intializare cerere
    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);

    // adaugare host
    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // adaugare cookie-uri daca exista
    if (cookies.size() > 0) {
        memset(line, 0, LINELEN);
        std::string aux = "Cookie: " + cookies[0];
        for (unsigned int i = 1; i < cookies.size(); i++) {
            aux += "; " + cookies[i];
        }
        compute_message(message, aux.c_str());
    }

    // daca este necesara autorizare
    if (authorization != NULL) {
        std::string aux = "Authorization: Bearer " + std::string(authorization);
        compute_message(message, aux.c_str());
    }


    // adaugarea Content-Type
    memset(line, 0, LINELEN);
    sprintf(line, "Content-Type: %s", type);
    compute_message(message, line);

    // adaugare Content-Length
    memset(line, 0, LINELEN);
    sprintf(line, "Content-Length: %ld", strlen(form_data));
    compute_message(message, line);

    // adaugare linie goala
    memset(line, 0, LINELEN);
    compute_message(message, line);

    // adaugarea datelor ce trebuiesc trimise
    memset(line, 0, LINELEN);
    sprintf(line, "%s", form_data);
    compute_message(message, line);

    return message;
}

char *compute_get_request(char *host, char *url, char *form_data, std::vector<std::string>& cookies, char* authorization)
{

    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));

    // creaza cererea in caz ca exista parametrii
    if (form_data != NULL)
    {
        sprintf(line, "GET %s?%s HTTP/1.1", url, form_data);
    }
    else
    {
        sprintf(line, "GET %s HTTP/1.1", url);
    }
    compute_message(message, line);

    // adauga hostul
    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    
    // adauga cookie-uri daca exista
    if (cookies.size() > 0) {
        memset(line, 0, LINELEN);
        std::string aux = "Cookie: " + cookies[0];
        for (unsigned int i = 1; i < cookies.size(); i++) {
            aux += "; " + cookies[i];
        }
        compute_message(message, aux.c_str());
    }

    // daca este necesara autorizare
    if (authorization != NULL) {
        std::string aux = "Authorization: Bearer " + std::string(authorization);
        compute_message(message, aux.c_str());
    }

    // printam linie goala
    memset(line, 0, LINELEN);
    compute_message(message, line);

    return message;
}
