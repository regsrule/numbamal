#ifndef TYPE_CL_H
#define TYPE_CL_H

typedef struct cellule
{
	int info;
	struct cellule* suivant;
	
}cellule;

typedef cellule* liste_t;

typedef cellule* cluster_t;

typedef struct hierarchie
{
	int nb_cluster;
	cluster_t* tab_cluster;

}hierarchie_t;

typedef struct super_hierarchie
{
	int nb_hierarchie;
	hierarchie_t* tab_hierarchie;

}super_hierarchie_t;




void insert_tete(int newElet, liste_t* tete);

void supprime_element(int elt,liste_t* tete);

void supprime_tete(liste_t* tete);

void afficher_liste(liste_t liste);

#endif





