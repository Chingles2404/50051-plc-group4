#include <stdio.h>
#include <math.h>
#include <float.h>
#include "main.h"

/* Define ASCII templates as a constant array */
const AsciiTemplate ASCII_TEMPLATES[TEMPLATE_COUNT] = {
    {'/', {
      {0, 0, 1},
      {0, 1, 0},
      {1, 0, 0}
    }},
    {'\\', {
      {1, 0, 0},
      {0, 1, 0},
      {0, 0, 1}
    }},
    {'|', {
      {0, 1, 0},
      {0, 1, 0},
      {0, 1, 0}
    }},
    {'-', {
      {0, 0, 0},
      {1, 1, 1},
      {0, 0, 0}
    }},
    {'X', {
      {1, 0, 1},
      {0, 1, 0},
      {1, 0, 1}
    }},
    {'+', {
      {0, 1, 0},
      {1, 1, 1},
      {0, 1, 0}
    }}
};

/* Normalize edge values into binary (0 or 1) */
void normalize_edge_matrix(int edge_matrix[SIZE][SIZE], int binary_matrix[SIZE][SIZE]) {
  int i, j;
  for (i = 0; i < SIZE; i++) {
      for (j = 0; j < SIZE; j++) {
          /* Convert grayscale edge values to binary: thresholding at 128 */
          binary_matrix[i][j] = (edge_matrix[i][j] > 128) ? 1 : 0;  
      }
  }
}

/* Compute MSE: sum of squared differences between matrices with weight for penalising mismatches */
double compute_mse(int edge_matrix[SIZE][SIZE], const int ascii_template[SIZE][SIZE], char ascii_char) {
  double mse = 0.0;
  int i, j;

  double weight = 1.0; /* Default neutral weight */

  /* Assign positive, neutral, and negative weights based on ASCII character type */
  if (ascii_char == '/' || ascii_char == '\\') {
      weight = 0.9;  /* Slightly favor diagonal edges */
  } else if (ascii_char == '|') {
      weight = 1.1;  /* Slightly penalize vertical bars if diagonals are dominant */
  } else if (ascii_char == '-') {
      weight = 1.2;  /* Penalize horizontal lines if not well-matched */
  } else if (ascii_char == 'X') {
      weight = 1.3;  /* Strong penalty if `X` is chosen when not needed */
  }

  /* Compute MSE with the adjusted weight */
  for (i = 0; i < SIZE; i++) {
      for (j = 0; j < SIZE; j++) {
          double diff = edge_matrix[i][j] - ascii_template[i][j];
          mse += (diff * diff) * weight; /* Apply weight to penalise mismatches */
      }
  }

  return mse / (SIZE * SIZE); /* Normalize MSE by dividing by total elements */
}

/* Finds the best matching ASCII character for a given 3x3 edge matrix */
char find_best_ascii(int edge_matrix[SIZE][SIZE]) {
  double min_mse = DBL_MAX;
  char best_match = ' ';
  int i;

  printf("Checking Weighted MSE for different ASCII templates:\n");

  /* Compare the input matrix with each ASCII template */
  for (i = 0; i < TEMPLATE_COUNT; i++) {
      double mse = compute_mse(edge_matrix, ASCII_TEMPLATES[i].ascii_template, ASCII_TEMPLATES[i].character);
      printf("Weighted MSE with '%c': %.5f\n", ASCII_TEMPLATES[i].character, mse);

      /* Select the ASCII character with the lowest MSE (best match) */
      if (mse < min_mse) {
          min_mse = mse;
          best_match = ASCII_TEMPLATES[i].character;
      }
  }

  printf("\nBest match: %c with Weighted MSE: %.5f\n\n", best_match, min_mse);
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

/* TODO: remove in future. Used for testing for now */
int main() {
  /* Test Case 1: `/` Diagonal */
  int edge_matrix_1[SIZE][SIZE] = {
      {0, 0, 1},
      {0, 1, 0},
      {1, 0, 0}
  };

  /* Test Case 2: `\` Diagonal */
  int edge_matrix_2[SIZE][SIZE] = {
      {1, 0, 0},
      {0, 1, 0},
      {0, 0, 1}
  };

  /* Test Case 3: Vertical Bar `|` */
  int edge_matrix_3[SIZE][SIZE] = {
      {0, 1, 0},
      {0, 1, 0},
      {0, 1, 0}
  };

  /* Test Case 4: Horizontal Bar `-` */
  int edge_matrix_4[SIZE][SIZE] = {
      {0, 0, 0},
      {1, 1, 1},
      {0, 0, 0}
  };

  /* Test Case 5: `X` Shape */
  int edge_matrix_5[SIZE][SIZE] = {
      {1, 0, 1},
      {0, 1, 0},
      {1, 0, 1}
  };

  /* Test Case 6: Realistic Edge Detection Input */
  int edge_matrix_6[SIZE][SIZE] = {
      {0, 100, 255},
      {50, 200, 100},
      {0, 50, 0}
  };

  /* Test Case 7: `+` Shape */
  int edge_matrix_7[SIZE][SIZE] = {
      {0, 1, 0},
      {1, 1, 1},
      {0, 1, 0}
  };

  /* Normalized binary matrix for Test Case 6 */
  int binary_matrix[SIZE][SIZE];

  /* Run Tests */
  printf("========== Test Case 1: `/` Diagonal ==========\n");
  print_matrix(edge_matrix_1);
  printf("ASCII character selected: %c\n\n", find_best_ascii(edge_matrix_1));

  printf("========== Test Case 2: `\\` Diagonal ==========\n");
  print_matrix(edge_matrix_2);
  printf("ASCII character selected: %c\n\n", find_best_ascii(edge_matrix_2));

  printf("========== Test Case 3: `|` Vertical Line ==========\n");
  print_matrix(edge_matrix_3);
  printf("ASCII character selected: %c\n\n", find_best_ascii(edge_matrix_3));

  printf("========== Test Case 4: `-` Horizontal Line ==========\n");
  print_matrix(edge_matrix_4);
  printf("ASCII character selected: %c\n\n", find_best_ascii(edge_matrix_4));

  printf("========== Test Case 5: `X` Shape ==========\n");
  print_matrix(edge_matrix_5);
  printf("ASCII character selected: %c\n\n", find_best_ascii(edge_matrix_5));

  printf("========== Test Case 6: `+` Shape ==========\n");
  print_matrix(edge_matrix_7);
  printf("ASCII character selected: %c\n\n", find_best_ascii(edge_matrix_7));

  printf("========== Test Case 7: Realistic Edge Detection ==========\n");
  print_matrix(edge_matrix_6);
  normalize_edge_matrix(edge_matrix_6, binary_matrix);
  printf("Normalized Binary Matrix:\n");
  print_matrix(binary_matrix);
  printf("ASCII character selected: %c\n\n", find_best_ascii(binary_matrix));

  return 0;
}