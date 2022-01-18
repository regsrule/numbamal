#include<stdlib.h>
#include<stdio.h>

#include "cl_hier.h"


int main (int argc, char*argv[])
{
	int i,j;
	int n_init;
	int **dist_init;//tab_distance_initial
	int **mat_dist, **mat_dist_temp;
	int **matrice_distance;
	liste_t l_i_min = NULL;
	liste_t l_j_min = NULL;
	int dmin;//distance minimale entre elements
	int d_min;//distance minimale entre clusters
	int *tab_cluster_init;
	int id_cluster;
	int nb_cluster;

	int* pre_tab_cluster;
	int nb_cluster_precedent;
	int iteration = 0;
	cluster_t* tab_cluster;
	cluster_t cl_i; 
	cluster_t cl_j;
	hierarchie_t* tab_hierarchie;
	cluster_t liste_elements;

	super_hierarchie_t s_hierarchie;

	int numero_hierarchie;
	int numero_cluster;

	
	int hierar = 0;
	int a=0;

	n_init = 0;

	int* re_numero;
	
		printf("entrer la taille de la matrice carrée: \n");
		scanf("%d",&n_init);
		//dist_init = initialiser_matrice(n_init, argv[1]);

		s_hierarchie = clustering_hierarchique(n_init, argv[1]);

		printf("je recupere le nombre d'hierarchie\n");
		int nb_hierar;
		nb_hierar = s_hierarchie.nb_hierarchie;

		printf("le nombre de hierarchie vaut %d \n",nb_hierar );
		//affiche_cluster_h0(tab_hierarchie, 0);
		
		int id_h = nb_hierar;
		int id_cl ;
		id_cl = 0;
		liste_elements = element_cluster_de_base(s_hierarchie, tab_hierarchie, id_h, id_cl);
		printf("la liste finale est: \n");
		afficher_liste(liste_elements);
		re_numero = renumerotation (s_hierarchie, tab_hierarchie, id_h, id_cl, n_init);
		mettre_dans_fichier_de_numerotation(argv[2], re_numero);

	/*while(id_h != -1)
	{
		printf("entrer la hierarchie que vous voulez:\n");
		scanf("%d",&id_h);

			
		a = tab_hierarchie[id_h].nb_cluster ;
		printf("a cette hierarchie on a %d cluster\n",a );


		printf("entrer le cluster dont vous voulez connaitre les éléments de base: \n");
		scanf("%d",&id_cl);

		liste_elements = element_cluster_de_base(tab_hierarchie, id_h, id_cl);
		printf("la liste finale est: \n");
		afficher_liste(liste_elements);
	}*/
	

	

	return 0;
}
