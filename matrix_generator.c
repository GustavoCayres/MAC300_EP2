#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#define NMAX 2500
#define E 0.0001
/* NMAX decide o tamanho da matriz e do vetor b */

double random_double() {
	double r = (double)rand() / RAND_MAX;
    return (-1 + r * 2);
}

double **Alocar_matriz_real (int m, int n){
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


void Liberar_matriz_real(int m,int n,double **v){
	int i; /* variavel auxiliar */
	if (v == NULL)
		return ;
	if(m < 1 || n < 1) {/* verifica parametros recebidos */
		printf ("** Erro: Parametro invalido **\n");
		return ;
	}
	for(i = 0; i < m; i++) free (v[i]);/* libera as linhas da matriz */
	free (v);/* libera a matriz */
	/* retorna um ponteiro nulo */
}

int main() {
	char file_name[50];
	FILE *file;
	double **A, T, r, x[NMAX], b[NMAX];
	int i, j;

	srand((unsigned) time(NULL));
	printf("Entre com T: \n");
	scanf("%lf", &T);
	sprintf(file_name, "sparse_matrix_%d.txt", NMAX);
	file = fopen(file_name, "w" );

	A = Alocar_matriz_real(NMAX, NMAX);

	for (i = 0; i < NMAX; i ++) {
		A[i][i] = 1;
		for(j = i + 1; j < NMAX; j ++) {
			r = random_double();
			if (fabs(r) > T)
				r = 0.0;
			A[i][j] = A[j][i] = r;
		}
	}

	for (i = 0; i < NMAX; i++)
    	x[i]= 1+i%(NMAX/100);

	for (i = 0; i < NMAX; i++) {
    	b[i] = 0;
    	for(j = 0; j < NMAX; j++)
      		b[i]+= A[i][j]*x[j];
    }

 	fprintf(file,"%d",NMAX);

  	for (i=0; i<NMAX; i++)
    	for (j=0; j<NMAX; j++)
      		fprintf(file,"\n%3d %3d % .20e",i,j,A[i][j]);

  	for (i=0; i<NMAX; i++)
      	fprintf(file,"\n%3d % .20e",i,b[i]);

    Liberar_matriz_real(NMAX,NMAX,A);
  	fclose(file);
	return 0;
}


	/*
	char file_name[100];
	FILE *file;
	double A[nmax][nmax], b[nmax], duration;
	int n, i, j, k, p[nmax], size;
	clock_t start, end;

	printf("Tamanho da matriz: ");
	scanf("%d", size);

	file = fopen(file_name, "r");

	
	if (file == NULL) {
		fprintf(stderr, "Não foi possível abrir o arquivo!\n");
		return -1;
	}
	fscanf(file, "%d", &n);
	for (k = 0; k < n*n; k ++) {
		fscanf(file, "%d %d", &i, &j);
		fscanf(file, "%lf", &A[i][j]);
	}
	for (k= 0; k < n; k ++) {
		fscanf(file, "%d", &i);
		fscanf(file, "%lf", &b[i]);
	}

	start = clock();
	conjugate_gradient(A, b, n, n_steps);
	end = clock();
	duration = (double)(end - start) / CLOCKS_PER_SEC;
	printf("conjugate_gradient tempo %e segundos\n", duration);

	for (i = 0; i < n; i ++) {
		if (b[i] - (1 + i%(n/100)) > E || b[i] - (1 + i%(n/100)) < -E)
			printf("Erro! %e  %d %d\n", b[i],-(1 + i%(n/100)), i);
	}
	printf("Fim da Análise!\n");
	return 0;
	*/
