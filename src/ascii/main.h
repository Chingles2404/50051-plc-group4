#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <math.h>
#include <float.h>

#define SIZE 3  /* 3x3 matrix size from edge detection output */
#define TEMPLATE_COUNT 6  /* Number of ASCII characters to choose from (i.e. number of templates) */

/* Structure to store ASCII character templates */
typedef struct {
    char character;
    int ascii_template[SIZE][SIZE];
} AsciiTemplate;

/* Function Prototypes */
void normalize_edge_matrix(int edge_matrix[SIZE][SIZE], int binary_matrix[SIZE][SIZE]);
double compute_mse(int edge_matrix[SIZE][SIZE], const int ascii_template[SIZE][SIZE], char ascii_char);
char find_best_ascii(int edge_matrix[SIZE][SIZE]);
void print_matrix(int matrix[SIZE][SIZE]);

#endif
