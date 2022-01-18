#include "cl_hier.h"
#include<stdio.h>
#include<stdlib.h>


int ** initialiser_matrice(int n_init, char* filename)
{
	int i;
	int j;
	int **dist_init = (int **) malloc (sizeof(int *)* n_init);
	FILE *fid;
	int ni, nf, d;


	// on initialise la matrice en mettant des 0.
	for ( i = 0; i < n_init; ++i)
	{
		dist_init[i]=(int *) malloc (sizeof(int)* n_init);
		for ( j = 0; j < n_init; ++j)
		{
			dist_init[i][j] = -1;		
			printf("la valeur de dist[%d][%d] = %d \n", i, j, dist_init[i][j] );
		}
	} 
	//on ouvre notre fichier en lecture
	fid = fopen(filename, "r");
   	if (fid == NULL)printf("Error opening data file\n");
   	ni = 0;	nf = 0; d = 0;// respectivement n_initial, n_finale et la distance entre les 2 n.

	while (!feof(fid))
   		{
   			printf("on va faire les scanf\n");
   			fscanf(fid," %d %d %d ",&ni,&nf,&d);printf("le scanf est bloqué\n");
   			printf("on recupere le suivant\n");
  			dist_init[ni][nf] = d;
   			printf("la distance a la position (%d,%d) est %d\n",ni, nf, dist_init[ni][nf] );
   			printf("on passe au suivant\n");
   		}
	return dist_init;	
}

int calcul_dist_min(int n_init, int** dist_init)
{
	int dmin= dist_init[0][1];

	for (int i = 0; i < n_init; ++i)
	{
		for (int j = i+1; j < n_init; ++j)
		{
			if(dist_init[i][j] != -1 && dist_init[i][j] < dmin)
 			{
 				dmin = dist_init[i][j];
 			}
		}
	}
	printf("la distance minimale est: %d\n", dmin);
	return dmin;
}

void elt_minimaux(int n_init, int** dist_init, int dmin, liste_t* l_i_min, liste_t* l_j_min)
{
	
	//les 2 listes passées en parametre sont les listes pour contenir d'une part les i à regrouper, et d'autre part les j à regrouper
	//donc pour une position précise dans les 2 listes, on aura un couple d'element à regrouper.ou encore le couple d'element ayant 
	//pour distance, la distance minimale dmin.
	for (int i = 0; i < n_init; ++i)
		{
			for (int j = i+1; j < n_init; ++j)
			{
				if(dist_init[i][j] == dmin)
	 			{
	 				insert_tete(i, l_i_min);
	 				insert_tete(j, l_j_min);
	 			}
			}
		}
		afficher_liste(*l_i_min);
		afficher_liste(*l_j_min);

}



int* init_tableau_cluster(int n_init)
{
	int* tab_cluster_init = (int *) malloc(sizeof(int)*n_init);
	int i;

	//printf("les elements de tab sont  \n" );
	for (i = 0; i < n_init; ++i)
	{
		tab_cluster_init[i] = i;
		!printf("%d  \t", tab_cluster_init[i]);
	}

	return tab_cluster_init;// je retourne le tableau pour voir l'utiliser dans les autres fonctions cible
}



