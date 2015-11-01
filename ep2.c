#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#define E 1e-10 /* Acceptable error */
/* Compilation command: gcc -Wall -Wextra -ansi -pedantic -o ep2 ep2.c -lm */

/* use of structs to implement linked list nodes */
/* the sparse matrix will be represented by a list of lists */
struct col{
	int col;
	double value;
	struct col *right;
}col;

typedef struct col* column;

struct row{
	int row;
	struct row *down;
	struct col *right;
}row;

typedef struct row* sparse_matrix;

void create_matrix(sparse_matrix *sp) {
	sparse_matrix x;
	x = malloc(sizeof(sparse_matrix));
	x->row = -1;
	x->down = NULL;
	x->right = NULL;
	*sp = x;
}

void insert(sparse_matrix sp, int i, int j, double value) {
	sparse_matrix rnew;
	column cnew, pointer;
	sparse_matrix aux;
	if(sp->row == -1) {
		sp->row = i;
		cnew = malloc(sizeof(col));
		cnew->col = j;
		cnew->value = value;
		cnew->right = NULL;
		sp->right = cnew;
	}
	else {
		cnew = malloc(sizeof(col));
		cnew->col = j;
		cnew->value = value;
		cnew->right = NULL;
		for(aux = sp; aux->row != i && aux->down != NULL; aux = aux->down);
		if(aux->row == i) {
			for(pointer = aux->right; pointer->right != NULL; pointer = pointer->right);
			pointer->right = cnew;
		}
		else {
			rnew = malloc(sizeof(row));
			rnew->row = i;
			rnew->down = NULL;
			rnew->right = cnew;
			aux->down = rnew;
		}
	}
}

void free_right(column c){
	column atual, next;
	atual=c;
	while(atual != NULL){
		next = atual->right;
		free(atual);
		atual = next;
	}
}

void free_sparse_matrix(sparse_matrix sp){
	sparse_matrix down;
	if(sp != NULL){
		down = sp->down;
		free_right(sp->right);
		free(sp);
		free_sparse_matrix(down);
	}
}


/* calculates the inner product between vectors v1 and v2 */
double inner_product(double *v1, double *v2, int size) {
	int i;
	double inner_product = 0;

	for (i = 0; i < size; i ++)
		inner_product += v1[i]*v2[i];
	
	return inner_product;
}

/* calculates the multiplication between a matrix and a vector */
void matrix_vector_product(sparse_matrix A, double *v, double *result) {
	sparse_matrix aux;
	column pointer;

	for(aux = A; aux != NULL; aux = aux->down) {
		result[aux->row] = 0;
		for(pointer = aux->right; pointer != NULL; pointer = pointer->right) {
			result[aux->row] += (pointer->value)*v[pointer->col];
		}
	}
}

/* multiplies a vector by a scalar */
void vector_scalar_product(double *v, double alfa, int size, double *result) {
	int i;

	for (i = 0; i < size; i ++)
		result[i] = v[i]*alfa;
}

/* adds vectors v1 and v2 */
void vector_sum(double *v1, double *v2, int size, double *result) {
	int i;

	for (i = 0; i < size; i ++)
		result[i] = v1[i] + v2[i];
}

/* subtracts vector v2 from v1 */
void vector_subtraction(double *v1, double *v2, int size, double *result) {
	int i;

	for (i = 0; i < size; i ++)
		result[i] = v1[i] - v2[i];
}

/*solution saved in b */
void conjugate_gradient(sparse_matrix A, double *b, int size) {
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
    /* result of operations from all void functions used here are stored in the last argument */
	while (1) {
		matrix_vector_product(A, p, Ap);
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
	sparse_matrix A;
	double *b;
	double duration, aux;
	int n, i, j, k;
	clock_t start, end;
 
	printf("Nome do Arquivo: ");
	scanf("%s", file_name);
	file = fopen(file_name, "r");

	/* Reading file */
	if (file == NULL) {
		fprintf(stderr, "Não foi possível abrir o arquivo!\n");
		return -1;
	}

	fscanf(file, "%d", &n);
	create_matrix(&A);
	b = malloc(n*sizeof(double));

	for (k = 0; k < n*n; k ++) {
		fscanf(file, "%d %d", &i, &j);
		fscanf(file, "%lf", &aux);
		if(aux != 0)
			insert(A, i, j, aux);
	}
	for (k= 0; k < n; k ++) {
		fscanf(file, "%d", &i);
		fscanf(file, "%lf", &b[i]);
	}
	printf("Matriz foi lida com sucesso.\n");
	start = clock();
	conjugate_gradient(A, b, n);
	end = clock();
	duration = (double)(end - start) / CLOCKS_PER_SEC;
	printf("Tempo de resolução por Gradientes Conjugados: %e segundos\n", duration);
        
        /* test */
	for (i = 0; i < n; i ++) {
		if (b[i] - (1 + i%(n/100)) > 1e-5 || b[i] - (1 + i%(n/100)) < -1e-5)
			printf("Erro! %e  %d %d\n", b[i], (1 + i%(n/100)), i);
	}
	fclose(file);
	printf("Fim da Análise!\n");
	free_sparse_matrix(A);
	free(b);
	return 0;
}
