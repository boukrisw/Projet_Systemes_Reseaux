#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "trajet.h"

#define MAXLINE 256


void Free(struct trajet t){
      free(t.depart);
      free(t.arrivee);
      free(t.heurdepart);
      free(t.heurarrivee);
      free(t.promo);
}

struct trajet readtrajet(char *line){
        struct trajet res;

        char * s = malloc(sizeof(char)*8);
        strcpy(s, strtok(line,";"));
        if(strcmp(s,"X")==0){
          res.num = -1;
        }else{
          res.num = atoi(s);
        }

        res.depart = malloc(sizeof(char)*30);
        strcpy(res.depart, strtok(NULL,";"));

        res.arrivee = malloc(sizeof(char)*30);
        strcpy(res.arrivee, strtok(NULL,";"));

        res.heurdepart = malloc(sizeof(char)*6);
        strcpy(res.heurdepart, strtok(NULL,";"));

        res.heurarrivee = malloc(sizeof(char)*6);
        strcpy(res.heurarrivee, strtok(NULL,";"));

        strcpy(s, strtok(NULL,";"));
        if(strcmp(s,"X")==0){
          res.prix = -1.0;
        }else{
          sscanf(s,"%lf",&res.prix);
        }
        free(s);

        res.promo = malloc(sizeof(char)*6);
        strcpy(res.promo, strtok(NULL,";"));
        char cmp[2];
        cmp[0]=res.promo[0];
        cmp[1]=0;
        if(strcmp(cmp,"\n")==0){
          res.promo[0]=0;
        }if(strcmp(cmp,"X")==0){
          res.promo[1]=0;
        }else{
          res.promo[5]=0;
        }

        return res;
}

char * toString(struct trajet t){
      char * res =malloc(sizeof(char)*50);

      sprintf(res, "%d", t.num);
      strcat(res," ");

      strcat(res,t.depart);
      strcat(res," ");

      strcat(res,t.arrivee);
      strcat(res," ");

      strcat(res,t.heurdepart);
      strcat(res," ");

      strcat(res,t.heurarrivee);
      strcat(res," ");

      char *s=malloc(sizeof(char)*5);
      sprintf(s, "%.2f", t.prix);
      strcat(res,s);
      strcat(res," ");

      strcat(res,t.promo);
      return res;
}


int comparer(struct trajet t1,struct trajet t2){

    if(t1.num>0){
      if(t1.num != t2.num) return 0;
    }

    if(strcmp(t1.depart,"X")!=0){
      if(strcmp(t1.depart,t2.depart)!=0) return 0;
    }

    if(strcmp(t1.arrivee,"X")!=0){
      if(strcmp(t1.arrivee,t2.arrivee)!=0) return 0;
    }
    if(strcmp(t1.heurdepart,"X")!=0){
      if(strcmp(t1.heurdepart,t2.heurdepart)!=0) return 0;
    }
    if(strcmp(t1.heurarrivee,"X")!=0){
      if(strcmp(t1.heurarrivee,t2.heurarrivee)!=0) return 0;
    }

    if(t1.prix > 0.0){
      if(t1.prix != t2.prix) return 0;
    }

    if(strcmp(t1.promo,"REDUC")==0 | strcmp(t1.promo,"SUPPL")==0){
      if(strcmp(t1.promo,t2.promo)!=0 ) return 0;
    }

    return 1;
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
            char *buf;
            int i = 0;
            buf = malloc(MAXLINE * sizeof(char));

            //Recuperation de trajet demande
            i=recv(acc, buf,MAXLINE, 0);

            struct trajet t ;
            t = readtrajet(buf);

            //  Recherche des trajets correspondants
            struct trajet  tabtemp[longeur+1];
            i=0;
            int index=0;
            while(i<longeur){
                int same = comparer(t,tab[i]);
                if(same){
                  tabtemp[index] = tab[i];
                  index++;
                }
                i++;
            }

            char * res=malloc(sizeof(char)*MAXLINE);
            i=0;
            while(i<index){
                if(i==0){
                  strcpy(res,toString(tabtemp[i]));
                  strcat(res,"\n");
                }else{
                  strcat(res,toString(tabtemp[i]));
                  strcat(res,"\n");
                }
                i++;
            }

            if(index==0){
                res="Pas de resultats";
            }else{
               res[strlen(res)-1]=0;
            }
            send(acc, res, strlen(res),0);
            Free(t);
            free(res);
            free(buf);
          }
        }else{

        }
    }
}
