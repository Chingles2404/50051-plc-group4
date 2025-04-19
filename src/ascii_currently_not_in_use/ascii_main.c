#include <stdio.h>
#include <math.h>
#include <float.h>
<<<<<<<< HEAD:src/ascii_currently_not_in_use/main.c
#include "main.h"
========
#include "ascii_main.h"
>>>>>>>> main:src/ascii_currently_not_in_use/ascii_main.c

/* Define ASCII templates as a constant array */
const AsciiTemplate ASCII_TEMPLATES[] = {
  /* All-heavy */
  {'@',  {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}}},

  /* Original 6 templates */
  {'/',  {{0, 0, 1}, {0, 1, 0}, {1, 0, 0}}},
  {'\\', {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}},
  {'|',  {{0, 1, 0}, {0, 1, 0}, {0, 1, 0}}},
  {'-',  {{0, 0, 0}, {1, 1, 1}, {0, 0, 0}}},
  {'X',  {{1, 0, 1}, {0, 1, 0}, {1, 0, 1}}},
  {'+',  {{0, 1, 0}, {1, 1, 1}, {0, 1, 0}}},

  /* Horizontal and vertical variants */
  {'-',  {{1, 1, 1}, {0, 0, 0}, {0, 0, 0}}},
  {'_',  {{0, 0, 0}, {0, 0, 0}, {1, 1, 1}}},
  {'=', {{1, 1, 1}, {0, 0, 0}, {1, 1, 1}}},
  {'=', {{1, 1, 1}, {1, 1, 1}, {0, 0, 0}}},
  {'=', {{0, 0, 0}, {1, 1, 1}, {1, 1, 1}}},
  {'|',  {{1, 0, 0}, {1, 0, 0}, {1, 0, 0}}},
  {'|',  {{0, 0, 1}, {0, 0, 1}, {0, 0, 1}}},

  /* Edges */
  {'L',  {{1, 0, 0}, {1, 0, 0}, {1, 1, 1}}},
  {'J',  {{1, 1, 1}, {0, 0, 1}, {0, 0, 1}}},
  {'r',  {{1, 1, 1}, {1, 0, 0}, {1, 0, 0}}},
  {'T',  {{1, 1, 1}, {0, 1, 0}, {0, 1, 0}}},
  {'7',  {{1, 1, 1}, {0, 1, 0}, {1, 0, 0}}},
  {'[',  {{1, 1, 0}, {1, 0, 0}, {1, 1, 0}}},
  {'[',  {{0, 1, 1}, {0, 1, 0}, {0, 1, 1}}},
  {']',  {{1, 1, 0}, {0, 1, 0}, {1, 1, 0}}},
  {']',  {{0, 1, 1}, {0, 0, 1}, {0, 1, 1}}},
  {'O',  {{1, 1, 1}, {1, 0, 1}, {1, 1, 1}}},
  {'F',  {{1, 1, 1}, {1, 1, 1}, {1, 0, 0}}},

  /* Dot variants */
  {'`', {{1, 0, 0}, {0, 0, 0}, {0, 0, 0}}},
  {'`', {{0, 1, 0}, {0, 0, 0}, {0, 0, 0}}},
  {'`', {{0, 0, 1}, {0, 0, 0}, {0, 0, 0}}},
  {'.', {{0, 0, 0}, {1, 0, 0}, {0, 0, 0}}},
  {'.', {{0, 0, 0}, {0, 0, 1}, {0, 0, 0}}},
  {'.', {{0, 0, 0}, {0, 0, 0}, {1, 0, 0}}},
  {'.', {{0, 0, 0}, {0, 0, 0}, {0, 1, 0}}},
  {'.', {{0, 0, 0}, {0, 0, 0}, {0, 0, 1}}},

  {':', {{1, 0, 0}, {1, 0, 0}, {0, 0, 0}}},
  {':', {{0, 0, 0}, {1, 0, 0}, {1, 0, 0}}},
  {':', {{0, 1, 0}, {0, 1, 0}, {0, 0, 0}}},
  {':', {{0, 0, 0}, {0, 1, 0}, {0, 1, 0}}},
  {':', {{0, 0, 1}, {0, 0, 1}, {0, 0, 0}}},
  {':', {{0, 0, 0}, {0, 0, 1}, {0, 0, 1}}},
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

