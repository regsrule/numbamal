#include "cl_hier.h"
#include<stdio.h>
#include<stdlib.h>


void insert_tete(int newElet, liste_t* tete)
{
	liste_t p;

	p = (liste_t) malloc (sizeof(liste_t));
	p->info = newElet;
	p->suivant = *tete;
	*tete = p;
	//afficher_liste(*tete);
}

void supprime_tete(liste_t* tete)
{
	
	liste_t p;
	if (*tete != NULL)
	{
		p = *tete;
		*tete = p->suivant;
		free(p);
	}
	else
	{
		printf("la liste est vide \n");
	}
}

void supprime_element(int elt,liste_t* tete)
{
	
	liste_t p = *tete, prec;

	if (p != NULL && p->info == elt)
	{
		supprime_tete(tete);
	}
	else
	{
		prec = p;
		p = p->suivant;
		while(p != NULL && p->info != elt)
		{
			prec = p;
			p = p->suivant;	
		}
		if (p != NULL)
		{
			prec->suivant = p->suivant ;
			free(p);
		}
	}
}



void afficher_liste(liste_t liste)
{
	liste_t p = liste;

	printf("liste : \t");
	while(p!= NULL)
	{
		printf("%d \t", p->info );
		p = p->suivant;
	}
	printf("\n");
}




