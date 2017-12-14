#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

struct lista {
    struct lista *next;
    int val;
};


int rosnaco(struct lista *wiazana);

void malejaco();

int sumowanie(struct lista *wiazana);

int odejmowanie(struct lista *wiazana);

int main(int argc , char *argv[])
{
    int socket_desc , new_socket , c;
    struct sockaddr_in server , client;
    int read_size;
    char *message;
//Utworz gniazdo
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
//Przygotuj addres gniazda
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
//Polacz deskrypor gniazda (gniazdo) z przypisanym adresem
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("Problem dolaczenia gniazda");
    }
    puts("Dolaczono gniazdo strumieniowe");
//Nasluchiwanie na gniezdzie (serwer)
    listen(socket_desc , 3);
//Akceptuj polaczenie przychodzace
    puts("SERWER TCP: Czekam na polaczenie...");
    c = sizeof(struct sockaddr_in);

    int pid;
    struct lista *listaWiazana;
    struct lista *staraListaWiazana;
    staraListaWiazana = NULL;
    while (1) {
        new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
        //fcntl(new_socket, F_SETFL, O_NONBLOCK);
        if (new_socket<0)
        {
            perror("Problem z akceptacja poloczenia");
        }
        puts("Polaczenie zaakceptowane");
//Odpowiedz do klienta
        //message = "Witam, poloczeyles sie z serwerem TCP. Czekam na dane...\n";
        //write(new_socket , message , strlen(message));
        pid = fork();
        if (pid < 0) {
            perror("ERROR in new process creation");
            _exit(1);
        }
        if (pid == 0) {

            //close(socket_desc);
            char client_message[3000];
            char message_result[3000];
            int value = 0;
            int wynik = 0;
            while ((read_size = recv(new_socket, client_message, 3000, 0)) > 0) {
                printf("SERWER: Dane odebrano\n");
                puts(client_message);
                value = atoi(client_message);
                if(value != 0) {
                    listaWiazana = malloc (sizeof(struct lista));
                    listaWiazana->val = value;
                    listaWiazana->next = staraListaWiazana;
                    staraListaWiazana = listaWiazana;
                }
                if (strstr(client_message, "rosnaco") != NULL) {
                    //wynik = rosnaco(listaWiazana);
                    //puts(wynik);

                    //snprintf(message_result, 3000, "Wynik: %d\n", wynik);
                    //message = "Wynik: jakis wynik\n";
                    //write(new_socket, message_result, strlen(message_result));
                } else if (strstr(client_message, "malejaco") != NULL){
                    malejaco();
                } else if (strstr(client_message, "sumowanie") != NULL){
                    wynik = sumowanie(listaWiazana);
                    snprintf(message_result, 3000, "Wynik: %d\n", wynik);
                    write(new_socket, message_result, strlen(message_result));
                } else if (strstr(client_message, "odejmowanie") != NULL){
                    wynik = odejmowanie(listaWiazana);
                    snprintf(message_result, 3000, "Wynik: %d\n", wynik);
                    write(new_socket, message_result, strlen(message_result));
                } else if (client_message[0] == 't') {
                    kill(getpid(), SIGKILL);
                } else {
                    message = "Potwierdzenie odebrania danych przez serwer\n";
                    write(new_socket, message, strlen(message));
                }
                //message = "Info od serwera: odebralem dane";
//Wyslij echo
                memset (client_message, 0, sizeof (client_message));
                //close(new_socket);
            }
        } else {
            //waitpid(-1, NULL, 0);
            close(new_socket);
        }
    }
    return 0;
}

int odejmowanie(struct lista *wiazana) {
    struct lista *wsk = wiazana;
    int suma = 0;
    while (wsk != NULL) {
        suma = suma - wsk->val;
        wsk = wsk->next;
        if(wsk->next == NULL) {
            suma = wsk->val + suma;
            break;
        }
    }
    return suma;
}

int  sumowanie(struct lista *wiazana) {
    struct lista *wsk = wiazana;
    int suma = 0;
    while (wsk != NULL) {
        suma = suma + wsk->val;
        wsk = wsk->next;
    }
    return suma;
}

void malejaco() {

}

int rosnaco(struct lista *wiazana) {

}

void send_result(char * message) {

}