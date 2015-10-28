#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#define nmax 1001
#define E 1e-5 /* Acceptable error */
/* Compilation command: gcc -Wall -Wextra -ansi -pedantic -o ep2 ep2.c -lm */

void swap(double* a, double* b) {
	double temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

/* calculates the inner product between vectors v1 and v2 */
double inner_product(double v1[], double v2[], int size) {
	int i;
	double inner_product = 0;

	for (i = 0; i < size; i ++)
		inner_product += v1[i]*v2[i];
	
	return inner_product;
}

/* calculates the multiplication between a matrix and a vector */
void matrix_vector_product(double A[][nmax], double v[], int size, double result[]) {
	int i, j;

	for (i = 0; i < size; i ++) {
		result[i] = 0;
		for (j = 0; j < size; j ++)
			result[i] += A[i][j]*v[j];
	}
}

/* multiplies a vector by a scalar */
void vector_scalar_product(double v[], double alfa, int size, double result[]) {
	int i;

	for (i = 0; i < size; i ++)
		result[i] = v[i]*alfa;
}

/* adds vectors v1 and v2 */
void vector_sum(double v1[], double v2[], int size, double result[]) {
	int i;

	for (i = 0; i < size; i ++)
		result[i] = v1[i] + v2[i];
}

/* subtracts vector v2 from v1 */
void vector_subtraction(double v1[], double v2[], int size, double result[]) {
	int i;

	for (i = 0; i < size; i ++)
		result[i] = v1[i] - v2[i];
}

void conjugate_gradient(double A[][nmax], double b[], int size) {
	double *x, *r, *p, *Ap, *aux, rnew, rold, alfa;
	int i;

    x = (double*) malloc(size*sizeof(double));
    r = (double*) malloc(size*sizeof(double));
    p = (double*) malloc(size*sizeof(double));
    Ap = (double*) malloc(size*sizeof(double));
    aux = (double*) malloc(size*sizeof(double));

	for (i = 0; i < size; i ++) {
		x[i] = 0;
		r[i] = b[i];
		p[i] = b[i];
	}

	rold = inner_product(r, r, size);
    /* result of operations from all void functions are saved at last argument */
	/* for (i = 1; i < size; i ++) {*/
	i = 0;
	while (1) {
		i ++;
		matrix_vector_product(A, p, size, Ap);
		alfa = rold / inner_product(p, Ap, size); /*step length*/
		vector_scalar_product(p, alfa, size, aux); 
		vector_sum(x, aux, size, x);
		vector_scalar_product(Ap, alfa, size, aux);
		vector_subtraction(r, aux, size, r);
		rnew = inner_product(r, r, size);
		if (sqrt(rnew) < E)
            break;

        vector_scalar_product(p, rnew / rold, size, p);
        vector_sum(p, r, size, p);
		rold = rnew;
	}
	printf("%d\n", i);

	for (i = 0; i < size; i ++) {
		b[i] = x[i];
	}

	free(x);
	free(r);
	free(p);
	free(Ap);
	free(aux);
}

int main() {
	char file_name[100];
	FILE *file;
	double A[nmax][nmax], b[nmax], duration;
	int n, i, j, k;
	clock_t start, end;

	printf("Nome do Arquivo: ");
	scanf("%s", file_name);
	file = fopen(file_name, "r");

	/* Lendo dados */
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
	conjugate_gradient(A, b, n);
	end = clock();
	duration = (double)(end - start) / CLOCKS_PER_SEC;
	printf("conjugate_gradient tempo %e segundos\n", duration);

	for (i = 0; i < n; i ++) {
		if (b[i] - (1 + i%(n/100)) > 1e-5 || b[i] - (1 + i%(n/100)) < -1e-5)
			printf("Erro! %e  %d %d\n", b[i], (1 + i%(n/100)), i);
	}
	printf("Fim da Análise!\n");
	return 0;
}