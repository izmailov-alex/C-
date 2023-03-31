#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>

#include <netdb.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <string.h>    //strlen
#include <stdlib.h>    //strlen
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <pthread.h>   //for threading , link with lpthread
#include <signal.h>
#include <ctype.h>      //isspace

#include "lgraph.cpp"
using namespace std;
int socket_desc;
void handler (int signo){
    cout<<"end"<<endl;
    exit(0);
}

int main (int argc, char const *argv[]) {
    char **mass = NULL;
    char *buf = NULL;
    int sizelist = 0, curlist = 0, sizebuf = 0, curbuf = 0;
    int n, port;
    struct sockaddr_in server;
    struct hostent *phe;
    int f = open("log_client", O_WRONLY | O_TRUNC | O_CREAT, 0777);
    if (argc != 3) {
        fprintf(stderr, "Parameters are wrong!\n");
        exit(1);
    }

    // получение адреса компьютера
    if (!(phe = gethostbyname(argv[1]))) {
        fprintf(stderr, "Bad host name: %s\n", argv[1]);
        exit(1);
    }

    // номер порта (порт с 1 до 1023 - служебные, не используются)
    if (sscanf(argv[2], "%d%n", &port, &n) != 1 || argv[2][n] || port <= 1023 || port > 65535) {
        fprintf(stderr, "Bad port number: %s\n", argv[2]);
        exit(1);
    }

    // создание сокета
    socket_desc = socket (PF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        fprintf(stderr, "Could not create socket\n");
        exit (1);
    }

    server.sin_family = PF_INET;
    server.sin_port = htons (port);
    memcpy(&server.sin_addr, phe->h_addr_list[0], sizeof(server.sin_addr));


    if (connect (socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0){
        fprintf(stderr, "Connect failed\n");
        exit (1);
    }

    signal(SIGINT, handler); 
    signal(SIGTSTP, handler);

    cout<<"socket_desc = "<<socket_desc<<endl; 
    while (1){
        if(socket_desc == 0) return 0;
        fd_set readfd;
        FD_ZERO (&readfd);
        FD_SET (0, &readfd);
        FD_SET (socket_desc, &readfd);

        if (select(socket_desc + 1, &readfd, NULL, NULL, NULL) <= 0){
            perror("select");
            exit(1); // корректный выход
        }
        if (FD_ISSET(socket_desc, &readfd)) {
            lex(&mass, buf, &sizelist, &curlist, &sizebuf, &curbuf, socket_desc);
            if(curlist == 0){
                close(socket_desc);
                clearlist(mass, &sizelist, &curlist);
                continue;
            }
            printlist(&mass, &sizelist, 1, f);
            clearlist(mass, &sizelist, &curlist);
        }
        if (FD_ISSET(0, &readfd)) {
            lex(&mass, buf, &sizelist, &curlist, &sizebuf, &curbuf, 0);
            if(curlist == 0){
                clearlist(mass, &sizelist, &curlist);
                continue;
            }
            printlist(&mass, &sizelist, socket_desc, f);
            if(curlist >= 2 && strcmp(mass[1],"quit")==0){
                shutdown(socket_desc, 2);
                socket_desc = 0;
            }
            clearlist(mass, &sizelist, &curlist);
        }
    }
}