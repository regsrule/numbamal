#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <limits.h>
#include "calcule_distance.h"
#include <string.h>
#define TAILLE_MAX 1000000 

typedef struct timezone timezone_t;
typedef struct timeval timeval_t;

timeval_t t1, t2;
timezone_t tz;


static struct timeval _t1, _t2;
static struct timezone _tz;
timeval_t t1, t2;
timezone_t tz;

static unsigned long _temps_residuel = 0;
#define top1() gettimeofday(&_t1, &_tz)
#define top2() gettimeofday(&_t2, &_tz)

void init_cpu_time(void)
{
   top1(); top2();
   _temps_residuel = 1000000L * _t2.tv_sec + _t2.tv_usec -
                     (1000000L * _t1.tv_sec + _t1.tv_usec );
}

unsigned long cpu_time(void) /* retourne des microsecondes */
{
   return 1000000L * _t2.tv_sec + _t2.tv_usec -
           (1000000L * _t1.tv_sec + _t1.tv_usec ) - _temps_residuel;
}




#define NUM_THREADS 5

typedef struct
{
	float *x;
	float *y;
	float sum;
	int length;
} data_t;


data_t data;
pthread_mutex_t mutex_sum;
trace* tab_trace;
int num_ligne;
int id_point_partage;
pthread_mutex_t mutex_id_point_partage;
pthread_mutex_t mutex_fichier_partage;


void *produit_scalaire_parallel_1(void *arg)
{
	/* Define and use local variables for convenience */
	int i, taille; 
	long debut, fin;
	long indice;
	float som_locale, *x, *y;
	indice = (long)arg;

	taille = data.length;

	debut = indice*(taille/NUM_THREADS);

	if(indice == (NUM_THREADS - 1))
	   fin = data.length;
	else
	   fin = debut + (taille/NUM_THREADS);

	x = data.x;
	y = data.y;

	/* Perform the dot product and assign result to the appropriate variable in the structure. */
	som_locale = 0;
	for (i=debut; i<fin ; i++)
	{
		som_locale += (x[i] * y[i]);
	}
	//printf("Thread %ld debut = %ld fin = %ld\n", indice, debut, fin);

	/* Lock a mutex prior to updating the value in the shared structure, and unlock it upon updating. */
	pthread_mutex_lock (&mutex_sum);
	data.sum += som_locale;
	pthread_mutex_unlock (&mutex_sum);


	pthread_exit((void*) 0);
}

float* creer_vecteur(int n)
{
	int i;
	float* tab = (float*)malloc(sizeof(float)*n);
	
	for(i=0;i<n;i++)
	{
		tab[i] = 1.0+ 0.5 * (rand()%100);
	}

	return tab;	
}

float produit_scalaire(float* x, float* y, int n)
{
	int i;
	float somme = 0.0;
	for(i = 0; i < n; i++)
		somme = somme + x[i]*y[i];

	return somme;
}

void *BusyWork(void *t)
{
	int i;
	long tid;
	double result=0.0;
	tid = (long)t;
	printf("Thread %ld starting...\n",tid);
	for (i=0; i<1000000; i++)
	{
		//result = result + sin(i) * tan(i);
		result = result + i*i + (4*i);
	}
	printf("Thread %ld done. Result = %e\n",tid, result);
	pthread_exit((void*) t);
}

