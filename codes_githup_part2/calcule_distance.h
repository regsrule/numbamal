#ifndef CALCULE_DISTANCE_H
#define CALCULE_DISTANCE_H


typedef struct trace
{
	int id_point;
	int taille_trace;
	int* elt_trace;

}trace;

int str_split(char* str, char* car, char*reslt[]);


void print_tab_string(char** tab_line, int nb_line);

int count_lines(char * file_name);

trace* lecture_du_fichier_de_trace(char* filename);


int max (int a , int b);

int min (int a, int b);

int intersection(trace* tab_trace, int id_point1, int id_point2);

void calcul_distance_sequentiel(trace* tab_trace, int num_ligne);

void* calcul_distance_parallel(void* arg);

#endif
