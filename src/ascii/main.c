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

/* Normalize edge values into binary (0 or 1) */
void normalize_edge_matrix(Matrix edge_matrix, Matrix binary_matrix) {
  int i, j;
  for (i = 0; i < SIZE; i++) {
      for (j = 0; j < SIZE; j++) {
          /* Convert grayscale edge values to binary: thresholding at 128 */
          binary_matrix[i][j] = (edge_matrix[i][j] > 128) ? 1 : 0;  
      }
  }
}

/* Compute MSE: sum of squared differences between matrices with weight for penalising mismatches */
double compute_mse(Matrix edge_matrix, const Matrix ascii_template, char ascii_char) {
  int i, j;
  double mse = 0.0;
  double weight = get_weight(ascii_char);

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
char find_best_ascii(Matrix edge_matrix) {
  int i;
  double min_mse = DBL_MAX;
  char best_match = ' ';

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
void print_matrix(Matrix matrix) {
    int i, j;
    printf("Matrix:\n");
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

/* === Test Case Runner === */
void run_test_case(const char *label, Matrix matrix, int normalize) {
  Matrix binary_matrix;

  printf("========== %s ==========\n", label);
  print_matrix(matrix);

  if (normalize) {
      normalize_edge_matrix(matrix, binary_matrix);
      printf("Normalized Binary Matrix:\n");
      print_matrix(binary_matrix);
      printf("ASCII character selected: %c\n\n", find_best_ascii(binary_matrix));
  } else {
      printf("ASCII character selected: %c\n\n", find_best_ascii(matrix));
  }
}

/* TODO: remove in future. Used for testing for now */
int main() {
  int i;

  Matrix test_cases[][1] = {
      {{{0, 0, 1}, {0, 1, 0}, {1, 0, 0}}},
      {{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}},
      {{{0, 1, 0}, {0, 1, 0}, {0, 1, 0}}},
      {{{0, 0, 0}, {1, 1, 1}, {0, 0, 0}}},
      {{{1, 0, 1}, {0, 1, 0}, {1, 0, 1}}},
      {{{0, 1, 0}, {1, 1, 1}, {0, 1, 0}}},
      {{{0, 100, 255}, {50, 200, 100}, {0, 50, 0}}}
  };

  const char *labels[] = {
      "Test Case 1: `/` Diagonal",
      "Test Case 2: `\\` Diagonal",
      "Test Case 3: `|` Vertical Line",
      "Test Case 4: `-` Horizontal Line",
      "Test Case 5: `X` Shape",
      "Test Case 6: `+` Shape",
      "Test Case 7: Realistic Edge Detection"
  };

  /* Flags indicating whether each corresponding test case should be normalized (1 = normalize, 0 = don't normalise) */
  int normalize_flags[] = {0, 0, 0, 0, 0, 0, 1};

  for (i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
      run_test_case(labels[i], test_cases[i][0], normalize_flags[i]);
  }

  return 0;
}