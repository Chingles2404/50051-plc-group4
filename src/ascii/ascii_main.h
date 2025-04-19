#ifndef ASCII_MAIN_H
#define ASCII_MAIN_H

#include <stdio.h>
#include <math.h>
#include <float.h>
#include "edge_detection.h"

#define DEFAULT_TEMPLATE_SIZE 3

// Use the "Matrix" structure instead as we have in edge_detection utils.
// typedef int Matrix[SIZE][SIZE];

typedef struct {
    char character;
    const int* ascii_template; // This is a flat array
    int rows;
    int cols;
} AsciiTemplate;

typedef struct {
    const char* label;
    const int values[9];
} TestCase;

int needs_normalisation(Matrix *matrix);
void normalise_edge_matrix(Matrix *edge_matrix, Matrix *binary_matrix);
double compute_mse(Matrix *edge_matrix, Matrix *ascii_template, char ascii_char);
char find_best_ascii(Matrix *edge_matrix);
void print_matrix(Matrix *edge_matrix);

#endif