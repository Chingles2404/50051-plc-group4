#include <stdio.h>
#include <math.h>
#include <float.h>
#include "ascii.h"
#include <assert.h>


const AsciiTemplate ASCII_TEMPLATES[] = {

  /* All-heavy */
  {'@', (int[]){1,1,1, 1,1,1, 1,1,1}, 3, 3},

  /* Original 6 templates */
  {'/', (int[]){0,0,1, 0,1,0, 1,0,0}, 3, 3},
  {'\\', (int[]){1,0,0, 0,1,0, 0,0,1}, 3, 3},
  {'|', (int[]){0,1,0, 0,1,0, 0,1,0}, 3, 3},
  {'-', (int[]){0,0,0, 1,1,1, 0,0,0}, 3, 3},
  {'X', (int[]){1,0,1, 0,1,0, 1,0,1}, 3, 3},
  {'+', (int[]){0,1,0, 1,1,1, 0,1,0}, 3, 3},

  /* Horizontal and vertical variants */
  {'-', (int[]){1,1,1, 0,0,0, 0,0,0}, 3, 3},
  {'-', (int[]){0,0,0, 1,1,1, 0,0,0}, 3, 3},
  {'_', (int[]){0,0,0, 0,0,0, 1,1,1}, 3, 3},
  {'=', (int[]){1,1,1, 0,0,0, 1,1,1}, 3, 3},
  {'=', (int[]){1,1,1, 1,1,1, 0,0,0}, 3, 3},
  {'=', (int[]){0,0,0, 1,1,1, 1,1,1}, 3, 3},
  {'|', (int[]){1,0,0, 1,0,0, 1,0,0}, 3, 3},
  {'|', (int[]){0,1,0, 0,1,0, 0,1,0}, 3, 3},
  {'|', (int[]){0,0,1, 0,0,1, 0,0,1}, 3, 3},

  /* Edges */
  {'L', (int[]){1,0,0, 1,0,0, 1,1,1}, 3, 3},
  {'J', (int[]){1,1,1, 0,0,1, 0,0,1}, 3, 3},
  {'r', (int[]){1,1,1, 1,0,0, 1,0,0}, 3, 3},
  {'T', (int[]){1,1,1, 0,1,0, 0,1,0}, 3, 3},
  {'7', (int[]){1,1,1, 0,1,0, 1,0,0}, 3, 3},
  {'[', (int[]){1,1,0, 1,0,0, 1,1,0}, 3, 3},
  {'[', (int[]){0,1,1, 0,1,0, 0,1,1}, 3, 3},
  {']', (int[]){1,1,0, 0,1,0, 1,1,0}, 3, 3},
  {']', (int[]){0,1,1, 0,0,1, 0,1,1}, 3, 3},
  {'O', (int[]){1,1,1, 1,0,1, 1,1,1}, 3, 3},
  {'F', (int[]){1,1,1, 1,1,1, 1,0,0}, 3, 3},

  /* Dot variants */
  {'`', (int[]){1,0,0, 0,0,0, 0,0,0}, 3, 3},
  {'`', (int[]){0,1,0, 0,0,0, 0,0,0}, 3, 3},
  {'`', (int[]){0,0,1, 0,0,0, 0,0,0}, 3, 3},
  {'.', (int[]){0,0,0, 1,0,0, 0,0,0}, 3, 3},
  {'.', (int[]){0,0,0, 0,1,0, 0,0,0}, 3, 3},
  {'.', (int[]){0,0,0, 0,0,1, 0,0,0}, 3, 3},
  {'.', (int[]){0,0,0, 0,0,0, 1,0,0}, 3, 3},
  {'.', (int[]){0,0,0, 0,0,0, 0,1,0}, 3, 3},
  {'.', (int[]){0,0,0, 0,0,0, 0,0,1}, 3, 3},

  /* Colon variants */
  {':', (int[]){1,0,0, 1,0,0, 0,0,0}, 3, 3},
  {':', (int[]){0,0,0, 1,0,0, 1,0,0}, 3, 3},
  {':', (int[]){0,1,0, 0,1,0, 0,0,0}, 3, 3},
  {':', (int[]){0,0,0, 0,1,0, 0,1,0}, 3, 3},
  {':', (int[]){0,0,1, 0,0,1, 0,0,0}, 3, 3},
  {':', (int[]){0,0,0, 0,0,1, 0,0,1}, 3, 3},
};