int mise_en_cluster(int n_init, int* tab_cluster_init, liste_t l_i_min, liste_t l_j_min)
{
	int* tab_bool = (int *) malloc(sizeof(int)* n_init);
	int a;
	int b;
	int id_cluster = 0;
	liste_t p = l_i_min;
	liste_t m = l_j_min;

	//printf("on cree notre tableau de booleen\n");

	for (int i = 0; i < n_init; ++i)
	{
		tab_bool[i] = 0;
		//printf("tab_bool de %d vaut %d\n",i, tab_bool[i] );
	}
	int d1=0,d2=0;// d1 signale qu'on a modifié l'élément "a" et d2 signale qu'on a modifié l'élément "b"

	while(p != NULL)
	{
		//printf("on met la valeur de p.info dans a\n");
		a = p->info;
		b = m->info; 
		
		d1 = 0, d2 = 0;
		if (tab_bool[a] == 0)
		{
			tab_cluster_init[a] = id_cluster;
			tab_bool[a] = 1;
			d1 = 1;	
		}
		if (tab_bool[b] == 0)
		{
			tab_cluster_init[b] = id_cluster;
			tab_bool[b] = 1;
			d2 = 1;
		}
		if (d1 == 1 || d2 == 1)
		{
			id_cluster++;
		}
		p= p->suivant;
		m= m->suivant;
		
	}

	for (int i = 0; i < n_init; ++i)
	{
		if (tab_bool[i] == 0)
		{
			tab_cluster_init[i] = id_cluster;
			id_cluster++;
			tab_bool[i] = 1;
		}
	}

	for (int i = 0; i < n_init; ++i)
	{
		printf("l'identifiant ou l'élément %d est dans le cluster %d \n",i, tab_cluster_init[i] );
	}
free(tab_bool);
return id_cluster;

}

cluster_t* calcul_element_cluster(int* pre_tab_cluster, int nb_cluster_precedent, int nb_cluster)//nb_cluster_precedent = au n initial et nb_cluster correspond au id(cluster): le nombre de cluster obtenu apres avoir determiné la distance minimale
{
	cluster_t* tab_cluster = (cluster_t*) malloc(sizeof(cluster_t*)*nb_cluster);
	int i,k;

	for ( i = 0; i < nb_cluster ; ++i)
	{
		tab_cluster[i] = NULL;
	}

	for ( i = 0; i < nb_cluster_precedent; ++i)
	{
		k = pre_tab_cluster[i];//k recoi le numero du cluster de l'element i 
		insert_tete(i, tab_cluster+k); //pour remplacer l'ecriture tab_cluster[k] Donc on met l'element i dans le cluster k
	}

	printf("ici nb_cluster = %d et nb_cluster_precedent = %d\n",nb_cluster,nb_cluster_precedent );
	//la on compte jusqu'a nb_cluster car on veut constituer nos cluster donc on a besoin du nombre exacte de cluster 
	for (int i = 0; i < nb_cluster; ++i)
	{
		printf("on affiche les éléments du cluster %d\n",i );
		afficher_liste(tab_cluster[i]);
	}
		return tab_cluster;
}

int calcul_distance_cluster(int** dist_init, cluster_t cl_i, cluster_t cl_j)
{

	cluster_t p_cl_i, q_cl_j;

	int d_min = dist_init[cl_j->info][cl_i->info];
	if (d_min == -1)
	{
		d_min = dist_init[cl_i->info][cl_j->info];	
	}
	
	printf("au debut on d_min qui vaut:%d\n",d_min );
	p_cl_i = cl_i;

	while(p_cl_i != NULL)
	{
		q_cl_j = cl_j;

		while(q_cl_j != NULL)
		{

			if (dist_init[p_cl_i->info][q_cl_j->info] == -1)
			{
				dist_init[p_cl_i->info][q_cl_j->info] = dist_init[q_cl_j->info][p_cl_i->info];	
			}
			else
			{
				if ( d_min > dist_init[p_cl_i->info][q_cl_j->info])
				{
					d_min = dist_init[p_cl_i->info][q_cl_j->info];
				}
				q_cl_j = q_cl_j->suivant;
			}
				
		}
		p_cl_i = p_cl_i->suivant;	
	}


	return d_min;
}





