#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <math.h>
#include <float.h>

#define SIZE 3  /* 3x3 matrix size from edge detection output */

typedef int Matrix[SIZE][SIZE];

typedef struct {
    char character;
    int ascii_template[SIZE][SIZE];
} AsciiTemplate;

void normalize_edge_matrix(Matrix edge_matrix, Matrix binary_matrix);
double compute_mse(Matrix edge_matrix, const Matrix ascii_template, char ascii_char);
char find_best_ascii(Matrix edge_matrix);
void print_matrix(Matrix edge_matrix);

#endif
