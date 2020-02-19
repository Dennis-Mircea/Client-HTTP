// Ciupitu Dennis-Mircea 323CA
#include <stdio.h>      
#include <iostream>
#include <stdlib.h>     
#include <unistd.h>     
#include <string.h>     
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <netdb.h>      
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include <sstream>
#include <vector>


// functie care itereaza prin text(antet) si cauta cooke-uri in functie de caz
void getCookies(std::string& tmp, std::stringstream& text, std::vector<std::string>& cookies) {
    getline(text, tmp, '\n');
    while (tmp.find("{") > 0) { // pana cand ajunge la textul JSON
        getline(text, tmp, '\n'); // obtinerea liniei
        if (tmp.find("Set-Cookie: ") == 0) { // daca contine cookie il baga in vector
            std::stringstream aux(tmp.substr(12).c_str());
            std::string mess;
            getline(aux, mess, ';');
            cookies.push_back(mess);
        }

    }
}

// functie care obtine toate ip-urile in urma inteorgarii dns
void get_ips(char* name, std::vector<std::string>& ips)
{
    struct addrinfo hints, *result, *p;

    // setarea hint-urilor
    memset(&hints, 0, sizeof(hints));
    // obtinerea adreselor
    getaddrinfo(name, NULL, &hints, &result);

    char dns[100];

    // iteram printre adrese
    for (p = result; p != NULL; p = p->ai_next) {
        if (p->ai_family == AF_INET) {
            struct sockaddr_in *addr = (struct sockaddr_in*) p->ai_addr;
            
            inet_ntop(p->ai_family, &addr->sin_addr, dns, sizeof(dns)); // obtinere adresa ip
            
            // introducere adresa ip in vector
            std::string elem(dns);
            ips.push_back(elem);
        }
        
    }
}

