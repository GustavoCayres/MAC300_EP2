#include <math.h>
#include <stdio.h>
#include <time.h>
#define nmax 700
#define E 0.0001 /*erro*/

void swap(double* a, double* b) {
	double temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

/* calculates the inner product between vectors v1 and v2 */
double inner_product(double v1[], double v2[], int size_v) {
	int i;
	double inner_product = 0;

	for (i = 0; i < size_v; i ++)
		inner_product += v1[i]*v2[i];
	
	return inner_product;
}

/* calculates the multiplication between a matrix and a vector */
void matrix_x_vector(double A[][nmax], double v[], int size, double aux) {
	int i, j;

	for (i = 0; i < size; i ++) {
		aux[i] = 0;
		for (j = 0; j < size; j ++)
			aux[i] += A[i][j]*v[j];
	}
}

/* calculates the A-norm of vector v */
/*double A_norm(double A[][nmax], double v[], int size) {
	int i, j;
	double temp[nmax];

	for (i = 0; i < size; i ++) {
		temp [i] = 0;
		for (j = 0; j < size; j ++)
			temp[i] += A[i][j]*v[j];
	}

	return sqrt(inner_product(v, temp))
}*/

/* multiplies a vector by a scalar */
void vector_by_a_scalar(double v[], double alfa, int size) {
	int i;

	for (i = 0; i < size; i ++)
		v[i] = v[i]*alfa;
}

/* multiplies a vector by a scalar and puts on the 4th argument*/
void vector_by_a_scalar_new_vector(double v[], double alfa, int size, double result[]) {
	int i;

	for (i = 0; i < size; i ++)
		result= v[i]*alfa;
 
}

/* Sum vectors, if operation = 1 it'll sum v1+v2, if it is -1 it'll subtract */
void vector_sum(double v1[], double v2[], int size, int operation) {
	int i;
	double temp[size];

	for (i = 0; i < size; i ++)
		v1[i] += operation*v2[i];

}


void conjugate_gradient(double A[][nmax], double b[], int size, int n_steps) {
	double x[size], r[size], p[size], pA[size], aux[size], rnew, rold, alfa, beta;
	int i;

	for (i = 0; i < size; i ++) {
		x[i] = 0;
		r[i] = b[i];
		p[i] = b[i];
	}

	rold = inner_product(r, r, size);

	for (i = 1; i < n_steps; i ++) {
		matrix_x_vector(A, p, size, pA); /* resposta no pA */
		alfa = rold / inner_product(p, pA, size);
		vector_by_a_scalar_new_vector(p, alfa, size, aux); 
		vector_sum(x, aux, size, 1); /* atualiza o próprio x */
		vector_by_a_scalar_new_vector(pA, alfa, size, aux);
		vector_sum(r, aux, size, -1); /* atualiza o próprio r */
		rnew = inner_product(r, r, size);
		if (sqrt(rnew) < 1e-10)
            break;

        vector_sum(vector_by_a_scalar(p ,rnew / rold, size), r, size), size, 1);
		rold = rnew;
	}

	for (i = 0; i < size; i ++) {
		b[i] = x[i];
	}
}

int main() {
	char file_name[100];
	FILE *file;
	double A[nmax][nmax], b[nmax], duration;
	int n, i, j, k, p[nmax];
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
}