void *PrintHello(void *threadid)
{
	long tid;
	tid = (long)threadid;
	printf("Hello World! It's me, thread #%ld!\n", tid);
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	pthread_t thread[NUM_THREADS];
	pthread_attr_t attr;
	int rc, n;
	float somme, somme_parallele;
	long t;
	void *status;

	srand(time(NULL));

	float* x, *y;
	
	//printf("Entrez la taille des vecteurs\n");
	//scanf("%d",&n);

	
	x= creer_vecteur(n);
	y= creer_vecteur(n);

	data.x = x;
	data.y = y;
	data.sum = 0.0;
	data.length= n;

	id_point_partage = 0;

	pthread_mutex_init(&mutex_sum, NULL);
	pthread_mutex_init(&mutex_id_point_partage, NULL);
	pthread_mutex_init(&mutex_fichier_partage, NULL);



	if(argc < 2)
	{
		printf("Nombre d'argument insuffisant");
		exit(-1);
	}
	
	char* filename = argv[1];

	num_ligne = count_lines(filename);

	tab_trace = lecture_du_fichier_de_trace(filename);
	
	
	
top1();
	//calcul_distance_sequentiel(tab_trace, num_ligne);
	printf("num_ligne = %d  \n", num_ligne);
top2();

	unsigned long temps = cpu_time();
	printf("\ntime seq = %ld.%03ldms\n", temps/1000, temps%1000);
	
	/* Initialize and set thread detached attribute */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

top1();
	for(t=0; t<NUM_THREADS; t++) {
		//printf("Main: creating thread %ld\n", t);
		rc = pthread_create(&thread[t], &attr, calcul_distance_parallel, (void *)t);
		if (rc) {
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	/* Free attribute and wait for the other threads */
	//pthread_attr_destroy(&attr);
	for(t=0; t<NUM_THREADS; t++) {
		rc = pthread_join(thread[t], &status);
		if (rc) {
			printf("ERROR; return code from pthread_join() is %d\n", rc);
			exit(-1);
		}
		//printf("Main: completed join with thread %ld having a statusof %ld\n",t,(long)status);
	}

top2();
unsigned long temps_par = cpu_time();
	printf("\ntime par = %ld.%03ldms\n", temps_par/1000, temps_par%1000);
	


	/*somme_parallele = data.sum;
	printf("Produit scalaire = %f\n", somme);
	printf("Produit scalaire parallel = %f\n", somme_parallele);*/
}

/*int main(int n, char*argv[])
{
	
	
	return 0;

}*/


//fonction qui separe notre chaine en fonction du delimiteur qu'on a choisit, il va parcourir la chaine et 
//à chaque fois qu'il va rencontrer 
//notre delimiteur il va sauter ce dernier et continuer la lecture de la chaine
int str_split(char* str, char* car, char*reslt[])
{
	char *str1, *token;
        char *saveptr1;
        int j, j_retour;

	for (j = 1, str1 = str; ; j++, str1 = NULL) {
               token = strtok_r(str1, car, &saveptr1);
               if (token == NULL)
                   break;
		reslt[j-1] = token;
		j_retour = j;
        }

	return j_retour;

}

//fonction pour afficher les elements de notre tableau de caracteres ayant deja la taille des éléments la
void print_tab_string(char** tab_line, int nb_line)
{
	int i=0;
	
	while(i<nb_line)
	{
		printf("%d: %s\n", i, tab_line[i]);
		i++;
	}

}

int count_lines(char * file_name){
  FILE * fichier = fopen(file_name, "r");
  char ligne [TAILLE_MAX];
  int nLignes = 0;

  while(fgets(ligne,TAILLE_MAX,fichier)!=NULL)
  {
    nLignes++;
  }
  
  fclose(fichier);
  return nLignes;
}



//ca c'est notre fonction qui est censé lire le fichier de trace et mettre cette trace dans notre structure "trace"
//qui sera constitué de l'id de l'element courant,son nombre de voisin et la liste de ses voisins 

trace* lecture_du_fichier_de_trace(char* filename)
{
	int ligne_courante_du_fichier = 0; // pour nous indiquer à quel ligne on se trouve dans le fichier
	int num_ligne = 0; //variable pour compter le nombre de ligne
	int split_resl_length = 0;//doit contenir le nombre d'élément séparé par le delimiteur

	FILE *fp = fopen(filename,"r");

	num_ligne = count_lines(filename);
	printf(" nombre de ligne du fichier = %d \n",num_ligne );

	trace* tab_trace = (trace*) malloc(sizeof(trace)*num_ligne);// on declare notre tableau de trace 
																//où chaque case constitue la trace d'un point
	
	char* chaine = (char*) malloc (sizeof(char)* TAILLE_MAX);//chaine qui recoit à chaque itération
															// la chaine qui est lue

	char** resl_split = (char**) malloc(sizeof(char*)* TAILLE_MAX); // chaine qui contient le resultat des decoupages

	if (fp == NULL){printf("Error opening data file\n"); return 0;}


	while(fgets(chaine,TAILLE_MAX,fp) != NULL)
	{	
		if (ligne_courante_du_fichier%1000 == 0)
        {
			printf("ligne_courante_du_fichier = %d \n", ligne_courante_du_fichier );
        }
		
		split_resl_length = 0;

		split_resl_length = str_split(chaine,":",resl_split);
	  	
	  	//printf("split du point  : %s\n",resl_split[0] );
		//printf("la chaine qu'il faut couper est :%s\n",resl_split[1] );

		tab_trace[ligne_courante_du_fichier].id_point = atoi(resl_split[0]);
		split_resl_length = str_split(resl_split[1],",",resl_split);
		
		//printf("nous sommes sur le point d'id %d \n",tab_trace[ligne_courante_du_fichier].id_point );
		
		tab_trace[ligne_courante_du_fichier].taille_trace = split_resl_length;
		
		//printf("ce point possède %d voisins \n",tab_trace[ligne_courante_du_fichier].taille_trace );
		//printf("mainteant on remplis la liste du point en question \n");

		tab_trace[ligne_courante_du_fichier].elt_trace = (int*) malloc(sizeof(int)*tab_trace[ligne_courante_du_fichier].taille_trace);
		
		//printf("on alloue l'espace à la liste des points voisins\n");

		for (int i = 0; i < tab_trace[ligne_courante_du_fichier].taille_trace ; ++i)
		{
		  tab_trace[ligne_courante_du_fichier].elt_trace[i] = atoi(resl_split[i]);	
	  	}
	   	
	   	//printf("allocation terminé pour les voisins\n on affiche donc la liste en question \n");
	  	//print_tab_string(resl_split, split_resl_length);

		ligne_courante_du_fichier++;
	}

	fclose(fp);

		//printf("nous sommes sur le point %d \n", tab_trace[0].id_point );
		//printf("nous sommes sur le point %d \n", tab_trace[1].id_point );

	return tab_trace;

}



int max (int a , int b)
{
	int maximum = 0;
	if (a < b)
	{
		maximum = b;
	}
	else
	{
		maximum = a;
	}
	return maximum;
}

int min (int a, int b)
{
	int minimum = 0;
	if (a < b)
	{
		minimum = a;
	}
	else
	{
		minimum = b;
	}
	return minimum;	
}

int intersection(trace* tab_trace, int id_point1, int id_point2) //fonction qui calcule l'intersection entre 2 points
{
	int inter = 0;
	int nb_elet_trace1, nb_elet_trace2;

	nb_elet_trace1 = tab_trace[id_point1].taille_trace;
	nb_elet_trace2 = tab_trace[id_point2].taille_trace;

	for (int compteur_trace1 = 0; compteur_trace1 < nb_elet_trace1; compteur_trace1++)
	{
		for (int compteur_trace2 = 0; compteur_trace2 < nb_elet_trace2; compteur_trace2++)
		{
			if (tab_trace[id_point1].elt_trace[compteur_trace1] == tab_trace[id_point2].elt_trace[compteur_trace2])
			{
				inter = inter + 1;
			}
		}
	}
	//printf("l'intersection entre %d et %d vaut %d \n", id_point1, id_point2, inter );

	return inter;


}

void* calcul_distance_parallel(void* arg)
{
	int dist = 0;
	int inter = 0;
	int i, k;
	int nb_elet_trace1, nb_elet_trace2;

	int id_point_local;
	int* tab_distance;
   
	do
	{
		pthread_mutex_lock (&mutex_id_point_partage);
			id_point_local = id_point_partage;
			id_point_partage = id_point_partage + 1;
		pthread_mutex_unlock (&mutex_id_point_partage);

		i = id_point_local;
		if(i < num_ligne)
		{	
			nb_elet_trace1 = tab_trace[i].taille_trace;
			tab_distance = (int*)malloc(sizeof(int)*(num_ligne-i-1));
			k = 0;
			for (int j = i+1; j < num_ligne; j++)
			{
				dist = 0;
				nb_elet_trace2 = tab_trace[j].taille_trace;
				inter = intersection(tab_trace, i, j);
				dist = nb_elet_trace1 + nb_elet_trace2 - inter;
				    //printf("distance entre %d et %d = %d  \n",i, j, dist );
				tab_distance[k] = dist;
			        k = k + 1;	
				//printf("distance entre %d et %d = %d \n",tab_trace[i].id_point, tab_trace[j].id_point, dist );            
			 }

			if (i%1000 == 0)
			{
				printf("le noeuds numero %d \n",i );
			}

		   }
			
			pthread_mutex_lock (&mutex_fichier_partage);
				FILE *fid = fopen("distance_parallel.txt","a");
		
			k = 0;	
			for (int j = i+1; j < num_ligne; j++)
			{
				fprintf(fid,"%d", tab_trace[i].id_point);
				fprintf(fid," %d ", tab_trace[j].id_point);
				fprintf(fid, "%d\n", tab_distance[k]);
				k = k + 1;
			}
				fclose(fid); 
			pthread_mutex_unlock (&mutex_fichier_partage);
			if(i<num_ligne)
				free(tab_distance);
	}	
	while(i < num_ligne);
	pthread_exit((void*) 0);
}

void calcul_distance_sequentiel(trace* tab_trace, int num_ligne)
{

	int dist = 0;
	int inter = 0;
	int nb_elet_trace1, nb_elet_trace2;

	FILE *fid = fopen("distance_sequentiel.txt","w");


	for (int i = 0; i < num_ligne; i++)
    {
        nb_elet_trace1 = tab_trace[i].taille_trace;

        for (int j = i+1; j < num_ligne; j++)
        {
        
        	dist = 0;

            nb_elet_trace2 = tab_trace[j].taille_trace;

        	inter = intersection(tab_trace, i, j);
            
            dist = nb_elet_trace1 + nb_elet_trace2 - inter;
            
            //printf("distance entre %d et %d = %d  \n",i, j, dist );
            
            fprintf(fid,"%d", tab_trace[i].id_point);
            fprintf(fid," %d ", tab_trace[j].id_point);
            fprintf(fid, "%d\n", dist );
            
            //printf("distance entre %d et %d = %d \n",tab_trace[i].id_point, tab_trace[j].id_point, dist );            
        }
        
        if (i%1000 == 0)
        {
        	printf("le noeuds numero %d \n",i );
        }
    }

	fclose(fid); 
}
	

