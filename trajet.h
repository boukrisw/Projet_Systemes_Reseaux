/*
 *Authors:
 ********BOUKRIS WALID***************
 ********SOW OUSMANE*****************
 ********Mini-shell: L3 MIAGE********
 *************2019-2020**************
 */

#ifndef __TRAJET_H
#define __TRAJET_H

/***************************************************************************
********Cree une Structure cmdline a partir de com**************************
****************************************************************************/
struct trajet readtrajet(char *line);

//typedef struct Listetrajets Listetrajets;
//typedef struct trajet trajet;


/* Structure returned by readcmd() */
struct trajet {

	int num;
	char *depart;
	char *arrivee;
	char *heurdepart;
	char *heurarrivee;
	double prix;
	char *promo;
};
/*
struct Listetrajets {
	trajet  tj;
	Listetrajets  * ptrtj;
};*/



#endif