int needs_normalization(Matrix matrix) {
  int i, j;
  for (i = 0; i < SIZE; i++) {
      for (j = 0; j < SIZE; j++) {
          if (matrix[i][j] > 1) {
              return 1;
          }
      }
  }
  return 0;
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

  /* printf("\nChecking Weighted MSE for different ASCII templates:\n"); */

  /* Compare the input matrix with each ASCII template */
  for (i = 0; i < TEMPLATE_COUNT; i++) {
      double mse = compute_mse(edge_matrix, ASCII_TEMPLATES[i].ascii_template, ASCII_TEMPLATES[i].character);
      /* printf("Weighted MSE with '%c': %.5f\n", ASCII_TEMPLATES[i].character, mse); */

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
void run_test_case(const char *label, Matrix matrix) {
  Matrix binary_matrix;

  printf("========== %s ==========\n", label);
  print_matrix(matrix);

  if (needs_normalization(matrix)) {
      normalize_edge_matrix(matrix, binary_matrix);
      printf("\nNormalized Binary Matrix:\n");
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
      /* Exact matches for defined templates */
      {{{1, 1, 1}, {1, 1, 1}, {1, 1, 1}}},  /* '@' */
      {{{0, 0, 1}, {0, 1, 0}, {1, 0, 0}}},  /* '/' */
      {{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}},  /* '\' */
      {{{0, 1, 0}, {0, 1, 0}, {0, 1, 0}}},  /* '|' */
      {{{0, 0, 0}, {1, 1, 1}, {0, 0, 0}}},  /* '-' */
      {{{1, 0, 1}, {0, 1, 0}, {1, 0, 1}}},  /* 'X' */
      {{{0, 1, 0}, {1, 1, 1}, {0, 1, 0}}},  /* '+' */
      {{{1, 1, 1}, {0, 0, 0}, {0, 0, 0}}},  /* '-' */
      {{{0, 0, 0}, {0, 0, 0}, {1, 1, 1}}},  /* '_' */
      {{{1, 1, 1}, {0, 0, 0}, {1, 1, 1}}},  /* '=' */
      {{{1, 1, 1}, {1, 1, 1}, {0, 0, 0}}},  /* '=' */
      {{{0, 0, 0}, {1, 1, 1}, {1, 1, 1}}},  /* '=' */
      {{{1, 0, 0}, {1, 0, 0}, {1, 0, 0}}},  /* '|' */
      {{{0, 0, 1}, {0, 0, 1}, {0, 0, 1}}},  /* '|' */
      {{{1, 0, 0}, {1, 0, 0}, {1, 1, 1}}},  /* 'L' */
      {{{1, 1, 1}, {0, 0, 1}, {0, 0, 1}}},  /* 'J' */
      {{{1, 1, 1}, {1, 0, 0}, {1, 0, 0}}},  /* 'r' */
      {{{1, 1, 1}, {0, 1, 0}, {0, 1, 0}}},  /* 'T' */
      {{{1, 1, 1}, {0, 1, 0}, {1, 0, 0}}},  /* '7' */
      {{{1, 1, 0}, {1, 0, 0}, {1, 1, 0}}},  /* '[' */
      {{{0, 1, 1}, {0, 1, 0}, {0, 1, 1}}},  /* '[' variant */
      {{{1, 1, 0}, {0, 1, 0}, {1, 1, 0}}},  /* ']' */
      {{{0, 1, 1}, {0, 0, 1}, {0, 1, 1}}},  /* ']' variant */
      {{{1, 1, 1}, {1, 0, 1}, {1, 1, 1}}},  /* 'O' */
      {{{1, 1, 1}, {1, 1, 1}, {1, 0, 0}}},  /* 'F' */
      {{{1, 0, 0}, {0, 0, 0}, {0, 0, 0}}},  /* '`' (dot top-left) */
      {{{0, 1, 0}, {0, 0, 0}, {0, 0, 0}}},  /* '`' (dot top-center) */
      {{{0, 0, 1}, {0, 0, 0}, {0, 0, 0}}},  /* '`' (dot top-right) */
      {{{0, 0, 0}, {1, 0, 0}, {0, 0, 0}}},  /* '.' (middle-left) */
      {{{0, 0, 0}, {0, 0, 1}, {0, 0, 0}}},  /* '.' (middle-right) */
      {{{0, 0, 0}, {0, 0, 0}, {1, 0, 0}}},  /* '.' (bottom-left) */
      {{{0, 0, 0}, {0, 0, 0}, {0, 1, 0}}},  /* '.' (bottom-center) */
      {{{0, 0, 0}, {0, 0, 0}, {0, 0, 1}}},  /* '.' (bottom-right) */
      {{{1, 0, 0}, {1, 0, 0}, {0, 0, 0}}},  /* ':' (top half left) */
      {{{0, 0, 0}, {1, 0, 0}, {1, 0, 0}}},  /* ':' (bottom half left) */
      {{{0, 1, 0}, {0, 1, 0}, {0, 0, 0}}},  /* ':' (top half center) */
      {{{0, 0, 0}, {0, 1, 0}, {0, 1, 0}}},  /* ':' (bottom half center) */
      {{{0, 0, 1}, {0, 0, 1}, {0, 0, 0}}},  /* ':' (top half right) */
      {{{0, 0, 0}, {0, 0, 1}, {0, 0, 1}}},  /* ':' (bottom half right) */

      /* Unmatched patterns — should match to closest known character */
      {{{1, 1, 1}, {1, 0, 0}, {0, 0, 0}}},  /* L-corner (should match '-', 'L', or similar) */
      {{{0, 0, 0}, {0, 0, 0}, {1, 1, 0}}},  /* Bottom-left block */
      {{{0, 1, 0}, {1, 1, 1}, {0, 0, 0}}},  /* T-shaped top-heavy */
      {{{0, 1, 1}, {0, 1, 1}, {0, 0, 0}}},  /* Top-right box */
      {{{1, 0, 1}, {1, 0, 1}, {1, 0, 1}}},  /* Double-vertical sides */
      {{{0, 1, 0}, {0, 0, 0}, {0, 1, 0}}},   /* Vertical dots top and bottom */

      {{{0, 0, 255}, {0, 200, 0}, {255, 0, 0}}},    /* Diagonal like '/' */
      {{{255, 0, 0}, {0, 200, 0}, {0, 0, 255}}},    /* Diagonal like '\' */
      {{{50, 180, 50}, {30, 200, 30}, {70, 255, 60}}}, /* Likely '|' */
      {{{5, 10, 20}, {240, 250, 245}, {10, 5, 0}}}, /* Likely '-' */
      {{{200, 0, 200}, {0, 180, 0}, {200, 0, 200}}}, /* Likely 'X' */
      {{{50, 160, 60}, {220, 255, 210}, {80, 170, 90}}}, /* Likely '+' */
      {{{120, 130, 140}, {130, 125, 120}, {140, 110, 100}}}, /* Random blur */
  
  };

  const char *labels[] = {
    "Direct match: '@'", "Direct match: '/'", "Direct match: '\\'", "Direct match: '|'", "Direct match: '-'",
    "Direct match: 'X'", "Direct match: '+'", "Direct match: '-' (top row)", "Direct match: '_'",
    "Direct match: '=' (sandwich)", "Direct match: '=' (top-heavy)", "Direct match: '=' (bottom-heavy)",
    "Direct match: '|' (left)", "Direct match: '|' (right)", "Direct match: 'L'", "Direct match: 'J'",
    "Direct match: 'r'", "Direct match: 'T'", "Direct match: '7'", "Direct match: '['", "Direct match: '[' alt",
    "Direct match: ']'", "Direct match: ']' alt", "Direct match: 'O'", "Direct match: 'F'", "Direct match: '`' (TL)",
    "Direct match: '`' (TC)", "Direct match: '`' (TR)", "Direct match: '.' (ML)", "Direct match: '.' (MR)",
    "Direct match: '.' (BL)", "Direct match: '.' (BC)", "Direct match: '.' (BR)", "Direct match: ':' (T-L)",
    "Direct match: ':' (B-L)", "Direct match: ':' (T-C)", "Direct match: ':' (B-C)", "Direct match: ':' (T-R)",
    "Direct match: ':' (B-R)", "Non-direct match: L-corner", "Non-direct match: Bottom-left block",
    "Non-direct match: T top-heavy", "Non-direct match: Top-right box", "Non-direct match: double-vertical",
    "Non-direct match: top & bottom vertical dots",
    "Non-direct match: Grayscale - Diagonal /",
    "Non-direct match: Grayscale - Diagonal \\",
    "Non-direct match: Grayscale - Vertical-ish",
    "Non-direct match: Grayscale - Horizontal-ish",
    "Non-direct match: Grayscale - X-like",
    "Non-direct match: Grayscale - Plus-like",
    "Non-direct match: Grayscale - Blurry noise",

  };

  int total = sizeof(test_cases) / sizeof(test_cases[0]);
  for (i = 0; i < total; i++) {
      run_test_case(labels[i], test_cases[i][0]);
  }

  return 0;
}
