#ifndef CL_HIER_H
#define CL_HIER_H
#include "type_cl.h"




int** initialiser_matrice(int n_init, char* filename);

int calcul_dist_min(int n_init, int** dist_init);

void elt_minimaux(int n_init, int** dist_init, int dmin, liste_t* l_i_min, liste_t* l_j_min);

int* init_tableau_cluster(int n_init);

int mise_en_cluster(int n_init, int* tab_cluster_init, liste_t l_i_min, liste_t l_j_min);

cluster_t* calcul_element_cluster(int* pre_tab_cluster, int nb_cluster_precedent, int nb_cluster);

int calcul_distance_cluster(int** dist_init, cluster_t cl_i, cluster_t cl_j);

int** mise_a_jour_distance( int** dist_init, cluster_t* tab_cluster, int nb_cluster);

super_hierarchie_t clustering_hierarchique(int n_init, char* filename);

//void afficher_hierarchie(cluster_t** tab_hierarchie, int numero_hierarchie, int numero_cluster);

void affiche_cluster_h0(super_hierarchie_t s_hierarchie, hierarchie_t* tab_hierarchie, int id_cluster);

cluster_t element_cluster_de_base(super_hierarchie_t s_hierarchie, hierarchie_t* tab_hierarchie, int id_h, int id_cl);

int* renumerotation (super_hierarchie_t s_hierarchie, hierarchie_t* hierarchie, int id_h, int id_cl, int n_init);

void mettre_dans_fichier_de_numerotation(char* filename, int* re_numero);








#endif

