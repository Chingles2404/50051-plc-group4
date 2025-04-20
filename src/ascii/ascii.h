#include <stdio.h>
#include <math.h>
#include <float.h>
#include "edge_detection.h"

#ifndef ASCII_MAIN_H
#define ASCII_MAIN_H
#define DEFAULT_TEMPLATE_SIZE 3

typedef struct {
    char character;
    const int* ascii_template; /* This is a flat array */
    int rows;
    int cols;
} AsciiTemplate;

double get_weight(char ascii_char);
int needs_normalisation(Matrix *matrix);
void normalise_edge_matrix(Matrix *edge_matrix, Matrix *binary_matrix);
double compute_mse(Matrix *edge_matrix, Matrix *ascii_template, char ascii_char);
char find_best_ascii(Matrix *edge_matrix);

#endif