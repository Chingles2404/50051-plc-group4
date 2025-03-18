#include <stdio.h>
#include <math.h>
#include <float.h>

#define SIZE 3  /* 3x3 matrix size */
#define TEMPLATE_COUNT 5  /* Number of ASCII templates */

/* Structure to store ASCII character templates */
typedef struct {
    char character;
    int ascii_template[SIZE][SIZE];
} AsciiTemplate;

/* Define ASCII templates as a constant array */
const AsciiTemplate ASCII_TEMPLATES[TEMPLATE_COUNT] = {
    {'/', {{0, 0, 1}, {0, 1, 0}, {1, 0, 0}}},
    {'\\', {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}},
    {'|', {{0, 1, 0}, {0, 1, 0}, {0, 1, 0}}},
    {'-', {{0, 0, 0}, {1, 1, 1}, {0, 0, 0}}},
    {'X', {{1, 0, 1}, {0, 1, 0}, {1, 0, 1}}}
};

/* Normalize edge values into binary (0 or 1) */
void normalize_edge_matrix(int edge_matrix[SIZE][SIZE], int binary_matrix[SIZE][SIZE]) {
  int i, j;
  for (i = 0; i < SIZE; i++) {
      for (j = 0; j < SIZE; j++) {
          binary_matrix[i][j] = (edge_matrix[i][j] > 128) ? 1 : 0;  
      }
  }
}

/* Computes Mean Squared Error (MSE) between two 3x3 matrices */
double compute_mse(int edge_matrix[SIZE][SIZE], const int ascii_template[SIZE][SIZE]) {
  double mse = 0.0;
  int i, j;  /* Declare loop variables at the start */

  for (i = 0; i < SIZE; i++) {
      for (j = 0; j < SIZE; j++) {
          double diff = edge_matrix[i][j] - ascii_template[i][j];
          mse += diff * diff;
      }
  }
  return mse / (SIZE * SIZE);
}

/* Finds the best matching ASCII character for a given 3x3 edge matrix */
char find_best_ascii(int edge_matrix[SIZE][SIZE]) {
    double min_mse = DBL_MAX;
    char best_match = ' ';  /* Default to space if no good match */
    int i;

    for (i = 0; i < TEMPLATE_COUNT; i++) {
        double mse = compute_mse(edge_matrix, ASCII_TEMPLATES[i].ascii_template);
        if (mse < min_mse) {
            min_mse = mse;
            best_match = ASCII_TEMPLATES[i].character;
        }
    }

    return best_match;
}

/* Prints a 3x3 matrix (for debugging) */
void print_matrix(int matrix[SIZE][SIZE]) {
    int i, j;
    printf("Matrix:\n");
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

/* Main function with both inputs */
int main() {
  /* First test case (original binary input) */
  int edge_matrix_1[SIZE][SIZE] = {
      {0, 0, 1},
      {0, 1, 0},
      {1, 0, 0}
  };

  /* Second test case (realistic edge detection matrix) */
  int edge_matrix_2[SIZE][SIZE] = {
      {0, 100, 255},
      {50, 200, 100},
      {0, 50, 0}
  };

  /* Normalized binary matrix for the second test case */
  int binary_matrix[SIZE][SIZE];
  char ascii_char_1 = find_best_ascii(edge_matrix_1);
  char ascii_char_2 = find_best_ascii(binary_matrix);

  printf("========== Test Case 1: Original Binary Input ==========\n");
  print_matrix(edge_matrix_1);
  printf("ASCII character selected: %c\n\n", ascii_char_1);

  /* Normalize and Print Test Case 2 */
  printf("========== Test Case 2: Realistic Edge Detection Input ==========\n");
  printf("Original Edge Matrix:\n");
  print_matrix(edge_matrix_2);
  
  normalize_edge_matrix(edge_matrix_2, binary_matrix);
  printf("Normalized Binary Matrix:\n");
  print_matrix(binary_matrix);

  printf("ASCII character selected: %c\n\n", ascii_char_2);

  return 0;
}