int** mise_a_jour_distance( int** dist_init, cluster_t* tab_cluster, int nb_cluster)
{
	int i,j,k, d_min;
	int ** mat_dist = (int**) malloc(sizeof(int*)*nb_cluster);
	
	//printf("on alloue de l'espace à la matrice_distance\n");
	for ( i = 0; i < nb_cluster; ++i)
	{
		mat_dist[i] = (int*) malloc(sizeof(int)*nb_cluster);
	}
	//printf("allocation terminée\n");

	for ( i = 0; i < nb_cluster; ++i)
	{
		for (int j = 0; j < nb_cluster; ++j)
		{
			mat_dist[i][j] = -1;		

		}
	}
	//printf("remplissage avec les -1 terminé\n");

	for ( i = 0; i < nb_cluster; ++i)
	{
		for (int j = i+1; j < nb_cluster; ++j)
		{
			//printf("calcul du d_min\n");
			d_min = calcul_distance_cluster(dist_init, tab_cluster[i], tab_cluster[j]);
			//printf("fin de calcul de d_min\n");
			mat_dist[i][j] = d_min;
			printf("matrice_distance[%d][%d] = %d \n",i,j, mat_dist[i][j] );
		}
	}
	
			//retrouver le cluster i(sous fome de liste: l_k_i  prende tous les elements k tel que tab_cluster de k = i)
	//liste_t l_k_i= NULL ;
			
			//retrouver le cluster j(sous fome de liste: l_k_j prende tous les elements k tel que tab_cluster de k = j)
            //mettre dans matrice_distance[i][j] la plus petite distancce entre les éléments de lki et lkj en servant de la matrice initial des distances

	return mat_dist;

}


void free_matrice_int(int** mat, int n)
{
	for (int i = 0; i < n; ++i)
	{
		free(mat[i]);
	}
	free(mat);
}


super_hierarchie_t clustering_hierarchique(int n_init, char* filename)
{
	int** mat_dist, **mat_dist_temp;
	int nb_cluster;
	int *tab_cluster_init;
	int dmin, d_min, hierar = 0;
	int id_cluster;
	liste_t l_i_min, l_j_min;

	super_hierarchie_t s_hierarchie;

	hierarchie_t* tab_hierarchie ;
	
	tab_hierarchie = (hierarchie_t*) malloc(sizeof(hierarchie_t));	
	mat_dist = initialiser_matrice(n_init, filename );

	nb_cluster = n_init;

	while(nb_cluster > 1)
	{
		tab_cluster_init =	init_tableau_cluster(nb_cluster);
		
		dmin = calcul_dist_min(nb_cluster, mat_dist);
		l_i_min = NULL;
		l_j_min = NULL;
		
		elt_minimaux(nb_cluster, mat_dist, dmin, &l_i_min, &l_j_min);
		
		id_cluster = mise_en_cluster(nb_cluster, tab_cluster_init, l_i_min, l_j_min);

		tab_hierarchie = (hierarchie_t*) realloc(tab_hierarchie, (hierar+1) * sizeof(hierarchie_t) );
		printf("nous sommes à la hierarchie %d\n", hierar);
		printf("affichons la liste des clusters et leurs éléments\n");

		tab_hierarchie[hierar].tab_cluster = calcul_element_cluster(tab_cluster_init, nb_cluster, id_cluster);
		tab_hierarchie[hierar].nb_cluster = id_cluster;

		hierar = hierar+1;

		mat_dist_temp = mise_a_jour_distance(mat_dist, tab_hierarchie[hierar-1].tab_cluster, id_cluster);
		
		//on libere mat_dist
		free_matrice_int(mat_dist, nb_cluster);
		mat_dist = mat_dist_temp;
		nb_cluster = id_cluster;
	}
	printf("on a fait au total: %d hierarchie \n", hierar-1);
	printf("donc les hierarchies vont de 0 à %d \n", hierar-1 );

	s_hierarchie.nb_hierarchie = hierar-1;
	s_hierarchie.tab_hierarchie = tab_hierarchie;

	return s_hierarchie;
}

