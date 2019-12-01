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

char * toString(struct trajet t){
      char * res =malloc(sizeof(char)*50);
/*
      char str[ENOUGH];
      sprintf(str, "%d", 42);
  */  //  strcpy(res,(char *)t.num);

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
    //  strcat(res,(char *)t.prix);
      strcat(res,t.promo);


      return res;
}


int comparer(struct trajet t1,struct trajet t2){

    if(t1.num>0){
      //printf("NUM\n");
      if(t1.num != t2.num) return 0;
    }

    if(strcmp(t1.depart,"X")!=0){

      //printf("DEPART");
      if(strcmp(t1.depart,t2.depart)!=0) return 0;
    }

    if(strcmp(t1.arrivee,"X")!=0){
      //printf("ARRIVEE");
      if(strcmp(t1.arrivee,t2.arrivee)!=0) return 0;
    }
    if(strcmp(t1.heurdepart,"X")!=0){
      //printf("HEURDEPART");
      if(strcmp(t1.heurdepart,t2.heurdepart)!=0) return 0;
    }
    if(strcmp(t1.heurarrivee,"X")!=0){
      //printf("HEUREARRIVE");
      if(strcmp(t1.heurarrivee,t2.heurarrivee)!=0) return 0;
    }

    if(strcmp(t1.promo,"X\n")!=0){
      /*printf("PROMO  *%s* diff *%s* \n",t1.promo,t2.promo);
      */if(strcmp(t1.promo,t2.promo)!=0) return 0;
    }

    if(t1.prix <= 0.0){
      //printf("priX %f",t1.prix);
      if(t1.prix != t2.prix) return 0;
    }
  //  printf("*******************THE SAME*************************************");

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
    /*i=0;

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
*/
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

          //  printf("*%s", buf);
            struct trajet t ;
            t = readtrajet(buf);
/*
            printf(">>>>>>>>>> DEBUT <<<<<<<<<<<<\n");
            printf("numero  :%d*\n", t.num);
            printf("depart : %s*\n", t.depart);
            printf("arrivee  : %s*\n", t.arrivee);
            printf("heurdepart  : %s*\n", t.heurdepart);
            printf("heurarrivee  : %s*\n", t.heurarrivee);
            printf("prix  : %f*\n", t.prix);
            printf("promo  : %s*\n", t.promo);
            printf(">>>>>>>>>> FIN <<<<<<<<<<<<\n");
*/
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
            //tabtemp[index]=NULL;

            //printf("iiiiii %d\n",i);
            printf("Index  %d\n",index);

            char * res=malloc(sizeof(char)*MAXLINE);
            i=0;
            while(i<index){
          //    printf("whileeeeeee\n");

                if(i==0){
                  //printf(">>>>>>>>>> strcopy<<<<<<<<<<<<\n");

                  strcpy(res,toString(tabtemp[i]));
                  strcat(res,"\n");
                }else{
                  strcat(res,toString(tabtemp[i]));
                  strcat(res,"\n");
                }
                i++;
            }

          //  strcpy(c,"recieveeee!\n");
            printf("Res %s \n",res);
            res[strlen(res)-1]=0;
            send(acc, res, strlen(res),0);
          //  printf(">>>>>>>>>> FIN <<<<<<<<<<<<\n");

            /*if ((i=recv(acc, buf,MAXLINE, 0)) < 0)
            {
              printf("NNNNNNNNN");
            }
            else{
              printf("*%s", buf);
            }*/
            free(res);
            free(buf);
          }
        }else{

        }
    }
}
