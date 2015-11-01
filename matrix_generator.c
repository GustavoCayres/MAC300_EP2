/*********************************************/
/*Grupo:                                     */
/*  Gustavo Rodrigues Cayres, nUSP: 8584323  */
/*   Pedro Marcondes, nUSP: 8941168          */
/*********************************************/


#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#define E 0.0001


double random_double() {
	double r = (double)rand() / RAND_MAX;
    return (-1 + r * 2);
}

double **Alocar_matriz_real (int m, int n) {
	double **v; /* ponteiro para a matriz */
	int i; /* variavel auxiliar */
	if (m < 1 || n < 1) { /* verifica parametros recebidos */
		printf ("** Erro: Parametro invalido **\n");
		return (NULL);
	}
	/* aloca as linhas da matriz */
	v = (double **) calloc (m, sizeof(double *));
	if (v == NULL) {
		printf ("** Erro: Memoria Insuficiente **");
		return (NULL);
	}

	/* aloca as colunas da matriz */
	for ( i = 0; i < m; i++ ) {
		v[i] = (double*) calloc (n, sizeof(double));
		if (v[i] == NULL) {
			printf ("** Erro: Memoria Insuficiente **");
			return (NULL);
		}
	}
	return (v); /* retorna o ponteiro para a matriz */
}


void Liberar_matriz_real(int m,int n,double **v) {
	int i; /* variavel auxiliar */
	if (v == NULL)
		return ;
	if(m < 1 || n < 1) {/* verifica parametros recebidos */
		printf ("** Erro: Parametro invalido **\n");
		return ;
	}
	for(i = 0; i < m; i++) free (v[i]);/* libera as linhas da matriz */
	free (v);/* libera a matriz */
}

int main() {
	char file_name[50];
	FILE *file;
	double **A, T, r, *x, *b;
	int i, j, n;

	srand((unsigned) time(NULL));
	printf("Entre com T e n: \n");
	scanf("%lf %d", &T, &n);
	sprintf(file_name, "sparse_matrix_%d.txt", n);
	file = fopen(file_name, "w" );

	A = Alocar_matriz_real(n, n);
	b = malloc(n*sizeof(double));
	x = malloc(n*sizeof(double));

	for (i = 0; i < n; i ++) {
		A[i][i] = 1;
		for(j = i + 1; j < n; j ++) {
			r = random_double();
			if (fabs(r) > T)
				r = 0.0;
			A[i][j] = A[j][i] = r;
		}
	}

	for (i = 0; i < n; i++)
    	x[i]= 1+i%(n/100);

	for (i = 0; i < n; i++) {
    	b[i] = 0;
    	for(j = 0; j < n; j++)
      		b[i]+= A[i][j]*x[j];
    }

 	fprintf(file,"%d",n);

  	for (i=0; i<n; i++)
    	for (j=0; j<n; j++)
      		fprintf(file,"\n%3d %3d % .20e",i,j,A[i][j]);

  	for (i=0; i<n; i++)
      	fprintf(file,"\n%3d % .20e",i,b[i]);

    Liberar_matriz_real(n,n,A);
    free(b);
    free(x);
  	fclose(file);
	return 0;
}