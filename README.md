# Client-HTTP
Created a client that interacts with a REST API

Pentru rezolvarea fiecarui task am pornit mereu conexiunea
 cu serverul pe ip-ul "185.118.200.35" si portul 8081, inchizand-o
 la terminarea fiecarui task. Functiile {open_connection, compute_get_request,
 send_to_server, receive_from_server, close_connection} realizeaza
 comunicarea cu serverul si cu clientul, aceste functii fiinf esentiale
 pentru trimiterea requesturilor si primirea raspunsurilor de la server.

 	---BIBLIOTECA FOLOSITA---
 	Pentru parsarea textelor in format JSON am folosit biblioteca rapidjson

 	--AFISAREA MESAJELOR--
 	La stdout sunt afisate toate requesturile facute de client pe rand si
 toate raspunsurile date de server pentru fiecare request in parte.

 	---TASK 1---
 		In aceasta parte a programului se realizeaza un simplu get la
 	serverul specificat in enuntul temei, requestul si raspunsul
 	fiind afisate la stdout.

 	---TASK 2---
 		In aceasta parte a programului gasesc toate cookie-urile primite
 	in raspunsul anterior de la server, si interpretez textul JSON prin
 	functia Parse, obtinand toate campurile necesare.
 		Apoi se construieste mesajul care va fi trimis catre sever,
 	urmandu-se apoi pasii necesari pentru trimiterea si primirea
 	mesajelor de la client-server.

 	---TASK 3---
 		In aceasta parte a programului se realizeaza o cerere de GET
 	pe baza unei chei de autentificare obtinute prin manipularea
 	textului JSON, si pe baza unui ID. Cu toate aceste informatii
 	se creeaza o cerere de get care va fi trimisa catre server.

 	---TASK 4---
 		Partea aceasta a programului, seamana in mare parte cu cea de la
 	TASK 1, realizandu-se de asemenea o cerere de get, doar ca
 	de data asta lista de parametrii a cereri va contine si id-ul
 	obtinut la pasul anterior.

 	---TASK 5---
 		In ultima parte a programului, se realizeaza mai intai o
 	cerere de GET pentru serverul, al carui url se obtine din
 	raspuns-ul primit anterior. Din url-ul acesta, prima parte
 	este reprezentata de host(un nou ip), iar a doua parte este
 	reprezentata de lista de parametrii ai cererii.
 		In urma cererii, se primeste de la server un mesaj JSON,
 	care va fi trimis catre serverul specificat intial, printr-o
 	cerere de POST.

 	OBS! - vectorul de cookie-uri este resetat la fiecare task
 	- unele functii sunt preluate in urma realizarii laboratoarelor
