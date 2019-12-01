#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "trajet.h"

#define MAXLINE 256

struct trajet readtrajet(char *line){
    struct trajet res;

    res.num = atoi(strtok(line,";"));

    res.depart = malloc(sizeof(char)*30);
    strcpy(res.depart, strtok(NULL,";"));

    res.arrivee = malloc(sizeof(char)*30);
    strcpy(res.arrivee, strtok(NULL,";"));

    res.heurdepart = malloc(sizeof(char)*6);
    strcpy(res.heurdepart, strtok(NULL,";"));

    res.heurarrivee = malloc(sizeof(char)*6);
    strcpy(res.heurarrivee, strtok(NULL,";"));

    sscanf((strtok(NULL,";")),"%lf",&res.prix);


    res.promo = malloc(sizeof(char)*10);
    strcpy(res.promo, strtok(NULL,";"));
//  printf("promo: %s\n", res.promo);

    return res;
}

int main(int argc, char **argv){


    //Stockage de données dans un tableau!

    FILE * fd = fopen("./test.txt","r");
    if(fd==NULL){
      printf("Erreur fichier \n" );
    }

    size_t len = 0;
    char *lineptr=NULL;
    int i=0;

    struct trajet  tab[50];
    while(getline(&lineptr, &len, fd)>0){
        struct trajet t ;
        t = readtrajet( lineptr );
        tab[i]=t;
        i++;
    }
    int longeur =i;
    i=0;

    while(i<longeur){
        printf("*********************%d*****************************\n",i);
        printf("numero  %d: %d\n",i, tab[i].num);
        printf("depart %d: %s\n",i, tab[i].depart);
        printf("arrivee %d: %s\n",i, tab[i].arrivee);
        printf("heurdepart %d: %s\n",i, tab[i].heurdepart);
        printf("prix %d: %f\n",i, tab[i].prix);
        printf("prix %d: %s***\n",i, tab[i].promo);
        i++;
    }
    printf("**************************************************");

    //creation Serveur!

    i=0;
    int port;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    port = atoi(argv[1]);

    struct sockaddr_in Sclient,Sserver;

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }else{
        printf("Socket successfully created..\n");
    }

    Sserver.sin_family = AF_INET;
    Sserver.sin_addr.s_addr = htonl(INADDR_ANY);
    Sserver.sin_port = htons(port);


    socklen_t Sclientlen = (socklen_t)sizeof(Sclient);

    int b = bind(sock, (const struct sockaddr * ) &Sserver, Sclientlen);

    if (b < 0) {
        printf("bind creation failed...\n");
        exit(0);
    }else{
        printf("bind successfully created..\n");
    }

  int list =  listen(sock, 1);
    if (list <0) {
        printf("listen creation failed...\n");
        exit(0);
    }else{
        printf("listen successfully created..\n");
    }



    while (1) {
        int acc = accept(sock, (struct sockaddr *) &Sclient, &Sclientlen);

        if (acc == -1) {
            printf("accept creation failed...\n");
            exit(0);
        }else{
            printf("accept successfully created..\n");
        }

        if(fork()==0){
          while (1) {
            /* code */
            char *buf;
            int i = 0;
            buf = malloc(MAXLINE * sizeof(char));

            //Recuperation de trajet demande
            i=recv(acc, buf,MAXLINE, 0);

            printf("*%s", buf);
            buf[strlen(buf)-1]=0;
            struct trajet t ;
            t = readtrajet(buf);

            printf(">>>>>>>>>> DEBUT <<<<<<<<<<<<\n");
            printf("numero  :%d*\n", t.num);
            printf("depart : %s*\n", t.depart);
            printf("arrivee  : %s*\n", t.arrivee);
            printf("heurdepart  : %s*\n", t.heurdepart);
            printf("heurarrivee  : %s*\n", t.heurarrivee);
            printf("prix  : %f*\n", t.prix);
            printf("promo  : %s*\n", t.promo);
            printf(">>>>>>>>>> FIN <<<<<<<<<<<<\n");

/*          Recherche des trajets correspondants
            struct trajet  tabtemp[longeur];
            i=0;
            while(i<longeur){
                if(t.num)
                i++;
            }
*/
            char * c=malloc(sizeof(char)*MAXLINE);
            strcpy(c,"recieveeee!\n");
            send(acc, c, strlen(c),0);
            /*if ((i=recv(acc, buf,MAXLINE, 0)) < 0)
            {
              printf("NNNNNNNNN");
            }
            else{
              printf("*%s", buf);
            }*/
            free(buf);
          }
        }else{

        }
    }
}