int main(int argc, char *argv[])
{
    char *message;
    int sockfd;
    unsigned int i;
    char *application, *url, *tologin, *token;
    std::vector<std::string> cookies;

    char* ip_server = strdup("185.118.200.35");
    url = strdup("/task1/start");

    // TASK1
    sockfd = open_connection(ip_server, 8081, PF_INET, SOCK_STREAM, 0); // deschiderea conexiunii
    message = compute_get_request(ip_server, url, NULL, cookies, NULL); // compunerea mesajului

    printf("\nREQUEST: %s\n", message); // mesajul pe care il trimite clientul
    
    send_to_server(sockfd, message); // trimiterea messajului catre server
    message = receive_from_server(sockfd); // mesajul primit de la server
    close_connection(sockfd); // inchiderea conexiunii

    printf("\nRESPONSE: %s\n", message); // mesajul primit de la server

    free(url);

    // TASK 2
    std::string tmp = "";
    std::stringstream text(message); // obtinem mesajul sub forma de string
    getCookies(tmp, text, cookies); // scanam antetul mesajului si adaugam cookie-uri daca este nevoie

    // parsarea textului JSON
    rapidjson::Document document;
    document.Parse(tmp.c_str());

    // obtinerea mesajului
    std::string x = document["url"].GetString();  
    url = strdup(x.c_str());

    // obtinerea tipului aplicatiei
    x = document["type"].GetString();   
    application = strdup(x.c_str());

    // obtinerea mesajului care va fi trimis in cererea de POST este stocat in tologin
    std::stringstream message_to_send; // mesaj de tipul "username=xxx&pasword=yyy"
    message_to_send << "username=" << document["data"]["username"].GetString()
                     << "&password=" << document["data"]["password"].GetString();
    std::string to_send = message_to_send.str(); // convertire catre string

    tologin = strdup(to_send.c_str()); // convertire la char*

    sockfd = open_connection(ip_server, 8081, PF_INET, SOCK_STREAM, 0); // conectare la server
    message = compute_post_request(ip_server, url, application, tologin, cookies, 0); // crearea mesajului

    printf("\nREQUEST: %s\n", message); // requestul de la client

    // eliberarea resurselor
    free(url);
    free(application);
    free(tologin);

    send_to_server(sockfd, message); // trimiterea mesajului catre server
    message = receive_from_server(sockfd); // mesajul primit de la server

    printf("\nRESPONSE: %s\n", message); // raspunsul de la server
    close_connection(sockfd); // inchiderea conexiunii


    // TASK 3
    cookies.clear();
    tmp = "";
    std::stringstream text3(message); // obtinem mesajul sub forma de string
    getCookies(tmp, text3, cookies); // scanam antetul mesajului si adaugam cookie-uri daca este nevoie

    // parsarea textului JSON
    rapidjson::Document document2;
    document2.Parse(tmp.c_str());

    // obtinerea type-ului
    x = document["type"].GetString();
    application = strdup(x.c_str());

    // obtinem url-ul
    x = document2["url"].GetString();
    url = strdup(x.c_str());

    // obtinem cheia de autentificare
    x = document2["data"]["token"].GetString();
    token = strdup(x.c_str());

    // obtinem id-ul de autentificare
    x = document2["data"]["queryParams"]["id"].GetString();
    char* id = strdup(x.c_str());

    // obtinem lista de parametrii
    std::string response = "raspuns1=omul&raspuns2=numele&id=" + std::string(id);
    tologin = strdup(response.c_str());

    sockfd = open_connection(ip_server, 8081, PF_INET, SOCK_STREAM, 0); // conectare la server
    message = compute_get_request(ip_server, url, tologin, cookies, token); // crearea mesajului

    printf("\nREQUEST: %s\n", message); // afisam requestul

    send_to_server(sockfd, message); // trimitere mesaj
    message = receive_from_server(sockfd); // primire raspuns

    printf("\nRESPONSE: %s\n", message); // afisam raspunsul de la server
    close_connection(sockfd); // inchidem conexiunea

    free(tologin); // eliberam spatiul de date
    free(url);

    // TASK 4
    cookies.clear(); // golim vectorul de cookie
    tmp = "";
    std::stringstream text4(message); // obtinem mesajul sub forma de string
    getCookies(tmp, text4, cookies); // scanam antetul mesajului si adaugam cookie-uri daca este nevoie

    // parsam textul ramas
    rapidjson::Document document3;
    document3.Parse(tmp.c_str());

    // obtinem url-ul serverului
    x = document3["url"].GetString();
    url = strdup(x.c_str());

    // cream lista de parametrii
    response = "id=" + std::string(id); // adaugam id-ul primit la task-ul anterior
    tologin = strdup(response.c_str());

    sockfd = open_connection(ip_server, 8081, PF_INET, SOCK_STREAM, 0); // conectare la server
    message = compute_get_request(ip_server, url, tologin, cookies, token); // obtinerea mesajului

    printf("\nREQUEST: %s\n", message); // requestul

    send_to_server(sockfd, message); // trimite mesajul la server
    message = receive_from_server(sockfd); // primeste raspuns de la server

    printf("\nRESPONSE: %s\n", message); // raspuns de la server
    close_connection(sockfd); // inchide conexiunea

    free(tologin); // eliberam spatiul de date
    free(url);
    free(application);

    // TASK 5
    cookies.clear(); // golim vectorul de cookie
    tmp = "";
    std::stringstream text5(message); // obtinem mesajul sub forma de string
    getCookies(tmp, text5, cookies); // scanam antetul mesajului si adaugam cookie-uri daca este nevoie

    // parsam textul ramas
    rapidjson::Document document5;
    document5.Parse(tmp.c_str()); 

    // url-ul server-ului catre care trebuiesc trimise datele finale
    x = document5["url"].GetString();
    url = strdup(x.c_str());

    // type-ul server-ului catre care trebuiesc trimise datele finale
    x = document5["type"].GetString();
    application = strdup(x.c_str());

    // GET-ul pentru vreme
    x = document5["data"]["url"].GetString(); // url-ul serverului pentru vreme
    // obtinem adresa hostului
    std::string host = x.substr(0, x.find("/"));
    // obtinem url-ul serverului pentru vreme
    char* weather_url = strdup(x.substr(x.find("/")).c_str());

    // extragem parametrii din queryParams
    std::string string1 = document5["data"]["queryParams"]["q"].GetString();
    std::string string2 = document5["data"]["queryParams"]["APPID"].GetString();

    // cream lista de parametrii
    std::string params = "q=" + string1 + "&APPID=" + string2;
    tologin = strdup(params.c_str());


    char *ip_address = strdup(host.c_str());
    std::vector<std::string> ips; // ip-urile ce vor rezulta din interogarea dns a serverului

    get_ips(ip_address, ips); // obtinerea ip-urilor in urma interogarii dns

    // verifica care ip este bun pentru conexiune
    char* ip;
    for (i = 0; i < ips.size(); i++) {
        ip = strdup(ips[i].c_str());
        sockfd = open_connection(ip, 80, PF_INET, SOCK_STREAM, 0); // incearca conectarea la serverul pentru vreme
        if (sockfd > 0) { // daca a gasit ip-ul bun, se opreste
            break;
        } else { // eliberam memoria in caz contrar
            free(ip);
        }
    }

    std::vector<std::string> nothing; // vector gol

    message = compute_get_request(ip, weather_url, tologin, nothing, NULL); // obtine mesajul care va fi ulterior trimis
    send_to_server(sockfd, message); // trimite mesajul
    message = receive_from_server(sockfd); // primeste raspunsul de la server

    close_connection(sockfd); // inchide conexiunea cu serverul pentru vreme


    // informatiile primite de la server cu vremea
    std::vector<std::string> cookies5;
    tmp = "";
    std::stringstream textWeather(message);
    getCookies(tmp, textWeather, cookies5);

    // stocam mesajul in formal JSON
    tologin = strdup(tmp.c_str());

    sockfd = open_connection(ip_server, 8081, PF_INET, SOCK_STREAM, 0);
    message = compute_post_request(ip_server, url, application, tologin, cookies, token);

    printf("\nREQUEST: %s\n", message); // requestul

    send_to_server(sockfd, message); // trimiterea mesajului catre server
    message = receive_from_server(sockfd); // primirea raspunsului

    printf("\nFINAL RESPONSE: %s\n", message); // raspunsul de la server
    close_connection(sockfd); // inchiderea conexiunii

    // eliberarea resurselor alocate
    free(message);
    free(url);
    free(id);
    free(application);
    free(token);
    free(ip);
    free(ip_server);
    free(weather_url);

    return 0;
}