/*
 *Authors:
 ********DIALLO Mariama**************
 ********KABAD Soufiane**************
 ********BOUKRIS WALID***************
 ********SOW OUSMANE*****************
 **Projet Système/Réseaux: L3 MIAGE**
 *************2019-2020**************
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "trajet.h"
#include <signal.h>
#include <sys/wait.h>


#define MAXLINE 256

/*
  recuperation des fils Zombies
*/

void traitement(int sig){
    wait(NULL);
}

/*
  liberation de l'allocation memoire de la structure trajet
*/
void Free(struct trajet t){
      free(t.depart);
      free(t.arrivee);
      free(t.heurdepart);
      free(t.heurarrivee);
      free(t.promo);
}

/*
  Determine la duree entre deux trajets
*/
int durre(char * strh1, char * strh2){

   char * s1 = malloc(sizeof(char)*5);
   strcpy(s1,strh1);

   int h1= atoi(strtok(s1,":"));
   int m1= atoi(strtok(NULL,":"));

   char * s2 = malloc(sizeof(char)*2);
   strcpy(s2,strh2);

   int h2= atoi(strtok(s2,":"));
   int m2= atoi(strtok(NULL,":"));
   int res = (h2-h1)*60;
   res = res + abs(m2-m1);

   return res;

 }


/*
   compare deux heures
   renvoie O si: strh2 < strh1
   renvoie 1 si: strh1 <= strh2
*/
int comparerHeure(char * strh1, char * strh2){

   char * s1 = malloc(sizeof(char)*5);
   strcpy(s1,strh1);

   int h1= atoi(strtok(s1,":"));
   int m1= atoi(strtok(NULL,":"));

   char * s2 = malloc(sizeof(char)*2);
   strcpy(s2,strh2);

   int h2= atoi(strtok(s2,":"));
   int m2= atoi(strtok(NULL,":"));

   if(h1<h2){
     return 1;
   }else if(h1==h2){
     if(m1<=m2){
       return 1;
     }else{
       return 0;
     }
   }else{
     return 0;
   }

 }

/*
  Prend une chaine de caracteres et renvoie
  une structure trajet avec tous les champs remplies
*/
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
          if(strcmp(res.promo,"SUPPL")==0){
            res.prix = res.prix + res.prix * 0.1;
          }else if (strcmp(res.promo,"REDUC")==0) {
            res.prix = res.prix - res.prix * 0.2;
          }
        }

        return res;
}

/*
  prend une structure trajet et renvoie une chaine de caracteres qui
  represente cette structure
*/
char * toString(struct trajet t){
      char * res =malloc(sizeof(char)*50);

      sprintf(res, "%d", t.num);
      strcat(res," ");

      strcat(res,t.depart);
      strcat(res," ");

      strcat(res,t.arrivee);
      strcat(res," ");

      strcat(res,t.heurdepart);/*
 *Authors:
 ********DIALLO Mariama**************
 ********KABAD Soufiane**************
 ********BOUKRIS WALID***************
 ********SOW OUSMANE*****************
 **Projet Système/Réseaux: L3 MIAGE**
 *************2019-2020**************
 */
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


/*
  prend deux structures trajet
  et renvoie 1 si les t2 correspond à t1
  sinon renvoie 0
*/
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
    if(strcmp(t1.heurdepart,"X")!=0 && strcmp(t1.heurarrivee,"X")!=0){
      int res = comparerHeure(t1.heurdepart,t2.heurdepart);
      int res2 = comparerHeure(t2.heurdepart,t1.heurarrivee);
      if( res2==0 || res == 0 ) return 0;
    }

    if(strcmp(t1.heurdepart,"X")!=0 && strcmp(t1.heurarrivee,"X")==0){
      int res = comparerHeure(t1.heurdepart,t2.heurdepart);
      if(res == 0 ) return 0;
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

    //Verification de nombres d'arguments
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    port = atoi(argv[1]);

    struct sockaddr_in Sclient,Sserver;

    //Creation de socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }else{
        printf("Socket successfully created..\n");
    }

    //Remplissage de struct sockaddr_in Sserver
    Sserver.sin_family = AF_INET;
    Sserver.sin_addr.s_addr = htonl(INADDR_ANY);
    Sserver.sin_port = htons(port);

    //Recuperation de la longeur de struct sockaddr_in Sclient
    socklen_t Sclientlen = (socklen_t)sizeof(Sclient);

    //Creation de bind
    int b = bind(sock, (const struct sockaddr * ) &Sserver, Sclientlen);

    if (b < 0) {
        printf("bind creation failed...\n");
        exit(0);
    }else{
        printf("bind successfully created..\n");
    }

    //Listen
    int list =  listen(sock, 1);
    if (list <0) {
        printf("listen creation failed...\n");
        exit(0);
    }else{
        printf("listen successfully created..\n");
    }

    while (1) {
        //Accept
        int acc = accept(sock, (struct sockaddr *) &Sclient, &Sclientlen);

        if (acc == -1) {
            printf("accept creation failed...\n");
            exit(0);
        }else{
            printf("accept successfully created..\n");
        }

        if(fork()==0){
          //Fils
          while (1) {
            char *buf;
            int i = 0;
            buf = malloc(MAXLINE * sizeof(char));

            //Recuperation de trajet demande
            i=recv(acc, buf,MAXLINE, 0);

            //Creation de structure demande
            struct trajet t ;
            t = readtrajet(buf);

            //Reception de critere de tri
            i=recv(acc, buf,MAXLINE, 0);
            buf[1]=0;

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

            //Trie par rapport au prix
            if(strcmp(buf,"p")==0){
              i=1;
              while(i<index){
                  struct trajet tTemp = tabtemp[i];

                  int j = i;
                  while(j>0 && tabtemp[j-1].prix > tTemp.prix){
                    tabtemp[j]=tabtemp[j-1];
                    j--;
                  }
                  tabtemp[j] = tTemp;
                  i++;
              }
            }else if(strcmp(buf,"d")==0){
              //tri par rapport a la duree
              i=1;
              while(i<index){
                  struct trajet tTemp = tabtemp[i];

                  int d1 = durre(tTemp.heurdepart,tTemp.heurarrivee);

                  int j = i;
                  int d2;
                  if(j>0) d2= durre(tabtemp[j-1].heurdepart,tabtemp[j-1].heurarrivee);
                  while(j>0 && d2 > d1){
                    tabtemp[j]=tabtemp[j-1];
                    j--;
                    if(j>0) d2= durre(tabtemp[j-1].heurdepart,tabtemp[j-1].heurarrivee);
                  }
                  tabtemp[j] = tTemp;
                  i++;
              }
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
            //renvoie de resultats !
            if(index==0){
                char *r="Pas de resultats";
                send(acc, r, strlen(r),0);
            }else{
               res[strlen(res)-1]=0;
               send(acc, res, strlen(res),0);
             }

            Free(t);

            free(res);

            free(buf);
          }
        }else{
              signal(SIGCHLD, traitement);
        }
    }
}
