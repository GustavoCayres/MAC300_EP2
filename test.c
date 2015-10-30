#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#define nmax 1001
#define E 1e-10 /* Acceptable error */
/* Compilation command: gcc -Wall -Wextra -ansi -pedantic -o test test.c -lm */

/* minha ideia é implementar a matrix como uma lista de lista, como a operação que vamos fazer é Axp, entao guardando
linha por linha, fica mais facil de implementar essa operação, e para ler do arquivo vai ser fácil */
struct column{
	int col;
	double value;
	struct column *right;
}column;

typedef struct column* coluna;

struct row{
	int row;
	struct row *down;
	struct column *right;
}row;

typedef struct row* sparse_matrix;

void create_matrix(sparse_matrix sp) {
	sparse_matrix x;
	x = malloc(sizeof(sparse_matrix));
	x->row = -1;
	x->down = NULL;
	x->right = NULL;
	sp = x;
}

void insert(sparse_matrix sp, int i, int j, double value) {
	row rnew;
	column cnew;
	column *pointer;
	sparse_matrix aux;
	if(sp->row == -1) {
		sp->row = i;
		cnew = malloc(sizeof(column));
		cnew->col = j;
		cnew->value = value;
		cnew->right = NULL;
		sp->right = cnew;
	}
	else {
		cnew = malloc(sizeof(column));
		cnew->col = j;
		cnew->value = value;
		cnew->right = NULL;
		for(aux = sp; aux->down != NULL && aux->row != i; aux = aux->down);
		if(aux->down == NULL) {
			rnew = malloc(sizeof(row));
			rnew->row = i;
			rnew->down = NULL;
			rnew->right = cnew;
			aux->down = rnew;
		}
		else{
			for(*pointer = aux->right; (*pointer)->right != NULL; *pointer = (*pointer)->right);
			(*pointer)->right = cnew;
		}
	}
}

void free_right(coluna c){
	coluna atual, next;
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

int main() {
	sparse_matrix sp;
	int i, j;
	for(i = 0; i<7; i++)
		for(j=0; j<3; j++)
			insert(sp, i, j, i+j+.5);

	printf("%f %d,%d\n", sp->right->value, sp->row, sp->right->col);
	free_sparse_matrix(sp);
	return 1;
}