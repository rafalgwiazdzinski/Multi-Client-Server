#include <stdio.h>
#include <sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc , char *argv[])
{
    if(argc <= 2) {
        perror("Blad uruchomienia klienta! Podaj poprawny adres IP oraz port po spacji i sprobuj ponownie");
        return 0;
    } else if(argc > 3) {
        perror("Blad uruchomienia klienta! Zbyt duzo parametrow, wystarczy jedynie adres IP oraz port ;)");
        return 0;
    }
    int socket_desc = -1;
    struct sockaddr_in server;
    char *message , server_reply[2000];
    socket_desc = socket(AF_INET , SOCK_STREAM , 0); // gniazdo strumieniowe (transmisja TCP)
    if (socket_desc == -1){
        printf("Nie moge utworzyc gniazda \n");
    } else {
        printf("Utworzono gniazdo o numerze: %d \n", socket_desc);
    }
//adres serwera zdalnego
//server.sin_addr.s_addr = inet_addr("172.217.20.206");
    //server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_family = AF_INET;
//server.sin_port = htons( 80 );
    server.sin_port = htons( atoi(argv[2]) );
//Polacz z serwerem zdalnym (TCP)
    if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0) {
        puts("Blad polaczenia");
        return 1;
    }
    puts("Polaczono");
    sleep (1);
/*#if 1
    message = "GET / HTTP/1.1\r\n\r\n";
    if( send(socket_desc , message , strlen(message) , 0) < 0) {
        puts("Blad wyslania danych");
        return 1;
    }
    puts("Dane wyslano\n");
#endif*/
    while (1) {

//wysylanie
        char number[256];
        fgets(number, sizeof(number), stdin);
        if( send(socket_desc , number , strlen(number) , 0) < 0) {
            puts("Blad wyslania danych");
            return 1;
        }
        puts("Dane wyslano\n");
        //Nasluchiwanie i odbior danych z serwera (TCP)
        if( recv(socket_desc, server_reply , 2000 , 0) < 0){
            puts("Blad odbioru danych");
        }
        //puts("Dane odebrano\n");
        puts(server_reply);

        if(strstr(server_reply, "Wynik") != NULL) {
            //puts(server_reply);
            puts("\nCzy chcesz zakonczyc prace klienta [t/n]?\n");
            fgets(number, sizeof(number), stdin);
            if(number[0] == 't') {
                if( send(socket_desc , number , strlen(number) , 0) < 0) {
                    puts("Blad wyslania danych");
                    return 1;
                }
                break;
            }
        }
        memset (server_reply, 0, sizeof (server_reply));
        sleep (1);
    }
    close(socket_desc);
    return 0;
}