#define TEMPLATE_COUNT (sizeof(ASCII_TEMPLATES) / sizeof(ASCII_TEMPLATES[0]))

/* Determine the weight based on the ASCII character type */
double get_weight(char ascii_char) {
  switch (ascii_char) {
      case '/': case '\\': return 0.9;
      case '|':            return 1.1;
      case '-':            return 1.2;
      case 'X':            return 1.3;
      default:             return 1.0;
  }
}

int needs_normalisation(Matrix *matrix) {
  for (int i = 0; i < matrix->numberRows; i++) {
      for (int j = 0; j < matrix->numberCols; j++) {
          if (getMatrixElement(matrix, i, j) > 1) {
              return 1;
          }
      }
  }
  return 0;
}

/* Normalise edge values into binary (0 or 1) */
void normalise_edge_matrix(Matrix *edge_matrix, Matrix *binary_matrix) {

  int rows = edge_matrix->numberRows;

  int cols = edge_matrix->numberCols;

  for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
          // Convert grayscale edge values to binary: thresholding at 128
          int value = getMatrixElement(edge_matrix, i, j);

          int binary_value = (value > 128) ? 1 : 0;
          setMatrixElement(binary_matrix, i, j, binary_value);
      }
  }
}


/* Sum of squared differences between matrices with weight for penalising mismatches */
double compute_mse(Matrix *edge_matrix, Matrix *ascii_template, char ascii_char) {

  int rows = edge_matrix->numberRows;
  int cols = edge_matrix->numberCols;

  double mse = 0.0;
  double weight = get_weight(ascii_char);

  for (int i = 0; i < rows; i++) {

      for (int j = 0; j < cols; j++) {

          double diff = getMatrixElement(edge_matrix, i, j) - getMatrixElement(ascii_template, i, j);
          mse += (diff * diff) * weight;  // Apply weight to penalise mismatches
      }
  }

  return mse / (rows * cols);  // Normalise by total number of elements
}


/* Finds the best matching ASCII character for a given 3x3 edge matrix */
char find_best_ascii(Matrix *edge_matrix) {
  int i;
  double min_mse = DBL_MAX;
  char best_match = ' ';

  /* printf("\nChecking Weighted MSE for different ASCII templates:\n"); */

  /* Compare the input matrix with each ASCII template */
  for (i = 0; i < TEMPLATE_COUNT; i++) {

    const int* flat_values = (const int*)ASCII_TEMPLATES[i].ascii_template;

    Matrix* template_matrix = createMatrixFromFlatArray(3, 3, flat_values);

    double mse = compute_mse(edge_matrix, template_matrix, ASCII_TEMPLATES[i].character);
    /* printf("Weighted MSE with '%c': %.5f\n", ASCII_TEMPLATES[i].character, mse); */

    /* Select the ASCII character with the lowest MSE (best match) */
    if (mse < min_mse) {
        min_mse = mse;
        best_match = ASCII_TEMPLATES[i].character;
    }
  }

  // printf("\nBest match: %c with Weighted MSE: %.5f\n\n", best_match, min_mse);
  return best_match;
}

void print_matrix(Matrix *matrix) {
  int i, j;
  printf("Matrix (%d x %d):\n", matrix->numberRows, matrix->numberCols);
  for (i = 0; i < matrix->numberRows; i++) {
      for (j = 0; j < matrix->numberCols; j++) {
          printf("%d ", getMatrixElement(matrix, i, j));
      }
      printf("\n");
  }
}