void affiche_cluster_h0(super_hierarchie_t s_hierarchie, hierarchie_t* tab_hierarchie, int id_cluster)
{
	tab_hierarchie = s_hierarchie.tab_hierarchie;

	if (id_cluster < tab_hierarchie[0].nb_cluster)
		{
			afficher_liste(tab_hierarchie[0].tab_cluster[id_cluster]);
		}	

}


cluster_t element_cluster_de_base(super_hierarchie_t s_hierarchie, hierarchie_t* tab_hierarchie, int id_h, int id_cl)
{
	
	cluster_t liste = NULL; // la liste qui va contenir la copie des elements d'un cluster choisi
	cluster_t liste_temp = NULL;// la liste pour stocker temporairement les elusters qui  viennent d'etre recueillis

	cluster_t liste_clusters = NULL;// la liste qui doit contenir les éléments de base à la fin, et qui contient generalement l'union des elements des clusters contenu dans la liste "liste"
	int k;

	tab_hierarchie = s_hierarchie.tab_hierarchie ;

	liste = tab_hierarchie[id_h].tab_cluster[id_cl];
	if (id_h == 0)
	{
		liste_clusters = liste;
	}
	
	k = id_h-1;
	liste_temp = liste;
	liste = NULL;

	
	while(k>=0)
	{
		while(liste_temp != NULL)
		{
			liste = tab_hierarchie[k].tab_cluster[liste_temp->info];
			while(liste != NULL)
			{
				insert_tete(liste->info, &liste_clusters);
				liste = liste->suivant;
			}

			liste_temp = liste_temp->suivant;
		}

		k = k-1;
		liste_temp = liste_clusters;

		if (k >= 0)
		{
			liste_clusters = NULL;
		}
		
	}
	return liste_clusters;
}


int* renumerotation (super_hierarchie_t s_hierarchie, hierarchie_t* tab_hierarchie, int id_h, int id_cl, int n_init)
{
    int a;
    int* re_numero = (int *) malloc(sizeof(int)*n_init);
    int i = 0;
    cluster_t liste_elements;
    int taille_finale_tableau;

    tab_hierarchie = s_hierarchie.tab_hierarchie;


   // printf("entrer la hierarchie que vous voulez:\n");
   // scanf("%d",&id_h);

        
    //a = tab_hierarchie[id_h].nb_cluster ;
    //printf("a cette hierarchie on a %d cluster\n",a );


    //printf("entrer le cluster dont vous voulez connaitre les éléments de base: \n");
    //scanf("%d",&id_cl);

  //  printf("on va lancer la renumerotation, entrer un nombre quelconque:\n");
   // scanf("%d",&a);

    liste_elements = element_cluster_de_base(s_hierarchie, tab_hierarchie, id_h, id_cl);
    printf("la liste finale est: \n");
    afficher_liste(liste_elements);

    while(liste_elements != NULL)
    {
    	re_numero[liste_elements->info]= i;
    
		liste_elements = liste_elements->suivant;
		i++;	

    }
    taille_finale_tableau = i;// c'est pour savoir le nombre d'éléments qu'on a eu 

    

    for (int i = 0; i < taille_finale_tableau; ++i)
    {
    	printf("element %d possede le numéro %d\n",i, re_numero[i] );
    }

    return re_numero;
    
}

void mettre_dans_fichier_de_numerotation(char* filename, int* re_numero)
{

	int source, destination, source_cl, destination_cl;
	FILE *fid;
	FILE *fp;

	fid = fopen(filename, "r");

	  	fp = fopen("fichier_renumerotation.txt","w"); 

	
   	if (fid == NULL)printf("Error opening data file\n");

	
	while(!feof(fid))
	{
		fscanf(fid," %d %d ",&source,&destination);printf("on a pris les premiers éléments\n");
   	printf("on recupere le suivant\n");
   	
   	source_cl = re_numero[source];
   	destination_cl = re_numero[destination];

   	fprintf(fp,"%d ",source_cl);
   	fprintf(fp,"%d \n",destination_cl);
	
	}
   	
}



