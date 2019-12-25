/*
 *Authors:
 ********DIALLO Mariama**************
 ********KABAD Soufiane**************
 ********BOUKRIS WALID***************
 ********SOW OUSMANE*****************
 **Projet Système/Réseaux: L3 MIAGE**
 *************2019-2020**************
 */

#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>

#define MAXLINE 256


int main(int argc, char **argv){

    int port;
    char *host;

    //Verification de nombres d'arguments
    if (argc != 3) {
        fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
        exit(0);
    }

    host = argv[1];
    port = atoi(argv[2]);

    struct sockaddr_in s;
    struct hostent * h;

    //Creation de socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }else{
        printf("Socket successfully created..\n");
    }

    //avec la macro h_addr qui correspond à h_addr_list[0].
    h = gethostbyname(host);



    //Remplissage de struct sockaddr_in s
    s.sin_family = AF_INET;
    s.sin_addr = *(struct in_addr *) h->h_addr;
    s.sin_port = htons(port);

    //Recuperation de la longeur de struct sockaddr_in s
    socklen_t slen = (socklen_t)sizeof(s);

    //Creation de connect
    int con = connect(sock, (struct sockaddr *) &s, slen);

    if (con == -1) {
        printf("connect creation failed...\n");
        exit(0);
    }else{
        printf("connect successfully created..\n");
    }

    printf("Entrer vos caracteristique de train sous la forme :\n");
    printf("Numero;VilleDepart;VileArrivee;HeureDepartMin;HeureDepartMax;Prix;Promo\n");
    printf("si vous ne voulez pas remplir un champ mettez X!\n");

    while (1) {
      char *buff=malloc(sizeof(char)*MAXLINE);
      printf("\n\n*******************************************************\n" );
      //Stockage de la requet de client dans buff
      fgets(buff,sizeof(char)*100,stdin);
      while((strcmp(buff,"\n")==0)){
        fgets(buff,sizeof(char)*100,stdin);
      }
      if(strlen(buff)> 0)
      {
        //Envoie de requet de client
        send(sock, buff, strlen(buff),0);
        char * buffReseau=malloc(sizeof(char)*MAXLINE);


        printf("\n*********voulez-vous que ça soit trier par rapport au prix(p)/dure(d)**************\n\n" );
        //Stockage de choix de tri prix ou duree ou rien
        fgets(buff,sizeof(char)*100,stdin);
        send(sock, buff, strlen(buff),0);
        //Receptions des resultats
        recv(sock, buffReseau,MAXLINE*10, 0);


        printf("\n*********Voici nos resultats**************\n\n" );


        int i=0;
        //Affichage des resultats!
        while (buffReseau[i]!=0) {
          printf("%c", buffReseau[i]);
          buffReseau[i]=0;
          i++;
        }
        free(buffReseau);
      }
      free(buff);
    }
}
