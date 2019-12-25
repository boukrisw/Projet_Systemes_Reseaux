/*
 *Authors:
 ********DIALLO Mariama**************
 ********KABAD Soufiane**************
 ********BOUKRIS WALID***************
 ********SOW OUSMANE*****************
 **Projet Système/Réseaux: L3 MIAGE**
 *************2019-2020**************
 */

#ifndef __TRAJET_H
#define __TRAJET_H

/***************************************************************************
********Cree une Structure cmdline a partir de line*************************
****************************************************************************/
struct trajet trajet(char *line);

/* Structure returned by trajet() */
struct trajet {

	int num;
	char *depart;
	char *arrivee;
	char *heurdepart;
	char *heurarrivee;
	double prix;
	char *promo;
};

#endif
