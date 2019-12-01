#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>

#define MAXLINE 256


int main(int argc, char **argv){

    int port;
    char *host;

    if (argc != 3) {
        fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
        exit(0);
    }

    host = argv[1];
    port = atoi(argv[2]);

    struct sockaddr_in s;
    struct hostent * h;


    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }else{
        printf("Socket successfully created..\n");
    }

    //avec la macro h_addr qui correspond à h_addr_list[0].
    h = gethostbyname(host);


    s.sin_family = AF_INET;
    s.sin_addr.s_addr = htonl(INADDR_ANY);
    s.sin_port = htons(port);

    socklen_t slen = (socklen_t)sizeof(s);

    int con = connect(sock, (struct sockaddr *) &s, slen);

    if (con == -1) {
        printf("connect creation failed...\n");
        exit(0);
    }else{
        printf("connect successfully created..\n");
    }

    while (1) {
      char *buff=malloc(sizeof(char)*MAXLINE);
      fgets(buff,sizeof(char)*100,stdin);
      if(strlen( buff)> 0)
      {
        //printf("buf = %s \n", buff);
        send(sock, buff, strlen(buff),0);

        char *buffReseau=NULL;
        buffReseau=malloc(sizeof(char)*MAXLINE);
        recv(sock, buffReseau,MAXLINE, 0);

        printf("buffReseau = %s\n", buffReseau);
        free(buffReseau);
      }
      free(buff);
    }
}
